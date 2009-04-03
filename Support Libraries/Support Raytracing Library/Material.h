#pragma once

#ifndef _MATERIAL_

#define _MATERIAL_

#include <Vector3D.h>

using namespace Math;

namespace Raytracing
{
	class Material
	{
		public:

			//---------------------------------- Phong Constants ----------------------------------

			Vector3D Ambient;

			Vector3D Diffuse;

			Vector3D Specular;

			Vector3D Color;

			float Shininess;

			//----------------------------- Reflection and Refraction -----------------------------
			
			Vector3D Reflective;
			
			Vector3D Refractive;	
			
			float RefractIndex;

			//-------------------------------- Material Identifier --------------------------------

			int Identifier;

			//------------------------------------ Constructor ------------------------------------
			
			Material ( const Vector3D& = Vector3D :: Unit,
				       const Vector3D& = Vector3D ( 0.2F, 0.2F, 0.2F ),
					   const Vector3D& = Vector3D ( 0.8F, 0.8F, 0.8F ),
					   const Vector3D& = Vector3D ( 0.8F, 0.8F, 0.8F ),
					   float = 32.0F,
					   const Vector3D& = Vector3D :: Zero,
					   const Vector3D& = Vector3D :: Zero,
					   float = 1.5F );
			
			//----------------------------------- Apply Settings ----------------------------------

			void Setup ( void );
	};
}

#endif