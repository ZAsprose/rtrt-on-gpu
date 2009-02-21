#include <stdlib.h>

#include <stdio.h>

#include <iostream>

#include <fstream>

#include "GLee.h"

#include "ShaderManager.h"

using namespace std;

namespace Render
{
	ShaderManager :: ShaderManager( int components )
	{
		cout << "Creating programm..." << endl;

		program = glCreateProgram ( );

		cout << "Attaching shaders to program..." << endl;

		switch ( components )
		{
			case Vertex:

				vertex = glCreateShader ( GL_VERTEX_SHADER );

				glAttachShader ( program, vertex );
				
				break;

			case Fragment:

				fragment = glCreateShader ( GL_FRAGMENT_SHADER );

				glAttachShader ( program, fragment );
				
				break;

			case Both:

				vertex = glCreateShader ( GL_VERTEX_SHADER );

				fragment = glCreateShader ( GL_FRAGMENT_SHADER );

				glAttachShader ( program, vertex );

				glAttachShader ( program, fragment );
				
				break;
		}
	}

	ShaderManager :: ~ShaderManager( void )
	{
	
	}

	bool ShaderManager :: Load ( int shader, const char ** filenames, int count )
	{
		cout << "Loading shader source..." << endl;

		char ** lines = new char * [count];		

		//---------------------------------------------------------------------

		bool loaded = true;

		for ( int index = 0; index < count; index++ )
		{
			cout << filenames [index] << "..." << endl;

			ifstream file ( filenames [index] );

			//-----------------------------------------------------------------
			
			if ( !file )
			{
				cout << "ERROR: Could not open file" << endl;
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

			char * source = new char [length + 1];

			unsigned long i = 0;
			
			while ( file )
			{
				source [i++] = file.get ( );
			}
			
			source [i - 1] = 0;

			//-----------------------------------------------------------------

			lines [index] = source;
			
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
			delete [] lines [index];
		}

		delete [] lines;

		return loaded;
	}

	bool ShaderManager :: Compile ( int shader )
	{
		//---------------------------------------------------------------------

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

		//---------------------------------------------------------------------
    	
    	int status = 0;
    	
    	glGetShaderiv ( shader, GL_COMPILE_STATUS, &status );

		if ( 0 == status )
		{
			cout << "ERROR! Could not compile shader" << endl;

			return false;
		}
		else
		{
			cout << "SUCCESS!" << endl;

			return true;
		}           
	}

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
		cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "+++                     VERTEX SHADER                     +++" << endl;
		cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
            
		return Load ( vertex, filenames, count ) && Compile ( vertex );
	}

	bool ShaderManager :: LoadFragmentShader ( const char ** filenames, int count )
	{
		cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "+++                    FRAGMENT SHADER                    +++" << endl;
		cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
            
		return Load ( fragment, filenames, count ) && Compile ( fragment );
	}

	bool ShaderManager :: BuildProgram ( void )
	{
		cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "+++                     PROGRAM OBJECT                    +++" << endl;
		cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
            
		cout << "Linking program..." << endl;
	            
		glLinkProgram ( program );

		//------------------------------------ Getting Info Log -----------------------------------

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

		//---------------------------------- Checking Link Status ---------------------------------
		
		int status = 0;
		
		glGetProgramiv ( program, GL_LINK_STATUS, &status );
	
		if ( 0 == status )
		{
			cout << "ERROR! Could not link program" << endl;

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
}
