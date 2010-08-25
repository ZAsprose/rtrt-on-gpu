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

#include "FilterMode.hpp"

namespace graphics
{
    /************************************************************************/
    /*                            PUBLIC FIELDS                             */
    /************************************************************************/

    const FilterMode FilterMode :: Nearest ( GL_NEAREST, GL_NEAREST );
    
    const FilterMode FilterMode :: Linear ( GL_LINEAR, GL_LINEAR );
    
    /************************************************************************/
    /*                      CONSTRUCTOR AND DESTRUCTOR                      */
    /************************************************************************/        

    FilterMode :: FilterMode ( GLint minification, GLint magnification )
    {
        Minification = minification;

        Magnification =  magnification;
    }

    /************************************************************************/
    /*                            PUBLIC METHODS                            */
    /************************************************************************/
            
    void FilterMode :: Setup ( GLenum target )
    {
        glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, Minification );
                
        glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, Magnification );
    }
}