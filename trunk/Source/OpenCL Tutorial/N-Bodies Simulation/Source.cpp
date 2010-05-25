/*
   ---------------------------------------------------------------------------
   |        N - B O D I E S   S I M U L A T I O N   D E M O   ( OCL )        |
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

#include <logger.h>

#include <Graphics.hpp>

#include <Compute.hpp>

using namespace graphics;

#define RAND( ) ( rand ( ) / ( cl_float ) RAND_MAX )

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
 * Width and height of the output window.
 */

int width = 512;

int height = 512;

/*
 * OpenCL objects.
 */

cl_context context;

cl_program program;

cl_kernel kernel;

cl_mem oldBuffer;

cl_mem newBuffer;

cl_mem velBuffer;

cl_command_queue queue;

/*
 * Number of particles ( MUST be a nice power of two for simplicity ).
 */

int count = 128 * 128;

/*
 * Initial positions and velocities of particles.
 */

cl_float4 * positions = NULL;

cl_float4 * velocities = NULL;

/*
 * Time step of simulation and smoothing factor.
 */

float timeStep = 0.0001F;

float epsilon = 0.0001F;

/*
 * Local size ( for modern GPUs must be >= 64 ).
 */

cl_int localSize = 64;

/////////////////////////////////////////////////////////////////////////////////////////
// N-Bodies subroutines

void SetupParticles ( void )
{
    /*
     * In ATI/AMD Stream SDK only aligned memory is used.
     */

#ifdef _WIN32

    positions = ( cl_float4 * ) _aligned_malloc ( count * sizeof ( cl_float4 ), 16 );

    velocities = ( cl_float4 * ) _aligned_malloc ( count * sizeof ( cl_float4 ), 16 );

#else

    positions = ( cl_float4 * ) memalign ( 16, count * sizeof ( cl_float4 ) );

    velocities = ( cl_float4 * ) memalign ( 16, count * sizeof ( cl_float4 ) );

#endif

    srand ( 2112 );

    for ( int i = 0; i < count; i++ )
    {
        positions [i].s [0] = RAND ( );
        positions [i].s [1] = RAND ( );
        positions [i].s [2] = RAND ( );
        positions [i].s [3] = RAND ( );

        velocities [i].s [0] = 0.0F;
        velocities [i].s [1] = 0.0F;
        velocities [i].s [2] = 0.0F;
        velocities [i].s [3] = 0.0F;
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

    kernel = cltCreateKernel ( program, "Update" );

    /*
     * Create OpenCL buffer objects for particle positions and velocities.
     */

    oldBuffer = cltCreateBuffer < cl_float4 > ( context, CL_MEM_READ_ONLY, count );

    newBuffer = cltCreateBuffer < cl_float4 > ( context, CL_MEM_WRITE_ONLY, count );

    velBuffer = cltCreateBuffer < cl_float4 > ( context, CL_MEM_READ_WRITE, count );

    /*
     * Set arguments of an OpenCL kernel.
     */

    cltSetArgument < cl_float > ( kernel, 0, &timeStep );

    cltSetArgument < cl_float > ( kernel, 1, &epsilon );

    cltSetArgument < cl_mem > ( kernel, 4, &velBuffer );

    cltCheckError ( clSetKernelArg ( kernel, 5, localSize * sizeof ( cl_float4 ), NULL ) );

    /*
     * Write initial positions and velocities of particles.
     */

    cltCheckError ( clEnqueueWriteBuffer (
        queue                          /* command_queue */,
        oldBuffer                      /* buffer */,
        CL_TRUE                        /* blocking_read */,
        0                              /* offset */,
        count * sizeof ( cl_float4 )   /* cb */,
        positions                      /* ptr */,
        0                              /* num_events_in_wait_list */,
        NULL                           /* event_wait_list */,
        NULL                           /* event */ ) );

    cltCheckError ( clEnqueueWriteBuffer (
        queue                          /* command_queue */,
        velBuffer                      /* buffer */,
        CL_TRUE                        /* blocking_read */,
        0                              /* offset */,
        count * sizeof ( cl_float4 )   /* cb */,
        velocities                     /* ptr */,
        0                              /* num_events_in_wait_list */,
        NULL                           /* event_wait_list */,
        NULL                           /* event */ ) );
}

bool even = false;

void StartKernels ( void )
{
    /*
     * For modeling of process dynamics switching of the input
     * and output position buffers is necessary ( or copy operation
     * on each iteration ).
     */

    if ( even = !even )
    {
        cltSetArgument < cl_mem > ( kernel, 2, &oldBuffer );

        cltSetArgument < cl_mem > ( kernel, 3, &newBuffer );
    }
    else
    {
        cltSetArgument < cl_mem > ( kernel, 3, &oldBuffer );

        cltSetArgument < cl_mem > ( kernel, 2, &newBuffer );
    }

    //----------------------------------------------------------

    cltRunKernel1D (
        queue,
        kernel,
        count,
        localSize   /* set the best size for your GPU */ );

    cltCheckError ( clFinish ( queue ) );

    //----------------------------------------------------------

    cltCheckError ( clEnqueueReadBuffer (
        queue                          /* command_queue */,
        oldBuffer                      /* buffer */,
        CL_TRUE                        /* blocking_read */,
        0                              /* offset */,
        count * sizeof ( cl_float4 )   /* cb */,
        positions                      /* ptr */,
        0                              /* num_events_in_wait_list */,
        NULL                           /* event_wait_list */,
        NULL                           /* event */ ) );
    
    cltCheckError ( clFinish ( queue ) );
}

void ReleaseOpenCL ( void )
{
    cltCheckError ( clReleaseKernel ( kernel ) );

    cltCheckError ( clReleaseProgram ( program ) );

    cltCheckError ( clReleaseMemObject ( oldBuffer ) );

    cltCheckError ( clReleaseMemObject ( newBuffer ) );

    cltCheckError ( clReleaseMemObject ( velBuffer ) );

    cltCheckError ( clReleaseCommandQueue ( queue ) );

    cltCheckError ( clReleaseContext ( context ) );
}

/////////////////////////////////////////////////////////////////////////////////////////
// OpenGL subroutines

/*
 * Draws particles with custom shaders.
 */

void DrawParticles ( int w, int h )
{

    camera.SetViewport ( w, h );

    camera.Setup ( );

    glPointSize ( 3.0F );

    glEnable ( GL_POINT_SMOOTH );


    glBegin ( GL_POINTS );

    for ( int i = 0; i < count; ++i )
    {
        glVertex3fv ( positions [i].s );
    }

    glEnd ( );
}

/*
 * Event handlers for mouse and keyboard.
 */

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

    SetupParticles ( );

    SetupOpenCL ( );

    SetupKernels ( );

    //-------------------------------------------------------------------------

    /* Set view frustum for camera ( we use default values ) */

    glEnable ( GL_COLOR_MATERIAL );

    glEnable ( GL_BLEND );

    glBlendFunc ( GL_ONE, GL_ONE );	

    camera.SetFrustum ( );

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