#include <stdio.h>

#include <GLee.h>

#include <GL/glfw.h>

#include "ShaderManager.h"

#include "Camera.h"

#include "Mouse.h"

#include "Keyboard.h"

#include "TextureData1D.h"

#include "Texture1D.h"

#include <Plane.h>

#include <Sphere.h>

#include <Box.h>

#include <LoaderOBJ.h>

using namespace Math;

using namespace Render;

using namespace Raytracing;

//=================================================================================================

Camera cam;

Mouse mouse ( 0.01F );

Keyboard keyboard ( 0.5F );

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

	OBJModel * model = LoaderOBJ :: LoadModel ( "C:/Web/BUNNY_1K.obj" );

	vector <Triangle*> trl;

	float scale = 20.0F;

	for ( int i = 0; i < model->FaceNumber; i++ )
	{
		
		Vertex * a = new Vertex ( scale * model->Vertices [ model->Faces[i].Vertex[A] - 1 ],
			                      model->Normals [ model->Faces[i].Normal[A] - 1 ] );

		Vertex * b = new Vertex ( scale * model->Vertices [ model->Faces[i].Vertex[B] - 1 ],
			                      model->Normals [ model->Faces[i].Normal[B] - 1 ] );

		Vertex * c = new Vertex ( scale * model->Vertices [ model->Faces[i].Vertex[C] - 1 ],
			                      model->Normals [ model->Faces[i].Normal[C] - 1 ] );
		
		Triangle * triangle = new Triangle ( a, b, c );

		trl.push_back ( triangle );
	}

    glfwInit();

    if( !glfwOpenWindow( 512, 512, 0, 0, 0, 0, 0, 0, GLFW_WINDOW ) )
    {
        glfwTerminate();

        return 0;
	}

	glEnable ( GL_TEXTURE_1D );

	//---------------------------------------------------------------------------------------------

	ShaderManager * manager = new ShaderManager ( );

	manager->LoadVertexShader ( "Vertex.vs" );

	manager->LoadFragmentShader ( "Fragment.fs" );

	manager->BuildProgram ( );

	//---------------------------------------------------------------------------------------------

	Sphere * sphere = new Sphere ( 1.0F, 25, 25, new Transform (), new Material () );

	//sphere->Transformation->SetScale ( Vector3D ( 1.0F, 2.0F, 3.0F ) );

	//sphere->Transformation->SetTranslation ( Vector3D ( 2.0F, -2.0F, 3.0F ) );

	//sphere->Transformation->SetOrientation( Vector3D ( 1.5F, 1.0F, 0.75F ) );

	sphere->Properties->Color = Vector3D ( 0.5F, 0.5F, 0.0F );

	sphere->Tesselate ( );

	//---------------------------------------------------------------------------------------------

	TextureData1D * data = new TextureData1D ( 4096, 3 );

	int index = 0;

	for ( int i = 0; i < trl.size (); i++ )
	{
		data->Pixel < Vector3D > ( index++ ) = trl[i]->VertexA->Position;

		data->Pixel < Vector3D > ( index++ ) = trl[i]->VertexB->Position;

		data->Pixel < Vector3D > ( index++ ) = trl[i]->VertexC->Position;
	}

	//---------------------------------------------------------------------------------------------

	Texture1D texture ( data, 0 );

	texture.Setup();

	//---------------------------------------------------------------------------------------------

	cam = Camera ( Vector3D ( 0.0F, 0.0F, -18.0F ), Vector3D ( 0.0F, 0.0F, 0.0F ) );

	cam.SetFrustum ( );

	//---------------------------------------------------------------------------------------------

    glfwSwapInterval( 0 );

	glfwSetMousePosCallback( MouseMove );
	glfwSetMouseButtonCallback( MouseButton );
	glfwSetKeyCallback ( KeyButton );

	//---------------------------------------------------------------------------------------------
	
	running = GL_TRUE;

	frames = 0;

	t0 = glfwGetTime();

    while ( running )
    {
        t = glfwGetTime();

        if ( ( t-t0 ) > 1.0 || frames == 0 )
        {
            fps = (double)frames / (t-t0);

            sprintf_s ( titlestr, "Implicity Surfaces (%.1f FPS)", fps );

            glfwSetWindowTitle( titlestr );
            t0 = t;
            frames = 0;
        }

        frames++;

		//-----------------------------------------------------------------------------------------

        glfwGetWindowSize ( &width, &height );

        height = height > 0 ? height : 1;

        glViewport ( 0, 0, width, height );

		//-----------------------------------------------------------------------------------------

        glMatrixMode ( GL_PROJECTION );

        glLoadIdentity ( );

		glOrtho ( -1.0F, 1.0F, -1.0F, 1.0F, -1.0F, 1.0F  );

		//-----------------------------------------------------------------------------------------

		mouse.Apply ( cam );

		keyboard.Apply ( cam );

		//-----------------------------------------------------------------------------------------
		
		glClear ( GL_COLOR_BUFFER_BIT );

		manager->Bind ( );

		manager->SetTexture ( "TestTexture", texture );

		manager->SetUniformInteger ( "Count", trl.size ( ) );

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

    return 0;
}