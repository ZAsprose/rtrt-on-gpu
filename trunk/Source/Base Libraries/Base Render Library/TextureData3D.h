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

			//---------------------------------- Type Conversion ----------------------------------

			operator float * ( void ) { return Pixels; }
			
			operator const float * ( void ) const { return Pixels; }

			//------------------------------------ Data Access ------------------------------------

			template <class Type> Type& Pixel ( unsigned );

			template <class Type> Type& Pixel ( unsigned, unsigned, unsigned );

			//------------------------------------ Data Format ------------------------------------
			
			unsigned GetPixelFormat ( void );

			unsigned GetInternalFormat ( void );

			unsigned GetType ( void ) { return GL_FLOAT; }
			
			unsigned GetWidth ( void ) { return Width; }

			unsigned GetHeight ( void ) { return Height; }

			unsigned GetDepth ( void ) { return Depth; }

			unsigned GetComponents ( void ) { return Components; }

			//------------------------------------ Data Upload ------------------------------------

			void Upload ( unsigned target = GL_TEXTURE_3D );
	};

	//------------------------------------- Template Functions ------------------------------------
	
	template <class Type> Type& TextureData3D :: Pixel ( unsigned x )
	{
		return ( Type& ) Pixels [ x * Components ];
	}

	template <class Type> Type& TextureData3D :: Pixel ( unsigned x, unsigned y, unsigned z )
	{
		return ( Type& ) Pixels [ ( x + y * Width + z * Width * Height ) * Components ];
	}
}

#endif