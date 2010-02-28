/**
 *** @file fastmv.cpp
 ***
 *** @date 2010
 ***
 *** @version 1.0
 ***
 *** @autor Andrey Morozov
 ***
 *** @email morozov.andrey.vmk@gmail.com
 ***
 **/

#include "fastmv.hpp"

namespace fastmath
{

vec4 operator * ( mat4 & m, vec4 & v)
{
	vec4 ret;

	#ifndef use_sse2
		for (std::size_t i = 0; i<v.vec_size(); ++i)
		{
			ret[i] = dot<4, float, float>(&(m[i*4]), &(v[0]));
		}
	#else
		ret.set( 
					dot<1,__m128, float>( m.get_line(0), v.get_line () ),
					dot<1,__m128, float>( m.get_line(1), v.get_line () ),
					dot<1,__m128, float>( m.get_line(2), v.get_line () ),
					dot<1,__m128, float>( m.get_line(3), v.get_line () )
				);
	#endif

	return ret;
}

mat4 RotateX(float angle)
{
	mat4 a;
	a.one();

	float _cos = cos(angle);
	float _sin = sin(angle);

	a(1, 1) = _cos;
	a(1, 2) = _sin;
	a(2, 1) = -_sin;
	a(2, 2) = _cos;

	return a;
}

mat4 RotateY(float angle)
{
	mat4 a;
	a.one();

	float _cos = cos(angle);
	float _sin = sin(angle);

	a(0, 0) = _cos;
	a(0, 2) = -_sin;
	a(2, 0) = _sin;
	a(2, 2) = _cos;

	return a;
}

mat4 RotateZ(float angle)
{
	mat4 a;
	a.one();

	float _cos = cos(angle);
	float _sin = sin(angle);

	a(0, 0) = _cos;
	a(0, 1) = _sin;
	a(1, 0) = -_sin;
	a(1, 1) = _cos;

	return a;
}

}

