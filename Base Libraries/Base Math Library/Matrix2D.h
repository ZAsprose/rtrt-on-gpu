#pragma once

#include "Vector2D.h"

#define SIZE2D 2

namespace Math
{
	class Matrix2D
	{
		private:

			//---------------------------------- Private Fields -----------------------------------

			float values[SIZE2D][SIZE2D];

		public:

			//--------------------------------- Public Constants ----------------------------------

			static Matrix2D Zero;

			static Matrix2D Unit;

			//---------------------------------- Constructors -------------------------------------

			Matrix2D ( float matrix[SIZE2D][SIZE2D] );

			//------------------------------- Arithmetic Operators --------------------------------

			friend Matrix2D operator + ( const Matrix2D& );
			
			friend Matrix2D operator - ( const Matrix2D& );

			friend Matrix2D operator + ( const Matrix2D&, const Matrix2D& );
			
			friend Matrix2D operator - ( const Matrix2D&, const Matrix2D& );

			friend Matrix2D operator * ( const Matrix2D&, const Matrix2D& );
			
			friend Vector2D operator * ( const Matrix2D&, const Vector2D& );

			friend Matrix2D operator * ( const Matrix2D&, float );

			friend Matrix2D operator / ( const Matrix2D&, float );
			
			friend Matrix2D operator * ( float, const Matrix2D& );

			friend Matrix2D operator / ( float, const Matrix2D& );


			//-------------------------------- Auxiliary Operators --------------------------------

			float * operator [] ( int index )
			{
				return &values[index][0];
			}

			const float * operator [] ( int index ) const
			{
				return &values[index][0];
			}

			//--------------------------------- Common Functions ----------------------------------
			
			friend Matrix2D Inverse ( const Matrix2D& );
	};
}
