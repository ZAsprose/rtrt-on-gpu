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

	manager.LoadVertexShader ( "Vertex.vs" );

	manager.LoadFragmentShader ( "Fragment.fs" );

	manager.BuildProgram ( );

	//---------------------------------------------------------------------------------------------

	cam = Camera ( Vector3D ( 0.0F, 0.0F, -18.0F ), Vector3D ( 0.0F, 0.0F, 0.0F ) );

	//---------------------------------------------------------------------------------------------

    glfwSwapInterval( 0 );

	glfwSetMousePosCallback( MouseMove );
	glfwSetMouseButtonCallback( MouseButton );
	glfwSetKeyCallback ( KeyButton );

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

        glfwGetWindowSize ( &width, &height );

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

		glClear ( GL_COLOR_BUFFER_BIT );

		cam.SetShaderData ( manager );

		glBegin ( GL_QUADS );

			glTexCoord2f( 0.0F,   0.0F );     glVertex2f ( -1.0F, -1.0F );
			glTexCoord2f( 0.0F,   512.0F );   glVertex2f ( -1.0F,  1.0F );
			glTexCoord2f( 512.0F, 512.0F );   glVertex2f (  1.0F,  1.0F );
			glTexCoord2f( 512.0F, 0.0F );     glVertex2f (  1.0F, -1.0F );

		glEnd ( );

		manager.Unbind ( );

        glfwSwapBuffers();

		//-----------------------------------------------------------------------------------------
		
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

    glfwTerminate();

    return 0;
}