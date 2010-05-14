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
 * Use this function to check errors.
 */
void cltCheckError ( cl_int error )
{
    if ( CL_SUCCESS != error )
    {
        EZLOGGERSTREAM << "ERROR: OpenCL failed with error " << error << std :: endl;

        exit ( EXIT_FAILURE );
    }
}

/***************************************************************************************/

/*
 * Obtain the list of platforms available.
 */
VECTOR < cl_platform_id > cltGetPlatforms ( void )
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
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        exit ( EXIT_FAILURE );
    }
    
    cl_platform_id * platforms =
        ( cl_platform_id * ) alloca ( count * sizeof ( cl_platform_id ) );
    
    error = clGetPlatformIDs (
        count       /* num_entries */,
        platforms   /* platforms */,
        NULL        /* num_platforms */ );
    
    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clGetPlatformIDs failed" << std :: endl;

        switch ( error )
        {
            case CL_INVALID_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        exit ( EXIT_FAILURE );
    }

    return VECTOR < cl_platform_id > ( platforms, platforms + count );
}

/*
 * Obtain the list of devices available on a platform.
 */
VECTOR < cl_device_id > cltGetDevices ( cl_platform_id platform,
                                        cl_device_type type )
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

        exit ( EXIT_FAILURE );
    }

    cl_device_id * devices =
        ( cl_device_id * ) alloca ( count * sizeof ( cl_device_id ) );

    error = clGetDeviceIDs (
        platform   /* platform */,
        type       /* device_type */,
        count      /* num_entries */,
        devices    /* devices */,
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

        exit ( EXIT_FAILURE );
    }
    
    return VECTOR < cl_device_id > ( devices, devices + count );
}

/*
 * Obtain the list of devices available on a context.
 */
VECTOR < cl_device_id > cltGetDevices ( cl_context context )
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

        exit ( EXIT_FAILURE );
    }

    cl_device_id * devices = ( cl_device_id * ) alloca ( size );

    error = clGetContextInfo (
        context              /* context */,
        CL_CONTEXT_DEVICES   /* param_name */,
        size                 /* param_value_size */,
        devices              /* param_value */,
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

        exit ( EXIT_FAILURE );
    }

    return VECTOR < cl_device_id > (
        devices, devices + size / sizeof ( cl_device_id ) );
}

/***************************************************************************************/

/*
 * Create an OpenCL context from a device type.
 */
cl_context cltCreateContext ( const cl_context_properties * properties,
                              cl_device_type type )
{
    cl_int error = CL_SUCCESS;

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

        exit ( EXIT_FAILURE );
    }
    
    return context;
}

/*
 * Create an OpenCL context with one or more devices.
 */
cl_context cltCreateContext ( const cl_context_properties * properties,
                              const VECTOR < cl_device_id > & devices )
{
    cl_int error = CL_SUCCESS;

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

        exit ( EXIT_FAILURE );
    }
    
    return context;
}

/***************************************************************************************/

/*
 * Create an OpenCL context from a device type.
 */
cl_context cltCreateContext ( cl_platform_id platform,
                              cl_device_type type )
{
    cl_context_properties properties [] = {
        
        CL_CONTEXT_PLATFORM, ( cl_context_properties ) platform,
        
        0
    };

    return cltCreateContext ( properties, type );
}

/*
 * Create an OpenCL context from a device type
 * based on current OpenGL context.
 */
cl_context cltCreateContextFromOpenGL ( cl_platform_id platform,
                                        cl_device_type type )
{
    cl_context_properties properties [] = {

        CL_CONTEXT_PLATFORM, ( cl_context_properties ) platform,

        CL_GL_CONTEXT_KHR, ( cl_context_properties ) wglGetCurrentContext ( ),

        CL_WGL_HDC_KHR, ( cl_context_properties ) wglGetCurrentDC ( ),

        0
    };

    return cltCreateContext ( properties, type );
}

/*
 * Create an OpenCL context with one or more devices.
 */
cl_context cltCreateContext ( cl_platform_id platform,
                              VECTOR < cl_device_id > & devices )
{
    cl_context_properties properties [] = {
        
        CL_CONTEXT_PLATFORM, ( cl_context_properties ) platform,
        
        0
    };

    return cltCreateContext ( properties, devices );
}

/*
 * Create an OpenCL context with one or more devices
 * based on current OpenGL context.
 */
cl_context cltCreateContextFromOpenGL ( cl_platform_id platform,
                                        VECTOR < cl_device_id > & devices )
{
    cl_context_properties properties [] = {

        CL_CONTEXT_PLATFORM, ( cl_context_properties ) platform,

        CL_GL_CONTEXT_KHR, ( cl_context_properties ) wglGetCurrentContext ( ),

        CL_WGL_HDC_KHR, ( cl_context_properties ) wglGetCurrentDC ( ),

        0
    };

    return cltCreateContext ( properties, devices );
}

/***************************************************************************************/

/*
 * Create a program object for a context and loads the
 * source code from a specified file.
 */
cl_program cltLoadProgram ( cl_context context,
                            const char * filename, 
                            const char * prefix )
{
    std :: ifstream file ( filename );

    if ( !file.is_open ( ) )
    {
        EZLOGGERSTREAM << "ERROR: could not open source file " <<
            "\"" << filename << "\"" << std :: endl;

        exit ( EXIT_FAILURE );
    }
    
    std :: string source (
        std :: istreambuf_iterator < char > ( file ),
        ( std :: istreambuf_iterator < char > ( ) ) );
    
    file.close ( );

    if ( NULL != prefix )
    {
        source = std :: string ( prefix ) + source;
    }

    const char * lines = source.c_str ( );

    size_t length = source.length ( ) + 1;

    cl_int error = CL_SUCCESS;

    cl_program program = clCreateProgramWithSource (
        context   /* context */,
        1         /* count */,
        &lines    /* strings */,
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

        exit ( EXIT_FAILURE );
    }

    return program;
}

/*
 * Builds a program executable from the program source.
 */
void cltBuildProgram ( cl_program program,
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

        /* Before exit we must get build info! */
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
            EZLOGGERSTREAM << "Failed to load build info" << std :: endl;
        }

        iterator++;
    }

    if ( error != CL_SUCCESS )
    {
        exit ( EXIT_FAILURE );
    }
}

/***************************************************************************************/

/*
 * Create an OpenCL 2D image object from an OpenGL 2D texture object.
 */
cl_mem cltCreateImageFromTexture ( cl_context context,
                                   cl_mem_flags flags,
                                   graphics :: Texture2D * texture )
{
    cl_int error = CL_SUCCESS;

    cl_mem image = clCreateFromGLTexture2D (
        context               /* context */,
        flags                 /* flags */,
        texture->Target ( )   /* target */,
        0                     /* miplevel */,
        texture->Handle ( )   /* texture */,
        &error                /* errcode_ret */ );

    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clCreateFromGLTexture2D failed" << std :: endl;

        switch ( error )
        {
            case CL_INVALID_CONTEXT:
                EZLOGGERSTREAM << "CL_INVALID_CONTEXT" << std :: endl; break;

            case CL_INVALID_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl; break;

            case CL_INVALID_GL_OBJECT:
                EZLOGGERSTREAM << "CL_INVALID_GL_OBJECT" << std :: endl; break;    

            case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
                EZLOGGERSTREAM << "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR" << std :: endl; break;

            case CL_OUT_OF_HOST_MEMORY:
                EZLOGGERSTREAM << "CL_OUT_OF_HOST_MEMORY" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        exit ( EXIT_FAILURE );
    }

    return image;
}

/*
 * Create an OpenCL 3D image object from an OpenGL 3D texture object.
 */
cl_mem cltCreateImageFromTexture ( cl_context context,
                                   cl_mem_flags flags,
                                   graphics :: Texture3D * texture )
{
    cl_int error = CL_SUCCESS;

    cl_mem image = clCreateFromGLTexture3D (
        context               /* context */,
        flags                 /* flags */,
        texture->Target ( )   /* target */,
        0                     /* miplevel */,
        texture->Handle ( )   /* texture */,
        &error                /* errcode_ret */ );

    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clCreateFromGLTexture3D failed" << std :: endl;

        switch ( error )
        {
            case CL_INVALID_CONTEXT:
                EZLOGGERSTREAM << "CL_INVALID_CONTEXT" << std :: endl; break;

            case CL_INVALID_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl; break;

            case CL_INVALID_GL_OBJECT:
                EZLOGGERSTREAM << "CL_INVALID_GL_OBJECT" << std :: endl; break;    

            case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
                EZLOGGERSTREAM << "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR" << std :: endl; break;

            case CL_OUT_OF_HOST_MEMORY:
                EZLOGGERSTREAM << "CL_OUT_OF_HOST_MEMORY" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        exit ( EXIT_FAILURE );
    }

    return image;
}

/*
 * Create an OpenCL 2D image object from an OpenGL renderbuffer object.
 */
cl_mem cltCreateImageFromRenderbuffer ( cl_context context,
                                        cl_mem_flags flags,
                                        graphics :: RenderBuffer * buffer )
{
    cl_int error = CL_SUCCESS;

    cl_mem image = clCreateFromGLRenderbuffer (
        context              /* context */,
        flags                /* flags */,
        buffer->Target ( )   /* renderbuffer */,
        &error               /* errcode_ret */ );

    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clCreateFromGLRenderbuffer failed" << std :: endl;

        switch ( error )
        {
            case CL_INVALID_CONTEXT:
                EZLOGGERSTREAM << "CL_INVALID_CONTEXT" << std :: endl; break;

            case CL_INVALID_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl; break;

            case CL_INVALID_GL_OBJECT:
                EZLOGGERSTREAM << "CL_INVALID_GL_OBJECT" << std :: endl; break;    

            case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
                EZLOGGERSTREAM << "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR" << std :: endl; break;

            case CL_OUT_OF_HOST_MEMORY:
                EZLOGGERSTREAM << "CL_OUT_OF_HOST_MEMORY" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        exit ( EXIT_FAILURE );
    }

    return image;
}

/***************************************************************************************/

/*
 * Acquire OpenCL memory object that has been created from OpenGL
 * object. These objects need to be acquired before they can be used
 * by any OpenCL commands queued to a command-queue.
 */
void cltAcquireGraphicsObject ( cl_command_queue queue,
                                const cl_mem & memory,
                                cl_event * event )
{
    cl_int error = clEnqueueAcquireGLObjects (
        queue     /* command_queue */,
        1         /* num_objects */,
        &memory   /* mem_objects */,
        0         /* num_events_in_wait_list */,
        NULL      /* event_wait_list */,
        event     /* event */ );

    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clEnqueueAcquireGLObjects failed" << std :: endl;

        switch ( error )
        {
            case CL_INVALID_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl; break;

            case CL_INVALID_MEM_OBJECT:
                EZLOGGERSTREAM << "CL_INVALID_MEM_OBJECT" << std :: endl; break;

            case CL_INVALID_COMMAND_QUEUE:
                EZLOGGERSTREAM << "CL_INVALID_COMMAND_QUEUE" << std :: endl; break;    

            case CL_INVALID_CONTEXT:
                EZLOGGERSTREAM << "CL_INVALID_CONTEXT" << std :: endl; break;

            case CL_INVALID_GL_OBJECT:
                EZLOGGERSTREAM << "CL_INVALID_GL_OBJECT" << std :: endl; break;

            case CL_INVALID_EVENT_WAIT_LIST:
                EZLOGGERSTREAM << "CL_INVALID_EVENT_WAIT_LIST" << std :: endl; break;

            case CL_OUT_OF_HOST_MEMORY:
                EZLOGGERSTREAM << "CL_OUT_OF_HOST_MEMORY" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        exit ( EXIT_FAILURE );
    }
}

/*
 * Release OpenCL memory objects that have been created from OpenGL
 * objects. These objects need to be released before they can be used
 * by OpenGL.
 */
void cltReleaseGraphicsObject ( cl_command_queue queue,
                                const cl_mem & memory,
                                cl_event * event )
{
    cl_int error = clEnqueueReleaseGLObjects (
        queue     /* command_queue */,
        1         /* num_objects */,
        &memory   /* mem_objects */,
        0         /* num_events_in_wait_list */,
        NULL      /* event_wait_list */,
        event     /* event */ );

    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clEnqueueReleaseGLObjects failed" << std :: endl;

        switch ( error )
        {
            case CL_INVALID_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl; break;

            case CL_INVALID_MEM_OBJECT:
                EZLOGGERSTREAM << "CL_INVALID_MEM_OBJECT" << std :: endl; break;

            case CL_INVALID_COMMAND_QUEUE:
                EZLOGGERSTREAM << "CL_INVALID_COMMAND_QUEUE" << std :: endl; break;    

            case CL_INVALID_CONTEXT:
                EZLOGGERSTREAM << "CL_INVALID_CONTEXT" << std :: endl; break;

            case CL_INVALID_GL_OBJECT:
                EZLOGGERSTREAM << "CL_INVALID_GL_OBJECT" << std :: endl; break;

            case CL_INVALID_EVENT_WAIT_LIST:
                EZLOGGERSTREAM << "CL_INVALID_EVENT_WAIT_LIST" << std :: endl; break;

            case CL_OUT_OF_HOST_MEMORY:
                EZLOGGERSTREAM << "CL_OUT_OF_HOST_MEMORY" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        exit ( EXIT_FAILURE );
    }
}

/***************************************************************************************/

/*
 * Create a command-queue on a specific device.
 */    
cl_command_queue cltCreateQueue ( cl_context context,
                                  cl_device_id device,
                                  cl_command_queue_properties properties )
{
    cl_int error = CL_SUCCESS;

    cl_command_queue queue = clCreateCommandQueue (
        context      /* context */,
        device       /* device */,
        properties   /* properties */,
        &error       /* errcode_ret */ );

    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clCreateCommandQueue failed" << std :: endl;

        switch ( error )
        {
            case CL_INVALID_CONTEXT:
                EZLOGGERSTREAM << "CL_INVALID_CONTEXT" << std :: endl; break;

            case CL_INVALID_DEVICE:
                EZLOGGERSTREAM << "CL_INVALID_DEVICE" << std :: endl; break;

            case CL_INVALID_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl; break;    

            case CL_INVALID_QUEUE_PROPERTIES:
                EZLOGGERSTREAM << "CL_INVALID_QUEUE_PROPERTIES" << std :: endl; break;

            case CL_OUT_OF_HOST_MEMORY:
                EZLOGGERSTREAM << "CL_OUT_OF_HOST_MEMORY" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        exit ( EXIT_FAILURE );
    }

    return queue;
}

/***************************************************************************************/

/*
 * Create a kernel object for a function declared in a program.
 */
cl_kernel cltCreateKernel ( cl_program program, const char * name )
{
    cl_int error = CL_SUCCESS;

    cl_kernel kernel = clCreateKernel (
        program   /* program */,
        name      /* kernel_name */,
        &error    /* errcode_ret */ );

    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clCreateKernel failed" << std :: endl;

        switch ( error )
        {
            case CL_INVALID_PROGRAM:
                EZLOGGERSTREAM << "CL_INVALID_PROGRAM" << std :: endl; break;

            case CL_INVALID_PROGRAM_EXECUTABLE:
                EZLOGGERSTREAM << "CL_INVALID_PROGRAM_EXECUTABLE" << std :: endl; break;

            case CL_INVALID_KERNEL_NAME:
                EZLOGGERSTREAM << "CL_INVALID_KERNEL_NAME" << std :: endl; break;    

            case CL_INVALID_KERNEL_DEFINITION:
                EZLOGGERSTREAM << "CL_INVALID_KERNEL_DEFINITION" << std :: endl; break;

            case CL_INVALID_VALUE:
                EZLOGGERSTREAM << "CL_INVALID_VALUE" << std :: endl; break;

            case CL_OUT_OF_HOST_MEMORY:
                EZLOGGERSTREAM << "CL_OUT_OF_HOST_MEMORY" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        exit ( EXIT_FAILURE );
    }

    return kernel;
}

/***************************************************************************************/

/*
 * Enqueues a command to execute a kernel on a device for 1D index space.
 */
void cltRunKernel1D ( cl_command_queue queue,
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

    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clSetKernelArg failed" << std :: endl;

        EZLOGGERVAR ( X );

        EZLOGGERVAR ( x );

        switch ( error )
        {
            case CL_INVALID_PROGRAM_EXECUTABLE:
                EZLOGGERSTREAM << "CL_INVALID_PROGRAM_EXECUTABLE" << std :: endl; break;

            case CL_INVALID_COMMAND_QUEUE:
                EZLOGGERSTREAM << "CL_INVALID_COMMAND_QUEUE" << std :: endl; break;

            case CL_INVALID_KERNEL:
                EZLOGGERSTREAM << "CL_INVALID_KERNEL" << std :: endl; break;

            case CL_INVALID_CONTEXT:
                EZLOGGERSTREAM << "CL_INVALID_CONTEXT" << std :: endl; break;

            case CL_INVALID_KERNEL_ARGS:
                EZLOGGERSTREAM << "CL_INVALID_KERNEL_ARGS" << std :: endl; break;

            case CL_INVALID_WORK_DIMENSION:
                EZLOGGERSTREAM << "CL_INVALID_WORK_DIMENSION" << std :: endl; break;

            case CL_INVALID_WORK_GROUP_SIZE:
                EZLOGGERSTREAM << "CL_INVALID_WORK_GROUP_SIZE" << std :: endl; break;

            case CL_INVALID_WORK_ITEM_SIZE:
                EZLOGGERSTREAM << "CL_INVALID_WORK_ITEM_SIZE" << std :: endl; break;

            case CL_INVALID_GLOBAL_OFFSET:
                EZLOGGERSTREAM << "CL_INVALID_GLOBAL_OFFSET" << std :: endl; break;

            case CL_OUT_OF_RESOURCES:
                EZLOGGERSTREAM << "CL_OUT_OF_RESOURCES" << std :: endl; break;

            case CL_MEM_OBJECT_ALLOCATION_FAILURE:
                EZLOGGERSTREAM << "CL_MEM_OBJECT_ALLOCATION_FAILURE" << std :: endl; break;

            case CL_INVALID_EVENT_WAIT_LIST:
                EZLOGGERSTREAM << "CL_INVALID_EVENT_WAIT_LIST" << std :: endl; break;

            case CL_OUT_OF_HOST_MEMORY:
                EZLOGGERSTREAM << "CL_OUT_OF_HOST_MEMORY" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        exit ( EXIT_FAILURE );
    }
}

/*
 * Enqueues a command to execute a kernel on a device for 2D index space.
 */
void cltRunKernel2D ( cl_command_queue queue,
                      cl_kernel kernel,
                      size_t X, size_t Y,
                      size_t x, size_t y,
                      cl_event * event )
{
    size_t global [] = { X, Y };

    size_t local [] = { x, y };

    cl_int error = clEnqueueNDRangeKernel (
        queue    /* command_queue */,
        kernel   /* kernel */,
        2        /* work_dim */,
        NULL     /* global_work_offset */,
        global   /* global_work_size */,
        local    /* local_work_size */,
        0        /* num_events_in_wait_list */,
        NULL     /* event_wait_list */,
        event    /* event */ );

    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clSetKernelArg failed" << std :: endl;

        EZLOGGERVAR ( X );  EZLOGGERVAR ( Y );

        EZLOGGERVAR ( x );  EZLOGGERVAR ( y );

        switch ( error )
        {
            case CL_INVALID_PROGRAM_EXECUTABLE:
                EZLOGGERSTREAM << "CL_INVALID_PROGRAM_EXECUTABLE" << std :: endl; break;

            case CL_INVALID_COMMAND_QUEUE:
                EZLOGGERSTREAM << "CL_INVALID_COMMAND_QUEUE" << std :: endl; break;

            case CL_INVALID_KERNEL:
                EZLOGGERSTREAM << "CL_INVALID_KERNEL" << std :: endl; break;

            case CL_INVALID_CONTEXT:
                EZLOGGERSTREAM << "CL_INVALID_CONTEXT" << std :: endl; break;

            case CL_INVALID_KERNEL_ARGS:
                EZLOGGERSTREAM << "CL_INVALID_KERNEL_ARGS" << std :: endl; break;

            case CL_INVALID_WORK_DIMENSION:
                EZLOGGERSTREAM << "CL_INVALID_WORK_DIMENSION" << std :: endl; break;

            case CL_INVALID_WORK_GROUP_SIZE:
                EZLOGGERSTREAM << "CL_INVALID_WORK_GROUP_SIZE" << std :: endl; break;

            case CL_INVALID_WORK_ITEM_SIZE:
                EZLOGGERSTREAM << "CL_INVALID_WORK_ITEM_SIZE" << std :: endl; break;

            case CL_INVALID_GLOBAL_OFFSET:
                EZLOGGERSTREAM << "CL_INVALID_GLOBAL_OFFSET" << std :: endl; break;

            case CL_OUT_OF_RESOURCES:
                EZLOGGERSTREAM << "CL_OUT_OF_RESOURCES" << std :: endl; break;

            case CL_MEM_OBJECT_ALLOCATION_FAILURE:
                EZLOGGERSTREAM << "CL_MEM_OBJECT_ALLOCATION_FAILURE" << std :: endl; break;

            case CL_INVALID_EVENT_WAIT_LIST:
                EZLOGGERSTREAM << "CL_INVALID_EVENT_WAIT_LIST" << std :: endl; break;

            case CL_OUT_OF_HOST_MEMORY:
                EZLOGGERSTREAM << "CL_OUT_OF_HOST_MEMORY" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        exit ( EXIT_FAILURE );
    }
}

/*
 * Enqueues a command to execute a kernel on a device for 3D index space.
 */
void cltRunKernel3D ( cl_command_queue queue,
                      cl_kernel kernel,
                      size_t X, size_t Y, size_t Z,
                      size_t x, size_t y, size_t z,
                      cl_event * event )
{
    size_t global [] = { X, Y, Z };

    size_t local [] = { x, y, z };

    cl_int error = clEnqueueNDRangeKernel (
        queue    /* command_queue */,
        kernel   /* kernel */,
        3        /* work_dim */,
        NULL     /* global_work_offset */,
        global   /* global_work_size */,
        local    /* local_work_size */,
        0        /* num_events_in_wait_list */,
        NULL     /* event_wait_list */,
        event    /* event */ );

    if ( error != CL_SUCCESS )
    {
        EZLOGGERSTREAM << "ERROR: clSetKernelArg failed" << std :: endl;

        EZLOGGERVAR ( X );  EZLOGGERVAR ( Y );  EZLOGGERVAR ( Z );

        EZLOGGERVAR ( x );  EZLOGGERVAR ( y );  EZLOGGERVAR ( z );

        switch ( error )
        {
            case CL_INVALID_PROGRAM_EXECUTABLE:
                EZLOGGERSTREAM << "CL_INVALID_PROGRAM_EXECUTABLE" << std :: endl; break;

            case CL_INVALID_COMMAND_QUEUE:
                EZLOGGERSTREAM << "CL_INVALID_COMMAND_QUEUE" << std :: endl; break;

            case CL_INVALID_KERNEL:
                EZLOGGERSTREAM << "CL_INVALID_KERNEL" << std :: endl; break;

            case CL_INVALID_CONTEXT:
                EZLOGGERSTREAM << "CL_INVALID_CONTEXT" << std :: endl; break;

            case CL_INVALID_KERNEL_ARGS:
                EZLOGGERSTREAM << "CL_INVALID_KERNEL_ARGS" << std :: endl; break;

            case CL_INVALID_WORK_DIMENSION:
                EZLOGGERSTREAM << "CL_INVALID_WORK_DIMENSION" << std :: endl; break;

            case CL_INVALID_WORK_GROUP_SIZE:
                EZLOGGERSTREAM << "CL_INVALID_WORK_GROUP_SIZE" << std :: endl; break;

            case CL_INVALID_WORK_ITEM_SIZE:
                EZLOGGERSTREAM << "CL_INVALID_WORK_ITEM_SIZE" << std :: endl; break;

            case CL_INVALID_GLOBAL_OFFSET:
                EZLOGGERSTREAM << "CL_INVALID_GLOBAL_OFFSET" << std :: endl; break;

            case CL_OUT_OF_RESOURCES:
                EZLOGGERSTREAM << "CL_OUT_OF_RESOURCES" << std :: endl; break;

            case CL_MEM_OBJECT_ALLOCATION_FAILURE:
                EZLOGGERSTREAM << "CL_MEM_OBJECT_ALLOCATION_FAILURE" << std :: endl; break;

            case CL_INVALID_EVENT_WAIT_LIST:
                EZLOGGERSTREAM << "CL_INVALID_EVENT_WAIT_LIST" << std :: endl; break;

            case CL_OUT_OF_HOST_MEMORY:
                EZLOGGERSTREAM << "CL_OUT_OF_HOST_MEMORY" << std :: endl; break;

            default:
                EZLOGGERSTREAM << "UNKNOWN_ERROR" << std :: endl;
        }

        exit ( EXIT_FAILURE );
    }
}