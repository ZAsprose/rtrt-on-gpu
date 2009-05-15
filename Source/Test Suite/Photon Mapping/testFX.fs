#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect PositionTexture;

uniform sampler2DRect IntensityTexture;

varying vec2 ScreenCoords;

const float epsilon = 0.2;

void main ( void )
{
	vec3 coords = vec3(20 * ScreenCoords.x, -5.0, 20 * ScreenCoords.y) ;
	
	vec3 pos = vec3 ( texture2DRect ( PositionTexture, 0.5 * 256.0 * ( ScreenCoords + vec2 ( 1.0 ) ) ) );

	//vec3 color = vec3 ( texture2DRect ( IntensityTexture, temp ) );

	gl_FragColor = vec4 ( ( pos + vec3(30.0) ) / 60.0, 1.0 );
}
	