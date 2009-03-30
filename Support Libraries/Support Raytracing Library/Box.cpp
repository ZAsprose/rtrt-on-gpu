#include "Box.h"

namespace Raytracing
{	
	//---------------------------------------- Constructor ----------------------------------------

	Box :: Box ( const Vector3D& halfSize,
		         Transform * transformation,
				 Material * properties,
				 const char * name ) : Solid ( transformation, properties, name )
	{
		HalfSize = halfSize;
	}

	//-------------------------------------- Build Triangles --------------------------------------
			
	void Box :: Tesselate ( void )
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

		Vertex * vertices [ 24 ];

		int index = 0;

		for ( float sign = -1.0F; sign < 2.0F; sign += 2.0F )
		{
			vertices [ index++ ] = new Vertex (
				Transformation->ForwardPoint ( Vector3D ( sign * HalfSize.X, -HalfSize.Y, -HalfSize.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisX ) ) );

			vertices [ index++ ] = new Vertex (
				Transformation->ForwardPoint ( Vector3D ( sign * HalfSize.X, -HalfSize.Y, HalfSize.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisX ) ) );

			vertices [ index++ ] = new Vertex (
				Transformation->ForwardPoint ( Vector3D ( sign * HalfSize.X, HalfSize.Y, HalfSize.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisX ) ) );

			vertices [ index++ ] = new Vertex (
				Transformation->ForwardPoint ( Vector3D ( sign * HalfSize.X, HalfSize.Y, -HalfSize.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisX ) ) );
		}

		for ( float sign = -1.0F; sign < 2.0F; sign += 2.0F )
		{
			vertices [ index++ ] = new Vertex (
				Transformation->ForwardPoint ( Vector3D ( -HalfSize.X, sign * HalfSize.Y, -HalfSize.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisY ) ) );

			vertices [ index++ ] = new Vertex (
				Transformation->ForwardPoint ( Vector3D ( -HalfSize.X, sign * HalfSize.Y, HalfSize.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisY ) ) );

			vertices [ index++ ] = new Vertex (
				Transformation->ForwardPoint ( Vector3D ( HalfSize.X, sign * HalfSize.Y, HalfSize.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisY ) ) );

			vertices [ index++ ] = new Vertex (
				Transformation->ForwardPoint ( Vector3D ( HalfSize.X, sign * HalfSize.Y, -HalfSize.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisY ) ) );
		}

		for ( float sign = -1.0F; sign < 2.0F; sign += 2.0F )
		{
			vertices [ index++ ] = new Vertex (
				Transformation->ForwardPoint ( Vector3D ( -HalfSize.X, -HalfSize.Y, sign * HalfSize.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisZ ) ) );

			vertices [ index++ ] = new Vertex (
				Transformation->ForwardPoint ( Vector3D ( -HalfSize.X, HalfSize.Y, sign * HalfSize.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisZ ) ) );

			vertices [ index++ ] = new Vertex (
				Transformation->ForwardPoint ( Vector3D ( HalfSize.X, HalfSize.Y, sign * HalfSize.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisZ ) ) );

			vertices [ index++ ] = new Vertex (
				Transformation->ForwardPoint ( Vector3D ( HalfSize.X, -HalfSize.Y, sign * HalfSize.Z ) ),
				Normalize ( Transformation->ForwardNormal ( sign * Vector3D :: AxisZ ) ) );
		}

		//-------------------------------------------------------------------------------

		for ( int vertex = 0; vertex < 24; vertex  += 4 )
		{
			Triangles.push_back (
				new Triangle ( vertices [ vertex ], vertices [ vertex + 1 ], vertices [ vertex + 2 ] ) );
			
			Triangles.push_back (
				new Triangle ( vertices [ vertex + 2 ], vertices [ vertex + 3 ], vertices [ vertex ] ) );
		}
	}
}