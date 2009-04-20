/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#pragma once

#ifndef _MATERIAL_

#define _MATERIAL_

#include <Vector3D.h>

#include <Texture2D.h>

using namespace Math;

using namespace Render;

namespace Raytracing
{
	class Material
	{
		public:

			//----------------------------- Phong Reflectance Coeffs ------------------------------

			Vector3D Ambient;

			Vector3D Diffuse;

			Vector3D Specular;
			
			float Shininess;

			//------------------- Reflection and Refraction and Optical Density -------------------
			
			Vector3D Reflection;

			Vector3D Refraction;
			
			float Density;

			//--------------------------------- Material Dissolve ---------------------------------
			
			float Dissolve;

			//--------------------------- Raster Texture and Image Scale --------------------------

			Texture2D * Texture;

			Vector2D Scale;

			//-------------------------------- Material Identifier --------------------------------

			int Identifier;

			//------------------------------------ Constructor ------------------------------------
			
			Material ( const Vector3D& = Vector3D ( 0.2F, 0.2F, 0.2F ),
					   const Vector3D& = Vector3D ( 0.8F, 0.8F, 0.8F ),
					   const Vector3D& = Vector3D ( 0.8F, 0.8F, 0.8F ),
					   float = 32.0F,
					   const Vector3D& = Vector3D :: Zero,
					   const Vector3D& = Vector3D :: Zero,
					   float = 1.5F,
					   float = 1.0F,
					   Texture2D * = NULL,
					   const Vector2D& = Vector2D :: Unit );
			
			//----------------------------------- Apply Settings ----------------------------------

			void Setup ( void );
	};
}

#endif