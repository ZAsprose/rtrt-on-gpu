#pragma once

#ifndef _INTERSECTOR_

#define _INTERSECTOR_

#include "Voxel.h"

namespace Raytracing
{
	class Intersector
	{
		public:

			//--------------------------- Translated triangle vertices ----------------------------

			static Vector3D MovedA;
			
			static Vector3D MovedB;
			
			static Vector3D MovedC;
		
			//---------------------------------- Triangle edges -----------------------------------
			
			static Vector3D EdgeAB;
					
			static Vector3D EdgeBC;
					
			static Vector3D EdgeCA;		
					
			//--------------------------------- Voxel parameters ----------------------------------
			
			static Vector3D Radius;

			static bool PlaneVoxelOverlap ( const Vector3D&, float );

			static bool SeparateAxisX ( void );

			static bool SeparateAxisY ( void );

			static bool SeparateAxisZ ( void );

		public:

			static bool TriangleVoxelOverlap ( const Triangle *, const Voxel * );
	};
}

#endif