/*
 * Author: Denis Bogolepov  ( bogdencmc@inbox.ru )
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

			case GLFW_KEY_LEFT: State.ArrowL = state > 0; break;

			case GLFW_KEY_RIGHT: State.ArrowR = state > 0; break;

			case GLFW_KEY_UP: State.ArrowU = state > 0; break;

			case GLFW_KEY_DOWN: State.ArrowD = state > 0; break;
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

		if ( State.ArrowU ) camera->RotateLocal ( Step, Vector3D :: AxisX );

		if ( State.ArrowD ) camera->RotateLocal ( -Step, Vector3D :: AxisX );

		if ( State.ArrowL ) camera->RotateLocal ( Step, Vector3D :: AxisY );

		if ( State.ArrowR ) camera->RotateLocal ( -Step, Vector3D :: AxisY );
	}
}
