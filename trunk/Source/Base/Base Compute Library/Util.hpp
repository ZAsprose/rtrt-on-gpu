/*
   -----------------------------------------------------------------------------
   |                  B A S E   C O M P U T E   L I B R A R Y                  |
   -----------------------------------------------------------------------------
                              
   Copyright (c) 2009 - 2010 Denis Bogolepov ( denisbogol @ gmail.com )

   This library is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free Software
   Foundation, either version 3 of the License, or (at your option) any later
   version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with
   this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _OPENCL_SUPPORT_
#define _OPENCL_SUPPORT_

#include <CL/cl.h>

#include <Eigen.h>

/*
 * Use this macro to set inline method for your compiler.
 */

#ifdef _MSC_VER

    #define INLINE __forceinline   /* VC++ specific */

#else

    #define INLINE inline          /* standard inline */

#endif

/***************************************************************************************/

INLINE cl_float2 cltFloat2 ( cl_float x = 0.0F,
                             cl_float y = 0.0F )
{
    cl_float2 result;

    result.s [0] = x;

    result.s [1] = y;

    return result;
}

INLINE cl_float2 cltFloat2 ( const Vector2f & vector )
{
    cl_float2 result;

    result.s [0] = vector.x ( );

    result.s [1] = vector.y ( );

    return result;
}

/***************************************************************************************/

INLINE cl_float4 cltFloat4 ( cl_float x = 0.0F,
                             cl_float y = 0.0F,
                             cl_float z = 0.0F,
                             cl_float w = 0.0F )
{
    cl_float4 result;

    result.s [0] = x;

    result.s [1] = y;

    result.s [2] = z;

    result.s [3] = w;

    return result;
}

INLINE cl_float4 cltFloat4 ( const Vector2f & vector,
                             cl_float z = 0.0F,
                             cl_float w = 0.0F )
{
    cl_float4 result;

    result.s [0] = vector.x ( );

    result.s [1] = vector.y ( );

    result.s [2] = z;

    result.s [3] = w;

    return result;
}

INLINE cl_float4 cltFloat4 ( const Vector3f & vector,
                             cl_float w = 0.0F )
{
    cl_float4 result;

    result.s [0] = vector.x ( );

    result.s [1] = vector.y ( );

    result.s [2] = vector.z ( );

    result.s [3] = w;

    return result;
}

INLINE cl_float4 cltFloat4 ( const Vector4f & vector )
{
    cl_float4 result;

    result.s [0] = vector.x ( );

    result.s [1] = vector.y ( );

    result.s [2] = vector.z ( );

    result.s [3] = vector.w ( );

    return result;
}

/***************************************************************************************/

INLINE cl_int2 cltInt2 ( cl_int x = 0,
                         cl_int y = 0 )
{
    cl_int2 result;

    result.s [0] = x;

    result.s [1] = y;

    return result;
}

INLINE cl_int2 cltInt2 ( const Vector2i & vector )
{
    cl_int2 result;

    result.s [0] = vector.x ( );

    result.s [1] = vector.y ( );

    return result;
}

/***************************************************************************************/

INLINE cl_int4 cltInt4 ( cl_int x = 0,
                         cl_int y = 0,
                         cl_int z = 0,
                         cl_int w = 0 )
{
    cl_int4 result;

    result.s [0] = x;

    result.s [1] = y;

    result.s [2] = z;

    result.s [3] = w;

    return result;
}

INLINE cl_int4 cltInt4 ( const Vector2i & vector,
                         cl_int z = 0,
                         cl_int w = 0 )
{
    cl_int4 result;

    result.s [0] = vector.x ( );

    result.s [1] = vector.y ( );

    result.s [2] = z;

    result.s [3] = w;

    return result;
}

INLINE cl_int4 cltInt4 ( const Vector3i & vector,
                         cl_int w = 0 )
{
    cl_int4 result;

    result.s [0] = vector.x ( );

    result.s [1] = vector.y ( );

    result.s [2] = vector.z ( );

    result.s [3] = w;

    return result;
}

INLINE cl_int4 cltInt4 ( const Vector4i & vector )
{
    cl_int4 result;

    result.s [0] = vector.x ( );

    result.s [1] = vector.y ( );

    result.s [2] = vector.z ( );

    result.s [3] = vector.w ( );

    return result;
}

#endif