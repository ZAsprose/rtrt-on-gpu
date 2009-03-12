#include "Texture1D.h"

#include <iostream>

using namespace std;

namespace RenderTools
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	Texture1D :: Texture1D ( int unit, const char * name )
	{
		Init ( GL_TEXTURE_1D, NULL, unit, name );
	}
				
	Texture1D :: Texture1D ( TextureData1D * data, int unit, const char * name )
	{
		Init ( GL_TEXTURE_1D, data, unit, name );
	}
	
	Texture1D :: ~Texture1D ( void )
	{
		delete Data;

		glDeleteTextures ( 1, &Handle );
	}

	//--------------------------------------- Texture Init ----------------------------------------

	void Texture1D :: Init ( int target, TextureData1D * data, int unit, const char * name )
	{
		Target = target;

		Data = data;

		Unit = unit;

		Name = name;

		glGenTextures ( 1, &Handle );
	}

	//------------------------------------ Texture Management -------------------------------------
			
	void Texture1D :: Setup ( void )
	{
		cout << "Loading texture data [ \"" << Name << "\" @ " << Unit << " ]" << endl;
	
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