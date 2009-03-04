#include "Camera.h"

#include <iostream>

namespace RenderTools
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
	
	//------------------------------------ Viewport and Frustum -----------------------------------
	
	void Camera :: SetViewport ( int width, int height )
	{
		Width = width;

		Height = height;

		Aspect = Width / ( float) Height;

		glViewport( DEFAULT_LEFT, DEFAULT_TOP, Width, Height );
	}

	void Camera :: SetFrustum ( float angle, float near, float far )
	{
		FieldOfView = angle;

		NearPlane = near;

		FarPlane = far;

        glMatrixMode( GL_PROJECTION );

        glLoadIdentity();

        gluPerspective( FieldOfView, Aspect, NearPlane, FarPlane );
		
		glMatrixMode( GL_MODELVIEW );
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

	void Camera :: SetShaderData ( ShaderManager& manager )
	{
		manager.SetUniformVector ( "Camera.Position", Position );
        	
		manager.SetUniformVector ( "Camera.Side", Side );
        	
		manager.SetUniformVector ( "Camera.Up", Up );
        	
		manager.SetUniformVector ( "Camera.View", View );
	}
	
	//-------------------------------------- Getting Settings -------------------------------------

	void Camera :: GetViewport ( int& width, int& height )
	{
		width = Width;

		height = Height;
	}

	void Camera :: GetFrustum ( float& angle, float& near, float& far )
	{
		angle = FieldOfView;

		near = NearPlane;

		far = FarPlane;
	}

	void Camera :: GetPosition ( Vector3D& position )
	{
		position = Position;
	}

	void Camera :: GetDirections ( Vector3D& side, Vector3D& up, Vector3D& view )
	{
		side = Side;

		up = Up;

		view = View;
	}

	void Camera :: GetTransformations ( Matrix3D& worldcamera, Matrix3D& cameraworld )
	{
		worldcamera = WorldToCamera;

		cameraworld = CameraToWorld;
	}
}
