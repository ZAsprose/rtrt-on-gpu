#pragma once

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

			friend float Length ( Vector2D& );

			friend float Square ( Vector2D& );

			friend float Distance ( Vector2D&, Vector2D& );
	        
			friend float Dot ( Vector2D&, Vector2D& );    
	        
			friend Vector2D Normalize ( Vector2D& );

			friend Vector2D Reflect ( Vector2D&, Vector2D& );

			friend Vector2D Refract ( Vector2D&, Vector2D&, float );

			//------------------------------- Exponential Functions -------------------------------

			friend  Vector2D Pow ( Vector2D&, Vector2D& );

			friend  Vector2D Pow ( Vector2D&, float );

			friend  Vector2D Exp ( Vector2D& );

			friend  Vector2D Log ( Vector2D& );

			friend  Vector2D Sqrt ( Vector2D& );
	};
}