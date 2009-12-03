#include <iostream>

#include <stdio.h>

#include <CL/cl.h>

#include <assert.h>

#include <vector>

#include "Platform.hpp"

#include "Memory.hpp"

#include "Compute.hpp"

using namespace std;

using namespace compute;

const char * helloStr = 
"__kernel void add ( __global float * a,               \
	                 __global float * b,               \
	                 __global float * answer )         \
{                                                      \
	int gid = get_global_id ( 0 );                     \
	answer [gid] = a [gid] + b [gid];                  \
}";

cl_int PrintBaseInfo ( const Device &device )
{
	char info [512];

	cl_int error = CL_SUCCESS;

	//----------------------------------------- CL_DEVICE_NAME ------------------------------------------

	error |= device.GetInfo ( CL_DEVICE_NAME, &info );

	printf ( "CL_DEVICE_NAME:\t\t\t\t%s\n", info );

	//---------------------------------------- CL_DEVICE_VENDOR -----------------------------------------

	error |= device.GetInfo ( CL_DEVICE_VENDOR, &info );

	printf ( "CL_DEVICE_VENDOR:\t\t\t%s\n", info );

	//---------------------------------------- CL_DRIVER_VERSION ----------------------------------------

	error |= device.GetInfo ( CL_DRIVER_VERSION, &info );
	
	printf ( "CL_DRIVER_VERSION:\t\t\t%s\n", info );

	//----------------------------------------- CL_DEVICE_TYPE ------------------------------------------
 
	cl_device_type type;

	error |= device.GetInfo ( CL_DEVICE_TYPE, &type );

	if ( type & CL_DEVICE_TYPE_CPU )
		printf ( "CL_DEVICE_TYPE:\t\t\t\t%s\n", "CPU" );

	if ( type & CL_DEVICE_TYPE_GPU )
		printf ( "CL_DEVICE_TYPE:\t\t\t\t%s\n", "GPU" );

	if ( type & CL_DEVICE_TYPE_ACCELERATOR )
		printf ( "CL_DEVICE_TYPE:\t\t\t\t%s\n", "Accelerator" );

	if ( type & CL_DEVICE_TYPE_DEFAULT )
		printf ( "CL_DEVICE_TYPE:\t\t\t\t%s\n", "Default" );

	//----------------------------------- CL_DEVICE_MAX_COMPUTE_UNITS -----------------------------------

	cl_uint max_compute_units;

	error |= device.GetInfo ( CL_DEVICE_MAX_COMPUTE_UNITS, &max_compute_units );

	printf ( "CL_DEVICE_MAX_COMPUTE_UNITS:\t\t%u\n\n", max_compute_units );

	//------------------------------- CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS --------------------------------

	size_t max_work_item_dimensions;
	
	error |= device.GetInfo ( CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, &max_work_item_dimensions );

	printf ( "CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS:\t%u\n", max_work_item_dimensions );

	//---------------------------------- CL_DEVICE_MAX_WORK_ITEM_SIZES ----------------------------------

	size_t max_work_item_sizes [3];

	error |= device.GetInfo ( CL_DEVICE_MAX_WORK_ITEM_SIZES, &max_work_item_sizes );

	printf ( "CL_DEVICE_MAX_WORK_ITEM_SIZES:\t\t%u %u %u\n",
		     max_work_item_sizes [0], max_work_item_sizes [1], max_work_item_sizes [2] );

	//---------------------------------- CL_DEVICE_MAX_WORK_GROUP_SIZE ----------------------------------

	size_t max_work_group_size;

	error |= device.GetInfo ( CL_DEVICE_MAX_WORK_GROUP_SIZE, &max_work_group_size );

	printf ( "CL_DEVICE_MAX_WORK_GROUP_SIZE:\t\t%u\n", max_work_group_size );

	//---------------------------------- CL_DEVICE_MAX_CLOCK_FREQUENCY ----------------------------------

	cl_uint max_clock_frequency;

	error |= device.GetInfo ( CL_DEVICE_MAX_CLOCK_FREQUENCY, &max_clock_frequency );

	printf ( "\nCL_DEVICE_MAX_CLOCK_FREQUENCY:\t\t%u MHz\n", max_clock_frequency );

	//------------------------------------- CL_DEVICE_ADDRESS_BITS --------------------------------------

	cl_uint address_bits;

	error |= device.GetInfo ( CL_DEVICE_ADDRESS_BITS, &address_bits );

	printf ( "CL_DEVICE_ADDRESS_BITS:\t\t\t%u\n", address_bits );

	//------------------------------- CL_DEVICE_ERROR_CORRECTION_SUPPORT --------------------------------

	cl_bool error_correction_support;

	error |= device.GetInfo ( CL_DEVICE_ERROR_CORRECTION_SUPPORT, &error_correction_support );

	printf ( "CL_DEVICE_ERROR_CORRECTION_SUPPORT:\t%s\n",
		     error_correction_support == CL_TRUE ? "Yes" : "No" );

	//---------------------------------- CL_DEVICE_MAX_MEM_ALLOC_SIZE -----------------------------------

	cl_ulong max_mem_alloc_size;

	error |= device.GetInfo ( CL_DEVICE_MAX_MEM_ALLOC_SIZE, &max_mem_alloc_size );

	printf ( "\nCL_DEVICE_MAX_MEM_ALLOC_SIZE:\t\t%u MB\n", max_mem_alloc_size / ( 1024 * 1024 ) );

	//------------------------------------ CL_DEVICE_GLOBAL_MEM_SIZE ------------------------------------

	cl_ulong global_mem_size;

	error |= device.GetInfo ( CL_DEVICE_GLOBAL_MEM_SIZE, &global_mem_size );

	printf ( "CL_DEVICE_GLOBAL_MEM_SIZE:\t\t%u MB\n", global_mem_size / ( 1024 * 1024 ) );

	//------------------------------------ CL_DEVICE_LOCAL_MEM_TYPE -------------------------------------

	cl_device_local_mem_type local_mem_type;

	error |= device.GetInfo ( CL_DEVICE_LOCAL_MEM_TYPE, &local_mem_type );

	printf ( "CL_DEVICE_LOCAL_MEM_TYPE:\t\t%s\n",
		     local_mem_type == CL_LOCAL ? "Local" : "Global" );

	//------------------------------------ CL_DEVICE_LOCAL_MEM_SIZE -------------------------------------

	cl_ulong local_mem_size;

	error |= device.GetInfo ( CL_DEVICE_LOCAL_MEM_SIZE, &local_mem_size );

	printf ( "CL_DEVICE_LOCAL_MEM_SIZE:\t\t%u KB\n", local_mem_size / 1024 );

	//------------------------------- CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE --------------------------------
	
	cl_ulong constant_buffer_size;

	error |= device.GetInfo ( CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, &constant_buffer_size );
	
	printf ( "CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE:\t%u KB\n\n", constant_buffer_size / 1024 );

	//----------------------------------- CL_DEVICE_QUEUE_PROPERTIES ------------------------------------

	cl_command_queue_properties command_queue_properties;

	error |= device.GetInfo ( CL_DEVICE_QUEUE_PROPERTIES, &command_queue_properties );

	if ( command_queue_properties & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE )
		printf ( "CL_DEVICE_QUEUE_PROPERTIES:\t\t%s\n", "CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE" );  

	if ( command_queue_properties & CL_QUEUE_PROFILING_ENABLE )
		printf ( "CL_DEVICE_QUEUE_PROPERTIES:\t\t%s\n", "CL_QUEUE_PROFILING_ENABLE" );

	//------------------------------------- CL_DEVICE_IMAGE_SUPPORT -------------------------------------

	cl_bool image_support;

	error |= device.GetInfo ( CL_DEVICE_IMAGE_SUPPORT, &image_support );

	printf ( "\nCL_DEVICE_IMAGE_SUPPORT:\t\t%s\n", image_support == CL_TRUE ? "Yes" : "No" );

	//---------------------------------- CL_DEVICE_MAX_READ_IMAGE_ARGS ----------------------------------

	cl_uint max_read_image_args;

	error |= device.GetInfo ( CL_DEVICE_MAX_READ_IMAGE_ARGS, &max_read_image_args );

	printf ( "CL_DEVICE_MAX_READ_IMAGE_ARGS:\t\t%u\n", max_read_image_args );

	//--------------------------------- CL_DEVICE_MAX_WRITE_IMAGE_ARGS ----------------------------------

	cl_uint max_write_image_args;

	error |= device.GetInfo ( CL_DEVICE_MAX_WRITE_IMAGE_ARGS, &max_write_image_args );

	printf ( "CL_DEVICE_MAX_WRITE_IMAGE_ARGS:\t\t%u\n", max_write_image_args );

	//-------------------------------------- CL_DEVICE_IMAGE <DIM> --------------------------------------

	size_t image_max_dim [5];

	printf ( "CL_DEVICE_IMAGE <DIM>" );

	error |= device.GetInfo ( CL_DEVICE_IMAGE2D_MAX_WIDTH, &image_max_dim [0] );

	printf ( "\t\t\t2D_MAX_WIDTH\t%u\n", image_max_dim [0] );

	error |= device.GetInfo ( CL_DEVICE_IMAGE2D_MAX_HEIGHT, &image_max_dim [1] );

	printf ( "\t\t\t\t\t2D_MAX_HEIGHT\t%u\n", image_max_dim [1] );

	error |= device.GetInfo ( CL_DEVICE_IMAGE3D_MAX_WIDTH, &image_max_dim [2] );

	printf ( "\t\t\t\t\t3D_MAX_WIDTH\t%u\n", image_max_dim [2] );

	error |= device.GetInfo ( CL_DEVICE_IMAGE3D_MAX_HEIGHT, &image_max_dim [3] );

	printf ( "\t\t\t\t\t3D_MAX_HEIGHT\t%u\n", image_max_dim [3] );

	error |= device.GetInfo ( CL_DEVICE_IMAGE3D_MAX_DEPTH, &image_max_dim [4] );

	printf ( "\t\t\t\t\t3D_MAX_DEPTH\t%u\n", image_max_dim [4] );
	
	//-------------------------------------- CL_DEVICE_EXTENSIONS ---------------------------------------
	
	error |= device.GetInfo ( CL_DEVICE_EXTENSIONS, &info );

	printf ( "\nCL_DEVICE_EXTENSIONS:" );

	if ( 0 != strlen ( info ) )
	{
		int start = 0;

		char extension [100];

		for ( int position = 0; position < strlen ( info ); position++ )
		{
			if ( info [position] == ' ' )
			{
				memset ( extension, 0, sizeof ( extension ) );

				memcpy ( extension, info + start, position - start );

				printf ( "%s%s\n", 0 == start ? "\t\t\t" : "\t\t\t\t\t", extension );

				start = position + 1;
			}
		}
	}

	//-------------------------------- CL_DEVICE_PREFERRED_VECTOR_WIDTH ---------------------------------

	cl_uint vector_width [6];
	
	error |= device.GetInfo ( CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, &vector_width [0] );

	error |= device.GetInfo ( CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, &vector_width [1] );

	error |= device.GetInfo ( CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, &vector_width [2] );

	error |= device.GetInfo ( CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, &vector_width [3] );

	error |= device.GetInfo ( CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, &vector_width [4] );

	error |= device.GetInfo ( CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, &vector_width [5] );

	printf ( "\nCL_DEVICE_PREFERRED_VECTOR_WIDTH:");

	printf ( "\tCHAR\t%u\n", vector_width [0] );

	printf ( "\t\t\t\t\tSHORT\t%u\n", vector_width [1] );

	printf ( "\t\t\t\t\tINT\t%u\n", vector_width [2] );

	printf ( "\t\t\t\t\tLONG\t%u\n", vector_width [3] );

	printf ( "\t\t\t\t\tFLOAT\t%u\n", vector_width [4] );

	printf ( "\t\t\t\t\tDOUBLE\t%u\n", vector_width [5] );

	return error;
}



 void main ( void )
{
	// Problem size
	int n = 32;
	
	// Allocate some memory and a place for the results
	float * a = new float [n];
	float * b = new float [n];
	float * results = new float [n];
	
	// Fill in the values
	for ( int i = 0; i < n; i++ )
	{
		a [i] = i;
		b [i] = n - i;
	}

	//-----------------------------------------------------------------------------------

	cl_int error = CL_SUCCESS;
	
	Context context ( CL_DEVICE_TYPE_GPU, 0, NULL, NULL, &error ); 
		
	vector <Device> devices;
		
	error = context.GetDevices ( &devices );
	
	PrintBaseInfo ( devices [0] );
	
	CommandQueue queue ( context, devices [0], 0, &error );

	Program::Sources source ( 1, make_pair ( helloStr, strlen ( helloStr ) ) );	
	Program program ( context, source );	
	error |= program.Build ( devices );
	assert(error == CL_SUCCESS);

	Buffer a_buff ( context, CL_MEM_READ_ONLY, sizeof ( float ) * n, NULL, NULL );
	queue.EnqueueWriteBuffer ( a_buff, CL_TRUE, 0, sizeof ( float ) * n, a, NULL, NULL );

	Buffer b_buff ( context, CL_MEM_READ_ONLY, sizeof ( float ) * n, NULL, NULL );
	queue.EnqueueWriteBuffer ( b_buff, CL_TRUE, 0, sizeof ( float ) * n, b, NULL, NULL );

	Buffer r_buff ( context, CL_MEM_READ_WRITE, sizeof ( float ) * n, NULL, NULL );

	queue.Finish ( );

	Kernel kernel ( program, "add", &error );

	error |= kernel.SetArgument ( 0, a_buff );
	error |= kernel.SetArgument ( 1, b_buff );
	error |= kernel.SetArgument ( 2, r_buff );
	assert(error == CL_SUCCESS);


	// Run the calculation by enqueuing it and forcing the 
	// command queue to complete the task
	size_t global_work_size = n;
	error |= clEnqueueNDRangeKernel(queue, kernel, 1, NULL, 
								 &global_work_size, NULL, 0, NULL, NULL);
	assert(error == CL_SUCCESS);
	queue.Finish ( );
	
	// Once finished read back the results from the answer 
	// array into the results array
	error |= clEnqueueReadBuffer(queue, r_buff, CL_TRUE, 0, sizeof ( float ) * n, 
							  results, 0, NULL, NULL);
	assert(error == CL_SUCCESS);
	queue.Finish ( );

	//-----------------------------------------------------------------------------------

	// Print out some results. For this example the values of all elements
	// should be the same as the value of n
	for ( int i = 0; i < n; i++ )
		printf ( "\t%f\t+\t%f\t=\t%f\n", a [i], b [i], results [i] );
	
	// Free up memory
	delete [] a;
	delete [] b;
	delete [] results;

	getchar ( );
}