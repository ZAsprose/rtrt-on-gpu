/*
   -----------------------------------------------------------------------------
   |                   B A S E   R E N D E R   L I B R A R Y                   |
   -----------------------------------------------------------------------------

                              Denis Bogolepov ( bogdencmc @ inbox.ru )
   Copyright (c) 2009 - 2010
                              Andrey Morozov ( morozov.andrey.vmk @ gmail.com )

   This library is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free Software
   Foundation, either version 3 of the License, or (at your option) any later
   version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with
   this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MOUSE_HPP_

#define _MOUSE_HPP_

#include "Camera.hpp"

namespace render
{
	/**
	 * Auxiliary class for controling of the camera by the mouse ( changes orientation ).
	 */
	class Mouse
	{
		protected:

			/**
			 * Current \e X coordinate of mouse location. 
			 */
			int locationX;
			
			/**
			 * Current \e Y coordinate of mouse location. 
			 */
			int locationY;

			//---------------------------------------------------------------------------

			/**
			 * Difference between last two positions of the mouse along \e X axis.
			 */
			int deltaX;

			/**
			 * Difference between last two positions of the mouse along \e Y axis.
			 */			
			int deltaY;

			//---------------------------------------------------------------------------

			/**
			 * Defines whether the mouse is \e active and used for changing camera
			 * orientation. By default the mouse is active when any of its buttons
			 * is pressed.
			 */
			bool active;

		public:
			
			/**
			 * Minimal rotation angle of the camera. The given angle corresponds to
			 * moving of the mouse by \e one pixel.
			 */
			float step;

			//---------------------------------------------------------------------------
			
			/**
			 * 
			 */
			Mouse ( float step = 0.005F );

			//---------------------------------------------------------------------------
			
			/**
			 *
			 */
			void StateChange ( int button, int state );
			
			/**
			 *
			 */
			void MouseMove ( int x, int y );

			//---------------------------------------------------------------------------

			/**
			 *
			 */
			void Apply ( Camera * camera );
	};

}

#endif
