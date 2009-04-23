#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect PositionTexture;

uniform sampler2DRect IntensityTexture;

varying vec2 ScreenCoords;

const float epsilon = 0.5;

void main ( void )
{
	vec3 coords = vec3(20 * ScreenCoords.x, -5.0, 20 * ScreenCoords.y) ;
	
	int i,j;

	vec2 temp;
	
	vec3 color = vec3(0.0);

	for ( i = 0; i < 128; ++i )

		for ( j = 0 ; j < 128; ++j )

		{
			temp = vec2( i,j );

			vec3 pos = vec3(texture2DRect(PositionTexture, temp));//тут не вычисляет текстурные координаты

			if ( length( pos - coords ) < epsilon )
				
				color += texture2DRect(IntensityTexture, temp );//тут не считает color
		}
		
	gl_FragColor = vec4 ( color, 1.0 );
}
	