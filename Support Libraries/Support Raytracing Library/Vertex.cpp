/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "Vertex.h"

#include <GL/glfw.h>

namespace Raytracing
{
	//---------------------------------------- Constructor ----------------------------------------

	Vertex :: Vertex ( const Vector3D& position, const Vector3D& normal, const Vector2D& texcoord )
	{
		Position = position;
		
		Normal = normal;

		TexCoord = texcoord;
	}

	//------------------------------------------- Draw --------------------------------------------
	
	void Vertex :: Draw ( void )
	{
		glNormal3fv ( Normal );
		
		glVertex3fv ( Position );
	}
}