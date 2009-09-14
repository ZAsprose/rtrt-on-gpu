/*
   Support Raytracing Library  
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

#ifndef _GRID_

#define _GRID_

#include "Volume.h"

#include "Intersector.h"

#include "Primitive.h"

namespace Raytracing
{
	class UniformGrid
	{
		protected:

			//------------------------------ Uniform Grid Dimensions ------------------------------

			int PartitionsX;
		
			int PartitionsY;
			
			int PartitionsZ;

		public:

			//---------------------------- Bounding Volume of the Scene ---------------------------

			Volume * Box;

			//----------------------------------- Voxels Array ------------------------------------

			Voxel **** Voxels;

			//----------------------------- Constructor and Destructor ----------------------------
		
			UniformGrid ( int = 32, int = 32, int = 32, Volume * = NULL );
			
			~UniformGrid ( void );

			//------------------------------- Building Uniform Grid -------------------------------

			virtual void BuildGrid ( vector <Primitive *>& );

			//-------------------------------- Getting Grid Params --------------------------------

			int GetPartitionsX ( void ) const { return PartitionsX; }

			int GetPartitionsY ( void ) const { return PartitionsY; }

			int GetPartitionsZ ( void ) const { return PartitionsZ; }
	};
}

#endif