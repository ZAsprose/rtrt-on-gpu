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

#ifndef _CL_MEMORY_HPP_
#define	_CL_MEMORY_HPP_

#include "Platform.hpp"

using namespace std;

namespace compute
{
	/**
	 * Interface for OpenCL memory object.
	 */
	class Memory
	{
		protected:

			/**
			 * OpenCL memory object.
			 */
			cl_mem memory;

		public:

			/**
			 * Conversion operator for casting to OpenCL memory object.
			 *
			 * @return The OpenCL memory object.
			 */
			operator cl_mem ( void ) const
			{
				return memory;
			}

			/**
			 * Conversion operator for casting to OpenCL memory object.
			 *
			 * @return Reference to OpenCL memory object.
			 */
			operator cl_mem& ( void )
			{
				return memory;
			}
			
			/**
			 * Returns information that is common to all memory objects ( buffer and
			 * image objects ).
			 *
			 * @param name specifies the information to query. The list of supported
			 *        \a name types is described in the table below:
			 *        \li \c CL_MEM_TYPE,
			 *        \li \c CL_MEM_FLAGS,
			 *        \li \c CL_MEM_SIZE,
			 *        \li \c CL_MEM_HOST_PTR,
			 *        \li \c CL_MEM_MAP_COUNT,
			 *        \li \c CL_MEM_REFERENCE_COUNT,
			 *        \li \c CL_MEM_CONTEXT.
			 *
			 * @param param pointer to memory where the appropriate result being
			 *        queried is returned. If \a param is NULL, it is ignored.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_VALUE if \a name is not valid.
			 */
			template <typename T>
			cl_int GetInfo ( cl_mem_info name, T * param ) const
			{
				return clGetMemObjectInfo ( memory         /* memobj */,
											name           /* param_name */,
											sizeof ( T )   /* param_value_size */,
											param          /* param_value */,
											NULL           /* param_value_size_ret */ );
			}
	};

	/**
	 * Interface for OpenCL buffer object.
	 */
	class Buffer : public Memory
	{
		public:

			/**
			 * Default constructor ( buffer is not valid at this point ).
			 */
			Buffer ( void ) { }

			/**
			 * Creates a buffer object.
			 *
			 * @param context valid OpenCL context used to create the buffer object.
			 *
			 * @param flags is a bit-field that is used to specify allocation and
			 *        usage information such as the memory arena that should be used
			 *        to allocate the buffer object and how it will be used. The
			 *        following table describes the possible values for \a flags:
			 *        \li \c CL_MEM_READ_WRITE,
			 *        \li \c CL_MEM_WRITE_ONLY,
 			 *        \li \c CL_MEM_READ_ONLY,
			 *        \li \c CL_MEM_USE_HOST_PTR.
 			 *        \li \c CL_MEM_ALLOC_HOST_PTR,
			 *        \li \c CL_MEM_COPY_HOST_PTR.
			 *
			 * @param size is the size in bytes of the buffer memory object to be
			 *        allocated.
			 *
			 * @param pointer is the pointer to the buffer data that may already be
			 *        allocated by the application. The size of the buffer that \a
			 *        pointer points to must be greater than \a size bytes. Passing
			 *        in a pointer to an already allocated buffer on the host and
			 *        using it as a buffer object allows applications to share data
			 *        efficiently with kernels and the host.
			 *
			 * @param error will return an appropriate error code. If \a error is
			 *        NULL, no error code is returned.
			 *
			 * @retval CL_SUCCESS if the buffer object is created successfully.
			 *
			 * @retval CL_INVALID_CONTEXT if \a context is not valid.
			 *
			 * @retval CL_INVALID_VALUE if values specified in \a flags are not valid.
			 *
			 * @retval CL_INVALID_BUFFER_SIZE if \a size is zero or is greater than \c
			 *         CL_DEVICE_MAX_MEM_ALLOC_SIZE value for all devices in \a context.
			 *
			 * @retval CL_INVALID_HOST_PTR if \a pointer is NULL and \c CL_MEM_USE_HOST_PTR
			 *         or \c CL_MEM_COPY_HOST_PTR are set in \a flags or if \a pointer is
			 *         not NULL but \c CL_MEM_COPY_HOST_PTR or \c CL_MEM_USE_HOST_PTR are
			 *         not set in flags.
			 *
			 * @retval CL_MEM_OBJECT_ALLOCATION_FAILURE if there is a failure to allocate
			 *         memory for buffer object.
			 *
			 * @retval CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *         required by the runtime.
			 */
			Buffer ( const Context& context,
				     cl_mem_flags flags,
					 size_t size,
					 void * pointer = NULL,
					 cl_int * error = NULL )
			{
				memory = clCreateBuffer ( context   /* context */,
										  flags     /* flags */,
										  size      /* size */,
										  pointer   /* host_ptr */,
										  error     /* errcode_ret */ );
			}
	};

	/**
	 * Base interface for all OpenCL images.
	 */
	class Image : public Memory
	{
		public:

			/**
			 * Returns information specific to an image object.
			 * 
			 * @param name specifies the information to query.
			 *
			 * @param param is a pointer to memory where the appropriate result being
			 *        queried is returned. If \a param is NULL, it is ignored.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_VALUE if \a param is not valid.
			 */
			template <typename T>
			cl_int GetImageInfo ( cl_image_info name, T * param ) const;
	};

	template <typename T>
	cl_int Image :: GetImageInfo ( cl_image_info name, T * param ) const
	{
		return clGetImageInfo ( memory         /* image */,
			                    name           /* param_name */,
								sizeof ( T )   /* param_value_size */,
								param          /* param_value */,
								NULL           /* param_value_size_ret */ );
	}

	/**
	 * Interface for OpenCL 2D images.
	 */
	class Image2D : public Image
	{
		public:

			/**
			 * Default constructor ( image is not valid at this point ).
			 */
			Image2D ( void ) { }

			/**
			 * Creates a 2D image object.
			 *
			 * @param context is a valid OpenCL context on which the image object is
			 *        to be created.
			 *
			 * @param flags is a bit-field that is used to specify allocation and
			 *        usage information such as the memory arena that should be used
			 *        to allocate the buffer object and how it will be used. The
			 *        following table describes the possible values for \a flags:
			 *        \li \c CL_MEM_READ_WRITE,
			 *        \li \c CL_MEM_WRITE_ONLY,
 			 *        \li \c CL_MEM_READ_ONLY,
			 *        \li \c CL_MEM_USE_HOST_PTR.
 			 *        \li \c CL_MEM_ALLOC_HOST_PTR,
			 *        \li \c CL_MEM_COPY_HOST_PTR.
			 *
			 * @param format is a pointer to a structure that describes format
			 *        properties of the image to be allocated.
			 *
			 * @param width is the width of the image in pixels ( must be greater
			 *        than or equal to 1 ).
			 *
			 * @param height is the height of the image in pixels ( must be greater
			 *        than or equal to 1 ).
			 *
 			 * @param line is the scan-line pitch in bytes. This must be 0 if \a
			 *        pointer is NULL and can be either 0 or greater than or equal to
			 *        \a width * size of element in bytes if \a pointer is not NULL.
			 *        If \a pointer is not NULL and \a line is equal to 0, \a line
			 *        is calculated as \a width * size of element in bytes. If \a
			 *        line is not 0, it must be a multiple of the image element size
			 *        in bytes.
			 *
			 * @param pointer is a pointer to the image data that may already be
			 *        allocated by the application. The size of the buffer that \a
			 *        pointer points to must be greater than or equal to \a pitch *
			 *        \a height. The size of each element in bytes must be a power of
			 *        2. The image data specified by \a pointer is stored as a linear
			 *        sequence of adjacent scanlines. Each scanline is stored as a
			 *        linear sequence of image elements.
			 *
			 * @param error will return an appropriate error code. If \a error is NULL,
			 *        no error code is returned.
			 *
			 * @retval CL_SUCCESS if the image object is created successfully.
			 *
			 * @retval CL_INVALID_CONTEXT if \a context is not valid.
 			 *
			 * @retval CL_INVALID_VALUE if values specified in \a flags are not valid.
			 *
			 * @retval CL_INVALID_IMAGE_FORMAT_DESCRIPTOR if values specified in \a
			 *         format are not valid.
 			 *
			 * @retval CL_INVALID_IMAGE_SIZE if \a width or \a height are 0 or if they
			 *         exceed values specified in \c CL_DEVICE_IMAGE2D_MAX_WIDTH or \c
			 *         CL_DEVICE_IMAGE2D_MAX_HEIGHT respectively for all devices in
			 *         \a context or if values specified by \a pitch do not follow rules
			 *         described in the argument description above.
			 *
			 * @retval CL_INVALID_HOST_PTR if \a pointer is NULL and \c CL_MEM_USE_HOST_PTR
			 *         or \c CL_MEM_COPY_HOST_PTR are set in \a flags or if \a pointer
			 *         is not NULL but \c CL_MEM_COPY_HOST_PTR or \c CL_MEM_USE_HOST_PTR
			 *         are not set in \a flags.
			 *
			 * @retval CL_IMAGE_FORMAT_NOT_SUPPORTED if the \a format is not supported.
			 *
			 * @retval CL_MEM_OBJECT_ALLOCATION_FAILURE if there is a failure to allocate
			 *         memory for image object.
			 *
			 * @retval CL_INVALID_OPERATION if there are no devices in \a context that
			 *         support images.
			 *
			 * @retval CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *         required by the OpenCL implementation on the host.
			 */
			Image2D ( const Context& context,
				      cl_mem_flags flags,
					  ImageFormat format,
					  size_t width,
					  size_t height,
					  size_t line,
					  void * pointer = NULL,
					  cl_int * error = NULL )
			{
				memory = clCreateImage2D ( context   /* context */,
										   flags     /* flags */,
										   &format   /* image_format */,
										   width     /* image_width */,
										   height    /* image_height */,
										   line      /* image_row_pitch */,
										   pointer   /* host_ptr */,
										   error     /* errcode_ret */ );
			}
	};

	/**
	 * Interface for OpenCL 3D images.
	 */
	class Image3D : public Image
	{
		public:

			/**
			 * Default constructor ( image is not valid at this point ).
			 */
			Image3D ( void ) { }

			/**
			 * Creates a 3D image object.
			 *
			 * @param context is a valid OpenCL context on which the image object is
			 *        to be created.
			 *
			 * @param flags is a bit-field that is used to specify allocation and
			 *        usage information such as the memory arena that should be used
			 *        to allocate the buffer object and how it will be used. The
			 *        following table describes the possible values for \a flags:
			 *        \li \c CL_MEM_READ_WRITE,
			 *        \li \c CL_MEM_WRITE_ONLY,
 			 *        \li \c CL_MEM_READ_ONLY,
			 *        \li \c CL_MEM_USE_HOST_PTR.
 			 *        \li \c CL_MEM_ALLOC_HOST_PTR,
			 *        \li \c CL_MEM_COPY_HOST_PTR.
			 *
			 * @param format is a pointer to a structure that describes format
			 *        properties of the image to be allocated.
			 *
			 * @param width is the width of the image in pixels ( must be greater
			 *        than or equal to 1 ).
			 *
			 * @param height is the height of the image in pixels ( must be greater
			 *        than or equal to 1 ).
			 *
			 * @param depth is the depth of the image in pixels ( must be greater
			 *        than or equal to 1 ).
			 *
			 * @param line is the scan-line pitch in bytes. This must be 0 if \a
			 *        pointer is NULL and can be either 0 or greater than or equal to
			 *        \a width * size of element in bytes if \a pointer is not NULL.
			 *        If \a pointer is not NULL and \a line is equal to 0, \a line
			 *        is calculated as \a width * size of element in bytes. If \a
			 *        line is not 0, it must be a multiple of the image element size
			 *        in bytes.
			 *
			 * @param slice is the size in bytes of each 2D slice in the 3D image.
			 *        This must be 0 if \a pointer is NULL and can be either 0 or
			 *        greater than or equal to \a line * \a height if \a pointer is
			 *        not NULL. If \a pointer is not NULL and \a slice = 0, \a slice
			 *        is calculated as \a line * \a height. If \a slice is not 0,
			 *        it must be a multiple of the \a line.
			 *
			 * @param pointer is a pointer to the image data that may already be
			 *        allocated by the application. The size of the buffer that \a
			 *        pointer points to must be greater than or equal to \a line * \a
			 *        depth. The size of each element in bytes must be a power of 2.
			 *        The image data specified by \a pointer is stored as a linear
			 *        sequence of adjacent 2D slices. Each 2D slice is a linear
			 *        sequence of adjacent scanlines. Each scanline is a linear
			 *        sequence of image elements.
			 *
			 * @param error will return an appropriate error code. If \a error is NULL,
			 *        no error code is returned.
			 *
			 * @retval CL_SUCCESS if the image object is created successfully.
			 *
			 * @retval CL_INVALID_CONTEXT if \a context is not valid.
 			 *
			 * @retval CL_INVALID_VALUE if values specified in \a flags are not valid.
			 *
			 * @retval CL_INVALID_IMAGE_FORMAT_DESCRIPTOR if values specified in \a
			 *         format are not valid.
 			 *
			 * @retval CL_INVALID_IMAGE_SIZE if \a width, \a height are 0 or if \a
			 *         depth less than or equal to 1 or if they exceed values specified
			 *         in \c CL_DEVICE_IMAGE3D_MAX_WIDTH, \c CL_DEVICE_IMAGE3D_MAX_HEIGHT
			 *         \c or CL_DEVICE_IMAGE3D_MAX_DEPTH respectively for all devices
			 *         in \a context or if values specified by \a line and \a slice do
			 *         not follow rules described in the argument description above.
			 *
			 * @retval CL_INVALID_HOST_PTR if \a pointer is NULL and \c CL_MEM_USE_HOST_PTR
			 *         or \c CL_MEM_COPY_HOST_PTR are set in \a flags or if \a pointer
			 *         is not NULL but \c CL_MEM_COPY_HOST_PTR or \c CL_MEM_USE_HOST_PTR
			 *         are not set in \a flags.
			 *
			 * @retval CL_IMAGE_FORMAT_NOT_SUPPORTED if the \a format is not supported.
			 *
			 * @retval CL_MEM_OBJECT_ALLOCATION_FAILURE if there is a failure to allocate
			 *         memory for image object.
			 *
			 * @retval CL_INVALID_OPERATION if there are no devices in \a context that
			 *         support images.
			 *
			 * @retval CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *         required by the OpenCL implementation on the host.
			 */
			Image3D ( const Context& context,
				      cl_mem_flags flags,
					  ImageFormat format,
					  size_t width,
					  size_t height,
					  size_t depth,
					  size_t line,
					  size_t slice,
					  void * pointer = NULL,
					  cl_int * error = NULL )
			{
				memory = clCreateImage3D ( context   /* context */,
										   flags     /* flags */,
										   &format   /* image_format */,
										   width     /* image_width */,
										   height    /* image_height */,
										   depth     /* image_depth */,
										   line      /* image_row_pitch */,
										   slice     /* image_slice_pitch */, 
										   pointer   /* host_ptr */,
										   error     /* errcode_ret */ );
			}
	};

	/**
	 * Interface for OpenCL sampler object.
	 */
	class Sampler
	{
		protected:

			/**
			 * OpenCL sampler object.
			 */
			cl_sampler sampler;

		public:

			/**
			 * Default constructor ( sampler is not valid at this point ).
			 */
			Sampler ( void ) { }

			/**
			 * Creates a sampler object.
			 *
			 * @param context must be a valid OpenCL context.
			 *
			 * @param normalized determines if the image coordinates specified are
			 *        normalized (if \a normalized is CL_TRUE) or not (if \a normalized
			 *        is CL_FALSE).
			 *
			 * @param addressing specifies how out-of-range image coordinates are
			 *        handled when reading from an image. This can be set to:
			 *        \li \c CL_ADDRESS_REPEAT,
			 *        \li \c CL_ADDRESS_CLAMP_TO_EDGE,
			 *        \li \c CL_ADDRESS_CLAMP,
			 *        \li \c CL_ADDRESS_NONE.
			 *
			 * @param filtering specifies the filter that must be applied when reading
			 *        an image. This can be \c CL_FILTER_NEAREST or \c CL_FILTER_LINEAR.
			 *
			 * @param error will return an appropriate error code. If \a error is NULL,
			 *        no error code is returned.
			 *
			 * @retval CL_SUCCESS if the sampler object is created successfully.
			 *
			 * @retval CL_INVALID_CONTEXT if \a context is not a valid context.
			 *
			 * @retval CL_INVALID_VALUE if \a addressing, \a filtering or \a normalized
			 *         or combination of these argument values are not valid.
			 *
			 * @retval CL_INVALID_OPERATION if images are not supported by any device
			 *         associated with \a context.
			 *
			 * @retval CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources
			 *         required by the OpenCL implementation on the host.
			 */
			Sampler ( cl_context context,
				      cl_bool normalized,
					  cl_addressing_mode addressing,
					  cl_filter_mode filtering,
					  cl_int * error )
			{
				sampler = clCreateSampler ( context      /* context */,
					                        normalized   /* normalized_coords */,
											addressing   /* addressing_mode */,
											filtering    /* filter_mode */,
											error        /* errcode_ret */ );
			}

			/**
			 * Returns information about the sampler object.
			 *
			 * @param name specifies the information to query.
			 *
			 * @param param is a pointer to memory where the appropriate result being
			 *        queried is returned. If \a param is NULL, it is ignored.
			 *
			 * @retval CL_SUCCESS if the function is executed successfully.
			 *
			 * @retval CL_INVALID_VALUE if \a name is not valid.
			 */
			template <typename T>
			cl_int GetInfo ( cl_sampler_info name, T * param ) const
			{
				return clGetSamplerInfo ( sampler        /* sampler */,
					                      name           /* param_name */,
										  sizeof ( T )   /* param_value_size */,
										  param          /* param_value */,
										  NULL           /* param_value_size_ret */ );
			}
	};
}

#endif

