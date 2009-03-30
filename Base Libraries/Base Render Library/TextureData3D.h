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
			
			int Width;
			
			int Height;

			int Depth;

			//--------------------------------- Pixel Components ----------------------------------

			int Components;
		
		public:

			//----------------------------- Constructor and Destructor ----------------------------
		
			TextureData3D ( int, int, int, int = 3 );
			
			~TextureData3D ( void );

			//---------------------------------- Type Conversion ----------------------------------

			operator float * ( void ) { return Pixels; }
			
			operator const float * ( void ) const { return Pixels; }

			//------------------------------------ Data Access ------------------------------------

			template < class Type > Type& Pixel ( int );

			template < class Type > Type& Pixel ( int, int, int );

			//------------------------------------ Data Format ------------------------------------
			
			int GetPixelFormat ( void );

			int GetInternalFormat ( void );

			int GetType ( void ) { return GL_FLOAT; }
			
			int GetWidth ( void ) { return Width; }

			int GetHeight ( void ) { return Height; }

			int GetDepth ( void ) { return Depth; }

			int GetComponents ( void ) { return Components; }

			//------------------------------------ Data Upload ------------------------------------

			void Upload ( int target = GL_TEXTURE_3D );
	};

	//------------------------------------- Template Functions ------------------------------------
	
	template < class Type > Type& TextureData3D :: Pixel ( int x )
	{
		return ( Type& ) Pixels [ x * Components ];
	}

	template < class Type > Type& TextureData3D :: Pixel ( int x, int y, int z )
	{
		return ( Type& ) Pixels [ ( x + y * Width + z * Width * Height ) * Components ];
	}
}

#endif