#include "Matrix2D.h"

#include <memory.h>

namespace Math
{
	Matrix2D :: Matrix2D( float matrix[SIZE2D][SIZE2D] )
	{
		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				values[i][j] = matrix[i][j];
			}
		}
	}

	//-------------------------------------- Public Constants -------------------------------------



	//------------------------------------ Arithmetic Operators -----------------------------------

	Matrix2D operator + ( const Matrix2D& source )
	{
		float result[SIZE2D][SIZE2D];

		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				result[i][j] = source[i][j];
			}
		}

		return Matrix2D ( result );
	}

	Matrix2D operator - ( const Matrix2D& source )
	{
		float result[SIZE2D][SIZE2D];

		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				result[i][j] = -source[i][j];
			}
		}

		return Matrix2D ( result );
	}

	Matrix2D operator + ( const Matrix2D& left, const Matrix2D& right )
	{
		float result[SIZE2D][SIZE2D];

		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				result[i][j] = left[i][j] + right[i][j];
			}
		}

		return Matrix2D ( result );
	}

	Matrix2D operator - ( const Matrix2D& left, const Matrix2D& right )
	{
		float result[SIZE2D][SIZE2D];

		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				result[i][j] = left[i][j] - right[i][j];
			}
		}

		return Matrix2D ( result );
	}

	Matrix2D operator * ( const Matrix2D& left, const Matrix2D& right )
	{
		float result[SIZE2D][SIZE2D];

		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				for ( int k = 0; k < SIZE2D; k++ )
				{
					result[i][j] += left[i][k] * right[k][j];
				}
			}
		}

		return Matrix2D ( result );
	}
			
	Vector2D operator * ( const Matrix2D& left, const Vector2D& right )
	{
		float result[SIZE2D];

		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int k = 0; k < SIZE2D; k++ )
			{
				result[i] += left[i][k] * right[k];
			}
		}
		
		return Vector2D ( result );
	}

	Matrix2D operator * ( const Matrix2D& left, float right )
	{
		float result[SIZE2D][SIZE2D];

		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				result[i][j] = left[i][j] * right;
			}
		}

		return Matrix2D ( result );
	}

	Matrix2D operator / ( const Matrix2D& left, float right )
	{
		float result[SIZE2D][SIZE2D];

		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				result[i][j] = left[i][j] / right;
			}
		}

		return Matrix2D ( result );
	}
			
	Matrix2D operator * ( float left, const Matrix2D& right )
	{
		float result[SIZE2D][SIZE2D];

		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				result[i][j] = left * right[i][j];
			}
		}

		return Matrix2D ( result );
	}

	Matrix2D operator / ( float left, const Matrix2D& right )
	{
		float result[SIZE2D][SIZE2D];

		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				result[i][j] = left / right[i][j];
			}
		}

		return Matrix2D ( result );
	}

	//------------------------------------- Common Functions --------------------------------------
			
	Matrix2D Inverse ( const Matrix2D& source )
	{
		float result[SIZE2D][SIZE2D];

		for ( int k = 0; k < SIZE2D; k++ )
		{
			for ( int i = 0; i < SIZE2D; i++ )
			{
				for ( int j = 0; j < SIZE2D; j++ )
				{
					if ( i == k )
					{
						if ( j == k )
						{
							result[i][j] = 1.0F / source[k][k];
						}
						else
						{
							result[i][j] = -source[i][j] / source[k][k];
						}
					}
					else
					{
						if ( j == k )
						{
							result[i][j] = source[i][k] / source[k][k];
						}
						else
						{
							result[i][j] = source[i][j] - source[k][j] * source[i][k] / source[k][k];
						}
					}
				}
			}
		}
		
		return Matrix2D ( result );
	}
}