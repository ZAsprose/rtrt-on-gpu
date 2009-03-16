#include "TextureData3D.h"

namespace Render
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	TextureData3D :: TextureData3D ( int width, int height, int depth, int components )
	{
		Width = width;

		Height = height;

		Depth = depth;

		Components = components;

		Pixels = new float [ Width * Height * Depth * Components ];

		memset ( Pixels, 0, Width * Height * Depth * Components );
	}
			
	TextureData3D :: ~TextureData3D ( void )
	{
		delete [] Pixels;
	}

	//---------------------------------------- Data Upload ----------------------------------------

	void TextureData3D :: Upload ( int target )
	{
		glTexImage3D ( target,
			           0,
					   GetInternalFormat ( ),
					   Width,
					   Height,
					   Depth,
					   0,
					   GetPixelFormat ( ),
					   GL_FLOAT,
					   Pixels );
	}
	
	//--------------------------------------- Texture Format --------------------------------------
	
	int TextureData3D :: GetPixelFormat ( void )
	{
		switch ( Components )
		{
			case 1:
				return GL_ALPHA;

			case 3:
				return GL_RGB;
					
			case 4:
				return GL_RGBA;
					
			default:
				return -1;
		}
	}

	int TextureData3D :: GetInternalFormat ( void )
	{
		switch ( Components )
		{
			case 1:
				return GL_ALPHA32F_ARB;

			case 3:
				return GL_RGB32F_ARB;
			
			case 4:
				return GL_RGBA32F_ARB;
			
			default:
				return -1;
		}
	}
}