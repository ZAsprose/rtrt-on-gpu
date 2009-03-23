#pragma once

#ifndef _LIGHT_SOURCE_

#define _LIGHT_SOURCE_

#include <Vector3D.h>

using namespace Math;

namespace Raytracing
{
	class Light
	{
		public:

			//------------------------------------- Intensity -------------------------------------

			Vector3D Ambient;

			Vector3D Diffuse;

			Vector3D Specular;

			//------------------------------------- Position --------------------------------------
	        
			Vector3D Position;

			//-------------------------------------- Number ---------------------------------------

			unsigned int Number;

			//----------------------------- Constructor and Destructor ----------------------------

			Light ( unsigned int = 0,
				    const Vector3D& = Vector3D ( 10.0F, 10.0F, 10.0F ),
					const Vector3D& = Vector3D ( 0.2F, 0.2F, 0.2F ),
					const Vector3D& = Vector3D ( 1.0F, 1.0F, 1.0F ),
					const Vector3D& = Vector3D ( 0.5F, 0.5F, 0.5F ) );

			~Light ( void );

			//----------------------------------- Apply Settings ----------------------------------

			void Setup ( void );

			//--------------------------------------- Draw ----------------------------------------

			void Draw ( void );
	};
}

#endif
