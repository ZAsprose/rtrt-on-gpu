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

/**********************************************************************************************************************/
/************************************************** SHADER INTERFACE **************************************************/
/**********************************************************************************************************************/

uniform SCamera Camera;

uniform sampler2DRect PositionTexture;

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
				 
	//return vec3 ( texture2D ( NoiseTexture, coord ).a + 1.0 ) / 2.0;
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

const float DELTA = 0.2;

const float INVDELTA = 1.0 / DELTA;

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
				
				result += Phong ( Light.Position, Camera.Position, point, AxisZ, color * 0.6 );
				
				//-----------------------------------------------------------------------------------------------------
				
                float a = BinSearch ( point - vec3 ( DELTA ) );
                
                float b = BinSearch ( point + vec3 ( DELTA ) );
                
                for ( float t = a; t <= b; ++t )
                {
					vec4 position = texture2DRect ( PositionTexture, vec2 ( mod ( t, Size.x ), floor ( t / Size.x ) ) );
					
					result += max ( 0.0, 1.0 - INVDELTA * length ( vec3 ( position ) - point ) ) * vec3 ( position.w );
					//result += smoothstep ( 0.0, 1.0, 1.0 - INVDELTA * length ( vec3 ( position ) - point ) ) * vec3 ( position.w );
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
				
					result += Phong ( Light.Position, Camera.Position, point, AxisZ, color * 0.6 );
					
					//-----------------------------------------------------------------------------------------------------
					
					float a = BinSearch ( point - vec3 ( DELTA ) );
	                
					float b = BinSearch ( point + vec3 ( DELTA ) );
	                
					for ( float t = a; t <= b; ++t )
					{
						vec4 position = texture2DRect ( PositionTexture, vec2 ( mod ( t, Size.x ), floor ( t / Size.x ) ) );
						
						result += max ( 0.0, 1.0 - INVDELTA * length ( vec3 ( position ) - point ) ) * vec3 ( position.w );
						//result += smoothstep ( 0.0, 1.0, 1.0 - INVDELTA * length ( vec3 ( position ) - point ) ) * vec3 ( position.w );
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
				
				result += Phong ( Light.Position, Camera.Position, point, AxisZ, color * 0.6 );
				
				//-----------------------------------------------------------------------------------------------------
				
                float a = BinSearch ( point - vec3 ( DELTA ) );
                
                float b = BinSearch ( point + vec3 ( DELTA ) );
                
                for ( float t = a; t <= b; ++t )
                {
					vec4 position = texture2DRect ( PositionTexture, vec2 ( mod ( t, Size.x ), floor ( t / Size.x ) ) );
					
					result += max ( 0.0, 1.0 - INVDELTA * length ( vec3 ( position ) - point ) ) * vec3 ( position.w );
					//result += smoothstep ( 0.0, 1.0, 1.0 - INVDELTA * length ( vec3 ( position ) - point ) ) * vec3 ( position.w );
				}
			}		
		}
	}
	
	gl_FragColor = vec4 ( result, 1.0 );
}