/*************************************************************************************************/
/*                       Fragment Shader for Sphere Texturing and Lighting                       */	
/*************************************************************************************************/

varying float Intensity;

void main ( void )
{
	vec4 color = vec4 ( 1.0, 1.0, 1.0, 1.0 );

	vec2 texcoord = fract ( 8.0 * gl_TexCoord [0].xy ) - vec2 ( 0.5 );

	if ( texcoord.y > abs ( texcoord.x ) || texcoord.y < -abs ( texcoord.x ) )
	{
		color = vec4 ( 1.0, 0.0, 0.0, 0.0 );
	}

	gl_FragColor = color * vec4 ( Intensity );
}