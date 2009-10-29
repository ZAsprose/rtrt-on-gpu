/*
   S U P P O R T   R A Y   T R A C I N G   L I B R A R Y

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

#ifndef _LIGHT_SOURCE_

#define _LIGHT_SOURCE_

#include <ShaderManager.h>

using namespace Render;

namespace Raytracing
{
	class Light
	{
		public:

			//------------------------------- Intensity Components --------------------------------

			Vector3D Ambient;

			Vector3D Diffuse;

			Vector3D Specular;

			//---------------- Constant, Linear and Quadratic Attenuation Factors -----------------
	        
			Vector3D Attenuation;

			//------------------------------- Light Source Position -------------------------------
	        
			Vector3D Position;

			//------------------------- Light Source Number in OpenGL API -------------------------

			unsigned Number;

			//----------------------------- Constructor and Destructor ----------------------------

			Light ( unsigned = 0,
				    const Vector3D& = Vector3D ( 10.0F, 10.0F, 10.0F ),
					const Vector3D& = Vector3D ( 0.1F, 0.1F, 0.1F ),
					const Vector3D& = Vector3D ( 0.9F, 0.9F, 0.9F ),
					const Vector3D& = Vector3D ( 0.6F, 0.6F, 0.6F ),
					const Vector3D& = Vector3D ( 1.5F, 0.6F, 0.3F ) );

			~Light ( void );

			//---------------------- Applying Settings to OpenGL and Shaders ----------------------

			void Setup ( void );
			
			void SetShaderData ( ShaderManager * manager );

			//------------------------ Drawing Light Source in OpenGL Mode ------------------------

			void Draw ( void );
	};
}

#endif
