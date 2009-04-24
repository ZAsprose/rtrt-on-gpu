#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect PositionTexture;

uniform sampler2DRect IntensityTexture;

varying vec2 ScreenCoords;

const float epsilon = 0.2;

void main ( void )
{
	vec3 coords = vec3(20 * ScreenCoords.x, -5.0, 20 * ScreenCoords.y) ;
	
	int i,j;

	vec2 temp;
	
	vec3 color = vec3(0.0);

	for ( i = 0; i < 256; ++i )

		for ( j = 0 ; j < 256; ++j )

		{
			temp = vec2( i,j );

			vec3 pos = vec3(texture2DRect(PositionTexture, temp));

			if ( length( pos - coords ) < epsilon )
				
				color += vec3(texture2DRect(IntensityTexture, temp ));
		}
		
	gl_FragColor = vec4 ( color, 1.0 );
}
	