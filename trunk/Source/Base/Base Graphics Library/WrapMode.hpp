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

#ifndef _WRAP_MODE_
#define _WRAP_MODE_

#include <OpenGL.h>

namespace graphics
{
    struct WrapMode
    {
        /************************************************************************/
        /*                            PUBLIC FIELDS                             */
        /************************************************************************/
        
        static const WrapMode Clamp;
        
        static const WrapMode Repeat;
        
        //------------------------------------------------------------------------
            
        GLint WrapS;
        
        GLint WrapT;
            
        GLint WrapR;

        /************************************************************************/
        /*                      CONSTRUCTOR AND DESTRUCTOR                      */
        /************************************************************************/    
        
        WrapMode ( GLint s = GL_CLAMP,
                   GLint t = GL_CLAMP,
                   GLint r = GL_CLAMP );

        /************************************************************************/
        /*                            PUBLIC METHODS                            */
        /************************************************************************/
        
        void Setup ( GLenum target = GL_TEXTURE_2D );
    };
}

#endif