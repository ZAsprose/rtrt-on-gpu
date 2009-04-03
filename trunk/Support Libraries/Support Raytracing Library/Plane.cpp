#include "Plane.h"

namespace Raytracing
{
	//---------------------------------------- Constructor ----------------------------------------

	Plane :: Plane ( const Vector2D& halfSize,
		             Transform * transformation,
					 Material * properties,
					 const char * name ) : Primitive ( transformation, properties, name )
	{
		HalfSize = halfSize;
	}

	//-------------------------------------- Build Triangles --------------------------------------
			
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

		Vertex * vertices [2][2];

		vertices [0][0] = new Vertex (
			Transformation->ForwardPoint ( Vector3D ( -HalfSize.X, -HalfSize.Y ) ),
			Normalize ( Transformation->ForwardNormal ( Vector3D :: AxisZ ) ) );

		vertices [0][1] = new Vertex (
			Transformation->ForwardPoint ( Vector3D ( -HalfSize.X, HalfSize.Y ) ),
			Normalize ( Transformation->ForwardNormal ( Vector3D :: AxisZ ) ) );

		vertices [1][1] = new Vertex (
			Transformation->ForwardPoint ( Vector3D ( HalfSize.X, HalfSize.Y ) ),
			Normalize ( Transformation->ForwardNormal ( Vector3D :: AxisZ ) ) );

		vertices [1][0] = new Vertex (
			Transformation->ForwardPoint ( Vector3D ( HalfSize.X, -HalfSize.Y ) ),
			Normalize ( Transformation->ForwardNormal ( Vector3D :: AxisZ ) ) );

		//-------------------------------------------------------------------------------

		Triangles.push_back ( new Triangle ( vertices [0][0],
			                                 vertices [0][1],
											 vertices [1][1],
											 Properties ) );
		
		Triangles.push_back ( new Triangle ( vertices [1][1],
			                                 vertices [1][0],
											 vertices [0][0],
											 Properties ) );
	}
}