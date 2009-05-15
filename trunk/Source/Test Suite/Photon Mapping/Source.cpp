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

#include "math.h"

using namespace Render;

using namespace Math;

#define CountX  64

#define CountY  64

TextureData2D* photonsTextureData = NULL;

TextureData2D* colorTextureData = NULL;

TextureData2D* noiseTextureData = NULL;

Texture2D* positionsTexture = NULL;

Texture2D* colorsTexture = NULL;

Texture2D* noiseTexture = NULL;

FrameBuffer* photonFrameBuffer = NULL;

//=================================================================================================

Camera * cam;

Mouse mouse ( 0.005F );

Keyboard keyboard ( 0.1F );

float time = 0.0f;

//=================================================================================================

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


//=================================================================================================

int main ( void )
{
    int width = 400, height = 400, running, frames;

    double t, t0, fps;

    glfwInit();

    if( !glfwOpenWindow( width, height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW ) )
    {
        glfwTerminate();

        return 0;
	}
	
	photonsTextureData = new TextureData2D(CountX,CountY,4);

	colorTextureData = new TextureData2D(CountX,CountY,4);

	noiseTextureData = TextureData2D :: FromTGA("Noise.tga");

	positionsTexture = new Texture2D(photonsTextureData, 0 , GL_TEXTURE_RECTANGLE_ARB);

	colorsTexture = new Texture2D(colorTextureData, 1 , GL_TEXTURE_RECTANGLE_ARB);

	noiseTexture = new Texture2D(noiseTextureData, 2 , GL_TEXTURE_2D);

	positionsTexture->Setup();

	colorsTexture->Setup();

	
	noiseTexture->Setup();

	
	//---------------------------------------------------------------------------------------------

	ShaderManager * PhotonManager = new ShaderManager ( );

	PhotonManager->LoadVertexShader ( "Vertex2.vs" );

	PhotonManager->LoadFragmentShader ( "Fragment2.fs" );

	PhotonManager->BuildProgram ( );

	PhotonManager->Bind();

	PhotonManager->SetTexture("Position",positionsTexture);

	PhotonManager->SetTexture("Color",colorsTexture);

	PhotonManager->Unbind();

	ShaderManager * RayTracingManager = new ShaderManager();

	RayTracingManager->LoadVertexShader( "Vertex.vs" );

	RayTracingManager->LoadFragmentShader( "Fragment.fs" );

	RayTracingManager->BuildProgram();

	ShaderManager * TestManager = new ShaderManager();

	TestManager->LoadVertexShader( "test.vs" );

	TestManager->LoadFragmentShader( "testFX.fs" );

	TestManager->BuildProgram();

	photonFrameBuffer = new FrameBuffer();

	photonFrameBuffer->ColorBuffers.push_back(positionsTexture);

	photonFrameBuffer->ColorBuffers.push_back(colorsTexture);

	photonFrameBuffer->Setup();

	//---------------------------------------------------------------------------------------------

	PhotonManager->Bind ( );

	PhotonManager->SetUniformVector ( "Plane.Center", Vector3D ( 0.0F, -5.0F, 0.0F ) );

	PhotonManager->SetUniformVector ( "Plane.Size", Vector2D (  30.0F,  30.0F ) );

	PhotonManager->SetUniformVector ( "Sphere.Center", Vector3D (  0.0F,  -2.0F,  0.0F ) );

	PhotonManager->SetUniformFloat ( "Sphere.Radius", 2.0F );

	PhotonManager->SetUniformVector("Light.Position", Vector3D(0.0F, 0.0F, 0.0F));

	PhotonManager->SetUniformFloat("Light.distance", 3.0F);

	PhotonManager->SetUniformVector("Light.HalfSize", Vector2D(20.0F, 20.0F));

	PhotonManager->SetUniformVector("Light.Intens", Vector3D(1.0f,1.0f,1.0f) /*/ (20.0F * 20.0F)*/);//делить не надо!!!!

	PhotonManager->SetTexture("Position",positionsTexture);

	PhotonManager->SetTexture("Color",colorsTexture);

	PhotonManager->Unbind ( );

	RayTracingManager->Bind();

	RayTracingManager->SetUniformVector("Camera.Position", Vector3D( 0.0f, 7.0f, 0.0f));

	RayTracingManager->SetUniformVector("Camera.Side", Vector3D( 1.0f,0.0f,0.0f));

	RayTracingManager->SetUniformVector("Camera.Up", Vector3D(0.0f,1.0f,0.0f));

	RayTracingManager->SetUniformVector("Camera.View", Vector3D(0.0f,0.0f,1.0f));

	RayTracingManager->SetUniformVector("Camera.Scale", Vector2D(0.0f,0.0f));

	RayTracingManager->SetUniformVector ( "Plane.Center", Vector3D ( 0.0F, -5.0F, 0.0F ) );

	RayTracingManager->SetUniformVector ( "Plane.Size", Vector2D (  30.0F,  30.0F ) );

	RayTracingManager->SetUniformVector ( "Sphere.Center", Vector3D (  0.0F,  -2.0F,  0.0F ) );

	RayTracingManager->SetUniformFloat ( "Sphere.Radius", 2.0F );

	RayTracingManager->SetUniformVector("Light.Position", Vector3D(0.0f,0.0f,0.0f));

	RayTracingManager->SetUniformVector("Light.Intens", Vector3D(1.0f,1.0f,1.0f) /*/ (20.0F * 20.0F)*/);

	RayTracingManager->SetTexture("NoiseTexture", noiseTexture);

	RayTracingManager->SetTexture("PositionTexture",positionsTexture);

	RayTracingManager->SetTexture("IntensityTexture",colorsTexture);

	RayTracingManager->Unbind();

	TestManager->Bind();
	
	TestManager->SetTexture("PositionTexture",positionsTexture);

	TestManager->SetTexture("IntensityTexture",colorsTexture);

	TestManager->Unbind();

	//---------------------------------------------------------------------------------------------

	cam = new Camera ( Vector3D ( 0.0F, 0.0F, -18.0F ), Vector3D ( 0.0F, 0.0F, 0.0F ) );

	cam->SetFrustum ( );

	cam->SetViewport ( width, height );

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

        cam->SetViewport ( width, height );

		//-----------------------------------------------------------------------------------------

		mouse.Apply ( cam );

		keyboard.Apply ( cam );

		++time;

		//-----------------------------------------------------------------------------------------

		glViewport (0, 0, CountX, CountY);

		glMatrixMode( GL_PROJECTION );

		glLoadIdentity ( );

		glOrtho ( -1.0F, 1.0F, -1.0F, 1.0F, -1.0F, 1.0F  );

		glMatrixMode(GL_MODELVIEW);

		glClear ( GL_COLOR_BUFFER_BIT );

		photonFrameBuffer->Bind();

		PhotonManager->Bind ( );

		glClear ( GL_COLOR_BUFFER_BIT );

		PhotonManager->SetUniformVector ( "Sphere.Center",
			Vector3D ( 5.0F * sinf ( time / 100.0F ),
			           -2.0F + sinf ( time / 20.0F ),
					   5.0F * cosf ( time / 100.0F ) ) );

		glBegin ( GL_QUADS );

			glVertex2f ( -1.0F, -1.0F );
			glVertex2f ( -1.0F,  1.0F );
			glVertex2f (  1.0F,  1.0F );
			glVertex2f (  1.0F, -1.0F );

		glEnd ( );

		PhotonManager->Unbind ( );

		photonFrameBuffer->FetchOutcome ( );

		photonFrameBuffer->Unbind();

		//-----------------------------------------------------------------------------------------

		//for ( int i = 0; i < photonsTextureData->GetWidth( ); i++ )
		//{
		//	for ( int j = 0; j < photonsTextureData->GetHeight( ); j++ )
		//	{
		//		cout << photonsTextureData->Pixel<Vector3D> ( i, j ) << endl;
		//	}
		//}

		positionsTexture->Update ( );

		//-----------------------------------------------------------------------------------------

		glViewport ( 0, 0, width, height );

		glMatrixMode( GL_PROJECTION );

		glLoadIdentity ( );

		glOrtho ( -1.0F, 1.0F, -1.0F, 1.0F, -1.0F, 1.0F  );

		glMatrixMode(GL_MODELVIEW);

		glClear ( GL_COLOR_BUFFER_BIT );

		RayTracingManager->Bind ( );

		cam->SetShaderData ( RayTracingManager );

		//RayTracingManager->SetUniformVector ( "Light.Position",
		//	Vector3D ( 10.0F * sinf ( time / 1500.0F ),
		//	           3.0F + 2.0F * sinf ( time / 500.0F ),
		//			   10.0F * cosf ( time / 1500.0F ) ) );

		RayTracingManager->SetUniformVector ( "Sphere.Center",
			Vector3D ( 5.0F * sinf ( time / 100.0F ),
			           -2.0F + sinf ( time / 20.0F ),
					   5.0F * cosf ( time / 100.0F ) ) );

		glBegin ( GL_QUADS );

			glVertex2f ( -1.0F, -1.0F );
			glVertex2f ( -1.0F,  1.0F );
			glVertex2f (  1.0F,  1.0F );
			glVertex2f (  1.0F, -1.0F );

		glEnd ( );

		RayTracingManager->Unbind ( );

        glfwSwapBuffers();
		
		//-----------------------------------------------------------------------------------------
		
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

    glfwTerminate();

    return 0;
}