//========================================================================
// This is a small test application for GLFW.
// The program opens a window (640x480), and renders a spinning colored
// triangle (it is controlled with both the GLFW timer and the mouse). It
// also calculates the rendering speed (FPS), which is displayed in the
// window title bar.
//========================================================================

#include <stdio.h>

#include <GLee.h>

#include <GL/glfw.h>

#include "ShaderManager.h"

#include "Camera.h"

#include "Mouse.h"

#include "Vector3D.h"

#include "Matrix3D.h"

using namespace RenderTools;

using namespace Math;

//========================================================================

Camera cam;

int LocationX;
			
int LocationY;

float Step1 = 0.004F;

bool Active = false;

void MouseMove ( int x, int y )
{
	if ( Active )
	{
				if ( LocationX != x )
				{
					cam.RotateLocal ( -( x - LocationX ) * Step1, Vector3D :: AxisY );
				}
        	
				if ( LocationY != y )
				{
					cam.RotateLocal ( ( y - LocationY ) * Step1, Vector3D :: AxisX );
				}
			
				LocationX = x;
			
				LocationY = y;
	}
}
		
void MouseButton ( int button, int state )
{
	glfwGetMousePos ( &LocationX, &LocationY );

	Active = state > 0;
}

bool W1 = false;
bool S1 = false;
bool A1 = false;
bool D1 = false;

void KeyButton ( int key, int state )
{
	switch ( key )
	{
		case 'W': W1 = state == GLFW_PRESS; break;

		case 'S': S1 = state == GLFW_PRESS; break;

		case 'A': A1 = state == GLFW_PRESS; break;

		case 'D': D1 = state == GLFW_PRESS; break;
	}
}

float Step2 = 0.004F;

void KeyApply ( )
{
	if ( W1 ) cam.MoveLocal ( Step2, Vector3D :: AxisZ );

	if ( S1 ) cam.MoveLocal ( -Step2, Vector3D :: AxisZ );

	if ( A1 ) cam.MoveLocal ( Step2, Vector3D :: AxisX );

	if ( D1 ) cam.MoveLocal ( -Step2, Vector3D :: AxisX );
}



int main( void )
{
    int     width, height, running, frames, x, y;
    double  t, t0, fps;
    char    titlestr[ 200 ];

    glfwInit();

    if( !glfwOpenWindow( 640, 480, 0,0,0,0, 16, 0, GLFW_WINDOW ) )
    {
        glfwTerminate();
        return 0;
	}

	glEnable( GL_COLOR_MATERIAL );

	//----------------------------------------------------

	ShaderManager smanager;

	smanager.LoadVertexShader ( "E:/Projects/rtrt-on-gpu/Test Suite/Shader Test/Vertex.vs" );

	smanager.LoadFragmentShader ( "E:/Projects/rtrt-on-gpu/Test Suite/Shader Test/Fragment.fs" );

	smanager.BuildProgram ( );

	//----------------------------------------------------

	cam = Camera ( Vector3D ( 0.0F, 0, 10.0F ), Vector3D ( 0.0F, 0.0F, 0.0F ) );

	//cam.MoveWorld ( 10, Vector3D::AxisY );

	//----------------------------------------------------

    // Enable sticky keys
    glfwEnable( GLFW_STICKY_KEYS );

    // Disable vertical sync (on cards that support it)
    glfwSwapInterval( 0 );

	glfwSetMousePosCallback( MouseMove );
	glfwSetMouseButtonCallback( MouseButton );
	glfwSetKeyCallback ( KeyButton );

    // Main loop
    running = GL_TRUE;
    frames = 0;
    t0 = glfwGetTime();

    while( running )
    {
        // Get time and mouse position
        t = glfwGetTime();
        glfwGetMousePos( &x, &y );

        // Calculate and display FPS (frames per second)
        if( (t-t0) > 1.0 || frames == 0 )
        {
            fps = (double)frames / (t-t0);
            sprintf( titlestr, "Spinning Triangle (%.1f FPS)", fps );
            glfwSetWindowTitle( titlestr );
            t0 = t;
            frames = 0;
        }
        frames ++;

        // Get window size (may be different than the requested size)
        glfwGetWindowSize( &width, &height );
        height = height > 0 ? height : 1;

        // Set viewport
        glViewport( 0, 0, width, height );

        // Clear color buffer
        glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
        glClear( GL_COLOR_BUFFER_BIT );

        // Select and setup the projection matrix
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluPerspective( 65.0f, (GLfloat)width/(GLfloat)height, 1.0f,
            100.0f );

		KeyApply ( );
		cam.Setup ( );

		glColor3f ( 1, 0, 0 );

		glBegin ( GL_LINES );

			glVertex3f ( 0, 0, 0 ); glVertex3f ( 20, 0, 0 );

		glEnd ( );

		glColor3f ( 0, 1, 0 );

		glBegin ( GL_LINES );

			glVertex3f ( 0, 0, 0 ); glVertex3f ( 0, 20, 0 );

		glEnd ( );

		glColor3f ( 0, 0, 1 );

		glBegin ( GL_LINES );

			glVertex3f ( 0, 0, 0 ); glVertex3f ( 0, 0, 20 );

		glEnd ( );

		//---

        GLUquadric * q = gluNewQuadric ( );

		glRotatef( 0.3f*(GLfloat)x + (GLfloat)t*100.0f, 0.0f, 1.0f, 1.0f );

		smanager.Bind ( );

		gluSphere (q, 2.0, 50, 50 );

		smanager.Unbind ( );

		gluDeleteQuadric ( q );

		//----------------------------------------

        glfwSwapBuffers();

        // Check if the ESC key was pressed or the window was closed
        running = !glfwGetKey( GLFW_KEY_ESC ) &&
                  glfwGetWindowParam( GLFW_OPENED );
    }

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
