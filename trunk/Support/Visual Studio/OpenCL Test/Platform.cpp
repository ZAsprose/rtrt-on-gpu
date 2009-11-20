#include <iostream>

#include "Platform.hpp"

using namespace std;

namespace compute
{
	Platform :: Platform ( cl_platform_id platform )
	{
		PlatformID = platform;
	}

	Platform :: Platform ( const Platform& platform )
	{
		PlatformID = platform.PlatformID;
	}

	Platform& Platform :: operator = ( const Platform& platform )
	{
        PlatformID = platform.PlatformID;

        return *this;
	}

	cl_int Platform :: GetInfo ( cl_platform_info name, char * param [] ) const
	{
		size_t size = 0;

		cl_int error = clGetPlatformInfo ( PlatformID,
			                               name,
										   0,
										   NULL,
										   &size );

		if ( error != CL_SUCCESS ) return error;

		*param = new char [size];

		return clGetPlatformInfo ( PlatformID,
			                       name,
								   size,
								   *param,
								   NULL );
	}
	
	cl_int Platform :: GetDevices ( cl_device_type type, vector <Device> * devices ) const
	{
		cl_uint count = 0;

		cl_int error = clGetDeviceIDs ( PlatformID,
			                            type,
										0,
										NULL,
										&count );

        if ( error != CL_SUCCESS ) return error;

        cl_device_id * ids = new cl_device_id [count];

        error = clGetDeviceIDs ( PlatformID,
			                     type,
								 count,
								 ids,
								 NULL );

        if ( error != CL_SUCCESS ) return error;

        devices->assign ( &ids [0], &ids [count] );

        return CL_SUCCESS;
	}
	
	cl_int Platform :: GetPlatforms ( vector <Platform> * platforms )
	{
		cl_uint count = 0;

        cl_int error = clGetPlatformIDs ( 0,
			                              NULL,
										  &count );

        if ( error != CL_SUCCESS ) return error;

		cl_platform_id * ids = new cl_platform_id [count];

        error = clGetPlatformIDs ( count,
			                       ids,
								   NULL );

        if ( error != CL_SUCCESS ) return error;

        platforms->assign ( &ids [0], &ids [count] );

        return CL_SUCCESS;
	}
}
