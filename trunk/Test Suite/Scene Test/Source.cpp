#include <GLee.h>

#include <GL/glfw.h>

#include <vector>

#include "Camera.h"

#include "Mouse.h"

#include "Keyboard.h"

#include "Plane.h"

#include "Sphere.h"

#include "Box.h"

#include "Scene.h"

//-------------------------------------------------------------------------------------------------

using namespace std;

using namespace Math;

using namespace Render;

using namespace Raytracing;

//-------------------------------------------------------------------------------------------------

Mouse mouse ( 0.005F );

Keyboard keyboard ( 0.05F );

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

	glEnable ( GL_DEPTH_TEST );
	
	glEnable ( GL_LIGHTING );

	glPointSize ( 3.0F );

	//---------------------------------------------------------------------------------------------

	Camera camera = Camera ( Vector3D ( 0.0F, 0.0F, -18.0F ), Vector3D ( 0.0F, 0.0F, 0.0F ) );
	
	camera.SetViewport ( width, height );
	
	camera.SetFrustum ( );

	//---------------------------------------------------------------------------------------------

	Sphere * sphere = new Sphere ( 2.0F, 50, 50, new Transform (), new Material () );

	sphere->Transformation->SetScale ( Vector3D ( 1.0F, 2.0F, 3.0F ) );

	sphere->Transformation->SetTranslation ( Vector3D ( 2.0F, -2.0F, 3.0F ) );

	sphere->Transformation->SetOrientation( Vector3D ( 1.5F, 1.0F, 0.75F ) );

	sphere->Properties->Color = Vector3D ( 0.5F, 0.5F, 0.0F );

	sphere->Tesselate ( );


	Plane * plane = new Plane ( Vector2D ( 10.0F, 10.0F ), new Transform (), new Material () );

	plane->Transformation->SetTranslation ( Vector3D ( 0.0F, 0.0F, 8.0F ) );

	plane->Transformation->SetOrientation( Vector3D ( 2.8F, 0.0F, 0.0F ) );

	plane->Properties->Color = Vector3D ( 0.0F, 0.5F, 0.5F );

	plane->Tesselate ( );


	Box * box = new Box ( Vector3D ( 2.0F, 2.0F, 2.0F ), new Transform (), new Material () );
	
	box->Transformation->SetTranslation ( Vector3D ( -5.0F, 0.0F, 0.0F ) );

	box->Transformation->SetOrientation( Vector3D ( 2.3F, 1.2F, 0.5F ) );

	box->Properties->Color = Vector3D ( 0.5F, 0.0F, 0.5F );

	box->Tesselate ( );


	Scene * scene = new Scene ( &camera );

	scene->Primitives.push_back ( sphere );
	scene->Primitives.push_back ( plane );
	scene->Primitives.push_back ( box );

	scene->Lights.push_back ( new Light (0, Vector3D ( 10.0F, 10.0F, -10.0F ) ) );
	scene->Lights.push_back ( new Light (1, Vector3D ( -10.0F, -10.0F, -10.0F ) ) );

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
		
		glDisable ( GL_LIGHTING );

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

		//-----------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------

		glColor3f ( 0.5F, 0.5F, 1.0F );
		
		scene->Draw ( );

		sphere->Draw ( );

		//-----------------------------------------------------------------------------------------

        glfwSwapBuffers();		
		
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

    glfwTerminate();

	delete scene;

    return 0;
}