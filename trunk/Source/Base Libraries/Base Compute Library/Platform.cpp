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

#include <iostream>

#include "Platform.hpp"

using namespace std;

namespace compute
{


	cl_int Context :: GetSupportedImageFormats ( cl_mem_flags flags,
		                                         cl_mem_object_type type,
												 vector <ImageFormat> * formats )
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

		formats->assign ( &value [0], &value [count] );
		
		return CL_SUCCESS;
	}
}