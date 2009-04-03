#include "Primitive.h"

#include <GL/glfw.h>

namespace Raytracing
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	Primitive :: Primitive ( Transform * transformation,
		                     Material * properties,
							 const char * name )
	{
		Transformation = transformation;

		Name = name;

		Properties = properties;
	}

	Primitive :: ~Primitive ( void )
	{
		delete Transformation;
		
		for ( unsigned index = 0; index < Triangles.size ( ); index++ )
		{
			delete Triangles [ index ];
		}

		Triangles.clear ( );
	}

	//-------------------------------------------- Draw -------------------------------------------
	
	void Primitive :: Draw ( void )
	{
		Properties->Setup ( );
		
		glBegin ( GL_TRIANGLES );

			for ( unsigned index = 0; index < Triangles.size ( ); index++ )
			{
				Triangles [index]->Draw ( );
			}

		glEnd ( );
	}
}