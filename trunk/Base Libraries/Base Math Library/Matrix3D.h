#pragma once

#include <iostream>

#include "Util.h"

#include "Vector3D.h"

namespace Math
{
	class Matrix3D
	{
		private:

			//---------------------------------- Private Fields -----------------------------------

			float values[SIZE3D][SIZE3D];

		public:

			//--------------------------------- Public Constants ----------------------------------

			static const Matrix3D Zero;

			static const Matrix3D Unit;

			//---------------------------------- Constructors -------------------------------------

			Matrix3D ( float );

			Matrix3D ( const float[SIZE3D] );

			Matrix3D ( const Vector3D& );

			Matrix3D ( const float[SIZE3D][SIZE3D] );

			Matrix3D ( const Matrix3D& );

			//------------------------------- Arithmetic Operators --------------------------------

			friend Matrix3D operator + ( const Matrix3D& );
			
			friend Matrix3D operator - ( const Matrix3D& );

			friend Matrix3D operator + ( const Matrix3D&, const Matrix3D& );
			
			friend Matrix3D operator - ( const Matrix3D&, const Matrix3D& );

			friend Matrix3D operator * ( const Matrix3D&, const Matrix3D& );
			
			friend Vector3D operator * ( const Matrix3D&, const Vector3D& );

			friend Matrix3D operator * ( const Matrix3D&, float );

			friend Matrix3D operator / ( const Matrix3D&, float );
			
			friend Matrix3D operator * ( float, const Matrix3D& );

			friend Matrix3D operator / ( float, const Matrix3D& );

			//----------------------------- Input and Output Operators ----------------------------

			friend istream& operator >> ( istream&, Matrix3D& );

			friend ostream& operator << ( ostream&, const Matrix3D& );

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
			
			friend float Determinant ( const Matrix3D& );

			friend Matrix3D Transpose ( const Matrix3D& );

			friend Matrix3D Adjugate ( const Matrix3D& );

			friend Matrix3D Inverse ( const Matrix3D& );
	};
}
