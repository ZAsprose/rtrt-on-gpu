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
		delete TextureArray;
	}

	//---------------------- Fetching Texture Data with Bilinear Filtration -----------------------

	Vector3D TextureManager :: FilterData ( TextureData2D * data, float x, float y )
	{
		int xMin = floor ( x ),
			yMin = floor ( y );

		float xRatio = x - xMin,
			  yRatio = y - yMin;

		int xMax = min ( xMin + 1, data->GetWidth ( ) - 1 ),
			yMax = min ( yMin + 1, data->GetHeight ( ) - 1 );

		return Mix ( Mix ( data->Pixel<Vector3D> ( xMin, yMin ),
			               data->Pixel<Vector3D> ( xMax, yMin ),
						   xRatio ),
			         Mix ( data->Pixel<Vector3D> ( xMin, yMax ),
					       data->Pixel<Vector3D> ( xMax, yMax ),
						   xRatio ),
					 yRatio );
	}

	//-------------------------- Applying Settings to OpenGL and Shaders --------------------------

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
					data->Pixel<Vector3D> ( x, y, index + 1 ) = FilterData (
						TextureData [index],
						TextureData [index]->GetWidth ( ) * x / ( float ) width,
						TextureData [index]->GetHeight ( ) * y / ( float ) height );
				}
			}			
		}

		//------------------------------------------------------------------------------------

		TextureArray = new Texture3D ( data, TextureUnit, GL_TEXTURE_2D_ARRAY );

		TextureArray->FilterMode = FilterMode :: Linear;

		TextureArray->WrapMode = WrapMode :: Repeat;

		TextureArray->Setup ( );
	}
	
	void TextureManager :: SetShaderData ( ShaderManager * manager )
	{
		manager->SetTexture ( "ImageTextures", TextureArray );
	}
}