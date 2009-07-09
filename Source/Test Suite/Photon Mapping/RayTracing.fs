#extension GL_ARB_texture_rectangle : enable

/**********************************************************************************************************************/
/*************************************************** DATA STRUCTURES **************************************************/
/**********************************************************************************************************************/

struct SCamera
{
	vec3 Position;
	
	vec3 Side;
	
	vec3 Up;
	
	vec3 View;
	
	vec2 Scale;
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
	
	vec3 Color;
};

//---------------------------------------------------------------------------------------------------------------------

struct SLight
{
	vec3 Position;

	vec3 Intens;
};

/**********************************************************************************************************************/
/************************************************** SHADER INTERFACE **************************************************/
/**********************************************************************************************************************/

uniform SCamera Camera;

uniform SLight Light;

uniform float Time;

uniform sampler2DRect PositionTexture;

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

const vec2 Size = vec2 ( 256.0 );

//---------------------------------------------------------------------------------------------------------------------

#define BIG 1000000.0

#define EPSILON 0.001

/**********************************************************************************************************************/
/************************************************* SUPPORT FUNCTIONS **************************************************/
/**********************************************************************************************************************/

SRay GenerateRay ( void )
{
	vec2 coords = gl_TexCoord[0].xy * Camera.Scale;
	
	vec3 direction = Camera.View - Camera.Side * coords.x + Camera.Up * coords.y;
   
	return SRay ( Camera.Position, normalize ( direction ) );
}

//---------------------------------------------------------------------------------------------------------------------

vec3 ChessBoardTexture ( vec3 firstColor, vec3 secondColor, vec2 coord )
{
	return mix ( firstColor,
	             secondColor,
				 ( sign ( ( coord.x - 0.5 ) * ( coord.y - 0.5 ) ) + 1.0 ) / 2.0 );
}

//---------------------------------------------------------------------------------------------------------------------

bool Compare ( vec3 a, vec3 b )
{
	bvec3 c = greaterThan ( a, b );
	
	bvec3 d = equal ( a, b );
	
	return c.x || ( d.x && c.y ) || ( d.x && d.y && c.z );
}

//---------------------------------------------------------------------------------------------------------------------

float BinSearch ( vec3 point )
{
	float a = 0.0;
	
	float b = Size.x * Size.x;
	
	float c;
    
    while ( a < b )
	{
        c = floor ( ( a + b ) / 2.0 );
        
        vec3 position = vec3 ( texture2DRect ( PositionTexture,
                                               vec2 ( mod ( c, Size.x ), floor ( c / Size.x ) ) ) ); 

        if ( Compare ( point, position ) )
        {
			a = c + 1.0;
		}        
        else
        {
			b = c - 1.0;
		}        
     }
	
	return c;
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
/************************************************* LIGHTING FUNCTIONS *************************************************/
/**********************************************************************************************************************/

#define DiffuseContribution 0.8

#define SpecularContribution 0.8

#define AmbientContribution 0.2

#define Epsilon	0.5

vec3 Phong ( vec3 lightpos, vec3 camerapos, vec3 point, vec3 normal, vec3 color )
{
	vec3 light = normalize ( lightpos - point );
	
	vec3 view = normalize ( camerapos - point );
	
	vec3 reflect = reflect ( -view, normal );
   
	float diffuse = abs ( dot ( light, normal ) );
	
	float specular = pow ( max ( dot ( view, reflect ), 0.0 ), 128.0 );  
	
	return DiffuseContribution * diffuse * color +
	       vec3 ( SpecularContribution * specular ) +
	       vec3 ( AmbientContribution ) * color;
}

/**********************************************************************************************************************/
/**************************************************** ENTRY POINT *****************************************************/
/**********************************************************************************************************************/

void main ( void )
{
	SRay ray = GenerateRay ( );
	
	vec3 result = Zero;
	
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
				
				vec3 point = ray.Origin + time * ray.Direction;
				
				vec3 color = ChessBoardTexture ( vec3 ( 0.0, 0.0, 1.0 ),
				                                 vec3 ( 1.0, 1.0, 0.0 ),
				                                 fract ( point.xy ) );
				
				result += Phong ( Light.Position, Camera.Position, point, AxisZ, color );
				
				//-----------------------------------------------------------------------------------------------------
				
                float a = BinSearch ( point - vec3 ( 0.2 ) );
                
                float b = BinSearch ( point + vec3 ( 0.2 ) );
                
                for ( float t = a; t <= b; ++t )
                {
					vec4 position = texture2DRect ( PositionTexture, vec2 ( mod ( t, Size.x ), floor ( t / Size.x ) ) );
					
					result += max ( 0.0, 1.0 - 5.0 * length ( vec3 ( position ) - point ) ) * vec3 ( position.w );
				}
			}
			else
			{
				vec3 point = ray.Origin + time * ray.Direction;
				
				vec3 normal = normalize ( CalcNormal ( point ) );
				
				result += Phong ( Light.Position,
				                  Camera.Position,
				                  point,
				                  normal,
				                  vec3 ( 0.3, 0.3, 0.6 ) );
				                  
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
					
					normal = AxisZ;
					
					vec3 color = ChessBoardTexture ( vec3 ( 0.0, 0.0, 1.0 ),
													 vec3 ( 1.0, 1.0, 0.0 ),
													 fract ( point.xy ) );
				
					result += Phong ( Light.Position, Camera.Position, point, AxisZ, color );
					
					//-----------------------------------------------------------------------------------------------------
					
					float a = BinSearch ( point - vec3 ( 0.2 ) );
	                
					float b = BinSearch ( point + vec3 ( 0.2 ) );
	                
					for ( float t = a; t <= b; ++t )
					{
						vec4 position = texture2DRect ( PositionTexture, vec2 ( mod ( t, Size.x ), floor ( t / Size.x ) ) );
						
						result += max ( 0.0, 1.0 - 5.0 * length ( vec3 ( position ) - point ) ) * vec3 ( position.w );
					}
				}				               
			}			
		}
		else
		{
			if ( IntersectPlane ( ray, start, final, AxisZ, BoxMinimum.z, current ) && current < time )
			{
				time = current;
				
				vec3 point = ray.Origin + time * ray.Direction;
				
				vec3 color = ChessBoardTexture ( vec3 ( 0.0, 0.0, 1.0 ),
				                                 vec3 ( 1.0, 1.0, 0.0 ),
				                                 fract ( point.xy ) );
				
				result += Phong ( Light.Position, Camera.Position, point, AxisZ, color );
				
				//-----------------------------------------------------------------------------------------------------
				
                float a = BinSearch ( point - vec3 ( 0.2 ) );
                
                float b = BinSearch ( point + vec3 ( 0.2 ) );
                
                for ( float t = a; t <= b; ++t )
                {
					vec4 position = texture2DRect ( PositionTexture, vec2 ( mod ( t, Size.x ), floor ( t / Size.x ) ) );
					
					result += max ( 0.0, 1.0 - 5.0 * length ( vec3 ( position ) - point ) ) * vec3 ( position.w );
				}
			}		
		}
	}
	
	gl_FragColor = vec4 ( result, 1.0 );
}