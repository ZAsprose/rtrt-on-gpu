/*
   Base Render Library   
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

#ifndef _NOISE2D_

#define _NOISE2D_

#include <Vector2D.h>

#include "Texture2D.h"

using namespace Math;

namespace Render
{
	class Noise2D
	{
		private:

			//----------------------------------- Noise Density -----------------------------------
			
			int Size;

			//------------------------------- Random Values on Grid -------------------------------
			
			float ** Values;

			//------------------------------ Calculating Noise Value ------------------------------

			float Noise ( float, float );

			
		public:
			
			//---------------------------- Constructor and Destructor -----------------------------

			Noise2D ( int = 8 );

			~Noise2D ( );

			//---------------------------- Building Noise Texture Data ----------------------------

			TextureData2D * BuildData ( int, int );			
	};
}

#endif