/*
   -----------------------------------------------------------------------------
   |                 B A S E   G R A P H I C S   L I B R A R Y                 |
   -----------------------------------------------------------------------------
                              
   Copyright (c) 2009 - 2010 Denis Bogolepov ( denisbogol @ gmail.com )

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

#ifndef _MOUSE_
#define _MOUSE_

#include "Camera.hpp"

namespace graphics
{
    class Mouse
    {
        public:

            /************************************************************************/
            /*                            PUBLIC FIELDS                             */
            /************************************************************************/

            /* Defines mouse sensitivity */
            
            GLfloat Step;

            //------------------------------------------------------------------------

            /* Axes for rotating camera when mouse is moving in X and Y directions */

            Vector3f AxisX;

            Vector3f AxisY;

            /************************************************************************/
            /*                      CONSTRUCTOR AND DESTRUCTOR                      */
            /************************************************************************/

            Mouse ( GLfloat step = 6E1F,
                    Vector3f axisX = Vector3f :: UnitY ( ),
                    Vector3f axisY = Vector3f :: UnitX ( ) );

            /************************************************************************/
            /*                            PUBLIC METHODS                            */
            /************************************************************************/

            /* Here we use GLFW library for handling mouse events */

            void MouseDownHandler ( GLint button, GLint state );

            void MouseMoveHandler ( GLint x, GLint y );

            //------------------------------------------------------------------------

            /* You can set current FPS for adaptive mouse sensitivity */

            void Apply ( Camera * camera, GLfloat fps = 100.0F );

        private:

            /************************************************************************/
            /*                            PRIVATE FIELDS                            */
            /************************************************************************/

            GLint fLocationX;

            GLint fLocationY;

            //------------------------------------------------------------------------

            GLint fDeltaX;

            GLint fDeltaY;

            //------------------------------------------------------------------------

            /* Is mouse used for rotating a camera? */

            bool fActive;
    };
}

#endif
