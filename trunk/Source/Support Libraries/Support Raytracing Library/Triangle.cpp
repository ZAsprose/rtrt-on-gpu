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

#include "Triangle.h"

namespace Raytracing
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	Triangle :: Triangle ( Vertex * vertexA,
		                   Vertex * vertexB,
						   Vertex * vertexC,
						   Material * properties )
	{
		VertexA = vertexA;
		
		VertexB = vertexB;
		
		VertexC = vertexC;
		
		Properties = properties;
	}

	Triangle :: ~Triangle ( void )
	{
		delete VertexA;		

		delete VertexB;	

		delete VertexC;		
	}

	void Triangle :: Update ( void )
	{
		Normal = Cross ( VertexB->Position - VertexA->Position,
			VertexC->Position - VertexA->Position );

		Minimum = Min ( Min ( VertexA->Position, VertexB->Position ), VertexC->Position );

		Maximum = Max ( Max ( VertexA->Position, VertexB->Position ), VertexC->Position );

		Centroid = ( VertexA->Position + VertexB->Position + VertexC->Position ) / 3.0F;
	}

	//------------------------------ Drawing Triangle in OpenGL Mode ------------------------------
	
	void Triangle :: Draw ( void )
	{
		glNormal3fv ( VertexA->Normal );   glVertex3fv ( VertexA->Position );

		glNormal3fv ( VertexB->Normal );   glVertex3fv ( VertexB->Position );

		glNormal3fv ( VertexC->Normal );   glVertex3fv ( VertexC->Position );
	}

	//------------------------------------- Support Functions -------------------------------------
		
	bool Triangle :: IsEmpty ( void ) const
	{
		Vector3D cross = Cross ( VertexB->Position - VertexA->Position,
			                     VertexC->Position - VertexA->Position );
			
		return Square ( cross ) == 0.0F;
	}
}