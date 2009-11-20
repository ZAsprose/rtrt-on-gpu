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

	vector <Device> cpudevices;

	error = platforms [0].GetDevices ( CL_DEVICE_TYPE_CPU, &cpudevices );

	if ( CL_SUCCESS != error )
	{
		cout << "ERROR! Could not get list of available CPU devices" << endl; exit ( -1 );
	}
	else
	{
		cout << "Available CPU devices: " << cpudevices.size ( ) << endl << endl;
	}

	//---------------------------------------------------------------------------------------------

	Device cpu = cpudevices [0];

	Device :: PrintBaseInfo ( cpu );

 	/*   





    

    
    // CL_DEVICE_EXTENSIONS: get device extensions, and if any then parse & log the string onto separate lines
    clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, sizeof(device_string), &device_string, NULL);
    if (device_string != 0) 
    {
        shrLog(iLogMode, 0.0, "\n  CL_DEVICE_EXTENSIONS:");
        std::string stdDevString;
        stdDevString = std::string(device_string);
        size_t szOldPos = 0;
        size_t szSpacePos = stdDevString.find(' ', szOldPos); // extensions string is space delimited
        while (szSpacePos != stdDevString.npos && (szSpacePos - szOldPos) > 0)
        {
            if( strcmp("cl_nv_device_attribute_query", stdDevString.substr(szOldPos, szSpacePos - szOldPos).c_str()) == 0 )
                nv_device_attibute_query = true;

            if (szOldPos > 0)
            {
                shrLog(iLogMode, 0.0, "\t\t");
            }
            shrLog(iLogMode, 0.0, "\t\t\t%s\n", stdDevString.substr(szOldPos, szSpacePos - szOldPos).c_str());
            
            szOldPos = szSpacePos + 1;
            szSpacePos = stdDevString.find(' ', szOldPos); 
        }
    }
    else 
    {
        shrLog(iLogMode, 0.0, "  CL_DEVICE_EXTENSIONS: None\n");
    }

    if(nv_device_attibute_query) 
    {
        cl_uint compute_capability_major, compute_capability_minor;
        clGetDeviceInfo(device, CL_NV_DEVICE_COMPUTE_CAPABILITY_MAJOR, sizeof(cl_uint), &compute_capability_major, NULL);
        clGetDeviceInfo(device, CL_NV_DEVICE_COMPUTE_CAPABILITY_MINOR, sizeof(cl_uint), &compute_capability_minor, NULL);

        shrLog(iLogMode, 0.0, "\n  CL_NV_DEVICE_COMPUTE_CAPABILITY:\t%u.%u\n", compute_capability_major, compute_capability_minor);        

        cl_uint regs_per_block;
        clGetDeviceInfo(device, CL_NV_DEVICE_REGISTERS_PER_BLOCK, sizeof(cl_uint), &regs_per_block, NULL);
        shrLog(iLogMode, 0.0, "  CL_NV_DEVICE_REGISTERS_PER_BLOCK:\t%u\n", regs_per_block);        

        cl_uint warp_size;
        clGetDeviceInfo(device, CL_NV_DEVICE_WARP_SIZE, sizeof(cl_uint), &warp_size, NULL);
        shrLog(iLogMode, 0.0, "  CL_NV_DEVICE_WARP_SIZE:\t\t%u\n", warp_size);        

        cl_bool gpu_overlap;
        clGetDeviceInfo(device, CL_NV_DEVICE_GPU_OVERLAP, sizeof(cl_bool), &gpu_overlap, NULL);
        shrLog(iLogMode, 0.0, "  CL_NV_DEVICE_GPU_OVERLAP:\t\t%s\n", gpu_overlap == CL_TRUE ? "CL_TRUE" : "CL_FALSE");        

        cl_bool exec_timeout;
        clGetDeviceInfo(device, CL_NV_DEVICE_KERNEL_EXEC_TIMEOUT, sizeof(cl_bool), &exec_timeout, NULL);
        shrLog(iLogMode, 0.0, "  CL_NV_DEVICE_KERNEL_EXEC_TIMEOUT:\t%s\n", exec_timeout == CL_TRUE ? "CL_TRUE" : "CL_FALSE");        

        cl_bool integrated_memory;
        clGetDeviceInfo(device, CL_NV_DEVICE_INTEGRATED_MEMORY, sizeof(cl_bool), &integrated_memory, NULL);
        shrLog(iLogMode, 0.0, "  CL_NV_DEVICE_INTEGRATED_MEMORY:\t%s\n", integrated_memory == CL_TRUE ? "CL_TRUE" : "CL_FALSE");        
    }

    // CL_DEVICE_PREFERRED_VECTOR_WIDTH_<type>
    shrLog(iLogMode, 0.0, "  CL_DEVICE_PREFERRED_VECTOR_WIDTH_<t>\t"); 
    cl_uint vec_width [6];
    clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, sizeof(cl_uint), &vec_width[0], NULL);
    clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, sizeof(cl_uint), &vec_width[1], NULL);
    clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, sizeof(cl_uint), &vec_width[2], NULL);
    clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, sizeof(cl_uint), &vec_width[3], NULL);
    clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, sizeof(cl_uint), &vec_width[4], NULL);
    clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, sizeof(cl_uint), &vec_width[5], NULL);
    shrLog(iLogMode, 0.0, "CHAR %u, SHORT %u, INT %u, FLOAT %u, DOUBLE %u\n\n\n", 
           vec_width[0], vec_width[1], vec_width[2], vec_width[3], vec_width[4]); 
   */

	getchar ( );
}