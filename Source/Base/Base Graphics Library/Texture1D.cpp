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

#include "Texture1D.hpp"

namespace graphics
{
    /************************************************************************/
    /*                      CONSTRUCTOR AND DESTRUCTOR                      */
    /************************************************************************/

    Texture1D :: Texture1D ( GLenum unit,
                             TextureData1D * data,
                             GLenum target )
    {
        fUnit = unit;

        fTarget = target;

        Data = data;

        glGenTextures ( 1, &fHandle );

        EZLOGGERSTREAM << "Texture 1D was created\n";

        EZLOGGERVAR ( fHandle );
    }

    /************************************************************************/
    
    Texture1D :: ~Texture1D ( void )
    {
        delete Data;

        glDeleteTextures ( 1, &fHandle );

        EZLOGGERSTREAM << "Texture 1D was deleted\n";

        EZLOGGERVAR ( fHandle );
    }
    
    /************************************************************************/
    /*                            PUBLIC METHODS                            */
    /************************************************************************/
            
    void Texture1D :: Setup ( void )
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

    void Texture1D :: Update ( void )
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
    
    void Texture1D :: Bind ( void )
    {
        glActiveTexture ( GL_TEXTURE0 + fUnit );
        
        glBindTexture ( fTarget, fHandle );
    }

    /************************************************************************/
    
    void Texture1D :: Unbind ( void )
    {
        glActiveTexture ( GL_TEXTURE0 + fUnit );
        
        glBindTexture ( fTarget, NULL );            
    }

    /************************************************************************/
    
    GLenum Texture1D :: Unit ( void ) const
    {
        return fUnit;
    }

    /************************************************************************/
    
    GLenum Texture1D :: Target ( void ) const
    {
        return fTarget;
    }

    /************************************************************************/
    
    GLuint Texture1D :: Handle ( void ) const
    {
        return fHandle;
    }
}