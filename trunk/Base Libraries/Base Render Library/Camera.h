#pragma once

#ifndef _CAMERA_

#define _CAMERA_

#include <Vector3D.h>

#include <Matrix3D.h>

#include "ShaderManager.h"

using namespace Math;

namespace RenderTools
{
	class Camera
	{
		private:

			//---------------------------------- Camera System ------------------------------------

			Vector3D Position;

			Vector3D Side;

			Vector3D Up;

			Vector3D View;

			//--------------------------------- Camera Transform ----------------------------------

			Matrix3D WorldToCamera;

			Matrix3D CameraToWorld;

			//----------------------------------- View Frustum ------------------------------------
			
			float FieldOfView;

			float NearPlane;			
			
			float FarPlane;

			//------------------------------------- Viewport --------------------------------------
			
			unsigned int Width;

			unsigned int Height;			
			
			float Aspect;

			//--------------------------------- Update Directions ---------------------------------
			
			void Update ( void );
	       
		public:			

			//----------------------------------- Constructors ------------------------------------

			Camera ( const Vector3D& = Vector3D :: Zero, const Vector3D& = Vector3D :: Zero );

			//---------------------------------- Camera Position ----------------------------------

			void MoveLocal ( float, const Vector3D& );
			
			void MoveWorld ( float, const Vector3D& );

			//--------------------------------- Camera Orientation --------------------------------

			void RotateLocal ( float, const Vector3D& );
			
			void RotateWorld ( float, const Vector3D& );

			//-------------------------------- Viewport and Frustum -------------------------------

			void SetViewport ( unsigned int = 512, unsigned int = 512 );

			void SetFrustum ( float = 45.0F, float = 0.1F, float = 1000.0F );

			//----------------------------------- Apply Settings ----------------------------------

			void Setup ( void );

			void SetShaderData ( ShaderManager& );

			//---------------------------------- Getting Settings ---------------------------------

			unsigned int GetWidth ( void ) { return Width; }

			unsigned int GetHeight ( void ) { return Height; }

			float GetFieldOfView ( void ) { return FieldOfView; }

			float GetNearPlane ( void ) { return NearPlane; }

			float GetFarPlane ( void ) { return FarPlane; }

			const Vector3D& GetPosition ( void ) { return Position; }

			const Vector3D& GetSide ( void ) { return Side; }

			const Vector3D& GetUp ( void ) { return Up; }

			const Vector3D& GetView ( void ) { return View; }

			const Matrix3D& GetWorldToCamera ( void ) { return WorldToCamera; }

			const Matrix3D& GetCameraToWorld ( void ) { return CameraToWorld; }
	};
}

#endif