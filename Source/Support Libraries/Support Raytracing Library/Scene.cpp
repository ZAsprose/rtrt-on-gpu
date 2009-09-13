/*
   Support Raytracing Library  
   Copyright (C) 2009  Denis Bogolepov ( bogdencmc@inbox.ru )

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see http://www.gnu.org/licenses.
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

		//-------------------------------------------------------------------------------
		
		for ( unsigned index = 0; index < Primitives.size ( ); index++ )
		{
			delete Primitives [index];
		}

		Primitives.clear ( );

		//-------------------------------------------------------------------------------

		for ( unsigned index = 0; index < TextureData.size ( ); index++ )
		{
			delete TextureData [index];
		}

		TextureData.clear ( );

		//-------------------------------------------------------------------------------

		delete Viewer;

		delete Box;

		delete Grid;
	}

	//-------------------------------- Drawing Scene in OpenGL Mode -------------------------------

	void Scene :: Draw ( void )
	{
		for ( unsigned index = 0; index < Lights.size ( ); index++ )
		{
			Lights [index]->Setup ( );
			
			Lights [index]->Draw ( );
		}

		//-------------------------------------------------------------------------------
		
		for ( unsigned index = 0; index < Primitives.size ( ); index++ )
		{
			if ( Primitives [index]->Visible )
			{
				Primitives [index]->Draw ( );
			}
		}

		//-------------------------------------------------------------------------------

		Box->Draw ( );
	}

	//------------------------------- Building Acceleration Structure -----------------------------
			
	void Scene :: BuildGrid ( int partitionsX, int partitionsY, int partitionsZ, bool proximity )
	{
		vector <Triangle *> triangles;

		for ( unsigned index = 0; index < Primitives.size ( ); index++ )
		{
			triangles.insert ( triangles.end ( ),
				               Primitives [index]->Triangles.begin ( ),
							   Primitives [index]->Triangles.end ( ) );
		}

		//-------------------------------------------------------------------------------

		if ( NULL != Grid )
		{
			delete Grid;
		}

		if ( proximity )
		{
			Grid = new ProximityGrid ( partitionsX, partitionsY, partitionsZ );
		}
		else
		{
			Grid = new UniformGrid ( partitionsX, partitionsY, partitionsZ );
		}

		Grid->BuildGrid ( Box, triangles );
	}

	//-------------------------------- Applying Settings to Shaders -------------------------------

	void Scene :: SetShaderData ( ShaderManager * manager )
	{
		manager->SetUniformVector ( "Grid.Minimum", Box->Minimum );

		manager->SetUniformVector ( "Grid.Maximum", Box->Maximum );

		//-------------------------------------------------------------------------------

		Vector3D dimesions ( Grid->GetPartitionsX ( ),
			                 Grid->GetPartitionsY ( ),
							 Grid->GetPartitionsZ ( ) );

		manager->SetUniformVector ( "Grid.VoxelCount", dimesions );

		manager->SetUniformVector ( "Grid.VoxelSize",
			( Box->Maximum - Box->Minimum ) / dimesions );

		//-------------------------------------------------------------------------------

		manager->SetUniformInteger ( "LightsCount" , Lights.size ( ) );

		for ( int index = 0; index < Lights.size ( ); index++)
		{
			Lights [index]->SetShaderData ( manager );
		}

		//-------------------------------------------------------------------------------
		
		Viewer->SetShaderData ( manager );
	}
}