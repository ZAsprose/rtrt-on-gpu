#extension GL_ARB_texture_rectangle : enable

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

uniform sampler2DRect CurrentPositionTexture;

uniform vec3 UpDirection;

uniform vec3 RightDirection;

varying vec4 FragmentColor;

varying vec2 FragmentPosition;

const float SizeScale = 0.005;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

void main ( void )
{
	vec4 position = texture2DRect ( CurrentPositionTexture, gl_MultiTexCoord0.xy );
	
	float size = position.w * SizeScale;
	
	vec3 vertex = ( -gl_Vertex.x * RightDirection + gl_Vertex.y * UpDirection ) * size;
	
	vertex += position.xyz;
	
	FragmentColor = gl_Color;	
   
	FragmentPosition = gl_Vertex.xy;
	
	gl_Position =  gl_ModelViewProjectionMatrix * vec4 ( vertex, 1.0 );
}
