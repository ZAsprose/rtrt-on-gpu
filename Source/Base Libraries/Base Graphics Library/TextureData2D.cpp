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

#include <SOIL.h>

#include <logger.h>

#include "TextureData2D.hpp"

namespace graphics
{
    /************************************************************************/
    /*                      CONSTRUCTOR AND DESTRUCTOR                      */
    /************************************************************************/

    TextureData2D :: TextureData2D ( GLsizei width,
                                     GLsizei height,
                                     GLuint components,
                                     bool empty )
    {
        fWidth = width;

        fHeight = height;

        fComponents = components;

        if ( empty )
        {
            fPixels = NULL;
        }
        else
        {
            fPixels = new float [width * height * components];
        }
    }

    /************************************************************************/

    TextureData2D :: ~TextureData2D ( void )
    {
        delete [] fPixels;
    }

    /************************************************************************/
    /*                         OVERLOADED OPERATORS                         */
    /************************************************************************/

    TextureData2D :: operator GLfloat * ( void )
    {
        return fPixels;
    }

    TextureData2D :: operator const GLfloat * ( void ) const
    {
        return fPixels;
    }

    /************************************************************************/
    /*                            PUBLIC METHODS                            */
    /************************************************************************/

    void TextureData2D :: Upload ( GLenum target )
    {
        glTexImage2D (
            target               /* the target texture */,
            0                    /* the level-of-detail number */,
            InternalFormat ( )   /* the number of color components */,
            Width ( )            /* the width of the texture with border */,
            Height ( )           /* the height of the texture with border */,
            0                    /* the width of the border */,
            PixelFormat ( )      /* the format of the pixel data */,
            Type ( )             /* the data type of the pixel data */,
            fPixels              /* a pointer to the image data */ );
    }

    /************************************************************************/

    GLfloat * TextureData2D :: Pixel ( GLuint x )
    {
        return fPixels + x * fComponents;
    }

    /************************************************************************/

    GLfloat * TextureData2D :: Pixel ( GLuint x, GLuint y )
    {
        return fPixels + ( x + y * fWidth ) * fComponents;
    }

    /************************************************************************/

    GLenum TextureData2D :: Type ( void ) const
    {
        /* we use ONLY floating point pixel data */

        return GL_FLOAT;
    }

    /************************************************************************/

    GLsizei TextureData2D :: Width ( void ) const
    {
        return fWidth;
    }

    /************************************************************************/

    GLsizei TextureData2D :: Height ( void ) const
    {
        return fHeight;
    }

    /************************************************************************/

    GLenum TextureData2D :: PixelFormat ( void ) const
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

    GLint TextureData2D :: InternalFormat ( void ) const
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

    /************************************************************************/
    
    TextureData2D * TextureData2D :: LoadFromFile ( char * filename )
    {
        GLint width = 0, height = 0, channels = 0;

        EZLOGGERSTREAM << "Loading image from file...\n";
        
        BYTE * image = SOIL_load_image ( filename,
                                         &width,
                                         &height,
                                         &channels,
                                         SOIL_LOAD_RGB );

        TextureData2D * data = NULL;

        if ( NULL != image )
        {
            data = new TextureData2D ( width, height, channels );

            for ( GLint index = 0; index < width * height * channels; index++ )
            {
                ( *data ) [index] = image [index] / 255.0F;
            }

            delete [] image;

            EZLOGGERSTREAM << "Image was loaded successfully!\n";
        }
        else
        {
            EZLOGGERSTREAM << "ERROR: Failed to load image\n";
        }
        
        return data;
    }
}