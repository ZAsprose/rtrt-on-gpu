/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "Matrix2D.h"

namespace Math
{
	//------------------------------------- Public Constants --------------------------------------

	const Matrix2D Matrix2D :: Zero ( 0.0F );

	const Matrix2D Matrix2D :: Unit ( 1.0F );

	//--------------------------------------- Constructors ----------------------------------------
	
	Matrix2D :: Matrix2D ( float diagonal )
	{
		for ( int i = 0; i < SIZE2D; i++ )
		{
			values[i][i] = diagonal;
		}
	}

	Matrix2D :: Matrix2D ( const float diagonal[SIZE2D] )
	{
		for ( int i = 0; i < SIZE2D; i++ )
		{
			values[i][i] = diagonal[i];
		}
	}
	
	Matrix2D :: Matrix2D ( const Vector2D& diagonal )
	{
		for ( int i = 0; i < SIZE2D; i++ )
		{
			values[i][i] = diagonal[i];
		}
	}
	
	Matrix2D :: Matrix2D ( const float matrix[SIZE2D][SIZE2D] )
	{
		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				values[i][j] = matrix[i][j];
			}
		}
	}
	
	Matrix2D :: Matrix2D ( const Matrix2D& matrix )
	{
		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				values[i][j] = matrix[i][j];
			}
		}
	}

	//------------------------------------ Arithmetic Operators -----------------------------------

	Matrix2D operator - ( const Matrix2D& source )
	{
		Matrix2D result = Matrix2D :: Zero;

		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				result[i][j] = -source[i][j];
			}
		}

		return result;
	}

	Matrix2D operator + ( const Matrix2D& left, const Matrix2D& right )
	{
		Matrix2D result = Matrix2D :: Zero;

		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				result[i][j] = left[i][j] + right[i][j];
			}
		}

		return result;
	}

	Matrix2D operator - ( const Matrix2D& left, const Matrix2D& right )
	{
		Matrix2D result = Matrix2D :: Zero;

		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				result[i][j] = left[i][j] - right[i][j];
			}
		}

		return result;
	}

	Matrix2D operator * ( const Matrix2D& left, const Matrix2D& right )
	{
		Matrix2D result = Matrix2D :: Zero;

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

		return result;
	}
			
	Vector2D operator * ( const Matrix2D& left, const Vector2D& right )
	{
		Vector2D result = Vector2D :: Zero;

		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int k = 0; k < SIZE2D; k++ )
			{
				result[i] += left[i][k] * right[k];
			}
		}
		
		return result;
	}

	Matrix2D operator * ( const Matrix2D& left, float right )
	{
		Matrix2D result = Matrix2D :: Zero;

		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				result[i][j] = left[i][j] * right;
			}
		}

		return result;
	}

	Matrix2D operator / ( const Matrix2D& left, float right )
	{
		Matrix2D result = Matrix2D :: Zero;

		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				result[i][j] = left[i][j] / right;
			}
		}

		return result;
	}
			
	Matrix2D operator * ( float left, const Matrix2D& right )
	{
		Matrix2D result = Matrix2D :: Zero;

		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				result[i][j] = left * right[i][j];
			}
		}

		return result;
	}

	Matrix2D operator / ( float left, const Matrix2D& right )
	{
		Matrix2D result = Matrix2D :: Zero;

		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				result[i][j] = left / right[i][j];
			}
		}

		return result;
	}
	
	//-------------------------------- Input and Output Operators ---------------------------------
	
	istream& operator >> ( istream& is, Matrix2D& source )
	{
		for ( int j = 0; j < SIZE2D; j++ )
		{
			for ( int i = 0; i < SIZE2D; i++ )
			{
				is >> source[i][j];
			}
		}
		return is;
	}	
	
	ostream& operator << ( ostream& os, const Matrix2D& source )
	{
		for ( int j = 0; j < SIZE2D; j++ )
		{
			for ( int i = 0; i < SIZE2D; i++ )
			{
				os << source[i][j] << " ";
			}

			os << endl;
		}
		return os;
	}

	//------------------------------------- Common Functions --------------------------------------

	float Determinant ( const Matrix2D& source )
	{
		return source[A][A] * source[B][B] - source[A][B] * source[B][A];
	}

	Matrix2D Transpose ( const Matrix2D& source )
	{
		Matrix2D result = Matrix2D :: Zero;

		for ( int i = 0; i < SIZE2D; i++ )
		{
			for ( int j = 0; j < SIZE2D; j++ )
			{
				result[i][j] = source[j][i];
			}
		}

		return result;
	} 

	Matrix2D Adjugate ( const Matrix2D& source )
	{
		Matrix2D result = Matrix2D :: Zero;
		
		result[A][A] = source[B][B];

		result[A][B] = -source[A][B];

		result[B][A] = -source[B][A];

		result[B][B] = source[A][A];
		
		return result;
	}

	Matrix2D Inverse ( const Matrix2D& source )
	{
		return Adjugate ( source ) / Determinant ( source );
	}
}