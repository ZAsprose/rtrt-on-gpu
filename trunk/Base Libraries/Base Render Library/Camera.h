#pragma once

#include "Vector3D.h"

#include "Matrix3D.h"

using namespace Math;

namespace RenderTools
{
	class Camera
	{
		private:

			//---------------------------------- Private Fields -----------------------------------

			Vector3D position;
	        
			Vector3D orientation;
			
			Vector3D upDirection;
			
			Vector3D rightDirection;
			
			Vector3D viewDirection;

			//---------------------------------- Private Methods ----------------------------------

			void UpdateDirections ( void );
	       
		public:

			//----------------------------------- Public Fields -----------------------------------
			
			int Width;
			
			int Height;
			
			float FieldOfView;

			//----------------------------------- Constructors ------------------------------------

			Camera(void);
			
			~Camera(void);

			//--------------------------------- Common Functions ----------------------------------
	};
}
