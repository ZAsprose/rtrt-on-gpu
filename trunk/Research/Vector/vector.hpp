/*****************************************************************

	vector.hpp

	Этот файл является частью библиотеки rt2

	Автор: Морозов Андрей

	Нижний Новгород, 2008-2009

*****************************************************************/
/**
	\file vector.hpp
	\author Морозов Андрей
 */

#ifndef __vector_hpp__
#define __vector_hpp__

#include <math.h>

#include "config.hpp"

#include <malloc.h>


#ifdef m128_t

	#include <xmmintrin.h>

#endif

#include <iostream>

#if defined(__GNUC__)
	#define inline __attribute((always_inline))
#else
	#define inline __forceinline
#endif



namespace smath
{

/// Выделение памяти //////////////////////////////////////////////////////////////////////
	
template <typename T>
inline T* new_memory(int aligned, std::size_t count)
{
	if (aligned == 0)
	{

		return new T[count];

	}
	else
	{

#ifdef WIN32
		return reinterpret_cast<T*>(_aligned_malloc(sizeof(T)*count, aligned));
#else
		return reinterpret_cast<T*>(memalign(aligned, sizeof(T)*count)); // posix
#endif

	}
}

template <typename T>
inline T* new_memory(std::size_t count)
{
#ifdef m128_t
    return new_memory<T>(16, count);
#else
    return new_memory<T>(0, count);
#endif
}


inline void del_memory(void* p)
{
#ifdef WIN32

    _aligned_free(p);

#else

    free(p);

#endif

}


////////////////////////////////////////////////////////////////////////////////////////////


	template <typename T>
	struct plus
	{
		inline T apply (const T& a, const T& b) const
		{

	#ifdef m128_t
			return _mm_add_ps(a,b);
	#else
			return a + b;
	#endif

		};
	};

	template <typename T>
	struct minus
	{
		inline T apply (const T& a, const T& b) const
		{

	#ifdef m128_t
			return _mm_sub_ps(a,b);
	#else
			return a - b;
	#endif

		};
	};

	template <typename T>
	struct mul
	{
		inline T apply (const T& a, const T& b) const
		{

	#ifdef m128_t
			return _mm_mul_ps(a,b);
	#else
			return a * b;
	#endif

		};
	};

	template <typename T>
	struct div
	{
		inline T apply (const T& a, const T& b) const
		{

	#ifdef m128_t
			return _mm_div_ps(a,b);
	#else
			return a / b;
	#endif

		};
	};

}


#define OP_BASE(_OP_,_OP_Z)										\
													\
template<typename T, typename Left, typename Right, int size>						\
inline X<T, Left, _OP_, Right, size>  operator _OP_Z (const Left& a, const Right& b)			\
{													\
	return X<T, Left, _OP_, Right, size>(a, b);							\
}													\
													\
template<typename T, typename Left, int size>								\
inline X<T, Left, _OP_, T, size>  operator _OP_Z (const Left& a, const T& b)				\
{													\
	return X<T, Left, _OP_, T, size>(a, b);								\
}													\
													\
template<typename T, typename Left, int size>								\
inline X<T, Left, _OP_, const T*, size>  operator _OP_Z (const Left& a, const Vector<T,size>& b)	\
{													\
	return X<T, Left, _OP_, const T*, size>(a, b.data);						\
}													\
													\
template<typename T, typename Right, int size>								\
inline X<T, const T*, _OP_, Right, size> operator _OP_Z (const Vector<T, size>& a, const Right& b)	\
{													\
	return X<T, const T*, _OP_, Right, size>(a.data, b);						\
}													\
													\
template<typename T, int size>										\
inline X<T, const T*, _OP_, T, size> operator _OP_Z (const Vector<T,size>& a, const T& b)		\
{													\
	return X<T, const T*, _OP_, T, size>(a.data, b);						\
}													\
													\
template<typename T, int size>										\
inline X<T, T, _OP_, const T*, size>  operator _OP_Z (const T& a, const Vector<T,size>& b)		\
{													\
	return X<T, T, _OP_, const T*, size>(a, b.data);						\
}													\
													\
template<typename T, typename Right, int size>								\
inline X<T, T, _OP_, Right, size>  operator _OP_Z (const T& a, const Right& b)				\
{													\
	return X<T, T, _OP_, Right, size>(a, b);							\
}													\
													\
template<typename T, int size>										\
inline X<T, T, _OP_, T, size>  operator _OP_Z (const T& a, const T& b)					\
{													\
	return X<T, T, _OP_, T, size>(a, b);								\
}													\
													\
template<typename T, int size>										\
inline X<T, const T*, _OP_, const T*, size> 								\
operator _OP_Z (const Vector<T, size>& a, const Vector<T,size>& b)					\
{													\
	return X<T, const T*, _OP_, const T*, size>(a.data, b.data);					\
}													\
													\
template												\
	<												\
		typename T, typename Left1, typename Op1, typename Right1, int size,			\
		typename Left2, typename Op2, typename Right2						\
	>												\
inline 	X<T, X< T, Left1, Op1, Right1, size>, _OP_, X<T, Left2, Op2, Right2, size>, size> 		\
operator _OP_Z												\
	(const  X<T, Left1, Op1, Right1, size>& a, const  X<T, Left2, Op2, Right2, size>& b)		\
{													\
	return X<T, X<T, Left1, Op1, Right1, size>, _OP_, 						\
	X<T, Left2, Op2, Right2, size>, size>(a, b);							\
}													\




namespace smath
{

template <typename T, typename Left, typename Op, typename Right, int size>
struct X
{
	X(Left t1, Right t2) : lNode_(t1), rNode_(t2){}

	T operator[] (int i) const
	{
		Op op_;

		return op_.apply(lNode_[i], rNode_[i]);
	}

	private:
		const Right rNode_;
		const Left  lNode_;
};

#ifdef m128_t

template <typename T, typename Left, typename Op, int size>
struct X<T, Left, Op, precision_t, size>
{
	X(Left t1, precision_t t2) : lNode_(t1), rNode_(t2){}

	T operator[] (int i) const
	{
		Op op_;

		return op_.apply(lNode_[i], _mm_set1_ps(rNode_));
	}

	private:
		const precision_t rNode_;
		const Left  lNode_;
};

template <typename T, typename Op, typename Right, int size>
struct X<T, precision_t, Op, Right, size>
{
	X(precision_t t1, Right t2) : lNode_(t1), rNode_(t2){}

	T operator[] (int i) const
	{
		Op op_;

		return op_.apply(_mm_set1_ps(lNode_), rNode_[i]);
	}

	private:
		const Right rNode_;
		const precision_t  lNode_;
};

#endif

template <typename T, typename Left, typename Op, int size>
struct X <T, Left, Op, T, size>
{
	X(Left t1, T t2) : lNode_(t1), rNode_(t2){}

        T operator[] (int i) const
        {
                Op op_;

                return op_.apply(lNode_, rNode_);
        }

        private:
                const T rNode_;
                const Left  lNode_;
};


template <typename T, typename Left, typename Op, int size>
struct X <T, Left, Op, T*, size>
{
	X(Left t1, T * t2) : lNode_(t1), rNode_(t2){}

        T operator[] (int i) const
        {
                Op op_;

                return op_.apply(lNode_, rNode_[i]);
        }

        private:
                const T* rNode_;
                const Left  lNode_;
};


template <typename T, typename Left, typename Op, int size>
struct X <T, Left*, Op, T, size>
{
	X(Left* t1, T t2) : lNode_(t1), rNode_(t2){}

        T operator[] (int i) const
        {
                Op op_;

                return op_.apply(lNode_[i], rNode_);
        }

        private:
                const T rNode_;
                const Left*  lNode_;
};



template <typename T, typename Left, typename Op, int size>
struct X <T, Left*, Op, T*, size>
{
	X(Left* t1, T* t2) : lNode_(t1), rNode_(t2){}

        T operator[] (int i) const
        {
                Op op_;

                return op_.apply(lNode_[i], rNode_[i]);
        }

        private:
                const T* rNode_;
                const Left*  lNode_;
};


template <typename T, typename Op, int size>
struct X <T, T, Op, T, size>
{
	X(T t1, T t2) : lNode_(t1), rNode_(t2){}

        T operator[] (int i) const
        {
                Op op_;

                return op_.apply(lNode_, rNode_);
        }

        private:
                const T rNode_;
                const T lNode_;
};



template <typename T, typename Op, int size>
struct X <T, T, Op, T*, size>
{
	X(T t1, T* t2) : lNode_(t1), rNode_(t2){}

        T operator[] (int i) const
        {
                Op op_;

                return op_.apply(lNode_, rNode_[i]);
        }

        private:
                const T* rNode_;
                const T lNode_;
};



template <typename T, typename Op, typename Right, int size>
struct X <T, T, Op, Right, size>
{
	X(T t1, Right t2) : lNode_(t1), rNode_(t2){}

        T operator[] (int i) const
        {
                Op op_;

                return op_.apply(lNode_, rNode_);
        }

        private:
                const Right rNode_;
                const T lNode_;
};


template <typename T, typename Op, typename Right, int size>
struct X <T, T, Op, Right*, size>
{
	X(T t1, Right* t2) : lNode_(t1), rNode_(t2){}

        T operator[] (int i) const
        {
                Op op_;

                return op_.apply(lNode_, rNode_[i]);
        }

        private:
                const Right* rNode_;
                const T lNode_;
};


template <typename T, typename Op, typename Right, int size>
struct X <T, T*, Op, Right, size>
{
	X(T* t1, Right t2) : lNode_(t1), rNode_(t2){}

        T operator[] (int i) const
        {
                Op op_;

                return op_.apply(lNode_[i], rNode_);
        }

        private:
                const Right rNode_;
                const T* lNode_;
};


template <typename T, typename Op, typename Right, int size>
struct X <T, T*, Op, Right*, size>
{
	X(T* t1, Right* t2) : lNode_(t1), rNode_(t2){}

        T operator[] (int i) const
        {
                Op op_;

                return op_.apply(lNode_[i], rNode_[i]);
        }

        private:
                const Right* rNode_;
                const T* lNode_;
};


template <typename T, typename Op, int size>
struct X <T, T*, Op, T, size>
{
	X(T* t1, T t2) : lNode_(t1), rNode_(t2){}

        T operator[] (int i) const
        {
                Op op_;

                return op_.apply(lNode_[i], rNode_);
        }

        private:
                const T  rNode_;
                const T* lNode_;
};


template <typename T, typename Op, int size>
struct X <T, T*, Op, T*, size>
{
	X(T* t1, T* t2) : lNode_(t1), rNode_(t2){}

        T operator[] (int i) const
        {
                Op op_;

                return op_.apply(lNode_[i], rNode_[i]);
        }

        private:
                const T* rNode_;
                const T* lNode_;
};


template <class T, int size>
struct Vector
{

#ifdef m128_t

	#ifndef WIN32
		__attribute__((aligned(16)))
	#else
		__declspec(align(16))
	#endif

#endif
		T data[size];


		template<typename Left, typename Op, typename Right>
		Vector<T,size> (const X<T, Left, Op, Right, size> expression)
		{
			for (int i=0; i<size; i++)
				data[i] = expression[i];
		}

		Vector<T,size>()
		{

#ifdef m128_t

			for (int i=0; i<size; ++i)
			{
				data[i] = _mm_setzero_ps();
			}
#else

			for (int i=0; i<size; ++i)
			{
				data[i] = T(0);
			}

#endif

		}

		Vector<T,size>(T t1)
		{
			data[0] = t1;
		}


#ifdef m128_t

		Vector<T, size>(float x, float y, float z, float w)
		{

			data[0] = _mm_setr_ps(x, y, z, w);

		}

#endif

		inline Vector (const T* data_a)
		{
			for(std::size_t i = 0; i < size; ++i)
				data[i] = data_a[i];
		}


		inline const T& operator[] (std::size_t i) const
		{
			return data[i];
		}


		template<typename Left, typename Op, typename Right>
		inline void operator= (const X<T,Left,Op,Right,size>& expression)
		{
			for (int i = 0; i < size; ++i)
				data[i] = expression[i];
		}

		inline Vector (const Vector<T,size>& x)
		{
			for(std::size_t i = 0; i < size; ++i)
				data[i] = x.data[i];
		}

		inline T& operator[] (std::size_t i)
		{
			return data[i];
		}

		template<typename Left, typename Op, typename Right>
		inline void operator+= (const X<T,Left,Op,Right,size>& expression)
		{
			for (int i = 0; i < size; ++i)
				data[i] += expression[i];
		}

		inline void Zero()
		{

#ifdef m128_t
			for (int i=0; i<size; ++i) data[i] = _mm_setzero_ps();
			/// R0   R1   R2   R3
			/// 0.0  0.0  0.0  0.0
#else
			for (int i=0; i<size; ++i) data[i] = 0;
#endif
		}

		inline Vector<T, size> Normalize()
		{

#ifdef m128_t

			Vector<T, size> r;
			__m128 a;

			precision_t max = 0;

			precision_t tmp;
			
			for (int i=0; i<size; ++i)
			{
				Vector<T,size> tt(data[i]);
				tmp = dot(tt,tt);
				max += (tmp);
			}

			max = 1.0f/sqrt(max);

			a  = _mm_set1_ps(max);

			for (int i=0; i<size; ++i)
			{
				r[i] = _mm_mul_ps(data[i], a);
			}

			return r;

#else
			Vector<T, size> r;
			T max = 0;

			for (int i = 0; i<size; ++i) max += data[i]*data[i];

			max = T(1) / sqrt(max);

			for (int i = 0; i<size; ++i) r[i] = max * data[i];

			return r;
#endif

		}

		inline void set (float x, float y, float z, float w)
		{

#ifdef m128_t

			data[0] = _mm_setr_ps(x, y, z, w);

#else

			data[0] = x;
			data[1] = y;
			data[2] = z;
			data[3] = w;

#endif
		}


#ifdef m128_t

		inline void Clump(float & _min, float & _max)
		{

			__m128 min = _mm_setl_ps(_min);
			__m128 max = _mm_setl_ps(_max);

			for (int i = 0; i<size; ++i)
			{
				data[i] = _mm_max_ps(data[i], min);
				data[i] = _mm_min_ps(data[i], max);
			}
		}

#else

		inline void Clump(T min, T max)
		{
			for (int i = 0; i<size; ++i)
			{
				if (data[i] > max) data[i] = max;
				if (data[i] < min) data[i] = min;
			}
		}

#endif

		

};

template <typename T, int size>
inline void Normalize(Vector<T, size>& a)
{
	a = a.Normalize();
}


template <typename T, int size>
inline precision_t dot(const Vector<T, size> a, const Vector<T, size> b)
{

#ifdef m128_t

			precision_t r = 0;
			precision_t tmp;
			__m128 t;
			
			for (int i=0; i<size; ++i)
			{
				t = _mm_mul_ps (a[i], b[i]);

#ifdef use_sse3
				t = _mm_hadd_ps(t, t);
				t = _mm_hadd_ps(t, t);
				_mm_store_ss(&tmp, t);
#else
				float p[4];
				_mm_storeu_ps(p,t);
				tmp = p[0] + p[1] + p[2] + p[3];
#endif
		
				r += tmp;
			}

			return r;

#else

	T r = 0;
	for (int i = 0; i<size; ++i) r += (a[i]*b[i]);
	return r;

#endif
}


template <typename T, typename Left, typename Op, typename Right, int size>
inline precision_t dot(const Vector<T, size> a, const X<T,Left,Op,Right,size> b)
{
	return dot(a, Vector<T,size>(b));
}

template <typename T, typename Left, typename Op, typename Right, int size>
inline precision_t dot(const X<T,Left,Op,Right,size> a, const Vector<T, size> b)
{
	return dot(Vector<T,size>(a), b);
}

template <typename T, typename Left, typename Op, typename Right, int size>
inline precision_t dot(const X<T,Left,Op,Right,size> a, const X<T,Left,Op,Right,size> b)
{
	return dot(Vector<T,size>(a), Vector<T,size>(b));
}

// 

/*
template <typename T, int size>
inline precision_t dot(const Vector<T, size>& a, const Vector<T, size>& b)
{

#ifdef m128_t

			precision_t r = 0;
			precision_t tmp;
			__m128 t;
			
			for (int i=0; i<size; ++i)
			{
				t = _mm_mul_ps (a[i], b[i]);

#ifdef use_sse3
				t = _mm_hadd_ps(t, t);
				t = _mm_hadd_ps(t, t);
				_mm_store_ss(&tmp, t);
#else
				float p[4];
				_mm_storeu_ps(p,t);
				tmp = p[0] + p[1] + p[2] + p[3];
#endif
		
				r += tmp;
			}

			return r;

#else

	T r = 0;
	for (int i = 0; i<size; ++i) r += (a[i]*b[i]);
	return r;

#endif
}
*/


template <typename T, int size>
inline precision_t CalcDistance(const Vector<T, size>& a, const Vector<T, size>& b)
{

#ifdef m128_t

	Vector<T,size> c = a - b;

	return sqrt(dot(c, c));

#else
	
	T t = 0;
	Vector<T,size> c = a - b;
	t = dot(c, c);
	return sqrt(t);

#endif

}


template <typename T, int size>
inline std::ostream& operator << (std::ostream& out, const Vector<T, size>& x)
{
#ifdef m128_t

	float q[4];

    for(std::size_t i = 0; i < size; ++i)
	{
		_mm_storeu_ps(q, x[i]);
        out << q[0] << ' ' << q[1] << ' '<< q[2] << ' '<< q[3] << ' ';
	}
    return out;

#else

    for(std::size_t i = 0; i < size; ++i)
        out << x.data[i] << ' ';
    return out;

#endif

}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

	OP_BASE(plus<T>,+)


	OP_BASE(minus<T>,-)


	OP_BASE(mul<T>,*)


	OP_BASE(div<T>,/)

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //


#ifdef m128_t


	typedef Vector<__m128, 1> Vector4;


#else


	typedef Vector<precision_t, 4> Vector4;


#endif


inline Vector4 cross(const Vector4& v1, const Vector4& v2)
{

#ifdef m128_t

	__m128 a,b,c;
	
	a = _mm_shuffle_ps(v1[0], v1[0], 0xC9); /// 11 00 10 01
	b = _mm_shuffle_ps(v2[0], v2[0], 0xD2); /// 11 01 00 10 
	c = _mm_mul_ps(a,b);

	a = _mm_shuffle_ps(v1[0], v1[0], 0xD2); /// 11 01 00 10
	b = _mm_shuffle_ps(v2[0], v2[0], 0xC9); /// 11 00 10 01
	a = _mm_mul_ps(a, b);

	c = _mm_sub_ps(c, a);

	return c;

#else

	Vector4 temp;

	temp[0] = v1[1]*v2[2] - v1[2]*v2[1];
	temp[1] = v1[2]*v2[0] - v1[0]*v2[2];
	temp[2] = v1[0]*v2[1] - v1[1]*v2[0];
	temp[3] = 0;

	return temp;

#endif

}

inline _uint32_t ToColor(Vector4& color)
{

#ifdef m128_t

	_uint32_t r;
	__m128 a;

	a = _mm_set1_ps(255.0f);

	color = _mm_mul_ps(color[0], a);
	color = _mm_min_ps(color[0], a);

	float q[4];
	_mm_storeu_ps(q, color[0]);

	r = 	((_uint32_t)q[0]*(1<<16)) +
		((_uint32_t)q[1]*(1<<8)) +
		((_uint32_t)q[2]);

	return r;

#else

	_uint32_t r;

	for (_uint32_t i=0; i<4; ++i)
	{
		color[i] *= 255;
		if (color[i]>255) color[i] = 255;
	}

	r = 	((_uint32_t)color[0]*(1<<16)) +
		((_uint32_t)color[1]*(1<<8)) +
		((_uint32_t)color[2]);

	return r;

#endif
}


}

#endif
