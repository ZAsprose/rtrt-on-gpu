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

#ifndef _BOX_

#define _BOX_

#include "Primitive.h"

namespace Raytracing
{
	class Box : public Primitive
	{
		public:

			//-------------------------------- Half Size ( Radius ) -------------------------------

			Vector3D Radius;

			//------------------------------------ Constructor ------------------------------------
		
			Box ( const Vector3D& = Vector3D :: Unit,
				  Transform * = new Transform ( ),
				  Material * = new Material ( ),
				  const char * = "Box",
				  bool = true );

			//--------------------------------- Building Triangles --------------------------------
			
			void Tesselate ( void );
	};
}

#endif