#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect TestTexture;

void main ( void )
{
	vec4 color = texture2DRect ( TestTexture, vec2( 10.0, 10.0 ) );

	gl_FragColor = color;
}
