#include <iostream>

#include "vector.hpp"
#include "nvec.hpp"

#include <malloc.h>

using namespace std;
using namespace smath;


#ifdef WIN32

	inline unsigned long long __rdtsc__()
	{
		__asm
		{
			RDTSC
		}
	}

#else

	inline unsigned long long  __rdtsc__()
	{
		unsigned long long result;
                __asm__ volatile ("rdtsc" : "=A"(result));
		return result;
	}

#endif

int main(int argc, char ** argv)
{
	unsigned long long tb,te;

#define _test1_

#ifdef _test1_
	Vector4 a,b,c,d,e,f,g,h,z;

	a.set(1,1,1,1);
	b.set(1,1,1,1);
	c.set(1,1,1,1);
	d.set(1,1,1,1);
	e.set(1,1,1,1);
	f.set(1,1,1,1);
	g.set(1,1,1,1);
	h.set(1,1,1,1);

	//cout << "\nexpression templates\n";

#else
	float v[4] = {1,1,1,1};

	vector<float> a(4,v);
	vector<float> b(4,v);
	vector<float> c(4,v);
	vector<float> d(4,v);
	vector<float> e(4,v);
	vector<float> f(4,v);
	vector<float> g(4,v);
	vector<float> h(4,v);
	vector<float> z(4,v);

	//cout << "\nNO expression templates\n";

#endif

	float test = 0.0;

	tb = __rdtsc__();

	test = 2.0 * dot( a*dot(c,c + 0.5f * c), b * dot(d, d*dot(h,h)) );

	te = __rdtsc__();

	cout << "\ntime: " << te-tb;
	cout << "\nresult: " << test;
	cout << "\n";

	return 0;
}
