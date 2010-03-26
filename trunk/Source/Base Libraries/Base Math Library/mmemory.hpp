/*
-----------------------------------------------------------------------------
|                   B A S E     M A T H     L I B R A R Y                   |
-----------------------------------------------------------------------------

Andrey Morozov ( morozov.andrey.vmk @ gmail.com )
Copyright (c) 2009 - 2010
Denis Bogolepov ( bogdencmc @ inbox.ru )

This library is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __mmemory_hpp__
#define __mmemory_hpp__

#include <stdlib.h>
#include <malloc.h>
#include <cstddef>

template <typename T>
T* new_memory(std::size_t count, int aligned = 16)
{
	if (aligned == 0)
	{
		return new T[count];
	}
	else
	{
		#ifdef WIN32
			return reinterpret_cast<T*> (_aligned_malloc(sizeof (T) * count, aligned));
		#else
			return reinterpret_cast<T*> (memalign(aligned, sizeof (T) * count)); // posix
		#endif
	}
}

void del_memory(void* p);

#endif
