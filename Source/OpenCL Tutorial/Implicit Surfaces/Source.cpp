/*
   ---------------------------------------------------------------------------
   |          I M P L I C I T   S U R F A C E S   D E M O   ( OCL )          |
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

#define NOMINMAX

#include <iostream>

#include <logger.h>

#include <Graphics.hpp>

#include <OpenCL.h>

using namespace graphics;

/////////////////////////////////////////////////////////////////////////////////////////
// Global variables

/*
 * Camera control with mouse ( orientation ) and keyboard ( position ).
 */

Mouse mouse;

Keyboard keyboard;

Camera camera ( Vector3f ( 0.0F, 0.0F, -18.0F ) /* position */,
                Vector3f ( 0.0F, 0.0F, 0.0F )   /* orientation ( Euler angles ) */ );

/*
 * OpenCL objects.
 */

cl_context context;

cl_program program;

cl_kernel kernel;

cl_mem image;

cl_command_queue queue;

/*
 * Width and height of the output window.
 */

int width = 512;

int height = 512;

/*
 * OpenGL texture for output image from OpenCL.
 */

Texture2D * texture = NULL;

/*
 * Position of light source.
 */

Vector3f lightPosition ( 10.0F, 10.0F, 10.0F );

/////////////////////////////////////////////////////////////////////////////////////////
// Event handlers for mouse and keyboard

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
// OpenCL subroutines

void SetupOpenCL ( void )
{
    /*
     * Obtain the list of platforms available ( AMD / NVIDIA ).
     * In this tutorial we use only first platform available.
     */

    VECTOR < cl_platform_id > platforms = cltGetPlatforms ( );

    /*
     * Create an OpenCL context from a device type based on
     * current OpenGL context.
     */

    context = cltCreateContextFromOpenGL ( platforms [0] );

    /*
     * Obtain the list of devices available on a platform ( CPUs / GPUs ).
     * In this tutorial we use only first device available.
     */
    
    VECTOR < cl_device_id > devices = cltGetDevices ( context );

    /*
     * Load and build a program object for a context.
     */

    program = cltLoadProgram ( context, "Render.cl" );
    
    cltBuildProgram ( program, devices );

    /*
     * Create a command-queue on a specific device.
     */
    
    queue = cltCreateQueue ( context, devices [0] );
}

void SetupKernels ( void )
{
    /*
     * Create a kernel object for a function declared in a program.
     */

    kernel = cltCreateKernel ( program, "Main" );

    /*
     * Create an OpenCL 2D image object from an OpenGL 2D texture
     * object for output rendered scene.
     */

    image = cltCreateFromTexture ( context, CL_MEM_WRITE_ONLY, texture );

    /*
     * Set the argument value for output image.
     */

    cltSetArgument < cl_mem > ( kernel, 0, &image );
}

void StartKernels ( void )
{
    float view [4] = { camera.View ( ).x ( ),
                       camera.View ( ).y ( ),
                       camera.View ( ).z ( ) };

    cltSetArgument < float [4] > ( kernel, 1, &view );

    //----------------------------------------------------------

    float up [4] = { camera.Up ( ).x ( ),
                     camera.Up ( ).y ( ),
                     camera.Up ( ).z ( ) };

    cltSetArgument < float [4] > ( kernel, 2, &up );

    //----------------------------------------------------------

    float side [4] = { camera.Side ( ).x ( ),
                       camera.Side ( ).y ( ),
                       camera.Side ( ).z ( ) };

    cltSetArgument < float [4] > ( kernel, 3, &side );

    //----------------------------------------------------------

    float position [4] = { camera.Position ( ).x ( ),
                           camera.Position ( ).y ( ),
                           camera.Position ( ).z ( ) };

    cltSetArgument < float [4] > ( kernel, 4, &position );

    //----------------------------------------------------------
    
    float scale [2] = { 2.0F * camera.Scale ( ).x ( ),
                        2.0F * camera.Scale ( ).x ( ) };

    cltSetArgument < float [2] > ( kernel, 5, &scale );

    //----------------------------------------------------------

    cltSetArgument < int > ( kernel, 6, &width );

    //----------------------------------------------------------

    cltSetArgument < int > ( kernel, 7, &height );

    //----------------------------------------------------------

    float source [4] = { lightPosition.x ( ),
                         lightPosition.y ( ),
                         lightPosition.z ( ) };

    cltSetArgument < float [4] > ( kernel, 8, &source );

    //----------------------------------------------------------

    cltRunKernel2D (
        queue,
        kernel,
        width, height,
        8, 8 );
    
    clFinish ( queue );
}

cl_int ReleaseOpenCL ( void )
{
    cltCheckError ( clReleaseKernel ( kernel ) );

    cltCheckError ( clReleaseProgram ( program ) );

    cltCheckError ( clReleaseMemObject ( image ) );

    cltCheckError ( clReleaseCommandQueue ( queue ) );

    cltCheckError ( clReleaseContext ( context ) );

    return CL_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Entry point for program

int main ( void )
{
    /* We use GLFW for window management and OpenGL output */


    glfwInit ( );

    //-------------------------------------------------------------------------

    /* Choose video mode ( window or fullscreen ) */

    std :: cout << "Do you want to run program in fullscreen mode? [Y/N]\n";

    int choice = getchar ( );

    int mode = GLFW_WINDOW;

    //-------------------------------------------------------------------------

    /* Set video mode ( window or fullscreen ) */

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

    /* Set event handlers for mouse and keyboard */

    glfwSwapInterval ( 0 );

    glfwSetMousePosCallback ( MouseMoveHandler );

    glfwSetMouseButtonCallback ( MouseDownHandler );

    glfwSetKeyCallback ( KeyDownHandler );

    //-------------------------------------------------------------------------

    /* Setup OpenCL compute environment */

    SetupOpenCL ( );

    //-------------------------------------------------------------------------

    /*
     * NOTE: From ATI Stream SDK v2.1 Developer Release Notes
     *
     * For OpenGL interoperability with OpenCL, there currently is a
     * requirement on when the OpenCL context is created and when
     * texture / buffer shared allocations can be made. To use shared
     * resources, the OpenGL application must create an OpenGL context
     * and then an OpenCL context. All resources ( GL buffers and
     * textures ) created after creation of the OpenCL context can be
     * shared between OpenGL and OpenCL. If resources are allocated before
     * the OpenCL context creation, they cannot be shared between OpenGL
     * and OpenCL.
     */

    /* Create OpenGL texture object */

    texture = new Texture2D (
        0,
        NULL,
        GL_TEXTURE_RECTANGLE_ARB );

    texture->Data = new TextureData2D (
        width      /* width of the texture */,
        height     /* height of the texture */,
        4          /* channels for each texel */,
        GL_TRUE    /* no texture data on CPU */ );

    texture->Setup ( );

    /* Now we can work with the texture */

    SetupKernels ( );

    //-------------------------------------------------------------------------

    /* Set parallel projection for drawing dummy quad */
    
    glMatrixMode ( GL_PROJECTION );
    
    glLoadIdentity ( );
    
    glOrtho ( -1.0F, 1.0F, -1.0F, 1.0F, -1.0F, 1.0F );
    
    glMatrixMode ( GL_MODELVIEW );
    
    glLoadIdentity ( );

    glEnable ( GL_TEXTURE_RECTANGLE_ARB );

    /* Set view frustum for camera ( we use default values ) */

    camera.SetViewFrustum ( );

    //-------------------------------------------------------------------------
    
    GLboolean running = GL_TRUE;

    GLchar caption [100];

    GLint frames = 0;

    GLdouble fps = 0.0,
             delta = 0.0,
             time = 0.0,
             start = glfwGetTime ( );

    //-------------------------------------------------------------------------

    while ( running )
    {
        /* Calculate frames per second ( FPS ) */

        time = glfwGetTime ( );

        delta = time - start;

        if ( delta > 0.1 /* interval of FPS averaging */ )
        {
            fps = frames / delta;

            sprintf ( caption, "Implicit Surfaces Demo - %.1f FPS", fps );

            glfwSetWindowTitle ( caption );

            start = time;

            frames = 0;
        }

        frames++;

        //---------------------------------------------------------------------

        /* Set new window size */
        
        glfwGetWindowSize ( &width, &height );
        
        camera.SetViewport ( width, height );
        
        //---------------------------------------------------------------------

        /* Set new camera position and orientation */

        mouse.Apply ( &camera, fps );

        keyboard.Apply ( &camera, fps );

        //---------------------------------------------------------------------

        /* Move light source */

        lightPosition ( 0 ) = -10.0F;
        lightPosition ( 1 ) =  10.0F + 4.0F * cosf ( time * 1.5F );
        lightPosition ( 2 ) = -10.0F + 4.0F * sinf ( time * 1.5F ); 

        //---------------------------------------------------------------------

        /* Compute ray traced image using OpenCL kernel */

        cltAcquireGraphicsObject ( queue, image );

        clFinish ( queue );

        StartKernels ( );

        cltReleaseGraphicsObject ( queue, image );

        clFinish ( queue );

        /* Draw dummy quad with custom fragment shader */      
        
        glBegin ( GL_QUADS );

            glTexCoord2f (     0,      0 ); glVertex2f ( -1.0F, -1.0F );
            glTexCoord2f (     0, height ); glVertex2f ( -1.0F,  1.0F );
            glTexCoord2f ( width, height ); glVertex2f (  1.0F,  1.0F );
            glTexCoord2f ( width,      0 ); glVertex2f (  1.0F, -1.0F );

        glEnd ( );

        glfwSwapBuffers ( );

        //---------------------------------------------------------------------
        
        running = !glfwGetKey ( GLFW_KEY_ESC ) &&
            glfwGetWindowParam ( GLFW_OPENED );
    }

    ReleaseOpenCL ( );

    glfwTerminate ( ); exit ( EXIT_SUCCESS );
}