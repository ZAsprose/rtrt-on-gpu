#pragma once

#include <GLee.h>

#ifndef _MAP_MODE_

#define _MAP_MODE_

namespace RenderTools
{
	class WrapMode
	{
		public:

			//----------------------------------- Wrap Settings -----------------------------------
			
			int WrapS;
		
			int WrapT;
			
			int WrapR;

			//------------------------------------ Constructor ------------------------------------
	
			WrapMode ( int = GL_REPEAT, int = GL_REPEAT, int = GL_REPEAT );

			//------------------------------------- Setup Mode ------------------------------------
			
			void Setup ( int );
	};
}

#endif
