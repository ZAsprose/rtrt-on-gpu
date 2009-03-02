#pragma once

#include <GL/glfw.h>

#include <Vector3D.h>

#include <Matrix3D.h>

using namespace Math;

//--------------------------------------- Useful Constants ----------------------------------------

#define DEFAULT_WIDTH 512

#define DEFAULT_HEIGHT 512

#define DEFAULT_LEFT 0

#define DEFAULT_TOP 0

#define DEFAULT_FOV 45.0F

#define DEFAULT_NEAR 0.1F

#define DEFAULT_FAR 1000.0F

//-------------------------------------------------------------------------------------------------

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
			
			int Width;

			int Height;			
			
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

			void SetViewport ( int = DEFAULT_WIDTH, int = DEFAULT_HEIGHT );

			void SetFrustum ( float = DEFAULT_FOV, float = DEFAULT_NEAR, float = DEFAULT_FAR );

			//----------------------------------- Apply Settings ----------------------------------

			void Setup ( void );

			//---------------------------------- Getting Settings ---------------------------------

			void GetViewport ( int&, int& );

			void GetFrustum ( float&, float&, float& );

			void GetPosition ( Vector3D& );

			void GetDirections ( Vector3D&, Vector3D&, Vector3D& );

			void GetTransformations ( Matrix3D&, Matrix3D& );
	};
}
