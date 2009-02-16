#pragma once

#include <iostream>

using namespace std;

namespace Math
{
	class Vector2D
	{
		public:

			//---------------------------------- Public Fields ------------------------------------

			float X;

			float Y;

			//--------------------------------- Public Constants ----------------------------------

			static Vector2D Zero;

			static Vector2D Unit;

			static Vector2D AxisX;

			static Vector2D AxisY;

			//---------------------------------- Constructors -------------------------------------

			Vector2D ( float x, float y )
			{
				X = x;

				Y = y;
			}

			//------------------------------- Arithmetic Operators --------------------------------

			friend Vector2D operator + ( const Vector2D& );
			
			friend Vector2D operator - ( const Vector2D& );

			friend Vector2D operator + ( const Vector2D&, const Vector2D& );
			
			friend Vector2D operator - ( const Vector2D&, const Vector2D& );

			friend Vector2D operator + ( const Vector2D&, float );
			
			friend Vector2D operator - ( const Vector2D&, float );

			friend Vector2D operator + ( float, const Vector2D& );
			
			friend Vector2D operator - ( float, const Vector2D& );
			
			friend Vector2D operator * ( const Vector2D&, const Vector2D& );
			
			friend Vector2D operator / ( const Vector2D&, const Vector2D& );

			friend Vector2D operator * ( const Vector2D&, float );

			friend Vector2D operator / ( const Vector2D&, float );	

			friend Vector2D operator * ( float, const Vector2D& );				

			friend Vector2D operator / ( float, const Vector2D& );

			friend Vector2D operator += ( const Vector2D&, const Vector2D& );
			
			friend Vector2D operator -= ( const Vector2D&, const Vector2D& );

			friend Vector2D operator += ( const Vector2D&, float );

			friend Vector2D operator -= ( const Vector2D&, float );	

			friend Vector2D operator *= ( const Vector2D&, const Vector2D& );
			
			friend Vector2D operator /= ( const Vector2D&, const Vector2D& );

			friend Vector2D operator *= ( const Vector2D&, float );

			friend Vector2D operator /= ( const Vector2D&, float );

			//---------------------------------- Logic Operators ----------------------------------

			friend bool operator < ( const Vector2D&, const Vector2D& );

			friend bool operator > ( const Vector2D&, const Vector2D& );

			friend bool operator < ( const Vector2D&, float );

			friend bool operator > ( const Vector2D&, float );

			friend bool operator < ( float, const Vector2D& );				

			friend bool operator > ( float, const Vector2D& );

			friend bool operator <= ( const Vector2D&, const Vector2D& );

			friend bool operator >= ( const Vector2D&, const Vector2D& );

			friend bool operator <= ( const Vector2D&, float );

			friend bool operator >= ( const Vector2D&, float );

			friend bool operator <= ( float, const Vector2D& );				

			friend bool operator >= ( float, const Vector2D& );

			friend bool operator == ( const Vector2D&, const Vector2D& );

			friend bool operator != ( const Vector2D&, const Vector2D& );

			friend bool operator == ( const Vector2D&, float );

			friend bool operator != ( const Vector2D&, float );

			friend bool operator == ( float, const Vector2D& );

			friend bool operator != ( float, const Vector2D& );

			//----------------------------- Input and Output Operators ----------------------------

			friend istream& operator >> ( istream& is, Vector2D& );

			friend ostream& operator << ( ostream& os, const Vector2D& );

			//-------------------------------- Auxiliary Operators --------------------------------

			operator float * ( )
			{
				return &X;
			}

			float& operator [] ( int index )
			{
				return * ( index + &X );
			}

			//--------------------------------- Common Functions ----------------------------------

			friend Vector2D Abs ( const Vector2D& );

			friend Vector2D Sign ( const Vector2D& );

			friend Vector2D Floor ( const Vector2D& );

			friend Vector2D Fract ( const Vector2D& );

			friend Vector2D Ceiling ( const Vector2D& );

			friend Vector2D Mod ( const Vector2D&, const Vector2D& );

			friend Vector2D Mod ( const Vector2D&, float );

			friend Vector2D Mod ( float, const Vector2D& );

			friend Vector2D Min ( const Vector2D&, const Vector2D& );

			friend Vector2D Min ( const Vector2D&, float );

			friend Vector2D Min ( float, const Vector2D& );

			friend Vector2D Max ( const Vector2D&, const Vector2D& );

			friend Vector2D Max ( const Vector2D&, float );

			friend Vector2D Max ( float, const Vector2D& );

			friend Vector2D Clamp ( const Vector2D&, const Vector2D&, const Vector2D& );

			friend Vector2D Clamp ( const Vector2D&, float, float );

			friend Vector2D Mix ( const Vector2D&, const Vector2D&, const Vector2D& );

			friend Vector2D Mix ( const Vector2D&, const Vector2D&, float );

			friend Vector2D Step ( const Vector2D&, const Vector2D& );

			friend Vector2D Step ( const Vector2D&, float );

			friend Vector2D Smooth ( const Vector2D&, const Vector2D&, const Vector2D& );

			friend Vector2D Smooth ( const Vector2D&, float, float );

			//-------------------------------- Geometric Functions --------------------------------

			friend float Length ( const Vector2D& );

			friend float Square ( const Vector2D& );

			friend float Distance ( const Vector2D&, const Vector2D& );
	        
			friend float Dot ( const Vector2D&, const Vector2D& );    
	        
			friend Vector2D Normalize ( const Vector2D& );

			friend Vector2D Reflect ( const Vector2D&, const Vector2D& );

			friend Vector2D Refract ( const Vector2D&, const Vector2D&, float );

			//-------------------------- Angle and Trigonometry Functions -------------------------

			friend Vector2D Radians ( const Vector2D& );

			friend Vector2D Degrees ( const Vector2D& );

			friend Vector2D Sin ( const Vector2D& );

			friend Vector2D Cos ( const Vector2D& );

			friend Vector2D Tan ( const Vector2D& );

			friend Vector2D Asin ( const Vector2D& );

			friend Vector2D Acos ( const Vector2D& );

			friend Vector2D Atan ( const Vector2D& );

			//------------------------------- Exponential Functions -------------------------------

			friend  Vector2D Pow ( const Vector2D&, const Vector2D& );

			friend  Vector2D Pow ( const Vector2D&, float );

			friend  Vector2D Exp ( const Vector2D& );

			friend  Vector2D Log ( const Vector2D& );

			friend  Vector2D Sqrt ( const Vector2D& );
	};
}