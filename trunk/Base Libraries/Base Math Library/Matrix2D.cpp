#include "Matrix2D.h"

#include <memory.h>


Matrix2D Matrix2D::Zero( mat2_zero );
Matrix2D Matrix2D::Unit( mat2_unit );


Matrix2D::Matrix2D(void)
{
	memcpy(matrix, mat2_zero, sizeof(ElemType) * MAT2_ELEMENTS_NUMBER);
}

Matrix2D::Matrix2D(const mat2 in_matrix)
{
	memcpy(matrix, in_matrix, sizeof(ElemType) * MAT2_ELEMENTS_NUMBER);
}


Matrix2D::~Matrix2D(void)
{
}

Matrix2D Matrix2D::operator + ( Matrix2D in_matrix )
{
	mat2 mat2_result;
	mat2_add( mat2_result, matrix, in_matrix.matrix );
	return Matrix2D( mat2_result );
}

Matrix2D Matrix2D::operator - ( Matrix2D in_matrix )
{
	mat2 mat2_result;
	mat2_sub( mat2_result, matrix, in_matrix.matrix );
	return Matrix2D( mat2_result );
}