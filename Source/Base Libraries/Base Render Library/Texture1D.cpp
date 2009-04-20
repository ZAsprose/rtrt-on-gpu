/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "Texture1D.h"

#include <iostream>

using namespace std;

namespace Render
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	Texture1D :: Texture1D ( unsigned unit )
	{
		Init ( GL_TEXTURE_1D, NULL, unit );
	}
				
	Texture1D :: Texture1D ( TextureData1D * data, unsigned unit )
	{
		Init ( GL_TEXTURE_1D, data, unit );
	}
	
	Texture1D :: ~Texture1D ( void )
	{
		delete Data;

		glDeleteTextures ( 1, &Handle );
	}

	//--------------------------------------- Texture Init ----------------------------------------

	void Texture1D :: Init ( unsigned target, TextureData1D * data, unsigned unit )
	{
		Target = target;

		Data = data;

		Unit = unit;

		glGenTextures ( 1, &Handle );
	}

	//------------------------------------ Texture Management -------------------------------------
			
	void Texture1D :: Setup ( void )
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
	
	void Texture1D :: Bind ( void )
	{
		glActiveTexture ( GL_TEXTURE0 + Unit );
		
		glBindTexture ( Target, Handle );
	}
	
	void Texture1D :: Unbind ( void )
	{
		glActiveTexture ( GL_TEXTURE0 + Unit );
		
		glBindTexture ( Target, 0 );			
	}
}