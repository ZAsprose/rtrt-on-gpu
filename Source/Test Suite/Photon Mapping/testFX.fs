#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect PositionTexture;

uniform sampler2DRect IntensityTexture;

varying vec2 ScreenCoords;

const float epsilon = 0.2;

void main ( void )
{
	vec3 coords = vec3(20 * ScreenCoords.x, -5.0, 20 * ScreenCoords.y) ;
	
	vec2 temp;
	
	vec3 color = vec3(0.0);

	for ( int x = 0; x < 128; ++x )
	{
		for ( int y = 0 ; y < 128; ++y )
		{
			temp = vec2 ( x, y );

			vec3 pos = vec3(texture2DRect(PositionTexture, temp));

			//if ( length( pos - coords ) < epsilon )
				
			color += max ( 0.0, 1.0 - length( pos - coords ) ) *vec3(texture2DRect(IntensityTexture, temp ));
		}
	}
		
	gl_FragColor = vec4 ( color, 1.0 );
}
	