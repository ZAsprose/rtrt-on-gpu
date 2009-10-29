/*
   S U P P O R T   R A Y   T R A C I N G   L I B R A R Y

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

#ifndef _STATIC_DATA_

#define _STATIC_DATA_

#include <Texture1D.h>

#include <Texture2D.h>

#include <Texture3D.h>

#include "UniformGrid.h"

#include "Scene.h"

using namespace Render;

namespace Raytracing
{
	class StaticData
	{
		private:

			//------------------ Fetching Texture Data with Bilinear Filtration -------------------

			Vector3D FilterData ( TextureData2D *, float, float );

		public:

			//-------------------------------- Constant Parameters --------------------------------

			static const unsigned VertexSize;

			static const unsigned MaterialSize;

			static const unsigned VoxelUnit;

			static const unsigned PositionUnit;

			static const unsigned NormalUnit;

			static const unsigned TexCoordUnit;

			static const unsigned MaterialUnit;
			
			static const unsigned TextureUnit;

			//----------------------------------- Data Textures -----------------------------------

			Texture3D * VoxelTexture;

			Texture2D * PositionTexture;

			Texture2D * NormalTexture;

			Texture2D * TexCoordTexture;

			Texture1D * MaterialTexture;

			//---------------------- 3D Texture Used as Array of 2D Textures ----------------------

			Texture3D * TextureArray;

			//---------------------------- Constructor and Destructor -----------------------------

			StaticData ( void );

			~StaticData ( void );

			//---------------------- Building Static Data for Specified Scene ---------------------

			void BuildData ( Scene * );

			//---------------------------- Applying Settings to Shaders ---------------------------

			void SetShaderData ( ShaderManager * );
	};
}

#endif