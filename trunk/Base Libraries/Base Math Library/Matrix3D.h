#pragma once

#include "matrices.h"

class Matrix3D
{
public:

	//---------------------------------- Public Fields ------------------------------------

	mat3 matrix;
	
	//--------------------------------- Public Constants ----------------------------------

	static Matrix3D Zero;
	static Matrix3D Unit;


	Matrix3D( void );
	~Matrix3D( void );

	Matrix3D( const mat3 in_matrix );

	Matrix3D operator + ( Matrix3D in_matrix );
	Matrix3D operator - ( Matrix3D in_matrix );
};
