/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#pragma once

#ifndef _PLANE_

#define _PLANE_

#include "Primitive.h"

namespace Raytracing
{
	class Plane : public Primitive
	{
		public:

			//-------------------------------- Half Size ( Radius ) -------------------------------

			Vector2D Radius;

			//------------------------------------ Constructor ------------------------------------
		
			Plane ( const Vector2D& = Vector2D :: Unit,
				    Transform * = new Transform ( ),
					Material * = new Material ( ),
					const char * = "Plane",
					bool = true );

			//---------------------------------- Build Triangles ----------------------------------
			
			void Tesselate ( void );
	};
}

#endif