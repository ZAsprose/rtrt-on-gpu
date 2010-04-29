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

#ifndef _TEXTURE_DATA_2D_
#define _TEXTURE_DATA_2D_

#include <OpenGL.h>

namespace graphics
{
    class TextureData2D
    {
        public:

            /************************************************************************/
            /*                      CONSTRUCTOR AND DESTRUCTOR                      */
            /************************************************************************/

            /* If data are created and used only on GPU set last argument to TRUE */

            TextureData2D ( GLsizei width = 512,
                            GLsizei height = 512,
                            GLuint components = 4,
                            bool empty = false );

            ~TextureData2D ( void );

            /************************************************************************/
            /*                         OVERLOADED OPERATORS                         */
            /************************************************************************/

            operator GLfloat * ( void );

            operator const GLfloat * ( void ) const;

            /************************************************************************/
            /*                            PUBLIC METHODS                            */
            /************************************************************************/

            /* For accessing to texture data as a 1D array */

            GLfloat * Pixel ( GLuint x );

            /* For accessing to texture data as a 2D array */

            GLfloat * Pixel ( GLuint x, GLuint y );

            //------------------------------------------------------------------------

            GLenum Type ( void ) const;

            //------------------------------------------------------------------------

            GLsizei Width ( void ) const;

            GLsizei Height ( void ) const;

            //------------------------------------------------------------------------

            GLenum PixelFormat ( void ) const;

            GLint InternalFormat ( void ) const;

            //------------------------------------------------------------------------

            /* Possible targets: GL_TEXTURE_2D and GL_TEXTURE_RECTANGLE_ARB */

            void Upload ( GLenum target = GL_TEXTURE_RECTANGLE_ARB );

            //------------------------------------------------------------------------

            /* Loads data from graphics file ( BMP, PNG, JPG, TGA, DDS, PSD, HDR ) */

            static TextureData2D * LoadFromFile ( char * filename );

        private:

            /************************************************************************/
            /*                            PRIVATE FIELDS                            */
            /************************************************************************/

            GLfloat * fPixels;

            //------------------------------------------------------------------------

            GLsizei fWidth;

            GLsizei fHeight;

            //------------------------------------------------------------------------

            GLuint fComponents;
    };
}

#endif
