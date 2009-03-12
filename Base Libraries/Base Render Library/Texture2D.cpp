#include "Texture2D.h"

#include <iostream>

using namespace std;

namespace RenderTools
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	Texture2D :: Texture2D ( int unit, const char * name )
	{
		Init ( GL_TEXTURE_2D, NULL, unit, name );
	}
				
	Texture2D :: Texture2D ( TextureData2D * data, int unit, const char * name )
	{
		Init ( GL_TEXTURE_2D, data, unit, name );
	}

	Texture2D :: Texture2D ( int target, int unit, const char * name )
	{
		Init ( target, NULL, unit, name );
	}
				
	Texture2D :: Texture2D ( int target, TextureData2D * data, int unit, const char * name )
	{
		Init ( target, data, unit, name );
	}
	
	Texture2D :: ~Texture2D ( void )
	{
		delete Data;

		glDeleteTextures ( 1, &Handle );
	}

	//--------------------------------------- Texture Init ----------------------------------------

	void Texture2D :: Init ( int target, TextureData2D * data, int unit, const char * name )
	{
		Target = target;

		Data = data;

		Unit = unit;

		Name = name;

		glGenTextures ( 1, &Handle );
	}

	//------------------------------------ Texture Management -------------------------------------
			
	void Texture2D :: Setup ( void )
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
