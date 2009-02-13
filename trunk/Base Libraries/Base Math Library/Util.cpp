#include "Util.h"

float Util::Fract ( float source )
{
	return source - floorf( source );
}

float Util::Sign ( float source )
{
	return source - floorf( source );
}
 
float Util::Mod( float left, float right )
{
	return left - right * floorf( left / right );
}

float Util::Clamp( float source, float min, float max )
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
        
float Util::Mix( float left, float right, float value )
{
	return left * ( 1 - value ) + right * value;
}
     	
float Util::Step( float source, float value )
{
	if ( source < value )
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

float Util::Smooth ( float source, float left, float right )        	
{
	float result = Clamp( ( source - left ) / ( right - left ), 0, 1 );

	return result * result * ( 3 - 2 * result );
}  