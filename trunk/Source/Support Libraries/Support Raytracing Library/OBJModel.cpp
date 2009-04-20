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

	MTLMaterial :: MTLMaterial ( char name [],
		                         const Vector3D& ambient,
								 const Vector3D& diffuse,
								 const Vector3D& specular,
								 float shininess,
								 const Vector3D& transmission,
								 float density,
								 float dissolve,
								 int model,
								 Texture2D * texture )
	{
		memcpy ( Name, name, LENGTH );

		//---------------------------------------------------------------------

		Ambient = ambient;

		Diffuse = diffuse;

		Specular = specular;

		Shininess = shininess;

		//---------------------------------------------------------------------
		
		Transmission = transmission;

		Density = density;

		//---------------------------------------------------------------------
			
		Dissolve = dissolve;

		//---------------------------------------------------------------------

		Model = model;

		//---------------------------------------------------------------------
		
		Texture = texture;
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

	//------------------------------------- Support Functions -------------------------------------
		
	Vector3D OBJModel :: GetMinimum ( void ) const
	{
		Vector3D minimum = Vertices [0];

		for ( int index = 0; index < Vertices.size ( ); index++ )
		{
			minimum = Min ( minimum, Vertices [index] );
		}

		return minimum;
	}
		
	Vector3D OBJModel :: GetMaximum ( void ) const
	{
		Vector3D maximum = Vertices [0];

		for ( int index = 0; index < Vertices.size ( ); index++ )
		{
			maximum = Max ( maximum, Vertices [index] );
		}

		return maximum;
	}
}