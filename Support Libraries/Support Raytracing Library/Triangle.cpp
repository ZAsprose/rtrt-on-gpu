#include "Triangle.h"

#include <GL/glfw.h>

namespace Raytracing
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	Triangle :: Triangle ( Vertex * vertexA,
		                   Vertex * vertexB,
						   Vertex * vertexC,
						   Raytracing::Material * material )
	{
		VertexA = vertexA;
		
		VertexB = vertexB;
		
		VertexC = vertexC;
		
		Material = material;
	}

	Triangle :: ~Triangle ( void )
	{
		delete VertexA;		

		delete VertexB;	

		delete VertexC;		
	}

	//------------------------------------------- Draw --------------------------------------------
	
	void Triangle :: Draw ( void )
	{
		glNormal3fv ( VertexA->Normal );   glVertex3fv ( VertexA->Position );

		glNormal3fv ( VertexB->Normal );   glVertex3fv ( VertexB->Position );

		glNormal3fv ( VertexC->Normal );   glVertex3fv ( VertexC->Position );
	}

	//------------------------------------- Support Functions -------------------------------------
		
	bool Triangle :: IsEmpty ( void )
	{
		Vector3D cross = Cross ( VertexB->Position - VertexA->Position,
			                     VertexC->Position - VertexA->Position );
			
		return Square ( cross ) == 0.0F;
	}

	Vector3D Triangle :: GetMinimum ( void )
	{
		return Min ( Min ( VertexA->Position, VertexB->Position ), VertexC->Position );
	}

	Vector3D Triangle :: GetMaximum ( void )
	{
		return Max ( Max ( VertexA->Position, VertexB->Position ), VertexC->Position );
	}
}