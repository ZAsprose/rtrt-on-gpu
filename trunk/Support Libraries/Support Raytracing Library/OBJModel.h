/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#pragma once

#ifndef _MODEL_OBJ_

#define _MODEL_OBJ_

#include "Triangle.h"

namespace Raytracing
{
	struct OBJFace
	{
		int Vertex[3];

		int Normal[3];

		int Texture[3];
	};

	class OBJModel
	{
		public:

			//------------------------------ Number of Data Elements ------------------------------

			int VertexNumber;
			
			int NormalNumber;
			
			int TextureNumber;
			
			int FaceNumber;

			//------------------------------------ Data Arrays ------------------------------------

			Vector3D * Vertices;

			Vector3D * Normals;

			Vector2D * Textures;

			OBJFace * Faces;

			//----------------------------- Constructor and Destructor ----------------------------

			OBJModel ( void );

			~OBJModel ( void );
	};
}

#endif