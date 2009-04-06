/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "OBJModel.h"

namespace Raytracing
{
	//--------------------------------- Constructor and Destructor --------------------------------

	OBJModel :: OBJModel ( void )
	{
		VertexNumber = 0;

		NormalNumber = 0;

		TextureNumber = 0;

		FaceNumber = 0;
		
		Vertices = NULL;

		Normals = NULL;

		Textures = NULL;

		Faces = NULL;
	}

	OBJModel :: ~OBJModel ( void )
	{
		delete [] Vertices;

		delete [] Normals;

		delete [] Textures;

		delete [] Faces;
	}
}