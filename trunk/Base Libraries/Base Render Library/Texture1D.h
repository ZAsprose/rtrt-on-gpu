#pragma once

#ifndef _TEXTURE1D_

#define _TEXTURE1D_

#include "TextureData1D.h"

#include "FilterMode.h"

#include "WrapMode.h"

namespace RenderTools
{
	class Texture1D
	{
		private:

			//----------------------------------- Texture Handle ----------------------------------

			unsigned int Handle;

			//---------------------------------- Texture Settings ---------------------------------

			unsigned int Target;

			unsigned int Unit;
			
			const char * Name;

			//------------------------------------ Texture Init -----------------------------------

			void Init ( int, TextureData1D *, int, const char * );
		
		public:

			//----------------------------------- Texture Data ------------------------------------

			TextureData1D * Data;

			//---------------------------------- Texture Params -----------------------------------

			FilterMode FilterMode;

			WrapMode WrapMode;

			//----------------------------- Constructor and Destructor ----------------------------

			Texture1D ( int = 0, const char * = "Texture" );
						
			Texture1D ( TextureData1D *, int = 0, const char * = "Texture" );

			~Texture1D ( void );

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