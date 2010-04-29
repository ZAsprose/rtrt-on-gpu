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

#ifndef _FILTER_MODE_
#define _FILTER_MODE_

#include <OpenGL.h>

namespace graphics
{
    struct FilterMode
    {
        /************************************************************************/
        /*                            PUBLIC FIELDS                             */
        /************************************************************************/

        static const FilterMode Nearest;

        static const FilterMode Linear;

        //------------------------------------------------------------------------
    
        GLint Minification;
        
        GLint Magnification;

        /************************************************************************/
        /*                      CONSTRUCTOR AND DESTRUCTOR                      */
        /************************************************************************/        

        FilterMode ( GLint minification = GL_NEAREST,
                     GLint magnification = GL_NEAREST );

        /************************************************************************/
        /*                            PUBLIC METHODS                            */
        /************************************************************************/
        
        void Setup ( GLenum target = GL_TEXTURE_2D );
    };
}

#endif