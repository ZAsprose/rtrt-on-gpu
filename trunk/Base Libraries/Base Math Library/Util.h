#pragma once

#include <math.h>

#include <stdio.h>

//----------------------------------- Useful Constants -----------------------------------

#define ONE_PI 3.141592653589F
		
#define TWO_PI 6.283185307179F
    	
#define EXPONENT 2.718281828459F
    	
#define EPSILON 0.0001F

//----------------------------------- Useful Functions -----------------------------------

namespace Math
{
    inline float Degrees ( float radian )
	{
		return 180.0F * radian / ONE_PI;
	}
    	
	inline float Radians ( float degree )
	{
		return ONE_PI * degree / 180.0F;
	}  

	inline float Sign ( float source )
	{
		if ( source < 0.0F )
		{
			return -1.0F;
		}
		else
		{
			if ( source > 0.0F )
			{
				return 1.0F;
			}
			else
			{
				return 0.0F;
			}			
		}		
	}

	inline float Fract ( float source )
	{
		return source - floorf( source );
	}
	 
	inline float Mod( float left, float right )
	{
		return left - right * floorf( left / right );
	}

	inline float Clamp( float source, float min, float max )
	{
		if ( source < min )
		{
			return min;
		}
		else
		{
			if ( source > max )
			{
				return max;
			}
			else
			{
				return source;
			}
		}
	}
	        
	inline float Mix( float left, float right, float value )
	{
		return left * ( 1.0F - value ) + right * value;
	}
	     	
	inline float Step( float source, float value )
	{
		if ( source < value )
		{
			return 0.0F;
		}
		else
		{
			return 1.0F;
		}
	}

	inline float Smooth ( float source, float left, float right )        	
	{
		float result = Clamp( ( source - left ) / ( right - left ), 0.0F, 1.0F );

		return result * result * ( 3.0F - 2.0F * result );
	} 
}