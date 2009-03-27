#include "Grid.h"

namespace Raytracing
{
	Grid :: Grid ( int partitionsX, int partitionsY, int partitionsZ )
	{
		PartitionsX = partitionsX;
		
		PartitionsY = partitionsY;			
		
		PartitionsZ = partitionsZ;
		
		Voxels = new Voxel *** [PartitionsX];

		for ( int x = 0; x < PartitionsX; x++ )
		{
			Voxels [x] = new Voxel ** [PartitionsY];

			for ( int y = 0; y < PartitionsY; y++ )
			{
				Voxels [x][y] = new Voxel * [PartitionsZ];
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
		//-------------------------------------------------------------------------------
			
		Vector3D size = ( Box->Maximum - Box->Minimum ) / Vector3D ( PartitionsX,
			                                                         PartitionsY,
																	 PartitionsZ );
			
		Vector3D radius = size / 2.0F;
		
		//-------------------------------------------------------------------------------
		
		{
			Vector3D start = Box->Minimum + radius;
					
			for ( int i = 0; i < PartitionsX; i++ )
			{
				float x = start.X + size.X * i;
				
				for ( int j = 0; j < PartitionsY; j++ )
				{
					float y = start.Y + size.Y * j;
					
					for ( int k = 0; k < PartitionsZ; k++ )
					{
						float z = start.Z + size.Z * k;
						
						Voxels [i][j][k] = new Voxel ( Vector3D ( x, y, z ), radius );
					}	
				}
			}
		}

		//-------------------------------------------------------------------------------
			
		//foreach (Triangle triangle in triangles)
		//{
		//	Vector3 first = (triangle.Minimum - Volume.Minimum) / fullSize; 	

		//	Vector3 last = (triangle.Maximum - Volume.Minimum) / fullSize; 				
		//		
		//	for (int i = (int) first.X; i <= (int) last.X; i++)
		//	{
		//		for (int j = (int) first.Y; j <= (int) last.Y; j++)
		//		{
		//			for (int k = (int) first.Z; k <= (int) last.Z; k++)
		//			{
		//				if (Intersector.TriangleVoxelOverlap(triangle, voxels[i, j, k]))
		//				{
		//					voxels[i, j, k].Triangles.Add(triangle);
		//				}
		//			}	
		//		}
		//	}
		//}
	}
}