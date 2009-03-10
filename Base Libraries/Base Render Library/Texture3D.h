#pragma once

#ifndef _TEXTURE3D_

#define _TEXTURE3D_

#include "TextureData3D.h"

namespace RenderTools
{
	class Texture3D
	{
		private:

			//----------------------------------- Texture Handle ----------------------------------

			unsigned int Handle;

			//---------------------------------- Texture Settings ---------------------------------

			unsigned int Target;

			unsigned int Unit;
			
			const char * Name;

			//------------------------------------ Texture Init -----------------------------------

			void Init ( int, TextureData3D *, int, const char * );
		
		public:

			//----------------------------------- Texture Data ------------------------------------

			TextureData3D * Data;

			//----------------------------- Constructor and Destructor ----------------------------

			Texture3D ( int = 0, const char * = "Texture" );
						
			Texture3D ( TextureData3D *, int = 0, const char * = "Texture" );

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