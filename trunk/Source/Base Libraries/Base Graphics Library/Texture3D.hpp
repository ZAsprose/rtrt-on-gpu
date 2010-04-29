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

#ifndef _TEXTURE_3D_
#define _TEXTURE_3D_

#include "WrapMode.hpp"

#include "FilterMode.hpp"

#include "TextureData3D.hpp"

namespace graphics
{
    class Texture3D
    {
        public:

            /************************************************************************/
            /*                            PUBLIC FIELDS                             */
            /************************************************************************/

            WrapMode WrapMode;

            FilterMode FilterMode;

            //------------------------------------------------------------------------

            TextureData3D * Data;

            /************************************************************************/
            /*                      CONSTRUCTOR AND DESTRUCTOR                      */
            /************************************************************************/

            Texture3D ( GLenum unit = 0,
                        TextureData3D * data = NULL,
                        GLenum target = GL_TEXTURE_3D );

            ~Texture3D ( void );

            /************************************************************************/
            /*                            PUBLIC METHODS                            */
            /************************************************************************/
            
            void Setup ( void );

            void Update ( void );

            //------------------------------------------------------------------------

            void Bind ( void );

            void Unbind ( void );

            //------------------------------------------------------------------------
            
            GLenum Unit ( void ) const;

            GLenum Target ( void ) const;

            GLuint Handle ( void ) const;

        private:

            /************************************************************************/
            /*                            PRIVATE FIELDS                            */
            /************************************************************************/

            GLenum fUnit;

            GLenum fTarget;

            GLuint fHandle;
    };
}

#endif