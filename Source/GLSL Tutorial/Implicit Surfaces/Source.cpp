/*
   ---------------------------------------------------------------------------
   |               I M P L I C I T   S U R F A C E S   D E M O               |
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

/////////////////////////////////////////////////////////////////////////////////////////
// Camera control with mouse ( orientation ) and keyboard ( position )

Mouse mouse;

Keyboard keyboard;

Camera camera ( Vector3f ( 0.0F, 0.0F, -18.0F ) /* position */,
                Vector3f ( 0.0F, 0.0F, 0.0F )   /* orientation ( Euler angles ) */ );

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
// Entry point for program

int main ( void )
{
    /* We use GLFW for window management and OpenGL output */

    glfwInit ( );

    //-------------------------------------------------------------------------

    /* Choose video mode ( window or fullscreen ) */

    std :: cout << "Do you want to run program in fullscreen mode? [Y/N]\n";

    int choice = getchar ( );

    int width = 512, height = 512, mode = GLFW_WINDOW;

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

    /* Set parallel projection for drawing dummy quad */
    
    glMatrixMode ( GL_PROJECTION );
    
    glLoadIdentity ( );
    
    glOrtho ( -1.0F, 1.0F, -1.0F, 1.0F, -1.0F, 1.0F );
    
    glMatrixMode ( GL_MODELVIEW );
    
    glLoadIdentity ( );

    /* Set view frustum for camera ( we use default values ) */

    camera.SetFrustum ( );

    //-------------------------------------------------------------------------

    /* Load and compile shader files */

    ShaderProgram program;
    
    if ( !program.LoadVertexShader ( "Vertex.glsl" ) )
    {
        std :: cout << "ERROR: failed to load vertex shader\n"; exit ( -1 );
    }
    
    if ( !program.LoadFragmentShader ( "Fragment.glsl" ) )
    {
        std :: cout << "ERROR: failed to load fragment shader\n"; exit ( -1 );
    }

    if ( !program.Build ( ) )
    {
        std :: cout << "ERROR: failed to build shader program\n"; exit ( -1 );
    }

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

        program.Bind ( );

        camera.SetShaderData ( &program );
        
        program.SetUniformVector ( "LightPosition", lightPosition );
        
        glBegin ( GL_QUADS );

            glVertex2f ( -1.0F, -1.0F );
            glVertex2f ( -1.0F,  1.0F );
            glVertex2f (  1.0F,  1.0F );
            glVertex2f (  1.0F, -1.0F );

        glEnd ( );

        program.Unbind ( );

        glfwSwapBuffers ( );

        //---------------------------------------------------------------------
        
        running = !glfwGetKey ( GLFW_KEY_ESC ) &&
            glfwGetWindowParam ( GLFW_OPENED );
    }

    glfwTerminate ( ); exit ( 0 );
}