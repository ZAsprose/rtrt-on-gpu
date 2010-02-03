/**
 *** @file fastmv.hpp
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

#ifndef __fastmv_hpp__
#define __fastmv_hpp__

#include "mmemory.hpp"
#include <math.h>

namespace fastmath
{

///////////////////////////////
/// configuration parametrs ///
///////////////////////////////

#define use_sse2
//#define use_sse3


////////// controls ///////////

#ifdef use_sse3
	#define use_sse2
#endif

///////////////////////////////
///////////////////////////////
///////////////////////////////

#ifdef use_sse2
	#include <xmmintrin.h>
	#include <mmintrin.h>
#endif

#ifdef use_sse3
	#include <pmmintrin.h>
#endif	

///////////////////////////////
///////////////////////////////
///////////////////////////////

#ifndef WIN32
	#define _align_ __attribute__((aligned(16)))
#else
	#define _align_ __declspec(align(16))
#endif

///////////////////////////////
///////////////////////////////
///////////////////////////////

template <bool b, std::size_t i, std::size_t j>
class IFThenElse
{
};

template <std::size_t i, std::size_t j>
class IFThenElse < true, i, j>
{
public:

	enum
	{
		ret = i
	};
};

template <std::size_t i, std::size_t j>
class IFThenElse < false, i, j>
{
public:

	enum
	{
		ret = j
	};
};

///////////////////////////////
///////////////////////////////
///////////////////////////////

template <std::size_t dim, typename T>
class DotProduct
{
public:

	static inline T result (T*a, T*b, std::size_t da = 1, std::size_t db = 1)
	{
		const std::size_t res = IFThenElse < dim >= 4, 4, 1 > ::ret;

		return	DotProduct < res, T>::result(a, b, da, db) +
				DotProduct < dim - res, T>::result(a + res*da, b + res*db, da, db);
		
	}
};

template <typename T>
class DotProduct < 1, T >
{
public:

	static inline T result(T*a, T*b, std::size_t da, std::size_t db)
	{
		return (*a)*(*b);
	}
};

template <typename T>
class DotProduct < 0, T >
{
public:

	static inline T result(T*a, T*b, std::size_t da, std::size_t db)
	{
		return 0;
	}
};

template <>
class DotProduct < 4, float>
{
public:

	static inline float result(float *a, float *b, std::size_t da, std::size_t db)
	{
		#ifdef use_sse2
			_align_ __m128 m1, m2, m3, m4;
			_align_	float res;

			m4 = _mm_setzero_ps();
			m1 = _mm_setr_ps(a[0], a[da], a[2 * da], a[3 * da]);
			m2 = _mm_setr_ps(b[0], b[db], b[2 * db], b[3 * db]);
			m3 = _mm_mul_ps(m1, m2);

			#ifdef use_sse3
				m3 = _mm_hadd_ps(m3, m3);
				m4 = _mm_hadd_ps(m3, m3);
				_mm_store_ss(&res, m4);
			#else
				_align_ float p[4];
				_mm_storeu_ps(p, m3);
				res = p[0] + p[1] + p[2] + p[3];
			#endif
		#else
				_align_	float res;
				res = a[0]*b[0] + a[da]*b[db] + a[2 * da]*b[2 * db] + a[3 * da]*b[3 * db];
		#endif

		return res;
	}
};

//////////////////////////////

template <std::size_t dim, typename T>
inline T dot(T * a, T * b, std::size_t da = 1, std::size_t db = 1)
{
	return DotProduct<dim, T>::result(a, b, da, db);
}



///////////////////////////////////////////


	template <typename T, typename Left, typename Op, typename Right, std::size_t size>
	struct X;


///////////////////////////////
///////////////////////////////
///////////////////////////////



	template <class T, std::size_t size>
	struct vec
	{
		_align_
		T data[size];

		template<typename Left, typename Op, typename Right >
		vec<T, size > (const X<T, Left, Op, Right, size> expression)
		{
			for (std::size_t i = 0; i < size; i++)
				data[i] = expression[i];
		}

		vec<T, size > ()
		{
			for (std::size_t i = 0; i < size; ++i) data[i] = T();
		}

		vec<T, size > (T t1)
		{
			data[0] = t1;
		}

		#ifdef use_sse2
			vec<T, size > (const __m128 & m)
			{
				_mm_storeu_ps(m, data);
			}
		#endif

		std::size_t vec_size()
		{
			return size;
		}

		inline vec(const T * data_a)
		{
			for (std::size_t i = 0; i < size; ++i)
				data[i] = data_a[i];
		}

		inline const T & operator[] (std::size_t i) const
		{
			return data[i];
		}

		template<typename Left, typename Op, typename Right>
		inline void operator=(const X<T, Left, Op, Right, size>& expression)
		{
			for (std::size_t i = 0; i < size; ++i)
				data[i] = expression[i];
		}

		template<typename Left, typename Op, typename Right>
		inline void operator+=(const X<T, Left, Op, Right, size>& expression)
		{
			for (std::size_t i = 0; i < size; ++i)
				data[i] += expression[i];
		}

		inline vec(const vec<T, size>& x)
		{
			for (std::size_t i = 0; i < size; ++i)
				data[i] = x.data[i];
		}

		inline T & operator[] (std::size_t i)
		{
			return data[i];
		}

		inline T & operator() (std::size_t i)
		{
			return data[i];
		}

		inline float * get()
		{
			return reinterpret_cast<float *>(&data);
		}

		inline void zero()
		{
			for (std::size_t i = 0; i < size; ++i) data[i] = 0;
		}

		inline void one()
		{
			for (std::size_t i = 0; i < size; ++i) data[i] = 1;
		}

		inline vec<T, size> normalize()
		{
			vec<T, size> r;
			T max = 0;
			for (std::size_t i = 0; i < size; ++i) max += data[i] * data[i];
			max = T(1) / sqrt(max);

			for (std::size_t i = 0; i < size; ++i)
				r[i] = max * data[i];

			return r;
		}

		inline void set(T data, std::size_t pos = 0)
		{
			data[pos] = data;
		}

		inline void clamp(T & min, T & max)
		{
			for (std::size_t i = 0; i < size; ++i)
			{
				if (data[i] > max) data[i] = max;
				if (data[i] < min) data[i] = min;
			}
		}
	};

#ifdef use_sse2
	template <std::size_t size>
	struct vec<__m128, size>
	{
		_align_
		__m128 data[size];

		template<typename Left, typename Op, typename Right >
		vec<__m128, size > (const X<__m128, Left, Op, Right, size> expression)
		{
			for (std::size_t i = 0; i < size; i++)
				data[i] = expression[i];
		}

		vec<__m128, size > ()
		{
			for (std::size_t i = 0; i < size; ++i)
			{
				data[i] = _mm_setzero_ps();
			}
		}

		vec<__m128, size > (__m128 & t1)
		{
			data[0] = t1;
		}

		inline vec(const __m128 * data_a)
		{
			for (std::size_t i = 0; i < size; ++i)
				data[i] = data_a[i];
		}

		std::size_t vec_size()
		{
			return size*4;
		}

		inline vec(const float * data_a)
		{
			for (std::size_t i = 0; i < size; ++i)
				data[i] = _mm_setr_ps(data_a[i*4+0], data_a[i*4+1], data_a[i*4+2], data_a[i*4+3]);
		}

		inline vec(float x, float y, float z, float w)
		{
			set(x, y, z, w);
		}

		inline const __m128 & operator[] (std::size_t i) const
		{
			return data[i];
		}

		inline float operator() (std::size_t i)
		{
			_align_
			float q[4];
			_mm_storeu_ps(q, data[i / 4]);
			return q[i % 4];
		}

		template<typename Left, typename Op, typename Right>
		inline void operator=(const X<__m128, Left, Op, Right, size>& expression)
		{
			for (std::size_t i = 0; i < size; ++i)
				data[i] = expression[i];
		}

		inline vec(const vec<__m128, size>& x)
		{
			for (std::size_t i = 0; i < size; ++i)
				data[i] = x.data[i];
		}

		inline __m128 & operator[] (std::size_t i)
		{
			return data[i];
		}

		template<typename Left, typename Op, typename Right>
		inline void operator+=(const X<__m128, Left, Op, Right, size>& expression)
		{
			for (std::size_t i = 0; i < size; ++i)
				data[i] += expression[i];
		}

		inline void zero()
		{
			for (std::size_t i = 0; i < size; ++i) data[i] = _mm_setzero_ps();
		}

		inline void one()
		{
			set(1.0f, 1.0f, 1.0f, 1.0f);
		}
		inline vec<__m128, size> normalize()
		{
			vec<__m128, size> r;
			__m128 a;

			float max = 0;

			for (std::size_t i = 0; i < size; ++i)
			{
				vec<__m128, size> tt(data[i]);
				max += Dot(tt, tt);
			}

			max = 1.0f / sqrt(max);
			a = _mm_set1_ps(max);

			for (std::size_t i = 0; i < size; ++i)
			{
				r[i] = _mm_mul_ps(data[i], a);
			}

			return r;
		}

		inline float * get()
		{
			return reinterpret_cast<float *>(&data);
		}


		inline void set(float x, float y, float z, float w, std::size_t pos = 0)
		{
			data[pos] = _mm_setr_ps(x, y, z, w);
		}

		inline void clamp(float & _min, float & _max)
		{
			__m128 min = _mm_set1_ps(_min);
			__m128 max = _mm_set1_ps(_max);

			for (std::size_t i = 0; i < size; ++i)
			{
				data[i] = _mm_max_ps(data[i], min);
				data[i] = _mm_min_ps(data[i], max);
			}
		}

		inline float x()
		{
			return operator ()(0);
		}

		inline float y()
		{
			return operator ()(1);
		}

		inline float z()
		{
			return operator ()(2);
		}

		inline float w()
		{
			return operator ()(3);
		}
	};

	typedef vec<__m128, 1> vec4;
#else
	typedef vec<float, 4> vec4;
#endif



//////////////////////////////////////////////////////////////

	template <typename T, std::size_t size>
	inline void Normalize(vec<T, size>& a)
	{
		a = a.normalize();
	}

	template <typename T, std::size_t size>
	inline T Dot(const vec<T, size> a, const vec<T, size> b)
	{
		T r = 0;

		for (std::size_t i = 0; i < size; ++i)
			r += (a[i] * b[i]);

		return r;
	}

#ifdef use_sse2
	template <std::size_t size>
	inline float Dot(const vec<__m128, size> a, const vec<__m128, size> b)
	{
		float r = 0.0f;
		float tmp;
		__m128 t;

		for (std::size_t i = 0; i < size; ++i)
		{
			t = _mm_mul_ps(a[i], b[i]);

			#ifdef use_sse3
				t = _mm_hadd_ps(t, t);
				t = _mm_hadd_ps(t, t);
				_mm_store_ss(&tmp, t);
			#else
				float p[4];
				_mm_storeu_ps(p, t);
				tmp = p[0] + p[1] + p[2] + p[3];
			#endif
				
			r += tmp;
		}

		return r;
	}
#endif

	template <typename T, typename Left, typename Op, typename Right, std::size_t size>
	inline float Dot(const vec<T, size> a, const X<T, Left, Op, Right, size> b)
	{
		return Dot(a, vec<T, size > (b));
	}

	template <typename T, typename Left, typename Op, typename Right, std::size_t size>
	inline float Dot(const X<T, Left, Op, Right, size> a, const vec<T, size> b)
	{
		return Dot(vec<T, size > (a), b);
	}

	template <typename T, typename Left, typename Op, typename Right, std::size_t size>
	inline float Dot(const X<T, Left, Op, Right, size> a, const X<T, Left, Op, Right, size> b)
	{
		return Dot(vec<T, size > (a), vec<T, size > (b));
	}

	template <typename T, std::size_t size>
	inline std::ostream & operator <<(std::ostream& out, const vec<T, size>& x)
	{
		for (std::size_t i = 0; i < size; ++i)
			out << x.data[i] << ' ';
		return out;
	}

	
#ifdef use_sse2
	template < std::size_t size>
	inline std::ostream & operator << (std::ostream& out, const vec<__m128, size>& x)
	{
		_align_ float q[4];

		for (std::size_t i = 0; i < size; ++i)
		{
			_mm_storeu_ps(q, x[i]);
			out << q[0] << ' ' << q[1] << ' ' << q[2] << ' ' << q[3] << ' ';
		}

		return out;
	}
#endif

	
	template <typename T, std::size_t size>
	inline float CalcDistance(const vec<T, size>& a, const vec<T, size>& b)
	{
		return sqrt(Dot(a - b, a - b));
	}

	template <typename T>
	inline vec<T, 3 > Cross(const vec<T, 3 > & v1, const vec<T, 3 > & v2)
	{
		vec<T, 3 > temp;

		temp[0] = v1[1] * v2[2] - v1[2] * v2[1];
		temp[1] = v1[2] * v2[0] - v1[0] * v2[2];
		temp[2] = v1[0] * v2[1] - v1[1] * v2[0];

		return temp;
	}


#ifdef use_sse2

	inline vec4 Cross(const vec4& v1, const vec4& v2)
	{
		__m128 a, b, c;

		a = _mm_shuffle_ps(v1[0], v1[0], 0xC9); /// 11 00 10 01
		b = _mm_shuffle_ps(v2[0], v2[0], 0xD2); /// 11 01 00 10
		c = _mm_mul_ps(a, b);

		a = _mm_shuffle_ps(v1[0], v1[0], 0xD2); /// 11 01 00 10
		b = _mm_shuffle_ps(v2[0], v2[0], 0xC9); /// 11 00 10 01
		a = _mm_mul_ps(a, b);

		c = _mm_sub_ps(c, a);

		return c;
	}

	inline std::size_t ToColor(vec4& color)
	{
		std::size_t r;

		__m128 a, b;

		b = color[0];
		a = _mm_set1_ps(255.0f);

		b = _mm_mul_ps(b, a);
		b = _mm_min_ps(b, a);

		float q[4];
		_mm_storeu_ps(q, b);

		r =		((std::size_t) q[0]*(1 << 16)) +
				((std::size_t) q[1]*(1 << 8)) +
				((std::size_t) q[2]);

		return r;

	}
#endif

	
///////////////////////////////
///////////////////////////////
///////////////////////////////


	template <typename T>
	struct plus
	{

		inline T apply(const T& a, const T & b) const
		{
			return a + b;
		}
	};

#ifdef use_sse2
	template <>
	struct plus<__m128>
	{

		inline __m128 apply(const __m128 & a, const __m128 & b) const
		{
			return _mm_add_ps(a, b);
		}
	};
#endif

	template <typename T>
	struct minus
	{

		inline T apply(const T& a, const T & b) const
		{
			return a - b;
		}
	};


#ifdef use_sse2
	template <>
	struct minus<__m128>
	{

		inline __m128 apply(const __m128& a, const __m128 & b) const
		{
			return _mm_sub_ps(a, b);
		}
	};
#endif

	template <typename T>
	struct mul
	{

		inline T apply(const T& a, const T & b) const
		{
			return a * b;
		}
	};


#ifdef use_sse2
	template <>
	struct mul<__m128>
	{

		inline __m128 apply(const __m128& a, const __m128 & b) const
		{
			return _mm_mul_ps(a, b);
		}
	};
#endif

	template <typename T>
	struct div
	{

		inline T apply(const T& a, const T & b) const
		{
			return a / b;
		}
	};


#ifdef use_sse2
	template <>
	struct div<__m128>
	{

		inline __m128 apply(const __m128& a, const __m128 & b) const
		{
			return _mm_div_ps(a, b);
		}
	};
#endif

////////////////////////////////////////////////////////////////////////////////////////////

	template <typename T, typename Left, typename Op, typename Right, std::size_t size>
	struct X
	{

		X(Left t1, Right t2) : lNode_(t1), rNode_(t2) { }

		T operator[] (std::size_t i) const
		{
			Op op_;
			return op_.apply(lNode_[i], rNode_[i]);
		}

	private:
		const Right rNode_;
		const Left lNode_;
	};

#ifdef use_sse2
	template <typename T, typename Left, typename Op, std::size_t size>
	struct X<T, Left, Op, float, size>
	{

		X(Left t1, float t2) : lNode_(t1), rNode_(t2) { }

		T operator[] (std::size_t i) const
		{
			Op op_;
			return op_.apply(lNode_[i], _mm_set1_ps(rNode_));
		}

	private:
		const float rNode_;
		const Left lNode_;
	};

	template <typename T, typename Op, typename Right, std::size_t size>
	struct X<T, float, Op, Right, size>
	{

		X(float t1, Right t2) : lNode_(t1), rNode_(t2) { }

		T operator[] (std::size_t i) const
		{
			Op op_;
			return op_.apply(_mm_set1_ps(lNode_), rNode_[i]);
		}

	private:
		const Right rNode_;
		const float lNode_;
	};
#endif

	template <typename T, typename Left, typename Op, std::size_t size>
	struct X <T, Left, Op, T, size>
	{

		X(Left t1, T t2) : lNode_(t1), rNode_(t2) { }

		T operator[] (std::size_t i) const
		{
			Op op_;
			return op_.apply(lNode_, rNode_);
		}

	private:
		const T rNode_;
		const Left lNode_;
	};

	template <typename T, typename Left, typename Op, std::size_t size>
	struct X <T, Left, Op, T*, size>
	{

		X(Left t1, T * t2) : lNode_(t1), rNode_(t2) { }

		T operator[] (std::size_t i) const
		{
			Op op_;
			return op_.apply(lNode_, rNode_[i]);
		}

	private:
		const T* rNode_;
		const Left lNode_;
	};

	template <typename T, typename Left, typename Op, std::size_t size>
	struct X <T, Left*, Op, T, size>
	{

		X(Left* t1, T t2) : lNode_(t1), rNode_(t2) { }

		T operator[] (std::size_t i) const
		{
			Op op_;
			return op_.apply(lNode_[i], rNode_);
		}

	private:
		const T rNode_;
		const Left* lNode_;
	};

	template <typename T, typename Left, typename Op, std::size_t size>
	struct X <T, Left*, Op, T*, size>
	{

		X(Left* t1, T * t2) : lNode_(t1), rNode_(t2) { }

		T operator[] (std::size_t i) const
		{
			Op op_;

			return op_.apply(lNode_[i], rNode_[i]);
		}

	private:
		const T* rNode_;
		const Left* lNode_;
	};

	template <typename T, typename Op, std::size_t size>
	struct X <T, T, Op, T, size>
	{

		X(T t1, T t2) : lNode_(t1), rNode_(t2) { }

		T operator[] (std::size_t i) const
		{
			Op op_;

			return op_.apply(lNode_, rNode_);
		}

	private:
		const T rNode_;
		const T lNode_;
	};

	template <typename T, typename Op, std::size_t size>
	struct X <T, T, Op, T*, size>
	{

		X(T t1, T * t2) : lNode_(t1), rNode_(t2) { }

		T operator[] (std::size_t i) const
		{
			Op op_;

			return op_.apply(lNode_, rNode_[i]);
		}

	private:
		const T* rNode_;
		const T lNode_;
	};

	template <typename T, typename Op, typename Right, std::size_t size>
	struct X <T, T, Op, Right, size>
	{

		X(T t1, Right t2) : lNode_(t1), rNode_(t2) { }

		T operator[] (std::size_t i) const
		{
			Op op_;

			return op_.apply(lNode_, rNode_);
		}

	private:
		const Right rNode_;
		const T lNode_;
	};

	template <typename T, typename Op, typename Right, std::size_t size>
	struct X <T, T, Op, Right*, size>
	{

		X(T t1, Right * t2) : lNode_(t1), rNode_(t2) { }

		T operator[] (std::size_t i) const
		{
			Op op_;


			return op_.apply(lNode_, rNode_[i]);
		}


	private:
		const Right* rNode_;
		const T lNode_;
	};

	template <typename T, typename Op, typename Right, std::size_t size>
	struct X <T, T*, Op, Right, size>
	{

		X(T* t1, Right t2) : lNode_(t1), rNode_(t2) { }

		T operator[] (std::size_t i) const
		{
			Op op_;

			return op_.apply(lNode_[i], rNode_);
		}

	private:
		const Right rNode_;
		const T* lNode_;
	};

	template <typename T, typename Op, typename Right, std::size_t size>
	struct X <T, T*, Op, Right*, size>
	{

		X(T* t1, Right * t2) : lNode_(t1), rNode_(t2) { }

		T operator[] (std::size_t i) const
		{
			Op op_;

			return op_.apply(lNode_[i], rNode_[i]);
		}

	private:
		const Right* rNode_;
		const T* lNode_;
	};

	template <typename T, typename Op, std::size_t size>
	struct X <T, T*, Op, T, size>
	{

		X(T* t1, T t2) : lNode_(t1), rNode_(t2) { }

		T operator[] (std::size_t i) const
		{
			Op op_;

			return op_.apply(lNode_[i], rNode_);
		}

	private:
		const T rNode_;
		const T* lNode_;
	};

	template <typename T, typename Op, std::size_t size>
	struct X <T, T*, Op, T*, size>
	{

		X(T* t1, T * t2) : lNode_(t1), rNode_(t2) { }

		T operator[] (std::size_t i) const
		{
			Op op_;

			return op_.apply(lNode_[i], rNode_[i]);
		}

	private:
		const T* rNode_;
		const T* lNode_;
	};

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //



#define OP_BASE(_OP_,_OP_Z)																					 \
                                                                                                             \
template<typename T, typename Left, typename Right, std::size_t size>                                        \
inline X<T, Left, _OP_, Right, size>  operator _OP_Z (const Left& a, const Right& b)                         \
{                                                                                                            \
                return X<T, Left, _OP_, Right, size>(a, b);                                                  \
}                                                                                                            \
                                                                                                             \
template<typename T, typename Left, std::size_t size>                                                        \
inline X<T, Left, _OP_, T, size>  operator _OP_Z (const Left& a, const T& b)                                 \
{                                                                                                            \
                return X<T, Left, _OP_, T, size>(a, b);                                                      \
}                                                                                                            \
                                                                                                             \
template<typename T, typename Left, std::size_t size>                                                        \
inline X<T, Left, _OP_, const T*, size>  operator _OP_Z (const Left& a, const vec<T,size>& b)				 \
{                                                                                                            \
                return X<T, Left, _OP_, const T*, size>(a, b.data);                                          \
}                                                                                                            \
                                                                                                             \
template<typename T, typename Right, std::size_t size>                                                       \
inline X<T, const T*, _OP_, Right, size> operator _OP_Z (const vec<T, size>& a, const Right& b)              \
{                                                                                                            \
                return X<T, const T*, _OP_, Right, size>(a.data, b);                                         \
}                                                                                                            \
                                                                                                             \
template<typename T, std::size_t size>                                                                       \
inline X<T, const T*, _OP_, T, size> operator _OP_Z (const vec<T,size>& a, const T& b)                       \
{                                                                                                            \
                return X<T, const T*, _OP_, T, size>(a.data, b);                                             \
}                                                                                                            \
                                                                                                             \
template<typename T, std::size_t size>                                                                       \
inline X<T, T, _OP_, const T*, size>  operator _OP_Z (const T& a, const vec<T,size>& b)                      \
{                                                                                                            \
    return X<T, T, _OP_, const T*, size>(a, b.data);														 \
}                                                                                                            \
                                                                                                             \
template<typename T, typename Right, std::size_t size>                                                       \
inline X<T, T, _OP_, Right, size>  operator _OP_Z (const T& a, const Right& b)                               \
{                                                                                                            \
                return X<T, T, _OP_, Right, size>(a, b);                                                     \
}                                                                                                            \
                                                                                                             \
template<typename T, std::size_t size>                                                                       \
inline X<T, T, _OP_, T, size>  operator _OP_Z (const T& a, const T& b)                                       \
{                                                                                                            \
                return X<T, T, _OP_, T, size>(a, b);                                                         \
}                                                                                                            \
                                                                                                             \
template<typename T, std::size_t size>                                                                       \
inline X<T, const T*, _OP_, const T*, size>                                                                  \
operator _OP_Z (const vec<T, size>& a, const vec<T,size>& b)                                                 \
{                                                                                                            \
                return X<T, const T*, _OP_, const T*, size>(a.data, b.data);                                 \
}                                                                                                            \
                                                                                                             \
template                                                                                                     \
        <                                                                                                    \
                typename T, typename Left1, typename Op1, typename Right1, std::size_t size,                 \
                typename Left2, typename Op2, typename Right2                                                \
        >                                                                                                    \
inline  X<T, X< T, Left1, Op1, Right1, size>, _OP_, X<T, Left2, Op2, Right2, size>, size>                    \
operator _OP_Z  (const  X<T, Left1, Op1, Right1, size>& a, const  X<T, Left2, Op2, Right2, size>& b)         \
{                                                                                                            \
                return X<T, X<T, Left1, Op1, Right1, size>, _OP_,                                            \
                X<T, Left2, Op2, Right2, size>, size>(a, b);                                                 \
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //


	OP_BASE(plus<T>, +)


	OP_BASE(minus<T>, -)


	OP_BASE(mul<T>, *)


	OP_BASE(div<T>, /)


///////////////////////////////
///////////////////////////////
///////////////////////////////



template <typename T, std::size_t M, std::size_t N = M>
class mat
{
private:
	// M = row
	// N = col

	_align_
	T d[M*N];

public:

	mat()
	{
		for (std::size_t i = 0; i < M * N; ++i) d[i] = T(0);
		//zero();
	}

	inline std::size_t size()
	{
		return M*N;
	}

	inline const T & operator[](std::size_t i) const
	{
		return d[i];
	}

	inline T & operator[](std::size_t i)
	{
		return d[i];
	}

	mat(const mat & m)
	{
		for (std::size_t i = 0; i < M * N; ++i) d[i] = m.d[i];
	}

	/*
	template <typename T1, std::size_t M1, std::size_t N1 = M1>
	void from (mat<T1, M1, N1> m, std::size_t x, std::size_t y)
	{
		//std::size_t h = 0;
		//std::cout << "\nx: " << x << " y: " << y;
		//std::cout << "\nM: " << M << "\nN: " << N;

		for (std::size_t i = 0; i < M; ++i)
			for (std::size_t j = 0; j < N; ++j) {
						std::cout << "\nd["<< i << "," << j << "]: " << d[i*N+j];
						std::cout << "\nI: " << (i+x)*N1+y;
						std::cout << "\nJ: " << (i+x)*N1+y+j;
						std::cout << "\nM(I,J): " << m((i+x)*N1+y,(i+x)*N1+y+j) << "\n";
				d[i * N + j] = m(i + x, j + y);
			}
	}
	*/

	inline void operator =(const mat<T, M, N> & cm)
	{
		for (std::size_t i = 0; i < M * N; ++i) d[i] = cm[i];
	}

	inline mat<T, M, N> operator * (mat<T, N, M> & b)
	{
		mat<T, M, N> t;

		for (std::size_t i = 0; i < M * N; i++)
			t.d[i] = dot<N, T > (d + i / N * M, b.d + i % N, 1, N);

		return t;
	}

	inline mat<T, M, N> operator * (T v)
	{
		mat<T, M, N> t;

		for (std::size_t i = 0; i < M * N; i++)
			t.d[i] = d[i] * v;

		return t;
	}


	inline mat<T, M, N> operator +(mat<T, N, M> & b)
	{
		mat<T, M, N> t;

		for (std::size_t i = 0; i < M * N; i++) t.d[i] = d[i] + b.d[i];

		return t;
	}

	inline mat<T, M, N> operator -(mat<T, N, M> & b)
	{
		mat<T, M, N> t;

		for (std::size_t i = 0; i < M * N; i++) t.d[i] = d[i] - b.d[i];

		return t;
	}

	inline mat<T, M, N> operator /(mat<T, N, M> & b)
	{
		mat<T, M, N> t;

		for (std::size_t i = 0; i < M * N; i++) t.d[i] = d[i] / b.d[i];

		return t;
	}


	// меняем местами 2 строки i и j

	void swap_row(std::size_t i, std::size_t j)
	{
		T tmp;
		for (std::size_t k = 0; k < N; ++k) {
			tmp = d[i * N + k];
			d[i * N + k] = d[j * N + k];
			d[j * N + k] = tmp;
		}
	}

	// умножаем строчку на число

	void mul_row(std::size_t str, T n)
	{
		for (std::size_t i = str * N; i < (str + 1) * N; ++i) {
			d[i] *= n;
		}
	}

	// вычитаем из i-ой строки j-ую, умноженную на число

	void mulsub_row(std::size_t i, std::size_t j, T n)
	{
		for (std::size_t k = 0; k < N; ++k) {
			d[i * N + k] -= (d[j * N + k] * n);
		}
	}

	// i - строка, j - столбец

	inline T & operator() (std::size_t i, std::size_t j)
	{
		//std::cout << "\noperator("<< i << ","<< j <<") (" << i*N+j << ") = " << d[i*N+j] << "\n";
		return d[i * N + j];
	}

	float * get(std::size_t line)
	{
		return d+line*N;
	}

	void zero()
	{
		for (std::size_t j = 0; j < N * M; ++j)
			d[j] = 0;
	}

	void one()
	{
		for (std::size_t j = 0; j < IFThenElse < M < N, N, M>::ret; ++j) d[j * N + j] = 1;
	}

	void transpose();

};

template <typename T, std::size_t M, std::size_t N>
void mat<T, M, N>::transpose()
{
	throw "It's not possible";
}

template <>
void mat<float, 4 > :: transpose()
{
	float tmp;

	for (std::size_t j = 0; j < 4; ++j)
		for (std::size_t i = j; i < 4; ++i) {
			tmp = d[i * 4 + j];
			d[i * 4 + j] = d[j * 4 + i];
			d[j * 4 + i] = tmp;
		}
}

template <typename T, std::size_t M, std::size_t N>
mat<T, M, N> Zero()
{
	mat<T, M, N> m;
	m.zero();
	return m;
}

template <typename T, std::size_t M, std::size_t N>
mat<T, N, M> Transpose(mat<T, M, N>& m)
{
	mat<T, N, M> ret;

	for (std::size_t j = 0; j < M; ++j)
		for (std::size_t i = 0; i < N; ++i) {
			ret(i, j) = m.operator ()(j, i);
		}

	return ret;
}

template <typename T>
T Determinant(const mat<T, 1, 1 > & m)
{
	return m[0];
}

template <typename T>
T Determinant(const mat<T, 2, 2 > & m)
{
	return m[0] * m[3] -
			m[1] * m[2];
}

template <typename T>
T Determinant(const mat<T, 3, 3 > & m)
{
	return m[0] * m[4] * m[8] -
			m[0] * m[5] * m[7] +
			m[1] * m[5] * m[6] -
			m[1] * m[3] * m[8] +
			m[2] * m[3] * m[7] -
			m[2] * m[4] * m[6];
}

template <typename T>
T Determinant(const mat<T, 4, 4 > & m)
{
	return m [0] * m [5] * m [10] * m [15] -
			m [0] * m [5] * m [14] * m [11] +
			m [0] * m [9] * m [14] * m [7] -
			m [0] * m [9] * m [6] * m [15] +
			m [0] * m [13] * m [6] * m [11] -
			m [0] * m [13] * m [10] * m [7] -
			m [4] * m [9] * m [14] * m [3] +
			m [4] * m [9] * m [2] * m [15] -
			m [4] * m [13] * m [2] * m [11] +
			m [4] * m [13] * m [10] * m [3] -
			m [4] * m [1] * m [10] * m [15] +
			m [4] * m [1] * m [14] * m [11] +
			m [8] * m [13] * m [2] * m [7] -
			m [8] * m [13] * m [6] * m [3] +
			m [8] * m [1] * m [6] * m [15] -
			m [8] * m [1] * m [14] * m [7] +
			m [8] * m [5] * m [14] * m [3] -
			m [8] * m [5] * m [2] * m [15] -
			m [12] * m [1] * m [6] * m [11] +
			m [12] * m [1] * m [10] * m [7] -
			m [12] * m [5] * m [10] * m [3] +
			m [12] * m [5] * m [2] * m [11] -
			m [12] * m [9] * m [2] * m [7] +
			m [12] * m [9] * m [6] * m [3];
}

template <typename T, std::size_t N>
T Determinant(mat<T, N> &m)
{

	// fixme: не всегда работает; выдает nan; нужна тщательная проверка.
	mat<T, N> L;
	mat<T, N> U;
	T sum = 0;

	for (std::size_t i = 0; i < N; i++) {
		U(0, i) = m(0, i);
		for (std::size_t j = 0; j < N; j++) {
			L(i, 0) = m(i, 0) / U(0, 0);
			sum = 0;
			for (std::size_t k = 0; k < i; k++) {
				sum += L(i, k) * U(k, j);
			}

			U(i, j) = m(i, j) - sum;

			if (i > j)
				L(j, i) = 0;
			else
			{
				sum = 0;

				for (std::size_t k = 0; k < i; k++) {
					sum += L(j, k) * U(k, i);
				}

				L(j, i) = (m(j, i) - sum) / U(i, i);
			}
		}
	}

	sum = U(0, 0);
	for (std::size_t i = 1; i < N; i++) sum *= U(i, i);

	//if (trans == true) sum = - sum;

	return sum;
}


template <typename T, std::size_t N>
mat<T, N> Inverse(mat<T, N>& m)
{
	mat<T, N, (2 * N) > e;
	e.zero();

	for (std::size_t i = 0; i < N; ++i)
		for (std::size_t j = 0; j < N; ++j)
			e(i, j) = m(i, j);


	for (std::size_t i = 0; i < N; ++i)
		e(0 + i, N + i) = 1;

	//std::cout << "\ne:\n" << e;

	std::size_t find;

	for (std::size_t h = 0; h < N; ++h)
	{
		if (e(h, h) == 0) {

			find = 0;
			for (std::size_t j = h; j < N; ++j) {
				if (e(j, h) != 0) {
					find = j;
					break;
				}
			}

			if (find != 0)
				e.swap_row(find, h);
		}

		e.mul_row(h, static_cast<T> (1) / e(h, h));

		for (std::size_t j = 0; j < N; ++j) {
			if (j != h) e.mulsub_row(j, h, e(j, h));
		}

		//std::cout << "\ne:\n" << e;
	}

	return mat<T, N > (e, 0, N);
}

template <typename T, std::size_t M, std::size_t N>
inline std::ostream & operator <<(std::ostream& out, const mat<T, M, N> & m)
{
	for (std::size_t i = 0; i < M; ++i) {
		for (std::size_t j = 0; j < N; ++j) {
			out.width(4);
			out << m[i * N + j] << " ";
		}
		out << "\n";
	}

	out << "\n";

	return out;
}


typedef mat<float,4> mat4;



vec4 operator * (mat4 m, vec4 v)
{
	vec4 ret;
	for (std::size_t i = 0; i<v.vec_size(); ++i)
	{
		#ifdef use_sse2
		float * tmp_v = v.get();
		float * tmp_m = m.get(i);
			ret[i] = dot<4,float>(tmp_m, tmp_v);
		#else
			ret[i] = dot<4,float>(&(m[i*4]),&(v[0]));
		#endif
	}
	return ret;
}


mat4 RotateX(float angle)
{
	mat4 a;
	a.one();

	float _cos = cosf(angle);
	float _sin = sinf(angle);

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

	float _cos = cosf(angle);
	float _sin = sinf(angle);

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

	float _cos = cosf(angle);
	float _sin = sinf(angle);

	a(0, 0) = _cos;
	a(0, 1) = _sin;
	a(1, 0) = -_sin;
	a(1, 1) = _cos;

	return a;
}



///////////////////////////////
///////////////////////////////
///////////////////////////////

	#undef use_sse2
	#undef use_sse3

}

#endif
