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

#pragma once

#ifndef _OBJ_MODEL_

#define _OBJ_MODEL_

#include "Triangle.h"

#include <vector>

#define LENGTH 255

using namespace std;

namespace Raytracing
{
	struct MTLTextureData
	{
		//--------------------------------- File Name in MTL File ---------------------------------

		char Name [LENGTH];

		//------------------------ Texture Data ( DELETED IN SCENE CLASS ) ------------------------

		TextureData2D * Data;

		//-------------------------------------- Constructor --------------------------------------
		
		MTLTextureData ( char [], TextureData2D * = NULL );
	};

	//---------------------------------------------------------------------------------------------

	struct MTLMaterial
	{
		//------------------------------- Material Name in MTL File -------------------------------

		char Name [LENGTH];

		//------------------ Material Properties ( DELETED IN PRIMITIVE CLASS ) -------------------

		Material * Properties;

		//-------------------------------------- Constructor --------------------------------------
		
		MTLMaterial ( char [], Material * = new Material ( ) );
	};

	//---------------------------------------------------------------------------------------------

	struct OBJFace
	{
		int Vertex [3];

		int Normal [3];

		int TexCoord [3];
	};

	//---------------------------------------------------------------------------------------------

	struct OBJGroup
	{
		//--------------- Material Properties and Name ( DELETED IN OBJMODEL CLASS ) --------------

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

		__declspec(align(16)) vector <Vector2D> TexCoords;

		//---------------- List of Face Groups ( Sets of Faces with Same Material ) ---------------

		vector <OBJGroup *> Groups;

		//------------------------------ List of Material Properties ------------------------------

		vector <MTLMaterial *> Materials;

		//-------------------------------- List of Raster Textures --------------------------------

		vector <MTLTextureData *> Textures;

		//-------------------------------------- Destructor ---------------------------------------
		
		~OBJModel ( void );

		//----------------------------------- Useful Functions ------------------------------------
		
		Vector3D GetMinimum ( void ) const;
		
		Vector3D GetMaximum ( void ) const;
	};
}

#endif