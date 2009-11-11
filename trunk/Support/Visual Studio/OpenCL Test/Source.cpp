#include <iostream>

#include <stdio.h>

#include <CL/cl.h>

#include <assert.h>

using namespace std;

bool InitOpenCL ( void );

void main ( void )
{
	InitOpenCL ( );

	getchar ( );
}

bool InitOpenCL ( void )
{
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "+++                           VERTEX SHADER                          +++" << endl;
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

	cl_uint count;

	cl_int error = CL_SUCCESS;

	error |= clGetPlatformIDs ( 0, NULL, &count );

	cl_platform_id * platforms = new cl_platform_id [count];

	error |= clGetPlatformIDs ( count, platforms, NULL );

	if ( CL_SUCCESS != error )
	{
		cout << "ERROR! Could not get list of available platforms" << endl;

		return false;
	}
	else
	{
		cout << "Available platforms: " << count << endl << endl;
	}

	for ( int index = 0; index < count; index++ )
	{
		cout << "Platform Info #" << index << endl << endl;

		char platname [100];
		
		error |= clGetPlatformInfo ( platforms [index],
			                         CL_PLATFORM_NAME,
									 sizeof ( platname ),
									 ( void * ) platname,
									 NULL );

		cout << "Platform name: " << platname << endl;
		
		char platprofile [100];
		
		error |= clGetPlatformInfo ( platforms [index],
							         CL_PLATFORM_PROFILE,
							         sizeof ( platprofile ),
							         ( void * ) platprofile,
							         NULL );

		cout << "OpenCL profile: " << platprofile << endl;
		
		char platvendor [100];
		
		error |= clGetPlatformInfo ( platforms [index],
							         CL_PLATFORM_VENDOR,
							         sizeof ( platvendor ),
							         ( void * ) platvendor,
							         NULL );

		cout << "Platform vendor: " << platvendor << endl;
		
		cl_uint numdevs;
		
		error |= clGetDeviceIDs ( platforms [index],
						          CL_DEVICE_TYPE_ALL,
						          0,
						          NULL,
						          &numdevs );

		cout << "Number of devices: " << numdevs << endl;
	}

	delete [] platforms;
}