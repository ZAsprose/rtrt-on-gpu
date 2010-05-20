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

#include "TextureData3D.hpp"

namespace graphics
{
    /************************************************************************/
    /*                      CONSTRUCTOR AND DESTRUCTOR                      */
    /************************************************************************/

    TextureData3D :: TextureData3D ( GLsizei width,
                                     GLsizei height,
                                     GLsizei depth,
                                     GLuint components,
                                     bool empty )
    {
        fWidth = width;

        fHeight = height;

        fDepth = depth;

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
                width * height * depth * components * sizeof ( float ), 16 );

#else

            fPixels = ( float * ) memalign (
                16, width * height * depth * components * sizeof ( float ) );

#endif
        }
    }

    /************************************************************************/

    TextureData3D :: ~TextureData3D ( void )
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

    TextureData3D :: operator GLfloat * ( void )
    {
        return fPixels;
    }

    TextureData3D :: operator const GLfloat * ( void ) const
    {
        return fPixels;
    }

    /************************************************************************/
    /*                            PUBLIC METHODS                            */
    /************************************************************************/

    void TextureData3D :: Upload ( GLenum target )
    {
        glTexImage3D (
            target               /* the target texture */,
            0                    /* the level-of-detail number */,
            InternalFormat ( )   /* the number of color components */,
            Width ( )            /* the width of the texture with border */,
            Height ( )           /* the height of the texture with border */,
            Depth ( )            /* the depth of the texture with border */,
            0                    /* the width of the border */,
            PixelFormat ( )      /* the format of the pixel data */,
            Type ( )             /* the data type of the pixel data */,
            fPixels              /* a pointer to the image data */ );
    }

    /************************************************************************/

    GLenum TextureData3D :: Type ( void ) const
    {
        /* we use ONLY floating point pixel data */

        return GL_FLOAT;
    }

    /************************************************************************/

    GLsizei TextureData3D :: Width ( void ) const
    {
        return fWidth;
    }

    /************************************************************************/

    GLsizei TextureData3D :: Height ( void ) const
    {
        return fHeight;
    }

    /************************************************************************/

    GLsizei TextureData3D :: Depth ( void ) const
    {
        return fDepth;
    }

    /************************************************************************/

    GLenum TextureData3D :: PixelFormat ( void ) const
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

    GLint TextureData3D :: InternalFormat ( void ) const
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