/*
   ---------------------------------------------------------------------------
   |        Q U A T E R N I O N   F R A C T A L S   D E M O   ( OCL )        |
   ---------------------------------------------------------------------------
                              
   Copyright (c) 2009 - 2010 Denis Bogolepov ( denisbogol @ gmail.com )

   This program is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation, either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License along
   with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/////////////////////////////////////////////////////////////////////////////////////////
// Specific data types

typedef struct __SRay
{
    float4 Origin;
    
    float4 Direction;
}
SRay, *PSRay;

/////////////////////////////////////////////////////////////////////////////////////////
// Bounding volume selection

/* Use these macros for switching between bounding box and sphere */

#define USE_BOX_

#define USE_SPHERE

/////////////////////////////////////////////////////////////////////////////////////////
// Program constants

/* Some useful vector constants */

#define ZERO ( float4 ) ( 0.0F, 0.0F, 0.0F, 0.0F )

#define UNIT ( float4 ) ( 1.0F, 1.0F, 1.0F, 0.0F )

#define AXIS_X ( float4 ) ( 1.0F, 0.0F, 0.0F, 0.0F )

#define AXIS_Y ( float4 ) ( 0.0F, 1.0F, 0.0F, 0.0F )

#define AXIS_Z ( float4 ) ( 0.0F, 0.0F, 1.0F, 0.0F )

/////////////////////////////////////////////////////////////////////////////////////////
// Quaternion math functions

float4 QuaternionMult ( float4 left, float4 right )
{
    return ( float4 ) ( left.x * right.x - left.y * right.y -
                        left.z * right.z - left.w * right.w,
                        left.x * right.y + left.y * right.x +
                        left.z * right.w - left.w * right.z,
	                    left.x * right.z + left.z * right.x +
                        left.w * right.y - left.y * right.w,
	                    left.x * right.w + left.w * right.x +
                        left.y * right.z - left.z * right.y );
}

float4 QuaternionSquare ( float4 quat )
{
    return ( float4 ) ( quat.x * quat.x -
                        quat.y * quat.y -
                        quat.z * quat.z -
                        quat.w * quat.w,
                        2.0F * quat.x * quat.y,
	                    2.0F * quat.x * quat.z,
	                    2.0F * quat.x * quat.w );
}

/////////////////////////////////////////////////////////////////////////////////////////
// Functions for compute ray-object intersection

/*
 * Intersects ray with sphere in coordinates origin.
 *
 * Function implements simple algorithm for ray-sphere
 * intersection based on equating of the equations of
 * ray and sphere. For details see:
 *
 * Ray Sphere Intersection ( CGS Wiki )
 * http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection
 */

bool IntersectSphere ( const PSRay ray   /* ray origin and direction */,
                       float sqrradius   /* square of sphere radius */,
                       float * start     /* time of 1st intersection */,
                       float * final     /* time of 2nd intersection */ )
{
    float A = dot ( ray->Direction, ray->Direction );

    float B = dot ( ray->Direction, ray->Origin );

    float C = dot ( ray->Origin, ray->Origin ) - sqrradius;

    float D = B * B - A * C;
    
    if ( D > 0.0F )
    {
        D = sqrt ( D );

        *start = max ( 0.0F, ( -B - D ) / A );

        *final = ( -B + D ) / A;

        return *final > 0.0F;
    }

    return false;
}

//---------------------------------------------------------------------------------------

/*
 * OpenCL version of function from project of Keenan Crane
 * "Ray Tracing Quaternion Julia Sets on the GPU"
 * http://www.devmaster.net/forums/showthread.php?t=4448
 *
 * Iterates the quaternion for the purposes of intersection.
 * This function also produces an estimate of the derivative
 * at specified quaternion, which is required for the distance
 * estimate.
 *
 * To estimate membership in the set, we recursively evaluate
 * 
 * q = q * q + c
 * 
 * until q has a magnitude greater than the threshold value
 * ( it probably diverges ) or we've reached the maximum number
 * of allowable iterations ( it probably converges ). More
 * iterations reveal greater detail in the set.
 * 
 * To estimate the derivative at q, we recursively evaluate
 * 
 * q' = 2 * q * q'
 * 
 * concurrently with the evaluation of q.
 */

/* If magnitude exceed this threshold series are considered divergent */

#define ESCAPE_THRESHOLD 10.0F

/*
 * To optimize execution of OpenCL code we do partial loop
 * unrolling. From ATI Stream Computing Programming Guide:
 *
 * "The ATI Stream OpenCL compiler performs simple loop
 * unrolling optimizations; however, for more complex loop
 * unrolling, it may be beneficial to do this manually".
 */

void Iterate ( float4 * quaternion    /* point to estimate membership in the set */,
               float4 * derivative    /* derivative at this point */,
               const float4 center    /* fractal center */,
               const int iterations   /* maximum number of iterations */ )
{
	for ( int i = 0; i < iterations; i += 2 )
	{
		*derivative = 2.0F * QuaternionMult ( *quaternion, *derivative );
	
		*quaternion = QuaternionSquare ( *quaternion ) + center;
		
		/*
         * NOTE: we don't check divergence here!
         *
         * In practice it is more efficient to do more calculations
         * and to reduce number of conditions.
         */

        //---------------------------------------------------------------------

		*derivative = 2.0F * QuaternionMult ( *quaternion, *derivative );
	
		*quaternion = QuaternionSquare ( *quaternion ) + center;
		
		if ( dot ( *quaternion, *quaternion ) > ESCAPE_THRESHOLD )
		{
			break;
		}
	}
}

//---------------------------------------------------------------------------------------

/*
 * OpenCL version of function from project of Keenan Crane
 * "Ray Tracing Quaternion Julia Sets on the GPU"
 * http://www.devmaster.net/forums/showthread.php?t=4448
 *
 * Finds the intersection of a ray with the quaternion Julia set.
 * The intersection is found using iterative sphere tracing, which
 * takes a conservative step along the ray at each iteration by
 * estimating the minimum distance between the current ray origin
 * and the closest point in the Julia set.
 */
 
/* Square radius of a bounding sphere for the set */
 
#define SQUARE_RADIUS 3.0F

float Intersect ( PSRay ray        /* ray origin and direction */,
                  float4 center    /* fractal center */,
                  int iterations   /* maximum number of iterations */,
                  float epsilon    /* accuracy of hit point calculation */ )
{
	float distance;
	
	while ( true )
	{
		float4 quaternion = ray->Origin;
		
		float4 derivative = AXIS_X;

		Iterate ( &quaternion, &derivative, center, iterations );

		float norm = fast_length ( quaternion );
		
		distance = 0.5F * norm * native_log ( norm ) / fast_length ( derivative );
		
		ray->Origin = mad ( ray->Direction, ( float4 ) ( distance ), ray->Origin );
		
		if ( distance < epsilon || dot ( ray->Origin, ray->Origin ) > SQUARE_RADIUS )
		{
			break;
		}
	}
	
	return distance;
}

//---------------------------------------------------------------------------------------

/*
 * OpenCL version of function from project of Keenan Crane
 * "Ray Tracing Quaternion Julia Sets on the GPU"
 * http://www.devmaster.net/forums/showthread.php?t=4448
 *
 * Create a shading normal for the current point. We use an
 * approximate normal of the isosurface of the potential function,
 * though there are other ways to generate a normal.
 */
 
/* Delta is used in the finite difference approximation of the gradient */
 
#define DELTA 1e-4F

/* Number of iterations for normal estimation */

#define NORMAL_ITERATIONS 10

float4 NormEstimate ( const float4 point    /* point in the set */,
                      const float4 center   /* fractal center */ )
{   
   float4 xLeft  = point - ( float4 ) ( DELTA, 0.0F, 0.0F, 0.0F );
   float4 xRight = point + ( float4 ) ( DELTA, 0.0F, 0.0F, 0.0F );
   
   float4 yLeft  = point - ( float4 ) ( 0.0F, DELTA, 0.0F, 0.0F );
   float4 yRight = point + ( float4 ) ( 0.0F, DELTA, 0.0F, 0.0F );
   
   float4 zLeft  = point - ( float4 ) ( 0.0F, 0.0F, DELTA, 0.0F );
   float4 zRight = point + ( float4 ) ( 0.0F, 0.0F, DELTA, 0.0F );

   for ( int i = 0; i < NORMAL_ITERATIONS; i++ )
   {
      xLeft  = QuaternionSquare ( xLeft )  + center;
      xRight = QuaternionSquare ( xRight ) + center;
      
      yLeft  = QuaternionSquare ( yLeft )  + center;
      yRight = QuaternionSquare ( yRight ) + center;
      
      zLeft  = QuaternionSquare ( zLeft )  + center;
      zRight = QuaternionSquare ( zRight ) + center;
   }

   float4 gradient = ( float4 ) (
       fast_length ( xRight ) - fast_length ( xLeft ),
       fast_length ( yRight ) - fast_length ( yLeft ),
       fast_length ( zRight ) - fast_length ( zLeft ),
       0.0F );
                        
   return normalize ( gradient );
}

/////////////////////////////////////////////////////////////////////////////////////////
// Support functions

/*
 * Generates primary ray for a current fragment.
 *
 * You can change camera state by setting function argument.
 * It can be used for stereo rendering ( move camera left and
 * right for each fragment and tracing two primary rays ) or
 * anti-aliasing.
 */

void GenerateRay ( PSRay ray,
                   const float4 view,
                   const float4 up,
                   const float4 side,
                   const float4 position,
                   const float2 scale,
                   const int width,
                   const int height )
{
    float2 screen = ( float2 ) (
        get_global_id ( 0 ) / ( float ) width  - 0.5F,
        get_global_id ( 1 ) / ( float ) height - 0.5F ) * scale;
    
    float4 direction = view - side * screen.x + up * screen.y;
   
    ray->Origin = position;

    ray->Direction = normalize ( direction );
}

//---------------------------------------------------------------------------------------

/*
 * For the incident vector and surface normal, returns the
 * reflection direction.
 */

float4 Reflect ( float4 incident, float4 normal )
{
    return incident - normal * 2.0F * dot ( incident, normal );
}

//---------------------------------------------------------------------------------------

/* Phong material of surface */

#define AMBIENT 0.1F

#define DIFFUSE 1.0F

#define SPECULAR 1.0F

#define SHININESS 64.0F

/* Computes lighting in an intersection point and its final color */

float4 Phong ( float4 source,   /* position of light the source */
               float4 camera,   /* position of the camera */
               float4 point     /* intersection point with surface */,
               float4 normal    /* surface normal in this point */,
               float4 color     /* diffuse color in this point */ )
{
    float4 light = normalize ( source - point );
    
    float4 view = normalize ( camera - point );
    
    float4 refl = Reflect ( -view, normal );
    
    //--------------------------------------------------------------------
   
    float diffuse = max ( dot ( light, normal ), 0.0F );

    float specular = pow ( max ( dot ( refl, light ), 0.0F ), SHININESS );
    
    //--------------------------------------------------------------------
    
    return AMBIENT * UNIT +
           DIFFUSE * diffuse * color +
           SPECULAR * specular * UNIT;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Core ray tracing function

float4 Raytrace ( PSRay ray               /* ray origin and direction */,
                  const float4 position   /* camera position */,     
                  const float4 center     /* fractal center */,
                  const int iterations    /* maximum number of iterations */,
                  const float epsilon     /* accuracy of hit point calculation */ )
{
    float4 result = ZERO;
    
    float time, final;
    
    if ( IntersectSphere ( ray, SQUARE_RADIUS, &time, &final ) )
    {
		ray->Origin = mad ( ray->Direction, ( float4 ) ( time ), ray->Origin );
			
		time = Intersect ( ray, center, iterations, epsilon );
			
		if ( time < epsilon )
		{
			float4 normal = NormEstimate ( ray->Origin, center );
				
			result = Phong ( position,
                             position,
                             ray->Origin,
                             normal,
                             0.8F * fabs ( ray->Origin ) + 0.3F * fabs ( normal ) );
		}
    }
    
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Ray tracing kernel function

__kernel void Main ( write_only image2d_t texture,
                     const float4 view,
                     const float4 up,
                     const float4 side,
                     const float4 position,
                     const float2 scale,
                     const int width,
                     const int height,
                     const float4 center     /* fractal center */,
                     const int iterations    /* maximum number of iterations */,
                     const float epsilon     /* accuracy of hit point calculation */ )
{
    private SRay ray;
    
    GenerateRay ( &ray,
                  view,
                  up,
                  side,
                  position,
                  scale,
                  width,
                  height );

    float4 result = Raytrace ( &ray,
                               position,
                               center,
                               iterations,
                               epsilon );

    write_imagef ( texture,
                   ( int2 ) ( get_global_id ( 0 ), get_global_id ( 1 ) ),
                   result );
}