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
			cout << "ERROR: Could not load MTL materials file" << endl;

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

				//----------------------------- Creating new material -----------------------------

				material = new MTLMaterial ( name );
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
						}
					}

					//---------------- Creating new texture ( if it is necessary ) ----------------

					if ( NULL == texture )
					{
						texture = new MTLTexture ( name,
							new Texture2D ( TextureData2D :: FromTGA ( name ) ) );

						model->Textures.push_back ( texture );
					}

					//------------------------ Setting texture to material ------------------------
					
					material->Texture = texture->Texture;
				}
				else
					if ( memcmp ( position, "Ka", 2 ) == 0 )
					{
						sscanf ( position, "Ka %f %f %f",
								 &material->Ambient.X,
								 &material->Ambient.Y,
								 &material->Ambient.Z );
					}
					else
						if ( memcmp ( position, "Kd", 2 ) == 0 )
						{
							sscanf ( position, "Kd %f %f %f",
									 &material->Diffuse.X,
									 &material->Diffuse.Y,
									 &material->Diffuse.Z );
						}
						else
							if ( memcmp ( position, "Ks", 2 ) == 0 )
							{
								sscanf ( position, "Ks %f %f %f",
										 &material->Specular.X,
										 &material->Specular.Y,
										 &material->Specular.Z );
							}
							else
								if ( memcmp ( position, "Tf", 2 ) == 0 )
								{
									sscanf ( position, "Tf %f %f %f",
											 &material->Transmission.X,
											 &material->Transmission.Y,
											 &material->Transmission.Z );
								}
								else
									if ( memcmp ( position, "Ns", 2 ) == 0 )
									{
										sscanf ( position, "Ns %f",
												 &material->Shininess );
									}
									else
										if ( memcmp ( position, "Ni", 2 ) == 0 )
										{
											sscanf ( position, "Ni %f",
													 &material->Density );
										}
										else
											if ( memcmp ( position, "d", 1 ) == 0 )
											{
												sscanf ( position, "d %f",
														 &material->Dissolve );
											}
											else
												if ( memcmp ( position, "illum", 5 ) == 0 )
												{
													sscanf ( position, "illum %d",
															 &material->Model );
												}

			while ( *position++ != '\n' );
		}

		if ( material != NULL )
		{
			model->Materials.push_back ( material );
		}

		return true;
	}

	//--------------------------------- Loading OBJ Geometry File ---------------------------------

	OBJModel * OBJLoader :: LoadOBJ ( const char * filename )
	{
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "+++                         OBJ MODEL LOADER                         +++" << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

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

		while ( position <= finish )
		{
			if ( memcmp ( position, "mtllib", 6 ) == 0 )
			{
				//-------------------- Groups will be created later ---------------------

				group = NULL;

				//----------------------- Getting MTL file path -------------------------

				char mtlfile [LENGTH];

				sscanf ( position, "mtllib %s", &mtlfile );

				//------------------- Loading materials from MTL file -------------------

				if ( !LoadMTL ( mtlfile, model ) )
				{
					return NULL;
				}
			}
			else
				if ( memcmp ( position, "usemtl", 6 ) == 0 )
				{
					//--------------- Adding previous group to the model ----------------

					if ( NULL != group )
					{
						model->Groups.push_back ( group );
					}

					//---------------------- Getting material name ----------------------

					char name [LENGTH];

					memset ( name, 0, LENGTH );

					sscanf ( position, "usemtl %s", &name );

					//---------------- Finding this material in the list ----------------

					MTLMaterial * material = NULL;

					for ( int index = 0; index < model->Materials.size ( ); index++ )
					{
						if ( memcmp ( model->Materials [index]->Name, name, LENGTH ) == 0 )
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
					if ( memcmp ( position, "vn", 2 ) == 0 )
					{
						Vector3D normal = Vector3D :: Zero;

						sscanf ( position, "vn %f %f %f",
								 &normal.X,
								 &normal.Y,
								 &normal.Z );

						model->Normals.push_back ( normal );
					}
					else
						if ( memcmp ( position, "vt", 2 ) == 0 )
						{
							Vector2D texture = Vector2D :: Zero;

							sscanf ( position, "vt %f %f",
									 &texture.X,
									 &texture.Y );

							model->TexCoords.push_back ( texture );
						}
						else
							if ( memcmp ( position, "v", 1 ) == 0 )
							{
								Vector3D vertex = Vector3D :: Zero;

								sscanf ( position, "v %f %f %f",
										 &vertex.X,
										 &vertex.Y,
										 &vertex.Z );

								model->Vertices.push_back ( vertex );
							}
							else
								if ( memcmp ( position, "f", 1 ) == 0 )
								{
									OBJFace face;

									sscanf ( position, "f %d/%d/%d %d/%d/%d %d/%d/%d",
											 &face.Vertex[0],
											 &face.Texture[0],
											 &face.Normal[0],
											 &face.Vertex[1],
											 &face.Texture[1],
											 &face.Normal[1],
											 &face.Vertex[2],
											 &face.Texture[2],
											 &face.Normal[2] );

									group->Faces.push_back ( face );
								}

			if ( progress > ( finish - position ) / ( float ) size )
			{
				progress -= 0.02F;

				cout << ".";
			}

			while ( *position++ != '\n' );
		}

		cout << endl;

		if ( NULL != group )
		{
			model->Groups.push_back ( group );
		}

		return model;
	}
}