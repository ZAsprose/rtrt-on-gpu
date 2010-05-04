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

#define __NO_STD_VECTOR
#define __NO_STD_STRING

#include <CL/cl.hpp>

using namespace cl;

using namespace graphics;

/////////////////////////////////////////////////////////////////////////////////////////
// Camera control with mouse ( orientation ) and keyboard ( position )

Mouse mouse;

Keyboard keyboard;

Camera camera ( Vector3f ( 0.0F, 0.0F, -18.0F ) /* position */,
                Vector3f ( 0.0F, 0.0F, 0.0F )   /* orientation ( Euler angles ) */ );

/////////////////////////////////////////////////////////////////////////////////////////
// OpenCL objects

Platform platform;    /* OpenCL platform */

Context context;      /* OpenCL context for specified device type */

Device device;        /* OpenCL device ( CPU or GPU ) */

CommandQueue queue;   /* OpenCL commandQueue for selected device */

/////////////////////////////////////////////////////////////////////////////////////////
// Position of light source

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

inline void CheckError ( cl_int error, const char * name )
{
    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: " << name
            << " (" << error << ")" << std :: endl;
        
        exit ( EXIT_FAILURE ); 
    }
}

cl_int SetupOpenCL ( cl_device_type deviceType )
{
    /*
     * Have a look at the available platforms and pick either
     * the AMD/NVIDIA one if available or a reasonable default.
     */

    cl_int status = CL_SUCCESS;

    vector <Platform> platforms;

    Platform :: get ( &platforms );

    CheckError ( platforms.size ( ) != 0 ? CL_SUCCESS : -1, "Get platforms" );

    /* Now we select first available platform */

    platform = platforms [0];

    /* Print some info on selected platform */
    {
        string info;

        platform.getInfo ( CL_PLATFORM_VENDOR, &info );
        std :: cout << "CL_PLATFORM_VENDOR: " << info.c_str ( ) << std :: endl;

        platform.getInfo ( CL_PLATFORM_NAME, &info );
        std :: cout << "CL_PLATFORM_NAME: " << info.c_str ( ) << std :: endl;

        platform.getInfo ( CL_PLATFORM_VERSION, &info );
        std :: cout << "CL_PLATFORM_VERSION: " << info.c_str ( ) << std :: endl;

        platform.getInfo ( CL_PLATFORM_PROFILE, &info );
        std :: cout << "CL_PLATFORM_PROFILE: " << info.c_str ( ) << std :: endl;

        std :: cout << std :: endl;
    }

    /*
     * Now we can create context for all devices with
     * specified type ( CPU or GPU ). Note that CPU is
     * available only on AMD/ATI platform. Also we create
     * compute context from OpenGL context. It can be
     * useful for sharing textures.
     */

    cl_context_properties properties [] = {
        CL_CONTEXT_PLATFORM, ( cl_context_properties ) ( platform ) ( ),
        CL_GL_CONTEXT_KHR, ( cl_context_properties ) wglGetCurrentContext ( ),
        CL_WGL_HDC_KHR, ( cl_context_properties ) wglGetCurrentDC ( ),
        0
    };
    
    Context context = Context ( CL_DEVICE_TYPE_GPU, 
                                properties,
                                NULL,
                                NULL,
                                &status );

    CheckError ( status, "Create context" );

    /*
     * Get all devices in the given context.
     */
    
    vector <Device> devices = context.getInfo <CL_CONTEXT_DEVICES> ( );
    
    CheckError ( devices.size ( ) > 0 ? CL_SUCCESS : -1, "No OpenCL devices");

    /* Now we select first available device */

    device = devices [0];

    /* Print some info on selected platform */

    {
        string info;

        device.getInfo ( CL_DEVICE_NAME, &info );
        std :: cout << "CL_DEVICE_NAME: " << info.c_str ( ) << std :: endl;

        device.getInfo ( CL_DEVICE_VENDOR, &info );
        std :: cout << "CL_DEVICE_VENDOR: " << info.c_str ( ) << std :: endl;

        device.getInfo ( CL_DRIVER_VERSION, &info );
        std :: cout << "CL_DRIVER_VERSION: " << info.c_str ( ) << std :: endl;

        device.getInfo ( CL_DEVICE_VERSION, &info );
        std :: cout << "CL_DEVICE_VERSION: " << info.c_str ( ) << std :: endl;

        cl_ulong temp;

        device.getInfo ( CL_DEVICE_MAX_CLOCK_FREQUENCY, &temp );
        std :: cout << "CL_DEVICE_MAX_CLOCK_FREQUENCY: " << temp << std :: endl;

        device.getInfo ( CL_DEVICE_GLOBAL_MEM_SIZE, &temp );
        std :: cout << "CL_DEVICE_GLOBAL_MEM_SIZE: " << temp << std :: endl;

        device.getInfo ( CL_DEVICE_LOCAL_MEM_SIZE, &temp );
        std :: cout << "CL_DEVICE_LOCAL_MEM_SIZE: " << temp << std :: endl;

        cl_device_local_mem_type type;

        device.getInfo ( CL_DEVICE_LOCAL_MEM_TYPE, &type );
        std :: cout << "CL_DEVICE_LOCAL_MEM_TYPE: " << ( type == CL_LOCAL ? 
            "CL_LOCAL" : "CL_GLOBAL" ) << std :: endl;
        
        std :: cout << std :: endl;
    }

    return status;
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

    int width = 640, height = 640, mode = GLFW_WINDOW;

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

    SetupOpenCL ( CL_DEVICE_TYPE_GPU );

    //-------------------------------------------------------------------------

    /* Set parallel projection for drawing dummy quad */
    
    glMatrixMode ( GL_PROJECTION );
    
    glLoadIdentity ( );
    
    glOrtho ( -1.0F, 1.0F, -1.0F, 1.0F, -1.0F, 1.0F );
    
    glMatrixMode ( GL_MODELVIEW );
    
    glLoadIdentity ( );

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

        /* Draw dummy quad with custom fragment shader */      
        
        glBegin ( GL_QUADS );

            glVertex2f ( -1.0F, -1.0F );
            glVertex2f ( -1.0F,  1.0F );
            glVertex2f (  1.0F,  1.0F );
            glVertex2f (  1.0F, -1.0F );

        glEnd ( );

        glfwSwapBuffers ( );

        //---------------------------------------------------------------------
        
        running = !glfwGetKey ( GLFW_KEY_ESC ) &&
            glfwGetWindowParam ( GLFW_OPENED );
    }

    glfwTerminate ( ); exit ( 0 );
}