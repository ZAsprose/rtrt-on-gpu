#pragma once

#ifndef _TEXTURE_1D_

#define _TEXTURE_1D_

#include "TextureData1D.h"

#include "FilterMode.h"

#include "WrapMode.h"

namespace RenderTools
{
	class Texture1D
	{
		private:

			//---------------------------------- Texture Handle -----------------------------------

			unsigned int Handle;

			//--------------------------------- OpenGL Identifier ---------------------------------
			
			unsigned int Target;

			//---------------------------------- Texture Settings ---------------------------------

			unsigned int Unit;

			//------------------------------------ Texture Init -----------------------------------

			void Init ( unsigned int, TextureData1D *, unsigned int );
		
		public:

			//----------------------------------- Texture Data ------------------------------------

			TextureData1D * Data;

			//---------------------------------- Texture Params -----------------------------------

			FilterMode FilterMode;

			WrapMode WrapMode;

			//----------------------------- Constructor and Destructor ----------------------------

			Texture1D ( unsigned int = 0 );
						
			Texture1D ( TextureData1D *, unsigned int = 0 );

			~Texture1D ( void );

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