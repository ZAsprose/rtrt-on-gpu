/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "Mouse.h"

#include <GL/glfw.h>

namespace Render
{
	//--------------------------------------- Constructors ----------------------------------------
	
	Mouse :: Mouse ( float step )
	{
		Step = step;
	}

	//-------------------------------------- Event Handlers ---------------------------------------
	
	void Mouse :: StateChange ( int state )
	{
		glfwGetMousePos ( &LocationX, &LocationY );

		Active = state > 0;
	}
	
	void Mouse :: MouseMove ( int x, int y )
	{
		if ( Active )
		{
			if ( LocationX != x )
			{
				DeltaX += x - LocationX;

				LocationX = x;
			}
			
			if ( LocationY != y )
			{
				DeltaY += y - LocationY;

				LocationY = y;
			}					
		}
	}

	//--------------------------------------- Camera Change ---------------------------------------

	void Mouse :: Apply ( Camera * camera )
	{
		if ( DeltaX != 0 )
		{
			camera->RotateLocal ( -DeltaX * Step, Vector3D :: AxisY );

			DeltaX = 0;
		}
		
		if ( DeltaY != 0 )
		{
			camera->RotateLocal ( DeltaY * Step, Vector3D :: AxisX );

			DeltaY = 0;
		}
	}
}