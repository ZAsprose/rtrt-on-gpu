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

#ifndef _TEXTURE_DATA_1D_
#define _TEXTURE_DATA_1D_

#include <OpenGL.h>

namespace graphics
{
    class TextureData1D
    {
        public:

            /************************************************************************/
            /*                      CONSTRUCTOR AND DESTRUCTOR                      */
            /************************************************************************/

            /* If data are created and used only on GPU set last argument to TRUE */

            TextureData1D ( GLsizei width,
                            GLuint components = 4,
                            bool empty = false );

            ~TextureData1D ( void );

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

            //------------------------------------------------------------------------

            GLenum Type ( void ) const;

            //------------------------------------------------------------------------

            GLsizei Width ( void ) const;

            //------------------------------------------------------------------------

            GLenum PixelFormat ( void ) const;

            GLint InternalFormat ( void ) const;

            //------------------------------------------------------------------------

            /* Possible targets: GL_TEXTURE_1D */

            void Upload ( GLenum target = GL_TEXTURE_1D );

        private:

            /************************************************************************/
            /*                            PRIVATE FIELDS                            */
            /************************************************************************/

            GLfloat * fPixels;

            //------------------------------------------------------------------------

            GLsizei fWidth;

            //------------------------------------------------------------------------

            GLuint fComponents;
    };
}

#endif