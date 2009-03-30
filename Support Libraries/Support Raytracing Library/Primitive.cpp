#include "Primitive.h"

namespace Raytracing
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	Primitive :: Primitive ( Transform * transformation, const char * name )
	{
		Transformation = transformation;

		Name = name;
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
}