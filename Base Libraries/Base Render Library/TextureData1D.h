#pragma once

#ifndef _TEXTURE_DATA_1D_

#define _TEXTURE_DATA_1D_

#include <GLee.h>

namespace Render
{
	class TextureData1D
	{
		private:

			//----------------------------------- Pixels Array ------------------------------------

			float * Pixels;

			//----------------------------------- Texture Size ------------------------------------
			
			int Width;

			//--------------------------------- Pixel Components ----------------------------------

			int Components;
		
		public:

			//----------------------------- Constructor and Destructor ----------------------------
		
			TextureData1D ( int, int = 3 );
			
			~TextureData1D ( void );

			//------------------------------------ Data Access ------------------------------------

			operator float * ( void ) { return Pixels; }
			
			operator const float * ( void ) const { return Pixels; }

			template < class Type > Type& Pixel ( int );

			//------------------------------------ Data Format ------------------------------------
			
			int GetPixelFormat ( void );

			int GetInternalFormat ( void );

			int GetType ( void ) { return GL_FLOAT; }
			
			int GetWidth ( void ) { return Width; }

			int GetComponents ( void ) { return Components; }

			//------------------------------------ Data Upload ------------------------------------

			void Upload ( int target = GL_TEXTURE_1D );
	};

	//------------------------------------- Template Functions ------------------------------------
	
	template < class Type > Type& TextureData1D :: Pixel ( int x )
	{
		return ( Type& ) Pixels [ x * Components ];
	}
}

#endif