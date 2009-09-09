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

#ifndef _TEXTURE_DATA_3D_

#define _TEXTURE_DATA_3D_

#include <GLee.h>

namespace Render
{
	class TextureData3D
	{
		private:

			//----------------------------------- Pixels Array ------------------------------------

			float * Pixels;

			//----------------------------------- Texture Size ------------------------------------
			
			unsigned Width;
			
			unsigned Height;

			unsigned Depth;

			//--------------------------------- Pixel Components ----------------------------------

			unsigned Components;
		
		public:

			//----------------------------- Constructor and Destructor ----------------------------
		
			TextureData3D ( unsigned, unsigned, unsigned, unsigned = 3 );
			
			~TextureData3D ( void );

			//---------------------- Converting Data to Arrays Used in OpenGL ---------------------

			operator float * ( void ) { return Pixels; }
			
			operator const float * ( void ) const { return Pixels; }

			//----------------------------- Accessing to Pixels Array -----------------------------

			template <class TYPE> TYPE& Pixel ( unsigned );

			template <class TYPE> TYPE& Pixel ( unsigned, unsigned, unsigned );

			//----------------------------- Uploading Data to OpenGL ------------------------------

			void Upload ( unsigned target = GL_TEXTURE_3D );

			//-------------------------------- Getting Data Format --------------------------------
			
			unsigned GetPixelFormat ( void ) const;

			unsigned GetInternalFormat ( void ) const;

			unsigned GetType ( void ) const { return GL_FLOAT; }
			
			unsigned GetWidth ( void ) const { return Width; }

			unsigned GetHeight ( void ) const { return Height; }

			unsigned GetDepth ( void ) const { return Depth; }

			unsigned GetComponents ( void ) const { return Components; }
	};

	//---------------------------- Implementation of Template Functions ---------------------------
	
	template <class TYPE> TYPE& TextureData3D :: Pixel ( unsigned x )
	{
		return ( TYPE& ) Pixels [ x * Components ];
	}

	template <class TYPE> TYPE& TextureData3D :: Pixel ( unsigned x, unsigned y, unsigned z )
	{
		return ( TYPE& ) Pixels [ ( x + y * Width + z * Width * Height ) * Components ];
	}
}

#endif