#pragma once

//----------------------------------- Useful Constants -----------------------------------

#define ONE_PI 3.141592653589F
		
#define TWO_PI 6.283185307179F
    	
#define EXPONENT 2.718281828459F
    	
#define EPSILON 0.0001F

//----------------------------------- Useful Functions -----------------------------------

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