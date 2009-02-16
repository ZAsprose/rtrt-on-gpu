#pragma once

namespace Math
{
	class Vector4D
	{
		public:

			//---------------------------------- Public Fields ------------------------------------

			float X;

			float Y;

			float Z;

			float W;

			//--------------------------------- Public Constants ----------------------------------

			static Vector4D Zero;

			static Vector4D Unit;

			static Vector4D AxisX;

			static Vector4D AxisY;

			static Vector4D AxisZ;

			static Vector4D AxisW;

			//---------------------------------- Constructors -------------------------------------

			Vector4D ( float x, float y, float z, float w )
			{
				X = x;

				Y = y;

				Z = z;

				W = w;
			}

			//------------------------------- Arithmetic Operators --------------------------------

			friend Vector4D operator + ( const Vector4D& );
			
			friend Vector4D operator - ( const Vector4D& );

			friend Vector4D operator + ( const Vector4D&, const Vector4D& );
			
			friend Vector4D operator - ( const Vector4D&, const Vector4D& );

			friend Vector4D operator + ( const Vector4D&, float );
			
			friend Vector4D operator - ( const Vector4D&, float );

			friend Vector4D operator + ( float, const Vector4D& );
			
			friend Vector4D operator - ( float, const Vector4D& );
			
			friend Vector4D operator * ( const Vector4D&, const Vector4D& );
			
			friend Vector4D operator / ( const Vector4D&, const Vector4D& );

			friend Vector4D operator * ( const Vector4D&, float );

			friend Vector4D operator / ( const Vector4D&, float );	

			friend Vector4D operator * ( float, const Vector4D& );				

			friend Vector4D operator / ( float, const Vector4D& );

			friend Vector4D operator += ( const Vector4D&, const Vector4D& );
			
			friend Vector4D operator -= ( const Vector4D&, const Vector4D& );

			friend Vector4D operator += ( const Vector4D&, float );

			friend Vector4D operator -= ( const Vector4D&, float );	

			friend Vector4D operator *= ( const Vector4D&, const Vector4D& );
			
			friend Vector4D operator /= ( const Vector4D&, const Vector4D& );

			friend Vector4D operator *= ( const Vector4D&, float );

			friend Vector4D operator /= ( const Vector4D&, float );

			//---------------------------------- Logic Operators ----------------------------------

			friend bool operator < ( const Vector4D&, const Vector4D& );

			friend bool operator > ( const Vector4D&, const Vector4D& );

			friend bool operator < ( const Vector4D&, float );

			friend bool operator > ( const Vector4D&, float );

			friend bool operator < ( float, const Vector4D& );				

			friend bool operator > ( float, const Vector4D& );

			friend bool operator <= ( const Vector4D&, const Vector4D& );

			friend bool operator >= ( const Vector4D&, const Vector4D& );

			friend bool operator <= ( const Vector4D&, float );

			friend bool operator >= ( const Vector4D&, float );

			friend bool operator <= ( float, const Vector4D& );				

			friend bool operator >= ( float, const Vector4D& );

			friend bool operator == ( const Vector4D&, const Vector4D& );

			friend bool operator != ( const Vector4D&, const Vector4D& );

			friend bool operator == ( const Vector4D&, float );

			friend bool operator != ( const Vector4D&, float );

			friend bool operator == ( float, const Vector4D& );

			friend bool operator != ( float, const Vector4D& );

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

			friend Vector4D Abs ( const Vector4D& );

			friend Vector4D Sign ( const Vector4D& );

			friend Vector4D Floor ( const Vector4D& );

			friend Vector4D Fract ( const Vector4D& );

			friend Vector4D Ceiling ( const Vector4D& );

			friend Vector4D Mod ( const Vector4D&, const Vector4D& );

			friend Vector4D Mod ( const Vector4D&, float );

			friend Vector4D Mod ( float, const Vector4D& );

			friend Vector4D Min ( const Vector4D&, const Vector4D& );

			friend Vector4D Min ( const Vector4D&, float );

			friend Vector4D Min ( float, const Vector4D& );

			friend Vector4D Max ( const Vector4D&, const Vector4D& );

			friend Vector4D Max ( const Vector4D&, float );

			friend Vector4D Max ( float, const Vector4D& );

			friend Vector4D Clamp ( const Vector4D&, const Vector4D&, const Vector4D& );

			friend Vector4D Clamp ( const Vector4D&, float, float );

			friend Vector4D Mix ( const Vector4D&, const Vector4D&, const Vector4D& );

			friend Vector4D Mix ( const Vector4D&, const Vector4D&, float );

			friend Vector4D Step ( const Vector4D&, const Vector4D& );

			friend Vector4D Step ( const Vector4D&, float );

			friend Vector4D Smooth ( const Vector4D&, const Vector4D&, const Vector4D& );

			friend Vector4D Smooth ( const Vector4D&, float, float );

			//-------------------------------- Geometric Functions --------------------------------

			friend float Length ( const Vector4D& );

			friend float Square ( const Vector4D& );

			friend float Distance ( const Vector4D&, const Vector4D& );
	        
			friend float Dot ( const Vector4D&, const Vector4D& );    
	        
			friend Vector4D Normalize ( const Vector4D& );

			friend Vector4D Reflect ( const Vector4D&, const Vector4D& );

			friend Vector4D Refract ( const Vector4D&, const Vector4D&, float );

			//------------------------------- Exponential Functions -------------------------------

			friend  Vector4D Pow ( const Vector4D&, const Vector4D& );

			friend  Vector4D Pow ( const Vector4D&, float );

			friend  Vector4D Exp ( const Vector4D& );

			friend  Vector4D Log ( const Vector4D& );

			friend  Vector4D Sqrt ( const Vector4D& );
	};
}