/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#pragma once

#ifndef _GRID_

#define _GRID_

#include "Volume.h"

#include "Intersector.h"

namespace Raytracing
{
	class UniformGrid
	{
		protected:

			//------------------------------ Uniform Grid Dimensions ------------------------------

			int PartitionsX;
		
			int PartitionsY;
			
			int PartitionsZ;

			//----------------------------------- Voxels Array ------------------------------------

			Voxel **** Voxels;

		public:

			//---------------------------- Constructor and Destructor -----------------------------
		
			UniformGrid ( int = 16, int = 16, int = 16 );
			
			~UniformGrid ( void );

			//------------------------------- Building Uniform Grid -------------------------------

			virtual void BuildGrid ( Volume *, vector <Triangle *> );

			//-------------------------------- Getting Grid Params --------------------------------

			int GetPartitionsX ( void ) { return PartitionsX; }

			int GetPartitionsY ( void ) { return PartitionsY; }

			int GetPartitionsZ ( void ) { return PartitionsZ; }

			Voxel * GetVoxel ( int, int, int );	
	};
}

#endif