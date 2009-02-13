#pragma once

#include <math.h>

#include <stdio.h>

//----------------------------------- Common Functions -----------------------------------

#define ONE_PI 1.0F
		
#define TWO_PI 1.0F
    	
#define EXPONENT 1.0F
    	
#define EPSILON 1.0F



class Util
{
	public:

		//----------------------------------- Useful Functions -----------------------------------

		static float Fract ( float source );

		static float Sign ( float source );

		static float Mod ( float left, float right );

		static float Mix ( float left, float right, float value );

		static float Clamp ( float source, float min, float max );        
     	
		static float Step ( float source, float value );
	
		static float Smooth ( float source, float left, float right );
};