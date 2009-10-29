#include <Mouse.h>

#include <Keyboard.h>

#include <Plane.h>

#include <Sphere.h>

#include <Box.h>

#include <Mesh.h>

#include <StaticData.h>

#include <OBJLoader.h>

#include <BVHTree.h>

#include <time.h>

using namespace Math;

using namespace Render;

using namespace Raytracing;

//=================================================================================================

Mouse mouse ( 0.01F );

Keyboard keyboard ( 0.01F );

bool Mode = true;

bool Recompile = true;

//=================================================================================================

void MouseMove ( int x, int y )
{
	mouse.MouseMove ( x, y );
}
		
void MouseButton ( int button, int state )
{
	mouse.StateChange ( button, state );
}

BVHTree * tree = NULL;

BVHNode * curr = NULL;

Camera * camera = NULL;

void KeyButton ( int key, int state )
{
	keyboard.StateChange ( key, state );

	if ( key == GLFW_KEY_F5 && state > 0 )
	{
		Mode = !Mode;
	}

	if ( key == GLFW_KEY_F6 && state > 0 )
	{
		Recompile = true;
	}

	if ( key == GLFW_KEY_F7 )
	{
		curr = tree->Root->Left;
	}

	if ( key == GLFW_KEY_F8 )
	{
		curr = tree->Root->Right;
	}
}

//=================================================================================================

int main ( void )
{
	int Width = 512;
	
	int Height = 512;
	
	bool Running = true;
	
	int Frames = 0;

    double Time, DeltaT = 0.0;
	
	double Start = 0.0;
	
	double FPS = 0.0;

    char Caption [200];

	//------------------------------------ Init OpenGl and GLFW -----------------------------------

	glfwInit ( );

	if( !glfwOpenWindow( Width, Height, 0, 0, 0, 0, 16, 0, GLFW_WINDOW ) )
    {
        glfwTerminate();

        return 0;
	}
	
	glfwSwapInterval ( 0 );

	glfwSetMousePosCallback ( MouseMove );

	glfwSetMouseButtonCallback ( MouseButton );
	
	glfwSetKeyCallback ( KeyButton );

	ShaderManager * shaderManager = new ShaderManager ( );

	//--------------------------- Setup Camera Position and Orientation ---------------------------

	camera = new Camera ( Vector3D ( -2.0F, 0.0F, 0.0F ),
		                  Vector3D ( ONE_PI / 2.0F, ONE_PI / 2.0F, 0.0F ) );

	camera->SetViewport ( Width, Height );

	camera->SetFrustum ( );

	//----------------- Loading OBJ Model and Building Scene Primitives ( Meshes ) ----------------
	
	OBJModel * model = OBJLoader :: LoadOBJ( "D:/Projects/RTRT on GPU/Support/Models/House/House.obj" );

	Vector3D minimum = model->GetMinimum ( ); 

	Vector3D maximum = model->GetMaximum ( );

	Vector3D size = maximum - minimum;

	float scale = 2.0F / max ( size.X, max ( size.Y, size.Z ) );

	Mesh ** meshes = new Mesh * [model->Groups.size ( )];

	for ( int index = 0; index < model->Groups.size ( ); index++ )
	{
		meshes [index] = new Mesh ( model, index, new Transform ( ) );

		meshes [index]->Transformation->SetScale ( Vector3D ( scale, scale, scale ) );
		
		meshes [index]->Transformation->SetTranslation ( -scale * ( minimum + maximum ) / 2.0F );
		
		meshes [index]->Tesselate ( );
	}

	//----------------------------- Building Scene for GPU Ray Tracing ----------------------------
	
	Scene * scene = new Scene ( camera, new Volume ( scale * ( minimum - maximum ) / 2.0F,
		                                             scale * ( maximum - minimum ) / 2.0F ) );

	for ( int index = 0; index < model->Groups.size ( ) ; index++ )
	{
		scene->Primitives.push_back ( meshes [index] );
	}

	for ( int index = 0; index < model->Textures.size ( ); index++ )
	{
		scene->TextureData.push_back ( model->Textures [index]->Data );
	}

	scene->Lights.push_back ( new Light ( 0, Vector3D :: Zero ) );

	scene->Lights.push_back ( new Light ( 1, Vector3D :: Zero ) );

	scene->BuildGrid ( 128, 128, 128, true );

	//-------------------------- Generating Static Texture Data for Scene -------------------------

	StaticData * staticData = new StaticData ( );

	staticData->BuildData ( scene );

	//---------------------------------------------------------------------------------------------

	Start = glfwGetTime ( );

	double ST = Start;

	while ( Running )
	{
		if ( Recompile )
		{
			shaderManager->LoadVertexShader ( "Vertex.vs" );

			shaderManager->LoadFragmentShader ( "Fragment.fs" );

			shaderManager->BuildProgram ( );

			shaderManager->Bind ( );

			staticData->SetShaderData ( shaderManager );

			shaderManager->Unbind ( );

			Recompile = false;
		}

		//------------------------------------ Calculating FPS ------------------------------------

		Time = glfwGetTime ( );

		DeltaT = Time - Start;

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

		scene->Lights [0]->Position.X = -1.8F * sin ( 0.3F + TWO_PI * ( Time - ST ) / 20.0F ); 
		scene->Lights [0]->Position.Y = -1.8F * cos ( 0.3F + TWO_PI * ( Time - ST ) / 20.0F );
		scene->Lights [0]->Position.Z = 2.0F;

		scene->Lights [1]->Position.X = 0.0F;
		scene->Lights [1]->Position.Y = 0.1F * cos ( Time * 3.0F );
		scene->Lights [1]->Position.Z = 0.0F;

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