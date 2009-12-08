
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

template <bool b, std::size_t i, std::size_t j>
class IFThenElse{};

template <std::size_t i, std::size_t j>
class IFThenElse<true, i, j>
{
public:
	enum {ret = i};
};

template <std::size_t i, std::size_t j>
class IFThenElse<false, i, j>
{
public:
	enum {ret = j};
};

///////////////////////////////////////////////////////////////////////////////

template <std::size_t dim, typename T>
class DotProduct
{
public:
	static inline T result (T*a, T*b, std::size_t da = 1, std::size_t db = 1)
	{
		const int res = IFThenElse< dim >= 4, 4, 1>::ret;
		return  DotProduct<      res, T>::result(a, b, da, db) +
			DotProduct<dim - res, T>::result(a+res*da, b+res*db, da, db);
		//return (*a) * (*b) + DotProduct<dim-1,T>::result(a+da,b+db, da, db);
	}
};


template <>
class DotProduct<4, float>
{
public:
	static inline float result (float *a, float *b, std::size_t da, std::size_t db)
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
	static inline T result (T*a, T*b, std::size_t da, std::size_t db)
	{
		return (*a)*(*b);
	}
};


template <std::size_t dim, typename T>
inline T dot ( T * a, T * b, std::size_t da = 1, std::size_t db = 1)
{
	return DotProduct<dim,T>::result(a, b, da, db);
}


template <typename T, std::size_t M, std::size_t N = M>
class Matrix
{

private:
	// M = row
	// N = col

	_align_
	T d[M*N];

public:

	Matrix()
	{
		for (std::size_t i=0; i<M*N; ++i) d[i] = T(i);
		//zero();
	}

	inline std::size_t size()
	{
		return M*N;
	}

	inline const T& operator[](std::size_t i) const
	{
		return d[i];
	}

	inline T& operator[](std::size_t i)
	{
		return d[i];
	}

	Matrix( const Matrix & m)
	{
		for (std::size_t i=0; i<M*N; ++i) d[i] = m.d[i];
	}

	template <typename T1, std::size_t M1, std::size_t N1 = M1>
	Matrix( Matrix<T1,M1,N1> m , std::size_t x, std::size_t y)
	{
		//std::size_t h = 0;
		//std::cout << "\nx: " << x << " y: " << y;
		//std::cout << "\nM: " << M << "\nN: " << N;

		for (std::size_t i = 0; i<M; ++i)
			for (std::size_t j = 0; j<N; ++j)
			{
			/*
				std::cout << "\nd["<< i << "," << j << "]: " << d[i*N+j];
				std::cout << "\nI: " << (i+x)*N1+y;
				std::cout << "\nJ: " << (i+x)*N1+y+j;
				std::cout << "\nM(I,J): " << m((i+x)*N1+y,(i+x)*N1+y+j) << "\n";
			*/
				d[i*N+j] = m(i+x,j+y);
			}
	}

	inline void operator = (const Matrix<T,M,N> & cm)
	{
		for (std::size_t i=0; i<M*N; ++i) d[i] = cm[i];
	}

	inline Matrix<T, M, N> operator * (Matrix<T, N, M> & b)
	{
		Matrix<T, M, N> t;

		for (std::size_t i=0; i<M*N; i++)
			t.d[i] = dot<N, T>(d + i/N*M, b.d + i%N, 1, N);

		return t;
	}

	inline Matrix<T, M, N> operator + (Matrix<T, N, M> & b)
	{
		Matrix<T, M, N> t;

		for (std::size_t i=0; i<M*N; i++) t.d[i] = d[i] + b.d[i];

		return t;
	}

	inline Matrix<T, M, N> operator - (Matrix<T, N, M> & b)
	{
		Matrix<T, M, N> t;

		for (std::size_t i=0; i<M*N; i++) t.d[i] = d[i] - b.d[i];

		return t;
	}

	inline Matrix<T, M, N> operator / (Matrix<T, N, M> & b)
	{
		Matrix<T, M, N> t;

		for (std::size_t i=0; i<M*N; i++) t.d[i] = d[i] / b.d[i];

		return t;
	}

	// меняем местами 2 строки i и j
	void swap_row ( std::size_t i, std::size_t j )
	{
		T tmp;
		for (std::size_t k = 0; k < N; ++k)
		{
			tmp = d[i*N+k];
			d[i*N+k] = d[j*N+k];
			d[j*N+k] = tmp;
		}
	}

	// умножаем строчку на число
	void mul_row ( std::size_t str, T n)
	{
		for (std::size_t i = str*N; i < (str+1)*N ; ++i)
		{
			d[i] *= n;
		}
	}

	// вычитаем из i-ой строки j-ую, умноженную на число
	void mulsub_row ( std::size_t i, std::size_t j, T n)
	{
		for (std::size_t k = 0; k < N ; ++k)
		{
			d[i*N+k] -= (d[j*N+k]*n);
		}
	}

	// i - строка, j - столбец
	inline T& operator() ( std::size_t i, std::size_t j )
	{
		//std::cout << "\noperator("<< i << ","<< j <<") (" << i*N+j << ") = " << d[i*N+j] << "\n";
		return d[i*N+j];
	}

	void zero()
	{
		for (std::size_t j = 0; j < N*M; ++j) d[j] = 0;
	}

	void one()
	{
		for (std::size_t j = 0; j < IFThenElse< M < N, N , M>::ret; ++j) d[j*N+j] = 1;
	}

	void transpose();

};


template <typename T, std::size_t M, std::size_t N>
void Matrix<T,M,N>::transpose()
{
	throw "It's not possible";
}


void Matrix<float, 4>::transpose()
{
	float tmp;

	for (std::size_t j = 0; j < 4; ++j)
		for (std::size_t i = j; i < 4; ++i)
		{
			tmp = d[i*4+j];
			d[i*4+j] = d[j*4+i];
			d[j*4+i] = tmp;
		}
}


template <typename T, std::size_t M, std::size_t N>
Matrix<T, M, N> Zero()
{
	Matrix<T, M, N> m;
	m.zero();
	return m;
}


template <typename T, std::size_t M, std::size_t N>
Matrix<T, N, M> Transpose(Matrix<T, M, N>& m)
{
	Matrix<T,N,M> ret;

	for (std::size_t j = 0; j < M; ++j)
		for (std::size_t i = 0; i < N; ++i)
		{
			ret(i,j) = m.operator ()(j,i);
		}

	return ret;
}


template <typename T>
T Determinant(const Matrix<T,1,1> & m)
{
	return m[0];
}


template <typename T>
T Determinant(const Matrix<T,2,2> & m)
{
	return  m[0]*m[3] -
		m[1]*m[2];
}


template <typename T>
T Determinant(const Matrix<T,3,3> & m)
{
	return	m[0]*m[4]*m[8] -
		m[0]*m[5]*m[7] +
		m[1]*m[5]*m[6] -
		m[1]*m[3]*m[8] +
		m[2]*m[3]*m[7] -
		m[2]*m[4]*m[6];
}


template <typename T>
T Determinant(const Matrix<T,4,4> & m)
{
	return     m [0]  * m [5]  * m [10] * m [15] -
		   m [0]  * m [5]  * m [14] * m [11] +
		   m [0]  * m [9]  * m [14] * m [7]  -
		   m [0]  * m [9]  * m [6]  * m [15] +
		   m [0]  * m [13] * m [6]  * m [11] -
		   m [0]  * m [13] * m [10] * m [7]  -
		   m [4]  * m [9]  * m [14] * m [3]  +
		   m [4]  * m [9]  * m [2]  * m [15] -
		   m [4]  * m [13] * m [2]  * m [11] +
		   m [4]  * m [13] * m [10] * m [3]  -
		   m [4]  * m [1]  * m [10] * m [15] +
		   m [4]  * m [1]  * m [14] * m [11] +
		   m [8]  * m [13] * m [2]  * m [7]  -
		   m [8]  * m [13] * m [6]  * m [3]  +
		   m [8]  * m [1]  * m [6]  * m [15] -
		   m [8]  * m [1]  * m [14] * m [7]  +
		   m [8]  * m [5]  * m [14] * m [3]  -
		   m [8]  * m [5]  * m [2]  * m [15] -
		   m [12] * m [1]  * m [6]  * m [11] +
		   m [12] * m [1]  * m [10] * m [7]  -
		   m [12] * m [5]  * m [10] * m [3]  +
		   m [12] * m [5]  * m [2]  * m [11] -
		   m [12] * m [9]  * m [2]  * m [7]  +
		   m [12] * m [9]  * m [6]  * m [3];
}


template <typename T, std::size_t N>
T Determinant(Matrix<T, N> &m)
{

	// fixme: не всегда работает; выдает nan; нужна тщательная проверка.
/*/
	int find = -1;
	bool trans = false;

	for (std::size_t j = 0; j < N; ++j)
	{
		if (m(j,j) == 0)
		{
			// найти первый не нулевой и поменять местами строчки
			for (std::size_t i = 1; i < N; ++i)
				if (m(i,j) != 0)
				{
					find = i;
					break;
				}

			if (find == -1) throw "bad matrix";

			trans = !trans;
			m.swap_row(j, find);
		}
	}
//*/
	Matrix<T,N> L;
	Matrix<T,N> U;
	T sum = 0;

	for (std::size_t i = 0; i < N; i++)
	{
		U(0, i) = m(0, i);
		for (std::size_t j = 0; j < N; j++)
		{
			L(i, 0) = m(i, 0) / U(0, 0);
			sum = 0;
			for (std::size_t k = 0; k < i; k++)
			{
				sum += L(i, k) * U(k, j);
			}

			U(i, j) = m(i, j) - sum;

			if (i > j)
				L(j, i) = 0;
			else
			{
				sum = 0;

				for (std::size_t k = 0; k < i; k++)
				{
					sum += L(j, k) * U(k, i);
				}

				L(j, i) = (m(j, i) - sum) / U(i, i);
			}
		}
	}

	sum = U(0,0);
	for (std::size_t i = 1; i < N; i++) sum *= U(i,i);

	//if (trans == true) sum = - sum;

	return sum;
}

// обратная матрица
template <typename T, std::size_t N>
Matrix<T, N> Inverse (Matrix<T, N>& m)
{
	Matrix<T,N,(2*N)> e;
	e.zero();

	for (std::size_t i = 0; i<N; ++i)
		for (std::size_t j = 0; j<N; ++j)
			e(i,j) = m(i,j);

	//std::cout << "\ne:\n" << e;
	//std::cout << "\ne.size(): " << e.size() << "\n";
	//std::cout <<   "m.size(): " << m.size() << "\n";

	for (std::size_t i = 0; i<N; ++i)
		e(0+i,N+i) = 1;

	//std::cout << "\ne:\n" << e;

	std::size_t find;

	for (std::size_t h = 0; h<N; ++h )
	{
		if ( e(h,h) == 0)
		{
			// ищем первый не ноль
			find = 0;
			for (std::size_t j = h; j<N; ++j)
			{
				if (e(j,h)!=0)
				{
					find = j;
					break;
				}
			}

			if (find != 0)
				e.swap_row(find, h);
		}

		// теперь делим
		e.mul_row(h, static_cast<T>(1)/e(h,h));

		for (std::size_t j = 0; j<N; ++j)
		{
			if (j!=h) e.mulsub_row(j,h,e(j,h));
		}

		//std::cout << "\ne:\n" << e;
	}

	return Matrix<T,N>(e,0,N);
}


template <typename T, std::size_t M, std::size_t N>
inline std::ostream& operator << (std::ostream& out, const Matrix<T, M, N> & m)
{
	//out << "M:N = " << M << ":" << N << "\n";

	for (std::size_t i = 0; i<M; ++i)
	{
		for (std::size_t j = 0; j<N; ++j)
		{
			out.width(4);
			out << m[i*N + j] << " ";
		}

		out << "\n";
	}

	out << "\n";

	return out;
}

