/*
   ---------------------------------------------------------------------------
   |                    S Q U A R E   A R R A Y   ( OCL )                    |
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

#include <Compute.hpp>

#define RAND( ) ( rand ( ) / ( cl_float ) RAND_MAX )

/*
 * Size of input and output arrays.
 */

const cl_int count = 1024;

/////////////////////////////////////////////////////////////////////////////////////////
// Global variables

/*
 * Input and output arrays for OpenCL kernel.
 */

float data [count];

float result [count];

/*
 * OpenCL objects.
 */

cl_context context;

cl_program program;

cl_kernel kernel;

cl_mem input;

cl_mem output;

cl_command_queue queue;

/////////////////////////////////////////////////////////////////////////////////////////
// Entry point for program

int main ( void )
{
	// Init input array

	for ( int i = 0; i < count; i++ )
	{
		data [i] = RAND ( );
	}

	// Obtain the list of platforms available ( AMD / NVIDIA )

    VECTOR < cl_platform_id > platforms = cltGetPlatforms ( );

    // Create an OpenCL context from a device type

    context = cltCreateContext ( platforms [0] );

    // Obtain the list of devices available on a platform
    
    VECTOR < cl_device_id > devices = cltGetDevices ( context );

    // Load and build a program object for a context

    program = cltLoadProgram ( context, "Compute.cl" );
    
    cltBuildProgram ( program, devices );

    // Create a command-queue on a specific device
    
    queue = cltCreateQueue ( context, devices [0] );
	
	// Create a kernel object for a function declared in a program

    kernel = cltCreateKernel ( program, "square" );

    // Create an OpenCL buffer objects for input and output arrays

    input = cltCreateBuffer < cl_float > ( context, CL_MEM_READ_ONLY, count );
	
	output = cltCreateBuffer < cl_float > ( context, CL_MEM_WRITE_ONLY, count );

	// Set kernel arguments
	
	cltSetArgument < cl_mem > ( kernel, 0, &input );

	cltSetArgument < cl_mem > ( kernel, 1, &output );

	cltSetArgument < cl_int > ( kernel, 2, &count );

	// Write data to input array

    cltCheckError ( clEnqueueWriteBuffer (
        queue                         /* command_queue */,
        input                         /* buffer */,
        CL_TRUE                       /* blocking_write */,
        0                             /* offset */,
        count * sizeof ( cl_float )   /* cb */,
        data                          /* ptr */,
        0                             /* num_events_in_wait_list */,
        NULL                          /* event_wait_list */,
        NULL                          /* event */ ) );
	
	// Run kernel on GPU

    cltRunKernel1D (
        queue,
        kernel,
        count   /* global size */,
        256     /* local size */ );

	cltFinish ( queue );
	
	// Read data from output array

	cltCheckError ( clEnqueueReadBuffer (
		queue                         /* command_queue */,
		output                        /* buffer */,
		CL_TRUE                       /* blocking_read */,
		0                             /* offset */,
		count * sizeof ( cl_float )   /* cb */,
		result                        /* ptr */,
		0                             /* num_events_in_wait_list */,
		NULL                          /* event_wait_list */,
		NULL                          /* event */) );

	// Check result on CPU

	for ( int i = 0; i < count; i++ )
	{
		if ( fabs ( data [i] * data [i] - result [i] ) > 1E-4 )
			std :: cout << "ERROR at " << i << std :: endl;
	}

	// Release OpenCl resources

	cltCheckError ( clReleaseKernel ( kernel ) );

	cltCheckError ( clReleaseProgram ( program ) );

	cltCheckError ( clReleaseMemObject ( input ) );

	cltCheckError ( clReleaseMemObject ( output ) );

	cltCheckError ( clReleaseCommandQueue ( queue ) );

	cltCheckError ( clReleaseContext ( context ) );
    
    exit ( EXIT_SUCCESS );
}