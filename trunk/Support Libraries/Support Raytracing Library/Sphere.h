#pragma once

#ifndef _SPHERE_

#define _SPHERE_

#include "Solid.h"

#include "Transform.h"

#include "Material.h"

using namespace std;

using namespace Math;

namespace Raytracing
{
	class Sphere : public Solid
	{
		public:

			//----------------------------------- Sphere Radius -----------------------------------

			float Radius;   
			 
			//-------------------------------- Tesselation Quality --------------------------------
			
			int Slices;
			
			int Stacks;

			//------------------------------------ Constructor ------------------------------------
		
			Sphere ( Math :: Transform *, Raytracing :: Material *, float radius = 1.0F );

			//---------------------------------- Build Triangles ----------------------------------
			
			void Tesselate ( void );
	};
}

#endif
