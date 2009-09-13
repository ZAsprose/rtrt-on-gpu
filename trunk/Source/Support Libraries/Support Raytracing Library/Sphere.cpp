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

#include "Sphere.h"

namespace Raytracing
{
	//---------------------------------------- Constructor ----------------------------------------

	Sphere :: Sphere ( float radius,
		               int slices,
					   int stacks,
		               Transform * transformation,
					   Material * properties,
					   const char * name,
					   bool visible ) : Primitive ( transformation, properties, name, visible )
	{
		Radius = radius;

		Slices = slices;

		Stacks = stacks;
	}

	//------------------------------------- Building Triangles ------------------------------------
			
	void Sphere :: Tesselate ( void )
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

		Vertex *** vertices = new Vertex ** [Slices + 1];

		for ( int index = 0; index <= Slices; index++ )
		{
			vertices [index] = new Vertex * [Stacks + 1];
		}

		//------------------------------------------------------------------------------------

		float uStep = TWO_PI / Slices;
			
		float vStep = ONE_PI / Stacks;
		
		for ( int x = 0; x <= Slices; x++ )
		{
			float u = uStep * x;
			
			for ( int y = 0; y <= Stacks; y++ )
			{
				float v = vStep * y - ONE_PI / 2.0F;
				
				Vector3D position ( Radius * sinf ( u ) * cosf ( v ),
					                Radius * cosf ( u ) * cosf ( v ),
									Radius * sinf ( v ) );
				
				Vector3D normal = Normalize ( position );
				
				vertices [x][y] = new Vertex ( Transformation->ForwardPoint ( position ),
					Normalize ( Transformation->ForwardNormal ( normal ) ) );
			}
		}

		//------------------------------------------------------------------------------------

		Triangle * triangle = NULL;

		for ( int x = 0; x < Slices; x++ )
		{
			for ( int y = 0; y < Stacks; y++ )
			{
				triangle = new Triangle ( vertices [x][y],
					                      vertices [x + 1][y],
					                      vertices [x][y + 1],
										  Properties );

				if ( !triangle->IsEmpty ( ) )
					Triangles.push_back ( triangle );

				triangle = new Triangle ( vertices [x][y + 1],
					                      vertices [x + 1][y],
					                      vertices [x + 1][y + 1],
										  Properties );

				if ( !triangle->IsEmpty ( ) )
					Triangles.push_back ( triangle );
			}
		}
	}
}