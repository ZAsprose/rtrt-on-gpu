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
			
			const char * Name;

			//------------------------------------ Texture Init -----------------------------------

			void Init ( unsigned int, TextureData2D *, unsigned int, const char * );
		
		public:

			//----------------------------------- Texture Data ------------------------------------

			TextureData2D * Data;

			//---------------------------------- Texture Params -----------------------------------

			FilterMode FilterMode;

			WrapMode WrapMode;

			//----------------------------- Constructor and Destructor ----------------------------

			Texture2D ( unsigned int = 0, const char * = "Texture" );
						
			Texture2D ( TextureData2D *, unsigned int = 0, const char * = "Texture" );

			Texture2D ( unsigned int, unsigned int = 0, const char * = "Texture" );
						
			Texture2D ( unsigned int, TextureData2D *,
				        unsigned int = 0, const char * = "Texture" );

			~Texture2D ( void );

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