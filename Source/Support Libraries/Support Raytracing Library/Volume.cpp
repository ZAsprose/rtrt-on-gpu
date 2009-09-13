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

#include "Volume.h"

namespace Raytracing
{
	//---------------------------------------- Constructor ----------------------------------------

	Volume :: Volume ( const Vector3D& minimum,
		               const Vector3D& maximum,
					   const Vector3D& color,
					   bool visible )
	{
		Minimum = minimum;

		Maximum = maximum;

		Color = color;

		Visible = visible;
	}

	//---------------------------- Drawing Volume Frame in OpenGL Mode ----------------------------

	void Volume :: Draw ( void )
	{
		if ( Visible )
		{
			glDisable ( GL_LIGHTING );

			glColor3fv ( Color );

			glBegin ( GL_LINE_LOOP );		

				glVertex3f ( Minimum.X, Minimum.Y, Minimum.Z );
				glVertex3f ( Maximum.X, Minimum.Y, Minimum.Z );
				glVertex3f ( Maximum.X, Maximum.Y, Minimum.Z );
				glVertex3f ( Minimum.X, Maximum.Y, Minimum.Z );		

			glEnd ( );

			glBegin ( GL_LINE_LOOP );	

				glVertex3f ( Minimum.X, Minimum.Y, Maximum.Z );
				glVertex3f ( Maximum.X, Minimum.Y, Maximum.Z );
				glVertex3f ( Maximum.X, Maximum.Y, Maximum.Z );
				glVertex3f ( Minimum.X, Maximum.Y, Maximum.Z );		

			glEnd ( );

			glBegin ( GL_LINES );			

				glVertex3f ( Minimum.X, Minimum.Y, Minimum.Z );
				glVertex3f ( Minimum.X, Minimum.Y, Maximum.Z );
				glVertex3f ( Maximum.X, Minimum.Y, Minimum.Z );
				glVertex3f ( Maximum.X, Minimum.Y, Maximum.Z );
				glVertex3f ( Maximum.X, Maximum.Y, Minimum.Z );
				glVertex3f ( Maximum.X, Maximum.Y, Maximum.Z );
				glVertex3f ( Minimum.X, Maximum.Y, Minimum.Z );
				glVertex3f ( Minimum.X, Maximum.Y, Maximum.Z );	

			glEnd ( );

			glEnable ( GL_LIGHTING );
		}
	}
}