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

#ifndef _FRAME_BUFFER_
#define _FRAME_BUFFER_

#include <vector>

#include "Texture2D.hpp"

#include "RenderBuffer.hpp"

namespace graphics
{
    class FrameBuffer
    {
        public:

            /************************************************************************/
            /*                            PUBLIC FIELDS                             */
            /************************************************************************/
            
            std :: vector < Texture2D * > ColorBuffers;

            std :: vector < RenderBuffer * > RenderBuffers;

            /************************************************************************/
            /*                      CONSTRUCTOR AND DESTRUCTOR                      */
            /************************************************************************/

            FrameBuffer ( void );

            ~FrameBuffer ( void );

            /************************************************************************/
            /*                            PUBLIC METHODS                            */
            /************************************************************************/
            
            void Setup ( void );

            //------------------------------------------------------------------------
            
            void Bind ( void );
            
            void Unbind ( void );

            //------------------------------------------------------------------------

            /* Loads texture data from GPU to CPU */

            void FetchOutcome ( void );

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