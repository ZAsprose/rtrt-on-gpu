/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

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

			Material * Properties;

			//----------------------------- Constructor and Destructor ----------------------------
			
			Triangle ( Vertex * = NULL, Vertex * = NULL, Vertex * = NULL, Material * = NULL );

			~Triangle ( void );

			//--------------------------------------- Draw ----------------------------------------
			
			void Draw ( void ) const;

			//--------------------------------- Support Functions ---------------------------------
			
			bool IsEmpty ( void ) const;

			Vector3D GetNormal ( void ) const;

			Vector3D GetMinimum ( void ) const;

			Vector3D GetMaximum ( void ) const;
	};
}

#endif
