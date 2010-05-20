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

#define NOMINMAX

#include <fstream>

#include <logger.h>

#include "ShaderProgram.hpp"

using namespace std;

namespace graphics
{
    /************************************************************************/
    /*                      CONSTRUCTOR AND DESTRUCTOR                      */
    /************************************************************************/

    ShaderProgram :: ShaderProgram ( void )
    {
        fVertexShader = glCreateShader ( GL_VERTEX_SHADER );

        fFragmentShader = glCreateShader ( GL_FRAGMENT_SHADER );

        fProgramObject = glCreateProgram ( );
    }

    /************************************************************************/

    ShaderProgram :: ~ShaderProgram ( void )
    {
        glDeleteShader ( fVertexShader );

        glDeleteShader ( fFragmentShader );

        glDeleteProgram ( fProgramObject );
    }

    /************************************************************************/
    /*                           PRIVATE METHODS                            */
    /************************************************************************/

    bool ShaderProgram :: LoadShader ( GLuint shader,
                                       const char ** filenames,
                                       GLint count,
                                       const char * prefix )
    {
        char ** lines = new char * [count + 1];

        for ( GLint index = 0; index < count + 1; ++index )
        {
            lines [index] = NULL;
        }

        //----------------------------------------------------------------

        EZLOGGERSTREAM << "Setting prefix..." << endl;

        const char * result = NULL != prefix ? prefix : "/*NO PREFIX*/";
        
        lines [0] = new char [strlen ( result ) + 1];

        strcpy ( lines [0], result );

        EZLOGGERVAR ( lines [0] );

        //----------------------------------------------------------------

        bool loaded = true;

        for ( GLint index = 0; index < count; ++index )
        {
            EZLOGGERVAR ( filenames [index] );

            ifstream file ( filenames [index], ios :: ate );

            //------------------------------------------------------------

            if ( !file )
            {
                EZLOGGERSTREAM << "ERROR: Could not open file" << endl;

                loaded = false; break;
            }

            //------------------------------------------------------------

            ULONG size = file.tellg ( );
            
            if ( 0 == size )
            {
                EZLOGGERSTREAM << "WARNING: File is empty" << endl;
            }
            else
            {
                EZLOGGERVAR ( size );
            }

            file.seekg ( 0, ios :: beg );

            //------------------------------------------------------------

            char * source = new char [size + 1];
            
            memset ( source, 0, size + 1 );

            file.read ( source, size );

            //------------------------------------------------------------
            
            lines [index + 1] = source;

            file.close ( );
        }

        //----------------------------------------------------------------

        if ( loaded )
        {
            EZLOGGERSTREAM << "Loading shader source..." << endl;
            
            glShaderSource (
                shader,
                count + 1,
                ( const char ** ) lines,
                NULL );
        }

        //----------------------------------------------------------------

        EZLOGGERSTREAM << "Clearing memory resources..." << endl;

        for ( GLint index = 0; index < count + 1; ++index )
        {
            delete [] lines [index];
        }

        delete [] lines;

        return loaded;
    }
    
    /************************************************************************/

    bool ShaderProgram :: CompileShader ( unsigned shader )
    {
        EZLOGGERSTREAM << "Compiling shader..." << endl;

        glCompileShader ( shader );

        //-----------------------------------------------------------

        EZLOGGERSTREAM << "Loading information log..." << endl;

        GLint capacity = 0;

        glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &capacity );

        char * info = new char [capacity];

        memset ( info, 0, capacity );

        glGetShaderInfoLog ( shader, capacity, NULL, info );

        if ( 0 == strlen ( info ) )
        {
            EZLOGGERSTREAM << "Information log is empty..." << endl;
        }
        else
        {
            EZLOGGERSTREAM << info << endl;
        }

        delete [] info;

        //-----------------------------------------------------------

        EZLOGGERSTREAM << "Loading compile status..." << endl;

        GLint status = 0;

        glGetShaderiv ( shader, GL_COMPILE_STATUS, &status );

        if ( 0 == status )
        {
            EZLOGGERSTREAM << "ERROR: Compilation failed" << endl;

            return false;
        }
        else
        {
            EZLOGGERSTREAM << "SUCCESS!" << endl;

            return true;
        }           
    }

    /************************************************************************/

    bool ShaderProgram :: AttachShader ( unsigned shader )
    {
        EZLOGGERSTREAM << "Attaching shader to program..." << endl;

        glAttachShader ( fProgramObject, shader );

        return true;
    }

    /************************************************************************/
    /*                            PUBLIC METHODS                            */
    /************************************************************************/

    bool ShaderProgram :: LoadVertexShader ( const char * filename,
                                             const char * prefix )
    {        
        return LoadVertexShader ( &filename, 1, prefix );
    }

    /************************************************************************/

    bool ShaderProgram :: LoadFragmentShader ( const char * filename,
                                               const char * prefix )
    {
        return LoadFragmentShader ( &filename, 1, prefix );
    }

    /************************************************************************/

    bool ShaderProgram :: LoadVertexShader ( const char ** filenames,
                                             GLint count,
                                             const char * prefix )
    {
        EZLOGGERSTREAM << "SETTING VERTEX SHADER" << endl;
        
        return LoadShader ( fVertexShader, filenames, count, prefix ) &&
               CompileShader ( fVertexShader ) &&
               AttachShader ( fVertexShader );
    }

    /************************************************************************/

    bool ShaderProgram :: LoadFragmentShader ( const char ** filenames,
                                               GLint count,
                                               const char * prefix )
    {
        EZLOGGERSTREAM << "SETTING VERTEX SHADER" << endl;

        return LoadShader ( fFragmentShader, filenames, count, prefix ) &&
               CompileShader ( fFragmentShader ) &&
               AttachShader ( fFragmentShader );
    }

    /************************************************************************/

    bool ShaderProgram :: Build ( void )
    {
        EZLOGGERSTREAM << "BUILDING PROGRAM OBJECT" << endl;

        glLinkProgram ( fProgramObject );

        //-----------------------------------------------------------

        GLint capacity = 0;

        glGetProgramiv ( fProgramObject,
                         GL_INFO_LOG_LENGTH,
                         &capacity );

        char * info = new char [capacity];

        memset ( info, 0, capacity );

        glGetProgramInfoLog ( fProgramObject,
                              capacity,
                              NULL,
                              info );

        if ( 0 == strlen ( info ) )
        {
            EZLOGGERSTREAM << "Information log is empty..." << endl;
        }
        else
        {
            EZLOGGERSTREAM << info << endl;
        }

        delete [] info;

        //-----------------------------------------------------------

        GLint status = 0;

        glGetProgramiv ( fProgramObject,
                         GL_LINK_STATUS,
                         &status );

        if ( 0 == status )
        {
            EZLOGGERSTREAM << "ERROR: Could not link program" << endl;

            return false;
        }
        else
        {
            EZLOGGERSTREAM << "SUCCESS!" << endl;

            return true;
        }
    }

    /************************************************************************/

    void ShaderProgram :: Bind ( void )
    {
        glUseProgram ( fProgramObject );
    }

    /************************************************************************/
    
    void ShaderProgram :: Unbind ( void )
    {
        glUseProgram ( NULL );
    }

    /************************************************************************/
    
    GLint ShaderProgram :: GetUniformLocation ( char * name )
    {
        return glGetUniformLocation ( fProgramObject, name );
    }

    /************************************************************************/

    GLint ShaderProgram :: GetAttributeLocation ( char * name )
    {
        return glGetAttribLocation ( fProgramObject, name );
    }

    /************************************************************************/

    Vector4f ShaderProgram :: GetUniformVector ( char * name )
    {
        GLint location = glGetUniformLocation ( fProgramObject, name );

        if ( location < 0 )
            return Vector4f :: Zero ( );

        float values [4];

        glGetUniformfv ( fProgramObject, location, values );

        return Vector4f ( values );
    }

    /************************************************************************/

    Vector4f ShaderProgram :: GetUniformVector ( GLint location )
    {
        float values [4];

        glGetUniformfv ( fProgramObject, location, values );

        return Vector4f ( values );
    }

    /************************************************************************/

    Vector4f ShaderProgram :: GetAttributeVector ( char * name )
    {
        GLint location = glGetAttribLocation ( fProgramObject, name );

        if ( location < 0 )
            return Vector4f :: Zero ( );

        float values [4];

        glGetVertexAttribfv ( location, GL_CURRENT_VERTEX_ATTRIB, values );

        return Vector4f ( values );
    }

    /************************************************************************/

    Vector4f ShaderProgram :: GetAttributeVector ( GLint location )
    {
        float values [4];

        glGetVertexAttribfv ( location, GL_CURRENT_VERTEX_ATTRIB, values );

        return Vector4f ( values );
    }

    /************************************************************************/

    bool ShaderProgram :: SetUniformInteger ( const char * name, GLint val )
    {
        GLint location = glGetUniformLocation ( fProgramObject, name );

        if ( location < 0 )
            return false;

        glUniform1i ( location, val );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetUniformInteger ( GLint location, GLint val )
    {
        glUniform1i ( location, val );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetUniformFloat ( const char * name, float val )
    {
        GLint location = glGetUniformLocation ( fProgramObject, name );

        if ( location < 0 )
            return false;

        glUniform1f ( location, val );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetUniformFloat ( GLint location, float val )
    {
        glUniform1f ( location, val );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetUniformVector ( const char * name,
                                             const Vector2f & val )
    {
        GLint location = glGetUniformLocation ( fProgramObject, name );

        if ( location < 0 )
            return false;

        glUniform2fv ( location, 1, val.data ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetUniformVector ( GLint location,
                                             const Vector2f & val )
    {
        glUniform2fv ( location, 1, val.data ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetUniformVector ( const char * name,
                                             const Vector3f & val )
    {
        GLint location = glGetUniformLocation ( fProgramObject, name );

        if ( location < 0 )
            return false;

        glUniform3fv ( location, 1, val.data ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetUniformVector ( GLint location,
                                             const Vector3f & val )
    {
        glUniform3fv ( location, 1, val.data ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetUniformVector ( const char * name,
                                             const Vector4f & val )
    {
        GLint location = glGetUniformLocation ( fProgramObject, name );

        if ( location < 0 )
            return false;

        glUniform4fv ( location, 1, val.data ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetUniformVector ( GLint location,
                                             const Vector4f & val )
    {
        glUniform4fv ( location, 1, val.data ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetUniformMatrix ( const char * name,
                                             const Matrix2f & val,
                                             GLboolean transpose )
    {
        GLint location = glGetUniformLocation ( fProgramObject, name );

        if ( location < 0 )
            return false;

        glUniformMatrix2fv ( location, 1, transpose, val.data ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetUniformMatrix ( GLint location,
                                             const Matrix2f & val,
                                             GLboolean transpose )
    {
        glUniformMatrix2fv ( location, 1, transpose, val.data ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetUniformMatrix ( const char * name,
                                             const Matrix3f & val,
                                             GLboolean transpose )
    {
        GLint location = glGetUniformLocation ( fProgramObject, name );

        if ( location < 0 )
            return false;

        glUniformMatrix3fv ( location, 1, transpose, val.data ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetUniformMatrix ( GLint location,
                                             const Matrix3f & val,
                                             GLboolean transpose )
    {
        glUniformMatrix3fv ( location, 1, transpose, val.data ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetUniformMatrix ( const char * name,
                                             const Matrix4f & val,
                                             GLboolean transpose )
    {
        GLint location = glGetUniformLocation ( fProgramObject, name );

        if ( location < 0 )
            return false;

        glUniformMatrix4fv ( location, 1, transpose, val.data ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetUniformMatrix ( GLint location,
                                             const Matrix4f & val,
                                             GLboolean transpose )
    {
        glUniformMatrix4fv ( location, 1, transpose, val.data ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetAttributeName ( GLint location,
                                             const char * name )
    {
        glBindAttribLocation ( fProgramObject, location, name );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetAttributeFloat ( const char * name,
                                              float val )
    {
        GLint location = glGetAttribLocation ( fProgramObject, name );

        if ( location < 0 )
            return false;

        glVertexAttrib1f ( location, val );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetAttributeFloat ( GLint location,
                                              float val )
    {
        glVertexAttrib1f ( location, val );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetAttributeVector ( const char * name,
                                               const Vector2f & val )
    {
        GLint location = glGetAttribLocation ( fProgramObject, name );

        if ( location < 0 )
            return false;

        glVertexAttrib2fv ( location, val.data ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetAttributeVector ( GLint location,
                                               const Vector2f & val )
    {
        glVertexAttrib2fv ( location, val.data ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetAttributeVector ( const char * name,
                                               const Vector3f & val )
    {
        GLint location = glGetAttribLocation ( fProgramObject, name );

        if ( location < 0 )
            return false;

        glVertexAttrib3fv ( location, val.data ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetAttributeVector ( GLint location,
                                               const Vector3f & val )
    {
        glVertexAttrib3fv ( location, val.data ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetAttributeVector ( const char * name,
                                               const Vector4f & val )
    {
        GLint location = glGetAttribLocation ( fProgramObject, name );

        if ( location < 0 )
            return false;

        glVertexAttrib4fv ( location, val.data ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetAttributeVector ( GLint location,
                                               const Vector4f & val )
    {
        glVertexAttrib4fv ( location, val.data ( ) );

        return true;
    }

    /************************************************************************/
    
    bool ShaderProgram :: SetTexture ( GLint location,
                                       const Texture1D * texture )
    {
        glUniform1i ( location, texture->Unit ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetTexture ( const char * name,
                                       const Texture1D * texture )
    {
        int location = glGetUniformLocation ( fProgramObject, name );

        if ( location < 0 )
            return false;
        
        glUniform1i ( location, texture->Unit ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetTexture ( GLint location,
                                       const Texture2D * texture )
    {
        glUniform1i ( location, texture->Unit ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetTexture ( const char * name,
                                       const Texture2D * texture )
    {
        int location = glGetUniformLocation ( fProgramObject, name );

        if ( location < 0 )
            return false;

        glUniform1i ( location, texture->Unit ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetTexture ( GLint location,
                                       const Texture3D * texture )
    {
        glUniform1i ( location, texture->Unit ( ) );

        return true;
    }

    /************************************************************************/

    bool ShaderProgram :: SetTexture ( const char * name,
                                       const Texture3D * texture )
    {
        int location = glGetUniformLocation ( fProgramObject, name );

        if ( location < 0 )
            return false;
        
        glUniform1i ( location, texture->Unit ( ) );

        return true;
    }

    /************************************************************************/

    GLuint ShaderProgram :: VertexShader ( void  )
    {
        return fVertexShader;
    }

    /************************************************************************/

    GLuint ShaderProgram :: FragmentShader ( void )
    {
        return fFragmentShader;
    }

    /************************************************************************/

    GLuint ShaderProgram :: ProgramObject ( void )
    {
        return fProgramObject;
    }
}