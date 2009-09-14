/*
   S U P P O R T   R A Y   T R A C I N G   L I B R A R Y

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

#include "UniformGrid.h"

namespace Raytracing
{
	//----------------------------------- Constructor and Destructor -----------------------------------

	UniformGrid :: UniformGrid ( int partitionsX, int partitionsY, int partitionsZ, Volume * box )
	{
		PartitionsX = partitionsX;
		
		PartitionsY = partitionsY;			
		
		PartitionsZ = partitionsZ;

		Box = box;

		//------------------------------------------------------------------------------------
		
		Voxels = new Voxel *** [PartitionsX];

		for ( int x = 0; x < PartitionsX; x++ )
		{
			Voxels [x] = new Voxel ** [PartitionsY];

			for ( int y = 0; y < PartitionsY; y++ )
			{
				Voxels [x][y] = new Voxel * [PartitionsZ];

				for ( int z = 0; z < PartitionsZ; z++ )
				{
					Voxels [x][y][z] = new Voxel ( );
				}
			}
		}
	}

	UniformGrid :: ~UniformGrid ( void )
	{
		for ( int x = 0; x < PartitionsX; x++ )
		{
			for ( int y = 0; y < PartitionsY; y++ )
			{
				for ( int z = 0; z < PartitionsZ; z++ )
				{
					delete Voxels [x][y][z];
				}

				delete Voxels [x][y];
			}
			
			delete Voxels [x];
		}
		
		delete Voxels;
	}

	//------------------------------------- Building Uniform Grid --------------------------------------
	
	void UniformGrid :: BuildGrid ( vector <Primitive *>& primitives )
	{
		Vector3D size = ( Box->Maximum - Box->Minimum ) / Vector3D ( ( float ) PartitionsX,
		                                                             ( float ) PartitionsY,
																	 ( float ) PartitionsZ );

		Vector3D radius = size / 2.0F;

		Vector3D position = Box->Minimum + radius;
		
		Vector3D maximum = Vector3D ( ( float ) ( PartitionsX - 1 ),
			                          ( float ) ( PartitionsY - 1 ),
									  ( float ) ( PartitionsZ - 1 ) );

		//------------------------------------------------------------------------------------

		vector <Primitive *> :: iterator primitive = primitives.begin ( ); 

		while ( primitive != primitives.end ( ) )
		{
			vector <Triangle *> :: iterator triangle = ( *primitive )->Triangles.begin ( );

			while ( triangle != ( *primitive )->Triangles.end ( ) )
			{
				Vector3D start = ( ( *triangle )->GetMinimum ( ) - Box->Minimum ) / size;

				start = Clamp ( start, Vector3D :: Zero, maximum );

				Vector3D final = ( ( *triangle )->GetMaximum ( ) - Box->Minimum ) / size;

				final = Clamp ( final, Vector3D :: Zero, maximum );
					
				for ( int i = ( int ) start.X; i <= ( int ) final.X; i++ )
				{
					float x = position.X + size.X * i;

					for ( int j = ( int ) start.Y; j <= ( int ) final.Y; j++ )
					{
						float y = position.Y + size.Y * j;

						for ( int k = ( int ) start.Z; k <= ( int ) final.Z; k++ )
						{
							float z = position.Z + size.Z * k;

							if ( Intersector :: TriangleVoxelOverlap (
								*triangle, Vector3D ( x, y, z ), radius ) )
							{
								Voxels [i][j][k]->Triangles.push_back ( *triangle );
							}
						}	
					}
				}

				++triangle;
			}

			++primitive;
		}
	}
}