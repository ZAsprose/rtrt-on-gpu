/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#pragma once

#ifndef _TEXTURE_MANAGER_

#define _TEXTURE_MANAGER_

#include <vector>

#include <Texture1D.h>

#include <Texture2D.h>

#include <Texture3D.h>

#include <ShaderManager.h>

using namespace Render;

namespace Raytracing
{
	class TextureManager
	{
		public:

			//---------------------------------- Raster Textures ----------------------------------

			vector < Texture2D * > Textures;

			//----------------------------- Load Static Texture Data ------------------------------

			void SetupTextures ( void );

			//---------------------------------- Apply Settings -----------------------------------

			void SetShaderData ( ShaderManager * );
	};
}

#endif