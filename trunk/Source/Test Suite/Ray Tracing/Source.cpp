#include <stdio.h>

#include <GLee.h>

#include <GL/glfw.h>

#include <Mouse.h>

#include <Keyboard.h>

#include <Plane.h>

#include <Sphere.h>

#include <Box.h>

#include <Mesh.h>

#include <StaticData.h>

#include <OBJLoader.h>

#include "TextureManager.h"

using namespace Math;

using namespace Render;

using namespace Raytracing;

//=================================================================================================

Mouse mouse ( 0.01F );

Keyboard keyboard ( 0.01F );

bool Mode;

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

	if ( key == GLFW_KEY_F5 && state > 0 )
	{
		Mode = !Mode;
	}
}

//=================================================================================================

int main ( void )
{
	int Width = 512;
	
	int Height = 512;
	
	bool Running = true;
	
	int Frames = 0;

    double Time = 0.0;
	
	double Start = 0.0;
	
	double FPS = 0.0;

    char Caption [200];

	//------------------------------------ Init OpenGl and GLFW -----------------------------------

	glfwInit();

	if( !glfwOpenWindow( Width, Height, 0, 0, 0, 0, 16, 0, GLFW_WINDOW ) )
    {
        glfwTerminate();

        return 0;
	}
	
	//glfwSwapInterval ( 0 );

	glfwSetMousePosCallback ( MouseMove );

	glfwSetMouseButtonCallback ( MouseButton );
	
	glfwSetKeyCallback ( KeyButton );

	//--------------------------- Setup Camera Position and Orientation ---------------------------

	Camera * camera = new Camera ( Vector3D ( -10.0F, 0.0F, 0.0F ),
		                           Vector3D ( ONEPI / 2.0F, ONEPI / 2.0F, 0.0F ) );

	camera->SetViewport ( Width, Height );

	camera->SetFrustum ( );

	//------------------------------ Loading Shaders for Ray Tracing ------------------------------

	ShaderManager * shaderManager = new ShaderManager ( );

	shaderManager->LoadVertexShader ( "Vertex.vs" );

	shaderManager->LoadFragmentShader ( "Fragment.fs" );

	shaderManager->BuildProgram ( );

	//----------------- Loading OBJ Model and Building Scene Primitives ( Meshes ) ----------------

	OBJModel * model = OBJLoader :: LoadOBJ( "C:/workspace/rtrt-on-gpu/trunk/Support/Girl/Girl.obj" );

	Vector3D minimum = model->GetMinimum ( );

	Vector3D maximum = model->GetMaximum ( );

	Vector3D size = maximum - minimum;

	float scale = 2.0F / max ( size.X, max ( size.Y, size.Z ) );

	Mesh ** meshes = new Mesh * [model->Groups.size ( )];

	for ( int index = 0; index < model->Groups.size ( ); index++ )
	{
		meshes [index] = new Mesh ( model, index, new Transform ( ), new Material ( ) );

		meshes [index]->Transformation->SetScale ( Vector3D ( scale, scale, scale ) );

		meshes [index]->Transformation->SetTranslation ( minimum * scale - Vector3D :: Unit );

		meshes [index]->Tesselate ( );
	}

	//-------------------------- Adding All Textures to Texture Manager ---------------------------

	TextureManager * textureManager = new TextureManager ( );

	for ( int index = 0; index < model->Textures.size ( ); index++ )
	{
		model->Textures [index]->Texture->Unit = index;

		model->Textures [index]->Texture->FilterMode.Magnification = GL_LINEAR;

		model->Textures [index]->Texture->FilterMode.Minification = GL_LINEAR;

		textureManager->Textures.push_back ( model->Textures [index]->Texture );
	}

	textureManager->SetupTextures ( );

	//----------------------------- Building Scene for GPU Ray Tracing ----------------------------

	Scene * scene = new Scene ( camera,
		new Volume ( minimum * scale + minimum * scale - Vector3D :: Unit,
		             maximum * scale + minimum * scale - Vector3D :: Unit ) );

	for ( int index = 0; index < model->Groups.size ( ); index++ )
	{
		scene->Primitives.push_back ( meshes [index] );
	}

	scene->Lights.push_back ( new Light ( 0, Vector3D ( -1.0F, -1.0F, 1.0F ) ) );

	scene->Lights.push_back ( new Light ( 1, Vector3D ( 1.0F, 1.0F, 1.0F ) ) );

	scene->BuildGrid ( 128, 128, 128 );

	//-------------------------- Generating Static Texture Data for Scene -------------------------

	StaticData * staticData = new StaticData ( );

	staticData->SetupTextures ( scene );

	//------------------------------- Setup Shader Uniform Variables ------------------------------

	shaderManager->Bind ( );

	staticData->SetShaderData ( shaderManager );

	textureManager->SetShaderData ( shaderManager );

	shaderManager->Unbind ( );

	//---------------------------------------------------------------------------------------------
	
	Start = glfwGetTime ( );

    while ( Running )
	{
		//------------------------------------ Calculating FPS ------------------------------------

		Time = glfwGetTime ( );

        if ( ( Time - Start ) > 1.0 || Frames == 0 )
        {
            FPS = ( double ) Frames / ( Time - Start );

			sprintf ( Caption, "GPU Ray Tracing [ %.1f FPS ]", FPS );

            glfwSetWindowTitle ( Caption );
			
			Start = Time;

            Frames = 0;
        }

        Frames++;		

		//------------------------- Moving Camera with Keyboard and Mouse -------------------------

		mouse.Apply ( camera );

		keyboard.Apply ( camera );

		//-------------------- Rendering Scene ( OpenGL or Ray Tracing Mode ) ---------------------

		if ( Mode )
		{
			//---------------- Setup Parallel Projection and Disabling Depth Test -----------------

			glDisable ( GL_DEPTH_TEST );

			glMatrixMode ( GL_PROJECTION );

			glLoadIdentity ( );

			glOrtho ( -1.0F, 1.0F, -1.0F, 1.0F, -1.0F, 1.0F );
			
			glMatrixMode ( GL_MODELVIEW );
			
			glLoadIdentity ( );

			//------------------------- Setup Shaders for GPU Ray Tracing -------------------------

			shaderManager->Bind ( );

			scene->SetShaderData ( shaderManager );

			camera->SetShaderData ( shaderManager );

			//------------------------------- Draw Screen-Size Quad -------------------------------

			glClear ( GL_COLOR_BUFFER_BIT );

			glBegin ( GL_QUADS );

				glVertex2f ( -1.0F, -1.0F );   glVertex2f ( -1.0F,  1.0F );

				glVertex2f (  1.0F,  1.0F );   glVertex2f (  1.0F, -1.0F );

			glEnd ( );

			shaderManager->Unbind ( );
		}
		else
		{
			//--------- Setup perspective projection ( from camera ) and enable depth test --------
			
			glEnable ( GL_DEPTH_TEST );
			
			camera->Setup ( );
			
			//------------------------------ Draw scene with OpenGL -------------------------------
			
			glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			
			scene->Draw ( );
		}

        glfwSwapBuffers ( );
		
		Running = !glfwGetKey ( GLFW_KEY_ESC ) && glfwGetWindowParam ( GLFW_OPENED );
	}

    glfwTerminate ( );

    return 0;
}