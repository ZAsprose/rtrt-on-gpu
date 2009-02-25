#include "Camera.h"

namespace RenderTools
{
	Camera :: Camera(void)
	{
	}

	Camera :: ~Camera(void)
	{
	}

	//------------------------------------- Private Methods -----------------------------------

	void Camera :: UpdateDirections ( void )
	{
		Matrix3D rotation = Matrix3D :: Rotate ( orientation );

		viewDirection = -rotation * Vector3D :: AxisX;
		
		upDirection = rotation * Vector3D :: AxisZ;
		
		rightDirection = rotation * Vector3D :: AxisY;
	}
}
