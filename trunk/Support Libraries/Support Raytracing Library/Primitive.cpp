/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "Primitive.h"

#include <GL/glfw.h>

namespace Raytracing
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	Primitive :: Primitive ( Transform * transformation,
		                     Material * properties,
							 const char * name,
							 bool visible )
	{
		Transformation = transformation;

		Properties = properties;

		Name = name;

		Visible = visible;
	}

	Primitive :: ~Primitive ( void )
	{
		for ( unsigned index = 0; index < Triangles.size ( ); index++ )
		{
			delete Triangles [ index ];
		}

		Triangles.clear ( );

		//---------------------------------------------------------------------

		delete Transformation;
		
		delete Properties;
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