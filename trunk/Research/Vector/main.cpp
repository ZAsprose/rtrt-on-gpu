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

	//cout << "\nz = " << z << "\n";
/*
	b.set(3,3,3,3);
	z.Zero();

	c = (a + b) * (b - a);

	float d = dot(c,c);

	float dist = CalcDistance(c, z);
	z = c.Normalize();

	Normalize(b);
	col.set(0,1,0,0);

	int s = 16;

	__m128 * pp = new __m128[s];

	Vector4 * Pv;
	
	Pv = new_memory<Vector4>(s);

	Pv[0].set(0,0,0,0);
	Pv[1].set(1,0,0,0);
	Pv[2].set(0,1,0,0);

	Pv[0] = cross(Pv[1], Pv[2]);

	cout << "\nPv[0] = " << Pv[0];
	cout << "\nPv[1] = " << Pv[1];
	cout << "\nPv[2] = " << Pv[2];

	for(i=0; i<s; i++)
	{
		j = (float)i;
		Pv[i].set(j,j,j,j);
	}

	for(i=0; i<s; i++)
		cout << "\n[" << i << "] = " << Pv[i];

	for(i=0; i<s; i++)
		Pv[i] = Pv[0] + Pv[s-1] + Pv[s/2];

	for(i=0; i<s; i++)
		cout << "\n[" << i << "] = " << Pv[i];

	del_memory(Pv);

	cout << "\ncol = " << col;
	cout << "\na = " << a;
	cout << "\nb = " << b;
	cout << "\n\nc = " << c;
	cout << "\nc.Normalize() = " << z;
	cout << "\ndot(a,b) = " << d;
	cout << "\ndist(c,a) = " << dist;
	cout << "\nToColor(col) = " << ToColor(col);

	s = 6;

	Vector4 ** p;

	p = new_memory<Vector4*>(s);

	for (i = 0; i<s; i++)
		p[i] = new_memory<Vector4>(s);

	for(i=0; i<s; i++)
		for(j=0; j<s; j++)
		{
			p[i][j].set(i,j,0,0);
		}

	cout << "\n";

	for(i=0; i<s; i++)
	{
		for(j=0; j<s; j++)
		{
			cout << " [ " << p[i][j] << "] ";
		}
		cout << "\n";
	}
	
	for (i=0; i<s; i++)
		del_memory(p[i]);
	del_memory(p);

	cout << "\nend tests\n";
*/
	return 0;
}
