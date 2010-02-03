/**
 *** @file mmemory.hpp
 ***
 *** @date 2010
 ***
 *** @version 1.0
 ***
 *** @autor Andrey Morozov
 ***
 *** @email morozov.andrey.vmk@gmail.com
 ***
 **/

#ifndef __mmemory_hpp__
#define __mmemory_hpp__

#include <stdlib.h>
#include <malloc.h>

template <typename T>
inline T* new_memory(std::size_t count, int aligned = 16)
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

inline void del_memory(void* p)
{
	#ifdef WIN32
		_aligned_free(p);
	#else
		free(p);
	#endif
}

#endif
