/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

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

		//---------------------------------------------------------------------
		
		for ( unsigned index = 0; index < Primitives.size ( ); index++ )
		{
			if ( Primitives [index]->Visible )
			{
				Primitives [index]->Draw ( );
			}
		}

		//---------------------------------------------------------------------

		Box->Draw ( );
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

		Grid = new UniformGrid ( partitionsX, partitionsY, partitionsZ );
		//Grid = new ProximityGrid ( partitionsX, partitionsY, partitionsZ );

		Grid->BuildGrid ( Box, triangles );
	}

	//-------------------------------------- Apply Settings ---------------------------------------

	void Scene :: SetShaderData ( ShaderManager * manager )
	{
		manager->SetUniformVector ( "Grid.Minimum", Box->Minimum );
		
		manager->SetUniformVector ( "Grid.Maximum", Box->Maximum );

		Vector3D dimesions ( Grid->GetPartitionsX ( ),
			                 Grid->GetPartitionsY ( ),
							 Grid->GetPartitionsZ ( ) );

		manager->SetUniformVector ( "Grid.VoxelCount", dimesions );

		manager->SetUniformVector ( "Grid.VoxelSize",
			                        ( Box->Maximum - Box->Minimum ) / dimesions );

		//---------------------------------------------------------------------

		manager->SetUniformInteger ( "LightsCount" , Lights.size ( ) );
		
		for ( int index = 0; index < Lights.size ( ); index++)
		{
			Lights [index]->SetShaderData ( manager );
		}
	}
}