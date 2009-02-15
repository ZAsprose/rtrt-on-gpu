#include <math.h>

#include <stdio.h>

#include "Vector3D.h"

#include "Util.h"

namespace Math
{
	//-------------------------------------- Public Constants -------------------------------------

	Vector3D Vector3D :: Zero ( 0.0F, 0.0F, 0.0F );

	Vector3D Vector3D :: Unit ( 1.0F, 1.0F, 1.0F );

	Vector3D Vector3D :: AxisX ( 1.0F, 0.0F, 0.0F );

	Vector3D Vector3D :: AxisY ( 0.0F, 1.0F, 0.0F );

	Vector3D Vector3D :: AxisZ ( 0.0F, 1.0F, 1.0F );

	//------------------------------------ Arithmetic Operators -----------------------------------

	Vector3D operator + ( const Vector3D& source )
	{
		return Vector3D ( source.X, source.Y, source.Z );
	}

	Vector3D operator - ( const Vector3D& source )
	{
		return Vector3D ( -source.X, -source.Y, -source.Z );
	}

	Vector3D operator + ( const Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X + right.X, left.Y + right.Y, left.Z + right.Z );
	}

	Vector3D operator - ( const Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X - right.X, left.Y - right.Y, left.Z - right.Z );
	}

	Vector3D operator + ( const Vector3D& left, float right )
	{
		return Vector3D ( left.X + right, left.Y + right, left.Z + right );
	}

	Vector3D operator - ( const Vector3D& left, float right )
	{
		return Vector3D ( left.X - right, left.Y - right, left.Z - right );
	}

	Vector3D operator + ( float left, const Vector3D& right )
	{
		return Vector3D ( left + right.X, left + right.Y, left + right.Z );
	}

	Vector3D operator - ( float left, const Vector3D& right )
	{
		return Vector3D ( left - right.X, left - right.Y, left - right.Z );
	}

	Vector3D operator * ( const Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X * right.X, left.Y * right.Y, left.Z * right.Z );
	}

	Vector3D operator / ( const Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X / right.X, left.Y / right.Y, left.Z / right.Z );
	}

	Vector3D operator * ( const Vector3D& left, float right )
	{
		return Vector3D ( left.X * right, left.Y * right, left.Z / right );
	}

	Vector3D operator / ( const Vector3D& left, float right )
	{
		return Vector3D ( left.X / right, left.Y / right, left.Z / right );
	}

	Vector3D operator * ( float left, const Vector3D& right )
	{
		return Vector3D ( left * right.X, left * right.Y, left * right.Z );
	}

	Vector3D operator / ( float left, const Vector3D& right )
	{
		return Vector3D ( left / right.X, left / right.Y, left / right.Z );
	}

	Vector3D operator += ( Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X += right.X, left.Y += right.Y, left.Z += right.Z );
	}

	Vector3D operator -= ( Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X -= right.X, left.Y -= right.X, left.Z -= right.Z );
	}

	Vector3D operator += ( Vector3D& left, float right )
	{
		return Vector3D ( left.X += right, left.Y += right, left.Z += right );
	}

	Vector3D operator -= ( Vector3D& left, float right )
	{
		return Vector3D ( left.X -= right, left.Y -= right, left.Z -= right );
	}

	Vector3D operator *= ( Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X *= right.X, left.Y *= right.Y, left.Z *= right.Z );
	}

	Vector3D operator /= ( Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X /= right.X, left.Y /= right.X, left.Z /= right.Z );
	}

	Vector3D operator *= ( Vector3D& left, float right )
	{
		return Vector3D ( left.X *= right, left.Y *= right, left.Z *= right );
	}

	Vector3D operator /= ( Vector3D& left, float right )
	{
		return Vector3D ( left.X /= right, left.Y /= right, left.Z /= right );
	}

	//-------------------------------------- Logic Operators --------------------------------------

	bool operator < ( const Vector3D& left, const Vector3D& right )
	{
		return left.X < right.X && left.Y < right.Y && left.Z < right.Z;
	}

	bool operator > ( const Vector3D& left, const Vector3D& right )
	{
		return left.X > right.X && left.Y > right.Y && left.Z > right.Z;
	}

	bool operator < ( const Vector3D& left, float right )
	{
		return left.X < right && left.Y < right && left.Z < right;
	}

	bool operator > ( const Vector3D& left, float right )
	{
		return left.X > right && left.Y > right && left.Z > right;
	}

	bool operator < ( float left, const Vector3D& right )
	{
		return left < right.X && left < right.Y && left < right.Z;
	}

	bool operator > ( float left, const Vector3D& right )
	{
		return left > right.X && left > right.Y && left > right.Z;
	}

	bool operator <= ( const Vector3D& left, const Vector3D& right )
	{
		return left.X <= right.X && left.Y <= right.Y && left.Z <= right.Z;
	}

	bool operator >= ( const Vector3D& left, const Vector3D& right )
	{
		return left.X >= right.X && left.Y >= right.Y && left.Z >= right.Z;
	}

	bool operator <= ( const Vector3D& left, float right )
	{
		return left.X <= right && left.Y <= right && left.Z <= right;
	}

	bool operator >= ( const Vector3D& left, float right )
	{
		return left.X >= right && left.Y >= right && left.Z >= right;
	}

	bool operator <= ( float left, const Vector3D& right )
	{
		return left <= right.X && left <= right.Y && left <= right.Z;
	}

	bool operator >= ( float left, const Vector3D& right )
	{
		return left >= right.X && left >= right.Y && left >= right.Z;
	}

	bool operator == ( const Vector3D& left, const Vector3D& right )
	{
		return left.X == right.X && left.Y == right.Y && left.Z == right.Z;
	}

	bool operator != ( const Vector3D& left, const Vector3D& right )
	{
		return left.X != right.X || left.Y != right.Y || left.Z != right.Z;
	}

	bool operator == ( const Vector3D& left, float right )
	{
		return left.X == right && left.Y == right && left.Z == right;
	}

	bool operator != ( const Vector3D& left, float right )
	{
		return left.X != right || left.Y != right || left.Z != right;
	}

	bool operator == ( float left, const Vector3D& right )
	{
		return left == right.X && left == right.Y && left == right.Z;
	}

	bool operator != ( float left, const Vector3D& right )
	{
		return left != right.X || left != right.Y || left != right.Z;
	}

	//------------------------------------- Common Functions --------------------------------------

	Vector3D Abs ( const Vector3D& source )
	{
		return Vector3D ( fabsf ( source.X ), fabsf ( source.Y ), fabsf ( source.Z ) );
	}

	Vector3D Sign ( const Vector3D& source )
	{
		return Vector3D ( Sign ( source.X ), Sign ( source.Y ), Sign ( source.Z ) );
	}

	Vector3D Floor ( const Vector3D& source )
	{
		return Vector3D ( floorf ( source.X ), floorf ( source.Y ), floorf ( source.Z ) );
	}

	Vector3D Fract ( const Vector3D& source )
	{
		return Vector3D ( Fract ( source.X ), Fract ( source.Y ), Fract ( source.Z ) );
	}

	Vector3D Ceil ( const Vector3D& source )
	{
		return Vector3D ( ceilf ( source.X ), ceilf ( source.Y ), ceilf ( source.Z ) );
	}

	Vector3D Mod ( const Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( Mod ( left.X, right.X ), Mod ( left.Y, right.Y ), Mod ( left.Z, right.Z ) );
	}

	Vector3D Mod ( const Vector3D& left, float right )
	{
		return Vector3D ( Mod ( left.X, right ), Mod ( left.Y, right ), Mod ( left.Z, right ) );
	}

	Vector3D Mod ( float left, const Vector3D& right )
	{
		return Vector3D ( Mod ( left, right.X ), Mod ( left, right.Y ), Mod ( left, right.Z ) );
	}

	Vector3D Min ( const Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X < right.X ? left.X : right.X,
						  left.Y < right.Y ? left.Y : right.Y,
						  left.Z < right.Z ? left.Z : right.Z );
	}

	Vector3D Min ( const Vector3D& left, float right )
	{
		return Vector3D ( left.X < right ? left.X : right,
						  left.Y < right ? left.Y : right,
						  left.Z < right ? left.Z : right );
	}

	Vector3D Min ( float left, const Vector3D& right )
	{
		return Vector3D ( left < right.X ? left : right.X,
						  left < right.Y ? left : right.Y,
						  left < right.Z ? left : right.Z );
	}

	Vector3D Max ( const Vector3D& left, const Vector3D& right )
	{
		return Vector3D ( left.X > right.X ? left.X : right.X,
						  left.Y > right.Y ? left.Y : right.Y,
						  left.Z > right.Z ? left.Z : right.Z );
	}

	Vector3D Max ( const Vector3D& left, float right )
	{
		return Vector3D ( left.X > right ? left.X : right,
						  left.Y > right ? left.Y : right,
						  left.Z > right ? left.Z : right );
	}

	Vector3D Max ( float left, const Vector3D& right )
	{
		return Vector3D ( left > right.X ? left : right.X,
						  left > right.Y ? left : right.Y,
						  left > right.Z ? left : right.Z );
	}

	Vector3D Clamp ( const Vector3D& source, const Vector3D& min, const Vector3D& max )
	{
		return Vector3D ( Clamp ( source.X, min.X, max.X ),
			              Clamp ( source.Y, min.Y, max.Y ),
						  Clamp ( source.Z, min.Z, max.Z ) );
	}

	Vector3D Clamp ( const Vector3D& source, float min, float max )
	{
		return Vector3D ( Clamp ( source.X, min, max ),
			              Clamp ( source.Y, min, max ),
						  Clamp ( source.Z, min, max ) );
	}

	Vector3D Mix ( const Vector3D& left, const Vector3D& right, const Vector3D& alpha )
	{
		return Vector3D ( Mix ( left.X, right.X, alpha.X ),
			              Mix ( left.Y, right.Y, alpha.Y ),
						  Mix ( left.Z, right.Z, alpha.Z ) );
	}

	Vector3D Mix ( const Vector3D& left, const Vector3D& right, float alpha )
	{
		return Vector3D ( Mix ( left.X, right.X, alpha ),
			              Mix ( left.Y, right.Y, alpha ),
						  Mix ( left.Z, right.Z, alpha ) );
	}

	Vector3D Step ( const Vector3D& source, const Vector3D& value )
	{
		return Vector3D ( Step ( source.X, value.X ),
			              Step ( source.Y, value.Y ),
						  Step ( source.Z, value.Z ) );
	}

	Vector3D Step ( const Vector3D& source, float value )
	{
		return Vector3D ( Step ( source.X, value ),
			              Step ( source.Y, value ),
						  Step ( source.Z, value ) );
	}

	Vector3D Smooth ( const Vector3D& source, const Vector3D& min, const Vector3D& max )
	{
		return Vector3D ( Smooth ( source.X, min.X, max.X ),
			              Smooth ( source.Y, min.Y, max.Y ),
						  Smooth ( source.Z, min.Z, max.Z ) );
	}

	Vector3D Smooth ( const Vector3D& source, float min, float max )
	{
		return Vector3D ( Smooth ( source.X, min, max ),
			              Smooth ( source.Y, min, max ),
						  Smooth ( source.Z, min, max ) );
	}

	//------------------------------------ Geometric Functions ------------------------------------

	float Length ( Vector3D& source )
	{
		return sqrtf ( source.X * source.X + source.Y * source.Y + source.Z * source.Z );
	}

	float Square ( Vector3D& source )
	{
		return source.X * source.X + source.Y * source.Y + source.Z * source.Z;
	}

	float Distance ( Vector3D& left, Vector3D& right )
	{
		return Length ( left - right );
	}

	float Dot ( Vector3D& left, Vector3D& right )
	{
		return left.X * right.X + left.Y * right.Y + left.Z * right.Z;
	}        

	Vector3D Normalize ( Vector3D& source )
	{
		return source / Length ( source );
	}

	Vector3D Reflect ( Vector3D& incident, Vector3D& normal )
	{
		return incident - 2.0F * Dot ( normal, incident ) * normal;
	}

	Vector3D Refract ( Vector3D& incident, Vector3D& normal, float index )
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

    Vector3D Pow ( Vector3D& left, Vector3D& right )
    {
		return Vector3D ( powf ( left.X, right.X ),
			              powf ( left.Y, right.Y ),
						  powf ( left.Z, right.Z ) );
	}

    Vector3D Pow ( Vector3D& left, float right )
    {
		return Vector3D ( powf ( left.X, right ),
			              powf ( left.Y, right ),
						  powf ( left.Z, right ) );
    }

    Vector3D Exp ( Vector3D& source )
    {
		return Vector3D ( expf ( source.X ),
			              expf ( source.Y ),
						  expf ( source.Z ) );
    }

    Vector3D Log ( Vector3D& source )
    {
		return Vector3D ( logf ( source.X ),
			              logf ( source.Y ),
						  logf ( source.Z ) );
    }

    Vector3D Sqrt ( Vector3D& source )
    {
		return Vector3D ( sqrtf ( source.X ),
			              sqrtf ( source.Y ),
						  sqrtf ( source.Z ) );
    }
}