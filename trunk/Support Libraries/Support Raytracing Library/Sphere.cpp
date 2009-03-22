#include "Sphere.h"

#include <math.h>

namespace Raytracing
{
	//---------------------------------------- Constructor ----------------------------------------

	Sphere :: Sphere ( Math::Transform * transform, Raytracing::Material * material, float radius )
	{
		Material = material;

		Radius = radius;
	}

	//-------------------------------------- Build Triangles --------------------------------------
			
	void Sphere :: Tesselate ( void )
	{
		if ( !Triangles.empty ( ) )
		{
			for ( int index = 0; index < Triangles.size ( ); index++ )
			{
				delete Triangles [ index ];
			}

			Triangles.clear ( );
		}

		//---------------------------------------------------------------------

		Vertex *** vertices = new Vertex ** [ Slices + 1 ];

		for ( int index = 0; index <= Slices; index++ )
		{
			vertices [ index ] = new Vertex * [ Stacks + 1 ];
		}

		//---------------------------------------------------------------------

		float uStep = TWOPI / Slices;
			
		float vStep = ONEPI / Stacks;
		
		for ( int x = 0; x <= Slices; x++ )
		{
			float u = uStep * x;
			
			for ( int y = 0; y <= Stacks; y++ )
			{
				float v = vStep * y - ONEPI / 2.0F;
				
				Vector3D position ( Radius * sinf ( u ) * cosf ( v ),
					                Radius * cosf ( u ) * cosf ( v ),
									Radius * sinf ( v ) );
				
				Vector3D normal = Normalize ( position );
				
				vertices [ x ][ y ] = new Vertex ( position, normal );
			}
		}

		//---------------------------------------------------------------------

		for ( int x = 0; x < Slices; x++ )
		{
			for ( int y = 0; y < Stacks; y++ )
			{
				Triangles.push_back ( new Triangle ( vertices [ x ][ y ],
					                                 vertices [ x + 1 ][ y ],
													 vertices [ x ][ y + 1 ] ) );
				
				Triangles.push_back ( new Triangle ( vertices [ x ][ y + 1 ],
					                                 vertices [ x + 1 ][ y ],
													 vertices [ x + 1 ][ y + 1 ] ) );
			}
		}
	}
}
