/*
   Support Raytracing Library  
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

#include "Material.h"

namespace Raytracing
{
	//---------------------------------------- Constructor ----------------------------------------
	
	Material :: Material ( const Vector3D& ambiant,
						   const Vector3D& diffuse,
						   const Vector3D& specular,
						   float shininess,
						   const Vector3D& reflection,
						   const Vector3D& refraction,
						   float density,
						   float dissolve,
						   TextureData2D * data,
						   const Vector2D& scale )
	{
		Ambient = ambiant;
				
		Diffuse = diffuse;
				
		Specular = specular;
				
		Shininess = shininess;

		//-------------------------------------------------------------------------------
				
		Reflection = reflection;

		Refraction = refraction;
				
		Density = density;

		//-------------------------------------------------------------------------------

		Dissolve = dissolve;

		//-------------------------------------------------------------------------------

		Data = data;

		Scale = scale;
	}

	//--------------------------------------- Apply Settings --------------------------------------
	
	void Material :: Setup ( void ) 
	{
		glMaterialfv ( GL_FRONT_AND_BACK, GL_AMBIENT, Ambient );
		
		glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE, Diffuse );
		
		glMaterialfv ( GL_FRONT_AND_BACK, GL_SPECULAR, Specular );
		
		glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, Shininess );
	}
}