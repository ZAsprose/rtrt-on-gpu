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

#include "Mesh.h"

namespace Raytracing
{
	//-------------------------------------------- Constructor --------------------------------------------

	Mesh :: Mesh ( const OBJModel * model,
		           int group,
		           Transform * transformation,
				   const char * name,
				   bool visible ) : Primitive ( transformation,
				                                model->Groups [group]->Material->Properties,
				                                name,
												visible )
	{
		Model = model;

		Group = group;
	}

	//----------------------------------------- Building Triangles ----------------------------------------
			
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

		//-------------------------------------------------------------------------------------------

		if ( 0 != Model->Textures.size ( ) )
		{
			for ( int index = 0; index < Model->Groups [Group]->Faces.size ( ); index++ )
			{
				Vertex * vertexA = new Vertex ( Transformation->ForwardPoint (
					Model->Vertices [  Model->Groups [Group]->Faces [index].Vertex [A] - 1 ] ),
					Normalize ( Transformation->ForwardNormal (
					Model->Normals [ Model->Groups [Group]->Faces [index].Normal [A] - 1 ] ) ),
					Model->TexCoords [ Model->Groups [Group]->Faces [index].TexCoords [A] - 1 ] );

				Vertex * vertexB = new Vertex ( Transformation->ForwardPoint (
					Model->Vertices [ Model->Groups [Group]->Faces [index].Vertex [B] - 1 ] ),
					Normalize ( Transformation->ForwardNormal (
					Model->Normals [ Model->Groups [Group]->Faces [index].Normal [B] - 1 ] ) ),
					Model->TexCoords [ Model->Groups [Group]->Faces [index].TexCoords [B] - 1 ] );

				Vertex * vertexC = new Vertex ( Transformation->ForwardPoint (
					Model->Vertices [ Model->Groups [Group]->Faces [index].Vertex [C] - 1 ] ),
					Normalize ( Transformation->ForwardNormal (
					Model->Normals [ Model->Groups [Group]->Faces [index].Normal [C] - 1 ] ) ),
					Model->TexCoords [ Model->Groups [Group]->Faces [index].TexCoords [C] - 1 ] );
				
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
	}
}