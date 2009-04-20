/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#pragma once

#ifndef _TEXTURE_2D_

#define _TEXTURE_2D_

#include "TextureData2D.h"

#include "FilterMode.h"

#include "WrapMode.h"

namespace Render
{
	class Texture2D
	{
		private:

			//---------------------------------- Texture Handle -----------------------------------

			unsigned Handle;

			//--------------------------------- OpenGL Identifier ---------------------------------
			
			unsigned Target;

			//----------------------------------- Texture Init ------------------------------------

			void Init ( unsigned, TextureData2D *, unsigned );
		
		public:

			//----------------------------------- Texture Unit ------------------------------------

			unsigned Unit;

			//----------------------------------- Texture Data ------------------------------------

			TextureData2D * Data;

			//---------------------------------- Texture Params -----------------------------------

			FilterMode FilterMode;

			WrapMode WrapMode;

			//----------------------------- Constructor and Destructor ----------------------------

			Texture2D ( unsigned = 0, unsigned = GL_TEXTURE_2D );

			Texture2D ( TextureData2D *, unsigned = 0, unsigned = GL_TEXTURE_2D );

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