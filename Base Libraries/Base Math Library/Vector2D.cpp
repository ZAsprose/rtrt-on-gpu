#include <math.h>

#include <stdio.h>

#include "Vector2D.h"

#include "Util.h"

namespace Math
{
	//-------------------------------------- Public Constants -------------------------------------

	Vector2D Vector2D :: Zero ( 0.0F, 0.0F );

	Vector2D Vector2D :: Unit ( 1.0F, 1.0F );

	Vector2D Vector2D :: AxisX ( 1.0F, 0.0F );

	Vector2D Vector2D :: AxisY ( 0.0F, 1.0F );

	//------------------------------------ Arithmetic Operators -----------------------------------

	Vector2D operator + ( const Vector2D& source )
	{
		return Vector2D ( source.X, source.Y );
	}

	Vector2D operator - ( const Vector2D& source )
	{
		return Vector2D ( -source.X, -source.Y );
	}

	Vector2D operator + ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X + right.X, left.Y + right.Y );
	}

	Vector2D operator - ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X - right.X, left.Y - right.Y );
	}

	Vector2D operator + ( const Vector2D& left, float right )
	{
		return Vector2D ( left.X + right, left.Y + right );
	}

	Vector2D operator - ( const Vector2D& left, float right )
	{
		return Vector2D ( left.X - right, left.Y - right );
	}

	Vector2D operator + ( float left, const Vector2D& right )
	{
		return Vector2D ( left + right.X, left + right.Y );
	}

	Vector2D operator - ( float left, const Vector2D& right )
	{
		return Vector2D ( left - right.X, left - right.Y );
	}

	Vector2D operator * ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X * right.X, left.Y * right.Y );
	}

	Vector2D operator / ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X / right.X, left.Y / right.Y );
	}

	Vector2D operator * ( const Vector2D& left, float right )
	{
		return Vector2D ( left.X * right, left.Y * right );
	}

	Vector2D operator / ( const Vector2D& left, float right )
	{
		return Vector2D ( left.X / right, left.Y / right );
	}

	Vector2D operator * ( float left, const Vector2D& right )
	{
		return Vector2D ( left * right.X, left * right.Y );
	}

	Vector2D operator / ( float left, const Vector2D& right )
	{
		return Vector2D ( left / right.X, left / right.Y );
	}

	Vector2D operator += ( Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X += right.X, left.Y += right.Y );
	}

	Vector2D operator -= ( Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X -= right.X, left.Y -= right.Y );
	}

	Vector2D operator += ( Vector2D& left, float right )
	{
		return Vector2D ( left.X += right, left.Y += right );
	}

	Vector2D operator -= ( Vector2D& left, float right )
	{
		return Vector2D ( left.X -= right, left.Y -= right );
	}

	Vector2D operator *= ( Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X *= right.X, left.Y *= right.Y );
	}

	Vector2D operator /= ( Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X /= right.X, left.Y /= right.Y );
	}

	Vector2D operator *= ( Vector2D& left, float right )
	{
		return Vector2D ( left.X *= right, left.Y *= right );
	}

	Vector2D operator /= ( Vector2D& left, float right )
	{
		return Vector2D ( left.X /= right, left.Y /= right );
	}

	//-------------------------------------- Logic Operators --------------------------------------

	bool operator < ( const Vector2D& left, const Vector2D& right )
	{
		return left.X < right.X && left.Y < right.Y;
	}

	bool operator > ( const Vector2D& left, const Vector2D& right )
	{
		return left.X > right.X && left.Y > right.Y;
	}

	bool operator < ( const Vector2D& left, float right )
	{
		return left.X < right && left.Y < right;
	}

	bool operator > ( const Vector2D& left, float right )
	{
		return left.X > right && left.Y > right;
	}

	bool operator < ( float left, const Vector2D& right )
	{
		return left < right.X && left < right.Y;
	}

	bool operator > ( float left, const Vector2D& right )
	{
		return left > right.X && left > right.Y;
	}

	bool operator <= ( const Vector2D& left, const Vector2D& right )
	{
		return left.X <= right.X && left.Y <= right.Y;
	}

	bool operator >= ( const Vector2D& left, const Vector2D& right )
	{
		return left.X >= right.X && left.Y >= right.Y;
	}

	bool operator <= ( const Vector2D& left, float right )
	{
		return left.X <= right && left.Y <= right;
	}

	bool operator >= ( const Vector2D& left, float right )
	{
		return left.X >= right && left.Y >= right;
	}

	bool operator <= ( float left, const Vector2D& right )
	{
		return left <= right.X && left <= right.Y;
	}

	bool operator >= ( float left, const Vector2D& right )
	{
		return left >= right.X && left >= right.Y;
	}

	bool operator == ( const Vector2D& left, const Vector2D& right )
	{
		return left.X == right.X && left.Y == right.Y;
	}

	bool operator != ( const Vector2D& left, const Vector2D& right )
	{
		return left.X != right.X || left.Y != right.Y;
	}

	bool operator == ( const Vector2D& left, float right )
	{
		return left.X == right && left.Y == right;
	}

	bool operator != ( const Vector2D& left, float right )
	{
		return left.X != right || left.Y != right;
	}

	bool operator == ( float left, const Vector2D& right )
	{
		return left == right.X && left == right.Y;
	}

	bool operator != ( float left, const Vector2D& right )
	{
		return left != right.X || left != right.Y;
	}

	//------------------------------------- Common Functions --------------------------------------

	Vector2D Abs ( const Vector2D& source )
	{
		return Vector2D ( fabsf ( source.X ), fabsf ( source.Y ) );
	}

	Vector2D Sign ( const Vector2D& source )
	{
		return Vector2D ( Sign ( source.X ), Sign ( source.Y ) );
	}

	Vector2D Floor ( const Vector2D& source )
	{
		return Vector2D ( floorf ( source.X ), floorf ( source.Y ) );
	}

	Vector2D Fract ( const Vector2D& source )
	{
		return Vector2D ( Fract ( source.X ), Fract ( source.Y ) );
	}

	Vector2D Ceil ( const Vector2D& source )
	{
		return Vector2D ( ceilf ( source.X ), ceilf ( source.Y ) );
	}

	Vector2D Mod ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( Mod ( left.X, right.X ), Mod ( left.Y, right.Y ) );
	}

	Vector2D Mod ( const Vector2D& left, float right )
	{
		return Vector2D ( Mod ( left.X, right ), Mod ( left.Y, right ) );
	}

	Vector2D Mod ( float left, const Vector2D& right )
	{
		return Vector2D ( Mod ( left, right.X ), Mod ( left, right.Y ) );
	}

	Vector2D Min ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X < right.X ? left.X : right.X,
						  left.Y < right.Y ? left.Y : right.Y );
	}

	Vector2D Min ( const Vector2D& left, float right )
	{
		return Vector2D ( left.X < right ? left.X : right,
						  left.Y < right ? left.Y : right );
	}

	Vector2D Min ( float left, const Vector2D& right )
	{
		return Vector2D ( left < right.X ? left : right.X,
						  left < right.Y ? left : right.Y );
	}

	Vector2D Max ( const Vector2D& left, const Vector2D& right )
	{
		return Vector2D ( left.X > right.X ? left.X : right.X,
						  left.Y > right.Y ? left.Y : right.Y );
	}

	Vector2D Max ( const Vector2D& left, float right )
	{
		return Vector2D ( left.X > right ? left.X : right,
						  left.Y > right ? left.Y : right );
	}

	Vector2D Max ( float left, const Vector2D& right )
	{
		return Vector2D ( left > right.X ? left : right.X,
						  left > right.Y ? left : right.Y );
	}

	Vector2D Clamp ( const Vector2D& source, const Vector2D& min, const Vector2D& max )
	{
		return Vector2D ( Clamp ( source.X, min.X, max.X ), Clamp ( source.Y, min.Y, max.Y ) );
	}

	Vector2D Clamp ( const Vector2D& source, float min, float max )
	{
		return Vector2D ( Clamp ( source.X, min, max ), Clamp ( source.Y, min, max ) );
	}

	Vector2D Mix ( const Vector2D& left, const Vector2D& right, const Vector2D& alpha )
	{
		return Vector2D ( Mix ( left.X, right.X, alpha.X ), Mix ( left.Y, right.Y, alpha.Y ) );
	}

	Vector2D Mix ( const Vector2D& left, const Vector2D& right, float alpha )
	{
		return Vector2D ( Mix ( left.X, right.X, alpha ), Mix ( left.Y, right.Y, alpha ) );
	}

	Vector2D Step ( const Vector2D& source, const Vector2D& value )
	{
		return Vector2D ( Step ( source.X, value.X ), Step ( source.Y, value.Y ) );
	}

	Vector2D Step ( const Vector2D& source, float value )
	{
		return Vector2D ( Step ( source.X, value ), Step ( source.Y, value ) );
	}

	Vector2D Smooth ( const Vector2D& source, const Vector2D& min, const Vector2D& max )
	{
		return Vector2D ( Smooth ( source.X, min.X, max.X ), Smooth ( source.Y, min.Y, max.Y ) );
	}

	Vector2D Smooth ( const Vector2D& source, float min, float max )
	{
		return Vector2D ( Smooth ( source.X, min, max ), Smooth ( source.Y, min, max ) );
	}

	//------------------------------------ Geometric Functions ------------------------------------

	float Length ( const Vector2D& source )
	{
		return sqrtf ( source.X * source.X + source.Y * source.Y );
	}

	float Square ( const Vector2D& source )
	{
		return source.X * source.X + source.Y * source.Y;
	}

	float Distance ( const Vector2D& left, const Vector2D& right )
	{
		return Length ( left - right );
	}

	float Dot ( const Vector2D& left, const Vector2D& right )
	{
		return left.X * right.X + left.Y * right.Y;
	}        

	Vector2D Normalize ( const Vector2D& source )
	{
		return source / Length ( source );
	}

	Vector2D Reflect ( const Vector2D& incident, const Vector2D& normal )
	{
		return incident - 2.0F * Dot ( normal, incident ) * normal;
	}

	Vector2D Refract ( const Vector2D& incident, const Vector2D& normal, float index )
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

    Vector2D Pow ( const Vector2D& left, const Vector2D& right )
    {
		return Vector2D ( powf ( left.X, right.X ), powf ( left.Y, right.Y ) );
	}

    Vector2D Pow ( const Vector2D& left, float right )
    {
		return Vector2D ( powf ( left.X, right ), powf ( left.Y, right ) );
    }

    Vector2D Exp ( const Vector2D& source )
    {
		return Vector2D ( expf ( source.X ), expf ( source.Y ) );
    }

    Vector2D Log ( const Vector2D& source )
    {
		return Vector2D ( logf ( source.X ), logf ( source.Y ) );
    }

    Vector2D Sqrt ( const Vector2D& source )
    {
		return Vector2D ( sqrtf ( source.X ), sqrtf ( source.Y ) );
    }
}