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

#ifndef _CL_DEVICE_HPP_
#define	_CL_DEVICE_HPP_

#include <CL/cl.h>

namespace compute
{
	/**
	 * Interface for OpenCL device ID.
	 */
	class Device
	{
		private:

			/**
			 * Identifier of a specific OpenCL device.
			 */
			cl_device_id DeviceID;

		public:

			/**
			 * Default constructor ( device is not valid at this point ).
			 */
			Device ( void ) { }

			/**
			 * Constructs a new device from a device ID.
			 *
			 * @param device identifier of a specific OpenCL device.
			 */
			Device ( cl_device_id device );

			/**
			 * Constructs a new device from a valid device object.
			 *
			 * @param device device object used for creation.
			 */
			Device ( const Device& device );

			/**
			 * Assigns a device to device.
			 *
			 * @param device device object on right hand side of the assignment.
			 */
			Device& operator = ( const Device &device );

			/**
			 * Gets specific information about an OpenCL device.
			 *
			 * @param name enumeration constant that identifies the device
			 *        information being queried.
			 *
			 * @param param pointer to memory location where appropriate values
			 *        for a given param name will be returned. If value is NULL,
			 *        it is ignored.
			 *
			 * @retval CL_INVALID_DEVICE if device is not valid.
			 *
			 * @retval CL_INVALID_VALUE if name is not one of the supported values.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 */
			template <typename T>
			cl_int GetInfo ( cl_device_info name, T * param ) const;

			/**
			 * Prints base information about an OpenCL device.
			 *
			 * @param device device object about which it is necessary to receive
			 *        the base information.
			 *
			 * @retval CL_INVALID_DEVICE if device is not valid.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 */
			static cl_int PrintBaseInfo ( const Device &device );
	};

	template <typename T>
	cl_int Device :: GetInfo ( cl_device_info name, T * param ) const
	{
		return clGetDeviceInfo ( DeviceID		/* device */,
			                     name			/* param_name */,
								 sizeof ( T )	/* param_value_size */,
								 param			/* param_value */,
								 NULL			/* param_value_size_ret */ );
	}
}

#endif