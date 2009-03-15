#include <GLee.h>

#include <GL/glfw.h>

#include <vector>

#include "Camera.h"

#include "Texture2D.h"

#include "Mouse.h"

#include "Keyboard.h"

#include "Vector3D.h"

#include "Matrix3D.h"

#include "FrameBuffer.h"

#include "RenderBuffer.h"

//-------------------------------------------------------------------------------------------------

using namespace std;

using namespace Math;

using namespace RenderTools;

//-------------------------------------------------------------------------------------------------

Mouse mouse ( 0.005F );

Keyboard keyboard ( 0.1F );

//-------------------------------------------------------------------------------------------------

int count = 128;

//-------------------------------------------------------------------------------------------------

TextureData2D * positions = NULL;

TextureData2D * velocities = NULL;

TextureData2D * colors = NULL;

//-------------------------------------------------------------------------------------------------

Vector4D positionAverage ( 0.0F, 0.0F, 0.0F, 20.0F );

Vector4D positionDispersion ( 2.0F, 2.0F, 2.0F, 10.0F );

Vector3D velocityAverage ( 5.0F, 5.0F, 5.0F );

Vector3D velocityDispersion ( 0.0F, 0.0F, 0.0F );

Vector3D colorAverage ( 0.75F, 0.75F, 0.75F );

Vector3D colorDispersion ( 0.25F, 0.25F, 0.25F );

//-------------------------------------------------------------------------------------------------

Texture2D * currentPositionTexture = NULL;

Texture2D * currentVelocityTexture = NULL;      

Texture2D * nextPositionTexture = NULL;

Texture2D * nextVelocityTexture = NULL;

//-------------------------------------------------------------------------------------------------

ShaderManager * firstProgram = NULL;

ShaderManager * secondProgram = NULL;

ShaderManager * renderProgram = NULL;

//-------------------------------------------------------------------------------------------------

FrameBuffer * firstFrameBuffer = NULL;

FrameBuffer * secondFrameBuffer = NULL;

Camera camera;

//-------------------------------------------------------------------------------------------------

float RandDouble ( )
{
	return ( rand ( ) / ( ( float ) ( RAND_MAX ) + 1.0F ) ) * 2.0F - 1.0F;
}

void SetupParticles ( )
{
	{
		positions = new TextureData2D ( count, count, 4 );

		velocities = new TextureData2D ( count, count, 3 );     		
                
		colors = new TextureData2D ( count, count, 3 );
	}
	
	{
		int index = 0;
		
		for (int i = 0; i < count; i++)
		{
			for (int j = 0; j < count; j++)
			{
				positions->Pixel<Vector4D> ( i, j ) =
					Vector4D ( positionAverage.X + positionDispersion.X * RandDouble ( ),
					           positionAverage.Y + positionDispersion.Y * RandDouble ( ),
							   positionAverage.Z + positionDispersion.Z * RandDouble ( ),
							   positionAverage.W + positionDispersion.W * RandDouble ( ) );

				velocities->Pixel<Vector3D> ( i, j ) =
					Vector3D ( velocityAverage.X + velocityDispersion.X * RandDouble ( ),
					           velocityAverage.Y + velocityDispersion.Y * RandDouble ( ),
							   velocityAverage.Z + velocityDispersion.Z * RandDouble ( ) );

				colors->Pixel<Vector3D> ( i, j ) =
					Vector3D ( colorAverage.X + colorDispersion.X * RandDouble ( ),
					           colorAverage.Y + colorDispersion.Y * RandDouble ( ),
							   colorAverage.Z + colorDispersion.Z * RandDouble ( ) );
			}
		}
	}
}

void SetupKernels ( )
{
	{
		currentPositionTexture = new Texture2D ( GL_TEXTURE_RECTANGLE_ARB, positions, 1 );
				
		currentVelocityTexture = new Texture2D ( GL_TEXTURE_RECTANGLE_ARB, velocities, 2 );

		nextPositionTexture = new Texture2D ( GL_TEXTURE_RECTANGLE_ARB, positions, 3 );
				
		nextVelocityTexture = new Texture2D ( GL_TEXTURE_RECTANGLE_ARB, velocities, 4 );
					
		currentPositionTexture->Setup ( );
				
		currentVelocityTexture->Setup ( );
				
		nextPositionTexture->Setup ( );
		
		nextVelocityTexture->Setup ( );					
	}
	
	{
		bool result = true;
		
		firstProgram = new ShaderManager ( );
		
		if ( !firstProgram->LoadVertexShader ( "Compute.vs" ) )
			result = false;
		
		if ( !firstProgram->LoadFragmentShader ( "Compute.fs" ) )
			result = false;
		
		if ( !firstProgram->BuildProgram ( ) )
			result = false;
		
		if (!result)
			cout << "Compute programs error!" << endl;
		
		firstProgram->Bind ( );
		
		firstProgram->SetTexture ( "CurrentPositionTexture", *currentPositionTexture );
		
		firstProgram->SetTexture ( "CurrentVelocityTexture", *currentVelocityTexture );
		
		firstProgram->Unbind ( );
	}
	
	{
		bool result = true;
		
		secondProgram = new ShaderManager ( );

		if ( !secondProgram->LoadVertexShader ( "Compute.vs" ) )
			result = false;
		
		if ( !secondProgram->LoadFragmentShader ( "Compute.fs" ) )
			result = false;
		
		if ( !secondProgram->BuildProgram ( ) )
			result = false;
		
		if (!result)
			cout << "Compute programs error!" << endl;
		        
		secondProgram->Bind ( );
		
		secondProgram->SetTexture ( "CurrentPositionTexture", *nextPositionTexture );
		
		secondProgram->SetTexture ( "CurrentVelocityTexture", *nextVelocityTexture );
		
		secondProgram->Unbind ( );
	}
	
	{
		bool result = true;
		
		renderProgram = new ShaderManager ( );

		if ( !renderProgram->LoadVertexShader ( "Render.vs" ) )
			result = false;
		
		if ( !renderProgram->LoadFragmentShader ( "Render.fs" ) )
			result = false;
		
		if ( !renderProgram->BuildProgram ( ) )
			result = false;
		
		if (!result)
			cout << "Render programs error!" << endl;
		        
		renderProgram->Bind ( );
		
		renderProgram->SetTexture ( "CurrentPositionTexture", *currentPositionTexture );
		
		renderProgram->Unbind ( );
	}
	
	{
		firstFrameBuffer = new FrameBuffer ( );
		
		firstFrameBuffer->ColorBuffers.push_back ( nextPositionTexture );

		firstFrameBuffer->ColorBuffers.push_back ( nextVelocityTexture );

		firstFrameBuffer->Setup ( );
	}
	
	{
		secondFrameBuffer = new FrameBuffer ( );
		
		secondFrameBuffer->ColorBuffers.push_back ( currentPositionTexture );
		
		secondFrameBuffer->ColorBuffers.push_back ( currentVelocityTexture );

		secondFrameBuffer->Setup ( );
	}
}

void GenerateStream ( int width, int height )
{
	{
		glViewport ( 0, 0, width, height );    
				
		glMatrixMode ( GL_PROJECTION );
	            
		glLoadIdentity ( ); 
		
		glOrtho ( 0.0F, 1.0F, 0.0F, 1.0F, -1.0F, 1.0F );
		
		glMatrixMode ( GL_MODELVIEW );
		
		glLoadIdentity ( );				
	}

	{
		glClear ( GL_COLOR_BUFFER_BIT );
		
		glBegin ( GL_QUADS );
		{
			glTexCoord2f (  0.0F,   0.0F );  glVertex2f ( 0.0F, 0.0F );
			glTexCoord2f (  0.0F,  count );  glVertex2f ( 0.0F, 1.0F );
			glTexCoord2f ( count,  count );  glVertex2f ( 1.0F, 1.0F );
			glTexCoord2f ( count,   0.0F );  glVertex2f ( 1.0F, 0.0F );
		}
		glEnd ( ); 
	}
}

bool step = true;

void StartKernels ( )
{
	if ( step = !step )
	{
		firstFrameBuffer->Bind ( );

		firstProgram->Bind ( );
	}
	else
	{
		secondFrameBuffer->Bind ( );

		secondProgram->Bind ( );
	}
	
	GenerateStream ( count, count );
}

void DrawParticles ( )
{
	glClear ( GL_COLOR_BUFFER_BIT );
			
	renderProgram->Bind ( );
	{
		int index = 0;
		
		renderProgram->SetUniformVector ( "UpDirection", camera.Up );
					
		renderProgram->SetUniformVector ( "RightDirection", camera.Side);			                
		
		for ( int i = 0; i < count; i++ )
		{
			for ( int j = 0; j < count; j++ )
			{
				renderProgram->SetUniformVector ( "TextureCoords", Vector2D ( i, j ) );
				
				glColor3fv ( colors->Pixel <Vector3D> ( i, j ) );
				
				glBegin ( GL_QUADS );
				{
					glVertex2f ( -1.0F, -1.0F );								
					glVertex2f ( -1.0F,  1.0F );								
					glVertex2f (  1.0F,  1.0F );								
					glVertex2f (  1.0F, -1.0F );
				}
				glEnd ( );
			}
		}
	}
}

void Draw ( int width, int height )
{
	glViewport ( 0, 0, width, height );                                  
            
	glBindFramebufferEXT ( GL_FRAMEBUFFER_EXT, 0 );

	mouse.Apply ( camera );

	keyboard.Apply ( camera );
            	
	camera.Setup ( );

	DrawParticles ( );
}

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

//-------------------------------------------------------------------------------------------------

int main ( void )
{
	int width = 800;
	int height = 800;
	
    double t, t0, fps;
    char titlestr [ 200 ];

    glfwInit ( );

    if( !glfwOpenWindow ( width, height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW ) )
    {
        glfwTerminate ( );

        return 0;
	}

	//---------------------------------------------------------------------------------------------

	glEnable ( GL_TEXTURE_2D );
	
	glEnable ( GL_COLOR_MATERIAL );
	
	glEnable ( GL_BLEND );
	
	glBlendFunc ( GL_ONE, GL_ONE );	

	//---------------------------------------------------------------------------------------------

	camera = Camera ( Vector3D ( 0.0F, 0.0F, -18.0F ), Vector3D ( 0.0F, 0.0F, 0.0F ) );

	//---------------------------------------------------------------------------------------------

    glfwSwapInterval ( 0 );

	glfwSetMousePosCallback( MouseMove );

	glfwSetMouseButtonCallback( MouseButton );

	glfwSetKeyCallback ( KeyButton );

	//---------------------------------------------------------------------------------------------
	
	SetupParticles();
	
	SetupKernels();

	//---------------------------------------------------------------------------------------------
	
	bool running = GL_TRUE;

	int frames = 0;

	t0 = glfwGetTime ( );

	//---------------------------------------------------------------------------------------------

    while ( running )
	{
        t = glfwGetTime ( );

        if ( ( t-t0 ) > 1.0 || frames == 0 )
        {
            fps = (double) frames / ( t - t0 );

            sprintf_s ( titlestr, "Implicity Surfaces (%.1f FPS)", fps );

            glfwSetWindowTitle ( titlestr );

            t0 = t;

            frames = 0;
        }

        frames++;

		//-----------------------------------------------------------------------------------------

        glfwGetWindowSize ( &width, &height );

        height = height > 0 ? height : 1;

        glViewport ( 0, 0, width, height );
		
		//-----------------------------------------------------------------------------------------

		StartKernels ( );
		
		Draw ( width, height );

		//-----------------------------------------------------------------------------------------

        glfwSwapBuffers();		
		
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

    glfwTerminate();

    return 0;
}