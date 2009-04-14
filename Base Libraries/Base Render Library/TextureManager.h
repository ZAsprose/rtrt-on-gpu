/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#pragma once

#ifndef _TEXTURE_MANAGER_

#define _TEXTURE_MANAGER_

#include <vector>

#include "Texture1D.h"

#include "Texture2D.h"

#include "Texture3D.h"

#include "ShaderManager.h"

namespace Render
{
	class TextureManager
	{
		public:

			//---------------------------------- Image Textures -----------------------------------

			vector < Texture2D * > ImageTextures;

			//---------------------------- Constructor and Destructor -----------------------------

			TextureManager ( void );

			~TextureManager ( void );

			//----------------------------- Load Static Texture Data ------------------------------

			void SetupTextures ( void );

			//---------------------------------- Apply Settings -----------------------------------

			void SetShaderData ( ShaderManager * );
	};
}

#endif