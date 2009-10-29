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

#pragma once

#ifndef _INTERSECTOR_

#define _INTERSECTOR_

#include "Triangle.h"

namespace Raytracing
{
	class Intersector
	{
		public:

			//--------------------------- Translated Triangle Vertices ----------------------------

			static Vector3D MovedA;
			
			static Vector3D MovedB;
			
			static Vector3D MovedC;
		
			//---------------------------------- Triangle Edges -----------------------------------
			
			static Vector3D EdgeAB;
					
			static Vector3D EdgeBC;
					
			static Vector3D EdgeCA;		
					
			//----------------------------- Voxel radius ( Half Size ) ----------------------------
			
			static Vector3D Radius;

			//--------------------------- Support Tests from SAT Theorem --------------------------

			static bool PlaneVoxelOverlap ( const Vector3D&, float );

			static bool SeparateAxisX ( void );

			static bool SeparateAxisY ( void );

			static bool SeparateAxisZ ( void );

		public:

			//-------------------------- Testing for Triangle-Box Overlap -------------------------

			static bool TriangleVoxelOverlap ( const Triangle *, Vector3D, Vector3D );
	};
}

#endif