/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#pragma once

#ifndef _OBJ_LOADER_

#define _OBJ_LOADER_

#include "OBJModel.h"

namespace Raytracing
{
	class OBJLoader
	{
		private:

			//--------------------------------- Loading Text File ---------------------------------

			static char * LoadFile ( const char *, unsigned& );

			//----------------------------- Loading MTL Material File -----------------------------

			static bool LoadMTL ( const char *, OBJModel * );

		public:

			//----------------------------- Loading OBJ Geometry File -----------------------------

			static OBJModel * LoadOBJ ( const char * );
	};
}

#endif
