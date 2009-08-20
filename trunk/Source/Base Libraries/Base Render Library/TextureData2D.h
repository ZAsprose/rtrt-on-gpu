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

#pragma once

#ifndef _TEXTURE_DATA_2D_

#define _TEXTURE_DATA_2D_

#include <GLee.h>

#include <GL/glfw.h>

namespace Render
{
	class TextureData2D
	{
		private:

			//----------------------------------- Pixels Array ------------------------------------

			float * Pixels;

			//----------------------------------- Texture Size ------------------------------------
			
			unsigned Width;
			
			unsigned Height;

			//--------------------------------- Pixel Components ----------------------------------

			unsigned Components;
		
		public:

			//----------------------------- Constructor and Destructor ----------------------------
		
			TextureData2D ( unsigned, unsigned, unsigned = 3 );
			
			~TextureData2D ( void );

			//---------------------------------- Type Conversion ----------------------------------

			operator float * ( void ) { return Pixels; }
			
			operator const float * ( void ) const { return Pixels; }

			//------------------------------------ Data Access ------------------------------------

			template <class Type> Type& Pixel ( unsigned );

			template <class Type> Type& Pixel ( unsigned, unsigned );

			//------------------------------------ Data Format ------------------------------------
			
			unsigned GetPixelFormat ( void );

			unsigned GetInternalFormat ( void );

			unsigned GetType ( void ) { return GL_FLOAT; }
			
			unsigned GetWidth ( void ) { return Width; }

			unsigned GetHeight ( void ) { return Height; }

			unsigned GetComponents ( void ) { return Components; }

			//------------------------------------ Data Upload ------------------------------------

			void Upload ( unsigned target = GL_TEXTURE_2D );

			//------------------------- Loading Texture Data from TGA File ------------------------

			static TextureData2D * FromTGA ( const char * file );
	};

	//------------------------------------- Template Functions ------------------------------------
	
	template <class Type> Type& TextureData2D :: Pixel ( unsigned x )
	{
		return ( Type& ) Pixels [ x * Components ];
	}

	template <class Type> Type& TextureData2D :: Pixel ( unsigned x, unsigned y )
	{
		return ( Type& ) Pixels [ ( x + y * Width ) * Components ];
	}
}

#endif