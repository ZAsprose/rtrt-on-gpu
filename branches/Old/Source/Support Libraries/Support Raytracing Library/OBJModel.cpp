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

#include "OBJModel.h"

namespace Raytracing
{
	//------------------------------ MTLTextureData Class Subroutines -----------------------------

	MTLTextureData :: MTLTextureData ( char name [], TextureData2D * data )
	{
		memcpy ( Name, name, LENGTH );

		Data = data;
	}

	//------------------------------- MTLMaterial Class Subroutines -------------------------------

	MTLMaterial :: MTLMaterial ( char name [], Material * properties )
	{
		memcpy ( Name, name, LENGTH );
		
		Properties = properties;
	}

	//--------------------------------- OBJGroup Class Subroutines --------------------------------

	OBJGroup :: OBJGroup ( MTLMaterial * material )
	{
		Material = material;
	}

	OBJGroup :: ~OBJGroup ( void )
	{
		Faces.clear ( );
	}

	//--------------------------------- OBJModel Class Subroutines --------------------------------

	OBJModel :: ~OBJModel ( void )
	{
		for ( int index = 0; index < Groups.size ( ); index++ )
		{
			delete Groups [index];
		}

		for ( int index = 0; index < Materials.size ( ); index++ )
		{
			delete Materials [index];
		}

		for ( int index = 0; index < Textures.size ( ); index++ )
		{
			delete Textures [index];
		}

		Groups.clear ( );

		Materials.clear ( );

		Textures.clear ( );

		Vertices.clear ( );
		
		Normals.clear ( );
		
		TexCoords.clear ( );
	}

	//------------------------------------- Support Functions -------------------------------------
		
	Vector3D OBJModel :: GetMinimum ( void ) const
	{
		Vector3D minimum = Vertices [0];

		for ( int index = 1; index < Vertices.size ( ); index++ )
		{
			minimum = Min ( minimum, Vertices [index] );
		}

		return minimum - EPSILON;
	}
		
	Vector3D OBJModel :: GetMaximum ( void ) const
	{
		Vector3D maximum = Vertices [0];

		for ( int index = 1; index < Vertices.size ( ); index++ )
		{
			maximum = Max ( maximum, Vertices [index] );
		}

		return maximum + EPSILON;
	}
}