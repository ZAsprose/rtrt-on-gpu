/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#pragma once

#ifndef _BOX_

#define _BOX_

#include "Primitive.h"

namespace Raytracing
{
	class Box : public Primitive
	{
		public:

			//-------------------------------- Half Size ( Radius ) -------------------------------

			Vector3D Radius;

			//------------------------------------ Constructor ------------------------------------
		
			Box ( const Vector3D& = Vector3D :: Unit,
				  Transform * = new Transform ( ),
				  Material * = new Material ( ),
				  const char * = "Box",
				  bool = true );

			//---------------------------------- Build Triangles ----------------------------------
			
			void Tesselate ( void );
	};
}
#endif