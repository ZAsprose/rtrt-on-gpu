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

			//--------------------------- 3D Model Loaded From OBJ File ---------------------------

			const OBJModel * Model;

			//------------------------------------ Constructor ------------------------------------
		
			Mesh ( const OBJModel *,
				   Transform * = new Transform ( ),
				   Material * = new Material ( ),
				   const char * = "Mesh" );

			//---------------------------------- Build Triangles ----------------------------------
			
			void Tesselate ( void );
	};
}

#endif