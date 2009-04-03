#include "Material.h"

#include <GL/glfw.h>

namespace Raytracing
{
	//---------------------------------------- Constructor ----------------------------------------
	
	Material :: Material ( const Vector3D& color,
		                   const Vector3D& ambiant,
						   const Vector3D& diffuse,
						   const Vector3D& specular,
						   float shininess,
						   const Vector3D& reflective,
						   const Vector3D& refractive,
						   float refractIndex )
	{
		Color = color;
				
		Ambient = ambiant;
				
		Diffuse = diffuse;
				
		Specular = specular;
				
		Shininess = shininess;
				
		Reflective = reflective;
				
		Refractive = refractive;
				
		RefractIndex = refractIndex;
	}

	//--------------------------------------- Apply Settings --------------------------------------
	
	void Material :: Setup ( void ) 
	{
		glMaterialfv ( GL_FRONT_AND_BACK, GL_AMBIENT, Ambient );
		
		glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE, Diffuse * Color );
		
		glMaterialfv ( GL_FRONT_AND_BACK, GL_SPECULAR, Specular );
		
		glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, Shininess );
	}
}