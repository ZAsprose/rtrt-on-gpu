#pragma once

#include <iostream>

#include "Util.h"

#include "Vector2D.h"

#include "Vector4D.h"

using namespace std;

namespace Math
{
	class Vector2D;

	class Vector4D;

	class Vector3D
	{
		public:

			//----------------------------------- Public Fields -----------------------------------

			float X;

			float Y;

			float Z;

			//---------------------------------- Public Constants ---------------------------------

			static Vector3D Zero;

			static Vector3D Unit;

			static Vector3D AxisX;

			static Vector3D AxisY;

			static Vector3D AxisZ;

			//------------------------------------ Constructors -----------------------------------

			Vector3D ( float = 0, float = 0, float = 0 );

			Vector3D ( const float[SIZE3D] );

			Vector3D ( const Vector2D& );

			Vector3D ( const Vector3D& );

			Vector3D ( const Vector4D& );

			//-------------------------------- Arithmetic Operators -------------------------------

			friend Vector3D operator + ( const Vector3D& );
			
			friend Vector3D operator - ( const Vector3D& );

			friend Vector3D operator + ( const Vector3D&, const Vector3D& );
			
			friend Vector3D operator - ( const Vector3D&, const Vector3D& );

			friend Vector3D operator + ( const Vector3D&, float );
			
			friend Vector3D operator - ( const Vector3D&, float );

			friend Vector3D operator + ( float, const Vector3D& );
			
			friend Vector3D operator - ( float, const Vector3D& );
			
			friend Vector3D operator * ( const Vector3D&, const Vector3D& );
			
			friend Vector3D operator / ( const Vector3D&, const Vector3D& );

			friend Vector3D operator * ( const Vector3D&, float );

			friend Vector3D operator / ( const Vector3D&, float );	

			friend Vector3D operator * ( float, const Vector3D& );				

			friend Vector3D operator / ( float, const Vector3D& );

			friend Vector3D operator += ( const Vector3D&, const Vector3D& );
			
			friend Vector3D operator -= ( const Vector3D&, const Vector3D& );

			friend Vector3D operator += ( const Vector3D&, float );

			friend Vector3D operator -= ( const Vector3D&, float );	

			friend Vector3D operator *= ( const Vector3D&, const Vector3D& );
			
			friend Vector3D operator /= ( const Vector3D&, const Vector3D& );

			friend Vector3D operator *= ( const Vector3D&, float );

			friend Vector3D operator /= ( const Vector3D&, float );

			//---------------------------------- Logic Operators ----------------------------------

			friend bool operator < ( const Vector3D&, const Vector3D& );

			friend bool operator > ( const Vector3D&, const Vector3D& );

			friend bool operator < ( const Vector3D&, float );

			friend bool operator > ( const Vector3D&, float );

			friend bool operator < ( float, const Vector3D& );				

			friend bool operator > ( float, const Vector3D& );

			friend bool operator <= ( const Vector3D&, const Vector3D& );

			friend bool operator >= ( const Vector3D&, const Vector3D& );

			friend bool operator <= ( const Vector3D&, float );

			friend bool operator >= ( const Vector3D&, float );

			friend bool operator <= ( float, const Vector3D& );				

			friend bool operator >= ( float, const Vector3D& );

			friend bool operator == ( const Vector3D&, const Vector3D& );

			friend bool operator != ( const Vector3D&, const Vector3D& );

			friend bool operator == ( const Vector3D&, float );

			friend bool operator != ( const Vector3D&, float );

			friend bool operator == ( float, const Vector3D& );

			friend bool operator != ( float, const Vector3D& );

			//----------------------------- Input and Output Operators ----------------------------

			friend istream& operator >> ( istream&, Vector3D& );

			friend ostream& operator << ( ostream&, const Vector3D& );

			//-------------------------------- Auxiliary Operators --------------------------------

			operator float * ( )
			{
				return &X;
			}

			operator const float * ( ) const
			{
				return &X;
			}

			float& operator [] ( int index )
			{
				return * ( index + &X );
			}

			//--------------------------------- Common Functions ----------------------------------

			friend Vector3D Abs ( const Vector3D& );

			friend Vector3D Sign ( const Vector3D& );

			friend Vector3D Floor ( const Vector3D& );

			friend Vector3D Fract ( const Vector3D& );

			friend Vector3D Ceiling ( const Vector3D& );

			friend Vector3D Mod ( const Vector3D&, const Vector3D& );

			friend Vector3D Mod ( const Vector3D&, float );

			friend Vector3D Mod ( float, const Vector3D& );

			friend Vector3D Min ( const Vector3D&, const Vector3D& );

			friend Vector3D Min ( const Vector3D&, float );

			friend Vector3D Min ( float, const Vector3D& );

			friend Vector3D Max ( const Vector3D&, const Vector3D& );

			friend Vector3D Max ( const Vector3D&, float );

			friend Vector3D Max ( float, const Vector3D& );

			friend Vector3D Clamp ( const Vector3D&, const Vector3D&, const Vector3D& );

			friend Vector3D Clamp ( const Vector3D&, float, float );

			friend Vector3D Mix ( const Vector3D&, const Vector3D&, const Vector3D& );

			friend Vector3D Mix ( const Vector3D&, const Vector3D&, float );

			friend Vector3D Step ( const Vector3D&, const Vector3D& );

			friend Vector3D Step ( const Vector3D&, float );

			friend Vector3D Smooth ( const Vector3D&, const Vector3D&, const Vector3D& );

			friend Vector3D Smooth ( const Vector3D&, float, float );

			//-------------------------------- Geometric Functions --------------------------------

			friend float Length ( const Vector3D& );

			friend float Square ( const Vector3D& );

			friend float Distance ( const Vector3D&, const Vector3D& );
	        
			friend float Dot ( const Vector3D&, const Vector3D& );

			friend Vector3D Cross ( const Vector3D&, const Vector3D& );
	        
			friend Vector3D Normalize ( const Vector3D& );

			friend Vector3D Reflect ( const Vector3D&, const Vector3D& );

			friend Vector3D Refract ( const Vector3D&, const Vector3D&, float );

			//-------------------------- Angle and Trigonometry Functions -------------------------

			friend Vector3D Radians ( const Vector3D& );

			friend Vector3D Degrees ( const Vector3D& );

			friend Vector3D Sin ( const Vector3D& );

			friend Vector3D Cos ( const Vector3D& );

			friend Vector3D Tan ( const Vector3D& );

			friend Vector3D Asin ( const Vector3D& );

			friend Vector3D Acos ( const Vector3D& );

			friend Vector3D Atan ( const Vector3D& );

			//------------------------------- Exponential Functions -------------------------------

			friend  Vector3D Pow ( const Vector3D&, const Vector3D& );

			friend  Vector3D Pow ( const Vector3D&, float );

			friend  Vector3D Exp ( const Vector3D& );

			friend  Vector3D Log ( const Vector3D& );

			friend  Vector3D Sqrt ( const Vector3D& );
	};
}