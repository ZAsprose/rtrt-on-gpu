varying vec2 TextureCoord;

void main()
{   
	TextureCoord = gl_Vertex.xy;
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
