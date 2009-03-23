#pragma once

#ifndef _SOLID_PRIMITIVE_

#define _SOLID_PRIMITIVE_

#include "Primitive.h"

namespace Raytracing
{
	class Solid : public Primitive
	{
		public:

			//-------------------------------- Material Properties --------------------------------

			Material * Properties;

			//------------------------------------ Constructor ------------------------------------

			Solid ( Transform *, Material *, const char * );

			//--------------------------------------- Draw ----------------------------------------
			
			void Draw ( void );
	};
}

#endif
