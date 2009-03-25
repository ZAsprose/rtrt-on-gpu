#include <GLee.h>

#include <GL/glfw.h>

#include <vector>

#include "Camera.h"

#include "Mouse.h"

#include "Keyboard.h"

#include "Intersector.h"

//-------------------------------------------------------------------------------------------------

using namespace std;

using namespace Math;

using namespace Render;

using namespace Raytracing;

//-------------------------------------------------------------------------------------------------

Mouse mouse ( 0.0025F );

Keyboard keyboard ( 0.001F );

Vector3D position;

//-------------------------------------------------------------------------------------------------

void MouseMove ( int x, int y )
{
	mouse.MouseMove ( x, y );
}
		
void MouseButton ( int button, int state )
{
	mouse.StateChange ( state );
}

float SPEED = 50000.0F;

void KeyButton ( int key, int state )
{
	keyboard.StateChange ( key, state );

		switch ( key )
		{
			case GLFW_KEY_UP: if ( state > 0 ) position += Vector3D::AxisY / SPEED; break;

			case GLFW_KEY_DOWN: if ( state > 0 ) position -= Vector3D::AxisY / SPEED; break;

			case GLFW_KEY_LEFT: if ( state > 0 ) position += Vector3D::AxisX / SPEED; break;

			case GLFW_KEY_RIGHT: if ( state > 0 ) position -= Vector3D::AxisX / SPEED; break;

			case GLFW_KEY_PAGEUP: if ( state > 0 ) position += Vector3D::AxisZ / SPEED; break;

			case GLFW_KEY_PAGEDOWN: if ( state > 0 ) position -= Vector3D::AxisZ / SPEED; break;

		}
}

//-------------------------------------------------------------------------------------------------

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

	glEnable ( GL_TEXTURE_2D );
	
	glEnable ( GL_COLOR_MATERIAL );

	glEnable ( GL_DEPTH_TEST );

	//---------------------------------------------------------------------------------------------

	Camera camera = Camera ( Vector3D ( 0.0F, 0.0F, -18.0F ), Vector3D ( 0.0F, 0.0F, 0.0F ) );
	
	camera.SetViewport ( width, height );
	
	camera.SetFrustum ( );

	//---------------------------------------------------------------------------------------------

	Triangle * triangle = new Triangle ( new Vertex ( Vector3D ( 0.1F, 0.3F, 0.2F ), Vector3D :: Unit ),
		                                 new Vertex ( Vector3D ( 1.5F, 0.6F, 0.3F ), Vector3D :: Unit ),
										 new Vertex ( Vector3D ( 0.3F, 1.1F, 0.2F ), Vector3D :: Unit ) );

	Voxel * voxel = new Voxel ( Vector3D :: Zero, Vector3D :: Unit );

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
            fps = ( double ) frames / ( t - t0 );

            sprintf_s ( titlestr, "Implicity Surfaces (%.1f FPS)", fps );

            glfwSetWindowTitle ( titlestr );

            t0 = t;

            frames = 0;
        }

        frames++;

		//-----------------------------------------------------------------------------------------

        glfwGetWindowSize ( &width, &height );

        height = height > 0 ? height : 1;
		
		//-----------------------------------------------------------------------------------------

		mouse.Apply ( camera );
		
		keyboard.Apply ( camera );
		
		camera.Setup ( );

		//-----------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------

		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glColor3f ( 1.0F, 0.0F, 0.0F );
		
		glBegin ( GL_LINES );

			glVertex3f ( 0, 0, 0 );
			glVertex3f ( 10, 0, 0 );

		glEnd ( );
		
		glColor3f ( 0.0F, 1.0F, 0.0F );
		
		glBegin ( GL_LINES );
		
			glVertex3f ( 0, 0, 0 );
			glVertex3f ( 0, 10, 0 );
		
		glEnd ( );
		
		glColor3f ( 0.0F, 0.0F, 1.0F );
		
		glBegin ( GL_LINES );
		
			glVertex3f ( 0, 0, 0 );
			glVertex3f ( 0, 0, 10 );
		
		glEnd ( );


		triangle->VertexA->Position += position;
		triangle->VertexB->Position += position;
		triangle->VertexC->Position += position;

		if ( Intersector :: TriangleVoxelOverlap ( triangle, voxel ) )
		{
			glColor3f ( 1.0F, 1.0F, 0.0F );
		}
		else
		{
			glColor3f ( 1.0F, 0.0F, 1.0F );
		}

		glBegin ( GL_TRIANGLES );
		
			triangle->Draw ( );
		
		glEnd ( );



		Vector3D vmin = voxel->Position - voxel->HalfSize;

		Vector3D vmax = voxel->Position + voxel->HalfSize;

		glColor3f ( 1.0F, 1.0F, 1.0F );
		
		glBegin ( GL_LINE_LOOP );
		
			glVertex3f ( vmin.X, vmin.Y, vmin.Z );
			glVertex3f ( vmax.X, vmin.Y, vmin.Z );
			glVertex3f ( vmax.X, vmax.Y, vmin.Z );
			glVertex3f ( vmin.X, vmax.Y, vmin.Z );
		
		glEnd ( );

		glBegin ( GL_LINE_LOOP );
		
			glVertex3f ( vmin.X, vmin.Y, vmax.Z );
			glVertex3f ( vmax.X, vmin.Y, vmax.Z );
			glVertex3f ( vmax.X, vmax.Y, vmax.Z );
			glVertex3f ( vmin.X, vmax.Y, vmax.Z );
		
		glEnd ( );

		glBegin ( GL_LINES );
		
			glVertex3f ( vmin.X, vmin.Y, vmin.Z );
			glVertex3f ( vmin.X, vmin.Y, vmax.Z );
			glVertex3f ( vmax.X, vmin.Y, vmin.Z );
			glVertex3f ( vmax.X, vmin.Y, vmax.Z );
			glVertex3f ( vmax.X, vmax.Y, vmin.Z );
			glVertex3f ( vmax.X, vmax.Y, vmax.Z );
			glVertex3f ( vmin.X, vmax.Y, vmin.Z );
			glVertex3f ( vmin.X, vmax.Y, vmax.Z );
		
		glEnd ( );

		//-----------------------------------------------------------------------------------------

        glfwSwapBuffers();		
		
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

    glfwTerminate();

    return 0;
}