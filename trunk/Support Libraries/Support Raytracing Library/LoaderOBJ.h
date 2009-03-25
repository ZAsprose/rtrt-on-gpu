#pragma once

#ifndef _LOADER_OBJ_

#define _LOADER_OBJ_

#include "OBJModel.h"

namespace Raytracing
{
	class LoaderOBJ
	{
		public:

			//--------------------------------- Loading OBJ Model ---------------------------------
			
			static OBJModel * LoadModel ( const char * filename );
	};
}

#endif
