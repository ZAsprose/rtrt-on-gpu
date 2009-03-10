#include <stdio.h>

#include <GLee.h>

#include <GL/glfw.h>

#include "ShaderManager.h"

#include "Camera.h"

#include "TextureData2D.h"

#include "Texture2D.h"

#include "Mouse.h"

#include "Keyboard.h"

#include "Vector3D.h"

#include "Matrix3D.h"

//-------------------------------------------------------------------------------------------------

using namespace Math;

using namespace RenderTools;

//-------------------------------------------------------------------------------------------------

int main ( void )
{
	int width = 512;
	int height = 512;

    glfwInit ( );

    if( !glfwOpenWindow ( width, height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW ) )
    {
        glfwTerminate ( );

        return 0;
	}

	//---------------------------------------------------------------------------------------------

    glfwSwapInterval ( 0 );

	//glEnable ( GL_TEXTURE_2D );

	glEnable ( GL_TEXTURE_RECTANGLE_ARB );

	//---------------------------------------------------------------------------------------------

	TextureData2D * data1 = new TextureData2D ( 256, 512, 3 );

	for ( int i = 0; i < 256; i++ )
	{
		for ( int j = 0; j < 512; j++ )
		{
			if ( ( i + j ) % 2 == 0 )
				data1->Pixel < Vector3D > ( i, j ) = Vector3D :: Unit;
			else
				data1->Pixel < Vector3D > ( i, j ) = Vector3D :: Unit / 6.0F;
		}
	}

	for ( int i = 0; i < 256; i++ )
	{
		data1->Pixel < Vector3D > ( i, 0 ) = Vector3D :: AxisX;

		data1->Pixel < Vector3D > ( i, 511 ) = Vector3D :: AxisX;
	}

	for ( int j = 0; j < 512; j++ )
	{
		data1->Pixel < Vector3D > ( 0, j ) = Vector3D :: AxisX;

		data1->Pixel < Vector3D > ( 255, j ) = Vector3D :: AxisX;
	}

	//---------------------------------------------------------------------------------------------

	TextureData2D * data2 = new TextureData2D ( 256, 512, 3 );

	for ( int i = 0; i < 256; i++ )
	{
		for ( int j = 0; j < 512; j++ )
		{
			if ( ( i + j ) % 2 == 0 )
				data2->Pixel < Vector3D > ( i, j ) = Vector3D :: AxisY;
			else
				data2->Pixel < Vector3D > ( i, j ) = Vector3D :: AxisY / 6.0F;
		}
	}

	for ( int i = 0; i < 256; i++ )
	{
		data2->Pixel < Vector3D > ( i, 0 ) = Vector3D :: AxisZ;

		data2->Pixel < Vector3D > ( i, 511 ) = Vector3D :: AxisZ;
	}

	for ( int j = 0; j < 512; j++ )
	{
		data2->Pixel < Vector3D > ( 0, j ) = Vector3D :: AxisZ;

		data2->Pixel < Vector3D > ( 255, j ) = Vector3D :: AxisZ;
	}

	//---------------------------------------------------------------------------------------------

	Texture2D texture1 ( GL_TEXTURE_RECTANGLE_ARB, data1 );

	texture1.Setup();

	Texture2D texture2 ( GL_TEXTURE_RECTANGLE_ARB, data2 );

	texture2.Setup();

	//---------------------------------------------------------------------------------------------
	
	bool running = GL_TRUE;

    while ( running )
    {
        glfwGetWindowSize( &width, &height );

        height = height > 0 ? height : 1;

        glViewport( 0, 0, width, height );

		//-----------------------------------------------------------------------------------------

        glMatrixMode ( GL_PROJECTION );

        glLoadIdentity ( );

		glOrtho ( -1.0F, 1.0F, -1.0F, 1.0F, -1.0F, 1.0F  );

		glMatrixMode ( GL_MODELVIEW );

        glLoadIdentity ( );

		texture1.Bind ( );

		glBegin ( GL_QUADS );

			glTexCoord2f( 0.0F,   0.0F );     glVertex2f ( -1.0F, -1.0F );
			glTexCoord2f( 0.0F,   512.0F );   glVertex2f ( -1.0F,  1.0F );
			glTexCoord2f( 256.0F, 512.0F );   glVertex2f (  0.0F,  1.0F );
			glTexCoord2f( 256.0F, 0.0F );     glVertex2f (  0.0F, -1.0F );

		glEnd ( );

		texture2.Bind ( );

		glBegin ( GL_QUADS );

			glTexCoord2f( 0.0F,   0.0F );     glVertex2f ( 0.0F, -1.0F );
			glTexCoord2f( 0.0F,   512.0F );   glVertex2f ( 0.0F,  1.0F );
			glTexCoord2f( 256.0F, 512.0F );   glVertex2f ( 1.0F,  1.0F );
			glTexCoord2f( 256.0F, 0.0F );     glVertex2f ( 1.0F, -1.0F );

		glEnd ( );

        glfwSwapBuffers();

		//-----------------------------------------------------------------------------------------
		
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

    glfwTerminate();

    return 0;
}