/*
   ---------------------------------------------------------------------------
   |               I M P L I C I T   S U R F A C E S   D E M O               |
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

struct SCamera
{
	vec3 Position;
	
	vec3 Side;
	
	vec3 Up;
	
	vec3 View;
	
	vec2 Scale;
};

//-----------------------------------------------------------------------------

struct SRay
{
	vec3 Origin;
	
	vec3 Direction;
};

///////////////////////////////////////////////////////////////////////////////
// Shader configuration

/* Use these macros for switching between bounding box and sphere */

#define USE_BOX_

#define USE_SPHERE

///////////////////////////////////////////////////////////////////////////////
// Shader interface

/* Camera position and orientation */

uniform SCamera Camera;

/* Current time ( may be used for morphing ) */

uniform float Time;

/* Current light position */

uniform vec3 LightPosition;

///////////////////////////////////////////////////////////////////////////////
// Shader constants

/* Minimum and maximum point of bounding box ( if we use box ) */

const vec3 BoxMinimum = vec3 ( -5.0, -5.0, -5.0 );    

const vec3 BoxMaximum = vec3 (  5.0,  5.0,  5.0 );

//-----------------------------------------------------------------------------

/* Radius of bounding sphere ( if we use sphere ) */

const float SphereRadius = 5.5;				

//-----------------------------------------------------------------------------

/* Some useful vector constants */

const vec3 Zero  = vec3 ( 0.0, 0.0, 0.0 );

const vec3 Unit  = vec3 ( 1.0, 1.0, 1.0 );

const vec3 AxisX = vec3 ( 1.0, 0.0, 0.0 );

const vec3 AxisY = vec3 ( 0.0, 1.0, 0.0 );

const vec3 AxisZ = vec3 ( 0.0, 0.0, 1.0 );

///////////////////////////////////////////////////////////////////////////////
// Compute implicit function value and normal in specific point

#define T 1.6180339887

/* We use these macros to simplify function equation */

#define X point.x
#define Y point.y
#define Z point.z

float CalcFunction ( in vec3 point )
{
	/* You can enter here any expression in the form F ( x, y, z ) */

	return 2.0 - cos ( X + T * Y ) - cos ( X - T * Y ) - cos ( Y + T * Z ) -
		         cos ( Y - T * Z ) - cos ( Z - T * X ) - cos ( Z + T * X );
	              
	// Also try this: 
	// return sin ( X ) + sin ( Y ) + sin ( Z );
}

#undef X
#undef Y
#undef Z

//-----------------------------------------------------------------------------

#define STEP 0.01 /* Step for numerical estimation of a gradient */

vec3 CalcNormal ( in vec3 point )
{
    /* We calculate normal by numerical estimation of a gradient */
    
    float A = CalcFunction ( point + AxisX * STEP ) - 
              CalcFunction ( point - AxisX * STEP );

    float B = CalcFunction ( point + AxisY * STEP ) -
              CalcFunction ( point - AxisY * STEP );

    float C = CalcFunction ( point + AxisZ * STEP ) -
              CalcFunction ( point - AxisZ * STEP );

    return normalize ( vec3 ( A, B, C ) );
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

bool IntersectBox ( in SRay ray       /* ray origin and direction */,
                    in vec3 minimum   /* minimum point of a box */,
                    in vec3 maximum   /* maximum point of a box */,
                    out float start   /* time of 1st intersection */,
                    out float final   /* time of 2nd intersection */ )
{
    vec3 OMIN = ( minimum - ray.Origin ) / ray.Direction;
    
    vec3 OMAX = ( maximum - ray.Origin ) / ray.Direction;
    
    vec3 MAX = max ( OMAX, OMIN );
    
    vec3 MIN = min ( OMAX, OMIN );
    
    final = min ( MAX.x, min ( MAX.y, MAX.z ) );
    
    start = max ( max ( MIN.x, 0.0 ), max ( MIN.y, MIN.z ) );    
    
    return final > start;
}

//-----------------------------------------------------------------------------

/* Intersects ray with box ( special case: ray origin is in a box ) */

float IntersectBox ( in SRay ray       /* ray origin and direction */,
                     in vec3 minimum   /* minimum point of a box */,
                     in vec3 maximum   /* maximum point of a box */ )
{
    vec3 OMIN = ( minimum - ray.Origin ) / ray.Direction;
   
    vec3 OMAX = ( maximum - ray.Origin ) / ray.Direction;
    
    vec3 MAX = max ( OMAX, OMIN );
    
    return min ( MAX.x, min ( MAX.y, MAX.z ) );
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

bool IntersectSphere ( in SRay ray       /* ray origin and direction */,
                       in float radius   /* sphere radius */,
                       out float start   /* time of 1st intersection */,
                       out float final   /* time of 2nd intersection */ )
{
	float A = dot ( ray.Direction, ray.Direction );

	float B = dot ( ray.Direction, ray.Origin );

	float C = dot ( ray.Origin, ray.Origin ) - radius * radius;

	float D = B * B - A * C;
	
	if ( D > 0.0 )
	{
		D = sqrt ( D );

		start = max ( 0.0, ( -B - D ) / A );

		final = ( -B + D ) / A;

		return final > 0.0;
	}

	return false;
}

//-----------------------------------------------------------------------------

#define INTERVALS 200

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

bool IntersectSurface ( in SRay ray      /* ray origin and direction */,
                        in float start   /* time when a ray enters a box */,
                        in float final   /* time when a ray leaves a box */,
                        out float val    /* time of ray-surface hit */ )
{
    float step = ( final - start ) / float ( INTERVALS );

    //----------------------------------------------------------

    float time = start;

    vec3 point = ray.Origin + time * ray.Direction;

    //----------------------------------------------------------
    
    float right, left = CalcFunction ( point );

    //----------------------------------------------------------

    for ( int i = 0; i < INTERVALS; ++i )
    {
        time += step;

        point += step * ray.Direction;

        right = CalcFunction ( point );
        
        if ( left * right < 0.0 )
        {
            val = time + right * step / ( left - right );

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

SRay GenerateRay ( in SCamera camera /* current camera state */ )
{
    vec2 coords = gl_TexCoord [0].xy * camera.Scale;
    
    vec3 direction = camera.View -
                     camera.Side * coords.x +
                     camera.Up * coords.y;
   
    return SRay ( camera.Position, normalize ( direction ) );
}

//-----------------------------------------------------------------------------

/* Phong material of metaballs */

#define AMBIENT 0.1

#define DIFFUSE 1.0

#define SPECULAR 1.0

#define SHININESS 64.0

/* Small value for moving origin of shadow ray ( see code ) */

#define EPSILON 0.01

/* Use these macro for enable / disable shadows */

#define SHADOWS

/* Computes lighting in an intersection point and its final color */

vec3 Phong ( in vec3 point    /* intersection point with surface */,
             in vec3 normal   /* normal to the surface in this point */,
             in vec3 color    /* diffuse color in this point */ )
{
    vec3 light = normalize ( LightPosition  - point );
    
    vec3 view = normalize ( Camera.Position - point );
    
    vec3 refl = reflect ( -view, normal );
    
    //--------------------------------------------------------------------
   
    float diffuse = max ( dot ( light, normal ), 0.0 );

    float specular = pow ( max ( dot ( refl, light ), 0.0 ), SHININESS );
    
    //--------------------------------------------------------------------
    
#ifdef SHADOWS
    
    SRay ray = SRay ( point + light * EPSILON, light );
    
	float start, final, time;

#if defined ( USE_BOX )

	if ( IntersectBox ( ray, BoxMinimum, BoxMaximum, start, final ) )

#elif defined ( USE_SPHERE )

	if ( IntersectSphere ( ray, SphereRadius, start, final ) )

#endif
    
    if ( IntersectSurface ( ray, start, final, time ) )
    {
        diffuse *= 0.25;
    }     
    
#endif 
    
    return AMBIENT * Unit +
           DIFFUSE * diffuse * color +
           SPECULAR * specular * Unit;
}

///////////////////////////////////////////////////////////////////////////////
// Core ray tracing function

vec3 Raytrace ( in SRay ray )
{
	vec3 result = Zero;
    
	float start, final, time;

#if defined ( USE_BOX )

	if ( IntersectBox ( ray, BoxMinimum, BoxMaximum, start, final ) )

#elif defined ( USE_SPHERE )

	if ( IntersectSphere ( ray, SphereRadius, start, final ) )

#endif

	{
		if ( IntersectSurface ( ray, start, final, time ) )
		{
			vec3 point = ray.Origin + ray.Direction * time;
					
			vec3 normal = CalcNormal ( point );

			vec3 color = ( point - BoxMinimum ) /
			    ( BoxMaximum - BoxMinimum );

			result = Phong ( point, normal, color );
		}
	}
	
	return result;
}

///////////////////////////////////////////////////////////////////////////////
// Shader entry point

void main ( void )
{
    SRay ray = GenerateRay ( Camera );
	
	gl_FragColor = vec4 ( Raytrace ( ray ), 1.0 );
}