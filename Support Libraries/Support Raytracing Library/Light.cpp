#include "Light.h"

#include <GL/glfw.h>

namespace Raytracing
{
	//---------------------------------------- Constructor ----------------------------------------

	Light :: Light ( unsigned int number,
		             const Vector3D& position,
					 const Vector3D& ambient,
					 const Vector3D& diffuse,
					 const Vector3D& specular  )
	{
		Number = number;

		Position = position;

		Ambient = ambient;

		Diffuse = diffuse;
		
		Specular = specular;
	}

	//--------------------------------------- Apply Settings --------------------------------------
	
	void Light :: Setup ( void ) 
	{
		glEnable ( GL_LIGHT0 + Number );

		float position [] = { Position.X, Position.Y, Position.Z, 1.0F };
		
		glLightfv ( GL_LIGHT0 + Number, GL_POSITION, position );
        	
		glLightfv ( GL_LIGHT0 + Number, GL_AMBIENT, Ambient );
        	
		glLightfv ( GL_LIGHT0 + Number, GL_DIFFUSE, Diffuse );
        	
		glLightfv ( GL_LIGHT0 + Number, GL_SPECULAR, Specular );
	}

	//-------------------------------------------- Draw -------------------------------------------
	
	void Light :: Draw ( void ) 
	{
		glDisable ( GL_LIGHTING );
			
		glBegin ( GL_POINTS );
		
		glColor3fv ( Diffuse );
		
		glVertex3fv ( Position );
		
		glEnd ( );
		
		glEnable ( GL_LIGHTING );
	}
}