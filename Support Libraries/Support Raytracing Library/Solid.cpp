#include "Solid.h"

#include <GL/glfw.h>

namespace Raytracing
{
	//----------------------------- Constructor and Destructor ----------------------------

	Solid :: Solid ( void )
	{

	}
	
	Solid :: ~Solid ( void )
	{

	}

	//--------------------------------------- Draw ----------------------------------------
	
	void Solid :: Draw ( void )
	{
		Material->Setup ( );
		
		glBegin ( GL_TRIANGLES );

			for ( int index = 0; index < Triangles.size ( ); index++ )
			{
				Triangles[index]->Draw ( );
			}

		glEnd ( );
	}
}
