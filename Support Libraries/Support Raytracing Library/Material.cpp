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
						   const Vector3D& reflective,
						   const Vector3D& refractive,
						   float refractIndex,
						   Texture2D * texture,
						   const Vector2D& scale )
	{
		Ambient = ambiant;
				
		Diffuse = diffuse;
				
		Specular = specular;
				
		Shininess = shininess;

		//-------------------------------------------------------------------------------
				
		Reflective = reflective;
				
		Refractive = refractive;
				
		RefractIndex = refractIndex;

		//-------------------------------------------------------------------------------

		Texture = texture;

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