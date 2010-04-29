#include <iostream>

#include <Graphics.hpp>

#include <logger.h>

using namespace std;

using namespace graphics;

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                            GLOBALS                                            //
///////////////////////////////////////////////////////////////////////////////////////////////////


Mouse mouse ( 6E1F );

Keyboard keyboard ( 2E1F );

Camera camera ( Vector3f ( 0.0F, 5.0F, 30.0F ), Vector3f ( 0.0F, -3.14, 0.0F ) );

Vector3f position = Vector3f ( 0.0F, 2.0F, 0.0F );

float angle = 0.0F;

ShaderProgram * sphereShader = NULL;

Texture2D * tex = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                           FUNCTIONS                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////

void mouseMoveHandler ( int x, int y )
{
	mouse.MouseMoveHandler ( x, y );
}

void mouseDownHandler ( int button, int state )
{
	mouse.MouseDownHandler ( button, state );
}

void keyDownHandler ( int key, int state )
{
	keyboard.KeyDownHandler ( key, state );
}

void Draw ( int width, int height, float time )
{
	{
		mouse.Apply ( &camera );

		keyboard.Apply ( &camera );

		camera.SetViewport ( width, height );

		camera.Setup ( );
	}
	
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnable ( GL_TEXTURE_2D );

	//tex->Setup ( );

	{
		glColor3f ( 0.8F, 0.8F, 0.8F );

		glBegin ( GL_QUADS );               

		glTexCoord2f ( 0, 0 ); glVertex3f ( -10.0F, 0.0F, -10.0F );
		glTexCoord2f ( 0, 1 ); glVertex3f (  10.0F, 0.0F, -10.0F );
		glTexCoord2f ( 1, 1 ); glVertex3f (  10.0F, 0.0F,  10.0F );
		glTexCoord2f ( 1, 0 ); glVertex3f ( -10.0F, 0.0F,  10.0F );            

		glEnd ( );
	}

	sphereShader->Bind ( );

	{
		GLUquadric * quad = gluNewQuadric ( );

		gluQuadricTexture ( quad, true );  

		glTranslatef ( position.x ( ),
			           position.y ( ),
					   position.z ( ) );

		glRotatef ( angle += 0.2F, 1.0F, 0.0F, 1.0F );

		gluSphere ( quad, 2.0F, 50, 50 );

		gluDeleteQuadric ( quad );
	}

	sphereShader->Unbind ( );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                          ENTRY POINT                                          //
///////////////////////////////////////////////////////////////////////////////////////////////////

int main ( int argc, char * argv [] )
{
	glfwInit ( );

	//---------------------------------------------------------------------------------------------

	cout << "Do you want to run program in full screen mode? [Y/N]" << endl;

	int choice = getchar ( );

	int width = 640, height = 480, mode = GLFW_WINDOW;

	//---------------------------------------------------------------------------------------------

	if ( choice == 'Y' || choice == 'y' )
	{
		GLFWvidmode vidmode;

		glfwGetDesktopMode ( &vidmode );

		width = vidmode.Width;

		height = vidmode.Height;

		mode = GLFW_FULLSCREEN;
	}

	//---------------------------------------------------------------------------------------------

	if( !glfwOpenWindow ( width, height, 0, 0, 0, 0, 16, 0, mode ) )
	{
		glfwTerminate ( ); exit ( 0 );
	}

	//---------------------------------------------------------------------------------------------

	glfwSwapInterval ( 0 );

	glfwSetMousePosCallback ( mouseMoveHandler );

	glfwSetMouseButtonCallback ( mouseDownHandler );

	glfwSetKeyCallback ( keyDownHandler );

	tex = new Texture2D ( 0, NULL, GL_TEXTURE_2D );

	tex->FilterMode = FilterMode :: Linear;

	tex->Data = TextureData2D :: LoadFromFile ( "C:/test.bmp" );

	for ( int i = 0; i < 512; i++ ) {
		tex->Data->Pixel ( i, i ) [0] = 1;
		tex->Data->Pixel ( i, i ) [1] = 0;
		tex->Data->Pixel ( i, i ) [2] = 1;
		tex->Data->Pixel ( i, i ) [3] = 1;
	}

	tex->Setup ( );

	//---------------------------------------------------------------------------------------------

	camera.SetViewFrustum ( );

	sphereShader = new ShaderProgram ( );

	sphereShader->LoadVertexShader ( "Render.vs" );

	sphereShader->LoadFragmentShader( "Render.fs" );

	sphereShader->Build ( );

	glEnable ( GL_DEPTH_TEST );

	//---------------------------------------------------------------------------------------------

	bool running = GL_TRUE;

	int frames = 0;

	char caption [100];

	double start = glfwGetTime ( );

	//---------------------------------------------------------------------------------------------

	while ( running )
	{
		double time = glfwGetTime ( );

		if ( ( time - start ) > 1.0 || frames == 0 )
		{
			double fps = frames / ( time - start );

			sprintf_s ( caption, "Simple GLSL Test - %.1f FPS", fps );

			glfwSetWindowTitle ( caption );

			start = time;

			frames = 0;
		}

		frames++;

		//-----------------------------------------------------------------------------------------

		glfwGetWindowSize ( &width, &height );

		height = max ( height,  1 );

		//-----------------------------------------------------------------------------------------

		position ( 0 ) = 5.0F * sinf ( 2.0F * time );
		position ( 2 ) = 5.0F * cosf ( 2.0F * time );

		Draw ( width, height, time );

		//-----------------------------------------------------------------------------------------

		glfwSwapBuffers ( );            

		running = !glfwGetKey ( GLFW_KEY_ESC ) && glfwGetWindowParam ( GLFW_OPENED );
	}

	

	glfwTerminate ( ); exit ( 0 );
}