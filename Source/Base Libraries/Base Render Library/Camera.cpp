/*
   Base Render Library   
   Copyright (C) 2009  Denis Bogolepov ( bogdencmc@inbox.ru )

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see http://www.gnu.org/licenses.
 */

#include "Camera.h"

namespace Render
{
	//---------------------------------------- Constructor ----------------------------------------

	Camera :: Camera ( const Vector3D& position, const Vector3D& orientation )
	{
		Position = position;

		WorldToCamera = Matrix3D :: Rotate ( orientation );

		Update ( );		
	}

	//------------------------- Updating Axes of Camera Coordinate System -------------------------
			
	void Camera :: Update ( void )
	{
		CameraToWorld = Transpose ( WorldToCamera );

		Side = Normalize ( CameraToWorld * Vector3D :: AxisX );

		Up = Normalize ( CameraToWorld * Vector3D :: AxisY );

		View = Normalize ( CameraToWorld * Vector3D :: AxisZ );
	}

	//-------------------- Moving Camera in Local and World Coordinate Systems --------------------

	void Camera :: MoveLocal ( float distance, const Vector3D& direction )
	{
		Vector3D global = CameraToWorld * direction;

		Position += global * distance;
	}
	
	void Camera :: MoveWorld ( float distance, const Vector3D& direction )
	{
		Position += direction * distance;
	}

	//------------------- Rotating Camera in Local and World Coordinate Systems -------------------

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

	//-------------------------- Applying Settings to OpenGL and Shaders --------------------------

	void Camera :: Setup ( void )
	{
		glViewport ( 0, 0, Width, Height );

		//-------------------------------------------------------------------------------

        glMatrixMode ( GL_PROJECTION );

        glLoadIdentity ( );

        gluPerspective ( Degrees ( FieldOfView ), Aspect, NearPlane, FarPlane );

		//-------------------------------------------------------------------------------

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

	//------------------------------- Setting Viewport and Frustum --------------------------------
	
	void Camera :: SetViewport ( unsigned width, unsigned height )
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
	
	Vector2D Camera :: GetScreenScale ( void ) const
	{
		return Vector2D ( tanf ( Aspect * FieldOfView / 2.0F ), tanf ( FieldOfView / 2.0F ) );
	}

	Vector2D Camera :: GetPixelSize ( void ) const
	{
		return Vector2D ( 2.0F / Width, 2.0F / Height );
	}
}