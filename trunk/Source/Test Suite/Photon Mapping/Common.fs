#extension GL_ARB_texture_rectangle : enable

/**********************************************************************************************************************/
/*************************************************** DATA STRUCTURES **************************************************/
/**********************************************************************************************************************/

struct SLight
{
	vec3 Position;
	
	vec2 Radius;
	
	float Distance;	
};

//---------------------------------------------------------------------------------------------------------------------

struct SRay
{
	vec3 Origin;
	
	vec3 Direction;
};

//---------------------------------------------------------------------------------------------------------------------

struct SIntersection
{
	float Time;
	
	vec3 Point;
	
	vec3 Normal;
};

/**********************************************************************************************************************/
/************************************************** SHADER INTERFACE **************************************************/
/**********************************************************************************************************************/

uniform SLight Light;

uniform float Time;

uniform float Depth;

uniform sampler2D NoiseTexture;

/**********************************************************************************************************************/
/************************************************** SHADER CONSTANTS **************************************************/
/**********************************************************************************************************************/

const vec3 Zero = vec3 ( 0.0, 0.0, 0.0 );

const vec3 Unit = vec3 ( 1.0, 1.0, 1.0 );

//---------------------------------------------------------------------------------------------------------------------

const vec3 AxisX = vec3 ( 1.0, 0.0, 0.0 );

const vec3 AxisY = vec3 ( 0.0, 1.0, 0.0 );

const vec3 AxisZ = vec3 ( 0.0, 0.0, 1.0 );

//---------------------------------------------------------------------------------------------------------------------

const vec3 BoxMinimum = vec3 ( -5.0 );

const vec3 BoxMaximum = vec3 ( 5.0 );

//---------------------------------------------------------------------------------------------------------------------

const float OpticalDensity = 1.5;

//---------------------------------------------------------------------------------------------------------------------

const vec2 Size = vec2 ( 128.0 );

//---------------------------------------------------------------------------------------------------------------------

#define BIG 1000000.0

#define EPSILON 0.001

/**********************************************************************************************************************/
/*********************************************** INTERSECTION FUNCTIONS ***********************************************/
/**********************************************************************************************************************/

float IntersectBox ( SRay ray, vec3 minimum, vec3 maximum )
{
	vec3 OMAX = ( minimum - ray.Origin ) / ray.Direction;
   
	vec3 OMIN = ( maximum - ray.Origin ) / ray.Direction;
	
	vec3 MAX = max ( OMAX, OMIN );
	
	return min ( MAX.x, min ( MAX.y, MAX.z ) );
}

//---------------------------------------------------------------------------------------------------------------------

bool IntersectBox ( SRay ray, vec3 minimum, vec3 maximum, out float start, out float final )
{
	vec3 OMAX = ( minimum - ray.Origin ) / ray.Direction;
	
	vec3 OMIN = ( maximum - ray.Origin ) / ray.Direction;
	
	vec3 MAX = max ( OMAX, OMIN );
	
	vec3 MIN = min ( OMAX, OMIN );
	
	final = min ( MAX.x, min ( MAX.y, MAX.z ) );
	
	start = max ( max ( MIN.x, 0.0), max ( MIN.y, MIN.z ) );	
	
	return final > start;
}

//---------------------------------------------------------------------------------------------------------------------

bool IntersectPlane ( SRay ray, float start, float final, vec3 normal, float distance, out float time )
{
	time = ( distance - dot ( normal, ray.Origin ) ) / dot ( normal, ray.Direction );
	
	return time >= start && time <= final;
}

//---------------------------------------------------------------------------------------------------------------------

#define intervals 100

float CalcFunction ( vec3 point )
{
	float x = point.x,
	      y = point.y,
	      z = point.z;
	
	//return z - 0.5 * sin ( ( x * x + y * y ) * 0.25 * ( 2.0 + sin ( Time ) ) ) - Depth;
	
	return z - sin ( ( x + y ) * 3.0 + Time ) * 0.1 - sin ( ( x - y ) * 3.0 + Time ) * 0.1 -
	           sin ( x * 6.0 + Time ) * 0.05 - sin ( y * 6.0 + Time ) * 0.05 - Depth;
}

vec3 CalcNormal ( vec3 point )
{
	float x = point.x,
	      y = point.y,
	      z = point.z;
	
	//return vec3 ( -0.125 * ( 2.0 + sin ( Time ) ) * x * cos ( ( x * x + y * y ) * 0.25 * ( 2.0 + sin ( Time ) ) ),
	//              -0.125 * ( 2.0 + sin ( Time ) ) * y * cos ( ( x * x + y * y ) * 0.25 * ( 2.0 + sin ( Time ) ) ),
	//               1.0 );
	
	return vec3 ( -cos ( ( x + y ) * 3.0 + Time ) * 0.3 - cos ( ( x - y ) * 3.0 + Time ) * 0.3 -
	               cos ( x * 6.0 + Time ) * 0.3,
	              -cos ( ( x + y ) * 3.0 + Time ) * 0.3 + cos ( ( x - y ) * 3.0 + Time ) * 0.3 -
	               cos ( y * 6.0 + Time ) * 0.3,
	               1.0 );
}

bool IntersectSurface ( SRay ray, float tmin, float tmax, out float val )
{
	float step = ( tmax - tmin ) / intervals;
	
	float left = CalcFunction ( ray.Origin + tmin * ray.Direction );

	float right = 0.0;
	
	for ( int i = 0; i < intervals; i++ )
	{		
		float t = tmin + i * step;
		
		vec3 point = ray.Origin + t * ray.Direction;
		
		right = CalcFunction ( point );
		
		if ( left * right < 0.0 )
		{
			val = t + ( right * step) / ( left - right );
			
			return true;
		}
		
		left = right;
	}

	return false;
}
