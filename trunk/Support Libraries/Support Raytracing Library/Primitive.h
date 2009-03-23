#pragma once

#ifndef _PRIMITIVE_

#define _PRIMITIVE_

#include "Triangle.h"

#include "Transform.h"

#include <vector>

using namespace std;

namespace Raytracing
{
	class Primitive abstract
	{
		public:

			//------------------------------ Triangles and Transform ------------------------------

			vector < Triangle * > Triangles;

			Transform * Transformation;

			//-------------------------------- Visibility and Name --------------------------------
			
			bool Visible;

			const char * Name;

			//----------------------------- Constructor and Destructor ----------------------------

			Primitive ( Transform *, const char * );
			
			virtual ~Primitive ( void );

			//---------------------------------- Build Triangles ----------------------------------
			
			virtual void Tesselate ( void ) = 0;

			//--------------------------------------- Draw ----------------------------------------
			
			virtual void Draw ( void ) = 0;
	};
}

#endif