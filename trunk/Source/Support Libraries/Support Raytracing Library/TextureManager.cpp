/*
   Support Raytracing Library  
   Copyright (C) 2009  Denis Bogolepov ( bogdencmc@inbox.ru )

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see http://www.gnu.org/licenses.
*/

#include "TextureManager.h"

namespace Raytracing 
{
	//------------------------------------ Constant Parameters ------------------------------------

	const unsigned TextureManager :: TextureUnit = 0;

	//---------------------------------------- Destructor -----------------------------------------

	TextureManager :: ~TextureManager  ( void )
	{
		delete Texture;
	}

	//--------------------------------- Load Static Texture Data ----------------------------------

	void TextureManager :: SetupTextures ( void )
	{
		int width = 0, height = 0;

		for ( int index = 0; index < TextureData.size ( ); index++ )
		{
			if ( TextureData [index]->GetWidth ( ) > width )
				width = TextureData [index]->GetWidth ( );

			if ( TextureData [index]->GetHeight ( ) > height )
				height = TextureData [index]->GetHeight ( );
		}

		//------------------------------------------------------------------------------------

		TextureData3D * data = new TextureData3D ( width, height, TextureData.size ( ) + 1 );

		for ( int x = 0; x < width; x++ )
		{
			for ( int y = 0; y < height; y++ )
			{
				data->Pixel<Vector3D> ( x, y, 0 ) = Vector3D :: Unit;
			}
		}

		for ( int index = 0; index < TextureData.size ( ); index++ )
		{
			TextureData [index]->Identifier = index + 1;

			for ( int x = 0; x < width; x++ )
			{
				for ( int y = 0; y < height; y++ )
				{
					data->Pixel<Vector3D> ( x, y, index + 1 ) =
						TextureData [index]->Pixel<Vector3D> (
						( int ) ( TextureData [index]->GetWidth ( ) * x / ( float ) width ),
						( int ) ( TextureData [index]->GetHeight ( ) * y / ( float ) height ) );
				}
			}			
		}

		//------------------------------------------------------------------------------------

		Texture = new Texture3D ( data, TextureUnit, GL_TEXTURE_2D_ARRAY );

		Texture->Setup ( );
	}

	//-------------------------------------- Apply Settings ---------------------------------------

	void TextureManager :: SetShaderData ( ShaderManager * manager )
	{
		manager->SetTexture ( "ImageTextures", Texture );
	}
}