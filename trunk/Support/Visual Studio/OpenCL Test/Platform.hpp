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

#include <vector>

#include <CL/cl.h>

#include "Device.hpp"

using namespace std;

namespace compute
{
	/**
	 * Interface for OpenCL platform ID.
	 */
	class Platform
	{
		private:

			/**
			 * Identifier of a specific OpenCL platform.
			 */
			cl_platform_id PlatformID;

		public:

			/**
			 * Default constructor ( platform is not valid at this point ).
			 */
			Platform ( void ) { }

			/**
			 * Constructs a new platform from a platform ID.
			 *
			 * @param platform identifier of a specific OpenCL platform.
			 */
			Platform ( cl_platform_id platform );

			/**
			 * Constructs a new platform from a valid platform object.
			 *
			 * @param platform platform object used for creation.
			 */
			Platform ( const Platform& platform );

			/**
			 * Assigns a platform to platform.
			 *
			 * @param platform platform object on right hand side of the assignment.
			 */
			Platform& operator = ( const Platform& platform );

			/**
			 * Gets specific information about the OpenCL platform.
			 *
			 * @param name enum that identifies the platform information being queried.

			 * @param param pointer to memory location where appropriate values for a
			 *        given name will be returned. If value is NULL, it is ignored.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_PLATFORM if platform is not a valid platform.
			 *
			 * @retval CL_INVALID_VALUE if there is not enough memory for information
			 *         being queried.
			 */
			cl_int GetInfo ( cl_platform_info name, char * param [] ) const;

			/**
			 * Gets the list of available devices.
			 *
			 * @param type bitfield that identifies the type of OpenCL device.
			 *        Currently supported values are:    \n
			 *        CL_DEVICE_TYPE_CPU,                \n
			 *        CL_DEVICE_TYPE_GPU,                \n
			 *        CL_DEVICE_TYPE_ACCELERATOR,        \n
			 *        CL_DEVICE_TYPE_DEFAULT             \n
			 *        or a combination of the above.
			 *
			 * @param devices returns a vector of OpenCL devices found.
			 *
			 * @retval CL_INVALID_DEVICE_TYPE if \a type is not a valid value.
			 *
			 * @retval CL_DEVICE_NOT_FOUND if no OpenCL devices that matched \a type
			 *         were found.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @note The application can query specific capabilities of the OpenCL
			 *       device to determine which device to use.
			 */
			cl_int GetDevices ( cl_device_type type, vector <Device> * devices ) const;

			/**
			 * Gets the list of available platforms.
			 *
			 * @param platforms returns a vector of OpenCL platforms found.
			 *
			 * @retval CL_INVALID_VALUE if there is no available OpenCL platforms.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @note The application can query specific capabilities of the OpenCL
			 *       platform to determine which platform to use.
			 */
			static cl_int GetPlatforms ( vector <Platform> * platforms );
	};
}

#endif