/*
   ---------------------------------------------------------------------------
   |                  M E T A B A L L S   D E M O   ( OCL )                  |
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
// Program constants

/* Minimum and maximum point of bounding box ( if we use box ) */

__constant float4 BoxMinimum = ( float4 ) ( -5.0F );    

__constant float4 BoxMaximum = ( float4 ) (  5.0F );        

//-----------------------------------------------------------------------------

/* Some useful vector constants */

__constant float4 Zero  = ( float4 ) ( 0.0F, 0.0F, 0.0F, 0.0F );

__constant float4 Unit  = ( float4 ) ( 1.0F, 1.0F, 1.0F, 0.0F );

__constant float4 AxisX = ( float4 ) ( 1.0F, 0.0F, 0.0F, 0.0F );

__constant float4 AxisY = ( float4 ) ( 0.0F, 1.0F, 0.0F, 0.0F );

__constant float4 AxisZ = ( float4 ) ( 0.0F, 0.0F, 1.0F, 0.0F );

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

float CalcFunction ( __constant float4 * metaballs,
                     float4 point )
{
    float result = 0.0;
    
    for ( int i = 0; i < 8; ++i )
    {
        float4 delta = point - metaballs [i];
        
        result += metaballs [i].w /
              ( delta.x * delta.x + delta.y * delta.y + delta.z * delta.z );
    }
    
    return THRESHOLD - result;
}

//-----------------------------------------------------------------------------

#define STEP 0.01F /* Step for numerical estimation of a gradient */

float4 CalcNormal ( __constant float4 * metaballs,
                    float4 point )
{
    /* We calculate normal by numerical estimation of a gradient */
    
    float A = CalcFunction ( metaballs, point + AxisX * STEP ) - 
              CalcFunction ( metaballs, point - AxisX * STEP );

    float B = CalcFunction ( metaballs, point + AxisY * STEP ) -
              CalcFunction ( metaballs, point - AxisY * STEP );

    float C = CalcFunction ( metaballs, point + AxisZ * STEP ) -
              CalcFunction ( metaballs, point - AxisZ * STEP );

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

bool IntersectSurface (
    __constant float4 * metaballs   /* metaballs positions */,
    const PSRay ray                 /* ray origin and direction */,
    float start                     /* time when a ray enters a box */,
    float final                     /* time when a ray leaves a box */,
    float * val                     /* time of ray-surface hit */ )
{
    float step = ( final - start ) / ( float ) FULL_INTERVALS;

    //----------------------------------------------------------

    float time = start;

    float4 point = mad ( ( float4 ) ( time ),
                         ray->Direction,
                         ray->Origin );

    //----------------------------------------------------------
    
    float right, left = CalcFunction ( metaballs, point );

    //----------------------------------------------------------
    
    for ( int i = 0; i < HALF_INTERVALS; ++i )
    {
        time += step;

        point = mad ( ( float4 ) ( step ),
                      ray->Direction,
                      point );

        right = CalcFunction ( metaballs, point );
        
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

        right = CalcFunction ( metaballs, point );
        
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

/* Phong material of metaballs */

#define AMBIENT 0.1F

#define DIFFUSE 1.0F

#define SPECULAR 1.0F

#define SHININESS 64.0F

/* Small value for moving origin of shadow ray ( see code ) */

#define EPSILON 0.01F

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
    
    return AMBIENT * Unit +
           DIFFUSE * diffuse * color +
           SPECULAR * specular * Unit;
}

///////////////////////////////////////////////////////////////////////////////
// Core ray tracing function

float4 Raytrace (
    __constant float4 * metaballs   /* metaballs positions */,
    const PSRay ray                 /* ray origin and direction */,
    const float4 position           /* camera position */ )
{
    float4 result = Zero;
    
    float start, final, time;
    
    if ( IntersectBox ( ray, BoxMinimum, BoxMaximum, &start, &final ) )
    {
        if ( IntersectSurface ( metaballs, ray, start, final, &time ) )
        {
            float4 point = ray->Origin + ray->Direction * time;
                    
            float4 normal = CalcNormal ( metaballs, point );

            float4 color = ( point - BoxMinimum ) /
                ( BoxMaximum - BoxMinimum );

            result = Phong ( position,
                             position,
                             point,
                             normal,
                             color );
        }
    }
    
    return result;
}

///////////////////////////////////////////////////////////////////////////////
// Ray tracing kernel function

__kernel void Main ( __write_only image2d_t texture,
                     const float4 view,
                     const float4 up,
                     const float4 side,
                     const float4 position,
                     const float2 scale,
                     const int width,
                     const int height,
                     __constant float4 * metaballs )
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

    float4 result = Raytrace ( metaballs,
                               &ray,
                               position );

    write_imagef ( texture,
                   ( int2 ) ( get_global_id ( 0 ), get_global_id ( 1 ) ),
                   result );
}