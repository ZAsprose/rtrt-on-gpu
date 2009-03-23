#pragma once

#ifndef _SPHERE_

#define _SPHERE_

#include "Solid.h"

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
		
			Sphere ( float = 1.0F,
				     int = 25,
					 int = 25,
				     Transform * = new Transform ( ),
					 Material * = new Material ( ),
					 const char * = "Sphere" );

			//---------------------------------- Build Triangles ----------------------------------
			
			void Tesselate ( void );
	};
}

#endif