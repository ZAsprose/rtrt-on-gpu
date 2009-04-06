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

			//------------------------------------- Half Size -------------------------------------

			Vector3D HalfSize;

			//------------------------------------ Constructor ------------------------------------
		
			Box ( const Vector3D& = Vector3D :: Unit,
				  Transform * = new Transform ( ),
				  Material * = new Material ( ),
				  const char * = "Box" );

			//---------------------------------- Build Triangles ----------------------------------
			
			void Tesselate ( void );
	};
}
#endif