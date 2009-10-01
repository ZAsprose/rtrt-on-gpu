/*
   B A S E   R E N D E R   L I B R A R Y

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

#pragma once

#ifndef _CAMERA_

#define _CAMERA_

#include "ShaderManager.h"

using namespace Math;

namespace Render
{
	class Camera
	{
		private:

			//------------------- Position and Axes of Camera Coordinate System -------------------

			Vector3D Position;

			Vector3D Side;

			Vector3D Up;

			Vector3D View;

			//------------------ Transform Matrices World Space <-> Camera Space ------------------

			Matrix3D WorldToCamera;

			Matrix3D CameraToWorld;

			//------------------------------- View Frustum Settings -------------------------------
			
			float FieldOfView;

			float NearPlane;			
			
			float FarPlane;

			//--------------------------------- Viewport Settings ---------------------------------
			
			unsigned Width;

			unsigned Height;			
			
			float Aspect;

			//--------------------- Updating Axes of Camera Coordinate System ---------------------
			
			void Update ( void );
	       
		public:			

			//------------------------------------ Constructor ------------------------------------

			Camera ( const Vector3D& = Vector3D :: Zero,
				     const Vector3D& = Vector3D :: Zero );

			//---------------- Moving Camera in Local and World Coordinate Systems ----------------

			void MoveLocal ( float, const Vector3D& );
			
			void MoveWorld ( float, const Vector3D& );

			//--------------- Rotating Camera in Local and World Coordinate Systems ---------------

			void RotateLocal ( float, const Vector3D& );
			
			void RotateWorld ( float, const Vector3D& );

			//------------------------- Loading and Saving Camera to File -------------------------

			void SaveToFile ( const char * path );

			void LoadFromFile ( const char * path );

			//---------------------- Applying Settings to OpenGL and Shaders ----------------------

			void Setup ( void );

			void SetShaderData ( ShaderManager * );

			//--------------------------- Setting Viewport and Frustum ----------------------------

			void SetViewport ( unsigned = 512, unsigned = 512 );

			void SetFrustum ( float = ONE_PI / 3.0F, float = 1E-3F, float = 1E6F );

			//--------------------------------- Getting Settings ----------------------------------

			unsigned GetWidth ( void ) const { return Width; }

			unsigned GetHeight ( void ) const { return Height; }

			float GetFieldOfView ( void ) const { return FieldOfView; }

			float GetNearPlane ( void ) const { return NearPlane; }

			float GetFarPlane ( void ) const { return FarPlane; }

			const Vector3D& GetPosition ( void ) const { return Position; }

			const Vector3D& GetSide ( void ) const { return Side; }

			const Vector3D& GetUp ( void ) const { return Up; }

			const Vector3D& GetView ( void ) const { return View; }

			const Matrix3D& GetWorldToCamera ( void ) const { return WorldToCamera; }

			const Matrix3D& GetCameraToWorld ( void ) const { return CameraToWorld; }

			Vector2D GetScreenScale ( void ) const;
	};
}

#endif