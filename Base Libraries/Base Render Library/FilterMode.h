#pragma once

#include <GLee.h>

#ifndef _FILTER_MODE_

#define _FILTER_MODE_

namespace RenderTools
{
	class FilterMode
	{
		public:

			//------------------------------------ Constructor ------------------------------------
		
			int Minification;
			
			int Magnification;

			//------------------------------------ Constructor ------------------------------------		

			FilterMode ( int = GL_NEAREST, int = GL_NEAREST );

			//------------------------------------- Setup Mode ------------------------------------
			
			void Setup ( int );
	};
}

#endif
