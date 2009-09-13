#include <Mouse.h>

#include <Keyboard.h>

#include <Plane.h>

#include <Sphere.h>

#include <Box.h>

#include <Mesh.h>

#include <StaticData.h>

#include <OBJLoader.h>

#include <time.h>

using namespace Math;

using namespace Render;

using namespace Raytracing;

//=================================================================================================

Mouse mouse ( 0.01F );

Keyboard keyboard ( 0.01F );

bool Mode = true;

bool Recompile;

bool Info;

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

	if ( key == GLFW_KEY_F6 && state > 0 )
	{
		Recompile = true;
	}

	if ( key == GLFW_KEY_F10 && state > 0 )
	{
		Info = true;
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

	//--------------------------- Setup Camera Position and Orientation ---------------------------

	Camera * camera = new Camera ( Vector3D ( -2.0F, 0.0F, 0.0F ),
		                           Vector3D ( ONE_PI / 2.0F, ONE_PI / 2.0F, 0.0F ) );

	//camera->Position = Vector3D (	0.0241486F,		0.392294F,		-0.0682123F );
	//camera->View = Vector3D (		-0.926735F,		0.375078F,		0.0218659F );
	//camera->Up = Vector3D (			0.0362941F,		0.0314469F,		0.998846F );
	//camera->Side = Vector3D (		-0.373961F,		-0.926458F,		0.0427565F );

	//cout << Length(camera->Position) << endl;
	//cout << Length(camera->View) << endl;
	//cout << Length(camera->Up) << endl;
	//cout << Length(camera->Side) << endl;

	camera->SetViewport ( Width, Height );

	camera->SetFrustum ( );

	//------------------------------ Loading Shaders for Ray Tracing ------------------------------

	ShaderManager * shaderManager = new ShaderManager ( );

	shaderManager->LoadVertexShader ( "Vertex.vs" );

	shaderManager->LoadFragmentShader ( "Fragment.fs" );

	shaderManager->BuildProgram ( );

	//----------------- Loading OBJ Model and Building Scene Primitives ( Meshes ) ----------------
	
	OBJModel * model = OBJLoader :: LoadOBJ( "D:/Projects/RTRT on GPU/Support/Models/House/House.obj" );

	OBJModel * model1 = OBJLoader :: LoadOBJ( "D:/Projects/RTRT on GPU/Support/Models/Figurine/Figurine.obj" );

	OBJModel * model2 = OBJLoader :: LoadOBJ( "D:/Projects/RTRT on GPU/Support/Models/Horse/Horse.obj" );

	Vector3D minimum = model->GetMinimum ( ); 

	Vector3D maximum = model->GetMaximum ( );

	Vector3D size = maximum - minimum;

	float scale = 2.0F / max ( size.X, max ( size.Y, size.Z ) );

	Mesh ** meshes = new Mesh * [model->Groups.size ( ) + model1->Groups.size ( ) + model2->Groups.size ( )];

	for ( int index = 0; index < model->Groups.size ( ); index++ )
	{
		meshes [index] = new Mesh ( model, index, new Transform ( ) );

		meshes [index]->Transformation->SetScale ( Vector3D ( scale, scale, scale ) );

		meshes [index]->Transformation->SetTranslation ( -scale * ( minimum + maximum ) / 2.0F );

		meshes [index]->Tesselate ( );
	}

	//{
	//	meshes [model->Groups.size ( )] = new Mesh ( model1, 0, new Transform ( ) );

	//	meshes [model->Groups.size ( )]->Transformation->SetScale ( Vector3D ( 1.0F / 1000.0F, 1.0F / 1000.0F, 1.0F / 1000.0F ) );

	//	meshes [model->Groups.size ( )]->Transformation->SetTranslation ( Vector3D ( 0.48F, 0.15F, -0.28F ) );

	//	meshes [model->Groups.size ( )]->Tesselate ( );
	//}

	//{
	//	meshes [model->Groups.size ( ) + 1] = new Mesh ( model2, 0, new Transform ( ) );

	//	meshes [model->Groups.size ( ) + 1]->Transformation->SetScale ( Vector3D ( 1.0F / 300.0F, 1.0F / 300.0F, 1.0F / 300.0F ) );

	//	meshes [model->Groups.size ( ) + 1]->Transformation->SetTranslation ( Vector3D ( 0.5F, -0.15F, -0.28F ) );

	//	meshes [model->Groups.size ( ) + 1]->Tesselate ( );
	//}

	/*
	OBJModel * model = OBJLoader :: LoadOBJ( "D:/Projects/RTRT on GPU/Support/Models/Lexus/Lexus.obj" );
	
	Vector3D minimum = model->GetMinimum ( ); 
	
	Vector3D maximum = model->GetMaximum ( );
	
	Vector3D size = maximum - minimum;
	
	float scale = 2.0F / max ( size.X, max ( size.Y, size.Z ) );
	
	Mesh ** meshes = new Mesh * [model->Groups.size ( )];
	
	for ( int index = 0; index < model->Groups.size ( ); index++ )
	{
		meshes [index] = new Mesh ( model, index, new Transform ( ), new Material ( ) );
		
		meshes [index]->Transformation->SetScale ( Vector3D ( scale, scale, scale ) );
		
		meshes [index]->Transformation->SetTranslation ( -scale * ( minimum + maximum ) / 2.0F );
		
		meshes [index]->Tesselate ( );
	}
	*/

	//----------------------------- Building Scene for GPU Ray Tracing ----------------------------
	
	Scene * scene = new Scene ( camera, new Volume ( scale * ( minimum - maximum ) / 2.0F,
		                                             scale * ( maximum - minimum ) / 2.0F ) );


	for ( int index = 0; index < model->Groups.size ( ) ; index++ )
	{
		scene->Primitives.push_back ( meshes [index] );
	}

	//-------------------------- Adding All Textures to Texture Manager ---------------------------

	for ( int index = 0; index < model->Textures.size ( ); index++ )
	{
		scene->TextureData.push_back ( model->Textures [index]->Data );
	}

	//scene->Primitives.push_back ( meshes [model->Groups.size ( )] );

	//scene->Primitives.push_back ( meshes [model->Groups.size ( ) + 1] );

	scene->Lights.push_back ( new Light ( 0, Vector3D ( 0.0F, -1.0F, 0.5F ) ) );

	scene->Lights.push_back ( new Light ( 1, Vector3D ( 1.0F, 0.0F, 0.5F ) ) );

	scene->BuildGrid ( 64, 64, 64 );

	//-------------------------- Generating Static Texture Data for Scene -------------------------

	StaticData * staticData = new StaticData ( );

	staticData->BuildData ( scene );

	//------------------------------- Setup Shader Uniform Variables ------------------------------

	shaderManager->Bind ( );

	staticData->SetShaderData ( shaderManager );

	shaderManager->Unbind ( );

	//---------------------------------------------------------------------------------------------
	
	Start = glfwGetTime ( );

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

		if ( Info )
		{
			//cout << "------------------------------------------" << endl;
			//cout << "Position = " << camera->GetPosition ( ) << endl;
			//cout << "View = " << camera->View << endl;
			//cout << "Up = " << camera->Up << endl;
			//cout << "Side = " << camera->Side << endl;

			//cout << "------------------------------------------" << endl;
			//cout << "Position = " << camera->GetPosition ( ) << endl;
			//cout << "View = " << camera->View << endl;
			//cout << "Up = " << camera->Up << endl;
			//cout << "Side = " << camera->Side << endl;

			Info = false;
		}

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