/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#pragma once

#ifndef _OBJ_MODEL_

#define _OBJ_MODEL_

#include "Triangle.h"

#include "Texture2D.h"

#include <vector>

using namespace std;

#define LENGTH 255

namespace Raytracing
{
	//----------------------------------- MTL Material Texture ------------------------------------

	struct MTLTexture
	{
		//----------------------------- File Name and OpenGL Texture ------------------------------

		char Name [LENGTH];

		Texture2D * Texture;

		//------------------------------- Constructor and Destructor ------------------------------
		
		MTLTexture ( char [], Texture2D * = NULL );

		~MTLTexture ( void );
	};

	//---------------------------------- MTL Material Properties ----------------------------------

	struct MTLMaterial
	{
		//------------------------------- Material Name in MTL File -------------------------------

		char Name [LENGTH];

		//------------------------------- Phong Reflectance Coeffs --------------------------------

		Vector3D Ambient;

		Vector3D Diffuse;

		Vector3D Specular;

		float Shininess;

		//------------------------ Transmission Filter and Optical Density ------------------------

		Vector3D Transmission;

		float Density;

		//----------------------------------- Material Dissolve -----------------------------------
			
		float Dissolve;

		//---------------------------- Illumination Model for Material ----------------------------

		int Model;

		//------------------------------------ Raster Texture -------------------------------------

		Texture2D * Texture;

		//-------------------------------------- Constructor --------------------------------------
		
		MTLMaterial ( char [],
			          const Vector3D& = Vector3D ( 0.2F, 0.2F, 0.2F ),
					  const Vector3D& = Vector3D ( 0.8F, 0.8F, 0.8F ),
					  const Vector3D& = Vector3D ( 0.8F, 0.8F, 0.8F ),
					  float = 32.0F,
					  const Vector3D& = Vector3D :: Zero,
					  float = 1.5F,
					  float = 1.0F,
					  int = 2,
					  Texture2D * = NULL );
	};

	//----------------------------------- OBJ Model Face Element ----------------------------------

	struct OBJFace
	{
		int Vertex [3];

		int Normal [3];

		int Texture [3];
	};

	//-------------------- OBJ Model Group ( Set of faces with one material ) ---------------------

	struct OBJGroup
	{
		//---------------------------------- Material Properties ----------------------------------

		MTLMaterial * Material;

		//-------------------------------------- Group Faces --------------------------------------
		
		vector < OBJFace > Faces;

		//------------------------------- Constructor and Destructor ------------------------------
		
		OBJGroup ( MTLMaterial * = NULL );

		~OBJGroup ( void );
	};

	//----------------------------------------- OBJ Model -----------------------------------------

	struct OBJModel
	{
		//-------------------------------------- Data Arrays --------------------------------------

		vector < Vector3D > Vertices;

		vector < Vector3D > Normals;

		vector < Vector2D > TexCoords;

		//------------------------------------ Groups of Faces ------------------------------------

		vector < OBJGroup * > Groups;

		//--------------------------- Material Properties from MTL File ---------------------------

		vector < MTLMaterial * > Materials;

		//----------------------------- Raster Textures from MTL File -----------------------------

		vector < MTLTexture * > Textures;

		//-------------------------------------- Destructor ---------------------------------------
		
		~OBJModel ( void );

		//----------------------------------- Support Functions -----------------------------------
		
		Vector3D GetMinimum ( void ) const;
		
		Vector3D GetMaximum ( void ) const;
	};
}

#endif