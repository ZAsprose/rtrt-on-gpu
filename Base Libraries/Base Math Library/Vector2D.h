#pragma once

#include <math.h>

#include <stdio.h>

#include "Util.h"

class Vector2D
{
	public:

		float X;

		float Y;

	public:

		Vector2D ( void );

		Vector2D ( float x, float y )
		{
			X = x;

			Y = y;
		}
	
		~Vector2D ( void );

		//----------------------------- Arithmetic Operators ----------------------------

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

		//------------------------------- Logic Operators ------------------------------

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

		//----------------------------- Auxiliary Operators -----------------------------

		operator float * ( )
		{
			return &X;
		}

		float& operator [] ( int index )
		{
			return * ( index + &X );
		}

		//------------------------------- Common Functions ------------------------------

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

		friend Vector2D Min ( const Vector2D&, const Vector2D& );

		friend Vector2D Max ( const Vector2D&, const Vector2D& );

		friend Vector2D Max ( const Vector2D&, float );

		friend Vector2D Max ( const Vector2D&, const Vector2D& );

		friend Vector2D Clamp ( const Vector2D&, const Vector2D&, const Vector2D& );

		friend Vector2D Clamp ( const Vector2D&, float, float );

		friend Vector2D Mix ( const Vector2D&, const Vector2D&, const Vector2D& );

		friend Vector2D Mix ( const Vector2D&, const Vector2D&, float );

		friend Vector2D Step ( const Vector2D&, const Vector2D& );

		friend Vector2D Step ( const Vector2D&, float );

		friend Vector2D Smooth ( const Vector2D&, const Vector2D&, const Vector2D& );

		friend Vector2D Smooth ( const Vector2D&, float, float );

		//----------------------------- Geometric Functions -----------------------------

        friend float Length ( Vector2D& );

        friend float Square ( Vector2D& );

        friend float Distance ( Vector2D&, Vector2D& );
        
		friend float Dot ( Vector2D&, Vector2D& );    
        
        friend Vector2D& Normalize ( Vector2D& );

        friend Vector2D& Reflect ( Vector2D&, Vector2D& );

        friend Vector2D& Refract ( Vector2D&, Vector2D&, float );
};

//-------------------------------------- Operators --------------------------------------

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

//----------------------------------- Common Functions -----------------------------------

inline Vector2D Abs ( const Vector2D& source )
{
	return Vector2D ( fabsf ( source.X ), fabsf ( source.Y ) );
}

inline Vector2D Sign ( const Vector2D& source )
{
	return Vector2D ( Util::Sign ( source.X ), Util::Sign ( source.Y ) );
}

inline Vector2D Floor ( const Vector2D& source )
{
	return Vector2D ( floorf ( source.X ), floorf ( source.Y ) );
}

inline Vector2D Fract ( const Vector2D& source )
{
	return Vector2D ( Util::Fract ( source.X ), Util::Fract ( source.Y ) );
}

inline Vector2D Ceil ( const Vector2D& source )
{
	return Vector2D ( ceilf ( source.X ), ceilf ( source.Y ) );
}

inline Vector2D Mod ( const Vector2D& left, const Vector2D& right )
{
	return Vector2D ( Util::Mod ( left.X, right.X ), Util::Mod ( left.Y, right.Y ) );
}

inline Vector2D Mod ( const Vector2D& left, const float right )
{
	return Vector2D ( Util::Mod ( left.X, right ), Util::Mod ( left.Y, right ) );
}

inline Vector2D Mod ( const float left, const Vector2D& right )
{
	return Vector2D ( Util::Mod ( left, right.X ), Util::Mod ( left, right.Y ) );
}




Vector2D Min ( const Vector2D&, const Vector2D& );

Vector2D Min ( const Vector2D&, float );

Vector2D Min ( const Vector2D&, const Vector2D& );

Vector2D Max ( const Vector2D&, const Vector2D& );

Vector2D Max ( const Vector2D&, float );

Vector2D Max ( const Vector2D&, const Vector2D& );

Vector2D Clamp ( const Vector2D&, const Vector2D&, const Vector2D& );

Vector2D Clamp ( const Vector2D&, float, float );

Vector2D Mix ( const Vector2D&, const Vector2D&, const Vector2D& );

Vector2D Mix ( const Vector2D&, const Vector2D&, float );

Vector2D Step ( const Vector2D&, const Vector2D& );

Vector2D Step ( const Vector2D&, float );

Vector2D Smooth ( const Vector2D&, const Vector2D&, const Vector2D& );

Vector2D Smooth ( const Vector2D&, float, float );

//----------------------------- Geometric Functions -----------------------------

float Length ( Vector2D& source )
{
	return sqrtf ( source.X * source.X + source.Y * source.Y );
}

float Square ( Vector2D& source )
{
	return source.X * source.X + source.Y * source.Y;
}

float Distance ( Vector2D& left, Vector2D& right )
{
    return Length ( left - right );
}

float Dot ( Vector2D& left, Vector2D& right )
{
	return left.X * right.X + left.Y * right.Y;
}        

Vector2D& Normalize ( Vector2D& source )
{
    return source / Length ( source );
}

Vector2D& Reflect ( Vector2D& incident, Vector2D& normal )
{
    return incident - 2.0F * Dot ( normal, incident ) * normal;
}

Vector2D& Refract ( Vector2D& incident, Vector2D& normal, float index )
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
