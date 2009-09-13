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

#include "Primitive.h"

namespace Raytracing
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	Primitive :: Primitive ( Transform * transformation,
		                     Material * properties,
							 const char * name,
							 bool visible )
	{
		Transformation = transformation;

		Properties = properties;

		Name = name;

		Visible = visible;
	}

	Primitive :: ~Primitive ( void )
	{
		for ( unsigned index = 0; index < Triangles.size ( ); index++ )
		{
			delete Triangles [index];
		}

		Triangles.clear ( );

		delete Transformation;
		
		delete Properties;
	}

	//------------------------------ Drawing Primitive in OpenGL Mode -----------------------------
	
	void Primitive :: Draw ( void )
	{
		Properties->Setup ( );

		glBegin ( GL_TRIANGLES );

			for ( unsigned index = 0; index < Triangles.size ( ); index++ )
			{
				Triangles [index]->Draw ( );
			}

		glEnd ( );
	}
}