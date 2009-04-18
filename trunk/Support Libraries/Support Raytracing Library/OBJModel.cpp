/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "OBJModel.h"

namespace Raytracing
{
	//-------------------------------- Constructors and Destructors -------------------------------

	MTLTexture :: MTLTexture ( char name [], Texture2D * texture )
	{
		memcpy ( Name, name, LENGTH );

		Texture = texture;
	}
	
	MTLTexture :: ~MTLTexture ( void )
	{
		delete Texture;
	}

	MTLMaterial :: MTLMaterial ( char name [] )
	{
		memcpy ( Name, name, LENGTH );

		Texture = NULL;
	}

	OBJGroup :: OBJGroup ( MTLMaterial * material )
	{
		Material = material;
	}

	OBJGroup :: ~OBJGroup ( void )
	{
		delete Material;
	}

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

		Vertices.clear ( );
		
		Normals.clear ( );
		
		TexCoords.clear ( );
		
		Groups.clear ( );

		Materials.clear ( );

		Textures.clear ( );
	}
}