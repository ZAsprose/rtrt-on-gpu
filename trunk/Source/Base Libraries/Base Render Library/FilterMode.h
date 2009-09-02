/*
   Base Render Library   
   Copyright (C) 2009  Denis Bogolepov ( bogdencmc@inbox.ru )

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see http://www.gnu.org/licenses.
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
		
			unsigned Minification;
			
			unsigned Magnification;

			//------------------------------------ Constructor ------------------------------------		

			FilterMode ( unsigned = GL_NEAREST, unsigned = GL_NEAREST );

			//-------------------------------- Setting Filter Mode --------------------------------
			
			void Setup ( unsigned );
	};
}

#endif
