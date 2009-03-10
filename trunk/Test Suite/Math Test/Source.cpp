#include <Vector2D.h>

#include <Vector3D.h>

#include <Vector4D.h>

#include <Matrix2D.h>

#include <Matrix3D.h>

#include <Matrix4D.h>

using namespace Math;

void main ( void )
{
	float v1[] = { 1, 2, 3 };

	float * v2 = new float[3];

	v2[0] = 1; v2[1] = 2; v2[2] = 3;

	Vector3D a ( v1 );

	Vector3D b ( v2 );

	Vector3D c = a + b;
}