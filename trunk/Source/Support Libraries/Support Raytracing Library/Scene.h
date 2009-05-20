/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#pragma once

#ifndef _SCENE_

#define _SCENE_

#include "Light.h"

#include "Primitive.h"

#include "Volume.h"

//#include "UniformGrid.h"
#include "ProximityGrid.h"

#include <Camera.h>

#include <vector>

using namespace std;

using namespace Render;

namespace Raytracing
{
	class Scene
	{
		public:

			//---------------------------- Primitives and Light Sources ---------------------------
			
			vector < Light * > Lights;
			
			vector < Primitive * > Primitives;

			//---------------------------------- Virtual Viewer -----------------------------------
			
			Camera * Viewer;

			//-------------------------------- Scene Bounding Box ---------------------------------

			Volume * Box;

			//----------------------------------- Uniform Grid ------------------------------------
			
			UniformGrid * Grid;

			//----------------------------- Constructor and Destructor ----------------------------
			
			Scene ( Camera * = new Camera ( ), Volume * = new Volume ( ) );

			~Scene ( void );

			//--------------------------------------- Draw ----------------------------------------
			
			void Draw ( void );

			//-------------------------------- Build Uniform Grid ---------------------------------
			
			void BuildGrid ( int, int, int );

			//---------------------------------- Apply Settings -----------------------------------

			void SetShaderData ( ShaderManager * );
	};
}

#endif