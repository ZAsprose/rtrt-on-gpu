/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#pragma once

#ifndef _KEYBOARD_

#define _KEYBOARD_

#include "Camera.h"

namespace Render
{
	struct KeyState
	{
		bool KeyW;

		bool KeyS;

		bool KeyA;

		bool KeyD;

		bool KeyX;

		bool KeyZ;
	};

	class Keyboard
	{
		private:
			
			KeyState State;

		public:

			//---------------------------------- Base Move Step -----------------------------------

			float Step;

			//----------------------------------- Constructors ------------------------------------
			
			Keyboard ( float = 0.005F );

			//---------------------------------- Event Handlers -----------------------------------

			void StateChange ( int, int );

			//----------------------------------- Camera Change -----------------------------------

			void Apply ( Camera& );
	};
}

#endif