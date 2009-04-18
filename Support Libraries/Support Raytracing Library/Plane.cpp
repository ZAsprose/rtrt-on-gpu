/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
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