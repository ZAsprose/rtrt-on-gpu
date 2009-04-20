/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
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
	
	//------------------------------ Transformation Functions -----------------------------
			
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