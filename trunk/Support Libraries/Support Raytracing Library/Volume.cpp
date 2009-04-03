#include "Volume.h"

namespace Raytracing
{
	//---------------------------------------- Constructor ----------------------------------------

	Volume :: Volume ( const Vector3D& minimum, const Vector3D& maximum )
	{
		Minimum = minimum;

		Maximum = maximum;
	}
}