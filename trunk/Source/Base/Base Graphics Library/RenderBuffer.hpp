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

#ifndef _RENDER_BUFFER_
#define _RENDER_BUFFER_

#include "OpenGL.h"

namespace graphics
{
    class RenderBuffer
    {
        public:

            /************************************************************************/
            /*                            PUBLIC FIELDS                             */
            /************************************************************************/
            
            GLuint Width;
            
            GLuint Height;

            //------------------------------------------------------------------------

            GLenum Attachment;
            
            GLenum InternalFormat;

            /************************************************************************/
            /*                      CONSTRUCTOR AND DESTRUCTOR                      */
            /************************************************************************/

            RenderBuffer ( GLuint width = 512,
                           GLuint height = 512,
                           GLenum attachment = GL_DEPTH_ATTACHMENT,
                           GLenum internalFormat = GL_DEPTH_COMPONENT16 );

            ~RenderBuffer ( void );

            /************************************************************************/
            /*                            PUBLIC METHODS                            */
            /************************************************************************/
            
            void Setup ( void );

            //------------------------------------------------------------------------

            void Bind ( void );
            
            void Unbind ( void );

            //------------------------------------------------------------------------
            
            GLenum Target ( void );

            GLuint Handle ( void );

        private:

            /************************************************************************/
            /*                            PRIVATE FIELDS                            */
            /************************************************************************/

            GLuint fHandle;
    };
}

#endif