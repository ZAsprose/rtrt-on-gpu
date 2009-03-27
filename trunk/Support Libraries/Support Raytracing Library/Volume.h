#pragma once

#ifndef _VOLUME_

#define _VOLUME_

#include <Vector3D.h>

using namespace Math;

namespace Raytracing
{
	class Volume
	{
		public:
			
			Vector3D Minimum;
		
			Vector3D Maximum;
			
			Vector3D Color;
			
			bool Visible;		
			
			Volume ( void );
			
			~Volume ( void );
	};
}

#endif
