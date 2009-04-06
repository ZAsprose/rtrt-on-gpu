/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "Voxel.h"

namespace Raytracing
{
	//---------------------------------------- Constructor ----------------------------------------

	Voxel :: Voxel ( const Vector3D& position, const Vector3D& radius )
	{
		Position = position;

		Radius = radius;
	}
}