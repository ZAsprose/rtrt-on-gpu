/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "Mesh.h"

namespace Raytracing
{
	//---------------------------------------- Constructor ----------------------------------------

	Mesh :: Mesh ( const OBJModel * model,
		           Transform * transformation,
				   Material * properties,
				   const char * name ) : Primitive ( transformation, properties, name )
	{
		Model = model;
	}

	//-------------------------------------- Build Triangles --------------------------------------
			
	void Mesh :: Tesselate ( void )
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
		
		if ( Model->TextureNumber != 0 )
		{
			for ( int index = 0; index < Model->FaceNumber; index++ )
			{
				Vertex * vertexA = new Vertex ( Transformation->ForwardPoint (
					Model->Vertices [ Model->Faces [index].Vertex [A] - 1 ] ),
					Normalize ( Transformation->ForwardNormal (
					Model->Normals [ Model->Faces [index].Normal [A] - 1 ] ) ),
					Model->Textures [ Model->Faces [index].Texture [A] - 1 ] );

				Vertex * vertexB = new Vertex ( Transformation->ForwardPoint (
					Model->Vertices [ Model->Faces [index].Vertex [B] - 1 ] ),
					Normalize ( Transformation->ForwardNormal (
					Model->Normals [ Model->Faces [index].Normal [B] - 1 ] ) ),
					Model->Textures [ Model->Faces [index].Texture [B] - 1 ] );

				Vertex * vertexC = new Vertex ( Transformation->ForwardPoint (
					Model->Vertices [ Model->Faces [index].Vertex [C] - 1 ] ),
					Normalize ( Transformation->ForwardNormal (
					Model->Normals [ Model->Faces [index].Normal [C] - 1 ] ) ),
					Model->Textures [ Model->Faces [index].Texture [C] - 1 ] );
				
				Triangles.push_back (
					new Triangle ( vertexA, vertexB, vertexC, Properties ) );
			}
		}
		else
		{
			for ( int index = 0; index < Model->FaceNumber; index++ )
			{
				Vertex * vertexA = new Vertex ( Transformation->ForwardPoint (
					Model->Vertices [ Model->Faces [index].Vertex [A] - 1 ] ),
					Normalize ( Transformation->ForwardNormal (
					Model->Normals [ Model->Faces [index].Normal [A] - 1 ] ) ) );

				Vertex * vertexB = new Vertex ( Transformation->ForwardPoint (
					Model->Vertices [ Model->Faces [index].Vertex [B] - 1 ] ),
					Normalize ( Transformation->ForwardNormal (
					Model->Normals [ Model->Faces [index].Normal [B] - 1 ] ) ) );

				Vertex * vertexC = new Vertex ( Transformation->ForwardPoint (
					Model->Vertices [ Model->Faces [index].Vertex [C] - 1 ] ),
					Normalize ( Transformation->ForwardNormal (
					Model->Normals [ Model->Faces [index].Normal [C] - 1 ] ) ) );
				
				Triangles.push_back (
					new Triangle ( vertexA, vertexB, vertexC, Properties ) );
			}
		}
	}
}