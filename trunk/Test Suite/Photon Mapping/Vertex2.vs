varying vec2 ScreenCoords;

void main ( void )
{
   ScreenCoords = gl_Vertex.xy;

   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}