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
				ret[i] = dot<4, mat4::type, vec4::type>(&(m[i*4]), &(v[0]));
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

	mat4 MirrorX (void)
	{
		mat4 m;
		m.one ();
		m(0,0) = -1.0f;
		return m;
	}

	mat4 MirrorY (void)
	{
		mat4 m;
		m.one ();
		m(1,1) = -1.0f;
		return m;
	}

	mat4 MirrorZ (void)
	{
		mat4 m;
		m.one ();
		m(2,2) = -1.0f;
		return m;
	}

	mat4 Rotate( vec4 & v )
	{
		mat4 mz, my, mx;
		mz = RotateZ (v.z());
		my = RotateY (v.y());
		mx = RotateX (v.x());
		mz = mz * my;
		mz = mz * mx;
		return mz;
	}

	mat4 Rotate ( float angle, vec4 & direction)
	{
		mat4 m;
		m.one ();

		float _cos = cos(angle);
		float _sin = sin(angle);

		m(0,0) = direction.x() * direction.x() + ( 1.0f - direction.x() * direction.x()) * _cos;
        m(0,1) = direction.x() * direction.y() * ( 1.0f - _cos ) + direction.z() * _sin;
		m(0,2) = direction.x() * direction.z() * ( 1.0f - _cos ) - direction.y() * _sin;
        m(1,0) = direction.x() * direction.y() * ( 1.0f - _cos ) - direction.z() * _sin;
        m(1,1) = direction.y() * direction.y() + ( 1.0f - direction.y() * direction.y() ) * _cos;
        m(1,2) = direction.y() * direction.z() * ( 1.0f - _cos ) + direction.x() * _sin;
        m(2,0) = direction.x() * direction.z() * ( 1.0f - _cos ) + direction.y() * _sin;
        m(2,1) = direction.y() * direction.z() * ( 1.0f - _cos ) - direction.x() * _sin;
        m(2,2) = direction.z() * direction.z() + ( 1.0f - direction.z() * direction.z() ) * _cos;

		return m;
	}

	mat4 ScaleX ( float scale )
	{
		mat4 m;
		m.one ();
		m(0,0) = scale;
		return m;
	}

	mat4 ScaleY ( float scale )
	{
		mat4 m;
		m.one ();
		m(1,1) = scale;
		return m;
	}

	mat4 ScaleZ ( float scale )
	{
		mat4 m;
		m.one ();
		m(2,2) = scale;
		return m;
	}

	mat4 Scale ( vec4 & v )
	{
		mat4 m;
		m.one ();
		m(0,0) = v.x();
		m(1,1) = v.y();
		m(2,2) = v.z();
		return m;
	}


}

