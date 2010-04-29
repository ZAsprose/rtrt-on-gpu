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

#include "WrapMode.hpp"

namespace graphics
{
    /************************************************************************/
    /*                            PUBLIC FIELDS                             */
    /************************************************************************/
    
    const WrapMode WrapMode :: Clamp ( GL_CLAMP, GL_CLAMP, GL_CLAMP );
    
    const WrapMode WrapMode :: Repeat ( GL_REPEAT, GL_REPEAT, GL_REPEAT );

    /************************************************************************/
    /*                      CONSTRUCTOR AND DESTRUCTOR                      */
    /************************************************************************/        

    WrapMode :: WrapMode ( GLint s, GLint t, GLint r )
    {
        WrapS = s;

        WrapT = t;

        WrapR = r;
    }

    /************************************************************************/
    /*                            PUBLIC METHODS                            */
    /************************************************************************/
            
    void WrapMode :: Setup ( GLenum target )
    {
        glTexParameteri ( target, GL_TEXTURE_WRAP_S, WrapS );
            
        glTexParameteri ( target, GL_TEXTURE_WRAP_T, WrapT );
                
        glTexParameteri ( target, GL_TEXTURE_WRAP_R, WrapR );
    }
}