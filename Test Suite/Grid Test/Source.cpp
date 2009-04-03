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

#include <OBJLoader.h>

#include <Data.h>

using namespace Math;

using namespace Render;

using namespace Raytracing;

//=================================================================================================

Camera camera;

Mouse mouse ( 0.01F );

Keyboard keyboard ( 0.1F );

Vector3D position ( 8.0F, 8.0F, 8.0F );

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
    char    titlestr[ 200 ];

	//---------------------------------------------------------------------------------------------

    glfwInit();

    if( !glfwOpenWindow( 512, 512, 0, 0, 0, 0, 16, 0, GLFW_WINDOW ) )
    {
        glfwTerminate();

        return 0;
	}

	glEnable ( GL_TEXTURE_1D );

	//---------------------------------------------------------------------------------------------

	OBJModel * model = OBJLoader :: LoadModel ( "C:/Web/BUNNY_4K.obj" );

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

	//---------------------------------------------------------------------------------------------

	ShaderManager * manager = new ShaderManager ( );

	manager->LoadVertexShader ( "Vertex.vs" );

	manager->LoadFragmentShader ( "Fragment.fs" );

	manager->BuildProgram ( );

	//---------------------------------------------------------------------------------------------

	Volume * box = new Volume ( );

	box->Minimum = Vector3D ( -5.0F, -5.0F, -5.0F );

	box->Maximum = Vector3D ( 5.0F, 5.0F, 5.0F );

	UniformGrid * grid = new UniformGrid ( box );

	grid->BuildGrid ( trl );

	Data * data = new Data ( );

	//data->BuildTextures ( grid, NULL );

	//---------------------------------------------------------------------------------------------

	camera = Camera ( Vector3D ( 0.0F, 0.0F, -18.0F ), Vector3D ( 0.0F, 0.0F, 0.0F ) );

	camera.SetFrustum ( );

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

			manager->SetTexture ( "VoxelTexture", data->VoxelTexture );

			manager->SetTexture ( "VertexTexture", data->PositionTexture );

			manager->SetUniformVector ( "Position", position );

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

			keyboard.Step = 0.001F;

			camera.Setup ( );

			glEnable ( GL_DEPTH_TEST );

			glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			//-------------------------------------------------------------------------------------

			glColor3f ( 1.0F, 0.0F, 0.0F );
			
			glBegin ( GL_LINES );
				glVertex3f ( 0, 0, 0 );
				glVertex3f ( 5, 0, 0 );
			glEnd ( );
			
			glColor3f ( 0.0F, 1.0F, 0.0F );
			
			glBegin ( GL_LINES );			
				glVertex3f ( 0, 0, 0 );
				glVertex3f ( 0, 5, 0 );			
			glEnd ( );
			
			glColor3f ( 0.0F, 0.0F, 1.0F );
			
			glBegin ( GL_LINES );			
				glVertex3f ( 0, 0, 0 );
				glVertex3f ( 0, 0, 5 );			
			glEnd ( );

			//-------------------------------------------------------------------------------------

			int i = position.X, j = position.Y, k = position.Z;

			glBegin ( GL_TRIANGLES );
			
			for ( int index = 0; index < grid->GetVoxel ( i, j, k )->Triangles.size ( ); index++ )
			{
				glColor3fv ( Abs ( grid->GetVoxel ( i, j, k )->Triangles [index]->VertexA->Position ) );

				grid->GetVoxel ( i, j, k )->Triangles [index]->Draw ( );
			}			
			
			glEnd ( );

			//-------------------------------------------------------------------------------------

			Vector3D vmin = grid->GetVoxel ( i, j, k )->Position - grid->GetVoxel ( i, j, k )->Radius;

			Vector3D vmax = grid->GetVoxel ( i, j, k )->Position + grid->GetVoxel ( i, j, k )->Radius;

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