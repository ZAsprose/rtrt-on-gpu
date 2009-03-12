#pragma once

#ifndef _TEXTURE2D_

#define _TEXTURE2D_

#include "TextureData2D.h"

#include "FilterMode.h"

#include "WrapMode.h"

namespace RenderTools
{
	class Texture2D
	{
		private:

			//----------------------------------- Texture Handle ----------------------------------

			unsigned int Handle;

			//---------------------------------- Texture Settings ---------------------------------

			unsigned int Target;

			unsigned int Unit;
			
			const char * Name;

			//------------------------------------ Texture Init -----------------------------------

			void Init ( int, TextureData2D *, int, const char * );
		
		public:

			//----------------------------------- Texture Data ------------------------------------

			TextureData2D * Data;

			//---------------------------------- Texture Params -----------------------------------

			FilterMode FilterMode;

			WrapMode WrapMode;

			//----------------------------- Constructor and Destructor ----------------------------

			Texture2D ( int = 0, const char * = "Texture" );
						
			Texture2D ( TextureData2D *, int = 0, const char * = "Texture" );

			Texture2D ( int, int = 0, const char * = "Texture" );
						
			Texture2D ( int, TextureData2D *, int = 0, const char * = "Texture" );

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