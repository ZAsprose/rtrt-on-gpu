#include "Solid.h"

#include <GL/glfw.h>

namespace Raytracing
{
	//----------------------------- Constructor and Destructor ----------------------------

	Solid :: Solid ( Transform * transformation,
		             Material * properties,
					 const char * name ) : Primitive ( transformation, name )
	{
		Properties = properties;
	}

	//--------------------------------------- Draw ----------------------------------------
	
	void Solid :: Draw ( void )
	{
		Properties->Setup ( );
		
		glBegin ( GL_TRIANGLES );

			for ( unsigned index = 0; index < Triangles.size ( ); index++ )
			{
				Triangles [ index ]->Draw ( );
			}

		glEnd ( );
	}
}
