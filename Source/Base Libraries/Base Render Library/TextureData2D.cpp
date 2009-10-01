/*
   B A S E   R E N D E R   L I B R A R Y

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

#include "TextureData2D.h"

namespace Render
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	TextureData2D :: TextureData2D ( unsigned width, unsigned height, unsigned components )
	{
		Width = width;

		Height = height;

		Components = components;

		Pixels = new float [ Width * Height * Components ];

		memset ( Pixels, 0, Width * Height * Components );
	}
			
	TextureData2D :: ~TextureData2D ( void )
	{
		delete [] Pixels;
	}

	//--------------------------------- Uploading Data to OpenGL ----------------------------------

	void TextureData2D :: Upload ( unsigned target )
	{
		glTexImage2D ( target,
			           0,
					   GetInternalFormat ( ),
					   Width,
					   Height,
					   0,
					   GetPixelFormat ( ),
					   GL_FLOAT,
					   Pixels );
	}
	
	//------------------------------------ Getting Data Format ------------------------------------
	
	unsigned TextureData2D :: GetPixelFormat ( void ) const
	{
		switch ( Components )
		{
			case 1: return GL_ALPHA;

			case 3: return GL_RGB;
					
			case 4: return GL_RGBA;
					
			default: return 0;
		}
	}

	unsigned TextureData2D :: GetInternalFormat ( void ) const
	{
		switch ( Components )
		{
			case 1: return GL_ALPHA32F_ARB;

			case 3: return GL_RGB32F_ARB;
			
			case 4: return GL_RGBA32F_ARB;
			
			default: return 0;
		}
	}

	//----------------------------- Loading Texture Data from TGA File ----------------------------

	TextureData2D * TextureData2D :: FromTGA ( const char * file )
	{
		TextureData2D * data = NULL;
		
		GLFWimage * image = new GLFWimage ( );

		if ( glfwReadImage ( file, image, 0 ) )
		{
			data = new TextureData2D ( image->Width, image->Height, image->BytesPerPixel );

			for ( int index = 0; index < data->Width * data->Height * data->Components; index++ )
			{
				( *data ) [index] = image->Data [index] / 255.0F;
			}
		}

		glfwFreeImage ( image );

		return data;
	}
}