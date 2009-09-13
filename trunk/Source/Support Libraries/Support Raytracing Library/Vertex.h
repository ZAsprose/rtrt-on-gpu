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

#ifndef _VERTEX_

#define _VERTEX_

#include <Vector3D.h>

using namespace Math;

namespace Raytracing
{
	class Vertex
	{
		public:

			//--------------------------------- Vertex Attributes ---------------------------------
			
			Vector3D Position;
			
			Vector3D Normal;

			Vector2D TexCoord;

			//------------------------------------ Constructor ------------------------------------
			
			Vertex ( const Vector3D& = Vector3D :: Zero,
				     const Vector3D& = Vector3D :: AxisZ,
					 const Vector2D& = Vector2D :: Zero );

			//----------  Drawing Vertex in OpenGL Mode ( Not Used for Scene Rendering ) ----------

			void Draw ( void );
	};
}

#endif