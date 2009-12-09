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
#include <malloc.h>
#include <iostream>

#define m128_t
//#define use_sse3


typedef float				precision_t;

typedef unsigned short 			_uint16_t;
typedef short 				_sint16_t;

typedef unsigned int 			_uint32_t;
typedef int 				_sint32_t;

typedef unsigned long long int 		_uint64_t;
typedef long long int 			_sint64_t;

typedef unsigned long int 		_uint_t;
typedef long int 			_sint_t;



#include <xmmintrin.h>
#include <mmintrin.h>


#ifdef use_sse3

	#include <pmmintrin.h>

#endif


#if defined(__GNUC__)
	#define inline __attribute((always_inline))
#else
	#define inline __forceinline
#endif


#ifndef WIN32
	#define _align_ __attribute__((aligned(16)))
#else
	#define _align_ __declspec(align(16))
#endif

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

//#ifdef m128_t

	return new_memory<T>(16, count);

//#else

//	return new_memory<T>(0, count);

//#endif

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


namespace math
{

	template <typename T>
	struct plus
	{
		inline T apply (const T& a, const T& b) const
		{
			return a + b;
		}
	};


	template <>
	struct plus<__m128>
	{
		inline __m128 apply (const __m128 & a, const __m128 & b) const
		{
			return _mm_add_ps(a,b);
		}
	};



	template <typename T>
	struct minus
	{
		inline T apply (const T& a, const T& b) const
		{
			return a - b;
		}
	};


	template <>
	struct minus<__m128>
	{
		inline __m128 apply (const __m128& a, const __m128& b) const
		{
			return _mm_sub_ps(a,b);
		}
	};



	template <typename T>
	struct mul
	{
		inline T apply (const T& a, const T& b) const
		{
			return a * b;
		}
	};


	template <>
	struct mul<__m128>
	{
		inline __m128 apply (const __m128& a, const __m128& b) const
		{
			return _mm_mul_ps(a,b);
		}
	};




	template <typename T>
	struct div
	{
		inline T apply (const T& a, const T& b) const
		{
			return a / b;
		}
	};


	template <>
	struct div<__m128>
	{
		inline __m128 apply (const __m128& a, const __m128& b) const
		{
			return _mm_div_ps(a,b);
		}
	};

////////////////////////////////////////////////////////////////////////////////////////////

template <typename T, typename Left, typename Op, typename Right, _uint32_t size>
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

template <typename T, typename Left, typename Op, _uint32_t size>
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

template <typename T, typename Op, typename Right, _uint32_t size>
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

template <typename T, typename Left, typename Op, _uint32_t size>
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


template <typename T, typename Left, typename Op, _uint32_t size>
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


template <typename T, typename Left, typename Op, _uint32_t size>
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



template <typename T, typename Left, typename Op, _uint32_t size>
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


template <typename T, typename Op, _uint32_t size>
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



template <typename T, typename Op, _uint32_t size>
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



template <typename T, typename Op, typename Right, _uint32_t size>
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


template <typename T, typename Op, typename Right, _uint32_t size>
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


template <typename T, typename Op, typename Right, _uint32_t size>
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


template <typename T, typename Op, typename Right, _uint32_t size>
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


template <typename T, typename Op, _uint32_t size>
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


template <typename T, typename Op, _uint32_t size>
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


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//     //       //   ////////   ///////  //////////   ////////   //////      //
//     //       //   //         //       //  //  //   //    //   //   //     //
//      //     //    /////      //           //       //    //   //////      //
//       //   //     //         //           //       //    //   //  //      //
//        // //      //         //           //       //    //   //   //     //
//         //        ////////   ///////      //       ////////   //    //    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

template <class T, _uint32_t size>
struct Vector
{

	_align_
	T data[size];


	template<typename Left, typename Op, typename Right>
	Vector<T,size> (const X<T, Left, Op, Right, size> expression)
	{
		for (int i=0; i<size; i++)
			data[i] = expression[i];
	}

	Vector<T,size>()
	{
		for (int i=0; i<size; ++i)
		{
			data[i] = T();
		}
	}



	Vector<T,size>(T t1)
	{
		data[0] = t1;
	}

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

	inline T& operator() (std::size_t i)
	{
		return data[i];
	}

	template<typename Left, typename Op, typename Right>
	inline void operator+= (const X<T,Left,Op,Right,size>& expression)
	{
		for (int i = 0; i < size; ++i)
			data[i] += expression[i];
	}

	inline void zero()
	{
		for (int i=0; i<size; ++i) data[i] = 0;
	}

	inline void one()
	{
		for (int i=0; i<size; ++i) data[i] = 1;
	}

	inline Vector<T, size> normalize()
	{
		Vector<T, size> r;
		T max = 0;
		for (int i = 0; i<size; ++i) max += data[i]*data[i];
			max = T(1) / sqrt(max);

		for (int i = 0; i<size; ++i)
			r[i] = max * data[i];

		return r;
	}


	inline void set (T data, _uint32_t pos = 0)
	{
		data[pos] = data;
	}



	inline void clamp(T & min, T & max)
	{
		for (int i = 0; i<size; ++i)
		{
			if (data[i] > max) data[i] = max;
			if (data[i] < min) data[i] = min;
		}
	}
};



template <_uint32_t size>
struct Vector<__m128, size>
{

	_align_
	__m128 data[size];


	template<typename Left, typename Op, typename Right>
	Vector<__m128,size> (const X<__m128, Left, Op, Right, size> expression)
	{
		for (_uint32_t i=0; i<size; i++)
			data[i] = expression[i];
	}

	Vector<__m128,size>()
	{
		for (_uint32_t i=0; i<size; ++i)
		{
			data[i] = _mm_setzero_ps();
		}
	}


	Vector<__m128,size>(__m128 & t1)
	{
		data[0] = t1;
	}


	inline Vector (const __m128 * data_a)
	{
		for(_uint32_t i = 0; i < size; ++i)
			data[i] = data_a[i];
	}

	inline Vector ( float x, float y, float z, float w )
	{
		set(x,y,z,w);
	}

	inline const __m128& operator[] (std::size_t i) const
	{
		return data[i];
	}

	inline float operator() (std::size_t i)
	{
		_align_
		float q[4];
		_mm_storeu_ps(q, data[i/4]);
		return q[i%4];
	}

	template<typename Left, typename Op, typename Right>
	inline void operator= (const X<__m128,Left,Op,Right,size>& expression)
	{
		for (_uint32_t i = 0; i < size; ++i)
			data[i] = expression[i];
	}

	inline Vector (const Vector<__m128,size>& x)
	{
		for(_uint32_t i = 0; i < size; ++i)
			data[i] = x.data[i];
	}

	inline __m128 & operator[] (std::size_t i)
	{
		return data[i];
	}

	template<typename Left, typename Op, typename Right>
	inline void operator+= (const X<__m128,Left,Op,Right,size>& expression)
	{
		for (_uint32_t i = 0; i < size; ++i)
			data[i] += expression[i];
	}

	inline void zero()
	{
		for (_uint32_t i=0; i<size; ++i) data[i] = _mm_setzero_ps();
	}

	inline Vector<__m128, size> normalize()
	{
		Vector<__m128, size> r;
		__m128 a;

		float max = 0;
		float tmp;

		for (_uint32_t i=0; i<size; ++i)
		{
			Vector<__m128,size> tt(data[i]);
			tmp = dot(tt,tt);
			max += (tmp);
		}

		max = 1.0f/sqrt(max);
		a  = _mm_set1_ps(max);

		for (_uint32_t i=0; i<size; ++i)
		{
			r[i] = _mm_mul_ps(data[i], a);
		}

		return r;
	}

	inline void set (float x, float y, float z, float w, _uint32_t pos = 0)
	{
		data[pos] = _mm_setr_ps(x, y, z, w);
	}

	inline void clamp(float & _min, float & _max)
	{
		__m128 min = _mm_set1_ps(_min);
		__m128 max = _mm_set1_ps(_max);

		for (_uint32_t i = 0; i<size; ++i)
		{
			data[i] = _mm_max_ps(data[i], min);
			data[i] = _mm_min_ps(data[i], max);
		}
	}
};




template <typename T, _uint32_t size>
inline void Normalize(Vector<T, size>& a)
{
	a = a.normalize();
}


template <typename T, _uint32_t size>
inline T Dot(const Vector<T, size> a, const Vector<T, size> b)
{
	T r = 0;

	for (_uint32_t i = 0; i<size; ++i)
		r += (a[i]*b[i]);

	return r;
}

template <_uint32_t size>
inline float Dot (const Vector<__m128, size> a, const Vector<__m128, size> b)
{
	float r = 0.0f;
	float tmp;
	__m128 t;

	for (_uint32_t i = 0; i<size; ++i)
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
}

template <typename T, typename Left, typename Op, typename Right, _uint32_t size>
inline precision_t Dot(const Vector<T, size> a, const X<T,Left,Op,Right,size> b)
{
		return Dot(a, Vector<T,size>(b));
}

template <typename T, typename Left, typename Op, typename Right, _uint32_t size>
inline precision_t Dot(const X<T,Left,Op,Right,size> a, const Vector<T, size> b)
{
		return Dot(Vector<T,size>(a), b);
}

template <typename T, typename Left, typename Op, typename Right, _uint32_t size>
inline precision_t Dot(const X<T,Left,Op,Right,size> a, const X<T,Left,Op,Right,size> b)
{
		return Dot(Vector<T,size>(a), Vector<T,size>(b));
}

template <typename T, _uint32_t size>
inline std::ostream& operator << (std::ostream& out, const Vector<T, size>& x)
{
	for(_uint32_t i = 0; i < size; ++i)
		out << x.data[i] << ' ';
	return out;
}


template < _uint32_t size>
inline std::ostream& operator << (std::ostream& out, const Vector<__m128, size>& x)
{
	_align_
	float q[4];

	for(std::size_t i = 0; i < size; ++i)
	{
		_mm_storeu_ps(q, x[i]);
		out << q[0] << ' ' << q[1] << ' '<< q[2] << ' '<< q[3] << ' ';
	}

	return out;
}

template <typename T, _uint32_t size>
inline precision_t CalcDistance(const Vector<T, size>& a, const Vector<T, size>& b)
{
	return sqrt(dot(a-b,a-b));
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

#define OP_BASE(_OP_,_OP_Z)																			\
																						\
template<typename T, typename Left, typename Right, _uint32_t size>														\
inline X<T, Left, _OP_, Right, size>  operator _OP_Z (const Left& a, const Right& b)												\
{																						\
		return X<T, Left, _OP_, Right, size>(a, b);															\
}																						\
																						\
template<typename T, typename Left, _uint32_t size>													   			\
inline X<T, Left, _OP_, T, size>  operator _OP_Z (const Left& a, const T& b)													\
{																						\
		return X<T, Left, _OP_, T, size>(a, b);													 			\
}																						\
																						\
template<typename T, typename Left, _uint32_t size>													   			\
inline X<T, Left, _OP_, const T*, size>  operator _OP_Z (const Left& a, const Vector<T,size>& b)										\
{																						\
		return X<T, Left, _OP_, const T*, size>(a, b.data);										 				\
}																						\
																						\
template<typename T, typename Right, _uint32_t size>													  			\
inline X<T, const T*, _OP_, Right, size> operator _OP_Z (const Vector<T, size>& a, const Right& b)  										\
{																						\
		return X<T, const T*, _OP_, Right, size>(a.data, b);														\
}																						\
																						\
template<typename T, _uint32_t size>																	  	\
inline X<T, const T*, _OP_, T, size> operator _OP_Z (const Vector<T,size>& a, const T& b)		   									\
{																						\
		return X<T, const T*, _OP_, T, size>(a.data, b);														\
}																						\
																						\
template<typename T, _uint32_t size>																	  	\
inline X<T, T, _OP_, const T*, size>  operator _OP_Z (const T& a, const Vector<T,size>& b)		  									\
{																						\
		return X<T, T, _OP_, const T*, size>(a, b.data);														\
}																						\
																						\
template<typename T, typename Right, _uint32_t size>													  			\
inline X<T, T, _OP_, Right, size>  operator _OP_Z (const T& a, const Right& b)					  								\
{																						\
		return X<T, T, _OP_, Right, size>(a, b);															\
}																						\
																						\
template<typename T, _uint32_t size>																	  	\
inline X<T, T, _OP_, T, size>  operator _OP_Z (const T& a, const T& b)							  							\
{																						\
		return X<T, T, _OP_, T, size>(a, b);																\
}																						\
																						\
template<typename T, _uint32_t size>																	  	\
inline X<T, const T*, _OP_, const T*, size>														 			\
operator _OP_Z (const Vector<T, size>& a, const Vector<T,size>& b)								  						\
{																						\
		return X<T, const T*, _OP_, const T*, size>(a.data, b.data);													\
}																						\
																						\
template																					\
	<																					\
		typename T, typename Left1, typename Op1, typename Right1, _uint32_t size,											\
		typename Left2, typename Op2, typename Right2									   						\
	>																					\
inline  X<T, X< T, Left1, Op1, Right1, size>, _OP_, X<T, Left2, Op2, Right2, size>, size>		   									\
operator _OP_Z																					\
		(const  X<T, Left1, Op1, Right1, size>& a, const  X<T, Left2, Op2, Right2, size>& b)										\
{																						\
		return X<T, X<T, Left1, Op1, Right1, size>, _OP_,										   				\
		X<T, Left2, Op2, Right2, size>, size>(a, b);															\
}																						\


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //


		OP_BASE(plus<T>,+)


		OP_BASE(minus<T>,-)


		OP_BASE(mul<T>,*)


		OP_BASE(div<T>,/)

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //



typedef Vector<__m128, 1> Vector4;
typedef Vector4 vec4;


inline Vector4 Cross(const Vector4& v1, const Vector4& v2)
{
	__m128 a,b,c;

	a = _mm_shuffle_ps(v1[0], v1[0], 0xC9); /// 11 00 10 01
	b = _mm_shuffle_ps(v2[0], v2[0], 0xD2); /// 11 01 00 10
	c = _mm_mul_ps(a,b);

	a = _mm_shuffle_ps(v1[0], v1[0], 0xD2); /// 11 01 00 10
	b = _mm_shuffle_ps(v2[0], v2[0], 0xC9); /// 11 00 10 01
	a = _mm_mul_ps(a, b);

	c = _mm_sub_ps(c, a);

	return c;
}

template <typename T>
inline Vector<T,3> Cross(const Vector<T,3>& v1, const Vector<T,3>& v2)
{
	Vector<T,3> temp;

	temp[0] = v1[1]*v2[2] - v1[2]*v2[1];
	temp[1] = v1[2]*v2[0] - v1[0]*v2[2];
	temp[2] = v1[0]*v2[1] - v1[1]*v2[0];

	return temp;
}

inline _uint32_t ToColor(Vector4& color)
{
	_uint32_t r;

	__m128 a, b;

	b = color[0];
	a = _mm_set1_ps(255.0f);

	b = _mm_mul_ps(b, a);
	b = _mm_min_ps(b, a);

	float q[4];
	_mm_storeu_ps(q, b);

	r =	((_uint32_t)q[0]*(1<<16)) +
		((_uint32_t)q[1]*(1<<8)) +
		((_uint32_t)q[2]);

	return r;

}

}

#undef m128_t
#undef use_sse3

#endif

