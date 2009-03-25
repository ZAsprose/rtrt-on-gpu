#include "Voxel.h"

namespace Raytracing
{
	//---------------------------------------- Constructor ----------------------------------------

	Voxel :: Voxel ( const Vector3D& position, const Vector3D& halfSize )
	{
		Position = position;

		HalfSize = halfSize;
	}
}