/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "Mesh.h"

namespace Raytracing
{
	//---------------------------------------- Constructor ----------------------------------------

	Mesh :: Mesh ( const OBJModel * model,
		           int group,
		           Transform * transformation,
				   Material * properties,
				   const char * name,
				   bool visible ) : Primitive ( transformation, properties, name, visible )
	{
		Model = model;

		Group = group;
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

		if ( 0 != Model->Textures.size ( ) )
		{
			for ( int index = 0; index < Model->Groups [Group]->Faces.size ( ); index++ )
			{
				Vertex * vertexA = new Vertex ( Transformation->ForwardPoint (
					Model->Vertices [  Model->Groups [Group]->Faces [index].Vertex [A] - 1 ] ),
					Normalize ( Transformation->ForwardNormal (
					Model->Normals [ Model->Groups [Group]->Faces [index].Normal [A] - 1 ] ) ),
					Model->TexCoords [ Model->Groups [Group]->Faces [index].Texture [A] - 1 ] );

				Vertex * vertexB = new Vertex ( Transformation->ForwardPoint (
					Model->Vertices [ Model->Groups [Group]->Faces [index].Vertex [B] - 1 ] ),
					Normalize ( Transformation->ForwardNormal (
					Model->Normals [ Model->Groups [Group]->Faces [index].Normal [B] - 1 ] ) ),
					Model->TexCoords [ Model->Groups [Group]->Faces [index].Texture [B] - 1 ] );

				Vertex * vertexC = new Vertex ( Transformation->ForwardPoint (
					Model->Vertices [ Model->Groups [Group]->Faces [index].Vertex [C] - 1 ] ),
					Normalize ( Transformation->ForwardNormal (
					Model->Normals [ Model->Groups [Group]->Faces [index].Normal [C] - 1 ] ) ),
					Model->TexCoords [ Model->Groups [Group]->Faces [index].Texture [C] - 1 ] );
				
				Triangles.push_back (
					new Triangle ( vertexA, vertexB, vertexC, Properties ) );
			}
		}
		else
		{
			for ( int index = 0; index < Model->Groups [Group]->Faces.size ( ); index++ )
			{
				Vertex * vertexA = new Vertex ( Transformation->ForwardPoint (
					Model->Vertices [ Model->Groups [Group]->Faces [index].Vertex [A] - 1 ] ),
					Normalize ( Transformation->ForwardNormal (
					Model->Normals [ Model->Groups [Group]->Faces [index].Normal [A] - 1 ] ) ) );

				Vertex * vertexB = new Vertex ( Transformation->ForwardPoint (
					Model->Vertices [ Model->Groups [Group]->Faces [index].Vertex [B] - 1 ] ),
					Normalize ( Transformation->ForwardNormal (
					Model->Normals [ Model->Groups [Group]->Faces [index].Normal [B] - 1 ] ) ) );

				Vertex * vertexC = new Vertex ( Transformation->ForwardPoint (
					Model->Vertices [ Model->Groups [Group]->Faces [index].Vertex [C] - 1 ] ),
					Normalize ( Transformation->ForwardNormal (
					Model->Normals [ Model->Groups [Group]->Faces [index].Normal [C] - 1 ] ) ) );
				
				Triangles.push_back (
					new Triangle ( vertexA, vertexB, vertexC, Properties ) );
			}
		}

		//-------------------------------------------------------------------------------

		if ( NULL != Model->Groups [Group]->Material )
		{
			Properties->Ambient = Model->Groups [Group]->Material->Ambient;

			Properties->Diffuse = Model->Groups [Group]->Material->Diffuse;

			Properties->Shininess = Model->Groups [Group]->Material->Shininess;

			if ( 0.0F != Properties->Shininess )
			{
				Properties->Specular = Model->Groups [Group]->Material->Specular;
			}
			else
			{
				Properties->Specular = Vector3D :: Zero;
			}

			//---------------------------------------------------------------------------

			if ( Model->Groups [Group]->Material->Model > 2 )
			{
				Properties->Reflection = Model->Groups [Group]->Material->Specular; 
			}

			if ( Model->Groups [Group]->Material->Model == 6 )
			{
				Properties->Refraction = Model->Groups [Group]->Material->Transmission *
					( Vector3D :: Unit - Model->Groups [Group]->Material->Specular ); 
			}

			Properties->Dissolve = Model->Groups [Group]->Material->Dissolve;
			
			//---------------------------------------------------------------------------

			Properties->Texture = Model->Groups [Group]->Material->Texture; 
		}
	}
}