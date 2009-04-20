#include "TextureManager.h"

namespace Raytracing
{
	//--------------------------------- Load Static Texture Data ----------------------------------

	void TextureManager :: SetupTextures ( void )
	{
		for ( int index = 0; index < Textures.size ( ); index++ )
		{
			Textures [index]->Setup ( );
		}
	}

	//-------------------------------------- Apply Settings ---------------------------------------

	void TextureManager :: SetShaderData ( ShaderManager * manager )
	{
		char name [200];

		for ( int index = 0; index < Textures.size ( ); index++ )
		{
			sprintf ( name, "ImageTexture%d", index );

			manager->SetTexture ( name, Textures [index] );
		}
	}
}