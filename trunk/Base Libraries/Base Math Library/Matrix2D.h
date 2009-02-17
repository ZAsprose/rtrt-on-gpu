#pragma once

#include "matrices.h"

class Matrix2D
{
public:

	//---------------------------------- Public Fields ------------------------------------

	mat2 matrix;
	
	//--------------------------------- Public Constants ----------------------------------

	static Matrix2D Zero;
	static Matrix2D Unit;


	Matrix2D( void );
	~Matrix2D( void );

	Matrix2D( const mat2 in_matrix );

	Matrix2D operator + ( Matrix2D in_matrix );
	Matrix2D operator - ( Matrix2D in_matrix );
};
