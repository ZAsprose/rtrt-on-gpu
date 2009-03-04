#pragma once

#include "Camera.h"

namespace RenderTools
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
