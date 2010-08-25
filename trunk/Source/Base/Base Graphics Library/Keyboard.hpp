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

#ifndef _KEYBOARD_
#define _KEYBOARD_

#include "Camera.hpp"

namespace graphics
{
    struct KeyboardState
    {
        bool KeyW;

        bool KeyS;

        bool KeyA;

        bool KeyD;

        bool KeyX;

        bool KeyZ;
    };

    class Keyboard
    {
        public:

            /************************************************************************/
            /*                            PUBLIC FIELDS                             */
            /************************************************************************/

            /* Defines keyboard sensitivity */

            GLfloat Step;

            /************************************************************************/
            /*                      CONSTRUCTOR AND DESTRUCTOR                      */
            /************************************************************************/

            Keyboard ( GLfloat step = 2E1F );

            /************************************************************************/
            /*                            PUBLIC METHODS                            */
            /************************************************************************/

            /* Here we use GLFW library for handling keyboard events */

            void KeyDownHandler ( GLint key, GLint state );

            //------------------------------------------------------------------------

            /* You can set current FPS for adaptive keyboard sensitivity */

            void Apply ( Camera * camera, GLfloat fps = 100.0F );

        private:

            /************************************************************************/
            /*                            PRIVATE FIELDS                            */
            /************************************************************************/

            KeyboardState fState;
    };
}

#endif