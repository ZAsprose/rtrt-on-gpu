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

		//----------------------------- Operators -----------------------------

		friend Vector2D operator + ( const Vector2D& );
		
		friend Vector2D operator - ( const Vector2D& );

		friend Vector2D operator + ( const Vector2D&, const Vector2D& );
		
		friend Vector2D operator - ( const Vector2D&, const Vector2D& );
		
		friend Vector2D operator * ( const Vector2D&, const Vector2D& );
		
		friend Vector2D operator * ( const Vector2D&, float );

		friend Vector2D operator * ( float, const Vector2D& );		

		friend Vector2D operator / ( const Vector2D&, const Vector2D& );
		
		friend Vector2D operator / ( const Vector2D&, float );		

		friend Vector2D operator / ( float, const Vector2D& );

		friend bool operator < ( const Vector2D&, const Vector2D& );

		friend bool operator < ( const Vector2D&, float );

		friend bool operator < ( float, const Vector2D& );

		friend bool operator > ( const Vector2D&, const Vector2D& );

		friend bool operator > ( const Vector2D&, float );

		friend bool operator > ( float, const Vector2D& );

		friend bool operator <= ( const Vector2D&, const Vector2D& );

		friend bool operator <= ( const Vector2D&, float );

		friend bool operator <= ( float, const Vector2D& );

		friend bool operator >= ( const Vector2D&, const Vector2D& );

		friend bool operator >= ( const Vector2D&, float );

		friend bool operator >= ( float, const Vector2D& );

		friend bool operator == ( const Vector2D&, const Vector2D& );

		friend bool operator != ( const Vector2D&, const Vector2D& );

		//----------------------------- Common Functions -----------------------------

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
};

//----------------------------- Operators -----------------------------

Vector2D operator + ( const Vector2D& );
		
Vector2D operator - ( const Vector2D& );

Vector2D operator + ( const Vector2D&, const Vector2D& );
		
Vector2D operator - ( const Vector2D&, const Vector2D& );
		
Vector2D operator * ( const Vector2D&, const Vector2D& );
		
Vector2D operator * ( const Vector2D&, float );

Vector2D operator * ( float, const Vector2D& );		

Vector2D operator / ( const Vector2D&, const Vector2D& );
		
Vector2D operator / ( const Vector2D&, float );		

Vector2D operator / ( float, const Vector2D& );

bool operator < ( const Vector2D&, const Vector2D& );

bool operator < ( const Vector2D&, float );

bool operator < ( float, const Vector2D& );

bool operator > ( const Vector2D&, const Vector2D& );

bool operator > ( const Vector2D&, float );

bool operator > ( float, const Vector2D& );

bool operator <= ( const Vector2D&, const Vector2D& );

bool operator <= ( const Vector2D&, float );

bool operator <= ( float, const Vector2D& );

bool operator >= ( const Vector2D&, const Vector2D& );

bool operator >= ( const Vector2D&, float );

bool operator >= ( float, const Vector2D& );

bool operator == ( const Vector2D&, const Vector2D& );

bool operator != ( const Vector2D&, const Vector2D& );

//----------------------------- Common Functions -----------------------------

Vector2D Abs ( const Vector2D& );

Vector2D Sign ( const Vector2D& );

Vector2D Floor ( const Vector2D& );

Vector2D Fract ( const Vector2D& );

Vector2D Ceiling ( const Vector2D& );

Vector2D Mod ( const Vector2D&, const Vector2D& );

Vector2D Mod ( const Vector2D&, float );

Vector2D Mod ( float, const Vector2D& );

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