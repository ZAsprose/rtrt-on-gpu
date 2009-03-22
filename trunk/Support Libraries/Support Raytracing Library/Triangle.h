#pragma once

#ifndef _TRIANGLE_

#define _TRIANGLE_

#include "Vertex.h"

#include "Material.h"

namespace Raytracing
{
	class Triangle
	{
		public:

			//--------------------------------- Triangle Vertices ---------------------------------

			Vertex * VertexA;

			Vertex * VertexB;
			
			Vertex * VertexC;

			//--------------------------------- Triangle Material ---------------------------------

			Raytracing::Material * Material;

			//----------------------------- Constructor and Destructor ----------------------------
			
			Triangle ( Vertex *, Vertex *, Vertex *, Raytracing::Material * = NULL );

			~Triangle ( void );

			//--------------------------------------- Draw ----------------------------------------
			
			void Draw ( void );

			//--------------------------------- Support Functions ---------------------------------
			
			bool IsEmpty ( void );

			Vector3D GetMinimum ( void );

			Vector3D GetMaximum ( void );
	};
}

#endif
