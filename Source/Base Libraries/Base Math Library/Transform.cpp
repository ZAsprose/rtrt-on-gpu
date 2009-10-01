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

#include "Transform.h"

namespace Math
{
	//---------------------------------------- Constructor ----------------------------------------

	Transform :: Transform ( const Vector3D& translation,
		                     const Vector3D& orientation,
							 const Vector3D& scale )
	{
		SetTranslation ( translation );

		SetOrientation ( orientation );

		SetScale ( scale );
	}
	
	//---------------------------------- Transformation Functions ---------------------------------
			
	Vector3D Transform :: ForwardPoint ( const Vector3D& position )
	{
		return ForwardRotation * ForwardScaling * position + Translation;
	}
	
	Vector3D Transform :: ForwardVector ( const Vector3D& vector )
	{
		return ForwardRotation * ForwardScaling * vector;
	}
	
	Vector3D Transform :: ForwardNormal ( const Vector3D& normal )
	{
		return ForwardRotation * BackwardScaling * normal;
	}
	
	Vector3D Transform :: BackwardPoint ( const Vector3D& position )
	{
		return BackwardScaling * BackwardRotation * ( position - Translation );
	}
	
	Vector3D Transform :: BackwardVector ( const Vector3D& direction )
	{
		return BackwardScaling * BackwardRotation * direction;
	} 

	//------------------------------- Set Transformation Parameters -------------------------------
	
	void Transform :: SetTranslation ( const Vector3D& translation )
	{
		Translation = translation;
	}
		
	void Transform :: SetOrientation ( const Vector3D& orientation )
	{
		Orientation = orientation;
		
		ForwardRotation = Matrix3D :: Rotate ( Orientation );
	        	
		BackwardRotation = Inverse ( ForwardRotation );
	}
	
	void Transform :: SetScale ( const Vector3D& scale )
	{
		Scale = scale;

		ForwardScaling = Matrix3D :: Scale ( scale );
		
		BackwardScaling = Inverse ( ForwardScaling );   
	}
}