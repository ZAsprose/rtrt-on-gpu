#include "Matrix3D.h"

#include <memory.h>


Matrix3D Matrix3D::Zero( mat3_zero );
Matrix3D Matrix3D::Unit( mat3_unit );


Matrix3D::Matrix3D(void)
{
	memcpy(matrix, mat3_zero, sizeof(ElemType) * MAT3_ELEMENTS_NUMBER);
}

Matrix3D::Matrix3D(const mat3 in_matrix)
{
	memcpy(matrix, in_matrix, sizeof(ElemType) * MAT3_ELEMENTS_NUMBER);
}


Matrix3D::~Matrix3D(void)
{
}

Matrix3D Matrix3D::operator + ( Matrix3D in_matrix )
{
	mat3 mat3_result;
	mat3_add( mat3_result, matrix, in_matrix.matrix );
	return Matrix3D( mat3_result );
}

Matrix3D Matrix3D::operator - ( Matrix3D in_matrix )
{
	mat3 mat3_result;
	mat3_sub( mat3_result, matrix, in_matrix.matrix );
	return Matrix3D( mat3_result );
}