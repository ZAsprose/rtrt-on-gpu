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
 * OpenCL objects.
 */

cl_context context;

cl_program program;

cl_kernel kernel;

cl_mem buffer;

cl_command_queue queue;

/*
 * Input buffer and its length.
 */

cl_int length = 262144; // 512 x 512

cl_uint * input;

cl_uint sortDescending = 0;

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
        "Sort.cl",
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
        "Sort"    /* kernel_name */,
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
     * Create buffer object for input array.
     */

    buffer = clCreateBuffer (
        context, 
        CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
        sizeof ( cl_uint ) * length,
        input, 
        &error );

    cltCheckError ( error );
}

cl_int ReleaseOpenCL ( void )
{
    cltCheckError ( clReleaseKernel ( kernel ) );

    cltCheckError ( clReleaseProgram ( program ) );

    cltCheckError ( clReleaseMemObject ( buffer ) );

    cltCheckError ( clReleaseCommandQueue ( queue ) );

    cltCheckError ( clReleaseContext ( context ) );

    return CL_SUCCESS;
}

void StartKernels ( void )
{
    cl_int   status;
    cl_event events[2];

    cl_uint numStages = 0;
    cl_uint temp;

    cl_uint stage;
    cl_uint passOfStage;

    size_t globalThreads  = length / 2;
    size_t localThreads = 256;
    
    
    for ( temp = length; temp > 1; temp >>= 1 )
    {
        ++numStages;
    }

    /*** Set appropriate arguments to the kernel ***/
    /* the input array - also acts as output for this pass (input for next) */

    cltCheckError ( clSetKernelArg (
        kernel,
        0,
        sizeof ( cl_mem ),
        &buffer ) );

    /* length - i.e number of elements in the array */

    cltCheckError ( clSetKernelArg (
        kernel,
        3,
        sizeof ( cl_uint ),
        &length ) );

    /* whether sort is to be in increasing order. CL_TRUE implies increasing */
    
    cltCheckError ( clSetKernelArg (
        kernel,
        4,
        sizeof ( cl_uint ),
        &sortDescending ) );


    for ( stage = 0; stage < numStages; ++stage )
    {
        /* stage of the algorithm */

        cltCheckError ( clSetKernelArg (
            kernel, 
            1, 
            sizeof ( cl_uint ),
            &stage ) );
        
        /* Every stage has stage+1 passes. */

        for ( passOfStage = 0; passOfStage < stage + 1; ++passOfStage )
        {
            /* pass of the current stage */
            cltCheckError ( clSetKernelArg (
                kernel,
                2, 
                sizeof ( cl_uint ),
                &passOfStage ) );
        
            
            cltCheckError ( cltRunKernel1D (
                         queue,
                         kernel,
                         globalThreads,
                         localThreads,
                         &events [0] ) );

            /* wait for the kernel call to finish execution */

            cltCheckError ( clWaitForEvents ( 1, &events [0] ) );
        }
   }

    /* Enqueue readBuffer*/

    cltCheckError ( clEnqueueReadBuffer (
                queue,
                buffer,
                CL_TRUE,
                0,
                length * sizeof ( cl_uint ),
                input,
                0,
                NULL,
                NULL ) );
}

/////////////////////////////////////////////////////////////////////////////////////////
// Entry point for program

int main ( void )
{
    /* We use GLFW for window management and OpenGL output */

    glfwInit ( );

    //-------------------------------------------------------------------------

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

    //-------------------------------------------------------------------------

    input = new cl_uint [length];

    for ( int i = 0; i < length; ++i )
    {
        input [i] = rand ( );
    }

    /*EZLOGGERSTREAM << "INPUT\n\n\n\n";
    for ( int i = 0; i < length; ++i )
    {
        EZLOGGERSTREAM << input [i] << "\n";
    }*/

    /* Setup OpenCL compute environment */

    SetupOpenCL ( );

    //-------------------------------------------------------------------------
    
    GLdouble start = glfwGetTime ( );

    StartKernels ( );

    GLdouble time = glfwGetTime ( ) - start;

    std :: cout << "Sorting time: " << time << std :: endl;
    
    /*EZLOGGERSTREAM << "OUTPUT\n\n\n\n";
    for ( int i = 0; i < length; ++i )
    {
        EZLOGGERSTREAM << input [i] << "\n";
    }*/

    //-------------------------------------------------------------------------

    ReleaseOpenCL ( );

    glfwTerminate ( );
    
    exit ( 0 );
}