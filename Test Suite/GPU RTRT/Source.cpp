#include <iostream>

//#include "Util.h"

//#include "Vector2D.h"
#include "Matrix2D.h"

using namespace std;

/*
int main()
{
    double a = 7.4;
    float b = 99;

	float fff = EPSILON;

	b = Math::Clamp(b, 0, 1);

	cout << b;

	int aa = 1, bb = 2, cc = 0;

	cc = aa += bb;

	Math::Vector2D v = Math::Vector2D(1,1);

	Math::Vector2D vvv = v + v;

	getchar();

    return 0;
}*/

void main( void )
{
	Matrix2D a = Matrix2D::Unit;
	Matrix2D b = Matrix2D::Unit;

	Matrix2D c = a + b;
	Matrix2D d = a - b;
}