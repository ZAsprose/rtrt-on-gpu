/*
   ---------------------------------------------------------------------------
   |                  M E T A B A L L S   D E M O   ( OCL )                  |
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
 * Positions and radiuses of metaballs.
 */

const cl_int count = 8;

cl_float positions [] = {
    0.0F, 0.0F, 0.0F, 0.50F,
    0.0F, 0.0F, 0.0F, 0.75F,
    0.0F, 0.0F, 0.0F, 1.00F,
    0.0F, 0.0F, 0.0F, 1.50F,
    0.0F, 0.0F, 0.0F, 1.50F,
    0.0F, 0.0F, 0.0F, 1.00F,
    0.0F, 0.0F, 0.0F, 0.75F,
    0.0F, 0.0F, 0.0F, 1.50F
};

/*
 * OpenCL objects.
 */

cl_context context;

cl_program program;

cl_kernel kernel;

cl_mem image;

cl_mem metaballs;

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

/////////////////////////////////////////////////////////////////////////////////////////
// Event handlers for mouse and keyboard

void MouseMoveHandler ( GLint x, GLint y )
{
    mouse.MouseMoveHandler ( x, y );
}
        
void MouseDownHandler ( GLint button, GLint state )
{
    mouse.MouseDownHandler ( button, state );
}

void KeyDownHandler ( GLint key, GLint state )
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
     *
     * NOTE: You can create OpenCL context from OpenGL only
     * for GPU device.
     */

    context = cltCreateContextFromOpenGL ( platforms [0] );

    /*
     * Obtain the list of devices available on a platform.
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

    image = cltCreateImageFromTexture ( context, CL_MEM_WRITE_ONLY, texture );
    
    cltSetArgument < cl_mem > ( kernel, 0, &image );

    /*
     * Create an OpenCL buffer object for array of ball positions.
     */

    metaballs = cltCreateBuffer < cl_float4 > ( context, CL_MEM_READ_ONLY, count );
    
    cltSetArgument < cl_mem > ( kernel, 8, &metaballs );
}

void StartKernels ( void )
{
    float view [4] = { camera.View ( ).x ( ),
                       camera.View ( ).y ( ),
                       camera.View ( ).z ( ) };
    
    float up [4] = { camera.Up ( ).x ( ),
                     camera.Up ( ).y ( ),
                     camera.Up ( ).z ( ) };
    
    float side [4] = { camera.Side ( ).x ( ),
                       camera.Side ( ).y ( ),
                       camera.Side ( ).z ( ) };
    
    float position [4] = { camera.Position ( ).x ( ),
                           camera.Position ( ).y ( ),
                           camera.Position ( ).z ( ) };
    
    float scale [2] = { 2.0F * camera.Scale ( ).x ( ),
                        2.0F * camera.Scale ( ).x ( ) };

    //----------------------------------------------------------

    cltSetArgument < float [4] > ( kernel, 1, &view );

    cltSetArgument < float [4] > ( kernel, 2, &up );

    cltSetArgument < float [4] > ( kernel, 3, &side );

    cltSetArgument < float [4] > ( kernel, 4, &position );

    cltSetArgument < float [2] > ( kernel, 5, &scale );
    
    cltSetArgument < int > ( kernel, 6, &width );
    
    cltSetArgument < int > ( kernel, 7, &height );

    //----------------------------------------------------------

    cltCheckError ( clEnqueueWriteBuffer (
        queue,
        metaballs,
        CL_TRUE,
        0,
        count * sizeof ( cl_float4 ),
        positions,
        0,
        NULL,
        NULL ) );

    //----------------------------------------------------------
    
    cltRunKernel2D (
        queue,
        kernel,
        width, height,
        8, 8 );

    //----------------------------------------------------------
    
    cltCheckError ( clFinish ( queue ) );
}

void ReleaseOpenCL ( void )
{
    cltCheckError ( clReleaseKernel ( kernel ) );

    cltCheckError ( clReleaseProgram ( program ) );

    cltCheckError ( clReleaseMemObject ( image ) );

    cltCheckError ( clReleaseMemObject ( metaballs ) );

    cltCheckError ( clReleaseCommandQueue ( queue ) );

    cltCheckError ( clReleaseContext ( context ) );
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
        glfwTerminate ( ); exit ( 0 );
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

            sprintf ( caption, "Metaballs Demo ( OCL ) - %.1f FPS", fps );

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

        /* Move metaballs */

        positions [0]  = sinf ( time ); 
        positions [1]  = cosf ( time );
        positions [2]  = 0.0F;

        positions [4]  = 2.0F * sinf ( time ); 
        positions [5]  = 0.0F;
        positions [6]  = 2.0F * cosf ( time );

        positions [8]  = 0.0F; 
        positions [9]  = 2.0F * sinf ( time );
        positions [10]  = 2.0F * cosf ( time );

        positions [12] = 3.0F * sinf ( time ); 
        positions [13] = 3.0F * sinf ( time );
        positions [14] = 3.0F * cosf ( time );

        positions [16] = 3.0F * cosf ( time );
        positions [17] = 3.0F * sinf ( time );
        positions [18] = 3.0F * cosf ( time );

        positions [20] = 2.0F * sinf ( time ) * cosf ( time );
        positions [21] = 2.0F * sinf ( time ) * cosf ( time );
        positions [22] = 2.0F * cosf ( time );

        positions [24] = 3.0F * sinf ( time ) * cosf ( time );
        positions [25] = 4.0F * sinf ( time ) * cosf ( time );
        positions [26] = 3.0F * cosf ( time );

        positions [28] = 4.0F * cosf ( time ) * cosf ( time );
        positions [29] = 3.0F * sinf ( time );
        positions [30] = 4.0F * cosf ( time ) * cosf ( time );

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

    glfwTerminate ( );
    
    exit ( EXIT_SUCCESS );
}