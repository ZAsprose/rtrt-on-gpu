/*
 * B A S E   R E N D E R   L I B R A R Y
 *
 * Copyright (C) 2009 Denis Bogolepov, Andrey Morozov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses.
 */

#include <iostream>

#include <fstream>

#include <string.h>

#include "ShaderManager.hpp"

using namespace std;

namespace render
{
	ShaderManager :: ShaderManager ( void )
	{
		Vertex = glCreateShader ( GL_VERTEX_SHADER );
		
		Fragment = glCreateShader ( GL_FRAGMENT_SHADER );

		Program = glCreateProgram ( );
    }

	ShaderManager :: ~ShaderManager ( void )
	{
		glDeleteShader ( Vertex );
		
		glDeleteShader ( Fragment );

		glDeleteProgram ( Program );
    }
	
	bool ShaderManager :: Load ( unsigned shader,
			                     const char ** files,
			                     int count,
			                     const char * prefix )
	{
		cout << "Loading shader source..." << endl;

		char ** lines = new char * [count + 1];

		//---------------------------------------------------------------------

		if ( NULL == prefix )
		{
			*lines = "";
		}
		else
		{
			*lines = new char [sizeof ( prefix )];

			strcpy ( *lines, prefix );
		}

		//---------------------------------------------------------------------
        
        bool loaded = true;

        for ( int index = 0; index < count; index++ )
		{
			cout << "File: \"" << files [index] << "\"...";

			ifstream file ( files [index] );

			if ( !file )
			{
				cout << endl << "ERROR: Could not open file" << endl;

				loaded = false; break;
			}

			//-----------------------------------------------------------------

			file.seekg ( 0, ios :: end );

			unsigned long length = file.tellg ( );

			file.seekg ( 0, ios :: beg );

			if ( 0 == length )
			{
				cout << endl << "WARNING: File is empty" << endl;
			}

			//-----------------------------------------------------------------

			char * source = new char [length + 1];
			
			memset ( source, 0, length + 1 );

			unsigned long i = 0;

			while ( file ) source [i++] = file.get ( );
			
			file.close ( );

			//-----------------------------------------------------------------

			lines [index + 1] = source;

			cout << "OK" << endl;
		}

		//---------------------------------------------------------------------

		if ( loaded )
		{
			glShaderSource ( shader,
					         count + 1,
					         ( const char** ) lines,
					         NULL );

			cout << "SUCCESS!" << endl;
		}

		//---------------------------------------------------------------------

		for ( int index = 0; index < count + 1; index++ )
		{
			delete [] lines [index];
		}

		delete [] lines;

		return loaded;
	}

	bool ShaderManager :: Compile ( unsigned shader )
	{
		cout << "Compiling shader..." << endl;
		
		glCompileShader ( shader );

		//---------------------------------------------------------------------
		
		int capacity = 0;

		glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &capacity );
		
		char * info = new char [capacity];

		memset ( info, 0, capacity );
		
		glGetShaderInfoLog ( shader, capacity, NULL, info );

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

	bool ShaderManager :: Attach ( unsigned shader )
	{
		cout << "Attaching shader to program..." << endl;

		glAttachShader ( Program, shader );

		return true;
	}

	bool ShaderManager :: LoadVertexShader ( const char * file, const char * prefix )
	{
		return LoadVertexShader ( &file, 1, prefix );
	}

	bool ShaderManager :: LoadFragmentShader ( const char * file, const char * prefix )
	{
		return LoadFragmentShader ( &file, 1, prefix );
	}

	bool ShaderManager :: LoadVertexShader ( const char ** files, int count, const char * prefix )
	{
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "+++                           VERTEX SHADER                          +++" << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

		return Load ( Vertex, files, count, prefix ) && Compile ( Vertex ) && Attach ( Vertex );
	}

	bool ShaderManager :: LoadFragmentShader ( const char ** files, int count, const char * prefix )
	{
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "+++                          FRAGMENT SHADER                         +++" << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

		return Load ( Fragment, files, count, prefix ) && Compile ( Fragment ) && Attach ( Fragment );
	}

	bool ShaderManager :: BuildProgram ( void )
	{
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "+++                          PROGRAM OBJECT                          +++" << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

		cout << "Linking program..." << endl;

		glLinkProgram ( Program );

		//---------------------------------------------------------------------

    	int capacity = 0;

        glGetProgramiv ( Program, GL_INFO_LOG_LENGTH, &capacity );

		char * info = new char [capacity];

		memset ( info, 0, capacity );

		glGetProgramInfoLog ( Program, capacity, NULL, info );

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

		glGetProgramiv ( Program, GL_LINK_STATUS, &status );

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

	void ShaderManager :: Bind ( void ) const
	{
		glUseProgram ( Program );
	}

	void ShaderManager :: Unbind ( void )
	{
		glUseProgram ( 0 );
	}
}
