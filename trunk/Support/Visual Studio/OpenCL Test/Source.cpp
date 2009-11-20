#include <iostream>

#include <stdio.h>

#include <CL/cl.h>

#include <assert.h>

#include <vector>

#include "Platform.hpp"

using namespace std;

using namespace compute;

void main ( void )
{
	vector <Platform> platforms;

	cl_int error = Platform :: GetPlatforms ( &platforms );

	if ( CL_SUCCESS != error )
	{
		cout << "ERROR! Could not get list of available platforms" << endl; exit ( -1 );
	}
	else
	{
		cout << "Available platforms: " << platforms.size ( ) << endl << endl;
	}
	
	for ( int index = 0; index < platforms.size ( ); index++ )
	{
		cout << "Platform #" << index << endl << endl;

		char * platname;

		error |= platforms [index].GetInfo ( CL_PLATFORM_NAME, &platname );

		cout << "Platform name: " << platname << endl;
		
		char * platprofile;

		error |= platforms [index].GetInfo ( CL_PLATFORM_PROFILE, &platprofile );

		cout << "OpenCL profile: " << platprofile << endl;
		
		char * platvendor;

		error |= platforms [index].GetInfo ( CL_PLATFORM_VENDOR, &platvendor );

		cout << "Platform vendor: " << platvendor << endl;
	}

	//---------------------------------------------------------------------------------------------

	vector <Device> cpus, gpus;

	platforms [0].GetDevices ( CL_DEVICE_TYPE_CPU, &cpus );
	platforms [0].GetDevices ( CL_DEVICE_TYPE_GPU, &gpus );

	//---------------------------------------------------------------------------------------------

	Device cpu = cpus [0];

	Device :: PrintBaseInfo ( cpu );

	cout << endl << "\t\t***************************************" << endl << endl;

	Device gpu = gpus [0];

	Device :: PrintBaseInfo ( gpu );

	getchar ( );
}