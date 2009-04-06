/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

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

			//-------------------------------- Volume End Vertices --------------------------------
			
			Vector3D Minimum;
		
			Vector3D Maximum;

			//----------------------------- Lines Color and Visibility ----------------------------
			
			Vector3D Color;
			
			bool Visible;

			//------------------------------------ Constructor ------------------------------------
			
			Volume ( const Vector3D& = Vector3D ( -5.0F, -5.0F, -5.0F ),
				     const Vector3D& = Vector3D ( 5.0F, 5.0F, 5.0F ) );
	};
}

#endif