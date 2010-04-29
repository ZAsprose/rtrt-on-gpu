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

#ifndef _SHADER_MANAGER_
#define _SHADER_MANAGER_

#include <Eigen.h>

#include "Texture1D.hpp"

#include "Texture2D.hpp"

#include "Texture3D.hpp"

namespace graphics
{
    class ShaderProgram
    {
        public:

            /************************************************************************/
            /*                      CONSTRUCTOR AND DESTRUCTOR                      */
            /************************************************************************/

            ShaderProgram ( void );

            ~ShaderProgram ( void );

            /************************************************************************/
            /*                            PUBLIC METHODS                            */
            /************************************************************************/

            bool LoadVertexShader ( const char * filename,
                                    const char * prefix = NULL );

            bool LoadFragmentShader ( const char * filename,
                                      const char * prefix = NULL );

            bool LoadVertexShader ( const char ** filenames,
                                    GLint count,
                                    const char * prefix = NULL );

            bool LoadFragmentShader ( const char ** filenames,
                                      GLint count,
                                      const char * prefix = NULL );

            //------------------------------------------------------------------------

            bool Build ( void );

            //------------------------------------------------------------------------

            void Bind ( void );
            
            void Unbind ( void );

            //------------------------------------------------------------------------

            GLint GetUniformLocation ( char * name );

            GLint GetAttributeLocation ( char * name );

            //------------------------------------------------------------------------

            Vector4f GetUniformVector ( char * name );

            Vector4f GetUniformVector ( GLint location );

            Vector4f GetAttributeVector ( char * name );

            Vector4f GetAttributeVector ( GLint location );

            //------------------------------------------------------------------------

            bool SetUniformInteger ( const char * name, GLint val );

            bool SetUniformInteger ( GLint location, GLint val );

            //------------------------------------------------------------------------

            bool SetUniformFloat ( const char * name, float val );

            bool SetUniformFloat ( GLint location, float val );

            //------------------------------------------------------------------------

            bool SetUniformVector ( const char * name, const Vector2f & val );

            bool SetUniformVector ( GLint location, const Vector2f & val );

            bool SetUniformVector ( const char * name, const Vector3f & val );

            bool SetUniformVector ( GLint location, const Vector3f & val );

            bool SetUniformVector ( const char * name, const Vector4f & val );

            bool SetUniformVector ( GLint location, const Vector4f & val );

            //------------------------------------------------------------------------

            bool SetUniformMatrix ( const char * name,
                                    const Matrix2f & val,
                                    GLboolean transpose = GL_FALSE );

            bool SetUniformMatrix ( GLint location,
                                    const Matrix2f & val,
                                    GLboolean transpose = GL_FALSE );

            bool SetUniformMatrix ( const char * name,
                                    const Matrix3f & val,
                                    GLboolean transpose = GL_FALSE );

            bool SetUniformMatrix ( GLint location,
                                    const Matrix3f & val,
                                    GLboolean transpose = GL_FALSE );

            bool SetUniformMatrix ( const char * name,
                                    const Matrix4f & val,
                                    GLboolean transpose = GL_FALSE );

            bool SetUniformMatrix ( GLint location,
                                    const Matrix4f & val,
                                    GLboolean transpose = GL_FALSE );
            
            //------------------------------------------------------------------------

            bool SetAttributeName ( GLint location, const char * name );

            //------------------------------------------------------------------------

            bool SetAttributeFloat ( const char * name, float val );

            bool SetAttributeFloat ( GLint location, float val );

            //------------------------------------------------------------------------

            bool SetAttributeVector ( const char * name, const Vector2f & val );

            bool SetAttributeVector ( GLint location, const Vector2f & val );

            bool SetAttributeVector ( const char * name, const Vector3f & val );

            bool SetAttributeVector ( GLint location, const Vector3f & val );

            bool SetAttributeVector ( const char * name, const Vector4f & val );

            bool SetAttributeVector ( GLint location, const Vector4f & val );

            //------------------------------------------------------------------------

            bool SetTexture ( GLint location, const Texture1D * texture );

            bool SetTexture ( const char * name, const Texture1D * texture );

            bool SetTexture ( GLint location, const Texture2D * texture );

            bool SetTexture ( const char * name, const Texture2D * texture );

            bool SetTexture ( GLint location, const Texture3D * texture );

            bool SetTexture ( const char * name, const Texture3D * texture );

            //------------------------------------------------------------------------

            GLuint VertexShader ( void  );

            GLuint FragmentShader ( void );

            GLuint ProgramObject ( void );

        private:

            /************************************************************************/
            /*                            PRIVATE FIELDS                            */
            /************************************************************************/

            GLuint fVertexShader;

            GLuint fFragmentShader;

            GLuint fProgramObject;

            /************************************************************************/
            /*                           PRIVATE METHODS                            */
            /************************************************************************/

            bool LoadShader ( GLuint shader,
                              const char ** filenames,
                              GLint count,
                              const char * prefix = NULL );

            bool CompileShader ( GLuint shader );

            bool AttachShader ( GLuint shader );
    };
}

#endif