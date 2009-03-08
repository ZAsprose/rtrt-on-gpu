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

using namespace RenderTools;

using namespace Math;

//=================================================================================================

Camera cam;

Mouse mouse ( 0.01F );

Keyboard keyboard ( 0.05F );

//=================================================================================================

void MouseMove ( int x, int y )
{
	mouse.MouseMove ( x, y );
}
		
void MouseButton ( int button, int state )
{
	mouse.StateChange ( state );
}

void KeyButton ( int key, int state )
{
	keyboard.StateChange ( key, state );
}

//=================================================================================================

int main ( void )
{
    int     width, height, running, frames;
    double  t, t0, fps;
    char    titlestr[ 200 ];

    glfwInit();

    if( !glfwOpenWindow( 512, 512, 0, 0, 0, 0, 0, 0, GLFW_WINDOW ) )
    {
        glfwTerminate();

        return 0;
	}

	//---------------------------------------------------------------------------------------------

	ShaderManager manager;

	manager.LoadVertexShader ( "E:\\Projects\\rtrt-on-gpu\\Test Suite\\Shader Test\\Debug\\Vertex.vs" );

	manager.LoadFragmentShader ( "E:\\Projects\\rtrt-on-gpu\\Test Suite\\Shader Test\\Debug\\Fragment.fs" );

	manager.BuildProgram ( );

	//---------------------------------------------------------------------------------------------

	cam = Camera ( Vector3D ( 0.0F, 0.0F, -18.0F ), Vector3D ( 0.0F, 0.0F, 0.0F ) );

	//---------------------------------------------------------------------------------------------

    glfwSwapInterval( 0 );

	glfwSetMousePosCallback( MouseMove );
	glfwSetMouseButtonCallback( MouseButton );
	glfwSetKeyCallback ( KeyButton );

	//---------------------------------------------------------------------------------------------

	glEnable ( GL_TEXTURE_2D );

	glEnable ( GL_TEXTURE_RECTANGLE_ARB );


	TextureData2D * data = new TextureData2D ( 512, 512, 3 );

	data->Pixel < Vector3D > ( 256, 256 ) = Vector3D :: Unit;
	data->Pixel < Vector3D > ( 500, 500 ) = Vector3D :: AxisX;
	data->Pixel < Vector3D > ( 500, 12  ) = Vector3D :: AxisY;
	data->Pixel < Vector3D > ( 12,  500 ) = Vector3D :: AxisZ;
	data->Pixel < Vector3D > ( 12,  12  ) = Vector3D ( 1, 1, 0 );

	Texture2D texture ( GL_TEXTURE_RECTANGLE_ARB, data, 0, "TestTexture" );

	texture.Setup();

	//---------------------------------------------------------------------------------------------
	
	running = GL_TRUE;

	frames = 0;

	t0 = glfwGetTime();

    while( running )
    {
        t = glfwGetTime();

        if ( ( t-t0 ) > 1.0 || frames == 0 )
        {
            fps = (double)frames / (t-t0);

            sprintf( titlestr, "Implicity Surfaces (%.1f FPS)", fps );

            glfwSetWindowTitle( titlestr );
            t0 = t;
            frames = 0;
        }

        frames++;

		//-----------------------------------------------------------------------------------------

        glfwGetWindowSize( &width, &height );

        height = height > 0 ? height : 1;

        glViewport( 0, 0, width, height );

		//-----------------------------------------------------------------------------------------

        glMatrixMode ( GL_PROJECTION );

        glLoadIdentity ( );

		glOrtho ( -1.0F, 1.0F, -1.0F, 1.0F, -1.0F, 1.0F  );

		//-----------------------------------------------------------------------------------------

		mouse.Apply ( cam );

		keyboard.Apply ( cam );

		//-----------------------------------------------------------------------------------------

		manager.Bind ( );

		manager.SetTexture ( texture );

		glClear ( GL_COLOR_BUFFER_BIT );

		//cam.SetShaderData ( manager );

		glBegin ( GL_QUADS );

			glTexCoord2f( 0.0F,   0.0F );   glVertex2f ( -1.0F, -1.0F );
			glTexCoord2f( 0.0F,   512.0F );   glVertex2f ( -1.0F,  1.0F );
			glTexCoord2f( 512.0F, 512.0F );   glVertex2f (  1.0F,  1.0F );
			glTexCoord2f( 512.0F, 0.0F );   glVertex2f (  1.0F, -1.0F );

		glEnd ( );

		manager.Unbind ( );

        glfwSwapBuffers();

		//-----------------------------------------------------------------------------------------
		
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

    glfwTerminate();

    return 0;
}
