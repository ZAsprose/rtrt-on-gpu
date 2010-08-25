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

#include <logger.h>

#include "FrameBuffer.hpp"

namespace graphics
{
    /************************************************************************/
    /*                      CONSTRUCTOR AND DESTRUCTOR                      */
    /************************************************************************/

    FrameBuffer :: FrameBuffer ( void )
    {
        glGenFramebuffers ( 1, &fHandle );

        EZLOGGERSTREAM << "Frame buffer was created\n";

        EZLOGGERVAR ( fHandle );
    }

    /************************************************************************/

    FrameBuffer :: ~FrameBuffer ( void )
    {
        glDeleteFramebuffers ( 1, &fHandle );

        EZLOGGERSTREAM << "Frame buffer was deleted\n";
        
        EZLOGGERVAR ( fHandle );
    }
    
    /************************************************************************/
    /*                            PUBLIC METHODS                            */
    /************************************************************************/
            
    void FrameBuffer :: Setup ( void )
    {
        if ( 0 == fHandle )
        {
            return;
        }

        //-------------------------------------------------------------

        GLint current = 0;

        glGetIntegerv ( GL_FRAMEBUFFER_BINDING, &current );
        
        if ( current != fHandle )
        {
            glBindFramebuffer ( GL_FRAMEBUFFER, fHandle );
        }

        //-------------------------------------------------------------
        
        {
            GLuint * buffers = new GLuint [ColorBuffers.size ( )];
                
            for ( GLuint i = 0; i < ColorBuffers.size ( ); ++i )
            {
                glFramebufferTexture2D (
                    Target ( ),
                    GL_COLOR_ATTACHMENT0 + i,
                    ColorBuffers [i]->Target ( ),
                    ColorBuffers [i]->Handle ( ),
                    0 );

                EZLOGGERSTREAM << "Texture was attached\n";

                EZLOGGERVAR ( i );

                buffers [i] = GL_COLOR_ATTACHMENT0 + i;
            }

            glDrawBuffers ( ColorBuffers.size ( ), buffers );

            delete [] buffers;
        }

        //-------------------------------------------------------------
        
        {
            for ( GLuint i = 0; i < RenderBuffers.size ( ); ++i )
            {
                glFramebufferRenderbuffer (
                    Target ( ),
                    RenderBuffers [i]->Attachment,
                    RenderBuffers [i]->Target ( ),
                    RenderBuffers [i]->Handle ( ) );

                EZLOGGERSTREAM << "Render buffer was attached\n";

                EZLOGGERVAR ( i );
            }
        }

        //-------------------------------------------------------------
        
        if ( current != fHandle )
        {
            glBindFramebuffer ( Target ( ), current );
        }
    }

    /************************************************************************/
    
    void FrameBuffer :: Bind ( void )
    {
        glBindFramebuffer ( Target ( ), fHandle );
    }

    /************************************************************************/

    void FrameBuffer :: Unbind ( void )
    {
        glBindFramebuffer ( Target ( ), NULL );
    }

    /************************************************************************/
    
    void FrameBuffer :: FetchOutcome ( void )
    {
        /* You can change method for reading data ( is it slow? ) */

        for ( GLuint i = 0; i < ColorBuffers.size ( ); ++i )
        {
            glReadBuffer ( GL_COLOR_ATTACHMENT0 + i );
                
            glReadPixels ( 0,
                           0,
                           ColorBuffers [i]->Data->Width ( ),
                           ColorBuffers [i]->Data->Height ( ),
                           ColorBuffers [i]->Data->PixelFormat ( ),
                           ColorBuffers [i]->Data->Type ( ),
                           *( ColorBuffers [i]->Data ) );
        }    
    }

    /************************************************************************/

    GLenum FrameBuffer :: Target ( void )
    {
        return GL_FRAMEBUFFER;
    }

    /************************************************************************/

    GLuint FrameBuffer :: Handle ( void )
    {
        return fHandle;
    }
}