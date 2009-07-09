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

#include <math.h>

#include <time.h>

#include <stdlib.h>

using namespace Math;

using namespace Render;

//-------------------------------------------------------------------------------------------------

const int CountX = 256;

const int CountY = 256;

//-------------------------------------------------------------------------------------------------

TextureData2D * photonsTextureData = NULL;

Texture2D * positionsTexture = NULL;

FrameBuffer * photonFrameBuffer = NULL;

//-------------------------------------------------------------------------------------------------

Camera * camera = NULL;

Mouse mouse ( 0.005F );

Keyboard keyboard ( 0.1F );

//-------------------------------------------------------------------------------------------------

void MouseMove ( int x, int y )
{
	mouse.MouseMove ( x, y );
}
		
void MouseButton ( int button, int state )
{
	mouse.StateChange ( button, state );
}

void KeyButton ( int key, int state )
{
	keyboard.StateChange ( key, state );
}

//-------------------------------------------------------------------------------------------------

int Compare ( const void * a, const void * b )
{
	Vector3D * left = ( Vector3D * ) a;
	
	Vector3D * right = ( Vector3D * ) b;

	if ( left->X > right->X ) return 1;
	
	else if ( left->X == right->X && left->Y > right->Y ) return 1;
	
	else if ( left->X == right->X && left->Y == right->Y && left->Z > right->Z ) return 1;
	
	else if ( left->X == right->X && left->Y == right->Y && left->Z == right->Z ) return 0;
	
	return -1;
}

//-------------------------------------------------------------------------------------------------

void GenerateStream ( int width, int height )
{
	glViewport ( 0, 0, width, height );

	glClear ( GL_COLOR_BUFFER_BIT );

	glBegin ( GL_QUADS );

		glVertex2f ( -1.0F, -1.0F );
		glVertex2f ( -1.0F,  1.0F );
		glVertex2f (  1.0F,  1.0F );
		glVertex2f (  1.0F, -1.0F );

	glEnd ( );
}

//-------------------------------------------------------------------------------------------------

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

	//---------------------------------------------------------------------------------------------
	
	glMatrixMode( GL_PROJECTION );
	
	glLoadIdentity ( );
	
	glOrtho ( -1.0F, 1.0F, -1.0F, 1.0F, -1.0F, 1.0F  );

	glMatrixMode ( GL_MODELVIEW );

	//---------------------------------------------------------------------------------------------

	camera = new Camera ( Vector3D ( -6.0F, 5.0F, 14.0F ), Vector3D ( -0.8F, 3.2F, 0.0F ) );

	camera->SetFrustum ( );

	camera->SetViewport ( width, height );

	//---------------------------------------------------------------------------------------------

    glfwSwapInterval ( 0 );

	glfwSetMousePosCallback ( MouseMove );

	glfwSetMouseButtonCallback ( MouseButton );

	glfwSetKeyCallback ( KeyButton );

	//---------------------------------------------------------------------------------------------
	
	photonsTextureData = new TextureData2D ( CountX, CountY, 4 );

	positionsTexture = new Texture2D ( photonsTextureData, 0 , GL_TEXTURE_RECTANGLE_ARB );

	positionsTexture->Setup ( );

	//---------------------------------------------------------------------------------------------

	ShaderManager * PhotonManager = new ShaderManager ( );

	PhotonManager->LoadVertexShader ( "PhotonMapping.vs" );

	PhotonManager->LoadFragmentShader ( "PhotonMapping.fs" );

	PhotonManager->BuildProgram ( );

	ShaderManager * RayTracingManager = new ShaderManager ( );

	RayTracingManager->LoadVertexShader( "RayTracing.vs" );

	RayTracingManager->LoadFragmentShader( "RayTracing.fs" );

	RayTracingManager->BuildProgram ( );

	//---------------------------------------------------------------------------------------------

	photonFrameBuffer = new FrameBuffer ( );

	photonFrameBuffer->ColorBuffers.push_back ( positionsTexture );

	photonFrameBuffer->Setup ( );

	//---------------------------------------------------------------------------------------------

	PhotonManager->Bind ( );

	PhotonManager->SetUniformVector ( "Light.Position", Vector3D ( 0.0F, 0.0F, 4.5F ) );

	PhotonManager->SetUniformVector ( "Light.Radius", Vector2D ( 2.5F, 2.5F ) );

	PhotonManager->SetUniformFloat ( "Light.Distance", 3.0F );

	PhotonManager->Unbind ( );

	//---------------------------------------------------------------------------------------------

	RayTracingManager->Bind();

	RayTracingManager->SetUniformVector ( "Light.Position", Vector3D ( 0.0F, 0.0F, 4.5F ) );

	RayTracingManager->SetUniformVector( "Size", Vector2D ( CountX, CountY ) );

	RayTracingManager->SetTexture ( "PositionTexture", positionsTexture );

	RayTracingManager->Unbind ( );

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

		mouse.Apply ( camera );

		keyboard.Apply ( camera );

		//-----------------------------------------------------------------------------------------

		photonFrameBuffer->Bind ( );

		PhotonManager->Bind ( );

		PhotonManager->SetUniformFloat ( "Time", t );
		
		GenerateStream ( CountX, CountY );

		PhotonManager->Unbind ( );

		photonFrameBuffer->FetchOutcome ( );

		photonFrameBuffer->Unbind ( );

		//-----------------------------------------------------------------------------------------

		qsort ( *photonsTextureData,
			    photonsTextureData->GetWidth ( ) * photonsTextureData->GetHeight ( ),
				sizeof ( Vector4D ),
				Compare );

		positionsTexture->Update ( );

		//-----------------------------------------------------------------------------------------

		RayTracingManager->Bind ( );

		camera->SetShaderData ( RayTracingManager );

		RayTracingManager->SetUniformFloat ( "Time", t );

		GenerateStream ( width, height );

		RayTracingManager->Unbind ( );

		//-----------------------------------------------------------------------------------------

		glfwSwapBuffers ( );

		running = !glfwGetKey ( GLFW_KEY_ESC ) && glfwGetWindowParam ( GLFW_OPENED );
	}

    glfwTerminate ( );

    return 0;
}