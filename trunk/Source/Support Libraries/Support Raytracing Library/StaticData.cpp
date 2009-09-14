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

#include "StaticData.h"

namespace Raytracing
{
	//----------------------------------------- Constant Parameters -----------------------------------------

	const unsigned StaticData :: VertexSize = 2048;

	const unsigned StaticData :: MaterialSize = 8192;

	const unsigned StaticData :: TextureUnit = 0;

	const unsigned StaticData :: VoxelUnit = 1;
	
	const unsigned StaticData :: PositionUnit = 2;
	
	const unsigned StaticData :: NormalUnit = 3;

	const unsigned StaticData :: TexCoordUnit = 4;

	const unsigned StaticData :: MaterialUnit = 5;

	//------------------------------------- Constructor and Destructor --------------------------------------
	
	StaticData :: StaticData ( void )
	{
		VoxelTexture = new Texture3D ( VoxelUnit );

		//-------------------------------------------------------------------------------
		
		PositionTexture = new Texture2D ( PositionUnit, GL_TEXTURE_RECTANGLE_ARB );
		
		NormalTexture = new Texture2D ( NormalUnit, GL_TEXTURE_RECTANGLE_ARB );

		TexCoordTexture = new Texture2D ( TexCoordUnit, GL_TEXTURE_RECTANGLE_ARB );

		MaterialTexture = new Texture1D ( MaterialUnit );

		//-------------------------------------------------------------------------------

		TextureArray = new Texture3D ( TextureUnit, GL_TEXTURE_2D_ARRAY );

		TextureArray->FilterMode = FilterMode :: Linear;

		TextureArray->WrapMode = WrapMode :: Repeat;

		//-------------------------------------------------------------------------------

		PositionTexture->Data = new TextureData2D ( VertexSize, VertexSize, 4 );

		NormalTexture->Data = new TextureData2D ( VertexSize, VertexSize, 4 );

		TexCoordTexture->Data =  new TextureData2D ( VertexSize, VertexSize, 3 );

		MaterialTexture->Data = new TextureData1D ( MaterialSize, 4 );
	}

	StaticData :: ~StaticData ( void )
	{
		delete VoxelTexture;

		delete PositionTexture;

		delete NormalTexture;

		delete TexCoordTexture;

		delete MaterialTexture;
	}

	//--------------------------- Fetching Texture Data with Bilinear Filtration ----------------------------

	Vector3D StaticData :: FilterData ( TextureData2D * data, float x, float y )
	{
		int xMin = floor ( x ),
			yMin = floor ( y );

		float xRatio = x - xMin,
			  yRatio = y - yMin;

		int xMax = min ( xMin + 1, data->GetWidth ( ) - 1 ),
			yMax = min ( yMin + 1, data->GetHeight ( ) - 1 );

		return Mix ( Mix ( data->Pixel<Vector3D> ( xMin, yMin ),
			               data->Pixel<Vector3D> ( xMax, yMin ),
						   xRatio ),
					 Mix ( data->Pixel<Vector3D> ( xMin, yMax ),
					       data->Pixel<Vector3D> ( xMax, yMax ),
						   xRatio ),
					 yRatio );
	}

	//------------------------------- Building Static Data for Specified Scene ------------------------------

	void StaticData :: BuildData ( Scene * scene )
	{
		//------------------------------ Generating raster textures data -------------------------------

		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "+++                       BUILDING STATIC DATA                       +++" << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

		if ( NULL != TextureArray->Data )
		{
			delete TextureArray->Data;
		}

		int width = 0, height = 0;

		for ( int index = 0; index < scene->TextureData.size ( ); index++ )
		{
			if ( scene->TextureData [index]->GetWidth ( ) > width )
				width = scene->TextureData [index]->GetWidth ( );

			if ( scene->TextureData [index]->GetHeight ( ) > height )
				height = scene->TextureData [index]->GetHeight ( );
		}

		//----------------------------------------------------------------------------------------------

		TextureArray->Data = new TextureData3D ( width, height, scene->TextureData.size ( ) + 1 );

		for ( int x = 0; x < width; x++ )
		{
			for ( int y = 0; y < height; y++ )
			{
				TextureArray->Data->Pixel<Vector3D> ( x, y, 0 ) = Vector3D :: Unit;
			}
		}

		for ( int index = 0; index < scene->TextureData.size ( ); index++ )
		{
			scene->TextureData [index]->Identifier = index + 1;

			for ( int x = 0; x < width; x++ )
			{
				for ( int y = 0; y < height; y++ )
				{
					TextureArray->Data->Pixel<Vector3D> ( x, y, index + 1 ) = FilterData (
						scene->TextureData [index],
						scene->TextureData [index]->GetWidth ( ) * x / ( float ) width,
						scene->TextureData [index]->GetHeight ( ) * y / ( float ) height );
				}
			}			
		}

		//---------------------------------- Generating material data ----------------------------------

		int offset = 0;

		for ( unsigned i = 0; i < scene->Primitives.size ( ); i++ )
		{
			scene->Primitives [i]->Properties->Identifier = offset;

			//------------------------------------------------------------------------------------------

			MaterialTexture->Data->Pixel <Vector3D> ( offset++ ) =
				scene->Primitives [i]->Properties->Ambient;

			MaterialTexture->Data->Pixel <Vector3D> ( offset++ ) =
				scene->Primitives [i]->Properties->Diffuse;

			MaterialTexture->Data->Pixel <Vector4D> ( offset++ ) = Vector4D (
				scene->Primitives [i]->Properties->Specular,
				scene->Primitives [i]->Properties->Shininess );

			//------------------------------------------------------------------------------------------

			MaterialTexture->Data->Pixel <Vector4D> ( offset++ ) = Vector4D (
				scene->Primitives [i]->Properties->Reflection,
				scene->Primitives [i]->Properties->Dissolve );

			MaterialTexture->Data->Pixel <Vector4D> ( offset++ ) = Vector4D (
				scene->Primitives [i]->Properties->Refraction,
				scene->Primitives [i]->Properties->Density );

			//------------------------------------------------------------------------------------------
			
			if ( scene->Primitives [i]->Properties->Data == NULL )
			{
				MaterialTexture->Data->Pixel <Vector3D> ( offset++ ) = Vector3D (
					scene->Primitives [i]->Properties->Scale, 0 );
			}
			else
			{
				MaterialTexture->Data->Pixel <Vector3D> ( offset++ ) = Vector3D (
					scene->Primitives [i]->Properties->Scale,
					scene->Primitives [i]->Properties->Data->Identifier );
			}
		}

		float used = floor ( 100.0F * offset / MaterialSize );

		cout << "Material Memory: " << used << "%" << endl;

		//---------------------------------- Generating geometry data ----------------------------------

		if ( NULL != VoxelTexture->Data )
		{
			delete VoxelTexture->Data;
		}

		VoxelTexture->Data = new TextureData3D ( scene->Grid->GetPartitionsX ( ), 
			                                     scene->Grid->GetPartitionsY ( ),
												 scene->Grid->GetPartitionsZ ( ) );
		offset = 0;

		for ( int x = 0; x < scene->Grid->GetPartitionsX ( ); x++ )
		{
			for ( int y = 0; y < scene->Grid->GetPartitionsY ( ); y++ )
			{
				for ( int z = 0; z < scene->Grid->GetPartitionsX ( ); z++ )
				{
					int triangles = scene->Grid->Voxels [x][y][z]->Triangles.size ( );

					int proximity = scene->Grid->Voxels [x][y][z]->Proximity;

					VoxelTexture->Data->Pixel <Vector3D> ( x, y, z ) =
						Vector3D ( triangles, offset, proximity );

					//----------------------------------------------------------------------------------

					for ( unsigned i = 0; i < triangles; i++ )
					{
						Vector3D normal =
							scene->Grid->Voxels [x][y][z]->Triangles [i]->GetNormal ( );

						//------------------------------------------------------------------------------
						
						PositionTexture->Data->Pixel <Vector4D> ( offset ) = Vector4D (
							scene->Grid->Voxels [x][y][z]->Triangles [i]->VertexA->Position,
							normal.X );

						NormalTexture->Data->Pixel <Vector3D> ( offset ) = 
							scene->Grid->Voxels [x][y][z]->Triangles [i]->VertexA->Normal;

						TexCoordTexture->Data->Pixel <Vector2D> ( offset++ ) = 
							scene->Grid->Voxels [x][y][z]->Triangles [i]->VertexA->TexCoord;

						//------------------------------------------------------------------------------
						
						PositionTexture->Data->Pixel <Vector4D> ( offset ) = Vector4D (
							scene->Grid->Voxels [x][y][z]->Triangles [i]->VertexB->Position,
							normal.Y );

						NormalTexture->Data->Pixel <Vector3D> ( offset ) = 
							scene->Grid->Voxels [x][y][z]->Triangles [i]->VertexB->Normal;

						TexCoordTexture->Data->Pixel <Vector2D> ( offset++ ) = 
							scene->Grid->Voxels [x][y][z]->Triangles [i]->VertexB->TexCoord;

						//------------------------------------------------------------------------------

						PositionTexture->Data->Pixel <Vector4D> ( offset ) = Vector4D (
							scene->Grid->Voxels [x][y][z]->Triangles [i]->VertexC->Position,
							normal.Z );

						NormalTexture->Data->Pixel <Vector4D> ( offset ) = Vector4D (  
							scene->Grid->Voxels [x][y][z]->Triangles [i]->VertexC->Normal,
							scene->Grid->Voxels [x][y][z]->Triangles [i]->Properties->Identifier );

						TexCoordTexture->Data->Pixel <Vector2D> ( offset++ ) =
							scene->Grid->Voxels [x][y][z]->Triangles [i]->VertexC->TexCoord;
					}
				}
			}
		}

		used = floor ( 100.0F * offset / ( VertexSize * VertexSize ) );

		cout << "Vertex Memory: " << used << "%" << endl;

		//------------------------------------ Setup data textures -------------------------------------

		TextureArray->Setup ( );

		VoxelTexture->Setup ( );

		PositionTexture->Setup ( );

		NormalTexture->Setup ( );

		TexCoordTexture->Setup ( );

		MaterialTexture->Setup ( );
	}

	//------------------------------------- Applying Settings to Shaders ------------------------------------

	void StaticData :: SetShaderData ( ShaderManager * manager )
	{
		manager->SetTexture ( "VoxelTexture", VoxelTexture );

		manager->SetTexture ( "PositionTexture", PositionTexture );

		manager->SetTexture ( "NormalTexture", NormalTexture );

		manager->SetTexture ( "TexCoordTexture", TexCoordTexture );

		manager->SetTexture ( "MaterialTexture", MaterialTexture );

		manager->SetUniformFloat ( "VertexTextureSize", VertexSize );
		
		manager->SetUniformVector ( "VoxelTextureStep",
			                         Vector3D ( 1.0F / VoxelTexture->Data->GetWidth ( ),
									            1.0F / VoxelTexture->Data->GetHeight ( ),
												1.0F / VoxelTexture->Data->GetDepth ( ) ) );
		
		manager->SetUniformFloat ( "VertexTextureStep", 1.0F / VertexSize );

		manager->SetUniformFloat ( "MaterialTextureStep", 1.0F / MaterialSize );
	}
}