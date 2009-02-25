#pragma once

#include <iostream>

#include "Util.h"

#include "Vector4D.h"

namespace Math
{
	class Matrix4D
	{
		private:

			//---------------------------------- Private Fields -----------------------------------

			float values[SIZE4D][SIZE4D];

		public:

			//--------------------------------- Public Constants ----------------------------------

			static const Matrix4D Zero;

			static const Matrix4D Unit;

			//---------------------------------- Constructors -------------------------------------

			Matrix4D ( float );

			Matrix4D ( const float[SIZE4D] );

			Matrix4D ( const Vector4D& );

			Matrix4D ( const float[SIZE4D][SIZE4D] );

			Matrix4D ( const Matrix4D& );

			//------------------------------- Arithmetic Operators --------------------------------

			friend Matrix4D operator + ( const Matrix4D& );
			
			friend Matrix4D operator - ( const Matrix4D& );

			friend Matrix4D operator + ( const Matrix4D&, const Matrix4D& );
			
			friend Matrix4D operator - ( const Matrix4D&, const Matrix4D& );

			friend Matrix4D operator * ( const Matrix4D&, const Matrix4D& );
			
			friend Vector4D operator * ( const Matrix4D&, const Vector4D& );

			friend Matrix4D operator * ( const Matrix4D&, float );

			friend Matrix4D operator / ( const Matrix4D&, float );
			
			friend Matrix4D operator * ( float, const Matrix4D& );

			friend Matrix4D operator / ( float, const Matrix4D& );

			//----------------------------- Input and Output Operators ----------------------------

			friend istream& operator >> ( istream&, Matrix4D& );

			friend ostream& operator << ( ostream&, const Matrix4D& );

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
			
			friend float Determinant ( const Matrix4D& );

			friend Matrix4D Transpose ( const Matrix4D& );

			friend Matrix4D Adjugate ( const Matrix4D& );

			friend Matrix4D Inverse ( const Matrix4D& );
	};
}
