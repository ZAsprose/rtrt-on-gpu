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

#include "RenderBuffer.hpp"

namespace graphics
{
    /************************************************************************/
    /*                      CONSTRUCTOR AND DESTRUCTOR                      */
    /************************************************************************/

    RenderBuffer :: RenderBuffer ( GLuint width,
                                   GLuint height,
                                   GLenum attachment,
                                   GLenum internalFormat )
    {
        Width = width;
        
        Height = height;
        
        Attachment = attachment;
        
        InternalFormat = internalFormat;
        
        glGenRenderbuffers ( 1, &fHandle );

        EZLOGGERSTREAM << "Render buffer was created\n";

        EZLOGGERVAR ( fHandle );
    }

    /************************************************************************/

    RenderBuffer :: ~RenderBuffer ( void )
    {
        glDeleteRenderbuffers ( 1, &fHandle );

        EZLOGGERSTREAM << "Render buffer was deleted\n";

        EZLOGGERVAR ( fHandle );
    }
    
    /************************************************************************/
    /*                            PUBLIC METHODS                            */
    /************************************************************************/
            
    void RenderBuffer :: Setup ( void )
    {
        Bind ( );
        
        glRenderbufferStorage ( Target ( ),
                                InternalFormat,
                                Width,
                                Height );
    }

    /************************************************************************/

    void RenderBuffer :: Bind ( void )
    {
        glBindRenderbuffer ( Target ( ), fHandle );
    }

    /************************************************************************/
    
    void RenderBuffer :: Unbind ( void )
    {
        glBindRenderbuffer ( Target ( ), NULL );
    }

    /************************************************************************/
    
    GLenum RenderBuffer :: Target ( void )
    {
        return GL_RENDERBUFFER;
    }

    /************************************************************************/
    
    GLuint RenderBuffer :: Handle ( void )
    {
        return fHandle;
    }
}