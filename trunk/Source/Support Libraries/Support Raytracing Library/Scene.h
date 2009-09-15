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

#ifndef _SCENE_

#define _SCENE_

#include "Light.h"

#include "Primitive.h"

#include "ProximityGrid.h"

#include "Volume.h"

#include <Camera.h>

#include <vector>

using namespace Render;

namespace Raytracing
{
	class Scene
	{
		public:

			//-------------------------------- List of Primitives ---------------------------------
			
			vector <Light *> Lights;

			//------------------------------- List of Light Sources -------------------------------
			
			vector <Primitive *> Primitives;

			//---------------------------- List of Raster Texture Data ----------------------------

			vector <TextureData2D *> TextureData;

			//---------------------------------- Virtual Viewer -----------------------------------
			
			Camera * Viewer;

			//-------------------------------- Scene Bounding Box ---------------------------------

			Volume * Box;

			//-------------------- Acceleration Structure Based on Uniform Grid -------------------
			
			UniformGrid * Grid;

			//----------------------------- Constructor and Destructor ----------------------------
			
			Scene ( Camera * = new Camera ( ), Volume * = new Volume ( ) );

			~Scene ( void );

			//---------------------------- Drawing Scene in OpenGL Mode ---------------------------
			
			void Draw ( void );

			//--------------------------- Building Acceleration Structure -------------------------
			
			void BuildGrid ( int, int, int, bool = false );

			//---------------------------- Applying Settings to Shaders ---------------------------

			void SetShaderData ( ShaderManager * );
	};
}

#endif