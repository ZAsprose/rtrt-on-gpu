/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#pragma once

#ifndef _MOUSE_

#define _MOUSE_

#include "Camera.h"

namespace Render
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

			int Button;

		public:

			//-------------------------------- Base Rotation Angle --------------------------------

			float Step;

			//----------------------------------- Constructors ------------------------------------
			
			Mouse ( float = 0.005F );

			//---------------------------------- Event Handlers -----------------------------------
			
			void StateChange ( int, int );
			
			void MouseMove ( int, int );

			//----------------------------------- Camera Change -----------------------------------

			void Apply ( Camera * );
	};
}

#endif