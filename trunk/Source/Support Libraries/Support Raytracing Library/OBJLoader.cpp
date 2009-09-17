/*
   S U P P O R T   R A Y   T R A C I N G   L I B R A R Y

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

#include <stdio.h>

#include <fstream>

#include <direct.h>

namespace Raytracing
{
	//------------------------------------------ Loading Text File ------------------------------------------
	
	char * OBJLoader :: LoadFile ( const char * path, unsigned& size )
	{
		FILE * file = fopen ( path, "rt" );

		if ( NULL == file )
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

	bool OBJLoader :: LoadMTL ( const char * path, OBJModel * model )
	{
		unsigned size = 0;

		char * memory = LoadFile ( path, size );

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
			//-------------------------------- NEW MATERIAL TOKEN ---------------------------------

			if ( memcmp ( position, "newmtl", 6 ) == 0 )
			{
				if ( material != NULL )
				{
					model->Materials.push_back ( material );
				}

				//---------------------------------------------------------------------------------

				char name [LENGTH];

				memset ( name, 0, LENGTH );
				
				sscanf ( position, "newmtl %s", name );

				//---------------------------------------------------------------------------------

				material = new MTLMaterial ( name );

				goto NEWLINE;
			}

			//--------------------------------- DIFFUSE MAP TOKEN ---------------------------------

			if ( memcmp ( position, "map_Kd", 6 ) == 0 )
			{
				//---------------------------------------------------------------------------------

				char name [LENGTH];

				memset ( name, 0, LENGTH );

				sscanf ( position, "map_Kd %s", &name );

				//---------------------------------------------------------------------------------

				MTLTextureData * texture = NULL;

				for ( int index = 0; index < model->Textures.size ( ); index++ )
				{
					if ( memcmp ( model->Textures [index]->Name, name, LENGTH ) == 0 )
					{
						texture = model->Textures [index];
						
						break;
					}
				}

				//---------------------------------------------------------------------------------

				if ( NULL == texture )
				{
					texture = new MTLTextureData ( name, TextureData2D :: FromTGA ( name ) );

					model->Textures.push_back ( texture );
				}

				//---------------------------------------------------------------------------------
				
				material->Properties->TextureData = texture->Data;

				goto NEWLINE;
			}

			//----------------------------- AMBIENT COEFFICIENT TOKEN -----------------------------
	
			if ( memcmp ( position, "Ka", 2 ) == 0 )
			{
				sscanf ( position, "Ka %f %f %f",
						 &material->Properties->Ambient.X,
						 &material->Properties->Ambient.Y,
						 &material->Properties->Ambient.Z );

				goto NEWLINE;
			}

			//----------------------------- DIFFUSE COEFFICIENT TOKEN -----------------------------
	
			if ( memcmp ( position, "Kd", 2 ) == 0 )
			{
				sscanf ( position, "Kd %f %f %f",
						 &material->Properties->Diffuse.X,
						 &material->Properties->Diffuse.Y,
						 &material->Properties->Diffuse.Z );

				goto NEWLINE;
			}

			//----------------------------- SPECULAR COEFFICIENT TOKEN ----------------------------

			if ( memcmp ( position, "Ks", 2 ) == 0 )
			{
				sscanf ( position, "Ks %f %f %f",
						 &material->Properties->Specular.X,
						 &material->Properties->Specular.Y,
						 &material->Properties->Specular.Z );

				goto NEWLINE;
			}

			//---------------------------- SHININESS COEFFICIENT TOKEN ----------------------------
	
			if ( memcmp ( position, "Ns", 2 ) == 0 )
			{
				sscanf ( position, "Ns %f", &material->Properties->Shininess );

				goto NEWLINE;
			}

			//------------------ REFLECTION COEFFICIENT TOKEN ( !NON STANDARD! ) ------------------
		
			if ( memcmp ( position, "Kr", 2 ) == 0 )
			{
				sscanf ( position, "Kr %f %f %f",
					     &material->Properties->Reflection.X,
						 &material->Properties->Reflection.Y,
						 &material->Properties->Reflection.Z );

				goto NEWLINE;
			}

			//------------------ REFRACTION COEFFICIENT TOKEN ( !NON STANDARD! ) ------------------

			if ( memcmp ( position, "Kt", 2 ) == 0 )
			{
				sscanf ( position, "Kt %f %f %f",
					     &material->Properties->Refraction.X,
						 &material->Properties->Refraction.Y,
						 &material->Properties->Refraction.Z );

				goto NEWLINE;
			}

			//---------------- OPTICAL DENSITY COEFFICIENT TOKEN ( !NON STANDARD! ) ---------------

			if ( memcmp ( position, "Ni", 2 ) == 0 )
			{
				sscanf ( position, "Ni %f", &material->Properties->Density );

				goto NEWLINE;
			}

			//------------------- DISSOLVE COEFFICIENT TOKEN ( !NON STANDARD! ) -------------------
	
			if ( memcmp ( position, "Nd", 2 ) == 0 )
			{
				sscanf ( position, "Nd %f", &material->Properties->Dissolve );

				goto NEWLINE;
			}

			NEWLINE: while ( *position++ != '\n' );
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
		int index = 0, length = 0;

		while ( 0 != path [length] )
		{
			if ( '/' == path [length] || '\\' == path [length] )
				index = length;

			length++;
		}

		memcpy ( directory, path, index );

		directory [index] = 0;
	}

	void GetFileName ( const char * path, char * file )
	{
		int index = 0, length = 0;

		while ( 0 != path [length] )
		{
			if ( '/' == path [length] || '\\' == path [length] )
				index = length + 1;

			length++;
		}

		memcpy ( file, path + index, length - index );

		file [length - index] = 0;
	}

	OBJModel * OBJLoader :: LoadOBJ ( const char * path )
	{
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "+++                         OBJ MODEL LOADER                         +++" << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

		char file [LENGTH];

		GetFileName ( path, file );

		cout <<  "File: " << file << endl;

		//-----------------------------------------------------------------------------------------

		char current [LENGTH];
		
		_getcwd ( current, LENGTH );

		char directory [LENGTH];

		GetDirectory ( path, directory );

		_chdir ( directory );

		//-----------------------------------------------------------------------------------------

		unsigned size = 0;

		char * memory = LoadFile ( path, size );

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

		cout << "Loading: ";

		float progress = 1.0F;
		
		//-----------------------------------------------------------------------------------------

		char line [LENGTH];

		while ( position <= finish )
		{
			int length = 0;

			while ( position [length++] != '\n' );

			memcpy ( line, position, length );

			line [length] = 0;

			//-------------------------------- MATERIAL FILE TOKEN --------------------------------

			if ( memcmp ( line, "mtllib", 6 ) == 0 )
			{
				if ( NULL != group )
				{
					delete group;
					
					group = NULL;
				}

				//---------------------------------------------------------------------------------

				char mtlfile [LENGTH];

				sscanf ( line, "mtllib %s", &mtlfile );

				//---------------------------------------------------------------------------------

				if ( !LoadMTL ( mtlfile, model ) )
				{
					return NULL;
				}

				goto NEWLINE;
			}

			//-------------------------------- USE MATERIAL TOKEN ---------------------------------

			if ( memcmp ( line, "usemtl", 6 ) == 0 )
			{
				if ( NULL != group )
				{
					model->Groups.push_back ( group );
				}

				//---------------------------------------------------------------------------------

				char name [LENGTH];

				memset ( name, 0, LENGTH );

				sscanf ( line, "usemtl %s", &name );

				//---------------------------------------------------------------------------------

				MTLMaterial * material = NULL;

				for ( int index = 0; index < model->Materials.size ( ); index++ )
				{
					if ( 0 == memcmp ( model->Materials [index]->Name, name, LENGTH ) )
					{
						material = model->Materials [index];
						
						break;
					}
				}

				if ( NULL == material )
				{
					cout << "WARNING: Could not find specified material" << endl;
				}

				//---------------------------------------------------------------------------------

				group = new OBJGroup ( material );

				goto NEWLINE;
			}

			//-------------------------------- VERTEX NORMAL TOKEN --------------------------------

			if ( memcmp ( line, "vn", 2 ) == 0 )
			{
				Vector3D normal = Vector3D :: Zero;

				sscanf ( line, "vn %f %f %f", &normal.X, &normal.Y, &normal.Z );

				model->Normals.push_back ( normal );

				goto NEWLINE;
			}

			//------------------------------- VERTEX TEXCOORD TOKEN -------------------------------

			if ( memcmp ( line, "vt", 2 ) == 0 )
			{
				Vector2D texcoord = Vector2D :: Zero;

				sscanf ( line, "vt %f %f", &texcoord.X, &texcoord.Y );

				model->TexCoords.push_back ( texcoord );

				goto NEWLINE;
			}

			//------------------------------- VERTEX POSITION TOKEN -------------------------------

			if ( memcmp ( line, "v", 1 ) == 0 )
			{
				Vector3D vertex = Vector3D :: Zero;
				
				sscanf ( line, "v %f %f %f", &vertex.X, &vertex.Y, &vertex.Z );
				
				model->Vertices.push_back ( vertex );

				goto NEWLINE;
			}

			//------------------------------------ FACE TOKEN -------------------------------------

			if ( memcmp ( line, "f", 1 ) == 0 )
			{
				OBJFace face;

				//if ( NULL != group->Material->Properties->TextureData )
				//{
					//sscanf ( line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
					//	&face.Vertex[0], &face.TexCoord[0], &face.Normal[0],
					//	&face.Vertex[1], &face.TexCoord[1], &face.Normal[1],
					//	&face.Vertex[2], &face.TexCoord[2], &face.Normal[2] );
				//}
				//else
				//{
					sscanf ( line, "f %d//%d %d//%d %d//%d",
						&face.Vertex[0], &face.Normal[0],
						&face.Vertex[1], &face.Normal[1],
						&face.Vertex[2], &face.Normal[2] );
				//}

				group->Faces.push_back ( face );

				goto NEWLINE;
			}

			NEWLINE: position += length;

			if ( progress > ( finish - position ) / ( float ) size )
			{
				progress -= 0.02F; cout << ".";
			}
		}

		cout << endl;

		//-----------------------------------------------------------------------------------------

		if ( NULL != group )
		{
			model->Groups.push_back ( group );
		}

		//-----------------------------------------------------------------------------------------

		cout << "Vertices: " << model->Vertices.size ( ) << endl;

		cout << "Groups: " << model->Groups.size ( ) << endl;

		//-----------------------------------------------------------------------------------------

		_chdir ( current );

		return model;
	}
}