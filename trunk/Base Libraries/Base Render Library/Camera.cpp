#include "Camera.h"

namespace RenderTools
{
	//--------------------------------------- Constructors ----------------------------------------

	Camera :: Camera( void )
	{
		Position = Vector3D :: Zero;

		Side = Vector3D :: AxisX;

		Up = Vector3D :: AxisY;

		View = -Vector3D :: AxisZ;
	}

	Camera :: Camera( const Vector3D& position )
	{
		Position = position;

		Side = Vector3D :: AxisX;

		Up = Vector3D :: AxisY;

		View = -Vector3D :: AxisZ;
	}

	Camera :: Camera( const Vector3D& position, const Vector3D& orientation )
	{
		Position = position;

		Matrix3D rotation = Matrix3D :: Rotate ( orientation );
		
		Side = rotation * Vector3D :: AxisX;

		Up = rotation * Vector3D :: AxisY;

		View = -rotation * Vector3D :: AxisZ;
	}

	//-------------------------------------- Camera Position --------------------------------------

	void Camera :: MoveLocal ( float distance, const Vector3D& direction )
	{

	}
	
	void Camera :: MoveGlobal ( float distance, const Vector3D& direction )
	{
		Position += direction * distance;
	}

	//------------------------------------- Camera Orientation ------------------------------------

	void Camera :: RotateLocal ( float angle, const Vector3D& direction )
	{

	}
	
	void Camera :: RotateGlobal ( float angle, const Vector3D& direction )
	{
		Matrix3D rotation = Matrix3D :: Rotate ( angle, direction );
		
		Side = rotation * Vector3D :: AxisX;

		Up = rotation * Vector3D :: AxisY;

		View = -rotation * Vector3D :: AxisZ;
	}

	//--------------------------------------- Apply Settings --------------------------------------

	void Camera :: Setup ( void )
	{
		glMatrixMode ( GL_MODELVIEW );
		
		glLoadIdentity ( );

		gluLookAt ( Position.X,
			        Position.Y,
					Position.Z,
			        Position.X + View.X,
					Position.Y + View.Y,
					Position.Z + View.Z,
			        Up.X,
					Up.Y,
					Up.Z );
	}
}
