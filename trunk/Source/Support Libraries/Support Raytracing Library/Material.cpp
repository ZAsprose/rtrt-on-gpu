/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "Material.h"

#include <GL/glfw.h>

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