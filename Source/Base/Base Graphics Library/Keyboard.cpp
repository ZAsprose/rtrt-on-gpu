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

#include "Keyboard.hpp"

namespace graphics
{
    /************************************************************************/
    /*                      CONSTRUCTOR AND DESTRUCTOR                      */
    /************************************************************************/

    Keyboard :: Keyboard ( GLfloat step )
    {
        Step = step;
    }

    /************************************************************************/
    /*                            PUBLIC METHODS                            */
    /************************************************************************/

    void Keyboard :: KeyDownHandler ( GLint key, GLint state )
    {
        switch ( key )
        {
            case 'W': fState.KeyW = state > 0; break;

            case 'S': fState.KeyS = state > 0; break;

            case 'A': fState.KeyA = state > 0; break;

            case 'D': fState.KeyD = state > 0; break;

            case 'X': fState.KeyX = state > 0; break;

            case 'Z': fState.KeyZ = state > 0; break;
        }
    }

    /************************************************************************/

    void Keyboard :: Apply ( Camera * camera, GLfloat fps )
    {
        if ( fState.KeyW ) camera->SetPosition (
            camera->Position ( ) + camera->View ( ) * ( Step / fps ) );

        if ( fState.KeyS ) camera->SetPosition (
            camera->Position ( ) - camera->View ( ) * ( Step / fps ) );

        if ( fState.KeyA ) camera->SetPosition (
            camera->Position ( ) + camera->Side ( ) * ( Step / fps ) );

        if ( fState.KeyD ) camera->SetPosition (
            camera->Position ( ) - camera->Side ( ) * ( Step / fps ) );

        if ( fState.KeyX ) camera->SetPosition (
            camera->Position ( ) + camera->Up ( ) * ( Step / fps ) );

        if ( fState.KeyZ ) camera->SetPosition (
            camera->Position ( ) - camera->Up ( ) * ( Step / fps ) );
    }
}