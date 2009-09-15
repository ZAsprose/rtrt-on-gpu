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

#ifndef _PRIMITIVE_

#define _PRIMITIVE_

#include "Triangle.h"

#include "Transform.h"

#include <vector>

using namespace std;

namespace Raytracing
{
	class Primitive abstract
	{
		public:

			//------------------------------------- Triangles -------------------------------------

			vector <Triangle *> Triangles;

			//------------------------------ Vertices Transformation ------------------------------

			Transform * Transformation;

			//-------------------------------- Material Properties --------------------------------

			Material * Properties;

			//-------------------------------- Name and Visibility --------------------------------

			const char * Name;

			bool Visible;

			//----------------------------- Constructor and Destructor ----------------------------

			Primitive ( Transform * = new Transform ( ),
				        Material * = new Material ( ),
						const char * = "Primitive",
						bool = true );
			
			virtual ~Primitive ( void );

			//--------------------------------- Building Triangles --------------------------------
			
			virtual void Tesselate ( void ) = 0;

			//-------------------------- Drawing Primitive in OpenGL Mode -------------------------
			
			void Draw ( void );
	};
}

#endif