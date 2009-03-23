#pragma once

#ifndef _BOX_

#define _BOX_

#include "Solid.h"

using namespace std;

namespace Raytracing
{
	class Box : public Solid
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