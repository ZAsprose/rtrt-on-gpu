#pragma once

namespace Math
{
	class Vector3D
	{
		public:

			//---------------------------------- Public Fields ------------------------------------

			float X;

			float Y;

			float Z;

			//--------------------------------- Public Constants ----------------------------------

			static Vector3D Zero;

			static Vector3D Unit;

			static Vector3D AxisX;

			static Vector3D AxisY;

			static Vector3D AxisZ;

			//---------------------------------- Constructors -------------------------------------

			Vector3D ( float x, float y, float z )
			{
				X = x;

				Y = y;

				Z = z;
			}

			//------------------------------- Arithmetic Operators --------------------------------

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

			friend float Length ( Vector3D& );

			friend float Square ( Vector3D& );

			friend float Distance ( Vector3D&, Vector3D& );
	        
			friend float Dot ( Vector3D&, Vector3D& );    
	        
			friend Vector3D Normalize ( Vector3D& );

			friend Vector3D Reflect ( Vector3D&, Vector3D& );

			friend Vector3D Refract ( Vector3D&, Vector3D&, float );

			//------------------------------- Exponential Functions -------------------------------

			friend  Vector3D Pow ( Vector3D&, Vector3D& );

			friend  Vector3D Pow ( Vector3D&, float );

			friend  Vector3D Exp ( Vector3D& );

			friend  Vector3D Log ( Vector3D& );

			friend  Vector3D Sqrt ( Vector3D& );
	};
}