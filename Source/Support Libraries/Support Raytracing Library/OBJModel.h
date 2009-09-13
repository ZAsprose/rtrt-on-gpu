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

#pragma once

#ifndef _OBJ_MODEL_

#define _OBJ_MODEL_

#include "Triangle.h"

#include "Texture2D.h"

#include <vector>

#define LENGTH 255

using namespace std;

namespace Raytracing
{
	struct MTLTexture
	{
		//----------------------------- File Name and OpenGL Texture ------------------------------

		char Name [LENGTH];

		TextureData2D * Data;

		//------------------------------- Constructor and Destructor ------------------------------
		
		MTLTexture ( char [], TextureData2D * = NULL );

		~MTLTexture ( void );
	};

	//---------------------------------------------------------------------------------------------

	struct MTLMaterial
	{
		//------------------------------- Material Name in MTL File -------------------------------

		char Name [LENGTH];

		//---------------------------------- Material Properties ----------------------------------

		Material * Properties;

		//-------------------------------------- Constructor --------------------------------------
		
		MTLMaterial ( char [], Material * = NULL );
	};

	//---------------------------------------------------------------------------------------------

	struct OBJFace
	{
		int Vertex [3];

		int Normal [3];

		int TexCoords [3];
	};

	//---------------------------------------------------------------------------------------------

	struct OBJGroup
	{
		//----------------------------- Material Properties and Name ------------------------------

		MTLMaterial * Material;

		//-------------------------------- List of Faces in Group ---------------------------------
		
		vector <OBJFace> Faces;

		//------------------------------ Constructor and Destructor -------------------------------
		
		OBJGroup ( MTLMaterial * = NULL );

		~OBJGroup ( void );
	};

	//---------------------------------------------------------------------------------------------

	struct OBJModel
	{
		//------------------------------ Lists of Vertex Attributes -------------------------------

		vector <Vector3D> Vertices;

		vector <Vector3D> Normals;

		vector <Vector2D> TexCoords;

		//---------------- List of Face Groups ( Sets of Faces with Same Material ) ---------------

		vector <OBJGroup *> Groups;

		//------------------------------ List of Material Properties ------------------------------

		vector <MTLMaterial *> Materials;

		//-------------------------------- List of Raster Textures --------------------------------

		vector <MTLTexture *> Textures;

		//-------------------------------------- Destructor ---------------------------------------
		
		~OBJModel ( void );

		//----------------------------------- Useful Functions ------------------------------------
		
		Vector3D GetMinimum ( void ) const;
		
		Vector3D GetMaximum ( void ) const;
	};
}

#endif