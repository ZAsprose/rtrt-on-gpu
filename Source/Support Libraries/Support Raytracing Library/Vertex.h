/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

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

			Vector2D TexCoord;

			//------------------------------------ Constructor ------------------------------------
			
			Vertex ( const Vector3D& = Vector3D :: Zero,
				     const Vector3D& = Vector3D :: AxisZ,
					 const Vector2D& = Vector2D :: Zero );

			//--------------------------------------- Draw ----------------------------------------

			void Draw ( void );
	};
}

#endif