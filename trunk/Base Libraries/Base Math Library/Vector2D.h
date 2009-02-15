#pragma once

#include <math.h>

#include <stdio.h>

#include "Util.h"

namespace Math
{
	class Vector2D
	{
		public:

			//---------------------------------- Public Fields ------------------------------------

			float X;

			float Y;

			//---------------------------------- Public Fields ------------------------------------

			static Vector2D Zero;

			static Vector2D Unit;

			static Vector2D UnitX;

			static Vector2D UnitY;

			//---------------------------------- Constructors -------------------------------------

			Vector2D ( void );

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

			friend  Vector2D Exp ( Vector2D& );

			friend  Vector2D Log ( Vector2D& );

			friend  Vector2D Sqrt ( Vector2D& );
	};

	//----------------------------------------- Operators -----------------------------------------

	inline Vector2D operator + ( const Vector2D& source )
	{
		return Vector2D ( source.X, source.Y );
	}

	inline Vector2D operator - ( const Vector2D& source )
	{
		return Vector2D ( -source.X, -source.Y );
	}

	inline Vector2D operator + ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X + right.X, left.Y + right.Y );
	}

	inline Vector2D operator - ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X - right.X, left.Y - right.Y );
	}

	inline Vector2D operator + ( const Vector2D& left, float right )
	{
		return Vector2D ( left.X + right, left.Y + right );
	}

	inline Vector2D operator - ( const Vector2D& left, float right )
	{
		return Vector2D ( left.X - right, left.Y - right );
	}

	inline Vector2D operator + ( float left, const Vector2D& right )
	{
		return Vector2D ( left + right.X, left + right.Y );
	}

	inline Vector2D operator - ( float left, const Vector2D& right )
	{
		return Vector2D ( left - right.X, left - right.Y );
	}

	inline Vector2D operator * ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X * right.X, left.Y * right.Y );
	}

	inline Vector2D operator / ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X / right.X, left.Y / right.Y );
	}

	inline Vector2D operator * ( const Vector2D& left, float right )
	{
		return Vector2D ( left.X * right, left.Y * right );
	}

	inline Vector2D operator / ( const Vector2D& left, float right )
	{
		return Vector2D ( left.X / right, left.Y / right );
	}

	inline Vector2D operator * ( float left, const Vector2D& right )
	{
		return Vector2D ( left * right.X, left * right.Y );
	}

	inline Vector2D operator / ( float left, const Vector2D& right )
	{
		return Vector2D ( left / right.X, left / right.Y );
	}

	inline Vector2D operator += ( Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X += right.X, left.Y += right.Y );
	}

	inline Vector2D operator -= ( Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X -= right.X, left.Y -= right.X );
	}

	inline Vector2D operator += ( Vector2D& left, float right )
	{
		return Vector2D ( left.X += right, left.Y += right );
	}

	inline Vector2D operator -= ( Vector2D& left, float right )
	{
		return Vector2D ( left.X -= right, left.Y -= right );
	}

	inline Vector2D operator *= ( Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X *= right.X, left.Y *= right.Y );
	}

	inline Vector2D operator /= ( Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X /= right.X, left.Y /= right.X );
	}

	inline Vector2D operator *= ( Vector2D& left, float right )
	{
		return Vector2D ( left.X *= right, left.Y *= right );
	}

	inline Vector2D operator /= ( Vector2D& left, float right )
	{
		return Vector2D ( left.X /= right, left.Y /= right );
	}

	inline bool operator < ( const Vector2D& left, const Vector2D& right )
	{
		return left.X < right.X && left.Y < right.Y;
	}

	inline bool operator > ( const Vector2D& left, const Vector2D& right )
	{
		return left.X > right.X && left.Y > right.Y;
	}

	inline bool operator < ( const Vector2D& left, float right )
	{
		return left.X < right && left.Y < right;
	}

	inline bool operator > ( const Vector2D& left, float right )
	{
		return left.X > right && left.Y > right;
	}

	inline bool operator < ( float left, const Vector2D& right )
	{
		return left < right.X && left < right.Y;
	}

	inline bool operator > ( float left, const Vector2D& right )
	{
		return left > right.X && left > right.Y;
	}

	inline bool operator <= ( const Vector2D& left, const Vector2D& right )
	{
		return left.X <= right.X && left.Y <= right.Y;
	}

	inline bool operator >= ( const Vector2D& left, const Vector2D& right )
	{
		return left.X >= right.X && left.Y >= right.Y;
	}

	inline bool operator <= ( const Vector2D& left, float right )
	{
		return left.X <= right && left.Y <= right;
	}

	inline bool operator >= ( const Vector2D& left, float right )
	{
		return left.X >= right && left.Y >= right;
	}

	inline bool operator <= ( float left, const Vector2D& right )
	{
		return left <= right.X && left <= right.Y;
	}

	inline bool operator >= ( float left, const Vector2D& right )
	{
		return left >= right.X && left >= right.Y;
	}

	inline bool operator == ( const Vector2D& left, const Vector2D& right )
	{
		return left.X == right.X && left.Y == right.Y;
	}

	inline bool operator != ( const Vector2D& left, const Vector2D& right )
	{
		return left.X != right.X || left.Y != right.Y;
	}

	inline bool operator == ( const Vector2D& left, float right )
	{
		return left.X == right && left.Y == right;
	}

	inline bool operator != ( const Vector2D& left, float right )
	{
		return left.X != right || left.Y != right;
	}

	inline bool operator == ( float left, const Vector2D& right )
	{
		return left == right.X && left == right.Y;
	}

	inline bool operator != ( float left, const Vector2D& right )
	{
		return left != right.X || left != right.Y;
	}

	//------------------------------------- Common Functions --------------------------------------

	inline Vector2D Abs ( const Vector2D& source )
	{
		return Vector2D ( fabsf ( source.X ), fabsf ( source.Y ) );
	}

	inline Vector2D Sign ( const Vector2D& source )
	{
		return Vector2D ( Sign ( source.X ), Sign ( source.Y ) );
	}

	inline Vector2D Floor ( const Vector2D& source )
	{
		return Vector2D ( floorf ( source.X ), floorf ( source.Y ) );
	}

	inline Vector2D Fract ( const Vector2D& source )
	{
		return Vector2D ( Fract ( source.X ), Fract ( source.Y ) );
	}

	inline Vector2D Ceil ( const Vector2D& source )
	{
		return Vector2D ( ceilf ( source.X ), ceilf ( source.Y ) );
	}

	inline Vector2D Mod ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( Mod ( left.X, right.X ), Mod ( left.Y, right.Y ) );
	}

	inline Vector2D Mod ( const Vector2D& left, const float right )
	{
		return Vector2D ( Mod ( left.X, right ), Mod ( left.Y, right ) );
	}

	inline Vector2D Mod ( const float left, const Vector2D& right )
	{
		return Vector2D ( Mod ( left, right.X ), Mod ( left, right.Y ) );
	}

	inline Vector2D Min ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X < right.X ? left.X : right.X,
						  left.Y < right.Y ? left.Y : right.Y );
	}

	inline Vector2D Min ( const Vector2D& left, float right )
	{
		return Vector2D ( left.X < right ? left.X : right,
						  left.Y < right ? left.Y : right );
	}

	inline Vector2D Min ( float left, const Vector2D& right )
	{
		return Vector2D ( left < right.X ? left : right.X,
						  left < right.Y ? left : right.Y );
	}

	inline Vector2D Max ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X > right.X ? left.X : right.X,
						  left.Y > right.Y ? left.Y : right.Y );
	}

	inline Vector2D Max ( const Vector2D& left, float right )
	{
		return Vector2D ( left.X > right ? left.X : right,
						  left.Y > right ? left.Y : right );
	}

	inline Vector2D Max ( float left, const Vector2D& right )
	{
		return Vector2D ( left > right.X ? left : right.X,
						  left > right.Y ? left : right.Y );
	}

	inline Vector2D Clamp ( const Vector2D& source, const Vector2D& min, const Vector2D& max )
	{
		return Vector2D ( Clamp ( source.X, min.X, max.X ), Clamp ( source.Y, min.Y, max.Y ) );
	}

	inline Vector2D Clamp ( const Vector2D& source, float min, float max )
	{
		return Vector2D ( Clamp ( source.X, min, max ), Clamp ( source.Y, min, max ) );
	}

	inline Vector2D Mix ( const Vector2D& left, const Vector2D& right, const Vector2D& alpha )
	{
		return Vector2D ( Mix ( left.X, right.X, alpha.X ), Mix ( left.Y, right.Y, alpha.Y ) );
	}

	inline Vector2D Mix ( const Vector2D& left, const Vector2D& right, float alpha )
	{
		return Vector2D ( Mix ( left.X, right.X, alpha ), Mix ( left.Y, right.Y, alpha ) );
	}

	inline Vector2D Step ( const Vector2D& source, const Vector2D& value )
	{
		return Vector2D ( Step ( source.X, value.X ), Step ( source.Y, value.Y ) );
	}

	inline Vector2D Step ( const Vector2D& source, float value )
	{
		return Vector2D ( Step ( source.X, value ), Step ( source.Y, value ) );
	}

	inline Vector2D Smooth ( const Vector2D& source, const Vector2D& min, const Vector2D& max )
	{
		return Vector2D ( Smooth ( source.X, min.X, max.X ), Smooth ( source.Y, min.Y, max.Y ) );
	}

	inline Vector2D Smooth ( const Vector2D& source, float min, float max )
	{
		return Vector2D ( Smooth ( source.X, min, max ), Smooth ( source.Y, min, max ) );
	}

	//------------------------------------ Geometric Functions ------------------------------------

	inline float Length ( Vector2D& source )
	{
		return sqrtf ( source.X * source.X + source.Y * source.Y );
	}

	inline float Square ( Vector2D& source )
	{
		return source.X * source.X + source.Y * source.Y;
	}

	inline float Distance ( Vector2D& left, Vector2D& right )
	{
		return Length ( left - right );
	}

	inline float Dot ( Vector2D& left, Vector2D& right )
	{
		return left.X * right.X + left.Y * right.Y;
	}        

	inline Vector2D Normalize ( Vector2D& source )
	{
		return source / Length ( source );
	}

	inline Vector2D Reflect ( Vector2D& incident, Vector2D& normal )
	{
		return incident - 2.0F * Dot ( normal, incident ) * normal;
	}

	inline Vector2D Refract ( Vector2D& incident, Vector2D& normal, float index )
	{
		float dot = Dot( incident, normal );

		float square = 1.0F - index * index * ( 1.0F - dot * dot );

		if ( square < 0.0F )
		{
			return Reflect ( incident, normal );
		}
		else
		{
			return index * incident - ( sqrtf( square ) + index * dot ) * normal;
		}
	}

	//----------------------------------- Exponential Functions -----------------------------------

    inline Vector2D Pow ( Vector2D& left, Vector2D& right )
    {
		return Vector2D ( powf ( left.X, right.X ), powf ( left.Y, right.Y ) );
    }

    inline Vector2D Exp ( Vector2D& source )
    {
		return Vector2D ( expf ( source.X ), expf ( source.Y ) );
    }

    inline Vector2D Log ( Vector2D& source )
    {
		return Vector2D ( logf ( source.X ), logf ( source.Y ) );
    }

    inline Vector2D Sqrt ( Vector2D& source )
    {
		return Vector2D ( sqrtf ( source.X ), sqrtf ( source.Y ) );
    }
}
