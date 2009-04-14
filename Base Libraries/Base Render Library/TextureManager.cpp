#include "TextureManager.h"

namespace Render
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	TextureManager :: TextureManager ( void ) { }

	TextureManager :: ~TextureManager ( void )
	{
		for ( int index = 0; index < ImageTextures.size ( ); index++ )
		{
			delete ImageTextures [index];
		}

		ImageTextures.clear ( );
	}

	//--------------------------------- Load Static Texture Data ----------------------------------

	void TextureManager :: SetupTextures ( void )
	{
		for ( int index = 0; index < ImageTextures.size ( ); index++ )
		{
			ImageTextures [index]->Setup ( );
		}
	}

	//-------------------------------------- Apply Settings ---------------------------------------

	void TextureManager :: SetShaderData ( ShaderManager * manager )
	{
		char name [200];

		for ( int index = 0; index < ImageTextures.size ( ); index++ )
		{
			sprintf ( name, "ImageTexture%d", index );

			manager->SetTexture ( name, ImageTextures [index] );
		}
	}
}
