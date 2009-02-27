#pragma once

#include <GL/glfw.h>

#include "Vector3D.h"

#include "Matrix3D.h"

using namespace Math;

namespace RenderTools
{
	class Camera
	{
		private:

			//----------------------------------- Camera System -----------------------------------

			Vector3D Position;

			Vector3D Side;

			Vector3D Up;

			Vector3D View;

			//----------------------------------- View Frustum ------------------------------------
			
			float FieldOfView;

			float NearPlane;			
			
			float FarPlane;

			//------------------------------------- Viewport --------------------------------------
			
			int Width;

			int Height;			
			
			float Aspect;

			//---------------------------------- Private Methods ----------------------------------

			void Update ( void );
	       
		public:			

			//----------------------------------- Constructors ------------------------------------

			Camera( void );

			Camera( const Vector3D& );

			Camera( const Vector3D&, const Vector3D& );

			//---------------------------------- Camera Position ----------------------------------

			void MoveLocal ( float, const Vector3D& );
			
			void MoveGlobal ( float, const Vector3D& );

			//--------------------------------- Camera Orientation --------------------------------

			void RotateLocal ( float, const Vector3D& );
			
			void RotateGlobal ( float, const Vector3D& );

			//----------------------------------- Apply Settings ----------------------------------

			void Setup ( void );
	};
}
