#include <GLee.h>

#include <GL/glfw.h>

#include <vector>

#include "Camera.h"

#include "Texture2D.h"

#include "Mouse.h"

#include "Keyboard.h"

#include "Vector3D.h"

#include "Matrix3D.h"

#include "FrameBuffer.h"

#include "RenderBuffer.h"

//-------------------------------------------------------------------------------------------------

using namespace std;

using namespace Math;

using namespace RenderTools;

//-------------------------------------------------------------------------------------------------

Mouse mouse ( 0.005F );

Keyboard keyboard ( 0.01F );

//-------------------------------------------------------------------------------------------------

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

//-------------------------------------------------------------------------------------------------

#define USE_FB

int main ( void )
{
	int width = 512;
	int height = 512;
	
    double t, t0, fps;
    char titlestr [ 200 ];

    glfwInit ( );

    if( !glfwOpenWindow ( width, height, 0, 0, 0, 0, 16, 0, GLFW_WINDOW ) )
    {
        glfwTerminate ( );

        return 0;
	}

	//---------------------------------------------------------------------------------------------

	glEnable ( GL_TEXTURE_RECTANGLE_ARB );
	
	glEnable ( GL_COLOR_MATERIAL );

	glEnable ( GL_LIGHT0 );

	//---------------------------------------------------------------------------------------------

	Camera camera = Camera ( Vector3D ( 0.0F, 0.0F, -18.0F ), Vector3D ( 0.0F, 0.0F, 0.0F ) );

	Texture2D texture ( GL_TEXTURE_RECTANGLE_ARB, new TextureData2D ( 512, 512, 4 ) );

	texture.Setup ( );

	RenderBuffer renderbuffer;

	renderbuffer.Setup ( );

#ifdef USE_FB

	FrameBuffer framebuffer;

	framebuffer.ColorBuffers.push_back ( &texture );

	framebuffer.RenderBuffers.push_back ( &renderbuffer );

	framebuffer.Setup ( );

#endif

	//---------------------------------------------------------------------------------------------

    glfwSwapInterval ( 0 );

	glfwSetMousePosCallback( MouseMove );

	glfwSetMouseButtonCallback( MouseButton );

	glfwSetKeyCallback ( KeyButton );

	//---------------------------------------------------------------------------------------------
	
	bool running = GL_TRUE;

	int frames = 0;

	t0 = glfwGetTime ( );

    while ( running )
	{
        t = glfwGetTime ( );

        if ( ( t-t0 ) > 1.0 || frames == 0 )
        {
            fps = (double) frames / ( t - t0 );

            sprintf_s ( titlestr, "Implicity Surfaces (%.1f FPS)", fps );

            glfwSetWindowTitle ( titlestr );

            t0 = t;

            frames = 0;
        }

        frames++;

		//-----------------------------------------------------------------------------------------

        glfwGetWindowSize ( &width, &height );

        height = height > 0 ? height : 1;

        glViewport ( 0, 0, width, height );
		
		//-----------------------------------------------------------------------------------------
		
		mouse.Apply ( camera );
		
		keyboard.Apply ( camera );
		
		camera.Setup ( );

		//-----------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------

		glEnable ( GL_DEPTH_TEST );

		glDisable ( GL_LIGHTING );

#ifdef USE_FB

		framebuffer.Bind ( );

		texture.Unbind ( );

#endif

		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glColor3f ( 1.0F, 0.0F, 0.0F );
		
		glBegin ( GL_LINES );

			glVertex3f ( 0, 0, 0 );
			glVertex3f ( 20, 0, 0 );

		glEnd ( );
		
		glColor3f ( 0.0F, 1.0F, 0.0F );
		
		glBegin ( GL_LINES );
		
			glVertex3f ( 0, 0, 0 );
			glVertex3f ( 0, 20, 0 );
		
		glEnd ( );
		
		glColor3f ( 0.0F, 0.0F, 1.0F );
		
		glBegin ( GL_LINES );
		
			glVertex3f ( 0, 0, 0 );
			glVertex3f ( 0, 0, 20 );
		
		glEnd ( );

		glEnable ( GL_LIGHTING );

		glColor3f ( 0.5F, 0.5F, 1.0F );
		
		GLUquadric * q = gluNewQuadric ( );
		
		gluSphere ( q, 2.0, 80, 80 );
		
		gluDeleteQuadric ( q );

#ifdef USE_FB

		framebuffer.Unbind ( );

#endif

		//-----------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------

		glMatrixMode ( GL_PROJECTION );

		glLoadIdentity ( );

		glOrtho ( -1.0F, 1.0F, -1.0F, 1.0F, -1.0F, 1.0F  );

		glMatrixMode ( GL_MODELVIEW );
		
		glLoadIdentity ( );

		//-----------------------------------------------------------------------------------------
		
		glDisable ( GL_LIGHTING );

		glDisable ( GL_DEPTH_TEST );

		//-----------------------------------------------------------------------------------------

		texture.Bind ( );

		glClear ( GL_COLOR_BUFFER_BIT );

		glColor3f ( 1.0F, 1.0F, 1.0F );

		glBegin ( GL_QUADS );

			glTexCoord2f( 0.0F,   0.0F );     glVertex2f ( -1.0F,  -1.0F );
			glTexCoord2f( 0.0F,   512.0F );   glVertex2f ( -1.0F,   1.0F );
			glTexCoord2f( 512.0F, 512.0F );   glVertex2f (  1.0F,   1.0F );
			glTexCoord2f( 512.0F, 0.0F );     glVertex2f (  1.0F,  -1.0F );

		glEnd ( );

		//-----------------------------------------------------------------------------------------

        glfwSwapBuffers();		
		
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

    glfwTerminate();

    return 0;
}