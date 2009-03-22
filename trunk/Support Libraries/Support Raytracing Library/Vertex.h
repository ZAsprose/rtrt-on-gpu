#pragma once

#ifndef _VERTEX_

#define _VERTEX_

#include <Vector3D.h>

using namespace Math;

namespace Raytracing
{
	class Vertex
	{
		public:

			//--------------------------------- Vertex Attributes ---------------------------------
			
			Vector3D Position;
			
			Vector3D Normal;

			//------------------------------------ Constructor ------------------------------------
			
			Vertex ( const Vector3D& = Vector3D :: Zero, const Vector3D& = Vector3D :: AxisZ );

			//--------------------------------------- Draw ----------------------------------------

			void Draw ( void );
	};
}

#endif