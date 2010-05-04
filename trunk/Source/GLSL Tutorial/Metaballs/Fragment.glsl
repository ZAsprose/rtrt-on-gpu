/*
   ---------------------------------------------------------------------------
   |                       M E T A B A L L S   D E M O                       |
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
// Shader interface

/* Camera position and orientation */

uniform SCamera Camera;

//-----------------------------------------------------------------------------

/* Positions ( XYZ components ) and radiuses ( W component ) of metaballs */

uniform vec4 Metaballs [8];

//-----------------------------------------------------------------------------

/* Textures for environment mapping */

uniform sampler2D LeftTexture;

uniform sampler2D RightTexture;

uniform sampler2D FrontTexture;

uniform sampler2D BackTexture;

uniform sampler2D TopTexture;

uniform sampler2D DownTexture;

///////////////////////////////////////////////////////////////////////////////
// Shader constants

/* Minimum and maximum point of bounding box ( the scene is in this box ) */

const vec3 BoxMinimum = vec3 ( -5.0, -5.0, -5.0 );    

const vec3 BoxMaximum = vec3 (  5.0,  5.0,  5.0 );

//-----------------------------------------------------------------------------

/* Some useful vector constants */

const vec3 Zero  = vec3 ( 0.0, 0.0, 0.0 );

const vec3 Unit  = vec3 ( 1.0, 1.0, 1.0 );

const vec3 AxisX = vec3 ( 1.0, 0.0, 0.0 );

const vec3 AxisY = vec3 ( 0.0, 1.0, 0.0 );

const vec3 AxisZ = vec3 ( 0.0, 0.0, 1.0 );

///////////////////////////////////////////////////////////////////////////////
// Compute implicit function value and normal in specific point

#define THRESHOLD 2.5 /* You can modify this value for different effects */

/*
 * For some info on metaballs see:
 *
 * Metaballs ( From Wikipedia, the free encyclopedia )
 * http://en.wikipedia.org/wiki/Metaballs
 *
 * Metaballs ( also known as blobs ) by Ryan Geiss
 * http://www.geisswerks.com/ryan/BLOBS/blobs.html
 *
 * Metaballs ( based on "Marching Cubes" algorithm )
 * http://www.paulsprojects.net/opengl/metaballs/metaballs.html
 *
 * In our implementation we use ray tracing algorithm for
 * metaballs rendering.
 */

float CalcFunction ( vec3 point )
{
    float result = 0.0;
    
    for ( int i = 0; i < 8; ++i )
    {
        vec3 delta = point - Metaballs [i].xyz;
        
        result += Metaballs [i].w /
              ( delta.x * delta.x + delta.y * delta.y + delta.z * delta.z );
    }
    
    return THRESHOLD - result;
}

//-----------------------------------------------------------------------------

#define STEP 0.01 /* Step for numerical estimation of a gradient */

vec3 CalcNormal ( vec3 point )
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

#define INTERVALS 80

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

/* Use this macro to enable / disable environment mapping */

#define ENVIRONMENT_MAPPING

/* Small value for checking hitted face of bounding box ( see code ) */

#define EPSILON 0.01

/* Phong material of metaballs */

#define AMBIENT 0.0

#define DIFFUSE 0.0   /* if no reflection set it > 0 */

#define SPECULAR 1.0

#define SHININESS 64.0

/* Computes lighting in an intersection point and its final color */

vec3 Phong ( in vec3 point    /* intersection point with surface */,
             in vec3 normal   /* normal to the surface in this point */,
             in vec3 color    /* diffuse color in this point */ )
{
    vec3 light = normalize ( Camera.Position - point );
    
    vec3 view = normalize ( Camera.Position - point );
    
    vec3 refl = reflect ( -view, normal );
    
    //--------------------------------------------------------------------
   
    float diffuse = max ( dot ( light, normal ), 0.0 );

    float specular = pow ( max ( dot ( refl, light ), 0.0 ), SHININESS );
    
    vec3 result = AMBIENT * Unit +
                  DIFFUSE * diffuse * color +
                  SPECULAR * specular * Unit;
                  
    //--------------------------------------------------------------------
    
#ifdef ENVIRONMENT_MAPPING
    
    SRay ray = SRay ( point, refl );
    
    float final = IntersectBox ( ray, BoxMinimum, BoxMaximum );
    
    point = ray.Origin + ray.Direction * final;
    
    //--------------------------------------------------------------------
    
    if ( abs ( point.y - BoxMinimum.y ) < EPSILON )
    {
        result += vec3 ( texture2D ( DownTexture,
            ( BoxMaximum.xz - point.xz ) /
            ( BoxMaximum.xz - BoxMinimum.xz ) ) );
    }
    
    if ( abs ( point.y - BoxMaximum.y ) < EPSILON )
    {
        result += vec3 ( texture2D ( TopTexture,
            ( point.xz - BoxMinimum.xz ) /
            ( BoxMaximum.xz - BoxMinimum.xz ) ) );
    }
    
    if ( abs ( point.x - BoxMinimum.x ) < EPSILON )
    {
        result += vec3 ( texture2D ( LeftTexture,
            ( BoxMaximum.zy - point.zy ) /
            ( BoxMaximum.zy - BoxMinimum.zy ) ) );
    }
    
    if ( abs ( point.x - BoxMaximum.x ) < EPSILON )
    {
        result += vec3 ( texture2D ( RightTexture,
            ( BoxMaximum.zy - point.zy ) /
            ( BoxMaximum.zy - BoxMinimum.zy ) ) );
    }
    
    if ( abs ( point.z - BoxMinimum.z ) < EPSILON )
    {
        result += vec3 ( texture2D ( BackTexture,
            ( BoxMaximum.xy - point.xy ) /
            ( BoxMaximum.xy - BoxMinimum.xy ) ) );
    }
    
    if ( abs ( point.z - BoxMaximum.z ) < EPSILON )
    {
        result += vec3 ( texture2D ( FrontTexture,
            ( BoxMaximum.xy - point.xy ) / 
            ( BoxMaximum.xy - BoxMinimum.xy ) ) );
    }
    
#endif
    
    return result;
}

///////////////////////////////////////////////////////////////////////////////
// Core ray tracing function

vec3 Raytrace ( in SRay ray )
{
    vec3 result = Zero;
    
    float start, final, time;
    
    if ( IntersectBox ( ray, BoxMinimum, BoxMaximum, start, final ) )
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