#pragma once

#ifndef _VOXEL_

#define _VOXEL_

#include "Triangle.h"

#include <vector>

namespace Raytracing
{
	class Voxel
	{
		public:

			//------------------------------- Position and Half Size ------------------------------
			
			Vector3D Position;
			
			Vector3D HalfSize;

			//------------------------------- Intersected Triangles -------------------------------
			
			vector < Triangle * > Triangles;

			//------------------------------------ Constructor ------------------------------------

			Voxel ( const Vector3D& position, const Vector3D& halfSize );
	};
}

#endif