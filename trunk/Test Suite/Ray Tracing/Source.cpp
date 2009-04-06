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

#include <Data.h>

#include <OBJLoader.h>

using namespace Math;

using namespace Render;

using namespace Raytracing;

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

	glEnable ( GL_TEXTURE_1D );

	//---------------------------------------------------------------------------------------------

	OBJModel * model = OBJLoader :: LoadModel ( "C:\\Web\\DUCK.obj" );

	//---------------------------------------------------------------------------------------------

	Sphere * sphere = new Sphere ( 2.0F, 100, 100, new Transform (), new Material () );

	sphere->Transformation->SetScale ( Vector3D ( 1.0F, 2.0F, 3.0F ) );

	sphere->Transformation->SetTranslation ( Vector3D ( 2.0F, -2.0F, 3.0F ) );

	sphere->Transformation->SetOrientation( Vector3D ( 1.5F, 1.0F, 0.75F ) );

	sphere->Properties->Color = Vector3D ( 0.5F, 0.5F, 0.0F );

	sphere->Properties->Shininess = 8.0F;

	sphere->Tesselate ( );


	Plane * plane = new Plane ( Vector2D ( 10.0F, 10.0F ), new Transform (), new Material () );

	plane->Transformation->SetTranslation ( Vector3D ( 0.0F, 0.0F, 8.0F ) );

	plane->Transformation->SetOrientation( Vector3D ( 2.8F, 0.0F, 0.0F ) );

	plane->Properties->Color = Vector3D ( 0.0F, 0.5F, 0.5F );

	plane->Properties->Shininess = 32.0F;

	plane->Tesselate ( );


	Box * box = new Box ( Vector3D ( 2.0F, 2.0F, 2.0F ), new Transform (), new Material () );
	
	box->Transformation->SetTranslation ( Vector3D ( -5.0F, 0.0F, 0.0F ) );

	box->Transformation->SetOrientation( Vector3D ( 2.3F, 1.2F, 0.5F ) );

	box->Properties->Color = Vector3D ( 0.5F, 0.0F, 0.5F );

	box->Properties->Shininess = 64.0F;

	box->Tesselate ( );


	Mesh * mesh = new Mesh ( model, new Transform ( ), new Material ( ) );
	
	mesh->Transformation->SetScale ( Vector3D ( 1.0F / 120.0F, 1.0F / 120.0F, 1.0F / 120.0F ) ); // Vector3D ( 1.0F / 500.0F, 1.0F / 500.0F, 1.0F / 500.0F )

	mesh->Tesselate ( );

	mesh->Properties->Color = Vector3D ( 0.8F, 0.8F, 0.0F );

	mesh->Properties->Shininess = 128.0F;


	Scene * scene = new Scene ( &camera, new Volume ( Vector3D ( -10, -10, -10 ), Vector3D ( 10, 10, 10 ) ) );

	//scene->Primitives.push_back ( sphere );
	//scene->Primitives.push_back ( plane );
	//scene->Primitives.push_back ( box );
	scene->Primitives.push_back ( mesh );

	scene->Lights.push_back ( new Light (0, Vector3D ( 10.0F, 10.0F, -10.0F ) ) );
	scene->Lights.push_back ( new Light (1, Vector3D ( -10.0F, -10.0F, -10.0F ) ) );

	scene->BuildGrid ( 64, 64, 64 );

	//---------------------------------------------------------------------------------------------

	ShaderManager * manager = new ShaderManager ( );

	manager->LoadVertexShader ( "Vertex.vs" );

	manager->LoadFragmentShader ( "Fragment.fs" );

	manager->BuildProgram ( );

	//---------------------------------------------------------------------------------------------

	Data * data = new Data ( );

	data->SetupTextures ( scene );

	manager->Bind ( );

	data->SetShaderData ( manager );

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

			int i = position.X, j = position.Y, k = position.Z;

			manager->SetUniformVector ( "Position", position );

			manager->SetUniformFloat ( "UCOUNT", scene->Grid->GetVoxel ( i, j, k )->Triangles.size ( ) );

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

			keyboard.Step = 0.001F;

			camera.Setup ( );

			glEnable ( GL_DEPTH_TEST );

			glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			//-------------------------------------------------------------------------------------

			glColor3f ( 1.0F, 0.0F, 0.0F );
			
			glBegin ( GL_LINES );
				glVertex3f ( 0, 0, 0 );
				glVertex3f ( 10, 0, 0 );
			glEnd ( );
			
			glColor3f ( 0.0F, 1.0F, 0.0F );
			
			glBegin ( GL_LINES );			
				glVertex3f ( 0, 0, 0 );
				glVertex3f ( 0, 10, 0 );			
			glEnd ( );
			
			glColor3f ( 0.0F, 0.0F, 1.0F );
			
			glBegin ( GL_LINES );			
				glVertex3f ( 0, 0, 0 );
				glVertex3f ( 0, 0, 10 );			
			glEnd ( );

			//-------------------------------------------------------------------------------------

			int i = position.X, j = position.Y, k = position.Z;

			glBegin ( GL_TRIANGLES );
			
			for ( int index = 0; index < scene->Grid->GetVoxel ( i, j, k )->Triangles.size ( ); index++ )
			{
				glColor3fv ( Abs ( scene->Grid->GetVoxel ( i, j, k )->Triangles [index]->VertexA->Position ) );

				scene->Grid->GetVoxel ( i, j, k )->Triangles [index]->Draw ( );
			}			
			
			glEnd ( );

			//-------------------------------------------------------------------------------------

			Vector3D vmin = scene->Grid->GetVoxel ( i, j, k )->Position - scene->Grid->GetVoxel ( i, j, k )->Radius;

			Vector3D vmax = scene->Grid->GetVoxel ( i, j, k )->Position + scene->Grid->GetVoxel ( i, j, k )->Radius;

			glColor3f ( 1.0F, 1.0F, 1.0F );
			
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
		}

        glfwSwapBuffers();

		//-----------------------------------------------------------------------------------------
		
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

    glfwTerminate();

    return 0;
}