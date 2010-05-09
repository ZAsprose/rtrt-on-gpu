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

#include <fstream>

#include <iostream>

#include <string>

#include <logger.h>

#include "OpenCL.h"

/*
 * Obtain the list of platforms available.
 */
cl_int cltGetPlatforms ( VECTOR < cl_platform_id > * platforms )
{
    cl_uint count = 0;
    
    cl_int error = clGetPlatformIDs (
        0        /* num_entries */,
        NULL     /* platforms */,
        &count   /* num_platforms */ );
    
    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clGetPlatformIDs failed" << std :: endl;

        switch ( error )
        {
            case CL_INVALID_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        return error;
    }
    
    cl_platform_id * ids = new cl_platform_id [count];
    
    error = clGetPlatformIDs (
        count   /* num_entries */,
        ids     /* platforms */,
        NULL    /* num_platforms */ );
    
    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clGetPlatformIDs failed" << std :: endl;

        switch ( error )
        {
            case CL_INVALID_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        delete [] ids;

        return error;
    }
    
    platforms->assign ( ids, ids + count );

    delete [] ids;
    
    return CL_SUCCESS;
}

/*
 * Obtain the list of devices available on a platform.
 */
cl_int cltGetDevices ( cl_platform_id platform,
                       cl_device_type type,
                       VECTOR < cl_device_id > * devices )
{
    cl_uint count = 0;

    cl_int error = clGetDeviceIDs (
        platform   /* platform */,
        type       /* device_type */,
        0          /* num_entries */,
        NULL       /* devices */,
        &count     /* num_devices */ );

    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clGetDeviceIDs failed" << std :: endl;

        switch ( error )
        {
            case CL_INVALID_PLATFORM:
                EZLOGGERSTREAM << "CL_INVALID_PLATFORM" << std :: endl; break;

            case CL_INVALID_DEVICE_TYPE:
                EZLOGGERSTREAM << "CL_INVALID_DEVICE_TYPE" << std :: endl; break;

            case CL_INVALID_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl; break;

            case CL_DEVICE_NOT_FOUND:
                EZLOGGERSTREAM << "CL_DEVICE_NOT_FOUND" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        return error;
    }

    cl_device_id * ids = new cl_device_id [count];

    error = clGetDeviceIDs (
        platform   /* platform */,
        type       /* device_type */,
        count      /* num_entries */,
        ids        /* devices */,
        NULL       /* num_devices */ );

    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clGetDeviceIDs failed" << std :: endl;

        switch ( error )
        {
            case CL_INVALID_PLATFORM:
                EZLOGGERSTREAM << "CL_INVALID_PLATFORM" << std :: endl; break;

            case CL_INVALID_DEVICE_TYPE:
                EZLOGGERSTREAM << "CL_INVALID_DEVICE_TYPE" << std :: endl; break;

            case CL_INVALID_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl; break;

            case CL_DEVICE_NOT_FOUND:
                EZLOGGERSTREAM << "CL_DEVICE_NOT_FOUND" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        delete [] ids;

        return error;
    }

    devices->assign ( ids, ids + count );

    delete [] ids;

    return CL_SUCCESS;
}

/*
 * Obtain the list of devices available on a context.
 */
cl_int cltGetDevices ( cl_context context,
                       VECTOR < cl_device_id > * devices )
{
    cl_uint size = 0;

    cl_int error = clGetContextInfo (
        context              /* context */,
        CL_CONTEXT_DEVICES   /* param_name */,
        0                    /* param_value_size */,
        NULL                 /* param_value */,
        &size                /* param_value_size_ret */ );

    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clGetContextInfo failed" << std :: endl;

        switch ( error )
        {
            case CL_INVALID_CONTEXT:
                EZLOGGERSTREAM << "CL_INVALID_CONTEXT" << std :: endl; break;

            case CL_INVALID_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        return error;
    }

    cl_device_id * ids = ( cl_device_id * ) alloca ( size );

    error = clGetContextInfo (
        context              /* context */,
        CL_CONTEXT_DEVICES   /* param_name */,
        size                 /* param_value_size */,
        ids                  /* param_value */,
        NULL                 /* param_value_size_ret */ );

    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clGetContextInfo failed" << std :: endl;

        switch ( error )
        {
            case CL_INVALID_CONTEXT:
                EZLOGGERSTREAM << "CL_INVALID_CONTEXT" << std :: endl; break;

            case CL_INVALID_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        return error;
    }

    devices->assign ( ids, ids + size / sizeof ( cl_device_id ) );

    return CL_SUCCESS;
}

/***************************************************************************************/

/*
 * Create an OpenCL context from a device type.
 */
cl_context cltCreateContext ( const cl_context_properties * properties,
                              cl_device_type type,
                              cl_int & error )
{
    cl_context context = clCreateContextFromType (
        properties   /* properties */,
        type         /* device_type */,
        NULL         /* pfn_notify */,
        NULL         /* user_data */,
        &error       /* errcode_ret */ );

    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clCreateContextFromType failed" << std :: endl;

        switch ( error )
        {
            case CL_INVALID_PLATFORM:
                EZLOGGERSTREAM << "CL_INVALID_PLATFORM" << std :: endl; break;

            case CL_INVALID_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl; break;

            case CL_DEVICE_NOT_AVAILABLE:
                EZLOGGERSTREAM << "CL_DEVICE_NOT_AVAILABLE" << std :: endl; break;

            case CL_DEVICE_NOT_FOUND:
                EZLOGGERSTREAM << "CL_DEVICE_NOT_FOUND" << std :: endl; break;

            case CL_INVALID_DEVICE_TYPE:
                EZLOGGERSTREAM << "CL_INVALID_DEVICE_TYPE" << std :: endl; break;

            case CL_OUT_OF_HOST_MEMORY:
                EZLOGGERSTREAM << "CL_OUT_OF_HOST_MEMORY" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }
    }
    
    return context;
}

/*
 * Create an OpenCL context with one or more devices.
 */
cl_context cltCreateContext ( const cl_context_properties * properties,
                              const VECTOR < cl_device_id > & devices,
                              cl_int & error )
{
    cl_context context = clCreateContext (
        properties                              /* properties */,
        ( cl_uint ) devices.size ( )            /* num_devices */,
        ( cl_device_id * ) &devices.front ( )   /* devices */,
        NULL                                    /* pfn_notify */,
        NULL                                    /* user_data */,
        &error                                  /* errcode_ret */ );
    
    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clCreateContext failed" << std :: endl;

        switch ( error )
        {
            case CL_INVALID_PLATFORM:
                EZLOGGERSTREAM << "CL_INVALID_PLATFORM" << std :: endl; break;

            case CL_INVALID_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl; break;

            case CL_INVALID_DEVICE:
                EZLOGGERSTREAM << "CL_INVALID_DEVICE" << std :: endl; break;

            case CL_DEVICE_NOT_AVAILABLE:
                EZLOGGERSTREAM << "CL_DEVICE_NOT_AVAILABLE" << std :: endl; break;

            case CL_OUT_OF_HOST_MEMORY:
                EZLOGGERSTREAM << "CL_OUT_OF_HOST_MEMORY" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }
    }
    
    return context;
}

/***************************************************************************************/

/*
 * Create an OpenCL context from a device type.
 */
cl_context cltCreateContext ( cl_platform_id platform,
                              cl_device_type type,
                              cl_int & error )
{
    cl_context_properties properties [] = {
        
        CL_CONTEXT_PLATFORM, ( cl_context_properties ) platform,
        
        0
    };

    return cltCreateContext ( properties, type, error );
}

/*
 * Create an OpenCL context from a device type
 * based on current OpenGL context.
 */
cl_context cltCreateContextFromOpenGL ( cl_platform_id platform,
                                        cl_device_type type,
                                        cl_int & error )
{
    cl_context_properties properties [] = {

        CL_CONTEXT_PLATFORM, ( cl_context_properties ) platform,

        CL_GL_CONTEXT_KHR, ( cl_context_properties ) wglGetCurrentContext ( ),

        CL_WGL_HDC_KHR, ( cl_context_properties ) wglGetCurrentDC ( ),

        0
    };

    return cltCreateContext ( properties, type, error );
}

/*
 * Create an OpenCL context with one or more devices.
 */
cl_context cltCreateContext ( cl_platform_id platform,
                              VECTOR < cl_device_id > & devices,
                              cl_int & error )
{
    cl_context_properties properties [] = {
        
        CL_CONTEXT_PLATFORM, ( cl_context_properties ) platform,
        
        0
    };

    return cltCreateContext ( properties, devices, error );
}

/*
 * Create an OpenCL context with one or more devices
 * based on current OpenGL context.
 */
cl_context cltCreateContextFromOpenGL ( cl_platform_id platform,
                                        VECTOR < cl_device_id > & devices,
                                        cl_int & error )
{
    cl_context_properties properties [] = {

        CL_CONTEXT_PLATFORM, ( cl_context_properties ) platform,

        CL_GL_CONTEXT_KHR, ( cl_context_properties ) wglGetCurrentContext ( ),

        CL_WGL_HDC_KHR, ( cl_context_properties ) wglGetCurrentDC ( ),

        0
    };

    return cltCreateContext ( properties, devices, error );
}

/***************************************************************************************/

/*
 * Create a program object for a context and loads the
 * source code from specified file.
 */
cl_program cltLoadProgram ( cl_context context,
                            const char * filename,
                            cl_int & error )
{
    std :: ifstream file ( filename );

    if ( !file.is_open ( ) )
    {
        EZLOGGERSTREAM << "ERROR: could nor open file" << std :: endl;

        return NULL;
    }
    
    std :: string str (
        std :: istreambuf_iterator < char > ( file ),
        ( std :: istreambuf_iterator < char > ( ) ) );

    const char * source = str.c_str ( );

    size_t length = str.length ( ) + 1;

    cl_program program = clCreateProgramWithSource (
        context   /* context */,
        1         /* count */,
        &source   /* strings */,
        &length   /* lengths */,
        &error    /* errcode_ret */ );

    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clCreateProgramWithSource failed" << std :: endl;

        switch ( error )
        {
            case CL_INVALID_CONTEXT:
                EZLOGGERSTREAM << "CL_INVALID_CONTEXT" << std :: endl; break;

            case CL_INVALID_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl; break;

            case CL_OUT_OF_HOST_MEMORY:
                EZLOGGERSTREAM << "CL_OUT_OF_HOST_MEMORY" << std :: endl; break;    

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }
    }

    file.close ( );

    return program;
}

/*
 * Builds a program executable from the program source.
 */
cl_int cltBuildProgram ( cl_program program,
                         VECTOR < cl_device_id > & devices,
                         const char * options  )
{
    cl_int error = clBuildProgram (
        program                                 /* program */,
        ( cl_uint ) devices.size ( )            /* num_devices */,
        ( cl_device_id * ) &devices.front ( )   /* devices */,
        options                                 /* options */,
        NULL                                    /* pfn_notify */,
        NULL                                    /* user_data */ );

    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clBuildProgram failed" << std :: endl;

        switch ( error )
        {
            case CL_INVALID_PROGRAM:
                EZLOGGERSTREAM << "CL_INVALID_PROGRAM" << std :: endl; break;

            case CL_INVALID_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl; break;

            case CL_INVALID_DEVICE:
                EZLOGGERSTREAM << "CL_INVALID_DEVICE" << std :: endl; break;

            case CL_INVALID_BINARY:
                EZLOGGERSTREAM << "CL_INVALID_BINARY" << std :: endl; break;

            case CL_INVALID_BUILD_OPTIONS:
                EZLOGGERSTREAM << "CL_INVALID_BUILD_OPTIONS" << std :: endl; break;

            case CL_INVALID_OPERATION:
                EZLOGGERSTREAM << "CL_INVALID_OPERATION" << std :: endl; break;

            case CL_COMPILER_NOT_AVAILABLE:
                EZLOGGERSTREAM << "CL_COMPILER_NOT_AVAILABLE" << std :: endl; break;

            case CL_BUILD_PROGRAM_FAILURE:
                EZLOGGERSTREAM << "CL_BUILD_PROGRAM_FAILURE" << std :: endl; break;

            case CL_OUT_OF_HOST_MEMORY:
                EZLOGGERSTREAM << "CL_OUT_OF_HOST_MEMORY" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }
    }

    VECTOR < cl_device_id > :: iterator iterator = devices.begin ( );

    while ( iterator != devices.end ( ) )
    {
        char name [100];

        clGetDeviceInfo (
            ( cl_device_id ) *iterator,
            CL_DEVICE_NAME,
            sizeof ( name ),
            name,
            NULL );

        EZLOGGERSTREAM << "Build info for device \"" << name << "\"" << std :: endl;

        size_t length = 0;

        if ( CL_SUCCESS == clGetProgramBuildInfo (
            program                      /* program */, 
            ( cl_device_id ) *iterator   /* device */, 
            CL_PROGRAM_BUILD_LOG         /* param_name */, 
            NULL                         /* param_value_size */, 
            NULL                         /* param_value */, 
            &length                      /* param_value_size_ret */ ) )
        {
            char * info = ( char * ) malloc ( length );
            
            memset ( info, 0, length );
            
            clGetProgramBuildInfo (
                program                      /* program */, 
                ( cl_device_id ) *iterator   /* device */, 
                CL_PROGRAM_BUILD_LOG         /* param_name */, 
                length                       /* param_value_size */, 
                info                         /* param_value */, 
                NULL                         /* param_value_size_ret */ );
            
            EZLOGGERSTREAM << info << std :: endl;
            
            free ( info );
        }
        else
        {
            EZLOGGERSTREAM << "Failed to load build info for device" << std :: endl;
        }

        iterator++;
    }

    return error;
}

/***************************************************************************************/

/*
 * Enqueues a command to execute a kernel on a device for 1D index space.
 */
cl_int cltRunKernel1D ( cl_command_queue queue,
                        cl_kernel kernel,
                        size_t X,
                        size_t x,
                        cl_event * event )
{
    cl_int error = clEnqueueNDRangeKernel (
        queue    /* command_queue */,
        kernel   /* kernel */,
        1        /* work_dim */,
        NULL     /* global_work_offset */,
        &X       /* global_work_size */,
        &x       /* local_work_size */,
        0        /* num_events_in_wait_list */,
        NULL     /* event_wait_list */,
        event    /* event */ );

    return error;
}

/*
 * Enqueues a command to execute a kernel on a device for 2D index space.
 */
cl_int cltRunKernel2D ( cl_command_queue queue,
                        cl_kernel kernel,
                        size_t X, size_t Y,
                        size_t x, size_t y,
                        cl_event * event )
{
    cl_int error = clEnqueueNDRangeKernel (
        queue    /* command_queue */,
        kernel   /* kernel */,
        2        /* work_dim */,
        NULL     /* global_work_offset */,
        &X       /* global_work_size */,
        &x       /* local_work_size */,
        0        /* num_events_in_wait_list */,
        NULL     /* event_wait_list */,
        event    /* event */ );

    return error;
}

/*
 * Enqueues a command to execute a kernel on a device for 3D index space.
 */
cl_int cltRunKernel3D ( cl_command_queue queue,
                        cl_kernel kernel,
                        size_t X, size_t Y, size_t Z,
                        size_t x, size_t y, size_t z,
                        cl_event * event )
{
    cl_int error = clEnqueueNDRangeKernel (
        queue    /* command_queue */,
        kernel   /* kernel */,
        3        /* work_dim */,
        NULL     /* global_work_offset */,
        &X       /* global_work_size */,
        &x       /* local_work_size */,
        0        /* num_events_in_wait_list */,
        NULL     /* event_wait_list */,
        event    /* event */ );

    return error;
}