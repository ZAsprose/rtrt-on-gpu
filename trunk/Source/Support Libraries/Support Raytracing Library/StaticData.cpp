/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "StaticData.h"

namespace Raytracing
{
	//------------------------------------------- Constant Parameters --------------------------------------------

	const unsigned StaticData :: VertexSize = 2048;

	const unsigned StaticData :: MaterialSize = 2048;
	
	const unsigned StaticData :: VoxelUnit = 8;
	
	const unsigned StaticData :: PositionUnit = 9;
	
	const unsigned StaticData :: NormalUnit = 10;

	const unsigned StaticData :: TexCoordUnit = 11;

	const unsigned StaticData :: MaterialUnit = 12;

	//---------------------------------------- Constructor and Destructor ----------------------------------------
	
	StaticData :: StaticData ( void )
	{
		VoxelTexture = new Texture3D ( VoxelUnit );

		//-------------------------------------------------------------------------------
		
		PositionTexture = new Texture2D ( PositionUnit, GL_TEXTURE_RECTANGLE_ARB );
		
		NormalTexture = new Texture2D ( NormalUnit, GL_TEXTURE_RECTANGLE_ARB );

		TexCoordTexture = new Texture2D ( TexCoordUnit, GL_TEXTURE_RECTANGLE_ARB );

		MaterialTexture = new Texture1D ( MaterialUnit );

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

	//--------------------------------------------- Data Generation ----------------------------------------------

	void StaticData :: SetupTextures ( Scene * scene )
	{
		//-------------------------------- Generating material data -------------------------------

		int offset = 0;

		int unit = 0;

		for ( unsigned i = 0; i < scene->Primitives.size ( ); i++ )
		{
			scene->Primitives [i]->Properties->Identifier = offset;

			//---------------------------------------------------------------------------

			MaterialTexture->Data->Pixel < Vector3D > ( offset++ ) =
				scene->Primitives [i]->Properties->Ambient;

			MaterialTexture->Data->Pixel < Vector3D > ( offset++ ) =
				scene->Primitives [i]->Properties->Diffuse;

			MaterialTexture->Data->Pixel < Vector4D > ( offset++ ) = Vector4D (
				scene->Primitives [i]->Properties->Specular,
				scene->Primitives [i]->Properties->Shininess );

			//---------------------------------------------------------------------------

			MaterialTexture->Data->Pixel < Vector4D > ( offset++ ) = Vector4D (
				scene->Primitives [i]->Properties->Reflection,
				scene->Primitives [i]->Properties->Dissolve );

			MaterialTexture->Data->Pixel < Vector4D > ( offset++ ) = Vector4D (
				scene->Primitives [i]->Properties->Refraction,
				scene->Primitives [i]->Properties->Density );

			//---------------------------------------------------------------------------
			
			//if ( scene->Primitives [i]->Properties->Texture == NULL )
			//{
			//	MaterialTexture->Data->Pixel < Vector3D > ( offset++ ) = Vector3D (
			//		scene->Primitives [i]->Properties->Scale, 0 );
			//}
			//else
			//{
			//	MaterialTexture->Data->Pixel < Vector3D > ( offset++ ) = Vector3D (
			//		scene->Primitives [i]->Properties->Scale,
			//		scene->Primitives [i]->Properties->Texture );
			//}
		}

		cout << "MATERIAL MEMORY: " << ( int ) ( 100.0F * offset / MaterialTexture->Data->GetWidth ( ) ) << endl;

		//-------------------------------- Generating geometry data -------------------------------

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
					int trianglesNumber = scene->Grid->GetVoxel ( x, y, z )->Triangles.size ( );

					int emptyRadius = ( trianglesNumber ) ? 0 : scene->Grid->GetVoxel ( x, y, z )->EmptyRadius;

					VoxelTexture->Data->Pixel < Vector3D > ( x, y, z ) =
						Vector3D ( trianglesNumber, offset, emptyRadius );

					for ( unsigned i = 0; i < scene->Grid->GetVoxel ( x, y, z )->Triangles.size ( ); i++ )
					{
						Vector3D normal =
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->GetNormal ( );

						//-------------------------------------------------------------------------
						
						PositionTexture->Data->Pixel < Vector4D > ( offset ) = Vector4D (
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->VertexA->Position,
							normal.X );

						NormalTexture->Data->Pixel < Vector3D > ( offset ) = 
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->VertexA->Normal;

						TexCoordTexture->Data->Pixel < Vector2D > ( offset++ ) = 
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->VertexA->TexCoord;

						//-------------------------------------------------------------------------
						
						PositionTexture->Data->Pixel < Vector4D > ( offset ) = Vector4D (
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->VertexB->Position,
							normal.Y );

						NormalTexture->Data->Pixel < Vector3D > ( offset ) = 
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->VertexB->Normal;

						TexCoordTexture->Data->Pixel < Vector2D > ( offset++ ) = 
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->VertexB->TexCoord;

						//-------------------------------------------------------------------------

						PositionTexture->Data->Pixel < Vector4D > ( offset ) = Vector4D (
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->VertexC->Position,
							normal.Z );

						NormalTexture->Data->Pixel < Vector4D > ( offset ) = Vector4D (  
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->VertexC->Normal,
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->Properties->Identifier );

						TexCoordTexture->Data->Pixel < Vector2D > ( offset++ ) =
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->VertexC->TexCoord;
					}
				}
			}
		}

		cout << "VERTEX MEMORY: " << ( int ) ( 100.0F * offset / (PositionTexture->Data->GetWidth ( ) * PositionTexture->Data->GetHeight ( )) ) << endl;

		//---------------------------------- Setup data textures ----------------------------------

		VoxelTexture->Setup ( );

		PositionTexture->Setup ( );

		NormalTexture->Setup ( );

		TexCoordTexture->Setup ( );

		MaterialTexture->Setup ( );
	}

	//---------------------------------------------- Apply Settings ----------------------------------------------

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

		manager->SetUniformInteger ( "ImageTexture0", 0 );

		manager->SetUniformInteger ( "ImageTexture1", 1 );

		manager->SetUniformInteger ( "ImageTexture2", 2 );

		manager->SetUniformInteger ( "ImageTexture3", 3 );

		manager->SetUniformInteger ( "ImageTexture4", 4 );

		manager->SetUniformInteger ( "ImageTexture5", 5 );

		manager->SetUniformInteger ( "ImageTexture6", 6 );

		manager->SetUniformInteger ( "ImageTexture7", 7 );
	}
}