#include "Scene.h"

namespace Raytracing
{
	//--------------------------------- Constructor and Destructor --------------------------------

	Scene :: Scene ( Camera * viewer, Volume * box )
	{
		Viewer = viewer;

		Box = box;

		Grid = NULL;
	}

	Scene :: ~Scene ( void )
	{
		for ( unsigned index = 0; index < Lights.size ( ); index++ )
		{
			delete Lights [index];
		}

		Lights.clear ( );
		
		for ( unsigned index = 0; index < Primitives.size ( ); index++ )
		{
			delete Primitives [index];
		}

		Primitives.clear ( );

		delete Viewer;
	}

	//-------------------------------------------- Draw -------------------------------------------

	void Scene :: Draw ( void )
	{
		for ( unsigned index = 0; index < Lights.size ( ); index++ )
		{
			Lights [index]->Setup ( );
			
			Lights [index]->Draw ( );
		}
		
		for ( unsigned index = 0; index < Primitives.size ( ); index++ )
		{
			if ( Primitives [index]->Visible )
			{
				Primitives [index]->Draw ( );
			}
		}
	}

	//------------------------------------- Build Uniform Grid ------------------------------------
			
	void Scene :: BuildGrid ( int partitionsX, int partitionsY, int partitionsZ )
	{
		vector < Triangle * > triangles;

		for ( unsigned index = 0; index < Primitives.size ( ); index++ )
		{
			triangles.insert ( triangles.end ( ),
				               Primitives [index]->Triangles.begin ( ),
							   Primitives [index]->Triangles.end ( ) );
		}

		//---------------------------------------------------------------------

		if ( NULL != Grid )
		{
			delete Grid;
		}

		Grid = new UniformGrid ( Box, partitionsX, partitionsY, partitionsZ );

		Grid->BuildGrid ( triangles );
	}
}