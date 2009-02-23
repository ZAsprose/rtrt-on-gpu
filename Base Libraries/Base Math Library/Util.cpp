#include <math.h>

#include <stdio.h>

#include "Util.h"

namespace Math
{
    float Degrees ( float radian )
	{
		return 180.0F * radian / ONE_PI;
	}
    	
	float Radians ( float degree )
	{
		return ONE_PI * degree / 180.0F;
	}  

	float Sign ( float source )
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

	float Fract ( float source )
	{
		return source - floorf( source );
	}
	 
	float Mod ( float left, float right )
	{
		return left - right * floorf( left / right );
	}

	float Clamp ( float source, float min, float max )
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
	        
	float Mix ( float left, float right, float value )
	{
		return left * ( 1.0F - value ) + right * value;
	}
	     	
	float Step ( float source, float value )
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

	float Smooth ( float source, float left, float right )        	
	{
		float result = Clamp( ( source - left ) / ( right - left ), 0.0F, 1.0F );

		return result * result * ( 3.0F - 2.0F * result );
	} 
}