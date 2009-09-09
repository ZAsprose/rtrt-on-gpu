/*
   Base Render Library   
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

#include "TextureData3D.h"

namespace Render
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	TextureData3D :: TextureData3D ( unsigned width, unsigned height,
		                             unsigned depth, unsigned components )
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

	//--------------------------------- Uploading Data to OpenGL ----------------------------------

	void TextureData3D :: Upload ( unsigned target )
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
	
	//------------------------------------ Getting Data Format ------------------------------------
	
	unsigned TextureData3D :: GetPixelFormat ( void ) const
	{
		switch ( Components )
		{
			case 1: return GL_ALPHA;

			case 3: return GL_RGB;
					
			case 4: return GL_RGBA;
					
			default: return 0;
		}
	}

	unsigned TextureData3D :: GetInternalFormat ( void ) const
	{
		switch ( Components )
		{
			case 1: return GL_ALPHA32F_ARB;

			case 3: return GL_RGB32F_ARB;
			
			case 4: return GL_RGBA32F_ARB;
			
			default: return 0;
		}
	}
}