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

#include <stdlib.h>
#include <malloc.h>
#include <cstddef>

void del_memory(void* p)
{
	#ifdef WIN32
		_aligned_free(p);
	#else
		free(p);
	#endif
}

