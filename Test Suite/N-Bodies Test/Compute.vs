varying vec2 Particle;

void main()
{
	Particle = gl_Vertex;
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
