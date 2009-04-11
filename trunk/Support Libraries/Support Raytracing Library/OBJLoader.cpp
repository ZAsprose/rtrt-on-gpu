/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "OBJLoader.h"

#include <stdlib.h>

#include <stdio.h>

#include <iostream>

#include <fstream>

#include <string>

namespace Raytracing
{
	//------------------------------------------ Loading OBJ Model ------------------------------------------

	OBJModel * OBJLoader :: LoadModel ( const char * filename )
	{
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "+++                         OBJ MODEL LOADER                         +++" << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

		FILE * file = fopen ( filename, "rt" );

		if ( file == NULL )
		{
			cout << "ERROR: Could not open model file" << endl;

			return NULL;
		}

		//-----------------------------------------------------------------------------------------
		
		fseek ( file, 0, SEEK_END );
			
		unsigned int end = ftell ( file );
			
		fseek ( file, 0, SEEK_SET );

		//-----------------------------------------------------------------------------------------

		if ( 0 == end )
		{
			cout << "ERROR: Model file is empty" << endl;

			return NULL;
		}

		//-----------------------------------------------------------------------------------------
   	
		char * memory = new char [end];
			
		unsigned int size = fread ( memory, sizeof ( char ), end, file );

		fclose ( file );
		
		//-----------------------------------------------------------------------------------------
		
		char * position = memory;
		
		char * finish = memory + size;

		//-----------------------------------------------------------------------------------------

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

		//-----------------------------------------------------------------------------------------

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

		//-----------------------------------------------------------------------------------------

		model->Vertices = new Vector3D [model->VertexNumber];

		model->Normals = new Vector3D [model->NormalNumber];

		model->Textures = new Vector2D [model->TextureNumber];

		model->Faces = new OBJFace [model->FaceNumber];

		//-----------------------------------------------------------------------------------------

		cout << "LOADING: ";

		float progress = 1.0F;

		//-----------------------------------------------------------------------------------------

		position = memory;

		int vertex = 0, normal = 0, texture = 0, face = 0;

		//-----------------------------------------------------------------------------------------
	
		while ( position != finish )
		{
			if ( memcmp ( position, "vn", 2 ) == 0 )
			{
				sscanf ( position, "vn %f %f %f",
					     &model->Normals [normal].X,
						 &model->Normals [normal].Y,
						 &model->Normals [normal].Z );

				normal++;
			}
			else
				if ( memcmp ( position, "vt", 2 ) == 0 )
				{
					sscanf ( position, "vt %f %f",
						     &model->Textures [texture].X,
							 &model->Textures [texture].Y );

					texture++;
				}
				else
					if ( memcmp ( position, "v", 1 ) == 0 )
					{
						sscanf ( position, "v %f %f %f",
							     &model->Vertices [vertex].X,
								 &model->Vertices [vertex].Y,
								 &model->Vertices [vertex].Z );
						vertex++;
					}
					else
						if ( memcmp ( position, "f", 1 ) == 0 )
						{
							if ( 0 != model->TextureNumber )
							{
								if ( 0 != model->NormalNumber )
								{
									sscanf ( position, "f %d/%d/%d %d/%d/%d %d/%d/%d",
											 &model->Faces [face].Vertex[0],
											 &model->Faces [face].Texture[0],
											 &model->Faces [face].Normal[0],
											 &model->Faces [face].Vertex[1],
											 &model->Faces [face].Texture[1],
											 &model->Faces [face].Normal[1],
											 &model->Faces [face].Vertex[2],
											 &model->Faces [face].Texture[2],
											 &model->Faces [face].Normal[2] );
								}
								else
								{
									sscanf ( position, "f %d/%d %d/%d %d/%d",
											 &model->Faces [face].Vertex[0],
											 &model->Faces [face].Texture[0],
											 &model->Faces [face].Vertex[1],
											 &model->Faces [face].Texture[1],
											 &model->Faces [face].Vertex[2],
											 &model->Faces [face].Texture[2] );
								}
							}
							else
							{
								if ( 0 != model->NormalNumber )
								{
									sscanf ( position, "f %d//%d %d//%d %d//%d",
											 &model->Faces [face].Vertex[0],
											 &model->Faces [face].Normal[0],
											 &model->Faces [face].Vertex[1],
											 &model->Faces [face].Normal[1],
											 &model->Faces [face].Vertex[2],
											 &model->Faces [face].Normal[2] );
								}
								else
								{
									sscanf ( position, "f %d %d %d",
											 &model->Faces [face].Vertex[0],
											 &model->Faces [face].Vertex[1],
											 &model->Faces [face].Vertex[2] );
								}
							}
							
							face++;
						}

			if ( progress > ( finish - position ) / ( float ) size )
			{
				progress -= 0.02F;

				cout << ".";
			}

			while ( *position++ != ( char ) 0x0A );
		}

		cout << endl;

		return model;
	}
}