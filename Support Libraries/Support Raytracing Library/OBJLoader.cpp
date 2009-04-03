#include "OBJLoader.h"

#include <stdlib.h>

#include <stdio.h>

#include <iostream>

#include <fstream>

#include <string>

namespace Raytracing
{
	//------------------------------------- Loading OBJ Model -------------------------------------

	OBJModel * OBJLoader :: LoadModel ( const char * filename )
	{
		FILE * file = fopen ( filename, "rt" );

		if ( file == NULL )
		{
			cout << "ERROR: Could not open model file" << endl;

			return NULL;
		}

		//-------------------------------------------------------------------------------
		
		fseek ( file, 0, SEEK_END );
			
		unsigned int end = ftell ( file );
			
		fseek ( file, 0, SEEK_SET );

		//-------------------------------------------------------------------------------

		if ( 0 == end )
		{
			cout << "ERROR: Model file is empty" << endl;

			return NULL;
		}

		//-------------------------------------------------------------------------------
   	
		char * memory = new char[end];
			
		unsigned int size = fread ( memory, sizeof ( char ), end, file );

		fclose ( file );
		
		//-------------------------------------------------------------------------------
		
		char * position = memory;
		
		char * finish = memory + size;

		//-------------------------------------------------------------------------------

		OBJModel * model = new OBJModel ( );

		while ( position != finish )
		{
			if ( memcmp ( position, "vn", 2 ) == 0 )
				model->NormalNumber++;
			else
				if ( memcmp ( position, "vt", 2 ) == 0 )
					model->TextureNumber++;
				else
					if ( memcmp ( position, "v",  1 ) == 0 )
						model->VertexNumber++;
					else
						if ( memcmp ( position, "f",  1 ) == 0 )
							model->FaceNumber++;

			while ( *position++ != ( char ) 0x0A );
		}

		//-------------------------------------------------------------------------------

		if ( 0 != model->VertexNumber )
		{
			cout << "MODEL: Vertices number = " << model->VertexNumber << endl;
		}
		else
		{
			cout << "ERROR: Model has no vertices" << endl;

			return NULL;
		}

		if ( 0 != model->NormalNumber )
		{
			cout << "MODEL: Normals number = " << model->NormalNumber << endl;
		}
		else
		{
			cout << "WARNING: Model has no normals" << endl;
		}

		if ( 0 != model->TextureNumber )
		{
			cout << "MODEL: Texture coords number = " << model->TextureNumber << endl;
		}
		else
		{
			cout << "WARNING: Model has no texture coordinates" << endl;
		}

		if ( 0 != model->FaceNumber )
		{
			cout << "MODEL: Faces number = " << model->FaceNumber << endl;
		}
		else
		{
			cout << "ERROR: Model has no faces" << endl;

			return NULL;
		}

		//-------------------------------------------------------------------------------

		model->Vertices = new Vector3D [model->VertexNumber];

		model->Normals = new Vector3D [model->NormalNumber];

		model->Textures = new Vector2D [model->TextureNumber];

		model->Faces = new OBJFace [model->FaceNumber];

		//-------------------------------------------------------------------------------

		position = memory;

		int nV = 0, nN = 0, nT = 0, nF = 0;
	
		while ( position != finish )
		{
			if ( memcmp ( position, "vn", 2 ) == 0 )
			{
				sscanf_s ( position, "vn %f %f %f",
					       &model->Normals [nN].X,
						   &model->Normals [nN].Y,
						   &model->Normals [nN].Z );

				nN++;
			}
			else
				if ( memcmp ( position, "vt", 2 ) == 0 )
				{
					sscanf_s ( position, "vt %f %f",
						       &model->Textures [nT].X,
							   &model->Textures [nT].Y );

					nT++;
				}
				else
					if ( memcmp ( position, "v", 1 ) == 0 )
					{
						sscanf_s ( position, "v %f %f %f",
							       &model->Vertices [nV].X,
								   &model->Vertices [nV].Y,
								   &model->Vertices [nV].Z );
						nV++;
					}
					else
						if ( memcmp ( position, "f", 1 ) == 0 )
						{
							if ( 0 != model->TextureNumber )
							{
								if ( 0 != model->NormalNumber )
								{
									sscanf_s ( position, "f %d/%d/%d %d/%d/%d %d/%d/%d",
											   &model->Faces [nF].Vertex[0],
											   &model->Faces [nF].Texture[0],
											   &model->Faces [nF].Normal[0],
											   &model->Faces [nF].Vertex[1],
											   &model->Faces [nF].Texture[1],
											   &model->Faces [nF].Normal[1],
											   &model->Faces [nF].Vertex[2],
											   &model->Faces [nF].Texture[2],
											   &model->Faces [nF].Normal[2] );
								}
								else
								{
									sscanf_s ( position, "f %d/%d %d/%d %d/%d",
											   &model->Faces [nF].Vertex[0],
											   &model->Faces [nF].Texture[0],
											   &model->Faces [nF].Vertex[1],
											   &model->Faces [nF].Texture[1],
											   &model->Faces [nF].Vertex[2],
											   &model->Faces [nF].Texture[2] );
								}
							}
							else
							{
								if ( 0 != model->NormalNumber )
								{
									sscanf_s ( position, "f %d//%d %d//%d %d//%d",
											   &model->Faces [nF].Vertex[0],
											   &model->Faces [nF].Normal[0],
											   &model->Faces [nF].Vertex[1],
											   &model->Faces [nF].Normal[1],
											   &model->Faces [nF].Vertex[2],
											   &model->Faces [nF].Normal[2] );
								}
								else
								{
									sscanf_s ( position, "f %d %d %d",
											   &model->Faces [nF].Vertex[0],
											   &model->Faces [nF].Vertex[1],
											   &model->Faces [nF].Vertex[2] );
								}
							}
							
							nF++;
						}

			while ( *position++ != ( char ) 0x0A );
		}

		return model;
	}
}