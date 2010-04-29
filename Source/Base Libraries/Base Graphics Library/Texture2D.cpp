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

#include "Texture2D.hpp"

namespace graphics
{
    /************************************************************************/
    /*                      CONSTRUCTOR AND DESTRUCTOR                      */
    /************************************************************************/

    Texture2D :: Texture2D ( GLenum unit,
                             TextureData2D * data,
                             GLenum target )
    {
        fUnit = unit;

        fTarget = target;

        Data = data;

        glGenTextures ( 1, &fHandle );

        EZLOGGERSTREAM << "Texture 2D was created\n";

        EZLOGGERVAR ( fHandle );
    }

    /************************************************************************/
    
    Texture2D :: ~Texture2D ( void )
    {
        delete Data;

        glDeleteTextures ( 1, &fHandle );

        EZLOGGERSTREAM << "Texture 2D was deleted\n";

        EZLOGGERVAR ( fHandle );
    }
    
    /************************************************************************/
    /*                            PUBLIC METHODS                            */
    /************************************************************************/
            
    void Texture2D :: Setup ( void )
    {
        EZLOGGERSTREAM << "Setting texture data to OpenGL...\n";
        
        EZLOGGERVAR ( fHandle );
        
        EZLOGGERVAR ( fUnit );
    
        if ( NULL != Data )
        {
            Bind ( );

            WrapMode.Setup ( fTarget );

            FilterMode.Setup ( fTarget );
            
            Data->Upload ( fTarget );
        }
        else
        {
            EZLOGGERSTREAM << "ERROR: Texture data is empty\n";
        }                            
    }

    /************************************************************************/

    void Texture2D :: Update ( void )
    {
        if ( NULL != Data )
        {
            Bind ( );
            
            Data->Upload ( fTarget );
        }
        else
        {
            EZLOGGERSTREAM << "ERROR: Texture data is empty\n";
        }
    }

    /************************************************************************/
    
    void Texture2D :: Bind ( void )
    {
        glActiveTexture ( GL_TEXTURE0 + fUnit );
        
        glBindTexture ( fTarget, fHandle );
    }

    /************************************************************************/
    
    void Texture2D :: Unbind ( void )
    {
        glActiveTexture ( GL_TEXTURE0 + fUnit );
        
        glBindTexture ( fTarget, NULL );            
    }

    /************************************************************************/
    
    GLenum Texture2D :: Unit ( void ) const
    {
        return fUnit;
    }

    /************************************************************************/
    
    GLenum Texture2D :: Target ( void ) const
    {
        return fTarget;
    }

    /************************************************************************/
    
    GLuint Texture2D :: Handle ( void ) const
    {
        return fHandle;
    }
}