#pragma once

#ifndef _MATH_UTIL_

#define _MATH_UTIL_

//--------------------------------------- Useful Constants ----------------------------------------

#define ONEPI 3.141592653589F
		
#define TWOPI 6.283185307179F
    	
#define EXPONENT 2.718281828459F
    	
#define EPSILON 0.0001F

//-------------------------------------------------------------------------------------------------

#define SIZE2D 2

#define SIZE3D 3

#define SIZE4D 4

//-------------------------------------------------------------------------------------------------

#define A 0

#define B 1

#define C 2

#define D 3

//--------------------------------------- Useful Functions ----------------------------------------

namespace Math
{
    float Degrees ( float );
    	
	float Radians ( float );

	float Sign ( float );

	float Fract ( float );
	 
	float Mod ( float, float );

	float Clamp ( float, float, float );
	        
	float Mix ( float, float, float );
	     	
	float Step ( float, float );

	float Smooth ( float, float, float );
}

#endif