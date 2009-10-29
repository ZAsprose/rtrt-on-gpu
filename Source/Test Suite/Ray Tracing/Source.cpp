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

	if ( key == GLFW_KEY_F9 )
	{
		//tree->Root = curr;

		//curr = tree->Root->Left;

		camera->SaveToFile ( "CamState.txt" );
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

	camera = new Camera ( );//Vector3D ( -2.0F, 0.0F, 0.0F ),
		                  //Vector3D ( ONE_PI / 2.0F, ONE_PI / 2.0F, 0.0F ) );

	camera->LoadFromFile ( "CamState.txt" );

	camera->SetViewport ( Width, Height );

	camera->SetFrustum ( );

	//----------------- Loading OBJ Model and Building Scene Primitives ( Meshes ) ----------------
	
	OBJModel * model = OBJLoader :: LoadOBJ( "H:/Projects/RTRT on GPU/Support/Models/Toasters/Toasters1.obj" );
	
	//OBJModel * model1 = OBJLoader :: LoadOBJ( "D:/Projects/RTRT on GPU/Support/Models/Figurine/Figurine.obj" );
	
	//OBJModel * model2 = OBJLoader :: LoadOBJ( "D:/Projects/RTRT on GPU/Support/Models/Horse/Horse.obj" );

	Vector3D minimum = model->GetMinimum ( ); 

	Vector3D maximum = model->GetMaximum ( );

	Vector3D size = maximum - minimum;

	float scale = 2.0F / max ( size.X, max ( size.Y, size.Z ) );

	Mesh ** meshes = new Mesh * [model->Groups.size ( )];

	minimum = Vector3D(minimum.X, minimum.Z, minimum.Y);
	maximum = Vector3D(maximum.X, maximum.Z, maximum.Y);

	for ( int index = 0; index < model->Groups.size ( ); index++ )
	{
		meshes [index] = new Mesh ( model, index, new Transform ( ) );

		meshes [index]->Transformation->SetScale ( Vector3D ( scale, scale, scale ) );

		meshes [index]->Transformation->SetOrientation ( Vector3D ( -ONE_PI / 2.0F, 0.0F, 0.0F ) );

		meshes [index]->Transformation->SetTranslation ( -scale * ( minimum + maximum ) / 2.0F );

		meshes [index]->Tesselate ( );
	}
	
	//{
	//	meshes [model->Groups.size ( )] = new Mesh ( model1, 0, new Transform ( ) );
	//	
	//	meshes [model->Groups.size ( )]->Transformation->SetScale ( Vector3D ( 1.0F / 850.0F, 1.0F / 850.0F, 1.0F / 850.0F ) );
	//	
	//	meshes [model->Groups.size ( )]->Transformation->SetTranslation ( Vector3D ( 0.35F, 0.1F, -0.29F ) );
	//	
	//	meshes [model->Groups.size ( )]->Tesselate ( );
	//}

	//----------------------------- Building Scene for GPU Ray Tracing ----------------------------
	
	Scene * scene = new Scene ( camera, new Volume ( scale * ( minimum - maximum ) / 2.0F,
		                                             scale * ( maximum - minimum ) / 2.0F ) );

       //Scene * scene = new Scene ( camera,
       //                            new Volume ( -0.645F * scale * size, 0.645F * scale * size ) );

	for ( int index = 0; index < model->Groups.size ( ) ; index++ )
	{
		scene->Primitives.push_back ( meshes [index] );
	}

	//TextureData2D * td1 = TextureData2D :: FromTGA ( "H:/Projects/RTRT on GPU/Support/Textures/Wood/Wood - 3.TGA" );
	//scene->TextureData.push_back ( td1 );

	//TextureData2D * td2 = TextureData2D :: FromTGA ( "H:/Projects/RTRT on GPU/Support/Textures/Wood/Wood - 5.TGA" );
	//scene->TextureData.push_back ( td2 );

	//TextureData2D * td3 = TextureData2D :: FromTGA ( "H:/Projects/RTRT on GPU/Support/Textures/Stone/Stone - 2.TGA" );
	//scene->TextureData.push_back ( td3 );

 //      {
 //               Plane * plane = new Plane ( 0.64F * Vector2D ( scale * size.X, scale * size.Y ) );

 //               plane->Transformation->SetTranslation ( -0.64F * scale * Vector3D ( 0.0F, 0.0F, size.Z ) );

 //               plane->Tesselate ( );

	//			plane->Properties->TextureData = td1;

	//			plane->Properties->TextureScale = Vector2D ( 4.0F, 2.0F ); 

 //               scene->Primitives.push_back ( plane );
 //       }

 //       {
 //               Plane * plane = new Plane ( 0.64F * Vector2D ( scale * size.X, scale * size.Y ) );

 //               plane->Transformation->SetTranslation ( 0.64F * scale * Vector3D ( 0.0F, 0.0F, size.Z ) );

 //               plane->Tesselate ( );

 //               plane->Properties->TextureData = td2;

	//			plane->Properties->TextureScale = Vector2D ( 2.0F, 1.0F );

 //               scene->Primitives.push_back ( plane );
 //       }

 //       {
 //               Plane * plane = new Plane ( 0.64F * Vector2D ( scale * size.X, scale * size.Z ) );

 //               plane->Transformation->SetTranslation ( -0.64F * scale * Vector3D ( 0.0F, size.Y, 0.0F ) );

 //               plane->Transformation->SetOrientation ( Vector3D ( -ONE_PI / 2.0F, 0.0F, 0.0F ) );

 //               plane->Tesselate ( );

 //               plane->Properties->TextureData = td3;

	//			plane->Properties->TextureScale = Vector2D ( 4.0F, 2.0F );

 //               scene->Primitives.push_back ( plane );
 //       }

 //       {
 //               Plane * plane = new Plane ( 0.64F * Vector2D ( scale * size.X, scale * size.Z ) );

 //               plane->Transformation->SetTranslation ( 0.64F * scale * Vector3D ( 0.0F, size.Y, 0.0F ) );

 //               plane->Transformation->SetOrientation ( Vector3D ( -ONE_PI / 2.0F, 0.0F, 0.0F ) );

 //               plane->Tesselate ( );

 //               plane->Properties->TextureData = td3;

	//			plane->Properties->TextureScale = Vector2D ( 4.0F, 2.0F );

 //               scene->Primitives.push_back ( plane );
 //       }

 //       {
 //               Plane * plane = new Plane ( 0.64F * Vector2D ( scale * size.Z, scale * size.Y ) );

 //               plane->Transformation->SetTranslation ( -0.64F * scale * Vector3D ( size.X, 0.0F, 0.0F ) );

 //               plane->Transformation->SetOrientation ( Vector3D ( 0.0F, -ONE_PI / 2.0F, 0.0F ) );

 //               plane->Tesselate ( );

 //               plane->Properties->TextureData = td3;

	//			//plane->Properties->TextureScale = Vector2D ( 4.0F, 2.0F );

 //               scene->Primitives.push_back ( plane );
 //       }

 //       {
 //               Plane * plane = new Plane ( 0.64F * Vector2D ( scale * size.Z, scale * size.Y ) );

 //               plane->Transformation->SetTranslation ( 0.64F * scale * Vector3D ( size.X, 0.0F, 0.0F ) );

 //               plane->Transformation->SetOrientation ( Vector3D ( 0.0F, ONE_PI / 2.0F, 0.0F ) );

 //               plane->Tesselate ( );

 //               plane->Properties->TextureData = td3;

	//			//plane->Properties->TextureScale = Vector2D ( 4.0F, 2.0F );

 //               scene->Primitives.push_back ( plane );
 //       }

	for ( int index = 0; index < model->Textures.size ( ); index++ )
	{
		scene->TextureData.push_back ( model->Textures [index]->Data );
	}

	scene->Lights.push_back ( new Light ( 0, Vector3D :: Zero ) );
	scene->Lights.push_back ( new Light ( 1, Vector3D :: Zero ) );

	scene->BuildGrid ( 128, 128, 128, true );

	//tree = new BVHTree ( );

	//tree->Build ( scene->Primitives );

	//tree->SetupTextures ( );

	//curr = tree->Root;

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

		//camera->Position.X = 1.4F * sin ( TWO_PI * ( Time - ST ) / 20.0F ); 
		//camera->Position.Y = 1.4F * cos ( TWO_PI * ( Time - ST ) / 20.0F );
		//camera->Position.Z = 0.4F;

		//camera->WorldToCamera = Matrix3D :: Rotate ( Vector3D ( ONE_PI / 2.0F, ONE_PI / 2.0F, 0.0F ) );
		//camera->Update ( );		
		//camera->RotateWorld ( -ONE_PI/2.0F - TWO_PI * ( Time - ST ) / 20.0F, Vector3D::AxisZ );
		//camera->RotateLocal ( 0.4F, Vector3D::AxisX );
		

		scene->Lights [0]->Position.X = -1.8F;// * sin ( 0.3F + TWO_PI * ( Time - ST ) / 20.0F ); 
		scene->Lights [0]->Position.Y = -1.8F;// * cos ( 0.3F + TWO_PI * ( Time - ST ) / 20.0F );
		scene->Lights [0]->Position.Z = 1.0F;

		scene->Lights [1]->Position.X = 0.3F;// * sin ( Time * 3.0F );
		scene->Lights [1]->Position.Y = 0;//0.3F * cos ( Time * 3.0F );
		scene->Lights [1]->Position.Z = 0;//0.5 + 0.3F * cos ( Time * 5.0F );

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

			//tree->DrawNode (curr );
		}

        glfwSwapBuffers ( );
		
		Running = !glfwGetKey ( GLFW_KEY_ESC ) && glfwGetWindowParam ( GLFW_OPENED );
	}

    glfwTerminate ( );

    return 0;
}