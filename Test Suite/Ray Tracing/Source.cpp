#include <stdio.h>

#include <GLee.h>

#include <GL/glfw.h>

#include <ShaderManager.h>

#include <Camera.h>

#include <Mouse.h>

#include <Keyboard.h>

#include <Plane.h>

#include <Sphere.h>

#include <Box.h>

#include <Mesh.h>

#include <StaticData.h>

#include <OBJModel.h>

#include <OBJLoader.h>

#include <TextureManager.h>

using namespace Math;

using namespace Render;

using namespace Raytracing;

using namespace std;

//=================================================================================================

Camera camera;

Mouse mouse ( 0.01F );

Keyboard keyboard ( 0.1F );

Vector3D position ( 4.0F, 4.0F, 4.0F );

bool Mode;

//=================================================================================================

void MouseMove ( int x, int y )
{
	mouse.MouseMove ( x, y );
}
		
void MouseButton ( int button, int state )
{
	mouse.StateChange ( state );
}

float SPEED = 1.0F;

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

		case GLFW_KEY_F5: if ( state > 0 ) Mode = !Mode; break;
	}
}

//=================================================================================================

int main ( void )
{
	int     width, height, running, frames;
    double  t, t0, fps;
    char    titlestr [ 200 ];

	//---------------------------------------------------------------------------------------------

    glfwInit();

    if( !glfwOpenWindow( 512, 512, 0, 0, 0, 0, 16, 0, GLFW_WINDOW ) )
    {
        glfwTerminate();

        return 0;
	}

	//---------------------------------------------------------------------------------------------
	
	TextureManager * textureManager = new TextureManager ( );

	//---------------------------------------------------------------------------------------------

	Sphere * sphere = new Sphere ( 2.0F, 30, 30, new Transform ( ), new Material ( ) );

	sphere->Transformation->SetScale ( Vector3D ( 1.0F, 2.0F, 3.0F ) );

	sphere->Transformation->SetTranslation ( Vector3D ( 2.0F, -2.0F, 3.0F ) );

	sphere->Transformation->SetOrientation( Vector3D ( 1.5F, 1.0F, 0.75F ) );

	sphere->Properties->Diffuse = Vector3D ( 0.0F, 0.1F, 0.1F );

	sphere->Properties->Specular = Vector3D ( 0.8F, 0.8F, 0.8F );

	sphere->Properties->Shininess = 8.0F;

	sphere->Tesselate ( );


	Plane * plane1 = new Plane ( Vector2D ( 20.0F, 20.0F ), new Transform ( ), new Material ( ) );

	plane1->Transformation->SetTranslation ( Vector3D ( 0.0F, 0.0F, 20.0F ) );

	plane1->Transformation->SetOrientation( Vector3D ( ONEPI, 0.0F, 0.0F ) );

	plane1->Properties->Diffuse = Vector3D ( 1.0F, 1.0F, 1.0F );

	plane1->Properties->Scale = Vector2D ( 2.0F, 2.0F );

	plane1->Tesselate ( );


	Plane * plane2 = new Plane ( Vector2D ( 20.0F, 20.0F ), new Transform ( ), new Material ( ) );

	plane2->Transformation->SetTranslation ( Vector3D ( 0.0F, 0.0F, -20.0F ) );

	plane2->Transformation->SetOrientation( Vector3D ( 0.0F, 0.0F, 0.0F ) );

	plane2->Properties->Diffuse = Vector3D ( 1.0F, 1.0F, 1.0F );

	plane2->Properties->Scale = Vector2D ( 2.0F, 2.0F );

	plane2->Tesselate ( );


	Plane * plane3 = new Plane ( Vector2D ( 20.0F, 20.0F ), new Transform ( ), new Material ( ) );

	plane3->Transformation->SetTranslation ( Vector3D ( 0.0F, -9.0F, 0.0F ) );

	plane3->Transformation->SetOrientation( Vector3D ( ONEPI / 2.0F, 0.0F, 0.0F ) );

	plane3->Properties->Diffuse = Vector3D ( 0.5F, 0.5F, 0.5F );

	plane3->Properties->Scale = Vector2D ( 1.0F, 1.0F );

	plane3->Tesselate ( );


	Plane * plane4 = new Plane ( Vector2D ( 20.0F, 20.0F ), new Transform ( ), new Material ( ) );

	plane4->Transformation->SetTranslation ( Vector3D ( 0.0F, 20.0F, 0.0F ) );

	plane4->Transformation->SetOrientation( Vector3D ( -ONEPI / 2.0F, 0.0F, 0.0F ) );

	plane4->Properties->Diffuse = Vector3D ( 1.0F, 1.0F, 1.0F );

	plane4->Properties->Scale = Vector2D ( 1.0F, 1.0F );

	plane4->Tesselate ( );


	Plane * plane5 = new Plane ( Vector2D ( 20.0F, 20.0F ), new Transform ( ), new Material ( ) );

	plane5->Transformation->SetTranslation ( Vector3D ( 20.0F, 0.0F, 0.0F ) );

	plane5->Transformation->SetOrientation( Vector3D ( 0.0F, ONEPI / 2.0F, 0.0F ) );

	plane5->Properties->Diffuse = Vector3D ( 1.0F, 1.0F, 1.0F );

	plane5->Properties->Scale = Vector2D ( 2.0F, 2.0F );

	plane5->Tesselate ( );


	Plane * plane6 = new Plane ( Vector2D ( 20.0F, 20.0F ), new Transform ( ), new Material ( ) );

	plane6->Transformation->SetTranslation ( Vector3D ( -20.0F, 0.0F, 0.0F ) );

	plane6->Transformation->SetOrientation( Vector3D ( 0.0F, -ONEPI / 2.0F, 0.0F ) );

	plane6->Properties->Diffuse = Vector3D ( 1.0F, 1.0F, 1.0F );

	plane6->Properties->Scale = Vector2D ( 2.0F, 2.0F );

	plane6->Tesselate ( );


	Box * box1 = new Box ( Vector3D ( 1.0F, 1.0F, 1.0F ), new Transform ( ), new Material ( ) );
	
	box1->Transformation->SetTranslation ( Vector3D ( 4.0F, 6.3F, -17.0F ) );

	box1->Transformation->SetOrientation( Vector3D ( 0.0F, 1.0F, 0.0F ) );

	box1->Properties->Diffuse = Vector3D ( 0.0F, 0.2F, 0.2F );

	box1->Properties->Refraction = Vector3D ( 0.8F, 0.8F, 0.8F );

	box1->Properties->Shininess = 64.0F;

	box1->Tesselate ( );


	Box * box2 = new Box ( Vector3D ( 1.0F, 2.0F, 1.0F ), new Transform ( ), new Material ( ) );
	
	box2->Transformation->SetTranslation ( Vector3D ( 0.0F, 7.3F, -17.0F ) );

	box2->Transformation->SetOrientation( Vector3D ( 0.0F, 0.5F, 0.0F ) );

	box2->Properties->Diffuse = Vector3D ( 0.0F, 0.2F, 0.2F );

	box2->Properties->Specular = Vector3D ( 0.6F, 0.6F, 0.6F );

	box2->Properties->Shininess = 64.0F;

	box2->Tesselate ( );


	OBJModel * model = OBJLoader :: LoadOBJ( "C:/test/Story/Story.obj" );


	for ( int i = 0; i < model->Textures.size ( ); i++ )
	{
		model->Textures [i]->Texture->Unit = i;

		model->Textures [i]->Texture->FilterMode.Magnification = GL_LINEAR;
		model->Textures [i]->Texture->FilterMode.Minification = GL_LINEAR;

		textureManager->ImageTextures.push_back ( model->Textures [i]->Texture );
	}

	textureManager->SetupTextures ( );


	Mesh ** meshes = new Mesh * [model->Groups.size ( )];

	for ( int i = 0; i < model->Groups.size ( ); i++ )
	{
		meshes [i] = new Mesh ( model, i, new Transform ( ), new Material ( ) );

		meshes [i]->Transformation->SetTranslation ( Vector3D ( -0.8F, -3.0F, 2.5F ) );
		
		meshes [i]->Transformation->SetScale ( Vector3D ( 1.0F / 400.0F, 1.0F / 400.0F, 1.0F / 400.0F ) );

		meshes [i]->Tesselate ( );
	}

	//Mesh * mesh2 = new Mesh ( OBJLoader :: LoadModel ( "H:/Projects/Support/Models/Lamp.obj" ),
	//	                     new Transform ( ), new Material ( ) );

	//mesh2->Transformation->SetTranslation ( Vector3D ( -3.0F, 5.5F, -15.0F ) );
	//
	//mesh2->Transformation->SetScale ( Vector3D ( 1.0F / 6.0F, 1.0F / 6.0F, 1.0F / 6.0F ) );

	//mesh2->Tesselate ( );

	//mesh2->Properties->Diffuse = Vector3D ( 1.0F, 0.5F, 0.0F );

	//mesh2->Properties->Shininess = 32.0F;


	Scene * scene = new Scene ( &camera, new Volume ( Vector3D ( -18.1F, -15.1F, -2.5F ),
		                                              Vector3D ( 16.1F, 14.1F, 13.5F ) ) );

	//scene->Primitives.push_back ( sphere );

	//scene->Primitives.push_back ( plane1 );
	//scene->Primitives.push_back ( plane2 );
	//scene->Primitives.push_back ( plane3 );
	//scene->Primitives.push_back ( plane4 );
	//scene->Primitives.push_back ( plane5 );
	//scene->Primitives.push_back ( plane6 );
	
	//scene->Primitives.push_back ( box1 );
	//scene->Primitives.push_back ( box2 );

	for ( int i = 0; i < model->Groups.size ( ); i++ )
	{
		scene->Primitives.push_back ( meshes [i] );
	}

	//scene->Primitives.push_back ( mesh2 );

	scene->Lights.push_back ( new Light (0, Vector3D ( -20.0F, -20.0F, 0.0F ) ) );
	scene->Lights.push_back ( new Light (1, Vector3D ( 20.0F, 20.0F, 0.0F ) ) );

	scene->BuildGrid ( 64, 64, 64 );

	//---------------------------------------------------------------------------------------------

	ShaderManager * manager = new ShaderManager ( );

	manager->LoadVertexShader ( "Vertex.vs" );

	manager->LoadFragmentShader ( "Fragment.fs" );

	manager->BuildProgram ( );

	//---------------------------------------------------------------------------------------------

	StaticData * data = new StaticData ( );

	data->SetupTextures ( scene );

	//---------------------------------------------------------------------------------------------

	manager->Bind ( );

	data->SetShaderData ( manager );

	textureManager->SetShaderData ( manager );

	manager->Unbind ( );

	//---------------------------------------------------------------------------------------------

	camera = Camera ( Vector3D ( 0.0F, 0.0F, -18.0F ), Vector3D ( 0.0F, 0.0F, 0.0F ) );

	camera.SetFrustum ( );

	//---------------------------------------------------------------------------------------------

    glfwSwapInterval ( 0 );

	glfwSetMousePosCallback ( MouseMove );
	glfwSetMouseButtonCallback ( MouseButton );
	glfwSetKeyCallback ( KeyButton );

	//---------------------------------------------------------------------------------------------
	
	running = GL_TRUE;

	frames = 0;

	t0 = glfwGetTime ( );

    while ( running )
    {
        t = glfwGetTime ( );

        if ( ( t-t0 ) > 1.0 || frames == 0 )
        {
            fps = ( double ) frames / ( t-t0 );

            sprintf_s ( titlestr, "GPU Ray Tracing Test (%.1f FPS)", fps );

            glfwSetWindowTitle( titlestr );

            t0 = t;

            frames = 0;
        }

        frames++;

		//-----------------------------------------------------------------------------------------

        glfwGetWindowSize ( &width, &height );

        height = height > 0 ? height : 1;

		camera.SetViewport ( width, height );

		//-----------------------------------------------------------------------------------------

		mouse.Apply ( camera );

		keyboard.Apply ( camera );

		if ( Mode )
		{
			mouse.Step = 0.01F;

			keyboard.Step = 0.1F;

			glDisable ( GL_DEPTH_TEST );
			
			glViewport ( 0, 0, width, height );

			glMatrixMode ( GL_PROJECTION );

			glLoadIdentity ( );

			glOrtho ( -1.0F, 1.0F, -1.0F, 1.0F, -1.0F, 1.0F  );
			
			glMatrixMode ( GL_MODELVIEW );
			
			glLoadIdentity ( );

			//-------------------------------------------------------------------------------------
			
			glClear ( GL_COLOR_BUFFER_BIT );

			manager->Bind ( );

			scene->SetShaderData ( manager );

			camera.SetShaderData ( manager );

			//-------------------------------------------------------------------------------------

			glBegin ( GL_QUADS );

				glVertex2f ( -1.0F, -1.0F );
				glVertex2f ( -1.0F,  1.0F );
				glVertex2f (  1.0F,  1.0F );
				glVertex2f (  1.0F, -1.0F );

			glEnd ( );

			manager->Unbind ( );
		}
		else
		{
			mouse.Step = 0.01F;

			keyboard.Step = 0.1F;

			camera.Setup ( );

			glEnable ( GL_DEPTH_TEST );

			glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			//-------------------------------------------------------------------------------------

			glColor3f ( 1.0F, 1.0F, 1.0F );

			Vector3D vmin = scene->Box->Minimum;

			Vector3D vmax = scene->Box->Maximum;
			
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

			//-------------------------------------------------------------------------------------

			scene->Draw ( );
		
		}

        glfwSwapBuffers();

		//-----------------------------------------------------------------------------------------
		
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

    glfwTerminate();

    return 0;
}