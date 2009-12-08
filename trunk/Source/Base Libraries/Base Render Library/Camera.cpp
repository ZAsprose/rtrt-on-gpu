/*
   -----------------------------------------------------------------------------
   |                   B A S E   R E N D E R   L I B R A R Y                   |
   -----------------------------------------------------------------------------

                              Denis Bogolepov ( bogdencmc @ inbox.ru )
   Copyright (c) 2009 - 2010
                              Andrey Morozov ( morozov.andrey.vmk @ gmail.com )

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

#include <GLee.h>

#include <GL\glfw.h>

#include <fstream>

#include "Camera.hpp"

using namespace smath;

using namespace std;

namespace render
{
	Camera :: Camera ( const vec4& position, const vec4& orientation )
	{
		this->position = position;

		this->orientation = orientation;

		update ( );		
	}

	//-----------------------------------------------------------------------------------

	void Camera :: update ( void )
	{

	}

	//-----------------------------------------------------------------------------------

	void Camera :: moveTo ( const vec4& position )
	{
		this->position = position;
	}

	void Camera :: moveBy ( const vec4& delta )
	{
		this->position = this->position + delta;
	}
	
	void Camera :: rotateTo ( const vec4& orientation )
	{
		this->orientation = orientation;

		update ( );
	}

	void Camera :: rotateBy ( const vec4& delta )
	{
		this->orientation = this->orientation + delta;

		update ( );
	}

	//-----------------------------------------------------------------------------------

	bool Camera :: saveToFile ( const char * path )
	{
		ofstream file;

		file.open ( path );

		if ( file.fail ( ) )
		{
			return false;
		}

		file << position << endl << orientation;

		file.close ( );

		return true;
	}

	bool Camera :: loadFromFile ( const char * path )
	{
		ifstream file;

		file.open ( path );

		if ( file.fail ( ) )
		{
			return false;
		}

		//file >> position >> orientation;

		file.close ( );

		update ( );

		return true;
	}

	//-----------------------------------------------------------------------------------

	void Camera :: setup ( void )
	{
		glMatrixMode ( GL_MODELVIEW );

		glLoadIdentity ( );

		//gluLookAt ( Position.X,
		//	        Position.Y,
		//			Position.Z,
		//	        Position.X + View.X,
		//			Position.Y + View.Y,
		//			Position.Z + View.Z,
		//	        Up.X,
		//			Up.Y,
		//			Up.Z );
	}

	void Camera :: setShaderData ( ShaderManager * manager )
	{
		//manager->SetUniformVector ( "Camera.Position", Position );
		//      	
		//manager->SetUniformVector ( "Camera.Side", Side );
		//      	
		//manager->SetUniformVector ( "Camera.Up", Up );
		//      	
		//manager->SetUniformVector ( "Camera.View", View );
		//
		//manager->SetUniformVector ( "Camera.Scale", GetScreenScale ( ) );
	}

	//-----------------------------------------------------------------------------------

	void Camera :: setViewport ( unsigned width,
		                         unsigned height,
								 unsigned left,
								 unsigned top )
	{
		this->width = width;

		this->height = height;
		
		glViewport ( left, top, width, height );
	}

	void Camera :: setFrustum ( float fieldOfView, float nearPlane, float farPlane )
	{
		this->fieldOfView = fieldOfView;

		this->nearPlane = nearPlane;

		this->farPlane = farPlane;

		//---------------------------------------------------------------------

		glMatrixMode ( GL_PROJECTION );

        glLoadIdentity ( );

        gluPerspective ( fieldOfView, aspect, nearPlane, farPlane );
	}
}