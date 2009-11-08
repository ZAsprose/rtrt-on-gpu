#include <iostream>
#include <malloc.h>
#include <string>



#include "vector.hpp"
#include "nvec.hpp"


using namespace std;
using namespace smath;

//#define _test1_
#ifdef  _test1_

        Vector4 a(1,1,1,1),
                b(1,1,1,1),
                c(1,1,1,1),
                d(1,1,1,1),
                e(1,1,1,1),
                f(1,1,1,1),
                g(1,1,1,1),
                h(1,1,1,1),
                z(1,1,1,1);

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

#endif
        
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
        

std::string trytest(bool t)
{
    return (t)?"passed":"failed";
}

void test_all()
{
    unsigned long long tb, te;
    float test = 0.0;

    cout << "test : ";
    tb = __rdtsc__();
    test = dot(c, c);
    te = __rdtsc__();
    cout << "result: " << test << "  ...  " << trytest( test == 4 ) << "\t\t\ttime:" << te-tb << endl;


    cout << "test : ";
    tb = __rdtsc__();
    test = 2.0 * dot(c, c);
    te = __rdtsc__();
    cout << "result: " << test << "  ...  " << trytest( test == 8 ) << "\t\t\ttime:" << te-tb << endl;

    cout << "test : ";
    tb = __rdtsc__();
    test = 2.0 * dot(c, c+b);
    te = __rdtsc__();
    cout << "result: " << test << "  ...  " << trytest( test == 16 ) << "\t\t\ttime:" << te-tb << endl;

    cout << "test : ";
    tb = __rdtsc__();
    test = 2.0 * dot(c+b, c+b);
    te = __rdtsc__();
    cout << "result: " << test << "  ...  " << trytest( test == 32 ) << "\t\t\ttime:" << te-tb << endl;

    cout << "test : ";
    tb = __rdtsc__();
    test = 2.0 * dot(c + 2.0f * b, c);
    te = __rdtsc__();
    cout << "result: " << test << "  ...  " << trytest( test == 24 ) << "\t\t\ttime:" << te-tb << endl;


    cout << "test : ";
    tb = __rdtsc__();
    test = 2.0 * dot(c - 2.0f * b , b - 3.0f * c);
    te = __rdtsc__();
    cout << "result: " << test << "  ...  " << trytest( test == 16 ) << "\t\t\ttime:" << te-tb << endl;


    cout << "test : ";
    tb = __rdtsc__();
    test = 2.0 * dot(c , b*(4.0f-1.0f)) - 1.0f;
    te = __rdtsc__();
    cout << "result: " << test << "  ...  " << trytest( test == 23 ) << "\t\t\ttime:" << te-tb << endl;


    cout << "test : ";
    tb = __rdtsc__();
    test = 2.0 * dot(c * (3.0f - 2.0f) , b*(4.0f-1.0f)) - 1.0f;
    te = __rdtsc__();
    cout << "result: " << test << "  ...  " << trytest( test == 23 ) << "\t\t\ttime:" << te-tb << endl;



    cout << "test : ";
    tb = __rdtsc__();
    test = 2.0 * dot(c , b*(4.0f*2.0f - 6.0f+1.0f))*dot(a+c,d+b);
    te = __rdtsc__();
    cout << "result: " << test << "  ...  " << trytest( test == 384 ) << "\t\t\ttime:" << te-tb << endl;

}


int main(int argc, char ** argv)
{
        test_all();

	cout << "\n";

	return 0;
}
