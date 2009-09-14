#include "ProximityGrid.h"

#include <math.h>

namespace Raytracing
{
	ProximityGrid :: ProximityGrid ( int partitionsX, int partitionsY, int partitionsZ, Volume * box ) 
		: UniformGrid ( partitionsX, partitionsY, partitionsZ, box )
	{
		saitoFirstMap    = new int ** [partitionsX];
		saitoSecondMap   = new int ** [partitionsX];
		saitoDistanceMap = new int ** [partitionsX];

		simpleDistanceMap = new int ** [partitionsX];

		for ( int i = 0; i < partitionsX; i++ )
		{
			saitoFirstMap[i]    = new int * [partitionsY];
			saitoSecondMap[i]   = new int * [partitionsY];
			saitoDistanceMap[i] = new int * [partitionsY];

			simpleDistanceMap[i] = new int * [partitionsY];

			for ( int j = 0; j < partitionsY; j++ )
			{
				saitoFirstMap[i][j]    = new int [partitionsZ];
				saitoSecondMap[i][j]   = new int [partitionsZ];
				saitoDistanceMap[i][j] = new int [partitionsZ];

				simpleDistanceMap[i][j] = new int [partitionsZ];

				for ( int k = 0; k < partitionsZ; k++ )
				{
					saitoFirstMap[i][j][k]    = 0;
					saitoSecondMap[i][j][k]   = 0;
					saitoDistanceMap[i][j][k] = 0;

					simpleDistanceMap[i][j][k] = 0;
				}
			}
		}
	}

	ProximityGrid :: ~ProximityGrid ()
	{
		for ( int i = 0; i < PartitionsX; i++ )
		{
			for ( int j = 0; j < PartitionsY; j++ )
			{
				delete [] saitoFirstMap[i][j];
				delete [] saitoSecondMap[i][j];
				delete [] saitoDistanceMap[i][j];

				delete [] simpleDistanceMap[i][j];
			}
			delete [] saitoFirstMap[i];
			delete [] saitoSecondMap[i];
			delete [] saitoDistanceMap[i];

			delete [] simpleDistanceMap[i];
		}
		
		delete [] saitoFirstMap;
		delete [] saitoSecondMap;
		delete [] saitoDistanceMap;

		delete [] simpleDistanceMap;
	}

	void ProximityGrid :: CalculateFirstSaitoMap()
	{
		for ( int i = 0; i < PartitionsX; i++ )
		{
			for ( int j = 0; j < PartitionsY; j++ )
			{
				for ( int k = 0; k < PartitionsZ; k++ )
				{
					// if voxel is not empty then ...
					if ( Voxels[i][j][k]->Triangles.size() )
					{
						saitoFirstMap[i][j][k] = 0;
					}
					// else calculating independent Z min distance to the object
					else
					{
						int min = PartitionsZ * PartitionsZ;
						
						for ( int l = 0; l < PartitionsZ; l++ )
						{
							if ( l == k )
							{
								continue;
							}
							if ( Voxels[i][j][l]->Triangles.size() )
							{
								int current_distance = (l - k) * (l - k);
								if ( current_distance < min )
								{
									min = current_distance;
								}
							}
						}
						saitoFirstMap[i][j][k] = min;
					}
				}
			}
		}
	}

	
	void ProximityGrid :: CalculateSecondSaitoMap()
	{
		for ( int i = 0; i < PartitionsX; i++ )
		{
			for ( int j = 0; j < PartitionsY; j++ )
			{
				for ( int k = 0; k < PartitionsZ; k++ )
				{
					// if voxel is not empty then ...
					if ( 0 == saitoFirstMap[i][j][k] )
					{
						saitoSecondMap[i][j][k] = 0;
					}
					// else calculating Y min distance to the object
					else
					{
						int min = PartitionsY * PartitionsY;
						
						for ( int l = 0; l < PartitionsY; l++ )
						{
							int current_distance = saitoFirstMap[i][l][k] + (j - l) * (j - l);
							if ( current_distance < min )
							{
								min = current_distance;
							}
						}
						saitoSecondMap[i][j][k] = min;
					}
				}
			}
		}
	}

	void ProximityGrid :: CalculateDistanceSaitoMap()
	{
		for ( int i = 0; i < PartitionsX; i++ )
		{
			for ( int j = 0; j < PartitionsY; j++ )
			{
				for ( int k = 0; k < PartitionsZ; k++ )
				{
					// if voxel is not empty then ...
					if ( 0 == saitoSecondMap[i][j][k] )
					{
						saitoDistanceMap[i][j][k] = 0;
					}
					// else calculating X min distance to the object
					else
					{
						int min = PartitionsX * PartitionsX;
						
						for ( int l = 0; l < PartitionsX; l++ )
						{
							int current_distance = saitoSecondMap[l][j][k] + (i - l) * (i - l);
							if ( current_distance < min )
							{
								min = current_distance;
							}
						}
						saitoDistanceMap[i][j][k] = min;
					}
				}
			}
		}
	}

	void ProximityGrid :: NormolizeAndApllyDistanceMap()
	{
		for ( int i = 0; i < PartitionsX; i++ )
		{
			for ( int j = 0; j < PartitionsY; j++ )
			{
				for ( int k = 0; k < PartitionsZ; k++ )
				{
					float emptyRadius = floor( sqrtf( (float)saitoDistanceMap[i][j][k] ) );
					emptyRadius -= 1.0f; // because of traversal algorithm
					Voxels[i][j][k]->Proximity = emptyRadius;
				}
			}
		}
	}

	bool ProximityGrid :: CheckVoxelProximity ( int x, int y, int z, int proximity )
	{
		int startX = x - proximity < 0 ? 0 : x - proximity;

		int startY = y - proximity < 0 ? 0 : y - proximity;

		int startZ = z - proximity < 0 ? 0 : z - proximity;


		int finalX = x + proximity >= PartitionsX ? PartitionsX - 1 : x + proximity;

		int finalY = y + proximity >= PartitionsY ? PartitionsY - 1 : y + proximity;

		int finalZ = z + proximity >= PartitionsZ ? PartitionsZ - 1 : z + proximity;


		for ( int i = startX; i <= finalX; i++ )
		{
			for ( int j = startY; j <= finalY; j++ )
			{
				if ( Voxels [i][j][startZ]->Triangles.size ( ) > 0 )
					return false;

				if ( Voxels [i][j][finalZ]->Triangles.size ( ) > 0 )
					return false;
			}
		}
		
		for ( int i = startX; i <= finalX; i++ )
		{
			for ( int k = startZ; k <= finalZ; k++ )
			{
				if ( Voxels [i][startY][k]->Triangles.size ( ) > 0 )
					return false;

				if ( Voxels [i][finalY][k]->Triangles.size ( ) > 0 )
					return false;
			}
		}

		for ( int j = startY; j <= finalY; j++ )
		{
			for ( int k = startZ; k <= finalZ; k++ )
			{
				if ( Voxels [startX][j][k]->Triangles.size ( ) > 0 )
					return false;

				if ( Voxels [finalX][j][k]->Triangles.size ( ) > 0 )
					return false;
			}
		}

		return true;
	}

	void ProximityGrid :: BuildDistanceMap ( void )
	{
		for ( int x = 0; x < PartitionsX; x++ )
		{
			for ( int y = 0; y < PartitionsY; y++ )
			{
				for ( int z = 0; z < PartitionsZ; z++ )
				{
					if ( Voxels [x][y][z]->Triangles.size ( ) > 0 )
					{
						simpleDistanceMap [x][y][z] = 0;
					}
					else
					{
						int proximity = 1;

						while ( proximity <= PartitionsX )
						{
							if ( !CheckVoxelProximity ( x, y, z, proximity ) )
								break;

							proximity++;
						}

						simpleDistanceMap [x][y][z] = proximity - 1;
					}

					//cout << "[ " << x << " ] [ " << y << " ] [ " << z << " ] = " << simpleDistanceMap [x][y][z] << endl;
				}
			}

			cout << "X = " << x << endl;
		}
	}
	
	void ProximityGrid :: BuildGrid ( vector <Primitive *>& primitives )
	{
		UniformGrid :: BuildGrid ( primitives );
		
		//CalculateFirstSaitoMap ( );
		//CalculateSecondSaitoMap ( );
		//CalculateDistanceSaitoMap ( );

		//NormolizeAndApllyDistanceMap ( );

		BuildDistanceMap ( );

		for ( int x = 0; x < PartitionsX; x++ )
		{
			for ( int y = 0; y < PartitionsY; y++ )
			{
				for ( int z = 0; z < PartitionsZ; z++ )
				{
					//if ( Voxels[x][y][z]->EmptyRadius != simpleDistanceMap [x][y][z] )
					//{
						Voxels[x][y][z]->Proximity = simpleDistanceMap [x][y][z];
						//cout << "[ " << x << "	" << y << "	" << z << " ]" << "	" << Voxels[x][y][z]->EmptyRadius << "	" << simpleDistanceMap [x][y][z] << endl;
					//}
				}
			}
		}
	}
}