#pragma once

#ifndef _GRID_

#define _GRID_

#include "Intersector.h"

#include "Scene.h"

namespace Raytracing
{
	class Grid
	{
		private:

			int PartitionsX;
		
			int PartitionsY;
			
			int PartitionsZ;

			Voxel **** Voxels;

			Volume * Box;

		public:
		
			Grid ( int = 16, int = 16, int = 16 );
			
			~Grid ( void );

			void BuildGrid ( vector < Triangle * > );
	};
}

#endif