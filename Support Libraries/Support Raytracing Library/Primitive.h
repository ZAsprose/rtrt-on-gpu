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

			//------------------------------ Vertices Transformation ------------------------------

			Transform * Transformation;

			//-------------------------------- Material Properties --------------------------------

			Material * Properties;

			//-------------------------------- Name and Visibility --------------------------------

			const char * Name;

			bool Visible;

			//----------------------------- Constructor and Destructor ----------------------------

			Primitive ( Transform * = new Transform ( ),
				        Material * = new Material ( ),
						const char * = "Primitive",
						bool = true );
			
			virtual ~Primitive ( void );

			//---------------------------------- Build Triangles ----------------------------------
			
			virtual void Tesselate ( void ) = 0;

			//--------------------------------------- Draw ----------------------------------------
			
			void Draw ( void );
	};
}

#endif