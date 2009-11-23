
#include <iostream>

#include "matrix.hpp"

using namespace std;

inline unsigned long long __rdtsc__()
{
	unsigned long long result;
	__asm__ volatile ("rdtsc" : "=A"(result));
	return result;
}

typedef Matrix<float, 4, 4> mat;

int main(int argc, char ** argv)
{
	unsigned long long t0, t1;

/*/
	const int size = 4;
	float a[size];
	float b[size];
	float r;

	for (int i=0; i<size; ++i)
		a[i] = static_cast<float>(i);

	for (int i=0; i<size; ++i)
		b[i] = static_cast<float>(i);

	t0 = __rdtsc__();

	r = dot<size/2>(a+2,b+1,1,2);

	t1 = __rdtsc__();

	cout << r;
//*/

	//////////////////////////
//*/
	mat ma, mb, mc, md;
	unsigned long long min = 10000000;

	for (unsigned long long i=0; i<10000; i++)
	{

		t0 = __rdtsc__();

		mc = ma * mb + md;
//		ma = md + mb;
//		mc = ma * md;
//		mb = md * md;

		t1 = __rdtsc__();

		if (min>(t1-t0)) min = t1 -t0;
	}

	cout << "\nma:\n" << ma << "\nmb:\n" << mb << "\nmd:\n" << md << "\nmc:\n" << mc;
//*/

/*
	t0 = __rdtsc__();


	t1 = __rdtsc__();
*/

	cout << "\ntime:" << min   << "\n";
//	cout << "\ntime: "<< t1-t0 << "\n";

	return 0;
}

