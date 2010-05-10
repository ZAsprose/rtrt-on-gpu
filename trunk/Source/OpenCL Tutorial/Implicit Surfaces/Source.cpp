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
    cl_int error = CL_SUCCESS;

    /*
     * Obtain the list of platforms available ( AMD / NVIDIA ).
     * In this tutorial we use only first platform available.
     */

    VECTOR < cl_platform_id > platforms;

    cltCheckError ( cltGetPlatforms ( &platforms ) );

    /*
     * Create an OpenCL context from a device type based on
     * current OpenGL context.
     */

    context = cltCreateContextFromOpenGL ( platforms [0],
                                           CL_DEVICE_TYPE_GPU,
                                           error );

    cltCheckError ( error );

    /*
     * Obtain the list of devices available on a platform ( CPUs / GPUs ).
     * In this tutorial we use only first device available.
     */
    
    VECTOR < cl_device_id > devices;

    cltCheckError ( cltGetDevices ( context, &devices ) );

    /*
     * Load and build a program object for a context.
     */

    program = cltLoadProgram (
        context,
        "Render.cl",
        error );

    cltCheckError ( error );
    
    cltCheckError ( cltBuildProgram (
        program,
        devices ) );

    /*
     * Create a kernel object for a function declared in a program.
     */

    kernel = clCreateKernel (
        program   /* program */,
        "Main"    /* kernel_name */,
        &error    /* errcode_ret */ );

    cltCheckError ( error );

    /*
     * Create a command-queue on a specific device.
     */
    
    queue = clCreateCommandQueue (
        context       /* context */,
        devices [0]   /* device */,
        0             /* properties */,
        &error        /* errcode_ret */ );

    cltCheckError ( error );

    /*
     * Create an OpenCL 2D image object from an OpenGL 2D texture
     * object for output rendered scene.
     */

    image = clCreateFromGLTexture2D (
        context                    /* context */,
        CL_MEM_WRITE_ONLY          /* flags */,
        GL_TEXTURE_RECTANGLE_ARB   /* target */,
        0                          /* miplevel */,
        texture->Handle ( )        /* texture */,
        &error                     /* errcode_ret */ );

    cltCheckError ( error );

    cltCheckError ( clEnqueueAcquireGLObjects (
        queue    /* command_queue */,
        1        /* num_objects */,
        &image   /* mem_objects */,
        0        /* num_events_in_wait_list */,
        NULL     /* event_wait_list */,
        NULL     /* event */ ) );

    /*
     * Set the argument value for output image.
     */

    cltCheckError ( clSetKernelArg (
        kernel              /* kernel */,
        0                   /* arg_index */,
        sizeof ( cl_mem )   /* arg_size */,
        &image              /* arg_value */ ) );
}

cl_int ReleaseOpenCL ( void )
{
    cltCheckError ( clReleaseKernel ( kernel ) );

    cltCheckError ( clReleaseProgram ( program ) );

    cltCheckError ( clEnqueueReleaseGLObjects ( 
        queue    /* command_queue */,
        1        /* num_objects */,
        &image   /* mem_objects */,
        0        /* num_events_in_wait_list */,
        NULL     /* event_wait_list */,
        NULL     /* event */ ) );

    cltCheckError ( clReleaseMemObject ( image ) );

    cltCheckError ( clReleaseCommandQueue ( queue ) );

    cltCheckError ( clReleaseContext ( context ) );

    return CL_SUCCESS;
}

void StartKernels ( void )
{
    float view [4] = { camera.View ( ).x ( ),
                       camera.View ( ).y ( ),
                       camera.View ( ).z ( ) };

    cltCheckError ( clSetKernelArg (
        kernel            /* kernel */,
        1                 /* arg_index */,
        sizeof ( view )   /* arg_size */,
        &view             /* arg_value */ ) );

    //----------------------------------------------------------

    float up [4] = { camera.Up ( ).x ( ),
                     camera.Up ( ).y ( ),
                     camera.Up ( ).z ( ) };

    cltCheckError ( clSetKernelArg (
        kernel          /* kernel */,
        2               /* arg_index */,
        sizeof ( up )   /* arg_size */,
        &up             /* arg_value */ ) );

    //----------------------------------------------------------

    float side [4] = { camera.Side ( ).x ( ),
                       camera.Side ( ).y ( ),
                       camera.Side ( ).z ( ) };

    cltCheckError ( clSetKernelArg (
        kernel            /* kernel */,
        3                 /* arg_index */,
        sizeof ( side )   /* arg_size */,
        &side             /* arg_value */ ) );

    //----------------------------------------------------------

    float position [4] = { camera.Position ( ).x ( ),
                           camera.Position ( ).y ( ),
                           camera.Position ( ).z ( ) };

    cltCheckError ( clSetKernelArg (
        kernel                /* kernel */,
        4                     /* arg_index */,
        sizeof ( position )   /* arg_size */,
        &position             /* arg_value */ ) );

    //----------------------------------------------------------
    
    float scale [2] = { 2.0F * camera.Scale ( ).x ( ),
                        2.0F * camera.Scale ( ).x ( ) };

    cltCheckError ( clSetKernelArg (
        kernel             /* kernel */,
        5                  /* arg_index */,
        sizeof ( scale )   /* arg_size */,
        &scale             /* arg_value */ ) );

    //----------------------------------------------------------

    cltCheckError ( clSetKernelArg (
        kernel           /* kernel */,
        6                /* arg_index */,
        sizeof ( int )   /* arg_size */,
        &width           /* arg_value */ ) );

    //----------------------------------------------------------

    cltCheckError ( clSetKernelArg (
        kernel           /* kernel */,
        7                /* arg_index */,
        sizeof ( int )   /* arg_size */,
        &height          /* arg_value */ ) );

    //----------------------------------------------------------

    float source [4] = { lightPosition.x ( ),
                         lightPosition.y ( ),
                         lightPosition.z ( ) };

    cltCheckError ( clSetKernelArg (
        kernel              /* kernel */,
        8                   /* arg_index */,
        sizeof ( source )   /* arg_size */,
        &source             /* arg_value */ ) );

    //----------------------------------------------------------

    cltCheckError ( cltRunKernel2D (
        queue,
        kernel,
        width,
        height,
        8,
        8 ) );
    
    cltCheckError ( clFinish ( queue ) );
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

    //-------------------------------------------------------------------------

    /* Setup OpenCL compute environment */

    SetupOpenCL ( );

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
        lightPosition ( 1 ) = 10.0F + 4.0F * cosf ( time * 1.5F );
        lightPosition ( 2 ) = -10.0F + 4.0F * sinf ( time * 1.5F ); 

        //---------------------------------------------------------------------

        /* Compute ray traced image using OpenCL kernel */

        StartKernels ( );

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

    glfwTerminate ( ); exit ( 0 );
}