/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#pragma once

#ifndef _TEXTURE_3D_

#define _TEXTURE_3D_

#include "TextureData3D.h"

#include "FilterMode.h"

#include "WrapMode.h"

namespace Render
{
	class Texture3D
	{
		private:

			//---------------------------------- Texture Handle -----------------------------------

			unsigned Handle;

			//--------------------------------- OpenGL Identifier ---------------------------------
			
			unsigned Target;

			//------------------------------------ Texture Init -----------------------------------

			void Init ( unsigned, TextureData3D *, unsigned );
		
		public:

			//----------------------------------- Texture Unit ------------------------------------

			unsigned Unit;

			//----------------------------------- Texture Data ------------------------------------

			TextureData3D * Data;

			//---------------------------------- Texture Params -----------------------------------

			FilterMode FilterMode;

			WrapMode WrapMode;

			//----------------------------- Constructor and Destructor ----------------------------

			Texture3D ( unsigned = 0 );
						
			Texture3D ( TextureData3D *, unsigned = 0 );

			~Texture3D ( void );

			//--------------------------------- Texture Management --------------------------------
			
			void Setup ( void );

			void Update ( void );

			void Bind ( void );

			void Unbind ( void );

			//------------------------------------ Texture Info -----------------------------------
			
			unsigned int GetHandle ( void ) const { return Handle; }

			unsigned int GetTarget ( void ) const { return Target; }

			unsigned int GetUnit ( void ) const { return Unit; }
	};
}

#endif