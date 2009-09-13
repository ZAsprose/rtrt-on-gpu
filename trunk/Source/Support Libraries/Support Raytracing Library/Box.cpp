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

#include "Box.h"

namespace Raytracing
{	
	//---------------------------------------- Constructor ----------------------------------------

	Box :: Box ( const Vector3D& radius,
		         Transform * transformation,
				 Material * properties,
				 const char * name,
				 bool visible ) : Primitive ( transformation, properties, name, visible )
	{
		Radius = radius;
	}

	//------------------------------------- Building Triangles ------------------------------------
			
	void Box :: Tesselate ( void )
	{
		if ( !Triangles.empty ( ) )
		{
			for ( unsigned index = 0; index < Triangles.size ( ); index++ )
			{
				delete Triangles [index];
			}

			Triangles.clear ( );
		}

		//------------------------------------------------------------------------------------

		Vertex * vertices [24];

		int index = 0;

		for ( float sign = -1.0F; sign < 2.0F; sign += 2.0F )
		{
			vertices [index++] = new Vertex ( Transformation->ForwardPoint (
				Vector3D ( sign * Radius.X, -Radius.Y, -Radius.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisX ) ) );

			vertices [index++] = new Vertex ( Transformation->ForwardPoint (
				Vector3D ( sign * Radius.X, -Radius.Y, Radius.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisX ) ) );

			vertices [index++] = new Vertex ( Transformation->ForwardPoint (
				Vector3D ( sign * Radius.X, Radius.Y, Radius.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisX ) ) );

			vertices [index++] = new Vertex ( Transformation->ForwardPoint (
				Vector3D ( sign * Radius.X, Radius.Y, -Radius.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisX ) ) );
		}

		for ( float sign = -1.0F; sign < 2.0F; sign += 2.0F )
		{
			vertices [index++] = new Vertex ( Transformation->ForwardPoint (
				Vector3D ( -Radius.X, sign * Radius.Y, -Radius.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisY ) ) );

			vertices [index++] = new Vertex ( Transformation->ForwardPoint (
				Vector3D ( -Radius.X, sign * Radius.Y, Radius.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisY ) ) );

			vertices [index++] = new Vertex ( Transformation->ForwardPoint (
				Vector3D ( Radius.X, sign * Radius.Y, Radius.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisY ) ) );

			vertices [index++] = new Vertex ( Transformation->ForwardPoint (
				Vector3D ( Radius.X, sign * Radius.Y, -Radius.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisY ) ) );
		}

		for ( float sign = -1.0F; sign < 2.0F; sign += 2.0F )
		{
			vertices [index++] = new Vertex ( Transformation->ForwardPoint (
				Vector3D ( -Radius.X, -Radius.Y, sign * Radius.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisZ ) ) );

			vertices [index++] = new Vertex ( Transformation->ForwardPoint (
				Vector3D ( -Radius.X, Radius.Y, sign * Radius.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisZ ) ) );

			vertices [index++] = new Vertex ( Transformation->ForwardPoint (
				Vector3D ( Radius.X, Radius.Y, sign * Radius.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisZ ) ) );

			vertices [index++] = new Vertex ( Transformation->ForwardPoint (
				Vector3D ( Radius.X, -Radius.Y, sign * Radius.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisZ ) ) );
		}

		//------------------------------------------------------------------------------------

		for ( int vertex = 0; vertex < 24; vertex  += 4 )
		{
			Triangles.push_back (
				new Triangle ( vertices [vertex], vertices [vertex + 1],
				               vertices [vertex + 2], Properties ) );
			
			Triangles.push_back (
				new Triangle ( vertices [vertex + 2], vertices [vertex + 3],
				               vertices [vertex], Properties ) );
		}
	}
}