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

#include <CL/cl_gl.h>

/*
 * Use this macro to set vector class.
 */

#define VECTOR std :: vector

/*
 * Use these macro to check errors.
 */

#define CLT_CHECK_ERROR( val ) if ( CL_SUCCESS != val ) exit ( EXIT_FAILURE )

/***************************************************************************************/

/*
 * Obtain the list of platforms available.
 */
cl_int cltGetPlatforms ( VECTOR < cl_platform_id > * platforms );

/*
 * Obtain the list of devices available on a platform.
 */
cl_int cltGetDevices ( cl_platform_id platform,
                       cl_device_type type,
                       VECTOR < cl_device_id > * devices );

/*
 * Obtain the list of devices available on a context.
 */
cl_int cltGetDevices ( cl_context context,
                       VECTOR < cl_device_id > * devices );

/***************************************************************************************/

/*
 * Create an OpenCL context from a device type.
 */
cl_context cltCreateContext ( cl_platform_id platform,
                              cl_device_type type,
                              cl_int & error );

/*
 * Create an OpenCL context from a device type
 * based on current OpenGL context.
 */
cl_context cltCreateContextFromOpenGL ( cl_platform_id platform,
                                        cl_device_type type,
                                        cl_int & error );

/*
 * Create an OpenCL context with one or more devices.
 */
cl_context cltCreateContext ( cl_platform_id platform,
                              VECTOR < cl_device_id > & devices,
                              cl_int & error );

/*
 * Create an OpenCL context with one or more devices
 * based on current OpenGL context.
 */
cl_context cltCreateContextFromOpenGL ( cl_platform_id platform,
                                        VECTOR < cl_device_id > & devices,
                                        cl_int & error );

/***************************************************************************************/

/*
 * Create a program object for a context and loads the
 * source code from specified file.
 */
cl_program cltLoadProgram ( cl_context context,
                            const char * filename,
                            cl_int & error );

/*
 * Builds a program executable from the program source.
 */
cl_int cltBuildProgram ( cl_program program,
                         VECTOR < cl_device_id > & devices,
                         const char * options = NULL );

/***************************************************************************************/

/*
 * Enqueues a command to execute a kernel on a device for 1D index space.
 */
cl_int cltRunKernel1D ( cl_command_queue queue,
                        cl_kernel kernel,
                        size_t X,
                        size_t x,
                        cl_event * event = NULL );

/*
 * Enqueues a command to execute a kernel on a device for 2D index space.
 */
cl_int cltRunKernel2D ( cl_command_queue queue,
                        cl_kernel kernel,
                        size_t X, size_t Y,
                        size_t x, size_t y,
                        cl_event * event = NULL );

/*
 * Enqueues a command to execute a kernel on a device for 3D index space.
 */
cl_int cltRunKernel3D ( cl_command_queue queue,
                        cl_kernel kernel,
                        size_t X, size_t Y, size_t Z,
                        size_t x, size_t y, size_t z,
                        cl_event * event = NULL );

#endif