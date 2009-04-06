/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

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

			//------------------------------------- Triangles -------------------------------------

			vector < Triangle * > Triangles;

			//----------------------- Transformation and Material Properties ----------------------

			Transform * Transformation;

			Material * Properties;

			//-------------------------------- Visibility and Name --------------------------------
			
			bool Visible;

			const char * Name;

			//----------------------------- Constructor and Destructor ----------------------------

			Primitive ( Transform *, Material *, const char * );
			
			virtual ~Primitive ( void );

			//---------------------------------- Build Triangles ----------------------------------
			
			virtual void Tesselate ( void ) = 0;

			//--------------------------------------- Draw ----------------------------------------
			
			void Draw ( void );
	};
}

#endif