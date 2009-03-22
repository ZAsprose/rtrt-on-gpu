#pragma once

#ifndef _PRIMITIVE_

#define _PRIMITIVE_

#include "Triangle.h"

#include <Transform.h>

#include <vector>

using namespace std;

namespace Raytracing
{
	class Primitive abstract
	{
		public:

			//-------------------------------- Primitive Triangles ---------------------------------

			vector < Triangle * > Triangles;

			//------------------------------ Triangles Transformation ------------------------------

			Transform * Transform;

			//--------------------------- Primitive Name and Visibility ---------------------------
			
			const char * Name;
			
			bool Visible;	  

			//----------------------------- Constructor and Destructor ----------------------------

			Primitive ( void );
			
			~Primitive ( void );

			//---------------------------------- Build Triangles ----------------------------------
			
			virtual void Tesselate ( void ) = 0;

			//--------------------------------------- Draw ----------------------------------------
			
			virtual void Draw ( void ) = 0;
	};
}

#endif
