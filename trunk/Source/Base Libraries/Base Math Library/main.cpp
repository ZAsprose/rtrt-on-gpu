
#include <iostream>
#include <stdlib.h>

#include "vector.hpp"
#include "matrix.hpp"

using namespace std;

inline unsigned long long __rdtsc__()
{
	unsigned long long result;
	__asm__ volatile ("rdtsc" : "=A"(result));
	return result;
}

typedef float T;

typedef Matrix<T, 3> mat3;

mat4 ma, mb, mc, md;
unsigned long long t0, t1;
unsigned long long minimum;

void init()
{
	minimum = 100000000;
	const int size = 4;
	float a[size];
	float b[size];
	float r;

	for (int i=0; i<size; ++i)
		a[i] = static_cast<float>(i);

	for (int i=0; i<size; ++i)
		b[i] = static_cast<float>(i);
}

void test01()
{
	init();

	for (unsigned long long i=0; i<1000; i++)
	{

		t0 = __rdtsc__();

		mc = ma * mb + md;
		ma = md + mb;
		mc = ma * md;
		mb = md * md;

		t1 = __rdtsc__();

		if (minimum>(t1-t0)) minimum = t1 -t0;
	}

	cout << "\n\ntest01:" << "\nma:\n" << ma << "\nmb:\n" << mb << "\nmd:\n" << md << "\nmc:\n" << mc << "\nmin: " << minimum;
}

void test02()
{
	init();
	cout << "\n\ntest02:";

	try
	{
		cout << "\nma:\n" << ma;
		cout << "\nTranspose(ma):\n" << Transpose(ma);
		ma.transpose();
		cout << "\nma.transpose():\n" << ma;
	}
	catch (const char* mess)
	{
		std::cerr << mess;
	}
}

void test03()
{
	init();

	float r;

	smath::vec4 v(1,1,1,1);

	for (unsigned long long i=0; i<1000; i++)
	{

		t0 = __rdtsc__();

		r = smath::Dot(v,v);

		t1 = __rdtsc__();

		if (minimum>(t1-t0)) minimum = t1 -t0;
	}

	cout << "\n\ntest03: v: " << v << " dot(v,v): " << r << " min:" << minimum;

}

void test04()
{
	init();

	mat3 mat;

	mat.zero();
	mat(0,0) = 1; mat(0,1) = 1; mat(0,2) = 1;
	mat(1,0) = 0; mat(1,1) = 2; mat(1,2) = 3;
	mat(2,0) = 5; mat(2,1) = 5; mat(2,2) = 1;

	cout << "\n\ntest04:\nmat:\n" << mat;
	cout << "\nInv(mat):\n" << Inverse(mat);

	mat4 m4;
	m4.zero();

	m4(0,0) = m4(1,1) = m4(2,2) = m4(3,3) = 4;

	cout << "mat4x4:\n" << m4;
	cout << "\nInv(m4):\n" << Inverse(m4);

}

void test05()
{
	init();

	float r;

	mat4 mat;

	mat.one();

	mat = mat + mat;

	for (unsigned long long i=0; i<1000; i++)
	{

		t0 = __rdtsc__();

		r = Determinant(mat);

		t1 = __rdtsc__();

		if (minimum>(t1-t0)) minimum = t1 -t0;
	}

	cout << "\n\ntest05: mat:\n" << mat << " det(mat): " << r << " min:" << minimum;

}

int main(int argc, char ** argv)
{
	srand((unsigned)time(NULL));

	test01();
	test02();
	test03();
	test04();
	test05();

	cout << "\n";
	return 0;
}

