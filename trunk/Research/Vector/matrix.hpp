
#include <ostream>
#include <mmintrin.h>
#include <xmmintrin.h>

//#define use_sse3
#ifdef use_sse3
	#include <pmmintrin.h>
#endif

#ifndef WIN32
	#define _align_ __attribute__((aligned(16)))
#else
	#define _align_ __declspec(align(16))
#endif


///////////////////////////////////////////////////////////////////////////////

template <bool b, int i, int j>
class Sdvig{};

template <int i, int j>
class Sdvig<true, i, j>
{
public:
	enum {ret = i};
};

template <int i, int j>
class Sdvig<false, i, j>
{
public:
	enum {ret = j};
};

///////////////////////////////////////////////////////////////////////////////

template <int dim, typename T>
class DotProduct
{
public:
	static inline T result (T*a, T*b, int da = 1, int db = 1)
	{
		const int res = Sdvig< dim >= 4, 4, 1>::ret;
		return  DotProduct<      res, T>::result(a, b, da, db) +
			DotProduct<dim - res, T>::result(a+res*da, b+res*db, da, db);
		//return (*a) * (*b) + DotProduct<dim-1,T>::result(a+da,b+db, da, db);
	}
};


template <>
class DotProduct<4, float>
{
public:
	static inline float result (float *a, float *b, int da, int db)
	{
		_align_
		__m128 m1,m2,m3,m4;
		float res;

		m4 = _mm_setzero_ps();

		m1 = _mm_load_ps(a);
		m2 = _mm_setr_ps(b[0],b[db],b[2*db],b[3*db]);

		m3 = _mm_mul_ps(m1,m2);
#ifdef use_sse3
		m3 = _mm_hadd_ps(m3,m3);
		m4 = _mm_hadd_ps(m3,m3);
		_mm_store_ss(&res, m4);
#else
		_align_
		float p[4];
		_mm_storeu_ps(p,m3);
		res = p[0] + p[1] + p[2] + p[3];
#endif

		return res;
	}
};

template <typename T>
class DotProduct<1,T>
{
public:
	static inline T result (T*a, T*b, int da, int db)
	{
		return (*a)*(*b);
	}
};

template <int dim, typename T>
inline T dot ( T * a, T * b, int da = 1, int db = 1)
{
	return DotProduct<dim,T>::result(a, b, da, db);
}




template <typename T, int M, int N>
class Matrix
{

	// M = row
	// N = col

	_align_
	T d[M*N];

public:

	Matrix()
	{
		for (int i=0; i<M*N; ++i) d[i] = T(i);
	}

	inline const T& operator[](std::size_t i) const
	{
		return d[i];
	}


	Matrix( const Matrix & m)
	{
		for (int i=0; i<M*N; ++i) d[i] = m.d[i];
	}

	inline void operator = (const Matrix<T,M,N> & cm)
	{
		for (int i=0; i<M*N; ++i) d[i] = cm[i];
	}


	inline Matrix<T, M, N> operator * (Matrix<T, N, M> & b)
	{
		Matrix<T, M, N> t;

		for (int i=0; i<M*N; i++)
			t.d[i] = dot<N, T>(d + i/N*M, b.d + i%N, 1, N);

		return t;
	}

	inline Matrix<T, M, N> operator + (Matrix<T, N, M> & b)
	{
		Matrix<T, M, N> t;

		for (int i=0; i<M*N; i++) t.d[i] = d[i] + b.d[i];

		return t;
	}

	inline Matrix<T, M, N> operator - (Matrix<T, N, M> & b)
	{
		Matrix<T, M, N> t;

		for (int i=0; i<M*N; i++) t.d[i] = d[i] - b.d[i];

		return t;
	}

	inline Matrix<T, M, N> operator / (Matrix<T, N, M> & b)
	{
		Matrix<T, M, N> t;

		for (int i=0; i<M*N; i++) t.d[i] = d[i] / b.d[i];

		return t;
	}




};


template <typename T, int M, int N>
inline std::ostream& operator << (std::ostream& out, const Matrix<T, M, N> & m)
{
	for (int j = 0; j<M; ++j)
	{
		for (int i = 0; i<N; ++i)
		{
			out.width(3);
			out << m[j*M + i] << " ";
		}

		out << "\n";
	}

	out << "\n";

	return out;
}

