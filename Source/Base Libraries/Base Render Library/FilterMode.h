/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#pragma once

#include <GLee.h>

#ifndef _FILTER_MODE_

#define _FILTER_MODE_

namespace Render
{
	class FilterMode
	{
		public:

			//---------------------------------- Public Constants ---------------------------------

			static const FilterMode Nearest;

			static const FilterMode Linear;

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
