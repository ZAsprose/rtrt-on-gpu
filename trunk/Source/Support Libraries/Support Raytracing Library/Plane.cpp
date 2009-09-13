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

#include "Plane.h"

namespace Raytracing
{
	//---------------------------------------- Constructor ----------------------------------------

	Plane :: Plane ( const Vector2D& radius,
		             Transform * transformation,
					 Material * properties,
					 const char * name,
					 bool visible ) : Primitive ( transformation, properties, name, visible )
	{
		Radius = radius;
	}

	//------------------------------------- Building Triangles ------------------------------------
			
	void Plane :: Tesselate ( void )
	{
		if ( !Triangles.empty ( ) )
		{
			for ( unsigned index = 0; index < Triangles.size ( ); index++ )
			{
				delete Triangles [index];
			}

			Triangles.clear ( );
		}

		//-------------------------------------------------------------------------------

		Vertex * vertices [4];

		vertices [0] = new Vertex (
			Transformation->ForwardPoint ( Vector3D ( -Radius.X, -Radius.Y ) ),
			Normalize ( Transformation->ForwardNormal ( Vector3D :: AxisZ ) ),
			Vector2D ( 0.0F, 0.0F ) );

		vertices [1] = new Vertex (
			Transformation->ForwardPoint ( Vector3D ( -Radius.X, Radius.Y ) ),
			Normalize ( Transformation->ForwardNormal ( Vector3D :: AxisZ ) ),
			Vector2D ( 0.0F, 1.0F ) );

		vertices [2] = new Vertex (
			Transformation->ForwardPoint ( Vector3D ( Radius.X, Radius.Y ) ),
			Normalize ( Transformation->ForwardNormal ( Vector3D :: AxisZ ) ),
			Vector2D ( 1.0F, 1.0F ) );

		vertices [3] = new Vertex (
			Transformation->ForwardPoint ( Vector3D ( Radius.X, -Radius.Y ) ),
			Normalize ( Transformation->ForwardNormal ( Vector3D :: AxisZ ) ),
			Vector2D ( 1.0F, 0.0F ) );

		//-------------------------------------------------------------------------------

		Triangles.push_back (
			new Triangle ( vertices [0], vertices [1], vertices [3], Properties ) );
		
		Triangles.push_back (
			new Triangle ( vertices [3], vertices [1], vertices [2], Properties ) );
	}
}