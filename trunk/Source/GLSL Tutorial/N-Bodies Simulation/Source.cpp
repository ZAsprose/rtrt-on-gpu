/*
   ---------------------------------------------------------------------------
   |             N - B O D I E S   S I M U L A T I O N   D E M O             |
   ---------------------------------------------------------------------------
                              
   Copyright (c) 2009 - 2010 Denis Bogolepov ( denisbogol @ gmail.com )

   This program is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation, either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License along
   with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <Graphics.hpp>

using namespace graphics;

#define RAND( ) ( 2.0F * rand ( ) / ( ( float ) RAND_MAX + 1.0F ) - 1.0F )

/////////////////////////////////////////////////////////////////////////////////////////
// Global variables

/*
 * Camera control with mouse ( orientation ) and keyboard ( position ).
 */

Mouse mouse;

Keyboard keyboard;

Camera camera ( Vector3f ( 0.0F, 0.0F, -18.0F )   /* position */,
                Vector3f ( 0.0F, 0.0F, 0.0F )     /* orientation ( Euler angles ) */ );

/*
 * Number or particles ( N = CountX x CountY ).
 */

GLint countX = 128;

GLint countY = 128;

/*
 * Parameters for generating initial data.
 */

Vector4f positionAverage ( 0.0F, 0.0F, 0.0F, 20.0F );

Vector4f positionDispersion ( 2.0F, 2.0F, 2.0F, 10.0F );

Vector3f velocityAverage ( 5.0F, 5.0F, 5.0F );

Vector3f velocityDispersion ( 0.0F, 0.0F, 0.0F );

Vector3f colorAverage ( 0.75F, 0.75F, 0.75F );

Vector3f colorDispersion ( 0.25F, 0.25F, 0.25F );

/*
 * Texture data for storing positions, velocities and colors of particles.
 */

TextureData2D * positions = NULL;

TextureData2D * velocities = NULL;

TextureData2D * colors = NULL;

/*
 * Textures for representing input and output data.
 */

Texture2D * currentPositionTexture = NULL;

Texture2D * currentVelocityTexture = NULL;      

Texture2D * nextPositionTexture = NULL;

Texture2D * nextVelocityTexture = NULL;

/*
 * Shader programs for updating particle states and visualization.
 */

ShaderProgram * firstProgram = NULL;

ShaderProgram * secondProgram = NULL;

ShaderProgram * renderProgram = NULL;

/*
 * Frame buffers for rendering to textures.
 */

FrameBuffer * firstFrameBuffer = NULL;

FrameBuffer * secondFrameBuffer = NULL;

/////////////////////////////////////////////////////////////////////////////////////////
// N-Bodies subroutines

/*
 * Generates initial data of particles in a random way.
 */

#define INIT_RANDOM_

#define INIT_SHELL

void SetupParticles ( void )
{
	positions = new TextureData2D ( countX, countY, 4 );

	velocities = new TextureData2D ( countX, countY, 3 );	

	colors = new TextureData2D ( countX, countY, 3 );

#if defined ( INIT_RANDOM )

	for ( int x = 0; x < countX; x++ )
	{
		for ( int y = 0; y < countY; y++ )
		{
			positions->Pixel < Vector4f > ( x, y ) = Vector4f (
                positionAverage.x ( ) + positionDispersion.x ( ) * RAND ( ),
                positionAverage.y ( ) + positionDispersion.y ( ) * RAND ( ),
                positionAverage.z ( ) + positionDispersion.z ( ) * RAND ( ),
                positionAverage.w ( ) + positionDispersion.w ( ) * RAND ( ) );
            
            velocities->Pixel < Vector3f > ( x, y ) = Vector3f (
                velocityAverage.x ( ) + velocityDispersion.x ( ) * RAND ( ),
                velocityAverage.y ( ) + velocityDispersion.y ( ) * RAND ( ),
                velocityAverage.z ( ) + velocityDispersion.z ( ) * RAND ( ) );

            colors->Pixel < Vector3f > ( x, y ) = Vector3f (
                colorAverage.x ( ) + colorDispersion.x ( ) * RAND ( ),
                colorAverage.y ( ) + colorDispersion.y ( ) * RAND ( ),
                colorAverage.z ( ) + colorDispersion.z ( ) * RAND ( ) );
		}
	}

#elif defined ( INIT_SHELL )

	float size = 2.0F;
	
	float scale = 12.0F * size;
	
	float inner = 2.5F * size;
	
	float outer = 4.0F * size;

	for ( int x = 0; x < countX; x++ )
	{
		for ( int y = 0; y < countY; y++ )
		{
			Vector3f point = Vector3f ( RAND ( ), RAND ( ), RAND ( ) );
			
			float length = point.norm ( );

			while ( length > 1.0F )
			{
				point = Vector3f ( RAND ( ), RAND ( ), RAND ( ) );
				
				length = point.norm ( );
			}
			
			point /= length;

			positions->Pixel < Vector4f > ( x, y ) = Vector4f (
				point.x ( ) * ( inner + ( outer - inner ) * rand ( ) / ( float ) RAND_MAX ),
				point.y ( ) * ( inner + ( outer - inner ) * rand ( ) / ( float ) RAND_MAX ),
				point.z ( ) * ( inner + ( outer - inner ) * rand ( ) / ( float ) RAND_MAX ),
				positionAverage.w ( ) );

			Vector3f axis = Vector3f :: UnitZ ( );
				
			if ( 1.0F - axis.dot ( point ) < 1e-6F )
			{
				axis ( 0 ) = point.y ( );
				axis ( 1 ) = point.x ( );

                axis.normalize ( );
			}
			
			velocities->Pixel < Vector3f > ( x, y ) = scale *
                positions->Pixel < Vector3f > ( x, y ).cross ( axis );

			colors->Pixel < Vector3f > ( x, y ) =
				Vector3f ( colorAverage.x ( ) + colorDispersion.x ( ) * RAND ( ),
				           colorAverage.y ( ) + colorDispersion.y ( ) * RAND ( ),
						   colorAverage.z ( ) + colorDispersion.z ( ) * RAND ( ) );
		}
	}

#endif
    
    currentPositionTexture = new Texture2D (
        0                          /* texture unit */,
        positions                  /* texture data */,
        GL_TEXTURE_RECTANGLE_ARB   /* texture target */ );		

    currentVelocityTexture = new Texture2D (
        1                          /* texture unit */,
        velocities                 /* texture data */,
        GL_TEXTURE_RECTANGLE_ARB   /* texture target */ );
    
    nextPositionTexture = new Texture2D (
        2                          /* texture unit */,
        positions                  /* texture data */,
        GL_TEXTURE_RECTANGLE_ARB   /* texture target */ );

    nextVelocityTexture = new Texture2D (
        3                          /* texture unit */,
        velocities                 /* texture data */,
        GL_TEXTURE_RECTANGLE_ARB   /* texture target */ );
    
    currentPositionTexture->Setup ( );
    currentVelocityTexture->Setup ( );

    nextPositionTexture->Setup ( );
    nextVelocityTexture->Setup ( );
}

/*
 * Loads shaders for computing and efficient rendering of n-bodies system.
 */

void SetupKernels ( void )
{
    /*
     * Load and compile compute shaders for even iterations ( ping-pong ).
     */

	{
		firstProgram = new ShaderProgram ( );
		
		if ( !firstProgram->LoadVertexShader ( "Compute.vs" ) )
		{
            std :: cout << "ERROR! Failed to load compute vertex shader!" << std :: endl;
            exit ( EXIT_FAILURE );
		}
		
		if ( !firstProgram->LoadFragmentShader ( "Compute.fs" ) )
		{
            std :: cout << "ERROR! Failed to load compute fragment shader!" << std :: endl;
            exit ( EXIT_FAILURE );
		}
		
		if ( !firstProgram->Build ( ) )
		{
            std :: cout << "ERROR! Failed to build compute shader program!" << std :: endl;
            exit ( EXIT_FAILURE );
		}	
		
		firstProgram->Bind ( );		

		    firstProgram->SetTexture ( "CurrentPositionTexture", currentPositionTexture );	

		    firstProgram->SetTexture ( "CurrentVelocityTexture", currentVelocityTexture );	

		firstProgram->Unbind ( );
    }

    /*
     * Load and compile compute shaders for odd iterations ( ping-pong ).
     */

	{
		secondProgram = new ShaderProgram ( );

		if ( !secondProgram->LoadVertexShader ( "Compute.vs" ) )
		{
            std :: cout << "ERROR! Failed to load compute vertex shader!" << std :: endl;
            exit ( EXIT_FAILURE );
		}
		
		if ( !secondProgram->LoadFragmentShader ( "Compute.fs" ) )
		{
            std :: cout << "ERROR! Failed to load compute fragment shader!" << std :: endl;
            exit ( EXIT_FAILURE );
		}
		
		if ( !secondProgram->Build ( ) )
		{
            std :: cout << "ERROR! Failed to build compute shader program!" << std :: endl;
            exit ( EXIT_FAILURE );
		}
		        
		secondProgram->Bind ( );

		    secondProgram->SetTexture ( "CurrentPositionTexture", nextPositionTexture );

		    secondProgram->SetTexture ( "CurrentVelocityTexture", nextVelocityTexture );

		secondProgram->Unbind ( );
    }

    /*
     * Load and compile shaders for rendering of particles.
     */

	{
		renderProgram = new ShaderProgram ( );

		if ( !renderProgram->LoadVertexShader ( "Render.vs" ) )
		{
            std :: cout << "ERROR! Failed to load render vertex shader!" << std :: endl;
            exit ( EXIT_FAILURE );
		}
		
		if ( !renderProgram->LoadFragmentShader ( "Render.fs" ) )
		{
            std :: cout << "ERROR! Failed to load render fragment shader!" << std :: endl;
            exit ( EXIT_FAILURE );
		}
		
		if ( !renderProgram->Build ( ) )
		{
            std :: cout << "ERROR! Failed to build render shader program!" << std :: endl;
            exit ( EXIT_FAILURE );
		}
		        
		renderProgram->Bind ( );

		    renderProgram->SetTexture ( "CurrentPositionTexture", currentPositionTexture );

		renderProgram->Unbind ( );
    }

    /*
     * Create framebuffer for rendering to textures with 'next' particle states.
     */
	
	{
		firstFrameBuffer = new FrameBuffer ( );

		firstFrameBuffer->ColorBuffers.push_back ( nextPositionTexture );

		firstFrameBuffer->ColorBuffers.push_back ( nextVelocityTexture );

		firstFrameBuffer->Setup ( );
    }

    /*
     * Create framebuffer for rendering to textures with 'current' particle states.
     */
	
	{
		secondFrameBuffer = new FrameBuffer ( );

		secondFrameBuffer->ColorBuffers.push_back ( currentPositionTexture );

		secondFrameBuffer->ColorBuffers.push_back ( currentVelocityTexture );

		secondFrameBuffer->Setup ( );
	}
}

/*
 * Generates stream of fragments for processing on GPU ( 1 fragment = 1 particle ).
 */

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
			glTexCoord2f (   0.0F,   0.0F );  glVertex2f ( 0.0F, 0.0F );
			glTexCoord2f (   0.0F, countY );  glVertex2f ( 0.0F, 1.0F );
			glTexCoord2f ( countX, countY );  glVertex2f ( 1.0F, 1.0F );
			glTexCoord2f ( countX,   0.0F );  glVertex2f ( 1.0F, 0.0F );
		}
		glEnd ( ); 
	}
}

/*
 * Starts compute shaders ( kernels ) on GPU with ping-pong technique.
 */

bool even = true;

void StartKernels ( )
{
	if ( even = !even )
	{
		firstFrameBuffer->Bind ( );

		firstProgram->Bind ( );
		
		GenerateStream ( countX, countY );

		firstFrameBuffer->Unbind ( );
	}
	else
	{
		secondFrameBuffer->Bind ( );

		secondProgram->Bind ( );

		GenerateStream ( countX, countY );

		secondFrameBuffer->Unbind ( );
	}
}

/*
 * Builds display list for efficient particle rendering.
 */

void BuildList ( void )
{
	glNewList ( 1, GL_COMPILE );

	glBegin ( GL_QUADS );

	for ( int x = 0; x < countX; x++ )
	{
		for ( int y = 0; y < countY; y++ )
        {
            glColor3fv ( colors->Pixel < Vector3f > ( x, y ).data ( ) );
			
			glTexCoord2f ( x, y );

			glVertex2f ( -1.0F, -1.0F );								
			glVertex2f ( -1.0F,  1.0F );								
			glVertex2f (  1.0F,  1.0F );								
			glVertex2f (  1.0F, -1.0F );
		}
	}

	glEnd ( );

	glEndList ( );
}

/*
 * Draws particles with custom shaders.
 */

void Draw ( int width, int height )
{
	mouse.Apply ( &camera );

	keyboard.Apply ( &camera );

	camera.SetViewport ( width, height );
            	
	camera.Setup ( );

	renderProgram->Bind ( );

	renderProgram->SetUniformVector ( "UpDirection", camera.Up ( ) );

	renderProgram->SetUniformVector ( "RightDirection", camera.Side ( ) );

	glClear ( GL_COLOR_BUFFER_BIT );

	glCallList ( 1 );
}

/*
 * Event handlers for mouse and keyboard.
 */

void MouseMoveHandler ( int x, int y )
{
	mouse.MouseMoveHandler ( x, y );
}

void MouseDownHandler ( int button, int state )
{
	mouse.MouseDownHandler ( button, state );
}

void KeyDownHandler ( int key, int state )
{
	keyboard.KeyDownHandler ( key, state );
}

/////////////////////////////////////////////////////////////////////////////////////////
// Entry point for program

int main ( void )
{
	glfwInit ( );

	//-------------------------------------------------------------------------

    std :: cout << "Do you want to run program in fullscreen mode? [Y/N]\n";

	int choice = getchar ( );

	int width = 800, height = 600, mode = GLFW_WINDOW;

	//-------------------------------------------------------------------------

	if ( choice == 'Y' || choice == 'y' )
	{
		GLFWvidmode vidmode;

		glfwGetDesktopMode ( &vidmode );

		width = vidmode.Width;
		
		height = vidmode.Height;

		mode = GLFW_FULLSCREEN;
	}

	//-------------------------------------------------------------------------

    /* Try to open rendering window */

    if ( !glfwOpenWindow (
            width    /* window width */,
            height   /* window height */,
            0        /* bits for red channel ( default ) */,
            0        /* bits for green channel ( default ) */,
            0        /* bits for blue channel ( default ) */,
            0        /* bits for alpha channel ( default ) */,
            0        /* bits for depth buffer ( not used ) */,
            0        /* bits for stencil buffer ( not used ) */,
            mode     /* windows mode ( fullscreen or window ) */ ) )
    {
        glfwTerminate ( ); exit ( EXIT_FAILURE );
    }

	//-------------------------------------------------------------------------

    glfwSwapInterval ( 0 );

	glfwSetMousePosCallback ( MouseMoveHandler );

	glfwSetMouseButtonCallback ( MouseDownHandler );

	glfwSetKeyCallback ( KeyDownHandler );

	//-------------------------------------------------------------------------
	
	glEnable ( GL_COLOR_MATERIAL );
	
	glEnable ( GL_BLEND );
	
	glBlendFunc ( GL_ONE, GL_ONE );	

	camera.SetFrustum ( );

	//-------------------------------------------------------------------------
	
	SetupParticles ( );
	
	SetupKernels ( );
	
	BuildList ( );

	//-------------------------------------------------------------------------
	
	GLboolean running = GL_TRUE;

	GLint frames = 0;

	GLchar caption [100];

	GLdouble start = glfwGetTime ( );

	//-------------------------------------------------------------------------

    while ( running )
	{
        double time = glfwGetTime ( );

        if ( ( time - start ) > 1.0 || frames == 0 )
        {
            double fps = frames / ( time - start );

            sprintf ( caption, "N-Bodies Simulation - %.1f FPS", fps );

            glfwSetWindowTitle ( caption );

            start = time;

            frames = 0;
        }

        frames++;

		//---------------------------------------------------------------------

        glfwGetWindowSize ( &width, &height );
        
        height = height < 1 ? 1 : height;
		
		//---------------------------------------------------------------------

		StartKernels ( );
		
		Draw ( width, height );

		//---------------------------------------------------------------------

        glfwSwapBuffers ( );		
		
		running = !glfwGetKey ( GLFW_KEY_ESC ) &&
            glfwGetWindowParam ( GLFW_OPENED );
	}

    glfwTerminate ( );

    return 0;
}