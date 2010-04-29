/*************************************************************************************************/
/*                        Vertex Shader for Sphere Texturing and Lighting                        */	
/*************************************************************************************************/

varying float Intensity;

void main ( void )
{
    vec3 ecPosition = vec3 ( gl_ModelViewMatrix * gl_Vertex );
    
    vec3 ecNormal = normalize ( gl_NormalMatrix * gl_Normal );
    
    vec3 ecLight = normalize ( -ecPosition );
    
    Intensity = max ( 0.2, dot ( ecLight, ecNormal ) );

	gl_TexCoord [0] = gl_MultiTexCoord0;
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}