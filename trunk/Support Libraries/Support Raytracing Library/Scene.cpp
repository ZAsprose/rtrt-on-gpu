#include "Scene.h"

namespace Raytracing
{
	//--------------------------------- Constructor and Destructor --------------------------------

	Scene :: Scene ( Camera * viewer )
	{
		Viewer = viewer;
	}

	Scene :: ~Scene ( void )
	{
		for ( int index = 0; index < Lights.size ( ); index++ )
		{
			delete Lights [ index ];
		}

		Lights.clear ( );
		
		for ( int index = 0; index < Primitives.size ( ); index++ )
		{
			delete Primitives [ index ];
		}

		Primitives.clear ( );

		delete Viewer;
	}

	//-------------------------------------------- Draw -------------------------------------------

	void Scene :: Draw ( void )
	{
		for ( int index = 0; index < Lights.size ( ); index++ )
		{
			Lights [ index ]->Setup ( );
			
			Lights [ index ]->Draw ( );
		}
		
		for ( int index = 0; index < Primitives.size ( ); index++ )
		{
			if ( Primitives [ index ]->Visible )
			{
				Primitives [ index ]->Draw ( );
			}
		}
	}
}