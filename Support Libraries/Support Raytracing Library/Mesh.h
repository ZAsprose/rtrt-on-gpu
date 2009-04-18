/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#pragma once

#ifndef _MESH_

#define _MESH_

#include "Primitive.h"

#include "OBJModel.h"

namespace Raytracing
{
	class Mesh : public Primitive
	{
		public:

			//--------------------------- 3D Model Loaded from OBJ File ---------------------------

			const OBJModel * Model;

			int Group;

			//------------------------------------ Constructor ------------------------------------
		
			Mesh ( const OBJModel *,
				   int = 0,
				   Transform * = new Transform ( ),
				   Material * = new Material ( ),
				   const char * = "Mesh",
				   bool = true );

			//---------------------------------- Build Triangles ----------------------------------
			
			void Tesselate ( void );
	};
}

#endif