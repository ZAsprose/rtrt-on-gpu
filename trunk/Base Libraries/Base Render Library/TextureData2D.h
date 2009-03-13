#pragma once

#ifndef _TEXTURE_DATA_2D_

#define _TEXTURE_DATA_2D_

#include <GLee.h>

namespace RenderTools
{
	class TextureData2D
	{
		private:

			//----------------------------------- Pixels Array ------------------------------------

			float * Pixels;

			//----------------------------------- Texture Size ------------------------------------
			
			int Width;
			
			int Height;

			//--------------------------------- Pixel Components ----------------------------------

			int Components;
		
		public:

			//----------------------------- Constructor and Destructor ----------------------------
		
			TextureData2D ( int, int, int = 3 );
			
			~TextureData2D ( void );

			//------------------------------------ Data Access ------------------------------------

			operator float * ( void ) { return Pixels; }
			
			operator const float * ( void ) const { return Pixels; }

			template < class Type > Type& Pixel ( int, int );

			//------------------------------------ Data Format ------------------------------------
			
			int GetPixelFormat ( void );

			int GetInternalFormat ( void );

			int GetType ( void ) { return GL_FLOAT; }
			
			int GetWidth ( void ) { return Width; }

			int GetHeight ( void ) { return Height; }

			int GetComponents ( void ) { return Components; }

			//------------------------------------ Data Upload ------------------------------------

			void Upload ( int target = GL_TEXTURE_3D );
	};

	//------------------------------------- Template Functions ------------------------------------
	
	template < class Type > Type& TextureData2D :: Pixel ( int x, int y )
	{
		return ( Type& ) Pixels [ ( x + y * Width ) * Components ];
	}
}

#endif