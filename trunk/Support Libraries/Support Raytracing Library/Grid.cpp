#include "Grid.h"

namespace Raytracing
{
	Grid :: Grid ( Volume * box, int partitionsX, int partitionsY, int partitionsZ )
	{
		Box = box;

		PartitionsX = partitionsX;
		
		PartitionsY = partitionsY;			
		
		PartitionsZ = partitionsZ;

		//-------------------------------------------------------------------------------
		
		Voxels = new Voxel *** [PartitionsX];

		for ( int x = 0; x < PartitionsX; x++ )
		{
			Voxels [x] = new Voxel ** [PartitionsY];

			for ( int y = 0; y < PartitionsY; y++ )
			{
				Voxels [x][y] = new Voxel * [PartitionsZ];

				for ( int z = 0; z < PartitionsZ; z++ )
				{
					Voxels [x][y][z] = NULL;
				}
			}
		}
	}

	Grid :: ~Grid ( void )
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
	
	void Grid :: BuildGrid ( vector < Triangle * > triangles )
	{
		//-----------------------------------------------------------------------------------------

		Vector3D size = ( Box->Maximum - Box->Minimum ) / Vector3D ( ( float ) PartitionsX,
		                                                             ( float ) PartitionsY,
																	 ( float ) PartitionsZ );

		Vector3D radius = size / 2.0F;
		
		//-----------------------------------------------------------------------------------------
		
		{
			Vector3D position = Box->Minimum + radius;
					
			for ( int i = 0; i < PartitionsX; i++ )
			{
				float x = position.X + size.X * i;
				
				for ( int j = 0; j < PartitionsY; j++ )
				{
					float y = position.Y + size.Y * j;
					
					for ( int k = 0; k < PartitionsZ; k++ )
					{
						float z = position.Z + size.Z * k;
						
						if ( Voxels [i][j][k] != NULL )
						{
							Voxels [i][j][k]->Triangles.clear ( );
						}
						else
						{
							Voxels [i][j][k] = new Voxel ( Vector3D ( x, y, z ),
								                           radius );
						}
					}	
				}
			}
		}

		//-----------------------------------------------------------------------------------------

		Vector3D maximum = Vector3D ( ( float ) ( PartitionsX - 1 ),
			                          ( float ) ( PartitionsY - 1 ),
									  ( float ) ( PartitionsZ - 1 ) );

		for ( unsigned index = 0; index < triangles.size ( ); index++ )
		{
			Vector3D start = ( triangles [index]->GetMinimum ( ) - Box->Minimum ) / size;

			start = Clamp ( start, Vector3D :: Zero, maximum );

			Vector3D final = ( triangles [index]->GetMaximum ( ) - Box->Minimum ) / size;

			final = Clamp ( final, Vector3D :: Zero, maximum );
				
			for ( int i = ( int ) start.X; i <= ( int ) final.X; i++ )
			{
				for ( int j = ( int ) start.Y; j <= ( int ) final.Y; j++ )
				{
					for ( int k = ( int ) start.Z; k <= ( int ) final.Z; k++ )
					{
						if ( Intersector :: TriangleVoxelOverlap ( triangles [index],
							                                       Voxels [i][j][k] ) )
						{
							Voxels [i][j][k]->Triangles.push_back ( triangles [index] );
						}
					}	
				}
			}
		}
	}

	Voxel * Grid :: GetVoxel ( int x, int y, int z )
	{
		return Voxels [x][y][z];
	}	
}