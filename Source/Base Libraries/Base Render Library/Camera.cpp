/*
 * Author: Denis Bogolepov  ( bogdencmc@inbox.ru )
 */

#include "Camera.h"

#include <GL/glfw.h>

#include <math.h>

namespace Render
{
	//--------------------------------------- Constructors ----------------------------------------

	Camera :: Camera ( const Vector3D& position, const Vector3D& orientation )
	{
		Position = position;

		WorldToCamera = Matrix3D :: Rotate ( orientation );

		Update ( );		
	}

	//------------------------------------- Update Directions -------------------------------------
			
	void Camera :: Update ( void )
	{
		CameraToWorld = Transpose ( WorldToCamera );

		Side = Normalize ( CameraToWorld * Vector3D :: AxisX );

		Up = Normalize ( CameraToWorld * Vector3D :: AxisY );

		View = Normalize ( CameraToWorld * Vector3D :: AxisZ );
	}

	//-------------------------------------- Camera Position --------------------------------------

	void Camera :: MoveLocal ( float distance, const Vector3D& direction )
	{
		Vector3D global = CameraToWorld * direction;

		Position += global * distance;
	}
	
	void Camera :: MoveWorld ( float distance, const Vector3D& direction )
	{
		Position += direction * distance;
	}

	//------------------------------------- Camera Orientation ------------------------------------

	void Camera :: RotateLocal ( float angle, const Vector3D& direction )
	{
		WorldToCamera = Matrix3D :: Rotate ( angle, direction ) * WorldToCamera;

		Update ( );
	}
	
	void Camera :: RotateWorld ( float angle, const Vector3D& direction )
	{
		Vector3D local = WorldToCamera * direction;

		WorldToCamera = Matrix3D :: Rotate ( angle, local ) * WorldToCamera;

		Update ( );
	}

	//--------------------------------------- Apply Settings --------------------------------------

	void Camera :: Setup ( void )
	{
		glViewport ( 0, 0, Width, Height );

		//---------------------------------------------------------------------

        glMatrixMode ( GL_PROJECTION );

        glLoadIdentity ( );

        gluPerspective ( Degrees ( FieldOfView ), Aspect, NearPlane, FarPlane );

		//---------------------------------------------------------------------

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

	void Camera :: SetShaderData ( ShaderManager * manager )
	{
		manager->SetUniformVector ( "Camera.Position", Position );
        	
		manager->SetUniformVector ( "Camera.Side", Side );
        	
		manager->SetUniformVector ( "Camera.Up", Up );
        	
		manager->SetUniformVector ( "Camera.View", View );

		manager->SetUniformVector ( "Camera.Scale", GetScreenScale ( ) );
	}

	//------------------------------------ Viewport and Frustum -----------------------------------
	
	void Camera :: SetViewport ( unsigned int width, unsigned int height )
	{
		Width = width;

		Height = height;

		Aspect = Width / ( float ) Height;
	}

	void Camera :: SetFrustum ( float fieldOfView, float nearPlane, float farPlane )
	{
		FieldOfView = fieldOfView;

		NearPlane = nearPlane;

		FarPlane = farPlane;
	}

	//------------------------------------- Getting Settings --------------------------------------
	
	Vector2D Camera :: GetScreenScale ( void )
	{
		return Vector2D ( tanf ( Aspect * FieldOfView / 2.0F ), tanf ( FieldOfView / 2.0F ) );
	}

	Vector2D Camera :: GetPixelSize ( void )
	{
		return Vector2D ( 2.0F / Width, 2.0F / Height );
	}
}