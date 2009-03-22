#include "Vertex.h"

#include <GL/glfw.h>

namespace Raytracing
{
	//---------------------------------------- Constructor ----------------------------------------

	Vertex :: Vertex ( const Vector3D& position, const Vector3D& normal )
	{
		Position = position;
		
		Normal = normal;
	}

	//------------------------------------------- Draw --------------------------------------------
	
	void Vertex :: Draw ( void )
	{
		glNormal3fv ( Normal );
		
		glVertex3fv ( Position );
	}
}
