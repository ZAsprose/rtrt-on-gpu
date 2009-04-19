/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
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

	//-------------------------------------------- Draw -------------------------------------------

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