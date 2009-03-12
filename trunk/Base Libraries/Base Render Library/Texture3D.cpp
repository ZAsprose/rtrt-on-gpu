#include "Texture3D.h"

#include <iostream>

using namespace std;

namespace RenderTools
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	Texture3D :: Texture3D ( int unit, const char * name )
	{
		Init ( GL_TEXTURE_3D, NULL, unit, name );
	}
				
	Texture3D :: Texture3D ( TextureData3D * data, int unit, const char * name )
	{
		Init ( GL_TEXTURE_3D, data, unit, name );
	}
	
	Texture3D :: ~Texture3D ( void )
	{
		delete Data;

		glDeleteTextures ( 1, &Handle );
	}

	//--------------------------------------- Texture Init ----------------------------------------

	void Texture3D :: Init ( int target, TextureData3D * data, int unit, const char * name )
	{
		Target = target;

		Data = data;

		Unit = unit;

		Name = name;

		glGenTextures ( 1, &Handle );
	}

	//------------------------------------ Texture Management -------------------------------------
			
	void Texture3D :: Setup ( void )
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
