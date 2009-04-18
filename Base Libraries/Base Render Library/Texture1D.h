/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#pragma once

#ifndef _TEXTURE_1D_

#define _TEXTURE_1D_

#include "TextureData1D.h"

#include "FilterMode.h"

#include "WrapMode.h"

namespace Render
{
	class Texture1D
	{
		private:

			//---------------------------------- Texture Handle -----------------------------------

			unsigned Handle;

			//--------------------------------- OpenGL Identifier ---------------------------------
			
			unsigned Target;

			//----------------------------------- Texture Init ------------------------------------

			void Init ( unsigned, TextureData1D *, unsigned );
		
		public:

			//----------------------------------- Texture Unit ------------------------------------

			unsigned Unit;

			//----------------------------------- Texture Data ------------------------------------

			TextureData1D * Data;

			//---------------------------------- Texture Params -----------------------------------

			FilterMode FilterMode;

			WrapMode WrapMode;

			//----------------------------- Constructor and Destructor ----------------------------

			Texture1D ( unsigned = 0 );
						
			Texture1D ( TextureData1D *, unsigned = 0 );

			~Texture1D ( void );

			//--------------------------------- Texture Management --------------------------------
			
			void Setup ( void );

			void Bind ( void );

			void Unbind ( void );

			//------------------------------------ Texture Info -----------------------------------
			
			unsigned GetHandle ( void ) const { return Handle; }

			unsigned GetTarget ( void ) const { return Target; }

			unsigned GetUnit ( void ) const { return Unit; }
	};
}

#endif