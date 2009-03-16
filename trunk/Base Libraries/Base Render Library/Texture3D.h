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

			unsigned int Handle;

			//--------------------------------- OpenGL Identifier ---------------------------------
			
			unsigned int Target;

			//---------------------------------- Texture Settings ---------------------------------

			unsigned int Unit;
			
			const char * Name;

			//------------------------------------ Texture Init -----------------------------------

			void Init ( unsigned int, TextureData3D *, unsigned int, const char * );
		
		public:

			//----------------------------------- Texture Data ------------------------------------

			TextureData3D * Data;

			//---------------------------------- Texture Params -----------------------------------

			FilterMode FilterMode;

			WrapMode WrapMode;

			//----------------------------- Constructor and Destructor ----------------------------

			Texture3D ( unsigned int = 0, const char * = "Texture" );
						
			Texture3D ( TextureData3D *, unsigned int = 0, const char * = "Texture" );

			~Texture3D ( void );

			//--------------------------------- Texture Management --------------------------------
			
			void Setup ( void );

			void Bind ( void );

			void Unbind ( void );

			//------------------------------------ Texture Info -----------------------------------
			
			unsigned int GetHandle ( void ) const { return Handle; }

			unsigned int GetTarget ( void ) const { return Target; }

			unsigned int GetUnit ( void ) const { return Unit; }

			const char * GetName ( void ) const { return Name; }
	};
}

#endif