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

#define BIG 1000000.0

#define EPSILON 0.001

/**********************************************************************************************************************/
/************************************************* SUPPORT FUNCTIONS **************************************************/
/**********************************************************************************************************************/

SRay GenerateRay ( void )
{
	vec3 direction = AxisX * gl_TexCoord[0].x * Light.Radius.x +
	                 AxisY * gl_TexCoord[0].y * Light.Radius.y -
	                 AxisZ * Light.Distance;
   
	return SRay ( Light.Position, normalize ( direction ) );
}

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
	
	//return z - 0.5 * sin ( ( x * x + y * y ) * 0.25 * ( 2.0 + sin ( Time ) ) ) + 2.0;
	
	return z - sin ( x + y + Time ) * 0.3 - sin ( x - y + Time ) * 0.3 + 2.0;
}

vec3 CalcNormal ( vec3 point )
{
	float x = point.x,
	      y = point.y,
	      z = point.z;
	
	//return vec3 ( -0.125 * ( 2.0 + sin ( Time ) ) * x * cos ( ( x * x + y * y ) * 0.25 * ( 2.0 + sin ( Time ) ) ),
	//              -0.125 * ( 2.0 + sin ( Time ) ) * y * cos ( ( x * x + y * y ) * 0.25 * ( 2.0 + sin ( Time ) ) ),
	//              1.0 );
	
	return vec3 ( -cos ( x + y + Time ) * 0.3 - cos ( x - y + Time ) * 0.3,
	              -cos ( x + y + Time ) * 0.3 + cos ( x - y + Time ) * 0.3,
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

/**********************************************************************************************************************/
/**************************************************** ENTRY POINT *****************************************************/
/**********************************************************************************************************************/

void main ( void )
{
	SRay ray = GenerateRay ( );
		
	vec3 point = vec3 ( BIG );
	
	//-----------------------------------------------------------------------------------------------------------------
	
	float start, final, current, time = BIG;
    
	if ( IntersectBox ( ray, BoxMinimum, BoxMaximum, start, final ) )
	{
		if ( IntersectSurface ( ray, start, final, current ) && current < time )
		{
			time = current;
			
			if ( IntersectPlane ( ray, start, final, AxisZ, BoxMinimum.z, current ) && current < time )
			{
				time = current;
				
				point = ray.Origin + time * ray.Direction;
			}
			else
			{
				point = ray.Origin + time * ray.Direction;
				
				vec3 normal = normalize ( CalcNormal ( point ) );
				                  
				//-----------------------------------------------------------------------------------------------------
				                  
				vec3 refract = refract ( ray.Direction,
				                         normal,
				                         1.0 / OpticalDensity );
				
				ray = SRay ( point + refract * EPSILON, refract );
				
				final = IntersectBox ( ray, BoxMinimum, BoxMaximum );
				
				time = BIG;
				
				if ( IntersectPlane ( ray, 0.0, final, AxisZ, BoxMinimum.z, current ) && current < time )
				{
					time = current;
					
					point = ray.Origin + time * ray.Direction;
				}				               
			}			
		}
		else
		{
			if ( IntersectPlane ( ray, start, final, AxisZ, BoxMinimum.z, current ) && current < time )
			{
				time = current;
				
				point = ray.Origin + time * ray.Direction;
			}		
		}
	}
	
	//-----------------------------------------------------------------------------------------------------------------
	
	gl_FragColor = vec4 ( point, 0.025 );
}