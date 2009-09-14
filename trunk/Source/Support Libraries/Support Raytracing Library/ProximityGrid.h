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

#ifndef _PROXIMITY_GRID_

#define _PROXIMITY_GRID_

#include "UniformGrid.h"

#define DEBUG_GRID_

namespace Raytracing
{
	class ProximityGrid : public UniformGrid
	{
		private:
			
			//------------------- Distance Maps for Building of Proximity Grid --------------------

			int *** WidthDistanceMap;

			int *** HeightDistanceMap;

			int *** DepthDistanceMap;

			#ifdef DEBUG_GRID

			int *** SimpleDistanceMap;

			#endif

			//------------------------------ Building Distance Maps -------------------------------
			
			void BuildWidthDistanceMap ( void );

			void BuildHeightDistanceMap ( void );

			void BuildDepthDistanceMap ( void );

			//------------------ Building Simple Distance Map ( Only for Debug ) ------------------		

			#ifdef DEBUG_GRID

			bool CheckFreeZone ( int, int, int, int );

			void BuildSimpleDistanceMap ( void );

			#endif

		public:

			//---------------------------- Constructor and Destructor -----------------------------
			
			ProximityGrid ( int = 32, int = 32, int = 32, Volume * = NULL );
		
			~ProximityGrid ( );

			//------------------------------ Building Proximity Grid ------------------------------

			virtual void BuildGrid ( vector <Primitive *>& );
	};
}

#endif