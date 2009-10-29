/*
   S U P P O R T   R A Y   T R A C I N G   L I B R A R Y

   Copyright (C) 2009  Denis Bogolepov ( bogdencmc@inbox.ru )

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see http://www.gnu.org/licenses.
 */

#include "Vertex.h"

#include <GL/glfw.h>

namespace Raytracing
{
	//---------------------------------------- Constructor ----------------------------------------

	Vertex :: Vertex ( const Vector3D& position,
		               const Vector3D& normal,
					   const Vector2D& texcoord )
	{
		Position = position;
		
		Normal = normal;

		TexCoord = texcoord;
	}

	//--------------  Drawing Vertex in OpenGL Mode ( Not Used for Scene Rendering ) --------------
	
	void Vertex :: Draw ( void )
	{
		glNormal3fv ( Normal );
		
		glVertex3fv ( Position );
	}
}