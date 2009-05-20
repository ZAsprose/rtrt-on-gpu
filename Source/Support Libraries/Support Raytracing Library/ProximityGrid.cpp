#include "ProximityGrid.h"

#include <math.h>

namespace Raytracing
{
	ProximityGrid :: ProximityGrid ( int partitionsX, int partitionsY, int partitionsZ ) 
		: UniformGrid ( partitionsX, partitionsY, partitionsZ )
	{
		saitoFirstMap    = new int ** [partitionsX];
		saitoSecondMap   = new int ** [partitionsX];
		saitoDistanceMap = new int ** [partitionsX];

		for ( int i = 0; i < partitionsX; i++ )
		{
			saitoFirstMap[i]    = new int * [partitionsY];
			saitoSecondMap[i]   = new int * [partitionsY];
			saitoDistanceMap[i] = new int * [partitionsY];

			for ( int j = 0; j < partitionsY; j++ )
			{
				saitoFirstMap[i][j]    = new int [partitionsZ];
				saitoSecondMap[i][j]   = new int [partitionsZ];
				saitoDistanceMap[i][j] = new int [partitionsZ];

				for ( int k = 0; k < partitionsZ; k++ )
				{
					saitoFirstMap[i][j][k]    = 0;
					saitoSecondMap[i][j][k]   = 0;
					saitoDistanceMap[i][j][k] = 0;
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
			}
			delete [] saitoFirstMap[i];
			delete [] saitoSecondMap[i];
			delete [] saitoDistanceMap[i];
		}
		
		delete [] saitoFirstMap;
		delete [] saitoSecondMap;
		delete [] saitoDistanceMap;
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
					Voxels[i][j][k]->EmptyRadius = emptyRadius;
				}
			}
		}
	}
	
	void ProximityGrid :: BuildGrid (  Volume * box, vector < Triangle * > triangles )
	{
		UniformGrid :: BuildGrid ( box, triangles );
		
		CalculateFirstSaitoMap();
		CalculateSecondSaitoMap();
		CalculateDistanceSaitoMap();

		NormolizeAndApllyDistanceMap();
	}
}