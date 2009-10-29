/*
   B A S E   M A T H   L I B R A R Y

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

#ifndef _TRANSFORM_

#define _TRANSFORM_

#include "Matrix3D.h"

namespace Math
{
	class Transform
	{
		private:

			//----------------------------- Transformation Parameters -----------------------------
			
			Vector3D Translation;
			
			Vector3D Orientation;
			
			Vector3D Scale;

			//--------------------------------- Rotation Matrices ---------------------------------
			
			Matrix3D ForwardRotation;

			Matrix3D BackwardRotation;

			//--------------------------------- Scaling Matrices ----------------------------------

			Matrix3D ForwardScaling;
			
			Matrix3D BackwardScaling;

		public:

			//------------------------------------ Constructor ------------------------------------

			Transform ( const Vector3D& = Vector3D :: Zero,
				        const Vector3D& = Vector3D :: Zero,
						const Vector3D& = Vector3D :: Unit );

			//------------------------------ Transformation Functions -----------------------------
			
			Vector3D ForwardPoint ( const Vector3D& position );
			
			Vector3D ForwardVector ( const Vector3D& direction );
			
			Vector3D ForwardNormal ( const Vector3D& normal );
			
			Vector3D BackwardPoint ( const Vector3D& position );
			
			Vector3D BackwardVector ( const Vector3D& direction );

			//--------------------------- Get Transformation Parameters ---------------------------

			Vector3D GetTranslation ( void ) { return Translation; }

			Vector3D GetOrientation ( void ) { return Orientation; }

			Vector3D GetScale ( void ) { return Scale; }

			//--------------------------- Set Transformation Parameters ---------------------------

			void SetTranslation ( const Vector3D& translation );

			void SetOrientation ( const Vector3D& orientation );

			void SetScale ( const Vector3D& scale );
	};
}

#endif