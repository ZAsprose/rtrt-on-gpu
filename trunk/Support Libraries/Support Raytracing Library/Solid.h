#pragma once

#ifndef _SOLID_PRIMITIVE_

#define _SOLID_PRIMITIVE_

#include "Primitive.h"

#include "Triangle.h"

#include <Transform.h>

#include <vector>

using namespace std;

namespace Raytracing
{
	class Solid : public Primitive
	{
		public:

			Material * Material;

			//----------------------------- Constructor and Destructor ----------------------------

			Solid ( void );
			
			~Solid ( void );

			//--------------------------------------- Draw ----------------------------------------
			
			void Draw ( void );
	};
}

#endif
