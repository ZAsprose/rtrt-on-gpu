#pragma once

#ifndef _TEXTURE_2D_

#define _TEXTURE_2D_

#include "TextureData2D.h"

#include "FilterMode.h"

#include "WrapMode.h"

namespace RenderTools
{
	class Texture2D
	{
		private:

			//---------------------------------- Texture Handle -----------------------------------

			unsigned int Handle;

			//--------------------------------- OpenGL Identifier ---------------------------------
			
			unsigned int Target;

			//---------------------------------- Texture Settings ---------------------------------

			unsigned int Unit;

			//------------------------------------ Texture Init -----------------------------------

			void Init ( unsigned int, TextureData2D *, unsigned int );
		
		public:

			//----------------------------------- Texture Data ------------------------------------

			TextureData2D * Data;

			//---------------------------------- Texture Params -----------------------------------

			FilterMode FilterMode;

			WrapMode WrapMode;

			//----------------------------- Constructor and Destructor ----------------------------

			Texture2D ( unsigned int = 0, unsigned int = GL_TEXTURE_2D );

			Texture2D ( TextureData2D *, unsigned int = 0, unsigned int = GL_TEXTURE_2D );

			~Texture2D ( void );

			//--------------------------------- Texture Management --------------------------------
			
			void Setup ( void );

			void Bind ( void );

			void Unbind ( void );

			//------------------------------------ Texture Info -----------------------------------
			
			unsigned int GetHandle ( void ) const { return Handle; }

			unsigned int GetTarget ( void ) const { return Target; }

			unsigned int GetUnit ( void ) const { return Unit; }
	};
}

#endif