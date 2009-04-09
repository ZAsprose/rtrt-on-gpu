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

#include "FrameBuffer.h"

using namespace Render;

using namespace Math;

#define CountX  128

#define CountY  128

TextureData2D* photons = NULL;

Texture2D* positions = NULL;

Texture2D* colors = NULL;

FrameBuffer* photonProgram = NULL;
//=================================================================================================

Camera cam;

Mouse mouse ( 0.005F );

Keyboard keyboard ( 0.005F );

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
    int width = 512, height = 512, running, frames;

    double t, t0, fps;

    glfwInit();

    if( !glfwOpenWindow( width, height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW ) )
    {
        glfwTerminate();

        return 0;
	}
	
	photons = new TextureData2D(CountX,CountY,4);

	positions = new Texture2D(photons, 1 , GL_TEXTURE_RECTANGLE_ARB);

	colors = new Texture2D(photons, 2 , GL_TEXTURE_RECTANGLE_ARB);

	positions->Setup();

	colors->Setup();

	
	//---------------------------------------------------------------------------------------------

	ShaderManager * manager = new ShaderManager ( );

	manager->LoadVertexShader ( "Vertex2.vs" );

	manager->LoadFragmentShader ( "Fragment2.fs" );

	manager->BuildProgram ( );

	manager->Bind();

	manager->SetTexture("Position",positions);

	manager->SetTexture("Color",colors);

	manager->Unbind();

	photonProgram = new FrameBuffer();

	photonProgram->ColorBuffers.push_back(positions);

	photonProgram->ColorBuffers.push_back(colors);

	photonProgram->Setup();

	//---------------------------------------------------------------------------------------------

	manager->Bind ( );

	manager->SetUniformVector ( "Plane.Center", Vector3D ( 0.0F, -5.0F, 0.0F ) );

	manager->SetUniformVector ( "Plane.Size", Vector2D (  20.0F,  20.0F ) );

	manager->SetUniformVector ( "Sphere.Center", Vector3D (  0.0F,  -3.0F,  0.0F ) );

	manager->SetUniformFloat ( "Sphere.Radius", 2.0F );

	manager->SetUniformVector("Light.Position", Vector3D(0.0F, 7.0F, 0.0F));

	manager->SetUniformFloat("Light.distance", 3.0F);

	manager->SetUniformVector("Light.HalfSize", Vector2D(20.0F, 20.0F));

	manager->SetUniformVector("Light.Intens", Vector3D(1.0f,1.0f,1.0f) / (20.0F * 20.0F));//На то ли число делим?

	manager->SetTexture("Position",positions);

	manager->SetTexture("Color",colors);

	manager->Unbind ( );

	//---------------------------------------------------------------------------------------------

	cam = Camera ( Vector3D ( 0.0F, 0.0F, -18.0F ), Vector3D ( 0.0F, 0.0F, 0.0F ) );

	cam.SetFrustum ( );

	cam.SetViewport ( width, height );

	//---------------------------------------------------------------------------------------------

    glfwSwapInterval( 0 );

	glfwSetMousePosCallback( MouseMove );

	glfwSetMouseButtonCallback( MouseButton );

	glfwSetKeyCallback ( KeyButton );

	//---------------------------------------------------------------------------------------------
	
	running = GL_TRUE;

	frames = 0;

	t0 = glfwGetTime ( );

    while ( running )
    {
        t = glfwGetTime();

        if ( ( t-t0 ) > 1.0F || frames == 0 )
        {
            fps = ( double ) frames / ( t - t0 );

			cout << "FPS: " << fps << endl;

            t0 = t;

            frames = 0;
        }

        frames++;

		//-----------------------------------------------------------------------------------------

        glfwGetWindowSize ( &width, &height );

        height = height > 0 ? height : 1;

        cam.SetViewport ( width, height );

		//-----------------------------------------------------------------------------------------

        glMatrixMode ( GL_PROJECTION );

        glLoadIdentity ( );

		glOrtho ( -1.0F, 1.0F, -1.0F, 1.0F, -1.0F, 1.0F  );

		//-----------------------------------------------------------------------------------------

		mouse.Apply ( cam );

		keyboard.Apply ( cam );

		//-----------------------------------------------------------------------------------------

		manager->Bind ( );

		glClear ( GL_COLOR_BUFFER_BIT );

		cam.SetShaderData ( manager );

		glBegin ( GL_QUADS );

			glVertex2f ( -1.0F, -1.0F );
			glVertex2f ( -1.0F,  1.0F );
			glVertex2f (  1.0F,  1.0F );
			glVertex2f (  1.0F, -1.0F );

		glEnd ( );

		manager->Unbind ( );

        glfwSwapBuffers();

		//-----------------------------------------------------------------------------------------
		
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

    glfwTerminate();

    return 0;///
}