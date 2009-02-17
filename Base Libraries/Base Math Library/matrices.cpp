#include "matrices.h"

void mat2_add(mat2 & out_resuls, mat2 in_a, mat2 in_b )
{
	for( int i = 0; i < MAT2_ELEMENTS_NUMBER; i++ )
	{
		out_resuls[i] = in_a[i] + in_b[i];
	}
}

void mat2_sub(mat2 & out_resuls, mat2 in_a, mat2 in_b )
{
	for( int i = 0; i < MAT2_ELEMENTS_NUMBER; i++ )
	{
		out_resuls[i] = in_a[i] - in_b[i];
	}
}

void mat3_add(mat3 & out_resuls, mat3 in_a, mat3 in_b )
{
	for( int i = 0; i < MAT3_ELEMENTS_NUMBER; i++ )
	{
		out_resuls[i] = in_a[i] + in_b[i];
	}
}

void mat3_sub(mat3 & out_resuls, mat3 in_a, mat3 in_b )
{
	for( int i = 0; i < MAT3_ELEMENTS_NUMBER; i++ )
	{
		out_resuls[i] = in_a[i] - in_b[i];
	}
}