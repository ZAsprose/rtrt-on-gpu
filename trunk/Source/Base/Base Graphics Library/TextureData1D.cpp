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

#include <malloc.h>

#include "TextureData1D.hpp"

namespace graphics
{
    /************************************************************************/
    /*                      CONSTRUCTOR AND DESTRUCTOR                      */
    /************************************************************************/

    TextureData1D :: TextureData1D ( GLsizei width,
                                     GLuint components,
                                     bool empty )
    {
        fWidth = width;

        fComponents = components;

        if ( empty )
        {
            fPixels = NULL;
        }
        else
        {
            /*
             * We use Eigen math library which use aligned data types.
             * For compatibility reasons we must do aligned malloc too.
             */

#ifdef _WIN32

            fPixels = ( float * ) _aligned_malloc (
                width * components * sizeof ( float ), 16 );

#else

            fPixels = ( float * ) memalign (
                16, width * components * sizeof ( float ) );

#endif
        }
    }

    /************************************************************************/

    TextureData1D :: ~TextureData1D ( void )
    {
#ifdef _WIN32

        _aligned_free ( fPixels );


#else

        free ( fPixels );

#endif
    }

    /************************************************************************/
    /*                         OVERLOADED OPERATORS                         */
    /************************************************************************/

    TextureData1D :: operator GLfloat * ( void )
    {
        return fPixels;
    }

    TextureData1D :: operator const GLfloat * ( void ) const
    {
        return fPixels;
    }

    /************************************************************************/
    /*                            PUBLIC METHODS                            */
    /************************************************************************/

    void TextureData1D :: Upload ( GLenum target )
    {
        glTexImage1D (
            target               /* the target texture */,
            0                    /* the level-of-detail number */,
            InternalFormat ( )   /* the number of color components */,
            Width ( )            /* the width of the texture with border */,
            0                    /* the width of the border */,
            PixelFormat ( )      /* the format of the pixel data */,
            Type ( )             /* the data type of the pixel data */,
            fPixels              /* a pointer to the image data */ );
    }

    /************************************************************************/

    GLenum TextureData1D :: Type ( void ) const
    {
        /* we use ONLY floating point pixel data */

        return GL_FLOAT;
    }

    /************************************************************************/

    GLsizei TextureData1D :: Width ( void ) const
    {
        return fWidth;
    }

    /************************************************************************/

    GLenum TextureData1D :: PixelFormat ( void ) const
    {
        /* we use ONLY three base formats for input pixel data */

        switch ( fComponents )
        {
            case 1:
                return GL_ALPHA;

            case 3:
                return GL_RGB;

            case 4:
                return GL_RGBA;

            default:
                return -1;
        }
    }

    /************************************************************************/

    GLint TextureData1D :: InternalFormat ( void ) const
    {
        /* we use ONLY floating point formats for storing texture data */

        switch ( fComponents )
        {
            case 1:
                return GL_ALPHA32F_ARB;

            case 3:
                return GL_RGB32F_ARB;

            case 4:
                return GL_RGBA32F_ARB;

            default:
                return -1;
        }
    }
}