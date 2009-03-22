#include "Primitive.h"

namespace Raytracing
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	Primitive :: Primitive ( void )
	{

	}

	Primitive :: ~Primitive ( void )
	{
		for ( int index = 0; index < Triangles.size ( ); index++ )
		{
			delete Triangles[index];
		}

		Triangles.clear ( );
	}
}