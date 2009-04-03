#pragma once

#ifndef _GRID_

#define _GRID_

#include "Volume.h"

#include "Intersector.h"

namespace Raytracing
{
	class UniformGrid
	{
		private:

			//------------------------------ Uniform Grid Dimensions ------------------------------

			int PartitionsX;
		
			int PartitionsY;
			
			int PartitionsZ;

			//----------------------------------- Voxels Array ------------------------------------

			Voxel **** Voxels;

		public:

			//-------------------------------- Scene Bounding Box ---------------------------------

			Volume * Box;

			//---------------------------- Constructor and Destructor -----------------------------
		
			UniformGrid ( Volume *, int = 16, int = 16, int = 16 );
			
			~UniformGrid ( void );

			//------------------------------- Building Uniform Grid -------------------------------

			void BuildGrid ( vector < Triangle * > );

			//-------------------------------- Getting Grid Params --------------------------------

			int GetPartitionsX ( void ) { return PartitionsX; }

			int GetPartitionsY ( void ) { return PartitionsY; }

			int GetPartitionsZ ( void ) { return PartitionsZ; }

			Voxel * GetVoxel ( int, int, int );	
	};
}

#endif