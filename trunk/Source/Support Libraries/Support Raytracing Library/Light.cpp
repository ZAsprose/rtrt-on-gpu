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

#include "Light.h"

namespace Raytracing
{
	//--------------------------------- Constructor and Destructor --------------------------------

	Light :: Light ( unsigned number,
		             const Vector3D& position,
					 const Vector3D& ambient,
					 const Vector3D& diffuse,
					 const Vector3D& specular,
					 const Vector3D& attenuation )
	{
		Number = number;

		Position = position;

		Ambient = ambient;

		Diffuse = diffuse;
		
		Specular = specular;

		Attenuation = attenuation;
	}

	Light :: ~Light ( void )
	{
		glDisable ( GL_LIGHT0 + Number );
	}

	//-------------------------- Applying Settings to OpenGL and Shaders --------------------------
	
	void Light :: Setup ( void ) 
	{
		glEnable ( GL_LIGHT0 + Number );

		float position [] = { Position.X, Position.Y, Position.Z, 1.0F };
		
		glLightfv ( GL_LIGHT0 + Number, GL_POSITION, position );
        	
		glLightfv ( GL_LIGHT0 + Number, GL_AMBIENT, Ambient );
        	
		glLightfv ( GL_LIGHT0 + Number, GL_DIFFUSE, Diffuse );
        	
		glLightfv ( GL_LIGHT0 + Number, GL_SPECULAR, Specular );

		glLightf ( GL_LIGHT0 + Number, GL_CONSTANT_ATTENUATION, Attenuation.X );
		
		glLightf ( GL_LIGHT0 + Number, GL_LINEAR_ATTENUATION, Attenuation.Y );
		
		glLightf ( GL_LIGHT0 + Number, GL_QUADRATIC_ATTENUATION, Attenuation.Z );
	}
	
	void Light :: SetShaderData ( ShaderManager * manager )
	{
		char name [200];

		sprintf ( name, "Lights[%d].Position", Number );

		manager->SetUniformVector ( name, Position );

		sprintf ( name, "Lights[%d].Ambient", Number );

		manager->SetUniformVector ( name, Ambient );

		sprintf ( name, "Lights[%d].Diffuse", Number );
        	
		manager->SetUniformVector ( name, Diffuse );

		sprintf ( name, "Lights[%d].Specular", Number );
        	
		manager->SetUniformVector ( name, Specular );

		sprintf ( name, "Lights[%d].Attenuation", Number );
        	
		manager->SetUniformVector ( name, Attenuation );
	}

	//---------------------------- Drawing Light Source in OpenGL Mode ----------------------------
	
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