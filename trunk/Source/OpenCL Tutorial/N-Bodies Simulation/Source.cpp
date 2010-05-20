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

#define frand( ) ( rand ( ) / ( cl_float ) RAND_MAX )

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

cl_mem currPositions;

cl_mem nextPositions;

cl_mem velocities;

cl_command_queue queue;

/*
 * Width and height of the output window.
 */

int width = 512;

int height = 512;


int count = 128 * 128; /* MUST be a nice power of two for simplicity */

float dt = 0.0001F;
float eps = 0.0001F;

int nthread = 64; /* chosen for ATI Radeon HD 5870 */

cl_float4 * pos;

cl_float4 * vel;

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

float testal(Vector4f& xxx)
{
    return xxx.x();
}

/////////////////////////////////////////////////////////////////////////////////////////
// N-Bodies subroutines

void NBodyInit ( void )
{
    pos = ( cl_float4 * ) _aligned_malloc ( count * sizeof ( cl_float4 ), 16 );

    vel  = ( cl_float4 * ) _aligned_malloc ( count * sizeof ( cl_float4 ), 16 );

    srand ( 2112 );

    for ( int i = 0; i < count; i++ )
    {
        pos [i].s [0] = frand();
        pos [i].s [1] = frand();
        pos [i].s [2] = frand();
        pos [i].s [3] = frand();

        vel [i].s [0] = 0.0F;
        vel [i].s [1] = 0.0F;
        vel [i].s [2] = 0.0F;
        vel [i].s [3] = 0.0F;
    }

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

    context = cltCreateContext ( platforms [0] );

    /*
     * Obtain the list of devices available on a platform.
     * In this tutorial we use only first device available.
     */
    
    VECTOR < cl_device_id > devices = cltGetDevices ( context );

    /*
     * Load and build a program object for a context.
     */

    program = cltLoadProgram ( context, "Compute.cl" );
    
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

    kernel = cltCreateKernel ( program, "nbody_kern" );

    /*
     * Create an OpenCL 2D image object from an OpenGL 2D texture
     * object for output rendered scene.
     */

    currPositions = cltCreateBuffer < cl_float4 > ( context, CL_MEM_READ_ONLY, count );

    nextPositions = cltCreateBuffer < cl_float4 > ( context, CL_MEM_WRITE_ONLY, count );

    velocities = cltCreateBuffer < cl_float4 > ( context, CL_MEM_READ_WRITE, count );

    cltSetArgument < cl_float > ( kernel, 0, &dt );

    cltSetArgument < cl_float > ( kernel, 1, &eps );

    cltSetArgument < cl_mem > ( kernel, 4, &velocities );

    cltCheckError ( clSetKernelArg ( kernel, 5, nthread * sizeof ( cl_float4 ), NULL ) );


    cl_int status = clEnqueueWriteBuffer (
        queue                          /* command_queue */,
        currPositions								  /* buffer */,
        CL_TRUE                               /* blocking_read */,
        0                                     /* offset */,
        count * sizeof ( cl_float4 )   /* cb */,
        pos                           /* ptr */,
        0                                     /* num_events_in_wait_list */,
        NULL                                  /* event_wait_list */,
        NULL                           /* event */ );

    status = clEnqueueWriteBuffer (
        queue                          /* command_queue */,
        velocities								  /* buffer */,
        CL_TRUE                               /* blocking_read */,
        0                                     /* offset */,
        count * sizeof ( cl_float4 )   /* cb */,
        vel                           /* ptr */,
        0                                     /* num_events_in_wait_list */,
        NULL                                  /* event_wait_list */,
        NULL                           /* event */ );
}

bool iter = true;

void StartKernels ( void )
{
    if (iter)
    {
        cltSetArgument < cl_mem > ( kernel, 2, &currPositions );

        cltSetArgument < cl_mem > ( kernel, 3, &nextPositions );
    }
    else
    {
        cltSetArgument < cl_mem > ( kernel, 3, &currPositions );

        cltSetArgument < cl_mem > ( kernel, 2, &nextPositions );
    }

    iter = !iter;

    //----------------------------------------------------------

    cltRunKernel1D (
        queue,
        kernel,
        count,
        nthread );

    cltCheckError ( clFinish ( queue ) );

    //----------------------------------------------------------

    cl_int status = clEnqueueReadBuffer (
        queue                          /* command_queue */,
        currPositions								  /* buffer */,
        CL_TRUE                               /* blocking_read */,
        0                                     /* offset */,
        count * sizeof ( cl_float4 )   /* cb */,
        pos                           /* ptr */,
        0                                     /* num_events_in_wait_list */,
        NULL                                  /* event_wait_list */,
        NULL                           /* event */ );
    
    cltCheckError ( clFinish ( queue ) );
}

void ReleaseOpenCL ( void )
{
    cltCheckError ( clReleaseKernel ( kernel ) );

    cltCheckError ( clReleaseProgram ( program ) );

    cltCheckError ( clReleaseMemObject ( currPositions ) );

    cltCheckError ( clReleaseMemObject ( nextPositions ) );

    cltCheckError ( clReleaseMemObject ( velocities ) );

    cltCheckError ( clReleaseCommandQueue ( queue ) );

    cltCheckError ( clReleaseContext ( context ) );
}

void DrawParticles ( int w, int h )
{

    camera.SetViewport ( w, h );

    camera.Setup ( );

    glPointSize(5);

    glEnable(GL_POINT_SMOOTH);



    glBegin ( GL_POINTS );

    for ( int i = 0; i < count; ++i )
    {
        glVertex3fv ( pos [i].s );
    }

    glEnd ( );
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

    NBodyInit ( );

    SetupOpenCL ( );

    SetupKernels ( );

    //-------------------------------------------------------------------------

    /* Set view frustum for camera ( we use default values ) */

    glEnable ( GL_COLOR_MATERIAL );

    glEnable ( GL_BLEND );

    glBlendFunc ( GL_ONE, GL_ONE );	

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

            sprintf (
                caption, "ZZZ ( OCL ) - %.1f FPS", fps );

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

        /* Compute ray traced image using OpenCL kernel */

        StartKernels ( );

        /* Draw dummy quad with custom fragment shader */      
        
        camera.Setup ( );

        glClear ( GL_COLOR_BUFFER_BIT );

        DrawParticles ( width, height );

        glfwSwapBuffers ( );

        //---------------------------------------------------------------------
        
        running = !glfwGetKey ( GLFW_KEY_ESC ) &&
            glfwGetWindowParam ( GLFW_OPENED );
    }

    ReleaseOpenCL ( );

    glfwTerminate ( );
    
    exit ( EXIT_SUCCESS );
}