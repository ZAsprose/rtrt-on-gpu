/*
 * B A S E   C O M P U T E   L I B R A R Y
 *
 * Copyright (C) 2009 Denis Bogolepov, Andrey Morozov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses.
 */

#ifndef _CL_PLATFORM_HPP_
#define	_CL_PLATFORM_HPP_

#include "Config.hpp"

using namespace std;

namespace compute
{
	/**
	 * Interface for OpenCL device.
	 */
	class Device
	{
		protected:

			/**
			 * Identifier of a specific OpenCL device.
			 */
			cl_device_id identifier;

		public:

			/**
			 * Default constructor ( device is not valid at this point ).
			 */
			Device ( void ) { }

			/**
			 * Creates a new device from an OpenCL device ID.
			 *
			 * @param device the identifier of an OpenCL device.
			 */
			Device ( cl_device_id device )
			{
				identifier = device;
			}

			/**
			 * Creates a new device from a valid device object.
			 *
			 * @param device the device object used for creation.
			 */
			Device ( const Device& device )
			{
				identifier = device.identifier;
			}

			/**
			 * Conversion operator for casting to OpenCL device ID.
			 *
			 * @return The identifier of an OpenCL device.
			 */
			operator cl_device_id ( void ) const
			{
				return identifier;
			}

			/**
			 * Conversion operator for casting to OpenCL device ID.
			 *
			 * @return Reference to identifier of an OpenCL device.
			 */
			operator cl_device_id& ( void )
			{
				return identifier;
			}

			/**
			 * Assigns a device object to device object.
			 *
			 * @param device the device object on right hand side of the assignment.
			 */
			Device& operator = ( const Device &device )
			{
				identifier = device.identifier;

				return *this;
			}

			/**
			 * Returns specific information about an OpenCL device.
			 *
			 * @param name is an enumeration constant that identifies the device
			 *        information being queried. It can be one of the following
			 *        values as specified in table below:
			 *        \li \c CL_DEVICE_TYPE,
			 *        \li \c CL_DEVICE_VENDOR_ID,
			 *        \li \c CL_DEVICE_MAX_COMPUTE_UNITS,
			 *        \li \c CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
			 *        \li \c CL_DEVICE_MAX_WORK_ITEM_SIZES,
			 *        \li \c CL_DEVICE_MAX_WORK_GROUP_SIZE,
			 *        \li \c CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR,
			 *        \li \c CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT,
			 *        \li \c CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT,
			 *        \li \c CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG,
			 *        \li \c CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT,
			 *        \li \c CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE,
			 *        \li \c CL_DEVICE_MAX_CLOCK_FREQUENCY,
			 *        \li \c CL_DEVICE_ADDRESS_BITS,
			 *        \li \c CL_DEVICE_MAX_MEM_ALLOC_SIZE,
			 *        \li \c CL_DEVICE_IMAGE_SUPPORT,
			 *        \li \c CL_DEVICE_MAX_READ_IMAGE_ARGS,
			 *        \li \c CL_DEVICE_MAX_WRITE_IMAGE_ARGS,
			 *        \li \c CL_DEVICE_IMAGE2D_MAX_WIDTH,
			 *        \li \c CL_DEVICE_IMAGE2D_MAX_HEIGHT,
			 *        \li \c CL_DEVICE_IMAGE3D_MAX_WIDTH,
			 *        \li \c CL_DEVICE_IMAGE3D_MAX_HEIGHT,
			 *        \li \c CL_DEVICE_IMAGE3D_MAX_DEPTH,
			 *        \li \c CL_DEVICE_MAX_SAMPLERS,
			 *        \li \c CL_DEVICE_MAX_PARAMETER_SIZE,
			 *        \li \c CL_DEVICE_MEM_BASE_ADDR_ALIGN,
			 *        \li \c CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE,
			 *        \li \c CL_DEVICE_SINGLE_FP_CONFIG,
			 *        \li \c CL_DEVICE_GLOBAL_MEM_CACHE_TYPE,
			 *        \li \c CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
			 *        \li \c CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
			 *        \li \c CL_DEVICE_GLOBAL_MEM_SIZE,
			 *        \li \c CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,
			 *        \li \c CL_DEVICE_MAX_CONSTANT_ARGS,
			 *        \li \c CL_DEVICE_LOCAL_MEM_TYPE,
			 *        \li \c CL_DEVICE_LOCAL_MEM_SIZE,
			 *        \li \c CL_DEVICE_ERROR_CORRECTION_SUPPORT,
			 *        \li \c CL_DEVICE_PROFILING_TIMER_RESOLUTION,
			 *        \li \c CL_DEVICE_ENDIAN_LITTLE,
			 *        \li \c CL_DEVICE_AVAILABLE,
			 *        \li \c CL_DEVICE_COMPILER_AVAILABLE,
			 *        \li \c CL_DEVICE_EXECUTION_CAPABILITIES,
			 *        \li \c CL_DEVICE_QUEUE_PROPERTIES,
			 *        \li \c CL_DEVICE_PLATFORM,
			 *        \li \c CL_DEVICE_NAME,
			 *        \li \c CL_DEVICE_VENDOR,
			 *        \li \c CL_DRIVER_VERSION,
			 *        \li \c CL_DEVICE_PROFILE,
			 *        \li \c CL_DEVICE_VERSION,
			 *        \li \c CL_DEVICE_EXTENSIONS.
			 *
			 * @param param is a pointer to memory location where appropriate values
			 *        for a given \a name will be returned. If value is NULL, it is
			 *        ignored. If \a param returns array, it should be declared
			 *        statically.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_DEVICE if device is not valid.
			 *
			 * @retval CL_INVALID_VALUE if \a name is not one of the supported values.
			 */
			template <typename T>
			cl_int GetInfo ( cl_device_info name, T * param ) const
			{
				return clGetDeviceInfo ( identifier		/* device */,
										 name			/* param_name */,
										 sizeof ( T )	/* param_value_size */,
										 param			/* param_value */,
										 NULL			/* param_value_size_ret */ );
			}
	};

	/**
	 * Interface for OpenCL platform ID.
	 */
	class Platform
	{
		protected:

			/**
			 * Identifier of a specific OpenCL platform.
			 */
			cl_platform_id identifier;

		public:

			/**
			 * Default constructor ( platform is not valid at this point ).
			 */
			Platform ( void ) { }

			/**
			 * Creates a new platform from a platform ID.
			 *
			 * @param platform the identifier of an OpenCL platform.
			 */
			Platform ( cl_platform_id platform )
			{
				identifier = platform;
			}

			/**
			 * Creates a new platform from a valid platform object.
			 *
			 * @param platform the platform object used for creation.
			 */
			Platform ( const Platform& platform )
			{
				identifier = platform.identifier;
			}

			/**
			 * Conversion operator for casting to OpenCL platform ID.
			 *
			 * @return The identifier of an OpenCL platform.
			 */
			operator cl_platform_id ( void ) const
			{
				return identifier;
			}

			/**
			 * Conversion operator for casting to OpenCL platform ID.
			 *
			 * @return Reference to identifier of an OpenCL platform.
			 */
			operator cl_platform_id& ( void )
			{
				return identifier;
			}

			/**
			 * Assigns a platform object to platform object.
			 *
			 * @param platform the platform object on right hand side of the assignment.
			 */
			Platform& operator = ( const Platform& platform );

			/**
			 * Returns specific information about the OpenCL platform.
			 *
			 * @param name is an enum that identifies the platform information being
			 *        queried. It can be one of the following values as specified in
			 *        table below:
			 *        \li \c CL_PLATFORM_PROFILE,
			 *        \li \c CL_PLATFORM_VERSION,
			 *        \li \c CL_PLATFORM_NAME,
			 *        \li \c CL_PLATFORM_VENDOR,
			 *        \li \c CL_PLATFORM_EXTENSIONS.
			 *
			 * @param param is a pointer to memory location where appropriate values
			 *        for a given \a name will be returned. If value is NULL, it is
			 *        ignored. If \a param returns array, it should be declared
			 *        statically.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_PLATFORM if platform is not valid.
			 *
			 * @retval CL_INVALID_VALUE if there is not enough memory for information
			 *         being queried.
			 */
			template <typename T>
			cl_int GetInfo ( cl_device_info name, T * param ) const
			{
				return clGetPlatformInfo ( PlatformID    /* platform */,
										   name          /* param_name */,
										   sizeof ( T )  /* param_value_size */,
										   param         /* param_value */,
										   NULL          /* param_value_size_ret */ );
			}

			/**
			 * Returns the list of available devices.
			 *
			 * @param type is a type bitfield that identifies the type of OpenCL device.
			 *        Currently supported values are:
			 *        \li \c CL_DEVICE_TYPE_CPU,
			 *        \li \c CL_DEVICE_TYPE_GPU,
			 *        \li \c CL_DEVICE_TYPE_ACCELERATOR,
			 *        \li \c CL_DEVICE_TYPE_DEFAULT
			 *
			 *        or a combination of the above.
			 *
			 * @param devices returns a vector of founded OpenCL devices.
			 *
 			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_DEVICE_TYPE if \a type is not a valid value.
			 *
			 * @retval CL_DEVICE_NOT_FOUND if no OpenCL devices that matched \a type
			 *         were found.
			 *
			 * @note The application can query specific capabilities of the OpenCL
			 *       device to determine which device to use.
			 */
			cl_int GetDevices ( cl_device_type type, VECTOR <Device> * devices ) const
			{
				cl_uint count = 0;

				cl_int error = clGetDeviceIDs ( identifier   /* platform */,
												type         /* device_type */,
												0            /* num_entries */,
												NULL         /* devices */,
												&count       /* num_devices */ );

				if ( error != CL_SUCCESS ) return error;

				cl_device_id * ids = new cl_device_id [count];

				error = clGetDeviceIDs ( identifier   /* platform */,
										 type         /* device_type */,
										 count        /* num_entries */,
										 ids          /* devices */,
										 NULL         /* num_devices */ );

				if ( error != CL_SUCCESS ) return error;

				devices->assign ( ids, ids + count );

				return CL_SUCCESS;
			}

			/**
			 * Returns the list of available platforms.
			 *
			 * @param platforms returns a vector of founded OpenCL platforms.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_VALUE if there is no available OpenCL platforms.
			 *
			 * @note The application can query specific capabilities of the OpenCL
			 *       platform to determine which platform to use.
			 */
			static cl_int GetPlatforms ( VECTOR <Platform> * platforms )
			{
				cl_uint count = 0;

				cl_int error = clGetPlatformIDs ( 0        /* num_entries */,
												  NULL     /* platforms */,
												  &count   /* num_platforms */ );

				if ( error != CL_SUCCESS ) return error;

				cl_platform_id * ids = new cl_platform_id [count];

				error = clGetPlatformIDs ( count   /* num_entries */,
										   ids     /* platforms */,
										   NULL    /* num_platforms */ );

				if ( error != CL_SUCCESS ) return error;

				platforms->assign ( ids, ids + count );

				return CL_SUCCESS;
			}
	};

	/**
	 * Interface for OpenCL image format.
	 */
	struct ImageFormat : public cl_image_format
	{
		/**
		 * Constructs an image format.
		 *
		 * @param order number of channels and the memory layout in which
		 *        channels are stored in the image.
		 *
		 * @param type size of the channel data type ( number of bits per
		 *        element ).
		 */
		ImageFormat ( cl_channel_order order = CL_RGBA,
			          cl_channel_type type = CL_FLOAT )
		{
			image_channel_order = order;			
			image_channel_data_type = type;
		}
	};

	/**
	 * Interface for OpenCL context.
	 */
	class Context
	{
		protected:

			/**
			 * OpenCL context.
			 */
			cl_context context;

		public:
			
			/**
			 * Default constructor ( context is not valid at this point ).
			 */
			Context ( void ) { }

			/**
			 * Creates an OpenCL context.
			 *
			 * An OpenCL context is created with one or more devices. Contexts are
			 * used by the OpenCL runtime for managing objects such as command-queues,
			 * memory, program and kernel objects and for executing kernels on one or
			 * more devices specified in the context.
			 *
			 * @param devices is a pointer to a list of OpenCL devices. If more than
			 *        one device is specified in \a devices, a selection criteria may 
			 *        be applied to determine if the list of devices specified can be
			 *        used together to create a context.
			 *
			 * @param properties specifies a list of context property names and their
			 *        corresponding values. Each property name is immediately followed
			 *        by the corresponding desired value. The list is NULL-terminated.
			 *        \a properties can be NULL in which case the platform that is
			 *        selected is implementation-defined. The list of properties is
			 *        described in following table:
			 *        \li \c CL_CONTEXT_PLATFORM.
			 *  
			 * @param notify is a callback function that can be registered by the
			 *        application. This callback function will be used by the runtime
			 *        to report information on errors that occur in this context. This
			 *        callback function may be called asynchronously by the runtime.
			 *        If \a notify is NULL ( default value ) no callback function is
			 *        registered.
			 *
			 * @param data will be passed as the data argument when \a notify is called.
			 *        \a data can be NULL ( default value ).
			 *
			 * @param error will return an appropriate error code. If \a error is NULL
			 *        ( default value ), no error code is returned.
			 *
			 * @retval CL_SUCCESS if the context is created successfully.
			 *
			 * @retval CL_INVALID_VALUE if context property name in \a properties is not
			 *         a supported property name, if the value specified for a supported
			 *         property name is not valid, or if the same property name is
			 *         specified more than once.
			 *
 			 * @retval CL_INVALID_VALUE if \a devices list is empty.
			 *
			 * @retval CL_INVALID_VALUE if \a notify is NULL but \a data is not NULL.
			 *
			 * @retval CL_INVALID_DEVICE if \a devices contains an invalid device or are
			 *         not associated with the specified platform.
			 *
			 * @retval CL_DEVICE_NOT_AVAILABLE if a device in \a devices is currently
			 *         not available.
			 *
			 * @retval CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *         required by the OpenCL implementation on the host.
			 */
			Context ( const VECTOR <Device> & devices,
				      cl_context_properties * properties = NULL,
					  void ( *notify ) ( const char *,
					                     const void *,
										 size_t,
										 void * ) = NULL,
					  void * data = NULL,
					  cl_int * error = NULL )
			{
				context = clCreateContext ( properties                              /* properties */,
											( cl_uint ) devices.size ( )            /* num_devices */,
											( cl_device_id * ) &devices.front ( )   /* devices */,
											notify                                  /* pfn_notify */,
											data                                    /* user_data */,
											error                                   /* errcode_ret */ );
			}

			/**
			 * Creates an OpenCL context from a device type that identifies the specific
			 * device(s) to use.
			 *
			 * @param type is a bit-field that identifies the type of device and is
			 *        described in following table:
			 *        \li \c CL_DEVICE_TYPE_CPU,
			 *        \li \c CL_DEVICE_TYPE_GPU,
			 *        \li \c CL_DEVICE_TYPE_ACCELERATOR,
			 *        \li \c CL_DEVICE_TYPE_DEFAULT.
			 *
			 * @param properties is described in previous definition of \c Context
			 *        constructor.
			 *
			 * @param notify is described in previous definition of \c Context
			 *        constructor.
			 *
			 * @param data is described in previous definition of \c Contex
			 *        constructor.
			 *  
			 * @param error is described in previous definition of \c Context
			 *        constructor.
			 *
			 * @retval CL_SUCCESS if the context is created successfully.
			 *
			 * @retval CL_INVALID_PLATFORM if \a properties is NULL and no platform could
			 *         be selected or if platform value specified in \a properties is not
			 *         a valid platform.
			 *
			 * @retval CL_INVALID_VALUE if context property name in \a properties is not
			 *         a supported property name, if the value specified for a supported
			 *         property name is not valid, or if the same property name is
			 *         specified more than once.
			 *
			 * @retval CL_INVALID_VALUE if \a notify is NULL but \a data is not NULL.
			 *
			 * @retval CL_INVALID_DEVICE_TYPE if \a type is not a valid value.
 			 *
			 * @retval CL_DEVICE_NOT_AVAILABLE if no devices that match \a type are
			 *         currently available.
 			 *
			 * @retval CL_DEVICE_NOT_FOUND if no devices that match \a type were found.
  			 *
			 * @retval CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *         required by the OpenCL implementation on the host.
			 */
			Context ( cl_device_type type,
				      cl_context_properties * properties = NULL,
					  void ( *notify ) ( const char *,
					                     const void *,
										 size_t,
										 void * ) = NULL,
					  void * data = NULL,
					  cl_int * error = NULL )
			{
				context = clCreateContextFromType ( properties   /* properties */,
													type         /* device_type */,
													notify       /* pfn_notify */,
													data         /* user_data */,
													error        /* errcode_ret */ );
			}

			/**
			 * Conversion operator for casting to OpenCL context.
			 *
			 * @return OpenCL context.
			 */
			operator cl_context ( void ) const
			{
				return context;
			}

			/**
			 * Conversion operator for casting to OpenCL context.
			 *
			 * @return Reference to OpenCL context.
			 */
			operator cl_context& ( void )
			{
				return context;
			}

			/**
			 * Returns information about an OpenCL context.
			 *
			 * @param context specifies the OpenCL context being queried.
			 *
			 * @param name is an enum that specifies the information to query. It can
			 *        be one of the following values as specified in table below:
			 *        \li \c CL_CONTEXT_REFERENCE_COUNT,
			 *        \li \c CL_CONTEXT_DEVICES,
			 *        \li \c CL_CONTEXT_PROPERTIES. 
			 *
			 * @param param is a pointer to memory location where appropriate values
			 *        for a given \a name will be returned. If value is NULL, it is
			 *        ignored. If \a param returns array, it should be declared
			 *        statically.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_CONTEXT if context is not valid.
			 *
			 * @retval CL_INVALID_VALUE if \a param is not one of the supported values.
			 *
			 * @note For \c CL_CONTEXT_DEVICES query it is recommended to use \c
			 *       GetDevices method.
			 */
			template <typename T>
			cl_int GetInfo ( cl_context_info name, T * param ) const
			{
				return clGetContextInfo ( context        /* context */,
										  name           /* param_name */,
										  sizeof ( T )   /* param_value_size */,
										  param          /* param_value */,
										  NULL           /* param_value_size_ret */ );
			}

			/**
			 * Returns the list of available devices.
			 *
			 * The given method is the specialised version of \c GetInfo method for
			 * \c CL_CONTEXT_DEVICES query and returns a vector instead of an array.
			 *
			 * @param devices returns a vector of founded OpenCL devices.
			 *
 			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_CONTEXT if context is not valid.
			 *
			 * @note The application can query specific capabilities of the OpenCL
			 *       device to determine which device to use.
			 */
			cl_int GetDevices ( VECTOR <Device> * devices ) const
			{
				cl_uint size = 0;

				cl_int error = clGetContextInfo ( context              /* context */,
										          CL_CONTEXT_DEVICES   /* param_name */,
												  0                    /* param_value_size */,
												  NULL                 /* param_value */,
												  &size                /* param_value_size_ret */ );

				if ( error != CL_SUCCESS ) return error;

				cl_device_id * ids = ( cl_device_id * ) alloca ( size );

				error = clGetContextInfo ( context              /* context */,
					                       CL_CONTEXT_DEVICES   /* param_name */,
										   size                 /* param_value_size */,
										   ids                  /* param_value */,
										   NULL                 /* param_value_size_ret */ );

				if ( error != CL_SUCCESS ) return error;

				devices->assign ( ids, ids + size / sizeof ( cl_device_id ) );

				return CL_SUCCESS;
			}

			/**
			 * Returns information about supported image formats.
			 *
			 * @param flags is a bit-field that is used to specify allocation and usage
			 *        information such as the memory arena that should be used to
			 *        allocate the buffer object and how it will be used. The following
			 *        table describes the possible values for \a flags:
			 *        \li \c CL_MEM_READ_WRITE,
			 *        \li \c CL_MEM_WRITE_ONLY,
 			 *        \li \c CL_MEM_READ_ONLY,
			 *        \li \c CL_MEM_USE_HOST_PTR.
 			 *        \li \c CL_MEM_ALLOC_HOST_PTR,
			 *        \li \c CL_MEM_COPY_HOST_PTR.
			 *
			 * @param type describes the image type and must be one of the following
			 *        values:
			 *        \li \c CL_MEM_OBJECT_IMAGE2D,
			 *        \li \c CL_MEM_OBJECT_IMAGE3D.
			 *
			 * @param formats is a pointer to a memory location where the vector of
			 *        supported image formats are returned. If \a formats is NULL,
			 *        it is ignored.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_CONTEXT if context is not a valid.
			 *
			 * @retval CL_INVALID_VALUE if \a flags or \type are not valid.
			 */
			cl_int GetSupportedImageFormats ( cl_mem_flags flags,
				                              cl_mem_object_type type,
											  VECTOR <ImageFormat> * formats )
			{
				cl_uint count = 0;

				cl_int error = clGetSupportedImageFormats ( context   /* context */,
															flags     /* flags */,
															type      /* image_type */,
															0         /* num_entries */,
															NULL      /* image_formats */,
															&count    /* num_image_formats */ );
				if ( error != CL_SUCCESS )
				{
					return error;
				}

				ImageFormat * value = new ImageFormat [count];

				error = clGetSupportedImageFormats ( context   /* context */,
													 flags     /* flags */,
													 type      /* image_type */,
													 count     /* num_entries */,
													 value     /* image_formats */,
													 NULL      /* num_image_formats */ );

				if ( error != CL_SUCCESS )
				{
					return error;
				}

				formats->assign ( value, value + count );
				
				return CL_SUCCESS;
			}
	};
}

#endif