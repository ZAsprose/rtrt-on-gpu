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

#include "OBJLoader.h"

#include "Material.h"

#include <stdlib.h>

#include <stdio.h>

#include <iostream>

#include <fstream>

#include <direct.h>

namespace Raytracing
{
	//------------------------------------------ Loading Text File ------------------------------------------
	
	char * OBJLoader :: LoadFile ( const char * filename, unsigned& size )
	{
		FILE * file = fopen ( filename, "rt" );

		if ( file == NULL )
		{
			cout << "ERROR: Could not open file" << endl;

			return NULL;
		}

		//-----------------------------------------------------------------------------------------
		
		fseek ( file, 0, SEEK_END );
			
		unsigned end = ftell ( file );
			
		fseek ( file, 0, SEEK_SET );

		//-----------------------------------------------------------------------------------------

		if ( 0 == end )
		{
			cout << "ERROR: File is empty" << endl;

			return NULL;
		}

		//-----------------------------------------------------------------------------------------
		
		char * memory = new char [end + 1];
		
		size = fread ( memory, sizeof ( char ), end, file );

		memory [end] = '\n';

		//-----------------------------------------------------------------------------------------
		
		fclose ( file );

		return memory;
	}
	
	//-------------------------------------- Loading MTL Material File --------------------------------------

	bool OBJLoader :: LoadMTL ( const char * filename, OBJModel * model )
	{
		unsigned size = 0;

		char * memory = LoadFile ( filename, size );

		if ( NULL == memory )
		{
			cout << "ERROR: Could not load MTL material file" << endl;

			return false;
		}

		//-----------------------------------------------------------------------------------------
		
		char * position = memory;
		
		char * finish = memory + size;

		//-----------------------------------------------------------------------------------------

		MTLMaterial * material = NULL;

		while ( position <= finish )
		{
			if ( memcmp ( position, "newmtl", 6 ) == 0 )
			{
				//--------------------- Adding previous material to the model ---------------------

				if ( material != NULL )
				{
					model->Materials.push_back ( material );
				}

				//----------------------------- Getting material name -----------------------------

				char name [LENGTH];

				memset ( name, 0, LENGTH );
				
				sscanf ( position, "newmtl %s", name );

				//----------------- Creating new material with default properties -----------------

				material = new MTLMaterial ( name, new Material ( ) );
			}
			else
				if ( memcmp ( position, "map_Kd", 6 ) == 0 )
				{
					//--------------------------- Getting texture name ----------------------------

					char name [LENGTH];

					memset ( name, 0, LENGTH );

					sscanf ( position, "map_Kd %s", &name );

					//--------------- Trying to search texture in previous textures ---------------

					MTLTexture * texture = NULL;

					for ( int i = 0; i < model->Textures.size ( ); i++ )
					{
						if ( memcmp ( model->Textures [i]->Name, name, LENGTH ) == 0 )
						{
							texture = model->Textures [i];
							
							break;
						}
					}

					//---------------- Creating new texture ( if it is necessary ) ----------------

					if ( NULL == texture )
					{
						texture = new MTLTexture ( name, TextureData2D :: FromTGA ( name ) );

						model->Textures.push_back ( texture );
					}

					//------------------------ Setting texture to material ------------------------
					
					material->Properties->Data = texture->Data;
				}
				else
					if ( memcmp ( position, "Ka", 2 ) == 0 )
					{
						sscanf ( position, "Ka %f %f %f",
								 &material->Properties->Ambient.X,
								 &material->Properties->Ambient.Y,
								 &material->Properties->Ambient.Z );
					}
					else
						if ( memcmp ( position, "Kd", 2 ) == 0 )
						{
							sscanf ( position, "Kd %f %f %f",
									 &material->Properties->Diffuse.X,
									 &material->Properties->Diffuse.Y,
									 &material->Properties->Diffuse.Z );
						}
						else
							if ( memcmp ( position, "Ks", 2 ) == 0 )
							{
								sscanf ( position, "Ks %f %f %f",
										 &material->Properties->Specular.X,
										 &material->Properties->Specular.Y,
										 &material->Properties->Specular.Z );
							}
							else
								if ( memcmp ( position, "Ns", 2 ) == 0 )
								{
									sscanf ( position, "Ns %f",
										&material->Properties->Shininess );
								}
								else
									if ( memcmp ( position, "Kr", 2 ) == 0 )
									{
										sscanf ( position, "Kr %f %f %f",
											&material->Properties->Reflection.X,
											&material->Properties->Reflection.Y,
											&material->Properties->Reflection.Z );
									}
									else
										if ( memcmp ( position, "Kt", 2 ) == 0 )
										{
											sscanf ( position, "Kt %f %f %f",
												&material->Properties->Refraction.X,
												&material->Properties->Refraction.Y,
												&material->Properties->Refraction.Z );
										}
										else
											if ( memcmp ( position, "Ni", 2 ) == 0 )
											{
												sscanf ( position, "Ni %f",
														 &material->Properties->Density );
											}
											else
												if ( memcmp ( position, "Nd", 2 ) == 0 )
												{
													sscanf ( position, "Nd %f",
															 &material->Properties->Dissolve );
												}

			while ( *position++ != '\n' );
		}

		if ( material != NULL )
		{
			model->Materials.push_back ( material );
		}

		return true;
	}

	//-------------------------------------- Loading OBJ Geometry File --------------------------------------

	void GetDirectory ( const char * path, char * directory )
	{
		int length = 0;

		int index = 0;

		while ( 0 != path [index] )
		{
			if ( '/' == path [index] || '\\' == path [index] )
				length = index;

			index++;
		}

		memcpy ( directory, path, length );

		directory [length] = 0;
	}

	void GetFileName ( const char * path, char * file )
	{
		int length = 0;

		int index = 0;

		while ( 0 != path [index] )
		{
			if ( '/' == path [index] || '\\' == path [index] )
				length = index;

			index++;
		}

		memcpy ( file, path, length );

		file [length] = 0;
	}

	OBJModel * OBJLoader :: LoadOBJ ( const char * filename )
	{
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "+++                         OBJ MODEL LOADER                         +++" << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

		char current [LENGTH];
		
		_getcwd ( current, LENGTH );

		char directory [LENGTH];

		GetDirectory ( filename, directory );

		_chdir ( directory );

		//-----------------------------------------------------------------------------------------

		unsigned size = 0;

		char * memory = LoadFile ( filename, size );

		if ( NULL == memory )
		{
			cout << "ERROR: Could not load OBJ model file" << endl;

			return NULL;
		}

		//-----------------------------------------------------------------------------------------
		
		char * position = memory;
		
		char * finish = memory + size;

		//-----------------------------------------------------------------------------------------

		OBJModel * model = new OBJModel ( );

		OBJGroup * group = new OBJGroup ( );

		//-----------------------------------------------------------------------------------------

		cout << "LOADING: ";

		float progress = 1.0F;
		
		//-----------------------------------------------------------------------------------------

		char line [LENGTH];

		while ( position <= finish )
		{
			//-------------------------------------------------------------------------------------

			int length = 0;

			while ( position [length++] != '\n' );

			memcpy ( line, position, length );

			line [length - 1] = 0;

			//-------------------------------------------------------------------------------------

			if ( memcmp ( line, "mtllib", 6 ) == 0 )
			{
				//-------------------- Groups must be created later ---------------------

				if ( NULL != group )
				{
					delete group;

					group = NULL;
				}

				//----------------------- Getting MTL file path -------------------------

				char mtlfile [LENGTH];

				sscanf ( line, "mtllib %s", &mtlfile );

				//------------------- Loading materials from MTL file -------------------

				if ( !LoadMTL ( mtlfile, model ) )
				{
					return NULL;
				}
			}
			else
				if ( memcmp ( line, "usemtl", 6 ) == 0 )
				{
					//--------------- Adding previous group to the model ----------------

					if ( NULL != group )
					{
						model->Groups.push_back ( group );
					}

					//---------------------- Getting material name ----------------------

					char name [LENGTH];

					memset ( name, 0, LENGTH );

					sscanf ( line, "usemtl %s", &name );

					//---------------- Finding this material in the list ----------------

					MTLMaterial * material = NULL;

					for ( int index = 0; index < model->Materials.size ( ); index++ )
					{
						if ( 0 == memcmp (
							model->Materials [index]->Name,name, LENGTH ) )
						{
							material = model->Materials [index];
							
							break;
						}
					}

					if ( NULL == material )
					{
						cout << "WARNING: Could not find specified material" << endl;
					}

					//------------ Creating new group with specified material -----------

					group = new OBJGroup ( material );
				}
				else
					if ( memcmp ( line, "vn", 2 ) == 0 )
					{
						Vector3D normal = Vector3D :: Zero;

						sscanf ( line, "vn %f %f %f",
								 &normal.X,
								 &normal.Y,
								 &normal.Z );

						model->Normals.push_back ( normal );
					}
					else
						if ( memcmp ( line, "vt", 2 ) == 0 )
						{
							Vector2D texcoords = Vector2D :: Zero;

							sscanf ( line, "vt %f %f",
									 &texcoords.X,
									 &texcoords.Y );

							model->TexCoords.push_back ( texcoords );
						}
						else
							if ( memcmp ( line, "v", 1 ) == 0 )
							{
								Vector3D vertex = Vector3D :: Zero;
								
								sscanf ( line, "v %f %f %f",
										 &vertex.X,
										 &vertex.Y,
										 &vertex.Z );
								
								model->Vertices.push_back ( vertex );
							}
							else
								if ( memcmp ( line, "f", 1 ) == 0 )
								{
									OBJFace face;

									sscanf ( line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
											 &face.Vertex[0],
											 &face.TexCoords[0],
											 &face.Normal[0],
											 &face.Vertex[1],
											 &face.TexCoords[1],
											 &face.Normal[1],
											 &face.Vertex[2],
											 &face.TexCoords[2],
											 &face.Normal[2] );

									group->Faces.push_back ( face );
								}

			if ( progress > ( finish - position ) / ( float ) size )
			{
				progress -= 0.02F; cout << ".";
			}

			position += length;
		}

		//-------------------------------------------------------------------------------

		cout << endl;

		cout << "VERTICES: " << model->Vertices.size ( ) << endl;

		cout << "GROUPS: " << model->Groups.size ( ) << endl;

		//-------------------------------------------------------------------------------

		if ( NULL != group )
		{
			model->Groups.push_back ( group );
		}

		//-------------------------------------------------------------------------------

		_chdir ( current );

		return model;
	}
}