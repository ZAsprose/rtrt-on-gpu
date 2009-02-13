#include "Vector2D.h"

Vector2D::Vector2D(void)
{
}

Vector2D::~Vector2D(void)
{
}

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

inline Vector2D operator * ( const Vector2D& left, const Vector2D& right )
{
	return Vector2D ( left.X * right.X, left.Y * right.Y );
}

inline Vector2D operator * ( const Vector2D& left, float right )
{
	return Vector2D ( left.X * right, left.Y * right );
}

inline Vector2D operator * ( float left, const Vector2D& right )
{
	return Vector2D ( left * right.X, left * right.Y );
}

inline Vector2D operator / ( const Vector2D& left, const Vector2D& right )
{
	return Vector2D ( left.X / right.X, left.Y / right.Y );
}

inline Vector2D operator / ( const Vector2D& left, float right )
{
	return Vector2D ( left.X / right, left.Y / right );
}

inline Vector2D operator / ( float left, const Vector2D& right )
{
	return Vector2D ( left / right.X, left / right.Y );
}

inline bool operator < ( const Vector2D& left, const Vector2D& right )
{
	return left.X < right.X && left.Y < right.Y;
}

inline bool operator < ( const Vector2D& left, float right )
{
	return left.X < right && left.Y < right;
}

inline bool operator < ( float left, const Vector2D& right )
{
	return left < right.X && left < right.Y;
}

inline bool operator > ( const Vector2D& left, const Vector2D& right )
{
	return left.X > right.X && left.Y > right.Y;
}

inline bool operator > ( const Vector2D& left, float right )
{
	return left.X > right && left.Y > right;
}

inline bool operator > ( float left, const Vector2D& right )
{
	return left > right.X && left > right.Y;
}

inline bool operator <= ( const Vector2D& left, const Vector2D& right )
{
	return left.X <= right.X && left.Y <= right.Y;
}

inline bool operator <= ( const Vector2D& left, float right )
{
	return left.X <= right && left.Y <= right;
}

inline bool operator <= ( float left, const Vector2D& right )
{
	return left <= right.X && left <= right.Y;
}

inline bool operator >= ( const Vector2D& left, const Vector2D& right )
{
	return left.X >= right.X && left.Y >= right.Y;
}

inline bool operator >= ( const Vector2D& left, float right )
{
	return left.X >= right && left.Y >= right;
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
