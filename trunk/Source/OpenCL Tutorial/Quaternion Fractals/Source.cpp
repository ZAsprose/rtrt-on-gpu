/*
   ---------------------------------------------------------------------------
   |             Q U A T E R N I O N   F R A C T A L S   ( OCL )             |
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

#include <Compute.hpp>

using namespace graphics;

/////////////////////////////////////////////////////////////////////////////////////////
// Useful macros

#define FRACT( value ) ( value - floorf ( value ) )

#define LERP( A, B, value ) ( ( 1 - value ) * A + value * B )

/////////////////////////////////////////////////////////////////////////////////////////
// Support structure for storing parameters of 4D quaternion Julia set

/*
 * For more info about quaternion Julia sets see:
 *
 * Paul Bourke. "Quaternion Julia Fractals", July 2001
 * http://local.wasp.uwa.edu.au/~pbourke/fractals/quatjulia
 *
 * See also:
 *
 * Keenan Crane. "Ray Tracing Quaternion Julia Sets on the GPU"
 * http://www.devmaster.net/forums/showthread.php?t=4448
 */

struct Fractal
{
    public:

        /* Center of quaternion Julia set */
        Vector4f Center;    

        /* Maximum number of iterations ( higher -> better quality ) */
        GLuint Iterations;

        /* Accuracy of hit point calculation ( higher -> better quality ) */
        GLfloat Epsilon;
        
        Fractal ( GLuint iterations = 100,
                  GLfloat epsilon = 0.0025F )
        {
            Iterations = iterations;

            Epsilon = epsilon;
        }

        void SetShaderData ( ShaderProgram * manager )
        {
            manager->SetUniformFloat ( "Fractal.Epsilon", Epsilon );

            manager->SetUniformInteger ( "Fractal.Iterations", Iterations );

            manager->SetUniformVector ( "Fractal.Center", Center );
        }
};

/////////////////////////////////////////////////////////////////////////////////////////
// Global variables

/*
 * Camera control with mouse ( orientation ) and keyboard ( position ).
 */

Mouse mouse ( 50.0F );

Keyboard keyboard ( 5.0F );

Camera camera ( Vector3f ( 0.0F, 0.0F, -5.0F )   /* position */,
                Vector3f ( 0.0F, 0.0F,  0.0F )   /* orientation ( Euler angles ) */ );

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
 * Structure with parameters of Julia fractal.
 */

Fractal fractal;

/*
 * Base points for interpolating fractal centers.
 */

Vector4f centers [] = {
    Vector4f ( -1.2F, 0.2F, 0.3F, 0.0F ),
    Vector4f ( -0.2F, 0.5F, 0.3F, 0.0F ),
    Vector4f ( -0.1F, 0.8F, 0.6F, 0.0F ),
    Vector4f (  0.1F, 0.8F, 0.6F, 0.0F ),
    Vector4f ( -1.1F, 0.2F, 0.1F, 0.0F ),
    Vector4f ( -0.4F, 0.1F, 0.8F, 0.1F ) };

/*
 * Indices of current and next fractal centers in the array above.
 */

GLint curr = 0;

GLint next = 1;

/*
 * OpenCL objects.
 */

cl_context context;

cl_program program;

cl_kernel kernel;

cl_mem image;

cl_command_queue queue;

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
}

void StartKernels ( void )
{
    /*
     * Set dynamic kernel arguments.
     */

    cl_float4 view = cltFloat4 ( camera.View ( ) );
    
    cltSetArgument < cl_float4 > ( kernel, 1, &view );

    //---------------------------------------------------------------

    cl_float4 up = cltFloat4 ( camera.Up ( ) );
    
    cltSetArgument < cl_float4 > ( kernel, 2, &up );

    //---------------------------------------------------------------

    cl_float4 side = cltFloat4 ( camera.Side ( ) );
    
    cltSetArgument < cl_float4 > ( kernel, 3, &side );

    //---------------------------------------------------------------

    cl_float4 position = cltFloat4 ( camera.Position ( ) );
    
    cltSetArgument < cl_float4 > ( kernel, 4, &position );

    //---------------------------------------------------------------

    cl_float2 scale = cltFloat2 ( 2.0F * camera.Scale ( ) );
    
    cltSetArgument < cl_float2 > ( kernel, 5, &scale );

    //---------------------------------------------------------------
    
    cltSetArgument < int > ( kernel, 6, &width );
    
    cltSetArgument < int > ( kernel, 7, &height );

    //---------------------------------------------------------------
    
    cltSetArgument < Vector4f > ( kernel, 8, &fractal.Center );

    cltSetArgument < cl_uint > ( kernel, 9, &fractal.Iterations );

    cltSetArgument < cl_float > ( kernel, 10, &fractal.Epsilon );

    //---------------------------------------------------------------

    /*
     * Run kernel on GPU.
     */

    cltRunKernel2D (
        queue,
        kernel,
        width, height   /* global size */,
        16, 16          /* local size */ );

    //---------------------------------------------------------------

    /*
     * Wait until the kernel is not completed.
     */
    
    cltFinish ( queue );
}

void ReleaseOpenCL ( void )
{
    cltCheckError ( clReleaseKernel ( kernel ) );

    cltCheckError ( clReleaseProgram ( program ) );

    cltCheckError ( clReleaseMemObject ( image ) );

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

    glfwOpenWindowHint ( GLFW_WINDOW_NO_RESIZE, GL_TRUE );

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

    /* Set parallel projection for drawing dummy quad */

    glMatrixMode ( GL_PROJECTION );

    glLoadIdentity ( );

    glOrtho ( -1.0F, 1.0F, -1.0F, 1.0F, -1.0F, 1.0F );

    glMatrixMode ( GL_MODELVIEW );

    glLoadIdentity ( );

    glEnable ( GL_TEXTURE_RECTANGLE_ARB );

    /* Set default view frustum and window size for the camera */

    camera.SetFrustum ( );

    camera.SetViewport ( width, height );

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

        if ( delta > 4.0 /* interval of FPS averaging */ )
        {
            fps = frames / delta;

            sprintf ( caption, "Quaternion Fractals ( OCL ) - %.1f FPS", fps );

            glfwSetWindowTitle ( caption );

            start = time;

            frames = 0;

            curr = next;

            next = ( curr + 1 ) % 6;
        }

        frames++;
        
        //---------------------------------------------------------------------

        /* Set new camera position and orientation */

        mouse.Apply ( &camera );

        keyboard.Apply ( &camera );

        //---------------------------------------------------------------------

        /* Set fractal center */

        fractal.Center = LERP (
            centers [curr],
            centers [next],
            FRACT ( 0.25 * ( time - start ) ) );

        //---------------------------------------------------------------------

        /* Compute ray traced image using OpenCL kernel */

        cltAcquireGraphicsObject ( queue, image );

        cltFinish ( queue );

        StartKernels ( );

        cltReleaseGraphicsObject ( queue, image );

        cltFinish ( queue );

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

    delete texture;

    glfwTerminate ( );
    
    exit ( EXIT_SUCCESS );
}