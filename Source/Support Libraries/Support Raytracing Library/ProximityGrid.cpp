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

#include "ProximityGrid.h"

namespace Raytracing
{
	//------------------------------------ Constructor and Destructor -------------------------------------

	ProximityGrid :: ProximityGrid ( int partitionsX, int partitionsY, int partitionsZ, Volume * box ) 
		: UniformGrid ( partitionsX, partitionsY, partitionsZ, box )
	{
		WidthDistanceMap = new int ** [partitionsX];
		HeightDistanceMap = new int ** [partitionsX];
		DepthDistanceMap = new int ** [partitionsX];

		#ifdef DEBUG_GRID

		SimpleDistanceMap = new int ** [partitionsX];

		#endif

		for ( int x = 0; x < partitionsX; x++ )
		{
			WidthDistanceMap [x] = new int * [partitionsY];
			HeightDistanceMap [x] = new int * [partitionsY];
			DepthDistanceMap [x] = new int * [partitionsY];

			#ifdef DEBUG_GRID

			SimpleDistanceMap[x] = new int * [partitionsY];

			#endif
			
			for ( int y = 0; y < partitionsY; y++ )
			{
				WidthDistanceMap [x][y] = new int [partitionsZ];
				HeightDistanceMap [x][y] = new int [partitionsZ];
				DepthDistanceMap [x][y] = new int [partitionsZ];

				#ifdef DEBUG_GRID

				SimpleDistanceMap [x][y] = new int [partitionsZ];

				#endif
			}
		}
	}

	ProximityGrid :: ~ProximityGrid ( void )
	{
		for ( int x = 0; x < PartitionsX; x++ )
		{
			for ( int y = 0; y < PartitionsY; y++ )
			{
				delete [] WidthDistanceMap [x][y];
				delete [] HeightDistanceMap [x][y];
				delete [] DepthDistanceMap [x][y];

				#ifdef DEBUG_GRID

				delete [] SimpleDistanceMap [x][y];

				#endif
			}

			delete [] WidthDistanceMap [x];
			delete [] HeightDistanceMap [x];
			delete [] DepthDistanceMap [x];

			#ifdef DEBUG_GRID

			delete [] SimpleDistanceMap [x];

			#endif
		}
		
		delete [] WidthDistanceMap;
		delete [] HeightDistanceMap;
		delete [] DepthDistanceMap;

		#ifdef DEBUG_GRID

		delete [] SimpleDistanceMap;

		#endif
	}

	//-------------------------------------- Building Distance Maps ---------------------------------------

	void ProximityGrid :: BuildWidthDistanceMap ( void )
	{
		for ( int x = 0; x < PartitionsX; x++ )
		{
			for ( int y = 0; y < PartitionsY; y++ )
			{
				for ( int z = 0; z < PartitionsZ; z++ )
				{
					if ( Voxels [x][y][z]->Triangles.size ( ) > 0 )
					{
						WidthDistanceMap [x][y][z] = 0;
					}
					else
					{
						int distance = INT_MAX;
						
						for ( int i = 0; i < PartitionsX; i++ )
						{
							if ( Voxels [i][y][z]->Triangles.size ( ) > 0 )
							{
								distance = min ( distance, abs ( i - x ) );
							}
						}

						WidthDistanceMap [x][y][z] = distance;
					}
				}
			}
		}
	}
	
	void ProximityGrid :: BuildHeightDistanceMap ( void )
	{
		for ( int x = 0; x < PartitionsX; x++ )
		{
			for ( int y = 0; y < PartitionsY; y++ )
			{
				for ( int z = 0; z < PartitionsZ; z++ )
				{
					int distance = INT_MAX;

					for ( int j = 0; j < PartitionsY; j++ )
					{
						distance = min ( distance, max ( WidthDistanceMap [x][j][z], abs ( j - y ) ) );
					}

					HeightDistanceMap [x][y][z] = distance;
				}
			}
		}
	}

	void ProximityGrid :: BuildDepthDistanceMap ( void )
	{
		for ( int x = 0; x < PartitionsX; x++ )
		{
			for ( int y = 0; y < PartitionsY; y++ )
			{
				for ( int z = 0; z < PartitionsZ; z++ )
				{
					int distance = INT_MAX;

					for ( int k = 0; k < PartitionsZ; k++ )
					{
						distance = min ( distance, max ( HeightDistanceMap [x][y][k], abs ( k - z ) ) );
					}

					DepthDistanceMap [x][y][z] = max ( distance - 1, 0 );
				}
			}
		}
	}

	//-------------------------- Building Simple Distance Map ( Only for Debug ) --------------------------

	#ifdef DEBUG_GRID

	bool ProximityGrid :: CheckFreeZone ( int x, int y, int z, int proximity )
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

	void ProximityGrid :: BuildSimpleDistanceMap ( void )
	{
		for ( int x = 0; x < PartitionsX; x++ )
		{
			for ( int y = 0; y < PartitionsY; y++ )
			{
				for ( int z = 0; z < PartitionsZ; z++ )
				{
					if ( Voxels [x][y][z]->Triangles.size ( ) > 0 )
					{
						SimpleDistanceMap [x][y][z] = 0;
					}
					else
					{
						int proximity = 1;

						while ( proximity <= PartitionsX )
						{
							if ( !CheckFreeZone ( x, y, z, proximity ) )
								break;

							proximity++;
						}

						SimpleDistanceMap [x][y][z] = proximity - 1;
					}
				}
			}
		}
	}

	#endif

	//-------------------------------------- Building Proximity Grid --------------------------------------
	
	void ProximityGrid :: BuildGrid ( vector <Primitive *>& primitives )
	{
		UniformGrid :: BuildGrid ( primitives );
		
		BuildWidthDistanceMap ( );
		BuildHeightDistanceMap ( );
		BuildDepthDistanceMap ( );

		#ifdef DEBUG_GRID

		BuildSimpleDistanceMap ( );

		#endif
		
		for ( int x = 0; x < PartitionsX; x++ )
		{
			for ( int y = 0; y < PartitionsY; y++ )
			{
				for ( int z = 0; z < PartitionsZ; z++ )
				{
					Voxels [x][y][z]->Proximity = DepthDistanceMap [x][y][z];

					#ifdef DEBUG_GRID

					if ( SimpleDistanceMap [x][y][z] != DepthDistanceMap [x][y][z] )
					{
						cout << "X = " << x << "/t" << "Y = " << y << "/t" << "Z = " << z << "/t";

						cout << "SimpleDistanceMap != DepthDistanceMap" << endl;
					}

					#endif
				}
			}
		}
	}
}