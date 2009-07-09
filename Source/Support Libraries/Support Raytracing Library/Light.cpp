/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "Light.h"

#include <GL/glfw.h>

namespace Raytracing
{
	//--------------------------------- Constructor and Destructor --------------------------------

	Light :: Light ( unsigned number,
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

	Light :: ~Light ( void )
	{
		glDisable ( GL_LIGHT0 + Number );
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
	
	void Light :: SetShaderData ( ShaderManager * manager )
	{
		char name [200];

		sprintf ( name, "Lights[%d].Ambient", Number );

		manager->SetUniformVector ( name, Ambient );

		sprintf ( name, "Lights[%d].Diffuse", Number );
        	
		manager->SetUniformVector ( name, Diffuse );

		sprintf ( name, "Lights[%d].Specular", Number );
        	
		manager->SetUniformVector ( name, Specular );

		sprintf ( name, "Lights[%d].Position", Number );

		manager->SetUniformVector ( name, Position );
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