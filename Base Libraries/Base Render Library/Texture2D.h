#pragma once

#include <Vector3D.h>

#include <GLee.h>

#include <GL/glfw.h>

using namespace Math;

namespace RenderTools
{
	//====================================== Data Proxy Class =====================================

	template <class Type> class TextureData2D;

	template <class Type> class DataProxy2D
	{
		friend class TextureData2D<Type>;

		private:
			
			//------------------------------------- Pixels Row ------------------------------------
			
			float * Pixels;

			//---------------------------------- Pixel Components ---------------------------------

			int Components;

			//------------------------------------ Constructor ------------------------------------

			DataProxy2D ( float* pixels, int components )
			{
				Pixels = pixels;

				Components = components;
			}

		public:

			//-------------------------------------- Indexer --------------------------------------
			
			Type& operator [] ( int index )
			{
				return ( Type& ) Pixels [ index * Components ];
			}
	};

	//===================================== Texture Data Class ====================================

	template <class Type> class TextureData2D
	{
		private:

			//----------------------------------- Texture Data ------------------------------------

			float * Pixels;

			//----------------------------------- Texture Size ------------------------------------
			
			int Width;
			
			int Height;

			//--------------------------------- Pixel Components ----------------------------------

			int Components;
		
		public:

			//----------------------------- Constructor and Destructor ----------------------------
		
			TextureData2D ( int width = 512, int height = 512, int components = 3 )
			{
				Width = width;

				Height = height;

				Components = components;

				Pixels = new float [ Width * Height * Components ];

				memset ( Pixels, 0, Width * Height * Components );
			}
			
			~TextureData2D ( void )
			{
				if ( Pixels != NULL )
				{
					delete [] Pixels;
				}
			}

			//-------------------------------- Auxiliary Operators --------------------------------

			operator float * ( )
			{
				return Pixels;
			}
			
			operator const float * ( ) const
			{
				return Pixels;
			}

			DataProxy2D<Type> operator [] ( int index )
			{
				return DataProxy2D<Type> ( &Pixels[index * Width * Components], Components );
			} 

			//------------------------------------ Data Upload ------------------------------------

			void Upload ( int target = GL_TEXTURE_2D )
			{
				glTexImage2D ( target,
					           0,
							   GetInternalFormat ( ),
							   Width,
							   Height,
							   0,
							   GetPixelFormat ( ),
							   GL_FLOAT,
							   Pixels);
			}

			//----------------------------------- Texture Format ----------------------------------
			
			int GetPixelFormat ( void )
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

			int GetInternalFormat ( void )
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
	};
}