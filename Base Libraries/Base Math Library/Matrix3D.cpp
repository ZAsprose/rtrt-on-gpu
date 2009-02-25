#include "Matrix3D.h"

namespace Math
{
	//-------------------------------------- Public Constants -------------------------------------

	const Matrix3D Matrix3D :: Zero ( 0.0F );

	const Matrix3D Matrix3D :: Unit ( 1.0F );

	//--------------------------------------- Constructors ----------------------------------------
	
	Matrix3D :: Matrix3D ( float diagonal )
	{
		for ( int i = 0; i < SIZE3D; i++ )
		{
			values[i][i] = diagonal;
		}
	}

	Matrix3D :: Matrix3D ( const float diagonal[SIZE3D] )
	{
		for ( int i = 0; i < SIZE3D; i++ )
		{
			values[i][i] = diagonal[i];
		}
	}
	
	Matrix3D :: Matrix3D ( const Vector3D& diagonal )
	{
		for ( int i = 0; i < SIZE3D; i++ )
		{
			values[i][i] = diagonal[i];
		}
	}
	
	Matrix3D :: Matrix3D ( const float matrix[SIZE3D][SIZE3D] )
	{
		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				values[i][j] = matrix[i][j];
			}
		}
	}
	
	Matrix3D :: Matrix3D ( const Matrix3D& matrix )
	{
		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				values[i][j] = matrix[i][j];
			}
		}
	}

	//------------------------------------ Arithmetic Operators -----------------------------------

	Matrix3D operator + ( const Matrix3D& source )
	{
		Matrix3D result = Matrix3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				result[i][j] = source[i][j];
			}
		}

		return result;
	}

	Matrix3D operator - ( const Matrix3D& source )
	{
		Matrix3D result = Matrix3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				result[i][j] = -source[i][j];
			}
		}

		return result;
	}

	Matrix3D operator + ( const Matrix3D& left, const Matrix3D& right )
	{
		Matrix3D result = Matrix3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				result[i][j] = left[i][j] + right[i][j];
			}
		}

		return result;
	}

	Matrix3D operator - ( const Matrix3D& left, const Matrix3D& right )
	{
		Matrix3D result = Matrix3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				result[i][j] = left[i][j] - right[i][j];
			}
		}

		return result;
	}

	Matrix3D operator * ( const Matrix3D& left, const Matrix3D& right )
	{
		Matrix3D result = Matrix3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				for ( int k = 0; k < SIZE3D; k++ )
				{
					result[i][j] += left[i][k] * right[k][j];
				}
			}
		}

		return result;
	}
			
	Vector3D operator * ( const Matrix3D& left, const Vector3D& right )
	{
		Vector3D result = Vector3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int k = 0; k < SIZE3D; k++ )
			{
				result[i] += left[i][k] * right[k];
			}
		}
		
		return result;
	}

	Matrix3D operator * ( const Matrix3D& left, float right )
	{
		Matrix3D result = Matrix3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				result[i][j] = left[i][j] * right;
			}
		}

		return result;
	}

	Matrix3D operator / ( const Matrix3D& left, float right )
	{
		Matrix3D result = Matrix3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				result[i][j] = left[i][j] / right;
			}
		}

		return result;
	}
			
	Matrix3D operator * ( float left, const Matrix3D& right )
	{
		Matrix3D result = Matrix3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				result[i][j] = left * right[i][j];
			}
		}

		return result;
	}

	Matrix3D operator / ( float left, const Matrix3D& right )
	{
		Matrix3D result = Matrix3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				result[i][j] = left / right[i][j];
			}
		}

		return result;
	}
	
	//-------------------------------- Input and Output Operators ---------------------------------
	
	istream& operator >> ( istream& is, Matrix3D& source )
	{
		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				is >> source[i][j];
			}
		}
		return is;
	}	
	
	ostream& operator << ( ostream& os, const Matrix3D& source )
	{
		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				os << source[i][j] << " ";
			}

			os << endl;
		}
		return os;
	}

	//------------------------------------- Common Functions --------------------------------------

	float Determinant ( const Matrix3D& source )
	{
		return source[A][A] * source[B][B] * source[C][C] +
			   source[B][A] * source[C][B] * source[A][C] +
			   source[C][A] * source[A][B] * source[B][C] -
			   source[A][A] * source[C][B] * source[B][C] -
			   source[C][A] * source[B][B] * source[A][C] -
			   source[B][A] * source[A][B] * source[C][C];
	}

	Matrix3D Transpose ( const Matrix3D& source )
	{
		Matrix3D result = Matrix3D :: Zero;

		for ( int i = 0; i < SIZE3D; i++ )
		{
			for ( int j = 0; j < SIZE3D; j++ )
			{
				result[i][j] = source[j][i];
			}
		}

		return result;
	} 

	Matrix3D Adjugate ( const Matrix3D& source )
	{
		Matrix3D result = Matrix3D :: Zero;
		
		result[A][A] = source[B][B] * source[C][C] - source[B][C] * source[C][B];

		result[A][B] = source[A][C] * source[C][B] - source[A][B] * source[C][C];

		result[A][C] = source[A][B] * source[B][C] - source[A][C] * source[B][B];

		result[B][A] = source[B][C] * source[C][A] - source[B][A] * source[C][C];

		result[B][B] = source[A][A] * source[C][C] - source[A][C] * source[C][A];

		result[B][C] = source[A][C] * source[B][A] - source[A][A] * source[B][C];

		result[C][A] = source[B][A] * source[C][B] - source[B][B] * source[C][A];

		result[C][B] = source[A][B] * source[C][A] - source[A][A] * source[C][B];

		result[C][C] = source[A][A] * source[B][B] - source[A][B] * source[B][A];
		
		return result;
	}

	Matrix3D Inverse ( const Matrix3D& source )
	{
		return Adjugate ( source ) / Determinant ( source );
	}
}