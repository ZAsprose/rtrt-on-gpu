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

#ifndef _TRIANGLE_

#define _TRIANGLE_

#include "Vertex.h"

#include "Material.h"

namespace Raytracing
{
	class Triangle
	{
		private:

			//--------------------------------- Triangle Vertices ---------------------------------

			Vertex * VertexA;

			Vertex * VertexB;

			Vertex * VertexC;

			//------------------------------ Normal to Triangle Plane -----------------------------

			Vector3D Normal;

			//---------------- Minimum and Maximum Points of Bounding Box ( AABB ) ----------------

			Vector3D Minimum;

			Vector3D Maximum;

			//--------------------------------- Triangle Centroid ---------------------------------

			Vector3D Centroid;

			//--------------------------------- Triangle Centroid ---------------------------------

			void Update ( void );

		public:

			//-------------------------- Properties of Triangle Material --------------------------

			Material * Properties;

			//----------------------------- Constructor and Destructor ----------------------------
			
			Triangle ( Vertex * = NULL, Vertex * = NULL, Vertex * = NULL, Material * = NULL );

			~Triangle ( void );

			//-------------------------- Drawing Triangle in OpenGL Mode --------------------------
			
			void Draw ( void );

			//--------------------------------- Support Functions ---------------------------------
			
			bool IsEmpty ( void ) const;

			Vector3D GetNormal ( void ) const { return Normal; }

			Vector3D GetMinimum ( void ) const { return Minimum; }

			Vector3D GetMaximum ( void ) const { return Maximum; }

			Vector3D GetCentroid ( void ) const { return Centroid; }
	};
}

#endif
