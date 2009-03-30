#include "Sphere.h"

#include <math.h>

namespace Raytracing
{
	//---------------------------------------- Constructor ----------------------------------------

	Sphere :: Sphere ( float radius,
		               int slices,
					   int stacks,
		               Transform * transformation,
					   Material * properties,
					   const char * name ) : Solid ( transformation, properties, name )
	{
		Radius = radius;

		Slices = slices;

		Stacks = stacks;
	}

	//-------------------------------------- Build Triangles --------------------------------------
			
	void Sphere :: Tesselate ( void )
	{
		if ( !Triangles.empty ( ) )
		{
			for ( unsigned index = 0; index < Triangles.size ( ); index++ )
			{
				delete Triangles [ index ];
			}

			Triangles.clear ( );
		}

		//-------------------------------------------------------------------------------

		Vertex *** vertices = new Vertex ** [ Slices + 1 ];

		for ( int index = 0; index <= Slices; index++ )
		{
			vertices [ index ] = new Vertex * [ Stacks + 1 ];
		}

		//-------------------------------------------------------------------------------

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
				
				vertices [ x ][ y ] =
					new Vertex ( Transformation->ForwardPoint ( position ),
					             Normalize ( Transformation->ForwardNormal ( normal ) ) );
			}
		}

		//-------------------------------------------------------------------------------

		for ( int x = 0; x < Slices; x++ )
		{
			for ( int y = 0; y < Stacks; y++ )
			{
				Triangle * triangle =
					new Triangle ( vertices [ x ][ y ],  vertices [ x + 1 ][ y ], vertices [ x ][ y + 1 ] );

				if ( !triangle->IsEmpty ( ) )
					Triangles.push_back ( triangle );

				triangle =
					new Triangle ( vertices [ x ][ y + 1 ], vertices [ x + 1 ][ y ], vertices [ x + 1 ][ y + 1 ] );

				if ( !triangle->IsEmpty ( ) )
					Triangles.push_back ( triangle );
			}
		}
	}
}