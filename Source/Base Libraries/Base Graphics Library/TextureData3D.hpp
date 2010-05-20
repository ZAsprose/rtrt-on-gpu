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

#ifndef _TEXTURE_DATA_3D_
#define _TEXTURE_DATA_3D_

#include <OpenGL.h>

namespace graphics
{
    class TextureData3D
    {
        public:

            /************************************************************************/
            /*                      CONSTRUCTOR AND DESTRUCTOR                      */
            /************************************************************************/

            TextureData3D ( GLsizei width = 128,
                            GLsizei height = 128,
                            GLsizei depth = 128,
                            GLuint components = 4,
                            bool empty = false );

            ~TextureData3D ( void );

            /************************************************************************/
            /*                         OVERLOADED OPERATORS                         */
            /************************************************************************/

            operator GLfloat * ( void );

            operator const GLfloat * ( void ) const;

            /************************************************************************/
            /*                            PUBLIC METHODS                            */
            /************************************************************************/

            /* For accessing to texture data as a 1D array of tuples */

            template < class TUPLE >
            TUPLE & Pixel ( GLuint x );

            /* For accessing to texture data as a 3D array of tuples */

            template < class TUPLE >
            TUPLE & Pixel ( GLuint x, GLuint y, GLuint z );

            //------------------------------------------------------------------------

            GLenum Type ( void ) const;

            //------------------------------------------------------------------------

            GLsizei Width ( void ) const;

            GLsizei Height ( void ) const;

            GLsizei Depth ( void ) const;

            //------------------------------------------------------------------------

            GLenum PixelFormat ( void ) const;

            GLint InternalFormat ( void ) const;

            //------------------------------------------------------------------------

            /* Possible targets: GL_TEXTURE_3D */

            void Upload ( GLenum target = GL_TEXTURE_3D );

        private:

            /************************************************************************/
            /*                            PRIVATE FIELDS                            */
            /************************************************************************/

            GLfloat * fPixels;

            //------------------------------------------------------------------------

            GLsizei fWidth;

            GLsizei fHeight;

            GLsizei fDepth;

            //------------------------------------------------------------------------

            GLuint fComponents;
    };

    /********************************************************************************/

    template < class TUPLE >
    TUPLE & TextureData3D :: Pixel ( GLuint x )
    {
        return ( TUPLE & ) *( fPixels + x * fComponents );
    }

    /********************************************************************************/

    template < class TUPLE >
    TUPLE & TextureData3D :: Pixel ( GLuint x, GLuint y, GLuint z )
    {
        return ( TUPLE & ) *( fPixels +
            ( x + ( y + z * fHeight ) * fWidth ) * fComponents );
    }
}

#endif
