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

#pragma once

#ifndef _TEXTURE_MANAGER_

#define _TEXTURE_MANAGER_

#include <vector>

#include <ShaderManager.h>

using namespace Render;

namespace Raytracing
{
	class TextureManager
	{
		private:

			//---------------------- 3D Texture Used as Array of 2D Textures ----------------------

			Texture3D * TextureArray;

			//------------------ Fetching Texture Data with Bilinear Filtration -------------------

			Vector3D FilterData ( TextureData2D *, float, float );

		public:

			//-------------------------------- Constant Parameters --------------------------------

			static const unsigned TextureUnit;

			//------------------------------------ Destructor -------------------------------------

			~TextureManager  ( void );

			//------------------------------- List of Texture Data --------------------------------

			vector <TextureData2D *> TextureData;

			//---------------------- Applying Settings to OpenGL and Shaders ----------------------

			void SetupTextures ( void );

			void SetShaderData ( ShaderManager * );
	};
}

#endif