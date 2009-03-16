#include "TextureData1D.h"

namespace Render
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	TextureData1D :: TextureData1D ( int width, int components )
	{
		Width = width;

		Components = components;

		Pixels = new float [ Width * Components ];

		memset ( Pixels, 0, Width * Components );
	}
			
	TextureData1D :: ~TextureData1D ( void )
	{
		delete [] Pixels;
	}

	//---------------------------------------- Data Upload ----------------------------------------

	void TextureData1D :: Upload ( int target )
	{
		glTexImage1D ( target,
			           0,
					   GetInternalFormat ( ),
					   Width,
					   0,
					   GetPixelFormat ( ),
					   GetType ( ),
					   Pixels );
	}
	
	//--------------------------------------- Texture Format --------------------------------------
	
	int TextureData1D :: GetPixelFormat ( void )
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

	int TextureData1D :: GetInternalFormat ( void )
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