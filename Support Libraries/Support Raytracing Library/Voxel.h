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
			
			Vector3D Radius;

			//------------------------------- Intersected Triangles -------------------------------
			
			vector < Triangle * > Triangles;

			//------------------------------------ Constructor ------------------------------------

			Voxel ( const Vector3D&, const Vector3D& );
	};
}

#endif