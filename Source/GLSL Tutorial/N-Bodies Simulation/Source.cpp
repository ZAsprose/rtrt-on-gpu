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

/////////////////////////////////////////////////////////////////////////////////////////
// Camera control with mouse ( orientation ) and keyboard ( position )

Mouse mouse;

Keyboard keyboard;

Camera * camera;

/////////////////////////////////////////////////////////////////////////////////////////
// Number or particles ( N = COUNT_X * COUNT_Y )

int countX = 64;

int countY = 64;

/////////////////////////////////////////////////////////////////////////////////////////
// Parameters for generating initial data

Vector4f positionAverage ( 0.0F, 0.0F, 0.0F, 20.0F );

Vector4f positionDispersion ( 2.0F, 2.0F, 2.0F, 10.0F );

Vector3f velocityAverage ( 5.0F, 5.0F, 5.0F );

Vector3f velocityDispersion ( 0.0F, 0.0F, 0.0F );

Vector3f colorAverage ( 0.75F, 0.75F, 0.75F );

Vector3f colorDispersion ( 0.25F, 0.25F, 0.25F );

/////////////////////////////////////////////////////////////////////////////////////////
// Texture data for storing positions, velocities and colors of particles

TextureData2D * positions = NULL;

TextureData2D * velocities = NULL;

TextureData2D * colors = NULL;

/////////////////////////////////////////////////////////////////////////////////////////
// Textures for representing input and output data

Texture2D * currentPositionTexture = NULL;

Texture2D * currentVelocityTexture = NULL;      

Texture2D * nextPositionTexture = NULL;

Texture2D * nextVelocityTexture = NULL;

/////////////////////////////////////////////////////////////////////////////////////////
// Shader programs for updating particle states and visualization

ShaderProgram * firstProgram = NULL;

ShaderProgram * secondProgram = NULL;

ShaderProgram * renderProgram = NULL;

/////////////////////////////////////////////////////////////////////////////////////////
// Frame buffers for rendering to textures

FrameBuffer * firstFrameBuffer = NULL;

FrameBuffer * secondFrameBuffer = NULL;

/////////////////////////////////////////////////////////////////////////////////////////
// Генерирует случайное число в диапазоне [-1, +1]

float RandDouble ( void )
{
	return ( rand ( ) / ( ( float ) ( RAND_MAX ) + 1.0F ) ) * 2.0F - 1.0F;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Генерирует случайным образом начальные состояния частиц

#define INIT_RANDOM

#define INIT_SHELL_

void SetupParticles ( void )
{
	positions = new TextureData2D ( countX, countY, 4 );

	velocities = new TextureData2D ( countX, countY, 3 );	

	colors = new TextureData2D ( countX, countY, 3 );

#if defined ( INIT_RANDOM )

	for ( int i = 0; i < countX; i++ )
	{
		for ( int j = 0; j < countY; j++ )
		{
			float * pos = positions->Pixel ( i, j );

            pos [0] = positionAverage.x ( ) + positionDispersion.x ( ) * RandDouble ( );
			pos [1] = positionAverage.y ( ) + positionDispersion.y ( ) * RandDouble ( );
			pos [2] = positionAverage.z ( ) + positionDispersion.z ( ) * RandDouble ( );
			pos [3] = positionAverage.w ( ) + positionDispersion.w ( ) * RandDouble ( );

			float * vel = velocities->Pixel ( i, j );

			vel [0] = velocityAverage.x ( ) + velocityDispersion.x ( ) * RandDouble ( );
            vel [1] = velocityAverage.y ( ) + velocityDispersion.y ( ) * RandDouble ( );
            vel [2] = velocityAverage.z ( ) + velocityDispersion.z ( ) * RandDouble ( );

			float * col = colors->Pixel ( i, j );
			col [0] = colorAverage.x ( ) + colorDispersion.x ( ) * RandDouble ( );
            col [1] = colorAverage.y ( ) + colorDispersion.y ( ) * RandDouble ( );
			col [2] = colorAverage.z ( ) + colorDispersion.z ( ) * RandDouble ( );
		}
	}

#elif defined ( INIT_SHELL )

	float size = 2.0F;
	
	float scale = 12.0F * size;
	
	float inner = 2.5F * size;
	
	float outer = 4.0F * size;

	for ( int i = 0; i < countX; i++ )
	{
		for ( int j = 0; j < countY; j++ )
		{
			Vector3f point = Vector3f ( RandDouble ( ), RandDouble ( ), RandDouble ( ) );
			
			float length = point.length();

			while ( length > 1.0F )
			{
				point = Vector3D ( RandDouble ( ), RandDouble ( ), RandDouble ( ) );
				
				length = Length ( point );
			}
			
			point /= length;

			positions->Pixel<Vector4D> ( i, j ) = Vector4D (
				point.x ( ) * ( inner + ( outer - inner ) * rand ( ) / ( float ) RAND_MAX ),
				point.y ( ) * ( inner + ( outer - inner ) * rand ( ) / ( float ) RAND_MAX ),
				point.z ( ) * ( inner + ( outer - inner ) * rand ( ) / ( float ) RAND_MAX ),
				positionAverage.w ( ) );

			Vector3f axis = Vector3f :: UnitZ ( );
				
			if ( 1.0 - Dot ( point, axis ) < 1e-6 )
			{
				axis ( 0 ) = point.y ( );
				axis ( 1 ) = point.x ( );
				Normalize ( axis );
			}
			
			velocities->Pixel<Vector3D> ( i, j ) = scale *
				Cross ( positions->Pixel<Vector3D> ( i, j ), axis );

			colors->Pixel<Vector3D> ( i, j ) =
				Vector3D ( colorAverage.X + colorDispersion.X * RandDouble ( ),
				           colorAverage.Y + colorDispersion.Y * RandDouble ( ),
						   colorAverage.Z + colorDispersion.Z * RandDouble ( ) );
		}
	}

#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Загружает и настраивает шейдеры (вычислительные ядра)

void SetupKernels ( void )
{
	{
		currentPositionTexture = new Texture2D ( 0, positions, GL_TEXTURE_RECTANGLE_ARB );				
		currentVelocityTexture = new Texture2D ( 1, velocities, GL_TEXTURE_RECTANGLE_ARB );
		nextPositionTexture = new Texture2D ( 2, positions, GL_TEXTURE_RECTANGLE_ARB );				
		nextVelocityTexture = new Texture2D ( 3, velocities, GL_TEXTURE_RECTANGLE_ARB );

		currentPositionTexture->Setup ( );				
		currentVelocityTexture->Setup ( );				
		nextPositionTexture->Setup ( );		
		nextVelocityTexture->Setup ( );					
	}
	
	{
		firstProgram = new ShaderProgram ( );
		
		if ( !firstProgram->LoadVertexShader ( "Compute.vs" ) )
		{
            std :: cout << "ERROR! Failed to load compute vertex shader!\n";
            exit ( -1 );
		}
		
		if ( !firstProgram->LoadFragmentShader ( "Compute.fs" ) )
		{
            std :: cout << "ERROR! Failed to load compute fragment shader!\n";
            exit ( -1 );
		}
		
		if ( !firstProgram->Build ( ) )
		{
            std :: cout << "ERROR! Failed to build compute shader program!\n";
            exit ( -1 );
		}	
		
		firstProgram->Bind ( );		
		    firstProgram->SetTexture ( "CurrentPositionTexture", currentPositionTexture );		
		    firstProgram->SetTexture ( "CurrentVelocityTexture", currentVelocityTexture );		
		firstProgram->Unbind ( );
	}
	
	{
		secondProgram = new ShaderProgram ( );

		if ( !secondProgram->LoadVertexShader ( "Compute.vs" ) )
		{
            std :: cout << "ERROR! Failed to load compute vertex shader!\n";
            exit ( -1 );
		}
		
		if ( !secondProgram->LoadFragmentShader ( "Compute.fs" ) )
		{
            std :: cout << "ERROR! Failed to load compute fragment shader!\n";
            exit ( -1 );
		}
		
		if ( !secondProgram->Build ( ) )
		{
            std :: cout << "ERROR! Failed to build compute shader program!\n";
            exit ( -1 );
		}
		        
		secondProgram->Bind ( );		
		    secondProgram->SetTexture ( "CurrentPositionTexture", nextPositionTexture );		
		    secondProgram->SetTexture ( "CurrentVelocityTexture", nextVelocityTexture );		
		secondProgram->Unbind ( );
	}
	
	{
		renderProgram = new ShaderProgram ( );

		if ( !renderProgram->LoadVertexShader ( "Render.vs" ) )
		{
            std :: cout << "ERROR! Failed to load render vertex shader!\n";
            exit ( -1 );
		}
		
		if ( !renderProgram->LoadFragmentShader ( "Render.fs" ) )
		{
            std :: cout << "ERROR! Failed to load render fragment shader!\n";
            exit ( -1 );
		}
		
		if ( !renderProgram->Build ( ) )
		{
            std :: cout << "ERROR! Failed to build render shader program!\n";
            exit ( -1 );
		}
		        
		renderProgram->Bind ( );		
		    renderProgram->SetTexture ( "CurrentPositionTexture", currentPositionTexture );		
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

///////////////////////////////////////////////////////////////////////////////////////////////////
// Генерирует двумерный поток фрагментов для обработки (фрагмент = частица)

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

///////////////////////////////////////////////////////////////////////////////////////////////////
// Запускает вычислительные ядра для обработки каждого элемента потока

bool step = true;

void StartKernels ( )
{
	if ( step = !step )
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

///////////////////////////////////////////////////////////////////////////////////////////////////
// Строит дисплейный список для эффективного рисования частиц

void BuildList ( void )
{
	glNewList ( 1, GL_COMPILE );

	glBegin ( GL_QUADS );

	for ( int i = 0; i < countX; i++ )
	{
		for ( int j = 0; j < countY; j++ )
		{			
			glColor3fv ( colors->Pixel ( i, j ) );
			
			glTexCoord2f ( i, j );

			glVertex2f ( -1.0F, -1.0F );								
			glVertex2f ( -1.0F,  1.0F );								
			glVertex2f (  1.0F,  1.0F );								
			glVertex2f (  1.0F, -1.0F );
		}
	}

	glEnd ( );

	glEndList ( );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Отрисовывает частицы

void Draw ( int width, int height )
{
	mouse.Apply ( camera );

	keyboard.Apply ( camera );

	camera->SetViewport ( width, height );
            	
	camera->Setup ( );

	renderProgram->Bind ( );

	renderProgram->SetUniformVector ( "UpDirection", camera->Up ( ) );

	renderProgram->SetUniformVector ( "RightDirection", camera->Side ( ) );

	glClear ( GL_COLOR_BUFFER_BIT );

	glCallList ( 1 );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Обработчики событий движения мыши и нажатия клавиш клавиатуры

void MouseMove ( int x, int y )
{
	mouse.MouseMoveHandler ( x, y );
}

void MouseButton ( int button, int state )
{
	mouse.MouseDownHandler ( button, state );
}

void KeyButton ( int key, int state )
{
	keyboard.KeyDownHandler ( key, state );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Точка входа в программу

int main ( void )
{
	glfwInit ( );

	//---------------------------------------------------------------------------------------------

    std :: cout << "Do you want to run program in fullscreen mode? [Y/N]\n";

	int choice = getchar ( );

	int width = 750, height = 750, mode = GLFW_WINDOW;

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

    if( !glfwOpenWindow ( width, height, 0, 0, 0, 0, 0, 0, mode ) )
    {
        glfwTerminate ( ); exit ( 0 );
	}

	//---------------------------------------------------------------------------------------------

    glfwSwapInterval ( 0 );

	glfwSetMousePosCallback ( MouseMove );

	glfwSetMouseButtonCallback ( MouseButton );

	glfwSetKeyCallback ( KeyButton );

	//---------------------------------------------------------------------------------------------
	
	glEnable ( GL_COLOR_MATERIAL );
	
	glEnable ( GL_BLEND );
	
	glBlendFunc ( GL_ONE, GL_ONE );	

	//---------------------------------------------------------------------------------------------

	camera = new Camera ( Vector3f ( 0.0F, 0.0F, -18.0F ), Vector3f ( 0.0F, 0.0F, 0.0F ) );

	camera->SetViewFrustum ( );

	//---------------------------------------------------------------------------------------------
	
	SetupParticles ( );
	
	SetupKernels ( );
	
	BuildList ( );

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

            sprintf_s ( caption, "N-Bodies Simulation - %.1f FPS", fps );

            glfwSetWindowTitle ( caption );

            start = time;

            frames = 0;
        }

        frames++;

		//-----------------------------------------------------------------------------------------

        glfwGetWindowSize ( &width, &height );

        //height = max ( height,  1 );
		
		//-----------------------------------------------------------------------------------------

		StartKernels ( );
		
		Draw ( width, height );

		//-----------------------------------------------------------------------------------------

        glfwSwapBuffers ( );		
		
		running = !glfwGetKey ( GLFW_KEY_ESC ) && glfwGetWindowParam ( GLFW_OPENED );
	}

    glfwTerminate ( );

    return 0;
}