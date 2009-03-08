#pragma once

#ifndef _MOUSE_

#define _MOUSE_

#include "Camera.h"

namespace RenderTools
{
	class Mouse
	{
		private:

			//---------------------------------- Mouse Location -----------------------------------
			
			int LocationX;
			
			int LocationY;

			//-------------------------------- Location Difference --------------------------------

			int DeltaX;
			
			int DeltaY;

			//-------------------------------- Mouse Activity Flag --------------------------------

			bool Active;

		public:

			//-------------------------------- Base Rotation Angle --------------------------------

			float Step;

			//----------------------------------- Constructors ------------------------------------
			
			Mouse ( float = 0.005F );

			//---------------------------------- Event Handlers -----------------------------------
			
			void StateChange ( int );
			
			void MouseMove ( int, int );

			//----------------------------------- Camera Change -----------------------------------

			void Apply ( Camera& );
	};
}

#endif