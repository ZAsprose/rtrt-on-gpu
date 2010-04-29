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

#include "Mouse.hpp"

namespace graphics
{
    /************************************************************************/
    /*                      CONSTRUCTOR AND DESTRUCTOR                      */
    /************************************************************************/

    Mouse :: Mouse ( float step, Vector3f axisX, Vector3f axisY )
    {
        Step = step;

        AxisX = axisX;

        AxisY = axisY;
    }

    /************************************************************************/
    /*                            PUBLIC METHODS                            */
    /************************************************************************/

    void Mouse :: MouseDownHandler ( GLint button, GLint state )
    {
        glfwGetMousePos ( &fLocationX, &fLocationY );

        fActive = state > 0;
    }

    /************************************************************************/
    
    void Mouse :: MouseMoveHandler ( GLint x, GLint y )
    {
        if ( fActive )
        {
            fDeltaX += x - fLocationX;
            fDeltaY += y - fLocationY;
            
            fLocationX = x;            
            fLocationY = y;
        }
    }

    /************************************************************************/

    void Mouse :: Apply ( Camera * camera, float fps )
    {
        Vector3f delta =
            fDeltaY * ( Step / ( fps + 1E4F ) ) * AxisY -
            fDeltaX * ( Step / ( fps + 1E4F ) ) * AxisX;

        camera->SetOrientation ( camera->Orientation ( ) + delta );

        fDeltaX = fDeltaY = 0;
    }
}