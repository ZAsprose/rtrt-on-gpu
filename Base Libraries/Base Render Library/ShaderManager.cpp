#include <stdlib.h>

#include <stdio.h>

#include <iostream>

#include <fstream>

#include "GLee.h"

#include "ShaderManager.h"

using namespace std;

namespace RenderTools
{
	//--------------------------------- Constructor and Destructor --------------------------------

	ShaderManager :: ShaderManager ( void )
	{
		vertex = glCreateShader ( GL_VERTEX_SHADER );
		
		fragment = glCreateShader ( GL_FRAGMENT_SHADER );

		program = glCreateProgram ( );
	}

	ShaderManager :: ~ShaderManager ( void )
	{
		glDeleteShader ( vertex );
                
		glDeleteShader ( fragment );
		
		glDeleteProgram ( program );
	}

	//-------------------------------------- Private Methods --------------------------------------

	bool ShaderManager :: Load ( int shader, const char ** filenames, int count )
	{
		cout << "Loading shader source..." << endl;

		char ** lines = new char *[count];		

		//---------------------------------------------------------------------

		bool loaded = true;

		for ( int index = 0; index < count; index++ )
		{
			cout << filenames[index] << "..." << endl;

			ifstream file ( filenames[index] );

			//-----------------------------------------------------------------
			
			if ( !file )
			{
				cout << "ERROR: Could not open file" << endl;

				loaded = false;

				break;
			}

			//-----------------------------------------------------------------

			file.seekg ( 0, ios :: end );
			
			unsigned long length = file.tellg ( );
			
			file.seekg ( 0, ios :: beg );

			//-----------------------------------------------------------------

			if ( 0 == length )
			{
				cout << "WARNING: File is empty" << endl;
			}

			//-----------------------------------------------------------------

			char * source = new char[length + 1];

			unsigned long i = 0;
			
			while ( file )
			{
				source[i++] = file.get ( );
			}
			
			source[i - 1] = 0;

			//-----------------------------------------------------------------

			lines[index] = source;
			
			file.close();
		}

		//---------------------------------------------------------------------

		if ( loaded )
		{
			glShaderSource ( shader, count, ( const char ** ) lines, NULL );

			cout << "SUCCESS!" << endl;
		}

		//---------------------------------------------------------------------

		for ( int index = 0; index < count; index++ )
		{
			delete [] lines[index];
		}

		delete [] lines;

		return loaded;
	}

	bool ShaderManager :: Compile ( int shader )
	{
    	cout << "Compiling shader..." << endl;            		

		glCompileShader ( shader );

		//---------------------------------------------------------------------

    	int capacity = 0;

        glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &capacity );

		char * info = new char [capacity];

		memset ( info, 0, capacity );

		glGetShaderInfoLog ( shader, UINT_MAX, NULL, info );

		if ( 0 == strlen ( info ) )
		{
			cout << "Information log is empty..." << endl;
		}
		else
		{
			cout << info << endl;
		}

		delete [] info;

		//---------------------------------------------------------------------
    	
    	int status = 0;
    	
    	glGetShaderiv ( shader, GL_COMPILE_STATUS, &status );

		if ( 0 == status )
		{
			cout << "ERROR: Could not compile shader" << endl;

			return false;
		}
		else
		{
			cout << "SUCCESS!" << endl;

			return true;
		}           
	}

	bool ShaderManager :: Attach ( int shader )
	{
		cout << "Attaching shader to program..." << endl;

		glAttachShader ( program, shader );

		return true;
	}

	//------------------------------------ Shaders Management -------------------------------------
	
	bool ShaderManager :: LoadVertexShader ( const char * filename )
	{        
		return LoadVertexShader ( &filename, 1 );
	}

	bool ShaderManager :: LoadFragmentShader ( const char * filename )
	{
		return LoadFragmentShader ( &filename, 1 );
	}

    bool ShaderManager :: LoadVertexShader ( const char ** filenames, int count )
	{
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "+++                           VERTEX SHADER                          +++" << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
            
		return Load ( vertex, filenames, count ) && Compile ( vertex ) && Attach ( vertex );
	}

	bool ShaderManager :: LoadFragmentShader ( const char ** filenames, int count )
	{
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "+++                          FRAGMENT SHADER                         +++" << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
            
		return Load ( fragment, filenames, count ) && Compile ( fragment ) && Attach ( fragment );
	}

	bool ShaderManager :: BuildProgram ( void )
	{
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "+++                          PROGRAM OBJECT                          +++" << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
            
		cout << "Linking program..." << endl;
	            
		glLinkProgram ( program );

		//---------------------------------------------------------------------

    	int capacity = 0;

        glGetProgramiv ( program, GL_INFO_LOG_LENGTH, &capacity );

		char * info = new char[capacity];

		memset ( info, 0, capacity );

		glGetProgramInfoLog ( program, UINT_MAX, NULL, info );

		if ( 0 == strlen ( info ) )
		{
			cout << "Information log is empty..." << endl;
		}
		else
		{
			cout << info << endl;
		}

		delete [] info;

		//---------------------------------------------------------------------
		
		int status = 0;
		
		glGetProgramiv ( program, GL_LINK_STATUS, &status );
	
		if ( 0 == status )
		{
			cout << "ERROR: Could not link program" << endl;

			return false;
		}
		else
		{
			cout << "SUCCESS!" << endl;

			return true;
		}
	}

	void ShaderManager :: Bind ( void )
	{
		glUseProgram ( program );
	}
			
	void ShaderManager :: Unbind ( void )
	{
		glUseProgram ( 0 );
	}

	//----------------------------------- Input and Output Data -----------------------------------

	bool ShaderManager :: SetUniformInteger ( const char * name, int value )
	{
		int location = glGetUniformLocation ( program, name );

		if ( location < 0 )
			return false;

		glUniform1i ( location, value );

		return true;
	}

	bool ShaderManager :: SetUniformInteger ( int location, int value )
	{
		glUniform1i ( location, value );

		return true;
	}

	bool ShaderManager :: SetUniformFloat ( const char * name, float value )
	{
		int location = glGetUniformLocation ( program, name );

		if ( location < 0 )
			return false;

		glUniform1f ( location, value );

		return true;
	}

	bool ShaderManager :: SetUniformFloat ( int location, float value )
	{
		glUniform1f ( location, value );

		return true;
	}

	bool ShaderManager :: SetUniformVector ( const char * name, const Vector2D& value )
	{
		int location = glGetUniformLocation ( program, name );

		if ( location < 0 )
			return false;

		glUniform2fv ( location, 1, value );

		return true;
	}

	bool ShaderManager :: SetUniformVector ( int location, const Vector2D& value )
	{
		glUniform2fv ( location, 1, value );

		return true;
	}

	bool ShaderManager :: SetUniformVector ( const char * name, const Vector3D& value )
	{
		int location = glGetUniformLocation ( program, name );

		if ( location < 0 )
			return false;

		glUniform3fv ( location, 1, value );

		return true;
	}

	bool ShaderManager :: SetUniformVector ( int location, const Vector3D& value )
	{
		glUniform3fv ( location, 1, value );

		return true;
	}

	bool ShaderManager :: SetUniformVector ( const char * name, const Vector4D& value )
	{
		int location = glGetUniformLocation ( program, name );

		if ( location < 0 )
			return false;

		glUniform4fv ( location, 1, value );

		return true;
	}

	bool ShaderManager :: SetUniformVector ( int location, const Vector4D& value )
	{
		glUniform4fv ( location, 1, value );

		return true;
	}

	bool ShaderManager :: SetAttributeName ( int location, const char * name )
	{
		glBindAttribLocation ( program, location, name );
		
		return true;
	}

	bool ShaderManager :: SetAttributeFloat ( const char * name, float value )
	{
		int location = glGetAttribLocation ( program, name );

		if ( location < 0 )
			return false;

		glVertexAttrib1f ( location, value );

		return true;
	}

	bool ShaderManager :: SetAttributeFloat ( int location, float value )
	{
		glVertexAttrib1f ( location, value );

		return true;
	}

	bool ShaderManager :: SetAttributeVector ( const char * name, const Vector2D& value )
	{
		int location = glGetAttribLocation ( program, name );

		if ( location < 0 )
			return false;

		glVertexAttrib2fv ( location, value );

		return true;
	}

	bool ShaderManager :: SetAttributeVector ( int location, const Vector2D& value )
	{
		glVertexAttrib2fv ( location, value );

		return true;
	}
	
	bool ShaderManager :: SetAttributeVector ( const char * name, const Vector3D& value )
	{
		int location = glGetAttribLocation ( program, name );

		if ( location < 0 )
			return false;

		glVertexAttrib3fv ( location, value );

		return true;
	}

	bool ShaderManager :: SetAttributeVector ( int location, const Vector3D& value )
	{
		glVertexAttrib3fv ( location, value );

		return true;
	}

	bool ShaderManager :: SetAttributeVector ( const char * name, const Vector4D& value )
	{
		int location = glGetAttribLocation ( program, name );

		if ( location < 0 )
			return false;

		glVertexAttrib4fv ( location, value );

		return true;
	}
	
	bool ShaderManager :: SetAttributeVector ( int location, const Vector4D& value )
	{
		glVertexAttrib4fv ( location, value );

		return true;
	}
}