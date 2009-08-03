/*
   Base Render Library   
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

#include "Noise2D.h"

namespace Render
{
	//---------------------------------- Constructor and Destructor -------------------------------
	
	Noise2D :: Noise2D ( int size )
	{
		Size = size;

		//---------------------------------------------------------------------

		Values = new float * [Size + 1];

		for ( int index = 0; index <= Size; index++ )
		{
			Values [index] = new float [Size + 1];
		}

		//---------------------------------------------------------------------

		for ( int x = 0; x <= Size; x++ )
		{
			for ( int y = 0; y <= Size; y++ )
			{
				Values [x][y] = ( float ) rand ( ) / ( RAND_MAX + 1 ) * 2.0F - 1.0F;
			}
		}
	}

	Noise2D :: ~Noise2D ( )
	{
		for ( int index = 0; index <= Size; index++ )
		{
			delete [] Values [index];
		}

		delete [] Values;
	}

	//---------------------------------- Calculating Noise Value ----------------------------------

	float Noise2D :: Noise ( float x, float y )
	{
		int XS = ( int ) floorf ( x );

		int YS = ( int ) floorf ( y );

		float XF = Fract ( x );

		float YF = Fract ( y );

		float D = Mix ( Values [XS][YS], Values [XS + 1][YS], Smooth ( XF, 0.0F, 1.0F ) );

		float U = Mix ( Values [XS][YS + 1], Values [XS + 1][YS + 1], Smooth ( XF, 0.0F, 1.0F ) );

		return Mix ( D, U, Smooth ( YF, 0.0F, 1.0F ) );
	}
	
	//-------------------------------- Building Noise Texture Data --------------------------------
	
	TextureData2D * Noise2D :: BuildData ( int width, int height )
	{
		TextureData2D * data = new TextureData2D ( width, height, 1 );

		for ( int s = 0; s < width; s++ )
		{
			float x = s / ( float ) width * Size;

			for ( int t = 0; t < height; t++ )
			{
				float y = t / ( float ) height * Size;

				data->Pixel <float> ( s, t ) = Noise ( x, y );
			}
		}

		return data;
	}
}