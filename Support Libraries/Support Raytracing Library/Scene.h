#pragma once

#ifndef _SCENE_

#define _SCENE_

#include "Light.h"

#include "Primitive.h"

#include "Volume.h"

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

			//----------------------------------- Virtual Viewer ----------------------------------
			
			Camera * Viewer;

			//----------------------------- Constructor and Destructor ----------------------------
			
			Scene ( Camera * = new Camera ( Vector3D ( 0.0F, 0.0F, -10.0F ) ) );

			~Scene ( void );

			//--------------------------------------- Draw ----------------------------------------
			
			void Draw ( void );
	};
}

#endif