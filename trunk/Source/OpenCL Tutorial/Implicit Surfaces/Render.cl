/*
   ---------------------------------------------------------------------------
   |          I M P L I C I T   S U R F A C E S   D E M O   ( OCL )          |
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

///////////////////////////////////////////////////////////////////////////////
// Specific data types

typedef struct __SRay
{
    float4 Origin;
    
    float4 Direction;
}
SRay, *PSRay;

///////////////////////////////////////////////////////////////////////////////
// Bounding volume selection

/* Use these macros for switching between bounding box and sphere */

#define USE_BOX_

#define USE_SPHERE

///////////////////////////////////////////////////////////////////////////////
// Program constants

/* Radius of bounding sphere ( if we use sphere ) */

#define SPHERE_RADIUS 5.5F

//-----------------------------------------------------------------------------

/* Minimum and maximum point of bounding box ( if we use box ) */

#define BOX_MINIMUM ( float4 ) ( -6.28F )  

#define BOX_MAXIMUM ( float4 ) (  6.28F )

//-----------------------------------------------------------------------------

/* Some useful vector constants */

#define ZERO ( float4 ) ( 0.0F, 0.0F, 0.0F, 0.0F )

#define UNIT ( float4 ) ( 1.0F, 1.0F, 1.0F, 0.0F )

#define AXIS_X ( float4 ) ( 1.0F, 0.0F, 0.0F, 0.0F )

#define AXIS_Y ( float4 ) ( 0.0F, 1.0F, 0.0F, 0.0F )

#define AXIS_Z ( float4 ) ( 0.0F, 0.0F, 1.0F, 0.0F )

///////////////////////////////////////////////////////////////////////////////
// Compute implicit function value and normal in specific point

#define T 1.6180339887F

/* We use these macros to simplify function equation */

#define X point.x
#define Y point.y
#define Z point.z

float CalcFunction ( float4 point )
{
    /* You can enter here any expression in the form F ( x, y, z ) */

    return 2.0F - native_cos ( X + T * Y ) - native_cos ( X - T * Y ) -
                  native_cos ( Y + T * Z ) - native_cos ( Y - T * Z ) -
                  native_cos ( Z - T * X ) - native_cos ( Z + T * X );
                  
    // Also try this: 
    // return native_cos ( X ) + native_cos ( Y ) + native_cos ( Z );
}

#undef X
#undef Y
#undef Z

//-----------------------------------------------------------------------------

#define STEP 0.01F /* Step for numerical estimation of a gradient */

float4 CalcNormal ( float4 point )
{
    /* We calculate normal by numerical estimation of a gradient */
    
    float A = CalcFunction ( point + AXIS_X * STEP ) - 
              CalcFunction ( point - AXIS_X * STEP );

    float B = CalcFunction ( point + AXIS_Y * STEP ) -
              CalcFunction ( point - AXIS_Y * STEP );

    float C = CalcFunction ( point + AXIS_Z * STEP ) -
              CalcFunction ( point - AXIS_Z * STEP );

    return fast_normalize ( ( float4 ) ( A, B, C, 0.0F ) );
}

///////////////////////////////////////////////////////////////////////////////
// Functions for compute ray-object intersection

/*
 * Intersects ray with box ( general case ).
 *
 * Function implements fastest algorithm for ray-box
 * intersection based on slab test. For details see:
 *
 * T. Kay, J. Kajiya. "Ray tracing complex scenes".
 * Computer Graphics, 20 (4), 269 – 278, 1986.
 */

bool IntersectBox ( const PSRay ray   /* ray origin and direction */,
                    float4 minimum    /* minimum point of a box */,
                    float4 maximum    /* maximum point of a box */,
                    float * start     /* time of 1st intersection */,
                    float * final     /* time of 2nd intersection */ )
{
    float4 OMIN = ( minimum - ray->Origin ) / ray->Direction;
    
    float4 OMAX = ( maximum - ray->Origin ) / ray->Direction;
    
    float4 MAX = max ( OMAX, OMIN );
    
    float4 MIN = min ( OMAX, OMIN );
    
    *final = min ( MAX.x, min ( MAX.y, MAX.z ) );
    
    *start = max ( max ( MIN.x, 0.0F ), max ( MIN.y, MIN.z ) );    
    
    return *final > *start;
}

//-----------------------------------------------------------------------------

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
                       float radius      /* sphere radius */,
                       float * start     /* time of 1st intersection */,
                       float * final     /* time of 2nd intersection */ )
{
    float A = dot ( ray->Direction, ray->Direction );

    float B = dot ( ray->Direction, ray->Origin );

    float C = dot ( ray->Origin, ray->Origin ) - radius * radius;

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

//-----------------------------------------------------------------------------

#define FULL_INTERVALS 200

#define HALF_INTERVALS 100

/*
 * Intersects ray with implicit surface.
 *
 * Function implements simple and computational expensive
 * algorithm. We divide a ray interval into small segments
 * and check each of them. The segment contains a root if
 * function changes a sign on its end points. For root
 * approximation linear interpolation is used. For more
 * advanced methods see:
 *
 * John C. Hart. "Ray Tracing Implicit Surfaces".
 * http://graphics.cs.uiuc.edu/~jch/papers/rtis-tr.pdf
 *
 * Jag Mohan Singh and P J Narayanan. "Real-Time Ray-Tracing
 * of Implicit Surfaces on the GPU". Technical Report:
 * IIIT/TR/2007/72
 */

/*
 * To optimize execution of OpenCL code we do loop unrolling
 * manually. From ATI Stream Computing Programming Guide:
 *
 * "The ATI Stream OpenCL compiler performs simple loop
 * unrolling optimizations; however, for more complex loop
 * unrolling, it may be beneficial to do this manually".
 */

bool IntersectSurface ( const PSRay ray   /* ray origin and direction */,
                        float start       /* time when a ray enters a box */,
                        float final       /* time when a ray leaves a box */,
                        float * val       /* time of ray-surface hit */ )
{
    float step = ( final - start ) / ( float ) FULL_INTERVALS;

    //----------------------------------------------------------

    float time = start;

    float4 point = mad ( ( float4 ) ( time ),
                         ray->Direction,
                         ray->Origin );

    //----------------------------------------------------------
    
    float right, left = CalcFunction ( point );

    //----------------------------------------------------------
    
    for ( int i = 0; i < HALF_INTERVALS; ++i )
    {
        time += step;

        point = mad ( ( float4 ) ( step ),
                      ray->Direction,
                      point );

        right = CalcFunction ( point );
        
        if ( left * right < 0.0F )
        {
            *val = mad ( right,
                         step / ( left - right ),
                         time );

            return true;
        }
        
        left = right;

        //--------------------------------------------
        
        time += step;

        point = mad ( ( float4 ) ( step ),
                      ray->Direction,
                      point );

        right = CalcFunction ( point );
        
        if ( left * right < 0.0F )
        {
            *val = mad ( right,
                         step / ( left - right ),
                         time );

            return true;
        }
        
        left = right;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////
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

//-----------------------------------------------------------------------------

float4 Reflect ( float4 incident, float4 normal )
{
    return incident - normal * 2.0F * dot ( incident, normal );
}

//-----------------------------------------------------------------------------

/* Phong material of surface */

#define AMBIENT 0.1F

#define DIFFUSE 1.0F

#define SPECULAR 1.0F

#define SHININESS 64.0F

/* Small value for moving origin of shadow ray ( see code ) */

#define EPSILON 0.01F

/* Use these macro for enable / disable shadows */

#define SHADOWS

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
    
#ifdef SHADOWS
    
    SRay ray;
    
    ray.Origin = point + light * EPSILON;
    
    ray.Direction = light;
    
    float start, final, time;

#if defined ( USE_BOX )

    if ( IntersectBox ( &ray, BOX_MINIMUM, BOX_MAXIMUM, &start, &final ) )

#elif defined ( USE_SPHERE )

    if ( IntersectSphere ( &ray, SPHERE_RADIUS, &start, &final ) )

#endif
    
    if ( IntersectSurface ( &ray, start, final, &time ) )
    {
        diffuse *= 0.25F;
    }     
    
#endif 
    
    return AMBIENT * UNIT +
           DIFFUSE * diffuse * color +
           SPECULAR * specular * UNIT;
}

///////////////////////////////////////////////////////////////////////////////
// Core ray tracing function

float4 Raytrace ( const PSRay ray,
                  const float4 position,
                  const float4 source )
{
    float4 result = ZERO;
    
    float start, final, time;

#if defined ( USE_BOX )

    if ( IntersectBox ( ray, BOX_MINIMUM, BOX_MAXIMUM, &start, &final ) )

#elif defined ( USE_SPHERE )

    if ( IntersectSphere ( ray, SPHERE_RADIUS, &start, &final ) )

#endif

    {
        if ( IntersectSurface ( ray, start, final, &time ) )
        {
            float4 point = ray->Origin + ray->Direction * time;
                    
            float4 normal = CalcNormal ( point );

            float4 color = ( point - BOX_MINIMUM ) /
                ( BOX_MAXIMUM - BOX_MINIMUM );

            result = Phong ( source, position, point, normal, color );
        }
    }
    
    return result;
}

///////////////////////////////////////////////////////////////////////////////
// Ray tracing kernel function

__kernel void Main ( write_only image2d_t texture,
                     const float4 view,
                     const float4 up,
                     const float4 side,
                     const float4 position,
                     const float2 scale,
                     const int width,
                     const int height,
                     const float4 source )
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
                               source );

    write_imagef ( texture,
                   ( int2 ) ( get_global_id ( 0 ), get_global_id ( 1 ) ),
                   result );
}