/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "StaticData.h"

namespace Raytracing
{
	//------------------------------------------- Constant Parameters --------------------------------------------

	const unsigned StaticData :: VertexSize = 1024;

	const unsigned StaticData :: MaterialSize = 1024;
	
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

		PositionTexture->Data = new TextureData2D ( VertexSize, VertexSize, 3 );

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

		for ( unsigned i = 0; i < scene->Primitives.size ( ); i++ )
		{
			scene->Primitives [i]->Properties->Identifier = offset;

			if ( scene->Primitives [i]->Properties->Texture == NULL )
			{
				MaterialTexture->Data->Pixel < Vector4D > ( offset++ ) =
					Vector4D ( scene->Primitives [i]->Properties->Ambient, 0 );
			}
			else
			{
				MaterialTexture->Data->Pixel < Vector4D > ( offset++ ) =
					Vector4D ( scene->Primitives [i]->Properties->Ambient,
					           scene->Primitives [i]->Properties->Texture->GetUnit ( ) + 1 );
			}

			MaterialTexture->Data->Pixel < Vector4D > ( offset++ ) =
				Vector4D ( scene->Primitives [i]->Properties->Diffuse,
				           scene->Primitives [i]->Properties->Scale.X );

			MaterialTexture->Data->Pixel < Vector4D > ( offset++ ) =
				Vector4D ( scene->Primitives [i]->Properties->Specular,
				           scene->Primitives [i]->Properties->Scale.Y );

			MaterialTexture->Data->Pixel < Vector4D > ( offset++ ) =
				Vector4D ( scene->Primitives [i]->Properties->Reflective,
				           scene->Primitives [i]->Properties->Shininess );

			MaterialTexture->Data->Pixel < Vector4D > ( offset++ ) =
				Vector4D ( scene->Primitives [i]->Properties->Refractive,
				           scene->Primitives [i]->Properties->RefractIndex );
		}

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
					VoxelTexture->Data->Pixel < Vector2D > ( x, y, z ) =
						Vector2D ( scene->Grid->GetVoxel ( x, y, z )->Triangles.size ( ), offset );

					for ( unsigned i = 0; i < scene->Grid->GetVoxel ( x, y, z )->Triangles.size ( ); i++ )
					{
						PositionTexture->Data->Pixel < Vector3D > ( offset ) = 
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->VertexA->Position;

						NormalTexture->Data->Pixel < Vector3D > ( offset ) = 
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->VertexA->Normal;

						TexCoordTexture->Data->Pixel < Vector2D > ( offset++ ) = 
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->VertexA->TexCoord;

						//-------------------------------------------------------------------------
						
						PositionTexture->Data->Pixel < Vector3D > ( offset ) = 
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->VertexB->Position;

						NormalTexture->Data->Pixel < Vector3D > ( offset ) = 
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->VertexB->Normal;

						TexCoordTexture->Data->Pixel < Vector2D > ( offset++ ) = 
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->VertexB->TexCoord;

						//-------------------------------------------------------------------------

						PositionTexture->Data->Pixel < Vector3D > ( offset ) = 
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->VertexC->Position;

						NormalTexture->Data->Pixel < Vector4D > ( offset ) = Vector4D (  
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->VertexC->Normal,
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->Properties->Identifier );

						TexCoordTexture->Data->Pixel < Vector2D > ( offset++ ) =
							scene->Grid->GetVoxel ( x, y, z )->Triangles [i]->VertexC->TexCoord;
					}
				}
			}
		}

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
		
		manager->SetUniformVector ( "VoxelTextureStep",
			                         Vector3D ( 1.0F / VoxelTexture->Data->GetWidth ( ),
									            1.0F / VoxelTexture->Data->GetHeight ( ),
												1.0F / VoxelTexture->Data->GetDepth ( ) ) );
		
		manager->SetUniformFloat ( "VertexTextureSize", VertexSize );
		
		manager->SetUniformFloat ( "VertexTextureStep", 1.0F / VertexSize );

		manager->SetUniformFloat ( "MaterialTextureStep", 1.0F / MaterialSize );
	}
}