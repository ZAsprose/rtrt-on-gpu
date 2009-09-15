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

#ifndef _VOLUME_

#define _VOLUME_

#include <Vector3D.h>

#include <GLee.h>

using namespace Math;

namespace Raytracing
{
	class Volume
	{
		public:

			//----------------------- Minimum and Maximum Vertices of Volume ----------------------
			
			Vector3D Minimum;
		
			Vector3D Maximum;

			//----------------------------- Frame Color and Visibility ----------------------------
			
			Vector3D Color;
			
			bool Visible;

			//------------------------------------ Constructor ------------------------------------
			
			Volume ( const Vector3D& = Vector3D ( -5.0F, -5.0F, -5.0F ),
				     const Vector3D& = Vector3D ( 5.0F, 5.0F, 5.0F ),
					 const Vector3D& = Vector3D ( 0.6F, 0.6F, 0.6F ),
					 bool = true );

			//------------------------ Drawing Volume Frame in OpenGL Mode ------------------------
			
			void Draw ( void );
	};
}

#endif