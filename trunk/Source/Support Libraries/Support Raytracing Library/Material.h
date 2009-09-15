/*
   S U P P O R T   R A Y   T R A C I N G   L I B R A R Y

   Copyright (C) 2009  Denis Bogolepov ( bogdencmc@inbox.ru )

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see http://www.gnu.org/licenses.
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

			//--------------------------- Phong Reflectance Coefficients --------------------------

			Vector3D Ambient;

			Vector3D Diffuse;

			Vector3D Specular;
			
			float Shininess;

			//------------------- Reflection and Refraction and Optical Density -------------------
			
			Vector3D Reflection;

			Vector3D Refraction;
			
			float Density;

			//-------------------------------- Dissolve Coefficient -------------------------------
			
			float Dissolve;

			//--------------------------- Texture Data and Texture Scale --------------------------

			TextureData2D * Data;

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
					   TextureData2D * = NULL,
					   const Vector2D& = Vector2D :: Unit );
			
			//----------------------------------- Apply Settings ----------------------------------

			void Setup ( void );
	};
}

#endif