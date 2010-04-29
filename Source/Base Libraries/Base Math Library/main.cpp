/* 
 * File:   main.cpp
 * Author: andrey
 *
 * Created on 23 Январь 2010 г., 22:47
 */

#include <stdlib.h>
#include <iostream>

#include "fastmv.hpp"

using namespace std;
using namespace fastmath;


//mat4 m;

/*
void test01()
{
	vec4 v(1,1,1,0);
	vec4 v2(v);
	v2 = 8.0f * v - 2.0f * v2;
	cout << v2 << "\n";
}
/*
void test02()
{	
	m.zero ();
}
*/

void test01 ()
{
	vec4 v0;
	vec4 v1;
	v0.one ();

	v1 = 3.0f * v0;
	cout << v1 << endl;
}

void test02 ()
{
	mat4 m;
	m.one ();
	m = m * 4.0f;
	cout << m << endl;
}

void test03 ()
{
	vec4 v0;
	vec4 v1;
	v0.one ();
	v0 = 2.0f * v0;
	cout << "vec:\n" << v0;
	mat4 m;
	m.one ();
	m = m * 3.0f; m(2,1) = 2.0f;
	cout << "\nmat:\n" << m;
	v1 = m * v0;
	cout << "result: mat * vec = " << v1 << endl;
}

int main ( int argc, char** argv )
{
	test01 ();

	test02 ();

	test03 ();

	return 0;
}

