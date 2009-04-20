/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#pragma once

#include <GLee.h>

#ifndef _MAP_MODE_

#define _MAP_MODE_

namespace Render
{
	class WrapMode
	{
		public:

			//---------------------------------- Public Constants ---------------------------------

			static const WrapMode Clamp;

			static const WrapMode Repeat;

			//----------------------------------- Wrap Settings -----------------------------------
			
			int WrapS;
		
			int WrapT;
			
			int WrapR;

			//------------------------------------ Constructor ------------------------------------
	
			WrapMode ( int = GL_CLAMP, int = GL_CLAMP, int = GL_CLAMP );

			//------------------------------------- Setup Mode ------------------------------------
			
			void Setup ( int );
	};
}

#endif
