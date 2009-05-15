/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "Keyboard.h"

namespace Render
{
	//--------------------------------------- Constructors ----------------------------------------
	
	Keyboard :: Keyboard ( float step )
	{
		Step = step;
	}

	//-------------------------------------- Event Handlers ---------------------------------------

	void Keyboard :: StateChange ( int key, int state )
	{
		switch ( key )
		{
			case 'W': State.KeyW = state > 0; break;

			case 'S': State.KeyS = state > 0; break;

			case 'A': State.KeyA = state > 0; break;

			case 'D': State.KeyD = state > 0; break;

			case 'X': State.KeyX = state > 0; break;

			case 'Z': State.KeyZ = state > 0; break;

			case GLFW_KEY_LEFT: State.KeyLeft = state > 0; break;

			case GLFW_KEY_RIGHT: State.KeyRight = state > 0; break;

			case GLFW_KEY_UP: State.KeyUp = state > 0; break;

			case GLFW_KEY_DOWN: State.KeyDown = state > 0; break;
		}
	}

	//--------------------------------------- Camera Change ---------------------------------------

	void Keyboard :: Apply ( Camera * camera )
	{
		if ( State.KeyW ) camera->MoveLocal ( Step, Vector3D :: AxisZ );

		if ( State.KeyS ) camera->MoveLocal ( -Step, Vector3D :: AxisZ );

		if ( State.KeyA ) camera->MoveLocal ( Step, Vector3D :: AxisX );

		if ( State.KeyD ) camera->MoveLocal ( -Step, Vector3D :: AxisX );

		if ( State.KeyX ) camera->MoveLocal ( Step, Vector3D :: AxisY );

		if ( State.KeyZ ) camera->MoveLocal ( -Step, Vector3D :: AxisY );

		if ( State.KeyLeft ) camera->RotateLocal ( 2*Step, Vector3D :: AxisY );

		if ( State.KeyRight ) camera->RotateLocal ( -2*Step, Vector3D :: AxisY );
	}
}
