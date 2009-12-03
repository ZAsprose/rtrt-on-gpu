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

#ifndef _CL_KERNEL_HPP_
#define	_CL_KERNEL_HPP_

#include <vector>

#include <CL/cl.h>

#include "Platform.hpp"

using namespace std;

namespace compute
{
	class Program;

	/**
	 * Interface for OpenCL kernel object.
	 */
	class Kernel
	{
		protected:

			/**
			 * OpenCL kernel object.
			 */
			cl_kernel kernel;

		public:

			/**
			 * Default constructor ( kernel is not valid at this point ).
			 */
			Kernel ( void ) { }
			
			/**
			 * Creates a kernel object.
			 *
			 * A kernel is a function declared in a program. A kernel is identified
			 * by the \c __kernel qualifier applied to any function in a program. A
			 * kernel object encapsulates the specific \c __kernel function declared
			 * in a program and the argument values to be used when executing this \c
			 * __kernel function.
			 *
			 * @param program is a program object with a successfully built executable.
			 *
			 * @param name is a function name in the program declared with the \c
			 *        __kernel qualifier.
			 *
			 * @param error will return an appropriate error code. If \a error is NULL,
			 *        no error code is returned.
			 *
			 * @retval CL_SUCCESS if the kernel object is created successfully.
			 *
			 * @retval CL_INVALID_PROGRAM if \a program is not valid.
			 *
			 * @retval CL_INVALID_PROGRAM_EXECUTABLE if there is no successfully built
			 *         executable for \a program.
			 *
			 * @retval CL_INVALID_KERNEL_NAME if \a name is not found in \a program.
			 *
			 * @retval CL_INVALID_KERNEL_DEFINITION if the function definition for \c
			 *         __kernel function given by \a name such as the number of
			 *         arguments, the argument types are not the same for all devices
			 *         for which the program executable has been built.
			 *
			 * @retval CL_INVALID_VALUE if \a name is NULL.
			 *
			 * @retval CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *         required by the runtime.
			 */
			Kernel ( const Program & program,
				     const char * name,
					 cl_int * error = NULL );

			/**
			 * Conversion operator for casting to OpenCL kernel object.
			 *
			 * @return The OpenCL kernel object.
			 */
			operator cl_kernel ( void ) const
			{
				return kernel;
			}

			/**
			 * Conversion operator for casting to OpenCL kernel object.
			 *
			 * @return Reference to OpenCL kernel object.
			 */
			operator cl_kernel& ( void )
			{
				return kernel;
			}

			/**
			 * Returns information about the kernel object.
			 *
			 * @param name specifies the information to query.
			 *
			 * @param param is a pointer to memory where the appropriate result
			 *        being queried is returned. If \a param is NULL, it is ignored.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_VALUE if \a param is not valid.
			 *
			 * @retval CL_INVALID_KERNEL if \a kernel is a not valid.
			 */
			template <typename T>
			cl_int GetInfo ( cl_kernel_info name, T * param ) const
			{
				return clGetKernelInfo ( kernel         /* kernel */,
										 name           /* param_name */,
										 sizeof ( T )   /* param_value_size */,
										 param          /* param_value */,
										 NULL           /* param_value_size_ret */ );
			}

			/**
			 * Returns information about the kernel object that may be specific
			 * to a device.
			 *
			 * @param device identifies a specific device in the list of devices
			 *        associated with kernel. The list of devices is the list of
			 *        devices in the OpenCL context that is associated with kernel.
			 *        If the list of devices associated with kernel is a single
			 *        device, device can be a NULL value.
			 *
			 * @param name specifies the information to query. The list of supported
			 *        \a name types is described in table below:
			 *        \li \c CL_KERNEL_WORK_GROUP_SIZE,
			 *        \li \c CL_KERNEL_COMPILE_WORK_GROUP_SIZE,
			 *        \li \c CL_KERNEL_LOCAL_MEM_SIZE.
			 *
			 * @param param is a pointer to memory where the appropriate result being
			 *        queried is returned. If \a param is NULL, it is ignored.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_DEVICE if \a device is not in the list of devices
			 *         associated with \a kernel.
			 *
			 * @retval CL_INVALID_VALUE if \a name is not valid.
			 *
			 * @retval CL_INVALID_KERNEL if \a kernel is a not a valid kernel object.
			 */
			template <typename T>
			cl_int GetWorkGroupInfo ( const Device & device,
				                      cl_kernel_work_group_info name,
									  T * param ) const
			{
				return clGetKernelWorkGroupInfo ( device         /* kernel */,
												  kernel         /* device */,
												  name           /* param_name */,
												  sizeof ( T )   /* param_value_size */,
												  param          /* param_value */,
												  NULL           /* param_value_size_ret */ );
			}

			/**
			 * Sets the argument value for a specific argument of a kernel.
			 *
			 * To execute a kernel, the kernel arguments must be set.
			 *
			 * @param index is the argument index. Arguments to the kernel are referred
			 *        by indices that go from 0 for the leftmost argument to n - 1,
			 *        where n is the total number of arguments declared by a kernel.
			 *
			 * @param value is a pointer to data that should be used as the argument
			 *        value for argument specified by \a index. The argument data
			 *        pointed to by \a value is copied and the \a value pointer can
			 *        therefore be reused by the application. The argument value
			 *        specified is the value used by all API calls that enqueue kernel
			 *        until the argument value is changed.
			 *
			 * @retval CL_SUCCESS if the function was executed successfully.
			 *
			 * @retval CL_INVALID_KERNEL if kernel is not valid.
			 *
			 * @retval CL_INVALID_ARG_INDEX if \a index is not valid.
			 *
			 * @retval CL_INVALID_ARG_VALUE if \a value specified is NULL for an
			 *         argument that is not declared with the \c __local qualifier
			 *         or vice-versa.
			 *
			 * @retval CL_INVALID_MEM_OBJECT for an argument declared to be a memory
			 *         object when the specified \a value is not a valid memory object.
			 *
			 * @retval CL_INVALID_SAMPLER for an argument declared to be of type \c
			 *         Sampler when the specified \a value is not a valid sampler
			 *         object.
			 */
			template <typename T>
			cl_int SetArgument ( cl_uint index, T value )
			{
				return clSetKernelArg ( kernel             /* kernel */,
					                    index              /* arg_index */,
										sizeof ( value )   /* arg_size */,
										&value             /* arg_value */ );
			}
	};

	/**
	 * Program interface that implements cl_program.
	 */
	class Program
	{
		protected:

			/**
			 * OpenCL program object.
			 */
			cl_program program;

		public:

			/**
			 * Represents sources of the OpenCL program.
			 */
			typedef vector <pair <const char*, size_t>> Sources;

			/**
			 * Represents binaries of the OpenCL program.
			 */
			typedef vector <pair <const void*, size_t>> Binaries;

			/**
			 * Default constructor ( program is not valid at this point ).
			 */
			Program ( void ) { }
			
			/**
			 * Creates a program object for a context, and loads the source code
			 * specified by the text strings in the strings array into the program
			 * object.
			 *
			 * @param context is a valid OpenCL context.
			 *
			 * @param sources is a source code of the OpenCL program.
			 *
			 * @param error will return an appropriate error code. If \a error is
			 *        NULL, no error code is returned.
			 *
			 * @retval CL_SUCCESS if the program object is created successfully.
			 *
			 * @retval CL_INVALID_CONTEXT if \a context is not valid.
			 *
			 * @retval CL_COMPILER_NOT_AVAILABLE if a compiler is not available.
			 *
			 * @retval CL_OUT_OF_HOST_MEMORY if there is a failure to allocate
			 *         resources required by the runtime.
			 */
			Program ( const Context& context,
				      const Sources& sources,
					  cl_int * error = NULL )
			{
				const int count = sources.size ( );

				size_t * lengths = new size_t [count];
				
				const char ** strings = new const char * [count];

				for ( int i = 0; i < count; i++ )
				{
					strings [i] = sources [i].first;
					lengths [i] = sources [i].second;
				}

				program = clCreateProgramWithSource ( context             /* context */,
					                                  ( cl_uint ) count   /* count */,
													  strings             /* strings */,
													  lengths             /* lengths */,
													  error               /* errcode_ret */ );
			}

			/**
			 * Creates a program object for a context, and loads the binary images 
			 * into the program object.
			 *
			 * @param context is a valid OpenCL context.
			 *
			 * @param devices is a list of devices that are in \a context ( the binaries
			 *        are loaded for devices specified in this list ).
			 *
			 * @param binaries is a program binary to be loaded for devices specified
			 *        by \a devices list. The program binaries specified by \a binaries
			 *        contain the bits that describe the program executable that will
			 *        be run on the device(s) associated with \a context. The program
			 *        binary can consist of either or both:
			 *        \li Device-specific executable(s),
			 *        \li Implementation specific intermediate representation (IR).
			 *
			 * @param status returns whether the program binary for each device specified
			 *        in \a devices was loaded successfully or not. It is an list with
			 *        the same length as \a devices list and returns \c CL_SUCCESS in
			 *        \a status[i] if binary was successfully loaded for device specified
			 *        by \a devices[i]; otherwise returns \c CL_INVALID_VALUE if \a
			 *        binaries[i] is a NULL value or \c CL_INVALID_BINARY in \a status[i]
			 *        if program binary is not a valid binary for the specified device.
			 *        If \a status is NULL, it is ignored.
			 *
			 * @param error will return an appropriate error code. If \a error is NULL,
			 *        no error code is returned.
			 *
			 * @retval CL_SUCCESS if the program object is created successfully.
			 *
			 * @retval CL_INVALID_CONTEXT if \a context is not valid.
			 *
			 * @retval CL_INVALID_VALUE if \a devices has a size of zero.
			 *
			 * @retval CL_INVALID_DEVICE if OpenCL devices listed in \a devices are not 
			 *         in the list of devices associated with \a context.
			 *
			 * @retval CL_INVALID_BINARY if an invalid program binary was encountered 
			 *         for any device ( \a status will return specific status for each
			 *         device ).
			 *
			 * @retval CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *         required by the runtime.
			 */
			Program ( const Context & context,
				      const vector <Device> & devices,
					  const Binaries & binaries,
					  vector <cl_int> * status = NULL,
					  cl_int * error = NULL )
			{
				const int count = binaries.size ( );

				size_t * lengths = new size_t [count];

				const unsigned char ** images = new const unsigned char * [count];

				for ( int i = 0; i < count; i++ )
				{
					images [i] = ( const unsigned char * ) binaries [i].first;

					lengths [i] = binaries [i].second;
				}

				program = clCreateProgramWithBinary (
					context                                                   /* context */,
					( cl_uint ) devices.size ( )                              /* num_devices */,
					( cl_device_id * ) &devices.front ( )                     /* device_list */,
					lengths                                                   /* lengths */,
					images                                                    /* binaries */,
					status != NULL ? ( cl_int * ) &status->front ( ) : NULL   /* binary_status */,
					error                                                     /* errcode_ret */);
			}

			/**
			 * Conversion operator for casting to OpenCL program object.
			 *
			 * @return The OpenCL program object.
			 */
			operator cl_program ( void ) const
			{
				return program;
			}

			/**
			 * Conversion operator for casting to OpenCL program object.
			 *
			 * @return Reference to OpenCL program object.
			 */
			operator cl_program& ( void )
			{
				return program;
			}

			/**
			 * Builds a program executable from the program source or binary for all
			 * the devices or a specific device(s) in the OpenCL context associated
			 * with program.
			 *
			 * OpenCL allows program executables to be built using the sources or
			 * binaries.
			 *
			 * @param program is the program object.
			 *
			 * @param devices is a list of devices associated with \a program. The
			 *        program executable is built for devices specified in \a devices
			 *        for which a source or binary has been loaded.
			 *
			 * @param options is a pointer to a string that describes the build options
			 *        to be used for building the program executable. \options can be
			 *        NULL and defaults to this value if not given.
			 *
			 * @param notify is a function pointer to a notification routine. The
			 *        notification routine allows an application to register a callback
			 *        function which will be called when the program executable has been
			 *        built ( successfully or unsuccessfully ). If \a notify is not NULL,
			 *        build does not need to wait for completion and can return immediately.
			 *        If \a notify is NULL, its default value, build does not return until
			 *        the build has completed. This callback function may be called
			 *        asynchronously by the OpenCL implementation. It is the application's
			 *        responsibility to ensure that the callback function is thread-safe.
			 *
			 * @param data will be passed as the argument when \a notify is called. \a data
			 *        can be NULL and is its default value.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_DEVICE if OpenCL devices listed in \a devices are not in
			 *         the list of devices associated with \a program.
			 *
			 * @retval CL_INVALID_BINARY if \a program is created with binary and devices
			 *         listed in \a devices do not have a valid program binary loaded.
			 *
			 * @retval CL_INVALID_BUILD_OPTIONS if the build options specified by \a options
			 *         are invalid.
			 *
			 * @retval CL_INVALID_OPERATION if the build of a program executable for any of
			 *         the devices listed in \a devices by a previous call to build for
			 *         \a program has not completed.
			 *
			 * @retval CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *         required by the runtime.
			 */
			cl_int Build ( const vector <Device> & devices,
				           const char * options = NULL,
						   void ( *notify ) ( cl_program, void * ) = NULL,
						   void * data = NULL ) const
			{
				return clBuildProgram ( program                                 /* program */,
					                    ( cl_uint ) devices.size ( )            /* num_devices */,
										( cl_device_id * ) &devices.front ( )   /* device_list */,
										options                                 /* options */,
										notify                                  /* pfn_notify */,
										data                                    /* user_data */ );
			}

			/**
			 * Returns information about the program object.
			 *
			 * @param name specifies the information to query.
			 *
			 * @param param is a pointer to memory where the appropriate result being
			 *        queried is returned. If \a param is NULL, it is ignored.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_VALUE if \a name is not valid.
			 *
			 * @retval CL_INVALID_PROGRAM if \a program is a not a valid program object.
			 */
			template <typename T>
			cl_int GetInfo ( cl_program_info name, T * param ) const
			{
				return clGetProgramInfo ( program        /* program */,
					                      name           /* param_name */,
										  sizeof ( T )   /* param_value_size */,
										  param          /* param_value */,
										  NULL           /* param_value_size_ret */ );
			}

			/**
			 * Returns build information for each device in the program object.
			 *
			 * @param device specifies the device for which build information is being
			 *        queried. \a device must be a valid device associated with \a program.
			 *
			 * @param name specifies the information to query.
			 *
			 * @param param is a pointer to memory where the appropriate result being
			 *        queried is returned. If \a param_value is NULL, it is ignored.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_DEVICE if \a device is not in the list of devices
			 *         associated with \a program.
			 *
			 * @retval CL_INVALID_VALUE if \a name is not valid.
			 *
			 * @retval CL_INVALID_PROGRAM if \a program is a not a valid program object.
			 */
			template <typename T>
			cl_int GetBuildInfo ( const Device& device,
				                  cl_program_build_info name,
								  T * param ) const
			{
				return clGetProgramBuildInfo ( program        /* program */,
					                           device         /* device */,
											   name           /* param_name */,
											   sizeof ( T )   /* param_value_size */,
											   param          /* param_value */,
											   NULL           /* param_value_size_ret */ );
			}

			/**
			 * Creates kernel objects for all kernel functions in program.
			 *
			 * Kernel objects may not be created for any kernel functions in program that
			 * do not have the same function definition across all devices for which a
			 * program executable has been successfully built.
			 *
			 * @param kernels is the vector where the kernel objects for kernels in
			 *        \a program will be returned. If \a kernels is NULL, it is ignored.
			 *
			 * @retval CL_SUCCESS if the kernel objects were successfully allocated.
			 *
			 * @retval CL_INVALID_PROGRAM_EXECUTABLE if there is no successfully built
			 *         executable for any device in \a program.
			 *
			 * @retval CL_INVALID_VALUE if \a kernels is not NULL and its size is less
			 *         than the number of kernels in program.
			 *
			 * @retval CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *         required by the runtime.
			 *
			 * @note Kernel objects can only be created once you have a program object with
			 *       a valid program source or binary loaded into the program object and the
			 *       program executable has been successfully built for one or more devices
			 *       associated with \a program. No changes to the program executable are
			 *       allowed while there are kernel objects associated with a program object.
			 *       This means that calls to \a build return CL_INVALID_OPERATION if there
			 *       are kernel objects attached to a program object. The OpenCL context
			 *       associated with program will be the context associated with kernel.
			 *       Devices associated with a program object for which a valid program
			 *       executable has been built can be used to execute kernels declared in the
			 *       program object.
			 */
			cl_int CreateKernels ( vector <Kernel> * kernels )
			{
				cl_uint count = 0;

				cl_int error = clCreateKernelsInProgram ( program   /* program */,
					                                      0         /* num_kernels */,
														  NULL      /* kernels */,
														  &count    /* num_kernels_ret */ );

				if ( error != CL_SUCCESS ) return error;

				Kernel * values = new Kernel [count];

				error = clCreateKernelsInProgram ( program                  /* program */,
					                               count                    /* num_kernels */,
												   ( cl_kernel * ) values   /* kernels */,
												   NULL                     /* num_kernels_ret */ );

				if ( error != CL_SUCCESS ) return error;

				kernels->assign ( &values [0], &values [count] );
				
				return CL_SUCCESS;
			}
	};

	Kernel :: Kernel ( const Program & program,
				     const char * name,
					 cl_int * error )
	{
		kernel = clCreateKernel ( program   /* program */,
					              name      /* kernel_name */,
								  error     /* errcode_ret */ );
	}

	class Event
	{
		private:

			/**
			 * Identifier of a specific OpenCL event.
			 */
			cl_event event;

		public:
			
			/**
			 * Default constructor ( event is not valid at this point ).
			 */
			Event ( void ) { }

			/** 
			 * Returns information about the event.
			 *
			 * @param name specifies the information to query.
			 *
			 * @param param is a pointer to memory where the appropriate result being
			 *        queried is returned. If \a param is NULL, it is ignored.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_VALUE if \a name is not valid.
			 *
			 * @note Using method to determine if a command identified by event has
			 *       finished execution (i.e. CL_EVENT_COMMAND_EXECUTION_STATUS returns
			 *       CL_COMPLETE) is not a synchronization point i.e. there are no
			 *       guarantees that the memory objects being modified by command
			 *       associated with event will be visible to other enqueued commands.
			 */
			template <typename T>
			cl_int GetInfo ( cl_event_info name, T * param ) const;

			/**
			 * Returns profiling information for the command associated with event.
			 *
			 * @param name specifies the profiling data to query.
			 *
			 * @param param is a pointer to memory where the appropriate result being
			 *        queried is returned. If \a param is NULL, it is ignored.
			 *
			 *  \return One of the following values:
			 *  - CL_SUCCESS if the function is executed successfully and the profiling
			 *    information has been recorded
			 *  - CL_PROFILING_INFO_NOT_AVAILABLE if the profiling information is currently
			 *    not available (because the command identified by event has not completed)
			 *  - CL_INVALID_VALUE if \a param_name is not valid.
			 *
			 * @note The unsigned 64-bit values returned can be used to measure the time
			 *       in nano-seconds consumed by OpenCL commands. OpenCL devices are required
			 *       to correctly track time across changes in frequency and p-states. The
			 *       CL_DEVICE_PROFILING_TIMER_RESOLUTION specifies the resolution of the timer
			 *       i.e. the number of nanoseconds elapsed before the timer is incremented.
			 */
			template <typename T>
			cl_int GetProfilingInfo ( cl_profiling_info name, T * param ) const;

			/**
			 * Waits on the host thread for command identified by event to complete.
			 *
			 * @retval CL_SUCCESS if the function was executed successfully.
			 *
			 * @retval CL_INVALID_EVENT if an event is not valid.
			 *
 			 * @note A command is considered complete if its execution status is
			 *       CL_COMPLETE or a negative value. The events specified in \a
			 *       events list act as synchronization points.
			 */
			cl_int Wait ( void ) const;

			/**
			 * Waits on the host thread for commands identified by event objects in
			 * \a events list to complete.
			 *
			 * \param events is a vector of events.
			 *
			 * @retval CL_SUCCESS if the function was executed successfully.
			 *
			 * @retval CL_INVALID_VALUE if size of \a events list is zero.
			 *
			 * @retval CL_INVALID_EVENT if an event in \a events list is not valid.
			 *
			 * @note A command is considered complete if its execution status is
			 *       CL_COMPLETE or a negative value. The events specified in \a
			 *       events list act as synchronization points.
			 */
			static cl_int Wait ( const vector <Event>& events );
	};

	template <typename T>
	cl_int Event :: GetInfo ( cl_event_info name, T * param ) const
	{
		return clGetEventInfo ( event          /* event */,
			                    name           /* param_name */,
								sizeof ( T )   /* param_value_size */,
								param          /* param_value */,
								NULL           /* param_value_size_ret */ );
	}

	template <typename T>
	cl_int Event :: GetProfilingInfo ( cl_profiling_info name, T * param ) const
	{
		return clGetEventProfilingInfo ( event          /* event */,
			                             name           /* param_name */,
										 sizeof ( T )   /* param_value_size */,
										 param          /* param_value */,
										 NULL           /* param_value_size_ret */ );
	}

	cl_int Event :: Wait ( void ) const
	{
		return clWaitForEvents ( 1        /* num_events */,
			                     &event   /* event_list */ );
	}

	cl_int Event :: Wait ( const vector <Event>& events )
	{
		return clWaitForEvents ( ( cl_uint ) events.size ( )        /* num_events */,
			                     ( cl_event * ) &events.front ( )   /* event_list */ );
	}

	class NDRange
	{
		private:
			size_t sizes_[3];
			cl_uint dimensions_;

		public:
			NDRange ( void )
				: dimensions_(0)
			{ }

			//! Create a 1D range
			NDRange( size_t size0 )
				: dimensions_ ( 1 )
			{
				sizes_[0] = size0;
			}

			//! Create a 2D range
			NDRange(size_t size0, size_t size1)
				: dimensions_(2)
			{
				sizes_[0] = size0;
				sizes_[1] = size1;
			}

			//! Create a 3D range
			NDRange(size_t size0, size_t size1, size_t size2)
				: dimensions_(3)
			{
				sizes_[0] = size0;
				sizes_[1] = size1;
				sizes_[2] = size2;
			}

			operator const size_t * ( void ) const { return ( const size_t * ) sizes_; }

			size_t dimensions() const { return dimensions_; }
	};

	/**
	 * Interface for OpenCL command queue.
	 */
	class CommandQueue
	{
		protected:

			/**
			 * OpenCL command queue.
			 */
			cl_command_queue queue;
	
		public:

			/**
			 * Default constructor ( command queue is not valid at this point ).
			 */
			CommandQueue() { }

			/**
			 * Creates a command-queue on a specific device.
			 *
			 * @param context must be a valid OpenCL context.
			 *
			 * @param device must be a device associated with \a context.
			 *
			 * @param properties specifies a list of properties for the command-queue.
			 *        This is a bit-field and is described in table below:
			 *        \li \c CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,
			 *        \li \c CL_QUEUE_PROFILING_ENABLE.
			 *
			 * @param error will return an appropriate error code. If \a error is
			 *        NULL, its default value, no error code is returned.
			 *
			 * @retval CL_SUCCESS if the command-queue is created successfully.
			 *
			 * @retval CL_INVALID_CONTEXT if \a context is not valid.
			 *
			 * @retval CL_INVALID_DEVICE if \a device is not a valid device or is not
			 *         associated with \a context.
			 *
			 * @retval CL_INVALID_VALUE if values specified in \a properties are not
			 *         valid.
			 *
			 * @retval CL_INVALID_QUEUE_PROPERTIES if values specified in \a properties
			 *         are valid but are not supported by the device.
			 *
			 * @retval CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *         required by the runtime.
			 */
			CommandQueue ( const Context & context,
				           const Device & device,
						   cl_command_queue_properties properties = 0,
						   cl_int * error = NULL )
			{
				queue = clCreateCommandQueue ( context      /* context */,
					                           device       /* device */,
											   properties   /* properties */,
											   error        /* errcode_ret */ );
			}

			/**
			 * Conversion operator for casting to OpenCL program object.
			 *
			 * @return The OpenCL program object.
			 */
			operator cl_command_queue ( void ) const
			{
				return queue;
			}

			/**
			 * Conversion operator for casting to OpenCL program object.
			 *
			 * @return Reference to OpenCL program object.
			 */
			operator cl_command_queue& ( void )
			{
				return queue;
			}

			/**
			 * Returns information about a command-queue.
			 *
			 * @param name specifies the information to query.
			 *
			 * @param param is a pointer to memory where the appropriate result
			 *        being queried is returned. If \a param is NULL, it is ignored.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_VALUE if \a name is not one of the supported values.
			 */
			template <typename T>
			cl_int GetInfo ( cl_command_queue_info name, T * param ) const
			{
				return clGetCommandQueueInfo ( queue          /* command_queue */,
					                           name           /* param_name */,
											   sizeof ( T )   /* param_value_size */,
											   param          /* param_value */,
											   NULL           /* param_value_size_ret */ );
			}

			/**
			 * Enables or disables the properties of a command-queue.
			 *
			 * @param properties specifies the new command-queue properties to be
			 *        applied to command-queue.
			 *
			 * @param enable determines whether the values specified by \a properties
			 *        are enabled ( if enable is \c CL_TRUE ) or disabled ( if enable
			 *        is \c  CL_FALSE ) for the command-queue.
			 *
			 * @param previous returns the command-queue properties before they were
			 *        changed. If \a previous is NULL, its default, it is ignored.
			 *
			 * @retval CL_SUCCESS if the command-queue properties are successfully
			 *          updated.
			 *
			 * @retval CL_INVALID_VALUE if the values specified in \a properties are
			 *         not valid.
			 *
			 * @retval CL_INVALID_QUEUE_PROPERTIES if values specified in \a properties
			 *         are not supported by the device.
			 */
			cl_int SetProperty ( cl_command_queue_properties properties,
				                 cl_bool enable,
								 cl_command_queue_properties * previous = NULL ) const
			{
				return clSetCommandQueueProperty ( queue        /* command_queue */,
					                               properties   /* properties */,
												   enable       /* enable */,
												   previous     /* old_properties */ );
			}

			/**
			 * Enqueues a command to read from a buffer object to host memory.
			 *
			 * @param buffer refers to a valid buffer object.
			 *
			 * @param blocking indicates if the read operation is blocking or
			 *        non-blocking. If \a blocking is \c CL_TRUE i.e. the read command
			 *        is blocking, EnqueueReadBuffer does not return until the buffer
			 *        data has been read and copied into memory pointed to by \a
			 *        pointer. If \a blocking is \c CL_FALSE i.e. the read command is
			 *        non-blocking, EnqueueReadBuffer queues a non-blocking read command
			 *        and returns. The contents of the buffer that \a pointer points to
			 *        cannot be used until the read command has completed. The \a event
			 *        argument returns an event object which can be used to query the
			 *        execution status of the read command. When the read command has
			 *        completed, the contents of the buffer that \a pointer points to
			 *        can be used by the application.
			 *
			 * @param offset is the offset in bytes in the buffer object to read from
			 *        or write to.
			 *
			 * @param size is the size in bytes of data being read or written.
			 *
			 * @param pointer is the pointer to buffer in host memory where data is to
			 *        be read into or to be written from.
			 *
			 * @param events specifies events that need to complete before this particular
			 *        command can be executed. If \a events is NULL, its default, then
			 *        this particular command does not wait on any event to complete. The
			 *        events specified in \a events act as synchronization points.
			 *
			 * @param event returns an event object that identifies this particular read
			 *        command and can be used to query or queue a wait for this
			 *        particular command to complete. \a event can be NULL, its default,
			 *        in which case it will not be possible for the application to query
			 *        the status of this command or queue a wait for this command to
			 *        complete.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_CONTEXT if the context associated with command-queue 
			 *         and \a buffer are not the same.
			 *
			 * @retval CL_INVALID_MEM_OBJECT if \a buffer is not a valid buffer object.
			 *
			 * @retval CL_INVALID_VALUE if the region being read or written specified by
			 *         ( offset, size ) is out of bounds or if \a pointer is a NULL.
			 *
			 * @retval CL_INVALID_EVENT_WAIT_LIST if event objects in \a events are not
			 *         valid events.
			 *
			 * @retval CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *         required by the runtime.
			 */
			cl_int EnqueueReadBuffer ( const Buffer & buffer,
				                       cl_bool blocking,
									   size_t offset,
									   size_t size,
									   void * pointer,
									   const vector <Event> * events = NULL,
									   Event * event = NULL ) const
			{
				return clEnqueueReadBuffer (
					queue                                                       /* command_queue */,
					buffer                                                      /* buffer */,
					blocking                                                    /* blocking_read */,
					offset                                                      /* offset */,
					size                                                        /* cb */,
					pointer                                                     /* ptr */,
					events != NULL ? ( cl_uint ) events->size ( ) : 0           /* num_events_in_wait_list */,
					events != NULL ? ( cl_event * ) &events->front ( ) : NULL   /* event_wait_list */,
					( cl_event * ) event                                        /* event */ );
			}

			/**
			 * Enqueue a command to write to a buffer object from host memory.
			 *
			 * @param buffer refers to a valid buffer object.
			 *
			 * @param blocking indicates if the write operation is blocking or non-blocking.
			 *        If \a blocking is \c CL_TRUE, the OpenCL implementation copies
			 *        the data referred to by \a pointer and enqueues the write operation
			 *        in the command-queue. The memory pointed to by \a pointer can be
			 *        reused by the application after the EnqueueWriteBuffer call returns.
			 *        If \a blocking is \c CL_FALSE, the OpenCL implementation will use \a
			 *        pointer to perform a non-blocking write. As the write is non-blocking
			 *        the implementation can return immediately. The memory pointed to by
			 *        \a pointer cannot be reused by the application after the call returns.
			 *        The \a event argument returns an event object which can be used to
			 *        query the execution status of the write command. When the write
			 *        command has completed, the memory pointed to by \a pointer can then be
			 *        reused by the application
			 *
			 * @param offset is the offset in bytes in the buffer object to write to.
			 *
			 * @param size is the size in bytes of data being read or written.
			 *
			 * @param pointer is the pointer to buffer in host memory where data is to be
			 *        read into or to be written from.
			 *
			 * @param events specifies events that need to complete before this particular
			 *        command can be executed. If \a events is NULL, its default, then
			 *        this particular command does not wait on any event to complete. The
			 *        events specified in \a events act as synchronization points.
			 *
			 * @param event returns an event object that identifies this particular write
			 *        command and can be used to query or queue a wait for this
			 *        particular command to complete. \a event can be NULL in which case
			 *        it will not be possible for the application to query the status of
			 *        this command or queue a wait for this command to complete.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_MEM_OBJECT if \a buffer is not a valid buffer object.
			 *
			 * @retval CL_INVALID_VALUE if the region being read or written specified by
			 *         ( offset, size ) is out of bounds or if \a pointer is a NULL.
			 *
			 * @retval CL_INVALID_EVENT_WAIT_LIST if event objects in \a events are not
			 *         valid events.
			 *
			 * @retval CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *         required by the runtime.
			 */
			cl_int EnqueueWriteBuffer ( const Buffer & buffer,
				                        cl_bool blocking,
										size_t offset,
										size_t size,
										void * pointer,
										const vector <Event> * events = NULL,
										Event * event = NULL ) const
			{
				return clEnqueueWriteBuffer (
					queue                                                       /* command_queue */,
					buffer                                                      /* buffer */,
					blocking                                                    /* blocking_write */,
					offset                                                      /* offset */,
					size                                                        /* cb */,
					pointer                                                     /* ptr */,
					events != NULL ? ( cl_uint ) events->size ( ) : 0           /* num_events_in_wait_list */,
					events != NULL ? ( cl_event * ) &events->front ( ) : NULL   /* event_wait_list */,
					( cl_event * ) event                                        /* event */ );
			}

			/**
			 * Enqueues a command to copy a buffer object to another.
			 *
			 * @param source is the source buffer object.
			 *
			 * @param dest is the destination buffer object.
			 *
			 * @param srcoffs refers to the offset where to begin reading data in
			 *        \a source.
			 *
			 * @param dstoffs refers to the offset where to begin copying data in
			 *        \a dest.
			 *
			 * @param size refers to the size in bytes to copy.
			 *
			 * @param events specifies events that need to complete before this
			 *        particular command can be executed. If \a events is NULL,
			 *        then this particular command does not wait on  any event to
			 *        complete. The events specified in \a events act as synchronization
			 *        points.
			 *
			 * @param event returns an event object that identifies this particular
			 *        copy command and can be used to query or queue a wait for this
			 *        particular command to complete. \a event can be NULL in which case it
			 *  will not be possible for the application to query the status of this
			 *  command or queue and wait for this command to complete. enqueueBarrier
			 *  can be used instead.
			 *
			 *  \return CL_SUCCESS if the function is executed successfully. Otherwise
			 *  it returns one of the following errors:
			 *  - CL_INVALID_CONTEXT if the context associated with \a command_queue,
			 *    \a src and \a dst are not the same.
			 *  - CL_INVALID_MEM_OBJECT if \a src_buffer and \a dst_buffer are not valid
			 *    buffer objects.
			 *  - CL_INVALID_VALUE if \a src_offset, \a dst_offset, \a cb,
			 *    \a src_offset + \a size or \a dst_offset + \a size require accessing
			 *    elements outside the buffer memory objects.
			 *  - CL_INVALID_EVENT_WAIT_LIST if event objects in \a events are not
			 *    valid events.
			 *  - CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *    required by the runtime
			 *
			 * \note In the case that exceptions are enabled and error value
			 * other than CL_SUCCESS is generated, then cl::Error exception is
			 * generated.
			 */
			cl_int EnqueueCopyBuffer ( const Buffer & source,
				                       const Buffer & destination,
									   size_t src_offset,
									   size_t dst_offset,
									   size_t size,
									   const vector <Event> * events = NULL,
									   Event * event = NULL ) const
			{
				return clEnqueueCopyBuffer (
					queue                                                       /* command_queue */,
					source                                                      /* src_buffer */,
					destination                                                 /* dst_buffer */,
					src_offset                                                  /* src_offset */,
					dst_offset                                                  /* dst_offset */,
					size                                                        /* cb */,
					events != NULL ? ( cl_uint ) events->size ( ) : 0           /* num_events_in_wait_list */,
					events != NULL ? ( cl_event * ) &events->front ( ) : NULL   /* event_wait_list */,
					( cl_event * ) event                                        /* event */ );
			}

			/**
			 * Enqueues a command to read from a 2D or 3D image object to host memory.
			 *         
			 * @param image refers to a valid 2D or 3D image object.
			 *
			 * @param blocking indicates if the read is blocking or non-blocking. If
			 *  \a blocking is CL_TRUE i.e. the read command is blocking,
			 *  enqueueReadImage does not return until the buffer data has been read and
			 *  copied into memory pointed to by \a ptr. If \a blocking is CL_FALSE
			 *  i.e. the read command is non-blocking, enqueueReadImage queues a
			 *  non-blocking read command and returns. The contents of the buffer that
			 *  \a ptr points to cannot be used until the read command has completed.
			 *  The \a event argument returns an event object which can be used to query
			 *  the execution status of the read command. When the read command has
			 *  completed, the contents of the buffer that ptr points to can be used by
			 *  the application
			 *
			 *  \param origin defines the (x, y, z) offset in the image from where to
			 *  read or write. If image is a 2D image object, the z value given by
			 *  origin[2] must be 0.
			 *
			 *  \param region defines the (width, height, depth) of the 2D or 3D
			 *  rectangle being read or written. If image is a 2D image object, the
			 *  depth value given by region[2] must be 1.
			 *
			 *  \param row_pitch in enqueueReadImage is the length of each row in bytes.
			 *  This value must be greater than or equal to the element size in bytes
			 *  width. If \a row_pitch is set to 0, the appropriate row pitch is
			 *  calculated based on the size of each element in bytes multiplied by
			 *  width.
			 *
			 *  \param slice_pitch in enqueueReadImage is the size in bytes of the 2D
			 *  slice of the 3D region of a 3D image being read or written respectively.
			 *  This must be 0 if image is a 2D image. This value must be greater than
			 *  or equal to row_pitch * height. If \a slice_pitch is set to 0, the
			 *  appropriate slice pitch is calculated based on the \a row_pitch *
			 *  \a height.
			 *
			 *  \param ptr is the pointer to a buffer in host memory where image data is
			 *  to be read from.
			 *
			 *  \param events specifies events that need to complete before
			 *  this particular command can be executed. If \a events is NULL, it
			 *  default then this particular command does not wait on any event to
			 *  complete.The events specified in \a events act as synchronization
			 *  points.
			 *
			 *  \param event returns an event object that identifies this particular
			 *  read command and can be used to query or queue a wait for this
			 *  particular command to complete. \a event can be NULL in which case it
			 *  will not be possible for the application to query the status of this
			 *  command or queue a wait for this command to complete.
			 *
			 *  \return CL_SUCCESS if the function is executed successfully. Otherwise
			 *  it returns one of the following errors:
			 *  - CL_INVALID_CONTEXT if the context associated with \a command_queue and
			 *    \a image are not the same.
			 *  - CL_INVALID_MEM_OBJECT if \a image is not a valid image object.
			 *  - CL_INVALID_VALUE if the region being read specified by \a origin and
			 *    \a region is out of bounds or if \a ptr is a NULL value.
			 *  - CL_INVALID_VALUE if \a image is a 2D image object and \a origin[2]
			 *    is not equal to 0 or \a region[2] is not equal to 1 or \a slice_pitch
			 *    is not equal to 0.
			 *  - CL_INVALID_EVENT_WAIT_LIST if event objects in \a events are not valid
			 *    events.
			 *  - CL_INVALID_VALUE if blocking is CL_FALSE and \a event is NULL.
			 *  - CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *    required by the runtime.
			 *
			 * \note In the case that exceptions are enabled and error value
			 * other than CL_SUCCESS is generated, then cl::Error exception is
			 * generated.
			 */
			cl_int EnqueueReadImage ( const Image & image,
				                      cl_bool blocking,
									  const size_t origin [3],
									  const size_t region [3],
									  size_t row_pitch,
									  size_t slice_pitch,
									  void* ptr,
									  const vector <Event> * events = NULL,
									  Event * event = NULL ) const
			{
				return clEnqueueReadImage (
					queue                                                        /* command_queue */,
					image                                                        /* image */,
					blocking                                                     /* blocking_read */,
					( const size_t * ) origin                                    /* origin[3] */,
					( const size_t * ) region                                    /* region[3] */,
					row_pitch                                                    /* row_pitch */,
					slice_pitch                                                  /* slice_pitch */,
					ptr                                                          /* ptr */,
					events != NULL ? ( cl_uint ) events->size ( ) : 0           /* num_events_in_wait_list */,
					events != NULL ? ( cl_event * ) &events->front ( ) : NULL   /* event_wait_list */,
					( cl_event * ) event                                         /* event */ );
			}

			/*! \brief Enqueue a command to write to a 2D or 3D image object from host
			 *         memory
			 *
			 *  \param image refers to a valid 2D or 3D image object.
			 *
			 *  \param blocking indicates if the write operation is blocking or
			 *  nonblocking. If blocking is CL_TRUE, the OpenCL implementation copies
			 *  the data referred to by \a ptr and enqueues the write command in the
			 *  command-queue. The memory pointed to by ptr can be reused by the
			 *  application after the enqueueWriteImage call returns. If blocking is
			 *  CL_FALSE, the OpenCL implementation will use ptr to perform a
			 *  nonblocking write. As the write is non-blocking the implementation can
			 *  return immediately. The memory pointed to by ptr cannot be reused by the
			 *  application after the call returns. The event argument returns an event
			 *  object which can be used to query the execution status of the write
			 *  command. When the write command has completed, the memory pointed to by
			 *  ptr can then be reused by the application.
			 *
			 *  \param origin defines the (x, y, z) offset in the image from where to
			 *  read or write. If image is a 2D image object, the z value given by
			 *  origin[2] must be 0.
			 *
			 *  \param region defines the (width, height, depth) of the 2D or 3D
			 *  rectangle being read or written. If image is a 2D image object, the
			 *  depth value given by region[2] must be 1.
			 *
			 *  \param input_row_pitch in is the length of each row in bytes.
			 *  This value must be greater than or equal to the element size in bytes
			 *  width. If \a input_row_pitch is set to 0, the appropriate row pitch is
			 *  calculated based on the size of each element in bytes multiplied by
			 *  width.
			 *
			 *  \param input_slice_pitch is the size
			 *  in bytes of the 2D slice of the 3D region of a 3D image being read or
			 *  written respectively. This must be 0 if image is a 2D image. This value
			 *  must be greater than or equal to input_row_pitch * height. If
			 *  \a input_slice_pitch is  set to 0, the appropriate slice pitch is
			 *  calculated based on the  \a input_row_pitch * \a height.
			 *
			 *  \param ptr is the pointer to a buffer in host memory where image data is
			 *  to be written to.
			 *
			 *  \param events specifies events that need to complete before
			 *  this particular command can be executed. If \a events is NULL, it
			 *  default then this particular command does not wait on any event to
			 *  complete.The events specified in \a events act as synchronization
			 *  points.
			 *
			 *  \param event returns an event object that identifies this particular
			 *  read command and can be used to query or queue a wait for this
			 *  particular command to complete. \a event can be NULL in which case it
			 *  will not be possible for the application to query the status of this
			 *  command or queue a wait for this command to complete.
			 *
			 *  \return CL_SUCCESS if the function is executed successfully. Otherwise
			 *  it returns one of the following errors:
			 *  - CL_INVALID_CONTEXT if the context associated with \a command_queue and
			 *    \a image are not the same.
			 *  - CL_INVALID_MEM_OBJECT if \a image is not a valid image object.
			 *  - CL_INVALID_VALUE if the region being written specified by \a origin
			 *    and \a region is out of bounds or if \a ptr is a NULL value.
			 *  - CL_INVALID_VALUE if \a image is a 2D image object and \a origin[2]
			 *    is not equal to 0 or \a region[2] is not equal to 1 or \a slice_pitch
			 *    is not equal to 0.
			 *  - CL_INVALID_EVENT_WAIT_LIST if event objects in \a events are not valid
			 *    events.
			 *  - CL_INVALID_VALUE if blocking is CL_FALSE and \a event is NULL.
			 *  - CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *    required by the runtime.
			 *
			 * \note In the case that exceptions are enabled and error value
			 * other than CL_SUCCESS is generated, then cl::Error exception is
			 * generated.
			 */
			cl_int EnqueueWriteImage( const Image & image,
				                      cl_bool blocking,
									  const size_t origin [3],
									  const size_t region [3],
									  size_t row_pitch,
									  size_t slice_pitch,
									  void * ptr,
									  const vector <Event> * events = NULL,
									  Event * event = NULL) const
			{
				return clEnqueueWriteImage (
					queue                                                       /* command_queue */,
					image                                                       /* image */,
					blocking                                                    /* blocking_write */,
					( const size_t * ) origin                                   /* origin[3] */,
					( const size_t * ) region                                   /* region[3] */,
					row_pitch                                                   /* input_row_pitch */,
					slice_pitch                                                 /* input_slice_pitch */,
					ptr                                                         /* ptr */,
					events != NULL ? ( cl_uint ) events->size ( ) : 0           /* num_events_in_wait_list */,
					events != NULL ? ( cl_event * ) &events->front ( ) : NULL   /* event_wait_list */,
					( cl_event * ) event                                        /* event */ );
			}

			/*! \brief Enqueue a command to copy image objects.
			 *
			 *  \param src is the source image object.
			 *
			 *  \param dst is the destination image object.
			 *
			 *  \param src_origin defines the starting (x, y, z) location in
			 *  \a src from where to start the data copy.  If \a src is a
			 *  2D image object, the z value given by \a src_origin[2] must be 0.
			 *
			 *  \param dst_origin defines the starting (x, y, z) location in \a
			 *  dst from where to start the data copy. If \a dst is a
			 *  2D image object, the z value given by \a dst_origin[2] must be 0.
			 *
			 *  \param region defines the (width, height, depth) of the 2D or 3D
			 *  rectangle to copy. If \a src or \a dst is a 2D image object,
			 *  the depth value given by \a region[2] must be 1.
			 *
			 *  \param events specifies events that need to complete before
			 *  this particular command can be executed. If \a events is NULL, it
			 *  default then this particular command does not wait on any event to
			 *  complete.The events specified in \a events act as synchronization
			 *  points.
			 *
			 *  \param event returns an event object that identifies this particular
			 *  copy command and can be used to query or queue a wait for this
			 *  particular command to complete. \a event can be NULL, its default, in
			 *  which case it will not be possible for the application to query the
			 *  status of this command or queue a wait for this command to complete.
			 *  enqueueBarrier can be used instead. It is currently a requirement that
			 *  the \a src_image and \a dst_image image memory objects for
			 *  enqueueCopyImage must have the exact image format (i.e. channel order
			 *  and channel data type must match).
			 *
			 *  \return CL_SUCCESS if the function is executed successfully. Otherwise
			 *  it returns one of the following errors:
			 *  - CL_INVALID_CONTEXT if the context associated with \a command-queue,
			 *    \a src and \a dst are not the same.
			 *  - CL_INVALID_MEM_OBJECT if \a src and \a dst are not valid
			 *    image objects.
			 *  - CL_IMAGE_FORMAT_MISMATCH if src and dst do not use the
			 *    same image format.
			 *  - CL_INVALID_VALUE if the 2D or 3D rectangular region specified by
			 *    \a src_origin and \a src_origin + \a region refers to a region outside
			 *    \a src, or if the 2D or 3D rectangular region specified by
			 *    \a dst_origin and \a dst_origin + \a region refers to a region outside
			 *    \a dst.
			 *  - CL_INVALID_VALUE if \a src is a 2D image object and \a origin[2]
			 *    is not equal to 0 or \a region[2] is not equal to 1.
			 *  - CL_INVALID_VALUE if \a dst is a 2D image object and \a
			 *    dst_origin[2] is not equal to 0 or \a region[2] is not equal to 1.
			 *  - CL_INVALID_EVENT_WAIT_LIST if event objects in \a events are not valid
			 *    events.
			 *  - CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *    required by the runtime.
			 *
			 * \note In the case that exceptions are enabled and error value
			 * other than CL_SUCCESS is generated, then cl::Error exception is
			 * generated.
			 */
			cl_int EnqueueCopyImage ( const Image & src,
				                      const Image & dst,
									  const size_t src_origin [3],
									  const size_t dst_origin [3],
									  const size_t region [3],
									  const vector <Event> * events = NULL,
									  Event * event = NULL) const
			{
				return clEnqueueCopyImage (
					queue                                                       /* command_queue */,
					src                                                         /* src_image */,
					dst                                                         /* dst_image */,
					( const size_t * ) src_origin                               /* src_origin[3] */,
					( const size_t * ) dst_origin                               /* dst_origin[3] */,
					( const size_t * ) region                                   /* region[3] */,
					events != NULL ? ( cl_uint ) events->size ( ) : 0           /* num_events_in_wait_list */,
					events != NULL ? ( cl_event * ) &events->front ( ) : NULL   /* event_wait_list */,
					( cl_event * ) event                                        /* event */ );
			}

			/*! \brief Enqueue a command to copy an image object to a buffer object.
			 *
			 *  \param src is a valid image object.
			 *
			 *  \param dst is a valid buffer object.
			 *
			 *  \param src_origin defines the (x, y, z) offset in the image from where
			 *  to copy. If \a src is a 2D image object, the z value given by
			 *  \a src_origin[2] must be 0.
			 *
			 *  \param region defines the (width, height, depth) of the 2D or 3D
			 *  rectangle to copy. If \a src_image is a 2D image object, the depth value
			 *  given by \a region[2] must be 1.
			 *
			 *  \param dst refers to the offset where to begin copying data in
			 *  \a dst. The size in bytes of the region to be copied referred to
			 *  as \a dst_cb is computed as width * height * depth * bytes/image element
			 *  if \a src is a 3D image object and is computed as
			 *  width * height * bytes/image element if \a src is a 2D image
			 *  object.
			 *
			 *  \param events specifies events that need to complete before
			 *  this particular command can be executed. If \a events is NULL, it
			 *  default then this particular command does not wait on any event to
			 *  complete.The events specified in \a events act as synchronization
			 *  points.
			 *
			 *  \param event returns an event object that identifies this particular
			 *  copy command and can be used to query or queue a wait for this
			 *  particular command to complete. \a event can be NULL, its default value,
			 *  in which case it will not be possible for the application to query the
			 *  status of this command or queue a wait for this command to complete.
			 *  enqueueBarrier can be used instead.
			 *
			 *  \return CL_SUCCESS if the function is executed successfully. Otherwise
			 *  it returns one of the following errors:
			 *  - CL_INVALID_CONTEXT if the context associated with \a command-queue,
			 *    \a src and \a dst_buffer are not the same.
			 *  - CL_INVALID_MEM_OBJECT if \a src_image is not a valid image object or
			 *    \a dst is not a valid buffer object.
			 *  - CL_INVALID_VALUE if the 2D or 3D rectangular region specified by
			 *    \a src_origin and \a src_origin + \a region refers to a region outside
			 *    \a src, or if the region specified by \a dst_offset and
			 *    \a dst_offset + \a dst_cb to a region outside \a dst.
			 *  - CL_INVALID_VALUE if \a src is a 2D image object and \a
			 *    src_origin[2] is not equal to 0 or \a region[2] is not equal to 1.
			 *  - CL_INVALID_EVENT_WAIT_LIST if event objects in \a events are not
			 *    valid events.
			 *  - CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *    required by the runtime.
			 *
			 * \note In the case that exceptions are enabled and error value
			 * other than CL_SUCCESS is generated, then cl::Error exception is
			 * generated.
			 */
			cl_int EnqueueCopyImageToBuffer ( const Image & src,
				                              const Buffer & dst,
											  const size_t src_origin [3],
											  const size_t region [3],
											  size_t dst_offset,
											  const vector <Event> * events = NULL,
											  Event * event = NULL ) const
			{
				return clEnqueueCopyImageToBuffer (
					queue                                                       /* command_queue */,
					src                                                         /* src_image */,
					dst                                                         /* dst_buffer */,
					( const size_t * ) src_origin                               /* src_origin[3] */,
					( const size_t * ) region                                   /* region[3] */,
					dst_offset                                                  /* dst_offset */,
					events != NULL ? ( cl_uint ) events->size ( ) : 0           /* num_events_in_wait_list */,
					events != NULL ? ( cl_event * ) &events->front ( ) : NULL   /* event_wait_list */,
					( cl_event * ) event                                        /* event */ );
			}

			/*! \brief Enqueue a command to copy a buffer object to an image object.
			 *
			 *  \param src is a valid buffer object.
			 *
			 *  \param dst is a valid image object.
			 *
			 *  \param src_offset refers to the offset where to begin copying data in
			 *  \a src.
			 *
			 *  \param dst_origin defines the (x, y, z) offset in the image from where
			 *  to copy. If \a dst is a 2D image object, the z value given by
			 *  \a dst_origin[2] must be 0.
			 *
			 *  \param region defines the (width, height, depth) of the 2D or 3D
			 *  rectangle to copy. If dst is a 2D image object, the depth value
			 *  given by \a region[2] must be 1. The size in bytes of the region to be
			 *  copied from \a src referred to as \a src_cb is computed as
			 *  width * height * depth * bytes/image element if \a dst is a 3D image
			 *  object and is computed as width * height * bytes/image element if
			 *  \a dst is a 2D image object.
			 *
			 *  \param events specifies events that need to complete before this
			 *  particular command can be executed. If \a events is NULL, then
			 *  this particular command does not wait on any event to complete.
			 *  The events specified in \a events act as synchronization points.
			 *
			 *  \param event returns an event object that identifies this particular
			 *  copy command and can be used to query or queue a wait for this
			 *  particular command to complete. \a event can be NULL, its default value,
			 *  in which case it will not be possible for the application to query the
			 *  status of this command or queue a wait for this command to complete.
			 *  enqueueBarrier can be used instead.
			 *
			 *  \return CL_SUCCESS if the function is executed successfully. Otherwise
			 *  it returns one of the following errors:
			 *  - CL_INVALID_CONTEXT if the context associated with \a command_queue,
			 *    \a src and \a dst are not the same.
			 *  - CL_INVALID_MEM_OBJECT if \a src_buffer is not a valid buffer object or
			 *    \a dst is not a valid image object.
			 *  - CL_INVALID_VALUE if the 2D or 3D rectangular region specified by
			 *    \a dst_origin and \a dst_origin + \a region refers to a region outside
			 *    \a dst, or if the region specified by \a src_offset and
			 *    \a src_offset + \a src_cb to a region outside \a src.
			 *  - CL_INVALID_VALUE if event objects in \a events are not valid events.
			 *  - CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *    required by the runtime
			 *
			 * \note In the case that exceptions are enabled and error value
			 * other than CL_SUCCESS is generated, then cl::Error exception is
			 * generated.
			 */
			cl_int EnqueueCopyBufferToImage ( const Buffer & src,
											  const Image & dst,
											  size_t src_offset,
											  const size_t dst_origin [3],
											  const size_t region [3],
											  const vector <Event> * events = NULL,
											  Event * event = NULL ) const
			{
				return clEnqueueCopyBufferToImage (
					queue                                                       /* command_queue */,
					src                                                         /* src_buffer */,
					dst                                                         /* dst_image */,
					src_offset                                                  /* src_offset */,
					( const size_t * ) dst_origin                               /* dst_origin[3] */,
					( const size_t * ) region                                   /* region[3] */,
					events != NULL ? ( cl_uint ) events->size ( ) : 0           /* num_events_in_wait_list */,
					events != NULL ? ( cl_event * ) &events->front ( ) : NULL   /* event_wait_list */,
					( cl_event * ) event                                        /* event */ );
			}

			/*! \brief Enqueue a command to map a region of a buffer object into the
			 *         host address.
			 *
			 *  \param blocking indicates if the map operation is blocking or
			 *  non-blocking. If \a blocking is CL_TRUE, enqueueMapBuffer does not
			 *  return until the specified region in \a buffer can be mapped. If
			 *  \a blocking is CL_FALSE i.e. map operation is non-blocking, the pointer
			 *  to the mapped region returned by enqueueMapBuffer cannot be used until
			 *  the map command has completed. The event argument returns an event
			 *  object which can be used to query the execution status of the map
			 *  command. When the map command is completed, the application can access
			 *  the contents of the mapped region using the pointer returned by
			 *  enqueueMapBuffer.
			 *
			 *  \param map_flags is a bit-field and can be set to CL_MAP_READ to
			 *  indicate that the region specified by (\a offset, \a size) in the buffer
			 *  object is being mapped for reading, and/or CL_MAP_WRITE to indicate that
			 *  the region specified by (\a offset, \a size) in the buffer object is
			 *  being mapped for writing.
			 *
			 *  \param buffer is a valid buffer object. The OpenCL context associated
			 *  with \a command-queue and \a buffer must be the same.
			 *
			 *  \param offset is the offset in bytes of the region in the buffer object
			 *  that is being mapped
			 *
			 *  \param size is the size in bytes of the region in the buffer object that
			 *  is being mapped.
			 *
			 *  \param events specifies events that need to complete before this
			 *  particular command can be executed. If \a events is NULL, its default
			 *  value, then this particular command does not wait on any event to
			 *  complete. The events specified in \a event_wait_list act as
			 *  synchronization points.
			 *
			 *  \param event returns an event object that identifies this particular
			 *  command and can be used to query or queue a wait for this particular
			 *  command to complete. \a event can be NULL in which case it will not be
			 *  possible for the application to query the status of this command or
			 *  queue a wait for this command to complete. enqueueBarrier can be used
			 *  instead.
			 *
			 *  \param err will return an appropriate error code. If \a err
			 *  is NULL, its default value, no error code is returned.
			 *
			 *  \return A pointer to the mapped region if  buffer  is  a memory object
			 *  created  with  clCreateBuffer  and the region specified by (offset , cb)
			 *  is a valid region in the buffer  object  and is successfully mapped into
			 *  the host address space .  The  \a errcode_ret  is set to CL_SUCCESS.
			 *  A NULL pointer is returned otherwise with one of the following error
			 *  values returned in \a errcode_ret:
			 *  - CL_INVALID_CONTEXT if context associated with \a command-queue and
			 *    \a buffer are not the same.
			 *  - CL_INVALID_MEM_OBJECT if \a buffer is not a valid buffer object.
			 *  - CL_INVALID_VALUE if region being mapped given by (\a offset, \a size)
			 *    is out of bounds or if values specified in \a map_flags are not valid.
			 *  - CL_INVALID_EVENT_WAIT_LIST if event objects in \a events are not
			 *    valid events.
			 *  - CL_MEM_O BJECT_MAP_FAILURE  if there is a failure to map  the
			 *    specified region  in the host address space.
			 *  - CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *    required by the runtime.
			 *
			 *  The pointer returned maps a region starting at \a offset and is at least
			 *  \a size bytes in size. The result of a memory access outside this region
			 *  is undefined.
			 *
			 * \note In the case that exceptions are enabled and error value
			 * other than CL_SUCCESS is generated, then cl::Error exception is
			 * generated.
			 */
			void * EnqueueMapBuffer ( const Buffer & buffer,
				                      cl_bool blocking,
									  cl_map_flags flags,
									  size_t offset,
									  size_t size,
									  const vector <Event> * events = NULL,
									  Event * event = NULL,
									  cl_int * error = NULL ) const
			{
				return clEnqueueMapBuffer (
					queue                                                       /* command_queue */,
					buffer                                                      /* buffer */,
					blocking                                                    /* blocking_map */,
					flags                                                       /* map_flags */,
					offset                                                      /* offset */,
					size                                                        /* cb */,
					events != NULL ? ( cl_uint ) events->size ( ) : 0           /* num_events_in_wait_list */,
					events != NULL ? ( cl_event * ) &events->front ( ) : NULL   /* event_wait_list */,
					( cl_event * ) event                                        /* event */,
					error                                                       /* errcode_ret */ );
			}

			/*! \brief Enqueue a command to map a region in an image object given into
			 *  the host address.
			 *
			 *  \param image is a valid image object. The OpenCL context associated with
			 *  the command-queue and \a image must be the same.
			 *
			 *  \param blocking indicates if the map operation is blocking or
			 *  non-blocking. If \a blocking is CL_TRUE, enqueueMapImage does not
			 *  return until the specified region in image is mapped. If \a blocking is
			 *  CL_FALSE i.e. map operation is non-blocking, the pointer to the mapped
			 *  region returned by enqueueMapImage cannot be used until the map command
			 *  has completed. The event argument returns an event object which can be
			 *  used to query the execution status of the map command. When the map
			 *  command is completed, the application can access the contents of the
			 *  mapped region using the pointer returned by enqueueMapImage.
			 *
			 *  \param flags is a bit-field and can be set to CL_MAP_READ to indicate
			 *  that the region specified by (\a origin, \a region) in the image object
			 *  is being mapped for reading, and/or CL_MAP_WRITE to indicate that the
			 *  region specified by (\a origin, \a region) in the image object is being
			 *  mapped for writing.
			 *
			 *  \param origin define the (x, y, z) offset of the 2D or 3D rectangle
			 *  region that is to be mapped. If image is a 2D image object, the z value
			 *  given by \a origin[2] must be 0.
			 *
			 *  \param region define the (width, height, depth) of the 2D or 3D
			 *  rectangle region that is to be mapped. If image is a 2D image object,
			 *  the depth value given by \a region[2] must be 1.
			 *
			 *  \param row_pitch returns the scan-line pitch in bytes for the mapped
			 *  region. This must be a non- NULL value.
			 *
			 *  \param slice_pitch returns the size in bytes of each 2D slice for the
			 *  mapped region. For a 2D image this argument is ignored. For a 3D image
			 *  this must be a non-NULL value.
			 *
			 *  \param events specifies events that need to complete before
			 *  enqueueMapImage can be executed. If \a events is NULL, then
			 *  enqueueMapImage does not wait on any event to complete. The events
			 *  specified in \a events act as synchronization points.
			 *
			 *  \param event returns an event object that identifies this particular
			 *  command and can be used to query or queue a wait for this particular
			 *  command to complete. \a event can be NULL, its default value, in which
			 *  case it will not be possible for the application to query the status of
			 *  this command or queue a wait for this command to complete.
			 *  enqueueBarrier can be used instead.
			 *
			 *  \param err will return an appropriate error code. If \a err
			 *  is NULL, its default value, o error code is returned.
			 *
			 *  \return A pointer to the mapped region if  image  is  a memory object
			 *  created  with  clCreateImage {2D|3D},  and the 2D or 3D rectangle
			 *  specified by  origin  and  region is a valid region in the image object
			 *  and can be mapped into the host address space.
			 *  The \a err is set to CL_SUCCESS. A NULL pointer is returned
			 *  otherwise with one of the following error values returned in \a err:
			 *  - CL_INVALID_CONTEXT if context associated with \a command_queue and
			 *    \a image are not the same.
			 *  - CL_INVALID_MEM_OBJECT if \a image is not a valid image object.
			 *  - CL_INVALID_VALUE if region being mapped given by
			 *    (\a origin, \a origin + \a region) is out of bounds or if values
			 *    specified in \a map_flags are not valid.
			 *  - CL_INVALID_VALUE if \a image is a 2D image object and \a origin[2]
			 *    is not equal to 0 or \a region[2] is not equal to 1.
			 *  - CL_INVALID_VALUE if \a row_pitch is NULL.
			 *  - CL_INVALID_VALUE if \a image is a 3D image object and \a slice_pitch
			 *    is NULL.
			 *  - CL_INVALID_EVENT_WAIT_LIST if event objects in \a events are not
			 *    valid events.
			 *  - CL_MEM_OBJECT_MAP_FAILURE  if there is a failure to map the  specified
			 *    region in the host address space.
			 *  - CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *    required by the runtime.
			 *
			 * The pointer returned maps a 2D or 3D region starting at origin and is
			 * at least (\a row_pitch * \a region[1] + \a region[0]) pixels in size
			 * for a 2D image, and is at least (\a slice_pitch * \a region[2] +
			 * \a row_pitch * \a region[1] + \a region[0]) pixels in size for a 3D
			 * image. The result of a memory access outside this region is undefined.
			 *
			 * \note In the case that exceptions are enabled and error value
			 * other than CL_SUCCESS is generated, then cl::Error exception is
			 * generated.
			 */
			void * EnqueueMapImage ( const Image & buffer,
				                     cl_bool blocking,
									 cl_map_flags flags,
									 const size_t origin [3],
									 const size_t region [3],
									 size_t * row_pitch,
									 size_t * slice_pitch,
									 const vector <Event> * events = NULL,
									 Event * event = NULL,
									 cl_int * error = NULL ) const
			{
				return clEnqueueMapImage (
					queue                                                       /* command_queue */,
					buffer                                                      /* image */,
					blocking                                                    /* blocking_map */,
					flags                                                       /* map_flags */,
					( const size_t * ) origin                                   /* origin[3] */,
					( const size_t * ) region                                   /* region[3] */,
					row_pitch                                                   /* image_row_pitch */,
					slice_pitch                                                 /* image_slice_pitch */,
					events != NULL ? ( cl_uint ) events->size ( ) : 0           /* num_events_in_wait_list */,
					events != NULL ? ( cl_event * ) &events->front ( ) : NULL   /* event_wait_list */,
					( cl_event * ) event                                        /* event */,
					error                                                       /* errcode_ret */ );
			}

			/*! \brief Enqueue a command to unmap a previously mapped region of a memory
			 *  object.
			 *
			 *  Reads or writes from the host using the pointer returned by
			 *  enqueueMapBuffer or enqueueMapImage are considered to be complete.
			 *
			 *  \param memobj is a valid memory object. The OpenCL context associated
			 *  with the command-queue and \a memobj must be the same.
			 *
			 *  \param mapped_ptr is the host address returned by a previous call to
			 *  enqueueMapBuffer or enqueueMapImage for \a memobj.
			 *
			 *
			 *  \param events specifies events that need to complete before
			 *  enqueueUnmapMemObject can be executed. If \a events is NULL,
			 *  then enqueueUnmapMemObject does not wait on any event to complete. The
			 *  events specified in \a event_wait_list act as synchronization points.
			 *
			 *  \param event returns an event object that identifies this particular
			 *  command and can be used to query or queue a wait for this particular
			 *  command to complete. \a event can be NULL, its default value, in which
			 *  case it will not be possible for the application to query the status
			 *  of this command or queue a wait for this command to complete.
			 *  enqueueBarrier can be used instead.
			 *
			 *  \return One of the following values:
			 *  - CL_SUCCESS if the function is executed successfully.
			 *  - CL_INVALID_MEM_OBJECT if \a memobj is not a valid memory object.
			 *  - CL_INVALID_VALUE if \a mapped_ptr is not a valid pointer returned by
			 *    enqueueMapBuffer or enqueueMapImage for \a memobj.
			 *  - CL_INVALID_EVENT_WAIT_LIST if event objects in \a events are not
			 *    valid events.
			 *  - CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *    required by the runtime.
			 *  - CL_INVALID_CONTEXT if context associated with the command-queue and
			 *    \a memobj are not the same.
			 *
			 * enqueueMapBuffer and enqueueMapImage increments the mapped count of the
			 * memory object. Multiple calls to enqueueMapBuffer or enqueueMapImage on
			 * the same memory object will increment this mapped count by appropriate
			 * number of calls. enqueueUnmapMemObject decrements the mapped count of the
			 * memory object. enqueueMapBuffer and enqueueMapImage act as
			 * synchronization points for a region of the memory object being mapped.
			 *
			 * \note In the case that exceptions are enabled and error value
			 * other than CL_SUCCESS is generated, then cl::Error exception is
			 * generated.
			 */
			cl_int EnqueueUnmapMemObject ( const Memory & memory,
				                           void * mapped_ptr,
										   const vector <Event> * events = NULL,
										   Event * event = NULL ) const
			{
				return clEnqueueUnmapMemObject (
					queue                                                       /* command_queue */,
					memory                                                      /* memobj */,
					mapped_ptr                                                  /* mapped_ptr */,
					events != NULL ? ( cl_uint ) events->size ( ) : 0           /* num_events_in_wait_list */,
					events != NULL ? ( cl_event * ) &events->front ( ) : NULL   /* event_wait_list */,
					( cl_event * ) event                                        /* event */ );
			}

			/*! \brief Enqueue a command to execute a kernel on a device.
			 *
			 *  \param kernel is a valid kernel object. The OpenCL context associated
			 *  with \a kernel and \a command-queue must be the same.
			 *
			 *  \param offset must currently be  a  NullRange value. In  a future
			 *  revision of OpenCL, \a global_work_offset can be used to specify an
			 *  array of \a work_dim unsigned values that describe the offset used to
			 *  calculate the global ID of a work-item instead of having the global IDs
			 *  always start at offset (0, 0, 0).
			 *
			 *  \param global describes  the number of global work-items in will execute
			 *  the  kernel  function. The  total  number  of  global
			 *  work-items is computed as global_work_size[0] * ...
			 *  * global_work_size[work_dim - 1].
			 *
			 *  \param local describes the number of work-items that  make  up  a
			 *  work-group (also referred to as the size of the work-group) that
			 *  will execute the  kernel specified by kernel.
			 *
			 *  \param events specifies events that need to complete  before  this
			 *  particular command can be executed. If \a events  is  NULL, its
			 *  default, or size zero then this particular command does not wait on
			 *  any event to complete. The events specified in \a event_wait_list act as
			 *  synchronization points.
			 *
			 *  \param event returns an event object that identifies this particular
			 *  kernel execution instance. Event objects are unique and can be used to
			 *  identify  a particular kernel execution instance later on.  If \a event
			 *  is  NULL, its default value, no event will be created for this kernel
			 *  execution instance and  therefore it will not be possible for the
			 *  application to query or queue a wait for  this particular kernel
			 *  execution instance.
			 *
			 *  The total number of work-items in a work-group is computed as
			 *  local_work_size[0] * ... * local_work_size[work_dim - 1].
			 *  The total number of work-items in the work-group must be less than or
			 *  equal to the CL_DEVICE_MAX_WORK_GROUP_SIZE. The explicitly specified
			 *  \a local_work_size will be used to determine how to break the global
			 *  work-items specified by global_work_size into appropriate work-group
			 *  instances. If \a local_work_size is specified, the values specified in
			 *  \a global_work_size[0], ...,  global_work_size[work_dim - 1] must be
			 *  evenly divisable by the corresponding values specified in
			 *  \a local_work_size[0],..., local_work_size[work_dim - 1].
			 *  \a local_work_size can also be  a  NULL value in which case the OpenCL
			 *  implementation  will  determine  how  to  be break the global work-items
			 *  into appropriate work-groups.
			 *
			 *  If \a local is NullRange and no work-group size is specified when  the
			 *  kernel is compiled, the OpenCL implementation will determine how to
			 *  break the global work-items specified by \a global into appropriate
			 *  work-group instances. The work-group size to be used for kernel can also
			 *  be specified in the program source using the
			 *  __attribute__((reqd_work_group_size(X, Y, Z))) qualifier. In this case
			 *  the size of work group specified by \a local_work_size must match the
			 *  value specified by the \a reqd_work_group_size attribute qualifier.
			 *
			 *  These work-group instances are executed in parallel across multiple
			 *  compute units or concurrently on the same compute unit. Each  work-item
			 *  is  uniquely identified by a global identifier. The global ID, which
			 *  can be read inside the kernel is computed using the value given by
			 *  \a global_work_size and \a global_work_offset.
			 *
			 *  \return One of the following values:
			 *
			 *  - CL_SUCCESS if the kernel execution was successfully queued.
			 *  - CL_INVALID_PROGRAM_EXECUTABLE if there is no successfully built
			 *    program executable available for device associated with command-queue.
			 *  - CL_INVALID_COMMAND_QUEUE if command-queue is not a valid
			 *    command-queue.
			 *  - CL_INVALID_KERNEL if \a kernel is not a valid kernel object.
			 *  - CL_INVALID_KERNEL_ARGS if the kernel argument values have not been
			 *    specified or are not valid for the device on which kernel will be
			 *    executed.
			 *  - CL_INVALID_WORK_DIMENSION if \a work_dim is not a valid value
			 *    (i.e. a value between 1 and 3).
			 *
			 *  - CL_INVALID_WORK_GROUP_SIZE if \a local is specified and  number
			 *    of workitems specified by \a global is not evenly divisable  by
			 *    size of work-given by \a local_work_size or does not match the
			 *    work-group size specified for kernel using the
			 *    __attribute__((reqd_work_group_size(X, Y, Z))) qualifier in program
			 *    source.
			 *
			 *  - CL_INVALID_GLOBAL_OFFSET if \a offset is not NullRange.
			 *
			 *  - CL_OUT_OF_RESOURCES if there is a failure to queue the execution
			 *    instance of \a kernel on  the  command-queue because of insufficient
			 *    resources needed to execute the kernel. For example, the explicitly
			 *    specified \a local_work_dim in range causes a failure to execute the
			 *    kernel because of insufficient resources such as  registers or local
			 *    memory.  Another example would be the number of read-only image args
			 *    used in kernel exceed the CL_DEVICE_MAX_READ_IMAGE_ARGS value for
			 *    device or the number of write-only image args used in kernel exceed
			 *    the CL_DEVICE_MAX_WRITE_IMAGE_ARGS value for device or the number of
			 *    samplers used in kernel exceed CL_DEVICE_MAX_SAMPLERS for device.
			 *
			 *  - CL_MEM_OBJECT_ALLOCATION_FAILURE if there is a failure to allocate
			 *    memory for image or buffer objects specified as arguments to kernel.
			 *
			 *  - CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *    required by the runtime.
			 *
			 * \note In the case that exceptions are enabled and error value
			 * other than CL_SUCCESS is generated, then cl::Error exception is
			 * generated.
			 */
			cl_int EnqueueNDRangeKernel ( const Kernel & kernel,
				                          const NDRange & offset,
										  const NDRange & global,
										  const NDRange & local,
										  const vector <Event> * events = NULL,
										  Event * event = NULL ) const
			{
				return clEnqueueNDRangeKernel (
					queue                                                           /* command_queue */,
					kernel                                                          /* kernel */,
					( cl_uint ) global.dimensions ( )                               /* work_dim */,
					offset.dimensions ( ) != 0 ? ( const size_t * ) offset : NULL   /* global_work_offset */,
					( const size_t * ) global                                       /* global_work_size */,
					local.dimensions ( ) != 0 ? ( const size_t * ) local : NULL     /* local_work_size */,
					events != NULL ? ( cl_uint ) events->size ( ) : 0               /* num_events_in_wait_list */,
					events != NULL ? ( cl_event * ) &events->front ( ) : NULL       /* event_wait_list */,
					( cl_event * ) event                                            /* event */ );
			}

			/*! \brief Enqueue a command to execute a kernel on a device.
			 *         The kernel is executed using a single work-item.
			 *
			 *  \param kernel is a valid kernel object. The OpenCL context associated
			 *  with \a kernel and \a command-queue must be the same.
			 *
			 *  \param events specifies the list of events that need to complete before
			 *  this particular command can be executed. If \a events is NULL, its
			 *  default value, then this particular command does not wait on any event
			 *  to complete. The events specified in \a events act as
			 *  synchronization points.
			 *
			 *  \param event returns an event object that identifies this particular
			 *  kernel execution instance. Event objects are unique and can be used to
			 *  identify a particular kernel execution instance later on.
			 *  If \a event is NULL, its default value, no event will be created for
			 *  this kernel execution instance and therefore it will not be possible for
			 *  the application to query or queue a wait for this particular kernel
			 *  execution instance.
			 *
			 *  \return One of the following values:
			 *  - CL_SUCCESS if the kernel execution was successfully queued.
			 *  - CL_INVALID_PROGRAM_EXECUTABLE if there is no successfully built
			 *    program executable available for device associated with command-queue.
			 *  - CL_INVALID_KERNEL if \a kernel is not a valid kernel object.
			 *  - CL_INVALID_KERNEL_ARGS if the kernel argument values have not been
			 *    specified or are not valid for the device on which kernel will be
			 *    executed.
			 *  - CL_INVALID_WORK_GROUP_SIZE if a work-group size is specified for
			 *    kernel using the __attribute__((reqd_work_group_size(X, Y, Z)))
			 *    qualifier in program source and is not (1, 1, 1).
			 *  - CL_OUT_OF_RESOURCES if there is a failure to queue the execution
			 *    instance of kernel on the command-queue because of insufficient
			 *    resources needed to execute the kernel. For example, the explicitly
			 *    specified \a local_work_dim in range causes a failure to execute the
			 *    kernel because of insufficient resources such as registers or local
			 *    memory. Another example would be the number of read-only image args
			 *    used in kernel exceed the CL_DEVICE_MAX_READ_IMAGE_ARGS value for
			 *    device or the number of write-only image args used in kernel exceed
			 *    the CL_DEVICE_MAX_WRITE_IMAGE_ARGS value for device or the number of
			 *    samplers used in kernel exceed CL_DEVICE_MAX_SAMPLERS for device.
			 *  - CL_MEM_OBJECT_ALLOCATION_FAILURE if there is a failure to allocate
			 *    memory for image or buffer objects specified as arguments to kernel.
			 *  - CL_INVALID_EVENT_WAIT_LIST if event objects in \a events are not valid
			 *    events.
			 *  - CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *    required by the runtime.
			 *
			 * \note In the case that exceptions are enabled and error value
			 * other than CL_SUCCESS is generated, then cl::Error exception is
			 * generated.
			 */
			cl_int EnqueueTask ( const Kernel & kernel,
				                 const vector <Event> * events = NULL,
								 Event * event = NULL ) const
			{
				return clEnqueueTask (
					queue                                                       /* command_queue */,
					kernel                                                      /* kernel */,
					events != NULL ? ( cl_uint ) events->size ( ) : 0           /* num_events_in_wait_list */,
					events != NULL ? ( cl_event * ) &events->front ( ) : NULL   /* event_wait_list */,
					( cl_event * ) event                                        /* event */ );
			}

			/*! \brief Enqueue a command to execute a native C/C++ function not compiled
			 *         using the OpenCL compiler.
			 *
			 *         A native user function can only be executed on a command-queue
			 *         created on a device that has CL_EXEC_NATIVE_KERNEL capability
			 *         set in CL_DEVICE_EXECUTION_CAPABILITIES.
			 *
			 *  \param userFptr is a pointer to a host-callable user function.
			 *
			 *  \param args a pair containing a a pointer to the args list that
			 *  \a user_func should be called with and the size in bytes of the args
			 *  list that args points to. Size  number of bytes of the data pointed to
			 *  by args will be copied and a pointer to this copied region will be
			 *  passed to \a userFptr. The copy needs to be done because the memory
			 *  objects (cl_mem values) that args may contain need to be modified and
			 *  replaced by appropriate pointers to global memory. When
			 *  clEnqueueNativeKernel returns, the memory region pointed to by
			 *  args can be reused by the application.
			 *
			 *  \param mem_objects is a list of valid memory objects, whose size > 0
			 *  implies that each element is a pointer to appropriate locations
			 *  that args points to where memory object handles (cl_mem values) are
			 *  stored. Before the user function is executed, the memory object handles
			 *  are replaced by pointers to global memory.
			 *
			 *  \param events as described in enqueueNDRangeKernel.
			 *
			 *  \param event returns an event objects that identifies this particular
			 *  kernel execution instance. Event objects are unique and can be used to
			 *  identify a particular kernel execution instance later on. If \a event
			 *  is NULL, its default, no event will be created for this kernel execution
			 *  instance and therefore it will not be possible for the application to
			 *  query or queue a wait for this particular kernel execution instance.
			 *
			 *  \return One of the following values:
			 *  - CL_SUCCESS if the user function execution instance was successfully
			 *    queued
			 *  - CL_INVALID_VALUE if \a user_func is NULL.
			 *  - CL_INVALID_OPERATION if device cannot execute the native kernel.
			 *  - CL_INVALID_MEM_OBJECT if one or more memory objects specified in
			 *    \a mem_list are not valid or are not buffer objects.
			 *  - CL_OUT_OF_RESOURCES if there is a failure to queue the execution
			 *    instance of kernel on the command-queue because of insufficient
			 *    resources needed to execute the kernel.
			 *  - CL_MEM_OBJECT_ALLOCATION_FAILURE if there is a failure to allocate
			 *    memory for buffer objects specified as arguments to \a kernel.
			 *  - CL_INVALID_EVENT_WAIT_LIST if event objects in \a events are not valid
			 *    events.
			 *  - CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *    required by the runtime.
			 *
			 * \note In the case that exceptions are enabled and error value
			 * other than CL_SUCCESS is generated, then cl::Error exception is
			 * generated.
			 */
			cl_int enqueueNativeKernel ( void ( * userFptr ) ( void * ),
				                         std :: pair <void *, size_t> args,
										 const vector <Memory> * mem_objects = NULL,
										 const vector <const void *> * mem_locs = NULL,
										 const vector <Event> * events = NULL,
										 Event * event = NULL ) const
			{
				return clEnqueueNativeKernel (
					queue,
					userFptr,
					args.first,
					args.second,
					mem_objects != NULL ? ( cl_uint ) mem_objects->size ( ) : 0,
					mem_objects != NULL ? ( const cl_mem * ) &mem_objects->front ( ): NULL,
					mem_locs != NULL ? ( const void ** ) &mem_locs->front ( ) : NULL,
					events != NULL ? ( cl_uint ) events->size ( ) : 0,
					events != NULL ? ( cl_event * ) &events->front ( ) : NULL,
					( cl_event * ) event );
			}

			/*! \brief Enqueue a marker command.
			 *
			 *  The marker command returns an event which can be used to queue a
			 *  wait on this marker event i.e. wait for all commands queued before
			 *  the marker command to complete.
			 *
			 *  \return One of the following values:
			 *  - CL_SUCCESS if the function is successfully executed
			 *  - CL_INVALID_VALUE if \a event is a NULL value
			 *  - CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *    required by the runtime.
			 *
			 * \note In the case that exceptions are enabled and error value
			 * other than CL_SUCCESS is generated, then cl::Error exception is
			 * generated.
			 */
			cl_int EnqueueMarker ( Event * event = NULL ) const
			{
				return clEnqueueMarker ( queue, ( cl_event * ) event );
			}

			/*! \brief Enqueue a wait for a specific event or a list of events to
			 *  complete before any future commands queued in the command-queue are
			 *  executed.
			 *
			 *
			 *  \param events is the list of events. Each event in \a events must
			 *  be a valid event object returned by a previous call to:
			 *  - enqueueNDRangeKernel
			 *  - enqueueTask
			 *  - enqueueNativeKernel
			 *  - enqueue{Read|Write|Map}{Buffer|Image}
			 *  - enqueueCopy{Buffer|Image}
			 *  - enqueueCopyBufferToImage
			 *  - enqueueCopyImageToBuffer
			 *  - enqueueMarker.
			 *  The events specified in \a event_list act as synchronization points.
			 *
			 *  \return One of the following values:
			 *  - CL_SUCCESS if the function was successfully executed.
			 *  - CL_INVALID_VALUE if size of \a events is zero
			 *  - CL_INVALID_EVENT if event objects specified in \a events are not valid
			 *    events
			 *  - CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *    required by the runtime.
			 *
			 * \note In the case that exceptions are enabled and error value
			 * other than CL_SUCCESS is generated, then cl::Error exception is
			 * generated.
			 */
			cl_int EnqueueWaitForEvents ( const vector <Event> & events ) const
			{
				return clEnqueueWaitForEvents (
					queue,
					( cl_uint ) events.size ( ),
					( const cl_event * ) &events.front ( ) );
			}

	/*		cl_int EnqueueAcquireGLObjects ( const vector <Memory> * mem_objects = NULL,
				                             const vector <Event> * events = NULL,
											 Event * event = NULL ) const
			{
				 return clEnqueueAcquireGLObjects (
					 queue,
					 mem_objects != NULL ? ( cl_uint ) mem_objects->size ( ) : 0,
					 mem_objects != NULL ? ( const cl_mem * ) &mem_objects->front ( ): NULL,
					 events != NULL ? ( cl_uint ) events->size ( ) : 0,
					 events != NULL ? ( cl_event * ) &events->front ( ) : NULL,
					 ( cl_event * ) event );
			}

			cl_int EnqueueReleaseGLObjects ( const vector <Memory> * mem_objects = NULL,
				                             const vector <Event> * events = NULL,
											 Event * event = NULL ) const
			 {
				 return clEnqueueReleaseGLObjects (
					 object_,
					 mem_objects != NULL ? ( cl_uint ) mem_objects->size ( ) : 0,
					 mem_objects != NULL ? ( const cl_mem * ) &mem_objects->front ( ) : NULL,
					 events != NULL ? ( cl_uint ) events->size ( ) : 0,
					 events != NULL ? ( cl_event * ) &events->front ( ) : NULL,
					 ( cl_event * ) event );
			 }*/

			/*! \brief Enqueue a barrier operation.
			 *
			 *  The enqueueBarrier command ensures that all queued commands in
			 *  command-queue have finished execution before the next batch of commands
			 *  can begin execution. enqueueBarrier is a synchronization point.
			 *
			 *  \return One of the following values:
			 *  - CL_SUCCESS if the function was executed successfully
			 *  - CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *  required by the runtime.
			 *
			 * \note In the case that exceptions are enabled and error value
			 * other than CL_SUCCESS is generated, then cl::Error exception is
			 * generated.
			 *
			 * \note In the case that exceptions are enabled and error value
			 * other than CL_SUCCESS is generated, then cl::Error exception is
			 * generated.
			 */
			cl_int EnqueueBarrier ( void ) const
			{
				return clEnqueueBarrier ( queue );
			}

			/*! \brief Issue all previously queued OpenCL commands in command-queue to
			 *  the device associated with command-queue.
			 *
			 *  flush only guarantees that all queued commands get issued to the
			 *  appropriate device. There is no guarantee that they will be
			 *  complete after flush returns.
			 *
			 *  \return One of the following values:
			 *  - CL_SUCCESS if the function call was executed successfully
			 *  - CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *    required by the runtime.
			 *
			 *  Any blocking commands queued in a command-queue such as
			 *  enqueueRead{Image|Buffer} with \a blocking_read set to CL_TRUE,
			 *s  enqueueWrite{Image|Buffer} with \a blocking_write set to CL_TRUE,
			 *  enqueueMap{Buffer|Image} with \a blocking_map set to CL_TRUE or
			 *  waitForEvents perform an implicit flush of the command-queue.
			 *
			 * \note In the case that exceptions are enabled and error value
			 * other than CL_SUCCESS is generated, then cl::Error exception is
			 * generated.
			 */
			cl_int Flush ( void ) const
			{
				return clFlush ( queue );
			}

			/*! \brief Block until all previously queued OpenCL runtime commands in
			 *  \a command_queue are issued to the associated device and have completed.
			 *
			 *  finish does not return until all queued commands in \a command_queue
			 *  have been processed and completed. finish is also a synchronization
			 *  point.
			 *
			 *  \return One of the following values:
			 *  - CL_SUCCESS if the function call was executed successfully.
			 *  - CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *    required by the runtime.
			 *
			 * \note In the case that exceptions are enabled and error value
			 * other than CL_SUCCESS is generated, then cl::Error exception is
			 * generated.
			 */
			cl_int Finish ( void ) const
			{
				return clFinish ( queue );
			}
	};
}

#endif