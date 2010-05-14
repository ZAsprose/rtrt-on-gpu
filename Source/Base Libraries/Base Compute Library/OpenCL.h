/*
   -----------------------------------------------------------------------------
   |                  O P E N C L   T O O L S   L I B R A R Y                  |
   -----------------------------------------------------------------------------
                              
   Copyright (c) 2009 - 2010 Denis Bogolepov ( denisbogol @ gmail.com )

   This library is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free Software
   Foundation, either version 3 of the License, or (at your option) any later
   version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with
   this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _OPENCL_TOOLS_
#define _OPENCL_TOOLS_

#include <vector>

#include <CL/cl.h>

#include <Texture2D.hpp>

#include <Texture3D.hpp>

#include <RenderBuffer.hpp>

#include <CL/cl_gl.h>

/*
 * ------------------------------ NOTE -----------------------------
 * |                                                               |
 * | All functions perform check of errors and print result in log |
 * | ( text file in the folder with a program executable ). If an  |
 * | error occurred the program finishes with code #1.             |
 * |                                                               |
 * -----------------------------------------------------------------
 */

/*
 * Use this macro to set vector class.
 */
#define VECTOR std :: vector

/***************************************************************************************/

/*
 * Use this function to check errors.
 */
void cltCheckError ( cl_int error );

/***************************************************************************************/

/*
 * Obtain the list of platforms available.
 */
VECTOR < cl_platform_id > cltGetPlatforms ( void );

/*
 * Obtain the list of devices available on a platform.
 */
VECTOR < cl_device_id > cltGetDevices ( cl_platform_id platform,
                                        cl_device_type type = CL_DEVICE_TYPE_GPU );

/*
 * Obtain the list of devices available on a context.
 */
VECTOR < cl_device_id > cltGetDevices ( cl_context context );

/***************************************************************************************/

/*
 * Create an OpenCL context from a device type.
 */
cl_context cltCreateContext ( cl_platform_id platform,
                              cl_device_type type = CL_DEVICE_TYPE_GPU );

/*
 * Create an OpenCL context from a device type
 * based on current OpenGL context.
 */
cl_context cltCreateContextFromOpenGL ( cl_platform_id platform,
                                        cl_device_type type = CL_DEVICE_TYPE_GPU );

/*
 * Create an OpenCL context with one or more devices.
 */
cl_context cltCreateContext ( cl_platform_id platform,
                              VECTOR < cl_device_id > & devices );

/*
 * Create an OpenCL context with one or more devices
 * based on current OpenGL context.
 */
cl_context cltCreateContextFromOpenGL ( cl_platform_id platform,
                                        VECTOR < cl_device_id > & devices );

/***************************************************************************************/

/*
 * Create a program object for a context and loads the
 * source code from a specified file.
 */
cl_program cltLoadProgram ( cl_context context,
                            const char * filename,
                            const char * prefix = NULL );

/*
 * Builds a program executable from a program source.
 */
void cltBuildProgram ( cl_program program,
                       VECTOR < cl_device_id > & devices,
                       const char * options = NULL );

/***************************************************************************************/

/*
 * Create an OpenCL buffer object with specified data type.
 * ( template function must be implemented in header file ).
 */
template < class T >
cl_mem cltCreateBuffer ( cl_context context,
                         cl_mem_flags flags,
                         size_t count,
                         T * data = NULL )
{
    cl_int error = CL_SUCCESS;

    cl_mem buffer = clCreateBuffer (
        context                /* context */,
        flags                  /* flags */,
        count * sizeof ( T )   /* size */,
        data                   /* host_ptr */,
        &error                 /* errcode_ret */ );

    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clCreateBuffer failed" << std :: endl;

        switch ( error )
        {
            case CL_INVALID_CONTEXT:
                EZLOGGERSTREAM << "CL_INVALID_CONTEXT" << std :: endl; break;

            case CL_INVALID_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl; break;

            case CL_INVALID_BUFFER_SIZE:
                EZLOGGERSTREAM << "CL_INVALID_BUFFER_SIZE" << std :: endl; break;    

            case CL_INVALID_HOST_PTR:
                EZLOGGERSTREAM << "CL_INVALID_HOST_PTR" << std :: endl; break;

            case CL_MEM_OBJECT_ALLOCATION_FAILURE:
                EZLOGGERSTREAM << "CL_MEM_OBJECT_ALLOCATION_FAILURE" << std :: endl; break;

            case CL_OUT_OF_HOST_MEMORY:
                EZLOGGERSTREAM << "CL_OUT_OF_HOST_MEMORY" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        exit ( EXIT_FAILURE );
    }

    return buffer;
}

/*
 * Create an OpenCL 2D image object from an OpenGL 2D texture object.
 */
cl_mem cltCreateImageFromTexture ( cl_context context,
                                   cl_mem_flags flags,
                                   graphics :: Texture2D * texture );

/*
 * Create an OpenCL 3D image object from an OpenGL 3D texture object.
 */
cl_mem cltCreateImageFromTexture ( cl_context context,
                                   cl_mem_flags flags,
                                   graphics :: Texture3D * texture );

/*
 * Create an OpenCL 2D image object from an OpenGL renderbuffer object.
 */
cl_mem cltCreateImageFromRenderbuffer ( cl_context context,
                                        cl_mem_flags flags,
                                        graphics :: RenderBuffer * buffer );

/***************************************************************************************/

/*
 * NOTE: How we can share textures with OpenCL
 *
 *----------------------------
 *
 * We start in OpenGL land
 *
 * do some GL stuff
 *
 * clEnqueueAcquireGLObjects
 *
 * Now we are in OpenCL land
 *
 * do some CL stuff
 *
 * clEnqueueReleaseGLObjects
 *
 * back to OpenGL land
 *
 *----------------------------
 *
 * clEnqueueAcquireGLObjects and clEnqueueReleaseGLObjects do not do any
 * really heavy handed synchronization they are more of a spec thing.
 */

/*
 * Acquire OpenCL memory object that has been created from OpenGL
 * object. These objects need to be acquired before they can be used
 * by any OpenCL commands queued to a command-queue.
 */
void cltAcquireGraphicsObject ( cl_command_queue queue,
                                const cl_mem & memory,
                                cl_event * event = NULL );

/*
 * Release OpenCL memory objects that have been created from OpenGL
 * objects. These objects need to be released before they can be used
 * by OpenGL.
 */
void cltReleaseGraphicsObject ( cl_command_queue queue,
                                const cl_mem & memory,
                                cl_event * event = NULL );

/***************************************************************************************/

/*
 * Create a command-queue on a specific device.
 */    
cl_command_queue cltCreateQueue ( cl_context context,
                                  cl_device_id device,
                                  cl_command_queue_properties properties = NULL );

/***************************************************************************************/

/*
 * Create a kernel object for a function declared in a program.
 */
cl_kernel cltCreateKernel ( cl_program program, const char * name );

/***************************************************************************************/

/*
 * Set the argument value for a specific argument of a kernel
 * ( template function must be implemented in header file ).
 */
template < class T >
void cltSetArgument ( cl_kernel kernel,
                      cl_uint index,
                      const T * value )
{
    cl_int error = clSetKernelArg (
        kernel         /* kernel */,
        index          /* arg_index */,
        sizeof ( T )   /* arg_size */,
        value          /* arg_value */ );

    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clSetKernelArg failed" << std :: endl;

        EZLOGGERVAR ( index );

        switch ( error )
        {
            case CL_INVALID_KERNEL:
                EZLOGGERSTREAM << "CL_INVALID_KERNEL" << std :: endl; break;

            case CL_INVALID_ARG_INDEX:
                EZLOGGERSTREAM << "CL_INVALID_ARG_INDEX" << std :: endl; break;

            case CL_INVALID_ARG_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_ARG_VALUE" << std :: endl; break;

            case CL_INVALID_MEM_OBJECT:
                EZLOGGERSTREAM << "CL_INVALID_MEM_OBJECT" << std :: endl; break;

            case CL_INVALID_SAMPLER:
                EZLOGGERSTREAM << "CL_INVALID_SAMPLER" << std :: endl; break;

            case CL_INVALID_ARG_SIZE:
                EZLOGGERSTREAM << "CL_INVALID_ARG_SIZE" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        exit ( EXIT_FAILURE );
    }
}

/***************************************************************************************/

/*
 * Enqueues a command to execute a kernel on a device for 1D index space.
 */
void cltRunKernel1D ( cl_command_queue queue,
                      cl_kernel kernel,
                      size_t X,   /* global size */
                      size_t x,   /* local size */
                      cl_event * event = NULL );

/*
 * Enqueues a command to execute a kernel on a device for 2D index space.
 */
void cltRunKernel2D ( cl_command_queue queue,
                      cl_kernel kernel,
                      size_t X, size_t Y,   /* global size */
                      size_t x, size_t y,   /* local size */
                      cl_event * event = NULL );

/*
 * Enqueues a command to execute a kernel on a device for 3D index space.
 */
void cltRunKernel3D ( cl_command_queue queue,
                      cl_kernel kernel,
                      size_t X, size_t Y, size_t Z,   /* global size */
                      size_t x, size_t y, size_t z,   /* local size */
                      cl_event * event = NULL );

#endif