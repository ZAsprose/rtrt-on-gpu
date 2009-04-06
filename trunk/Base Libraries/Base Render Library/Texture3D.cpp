/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "Texture3D.h"

#include <iostream>

using namespace std;

namespace Render
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	Texture3D :: Texture3D ( unsigned int unit )
	{
		Init ( GL_TEXTURE_3D, NULL, unit );
	}
				
	Texture3D :: Texture3D ( TextureData3D * data, unsigned int unit )
	{
		Init ( GL_TEXTURE_3D, data, unit );
	}
	
	Texture3D :: ~Texture3D ( void )
	{
		delete Data;

		glDeleteTextures ( 1, &Handle );
	}

	//--------------------------------------- Texture Init ----------------------------------------

	void Texture3D :: Init ( unsigned int target, TextureData3D * data, unsigned int unit )
	{
		Target = target;

		Data = data;

		Unit = unit;

		glGenTextures ( 1, &Handle );
	}

	//------------------------------------ Texture Management -------------------------------------
			
	void Texture3D :: Setup ( void )
	{
		cout << "Loading texture data [ \"" << Handle << "\" @ " << Unit << " ]" << endl;
	
		if ( NULL != Data )
		{
			Bind ( );

			WrapMode.Setup ( Target );

			FilterMode.Setup ( Target );
			
			Data->Upload ( Target );
		}
		else
		{
			cout << "ERROR: Data is empty" << endl;
		}			                
	}
	
	void Texture3D :: Bind ( void )
	{
		glActiveTexture ( GL_TEXTURE0 + Unit );
		
		glBindTexture ( Target, Handle );
	}
	
	void Texture3D :: Unbind ( void )
	{
		glActiveTexture ( GL_TEXTURE0 + Unit );
		
		glBindTexture ( Target, 0 );			
	}
}
