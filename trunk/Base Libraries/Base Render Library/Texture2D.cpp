#include "Texture2D.h"

#include <iostream>

using namespace std;

namespace RenderTools
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	Texture2D :: Texture2D ( unsigned int unit, unsigned int target )
	{
		Init ( target, NULL, unit );
	}
				
	Texture2D :: Texture2D ( TextureData2D * data, unsigned int unit, unsigned int target )
	{
		Init ( target, data, unit );
	}

	Texture2D :: ~Texture2D ( void )
	{
		delete Data;

		glDeleteTextures ( 1, &Handle );
	}

	//--------------------------------------- Texture Init ----------------------------------------

	void Texture2D :: Init ( unsigned int target, TextureData2D * data, unsigned int unit )
	{
		Target = target;

		Data = data;

		Unit = unit;

		glGenTextures ( 1, &Handle );
	}

	//------------------------------------ Texture Management -------------------------------------
			
	void Texture2D :: Setup ( void )
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
	
	void Texture2D :: Bind ( void )
	{
		glActiveTexture ( GL_TEXTURE0 + Unit );
		
		glBindTexture ( Target, Handle );
	}
	
	void Texture2D :: Unbind ( void )
	{
		glActiveTexture ( GL_TEXTURE0 + Unit );
		
		glBindTexture ( Target, 0 );			
	}
}
