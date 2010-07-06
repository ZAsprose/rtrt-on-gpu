/*
   ---------------------------------------------------------------------------
   |           M A T R I X   M U L T I P L I C A T I O N   D E M O           |
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

#include <iostream>

#include <Graphics.hpp>

using namespace graphics;

#define CHECK_RESULT_

/////////////////////////////////////////////////////////////////////////////////////////
// Global variables

/* Matrix size and FLOPS */

const GLint N = 512;

const GLdouble FLOPS = N * N * N;

/* OpenGL texture objects */

Texture2D * textureA  = NULL;

Texture2D * textureB  = NULL;

Texture2D * textureC  = NULL;

/* OpenGL framebuffer object for rendering to texture */

FrameBuffer * frameBuffer = NULL;

/* Shader program for computing matrix C = A * B */

ShaderProgram * shaderProgram = NULL;

/////////////////////////////////////////////////////////////////////////////////////////
// Entry point for program

int main ( void )
{
    /* We use GLFW for window management and OpenGL output */

    glfwInit ( );
    
    /* Try to open rendering window */

    if ( !glfwOpenWindow (
            512           /* window width */,
            512           /* window height */,
            0             /* bits for red channel ( default ) */,
            0             /* bits for green channel ( default ) */,
            0             /* bits for blue channel ( default ) */,
            0             /* bits for alpha channel ( default ) */,
            0             /* bits for depth buffer ( not used ) */,
            0             /* bits for stencil buffer ( not used ) */,
            GLFW_WINDOW   /* windows mode ( fullscreen or window ) */ ) )
    {
        glfwTerminate ( ); exit ( 0 );
    }

    /* Set event handlers for mouse and keyboard */

    glfwSwapInterval ( 0 );
    
    //-----------------------------------------------------------------------------------

    /* Set parallel projection for drawing dummy quad */
    
    glMatrixMode ( GL_PROJECTION );
    
    glLoadIdentity ( );
    
    glOrtho ( -1.0F, 1.0F, -1.0F, 1.0F, -1.0F, 1.0F );
    
    glMatrixMode ( GL_MODELVIEW );
    
    glLoadIdentity ( );

	/* Set view port ( global work size ) */

    glViewport ( 0, 0, N, N );

    //-----------------------------------------------------------------------------------

    /* Generate matrices */

    std :: cout << "Generate matrices...\n\n";

    MatrixXf matrixA = MatrixXf :: Random ( N, N );

    MatrixXf matrixB = MatrixXf :: Random ( N, N );

    MatrixXf matrixC = matrixA * matrixB;

	std :: cout << "Done!" << std :: endl;

    /* Create texture data */

    TextureData2D * dataA = new TextureData2D ( N  /* texture width */,
                                                N  /* texture height */,
                                                1  /* channels */ );

    TextureData2D * dataB = new TextureData2D ( N  /* texture width */,
                                                N  /* texture height */,
                                                1  /* channels */ );

    TextureData2D * dataC = new TextureData2D ( N  /* texture width */,
                                                N  /* texture height */,
                                                1  /* channels */ );

    for ( int x = 0; x < N; x++ )
    {
        for ( int y = 0; y < N; y++ )
        {
            dataA->Pixel < GLfloat > ( x, y ) = matrixA ( y, x );
            dataB->Pixel < GLfloat > ( x, y ) = matrixB ( x, y );
        }
    }

    /* Create textures */

    textureA = new Texture2D ( 0, dataA, GL_TEXTURE_RECTANGLE_ARB );
    textureB = new Texture2D ( 1, dataB, GL_TEXTURE_RECTANGLE_ARB );
    textureC = new Texture2D ( 2, dataC, GL_TEXTURE_RECTANGLE_ARB );

    textureA->Setup ( );
    textureB->Setup ( );
    textureC->Setup ( );

    /* Create framebuffer for rendering to texture */
	
	frameBuffer = new FrameBuffer ( );
	
	frameBuffer->ColorBuffers.push_back ( textureC );
    
    frameBuffer->Setup ( );

    //-----------------------------------------------------------------------------------

    /* Load and compile shader files */

    shaderProgram = new ShaderProgram ( );
    
    if ( !shaderProgram->LoadVertexShader ( "Vertex.glsl" ) )
    {
        std :: cout << "ERROR: failed to load vertex shader\n"; exit ( -1 );
    }

	char prefix [100];

	sprintf ( prefix, "#define N %d\n", N );
    
    if ( !shaderProgram->LoadFragmentShader ( "Fragment.glsl", prefix ) )
    {
        std :: cout << "ERROR: failed to load fragment shader\n"; exit ( -1 );
    }

    if ( !shaderProgram->Build ( ) )
    {
        std :: cout << "ERROR: failed to build shader program\n"; exit ( -1 );
    }

    shaderProgram->Bind ( );

    shaderProgram->SetTexture ( "MatrixA", textureA );
    shaderProgram->SetTexture ( "MatrixB", textureB );

    //-----------------------------------------------------------------------------------
    
    GLint frames = 0;

    GLchar caption [100];    

    GLdouble time, start = glfwGetTime ( );

    //-----------------------------------------------------------------------------------

    while ( GL_TRUE )
    {
        /* Calculate frames per second ( FPS ) and performance ( GFLOPS ) */

        time = glfwGetTime ( );

        GLdouble delta = time - start;

        if ( delta > 0.5 )
        {
            GLdouble fps = frames / delta, gflops = fps * FLOPS / 1E9;

            sprintf ( caption,
				      "Matrix Multiplication - %.1f FPS | %.1f GFLOPS",
                      fps, gflops );

            glfwSetWindowTitle ( caption );

            start = time;

            frames = 0;
        }

        frames++;

        //-------------------------------------------------------------------------------

        /* Draw dummy quad with custom fragment shader */

        frameBuffer->Bind ( );

        glBegin ( GL_QUADS );

            glTexCoord2f ( 0, 0 );   glVertex2f ( -1.0F, -1.0F );
            glTexCoord2f ( 0, N );   glVertex2f ( -1.0F,  1.0F );
            glTexCoord2f ( N, N );   glVertex2f (  1.0F,  1.0F );
            glTexCoord2f ( N, 0 );   glVertex2f (  1.0F, -1.0F );

        glEnd ( );

#ifdef CHECK_RESULT

        frameBuffer->FetchOutcome ( );

        MatrixXf matrixD ( N, N );

        for ( int x = 0; x < N; x++ )
        {
            for ( int y = 0; y < N; y++ )
            {
                matrixD ( y, x ) = dataC->Pixel < GLfloat > ( x, y );
            }
        }

        MatrixXf matrixE = matrixC - matrixD;

        for ( int x = 0; x < N; x++ )
        {
            for ( int y = 0; y < N; y++ )
            {
                if ( fabs ( matrixE ( x, y ) ) > 0.001F )
					std :: cout << "ERROR at [" << x << ", " << y << "]\n";
            }
		}

        break;

#endif

        frameBuffer->Unbind ( );

        glfwSwapBuffers ( );

        //-------------------------------------------------------------------------------
        
        if ( glfwGetKey ( GLFW_KEY_ESC ) || !glfwGetWindowParam ( GLFW_OPENED ) ) break;
    }

	/* Clear resources */

	delete shaderProgram;

	delete frameBuffer;

	delete textureA;

	delete textureB;

	delete textureC;

    glfwTerminate ( );
	
	exit ( 0 );
}