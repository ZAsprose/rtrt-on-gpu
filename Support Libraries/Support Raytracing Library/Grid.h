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

			Voxel *** Voxels;

		public:
			
			int PartitionsX;
		
			int PartitionsY;
			
			int PartitionsZ;
		
			Grid ( void );
			
			~Grid ( void );
	};
}

#endif