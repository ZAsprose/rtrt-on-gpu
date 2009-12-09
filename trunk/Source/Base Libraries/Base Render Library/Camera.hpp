/*
   -----------------------------------------------------------------------------
   |                   B A S E   R E N D E R   L I B R A R Y                   |
   -----------------------------------------------------------------------------

                              Denis Bogolepov ( bogdencmc @ inbox.ru )
   Copyright (c) 2009 - 2010
                              Andrey Morozov ( morozov.andrey.vmk @ gmail.com )

   This library is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free Software
   Foundation, either version 3 of the License, or (at your option) any later
   version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with
   this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CAMERA_HPP_

#define _CAMERA_HPP_

#include "vector.hpp"

#include "nvec.hpp"

#include "ShaderManager.hpp"

using namespace smath;

namespace render
{
	class Camera
	{
		protected:
			
			/**
			 * The position of the eye point.
			 */
			vec4 position;

			/**
			 * The orientation of the camera coordinate system.
			 */
			vec4 orientation;

			//---------------------------------------------------------------------------

			/**
			 * The direction of the side ( left or right ) vector.
			 */
			vec4 side;

			/**
			 * The direction of the up vector.
			 */
			vec4 up;

			/**
			 * The direction of the view vector.
			 */
			vec4 view;

			//---------------------------------------------------------------------------
			
			/**
			 * The field of view angle, in degrees, in the \e Y direction.
			 */
			float fieldOfView;

			/**
			 * The distance from the viewer to the near clipping plane ( positive ).
			 */
			float nearPlane;			
			
			/**
			 * The distance from the viewer to the far clipping plane ( positive ).
			 */
			float farPlane;

			//---------------------------------------------------------------------------
			
			/**
			 * The width of the viewport.
			 */
			unsigned width;

			/**
			 * The the height of the viewport.
			 */
			unsigned height;			
			
			/**
			 * The direction of the side ( left or right ) vector.
			 */
			float aspect;

			//---------------------------------------------------------------------------
			
			/**
			 * Updates axes of the camera coordinate system depending on current
			 * orientation.
			 */
			void update ( void );
	       
		public:
			
			/**
			 * Creates a camera object.
			 *
			 * A camera is abstraction of the viewer in world space and is used for
			 * moving in a 3D scene.
			 *
			 * @param position is the initial position of the camera.
			 *
			 * @param orientation is the initial orientation ( Euler angles ) of the
			 *        camera.
			 */
			Camera ( const vec4 & position, const vec4 & orientation );

			//---------------------------------------------------------------------------

			/**
			 * Moves the camera \e to the given point of \e world space.
			 *
			 * @param position sets new position of the camera.
			 */
			void moveTo ( const vec4 & position );

			/**
			 * Moves the camera \e by the given delta in \e local space.
			 *
			 * @param delta sets movings along axes of the camera coordinate system.
			 *
			 * @note Components of \a delta vector set following movings:
			 *       \li \e X - moving along the \e Side vector,
			 *       \li \e Y - moving along the \e Up vector,
			 *       \li \e Z - moving along the \e View vector.
			 */						
			void moveBy ( const vec4 & delta );
			
			/**
			 * Sets orientation of the camera ( rotation angles around axes ) in \e
			 * world space.
			 *
			 * @param orientation sets new rotation angles of the camera.
			 *
			 * @note Components of \a orientation vector set following angles:
			 *       \li \e X - rotation angle around the \e X axis,
			 *       \li \e Y - rotation angle around the \e Y axis,
			 *       \li \e Z - rotation angle around the \e Z axis,
			 */	
			void rotateTo ( const vec4 & orientation );
			
			/**
			 * Rotates the camera by given angles around axes of the \e world space.
			 *
			 * @param delta sets rotation angles around axes.
			 *
			 * @note Components of \a delta vector set following angles:
			 *       \li \e X - rotation angle around the \e X axis,
			 *       \li \e Y - rotation angle around the \e Y axis,
			 *       \li \e Z - rotation angle around the \e Z axis,
			 */	
			void rotateBy ( const vec4 & delta );
			
			//---------------------------------------------------------------------------

			/**
			 * Saves state of the camera ( position and orientation ) to a file.
			 *
			 * @param path is a path to a text file.
			 *
			 * @return Result ( true or false ) of saving of camera's state.
			 *
			 * @note If the file with the specified name already exists, it will be
			 *       rewritten.
			 */
			bool saveToFile ( const char * path );

			/**
			 * Loads state of the camera ( position and orientation ) from a file.
			 *
			 * @param path is a path to existing file.
			 *
			 * @return Result ( true or false ) of loading of camera's state.
			 */
			bool loadFromFile ( const char * path );

			//---------------------------------------------------------------------------

			/**
			 * Creates a viewing matrix derived from an camera position, a reference
			 * point ( \e Position + \e View ) indicating the center of the scene,
			 * and an \e Up vector.
			 *
			 * The matrix maps the reference point to the negative \e Z axis and the
			 * camera point to the origin. When a typical projection matrix is used,
			 * the center of the scene therefore maps to the center of the viewport.
			 * Similarly, the direction described by the \e Up vector projected onto
			 * the viewing plane is mapped to the positive \e Y axis so that it points
			 * upward in the viewport. The \e Up vector must not be parallel to the
			 * line of sight from the eye point to the reference point.
			 */
			void setup ( void );

			/**
			 * Sets camera data to the specified shader program object.
			 *
			 * @param program is a instance of shader program for setting data.
			 *
			 * @note This method sets the following camera attributes:
			 *       \li \e Position - camera point in \e world space,
			 *       \li \e Side - side vector ( left or right ) of the camera,
			 *       \li \e Up - up vector of the camera,
			 *       \li \e View - view vector of the camera,
			 *       \li \e Size - size of a screen plane on unit distance from camera.
			 *
			 *       In the shader source the camera object should be described by the
			 *       following structure:
			 *       
			 *       \code
			 *       struct SCamera // Structure name is not important 
			 *       {
			 *           vec3 Position;
			 *           vec3 Side;
			 *           vec3 Up;
			 *           vec3 View;
			 *           vec2 Size;
			 *       };
			 *       \endcode
			 *
			 *       In the shader interface there should be a following declaration:
			 *
			 *       \code
			 *       uniform SCamera Camera; // Variable name is used for setting data
			 *       \endcode
			 */
			void setShaderData ( ShaderManager * program );

			//---------------------------------------------------------------------------

			/**
			 * Specifies the width and height of the viewport and ( if needed ) the
			 * lower left corner of the viewport rectangle, in pixels.
			 *
			 * @param width is the width of the viewport.
			 *
			 * @param height is the height of the viewport.
			 *
			 * @param left is the \e X coordinate of the lower left viewport corner.
			 *
			 * @param top is the \e Y coordinate of the lower left viewport corner.
			 *
			 * @note When a OpenGL context is first attached to a window, \a width
			 *       and \a height are set to the dimensions of that surface. The
			 *       initial value of lower left corner is (0, 0).
			 */
			void setViewport ( unsigned width = 512,
				               unsigned height = 512,
							   unsigned left = 0,
							   unsigned top = 0 );

			/**
			 * Specifies a viewing frustum into the world coordinate system and sets
			 * up a perspective projection matrix.
			 *
 			 * @param fieldOfView is the field of view angle, in degrees, in the \e
			 *        Y direction.
			 *
			 * @param nearPlane is the distance from the viewer to the near clipping
			 *        plane ( always positive ).
			 *
			 * @param farPlane is the distance from the viewer to the far clipping
			 *        plane (always positive).
			 *
			 * @note Depth buffer precision is affected by the values specified for \a 
			 *       nearPlane and \a farPlane. The greater the ratio of \a farPlane to
			 *       \a nearPlane is, the less effective the depth buffer will be at
			 *       distinguishing between surfaces that are near each other. If \e r 
			 *       = \a farPlane / \a nearPlane about \e Log2 ( \e r ) bits of depth
			 *       buffer precision are lost. Because \e r approaches infinity as \a
			 *       nearPlane approaches 0, \a nearPlane must never be set to 0.
			 */
			void setFrustum ( float fieldOfView = 45.0F,
				              float nearPlane = 1E-2F,
							  float farPlane = 1E3F );

			//---------------------------------------------------------------------------

			/**
			 * Returns width of the viewport rectangle, in pixels.
			 */
			unsigned getWidth ( void ) const
			{
				return width;
			}

			/**
			 * Returns height of the viewport rectangle, in pixels.
			 */
			unsigned getHeight ( void ) const
			{
				return height;
			}
			
			/**
			 * Returns field of view angle, in the \e Y direction, in degrees.
			 */
			float getFieldOfView ( void ) const
			{
				return fieldOfView;
			}

			/**
			 * Returns the distance from the viewer to the near clipping plane.
			 */
			float getNearPlane ( void ) const
			{
				return nearPlane;
			}

			/**
			 * Returns the distance from the viewer to the far clipping plane.
			 */
			float getFarPlane ( void ) const
			{
				return farPlane;
			}

			/**
			 * Returns the distance from the viewer to the far clipping plane.
			 */
			const vec4 & getPosition ( void ) const
			{
				return position;
			}

			/**
			 * Returns the distance from the viewer to the far clipping plane.
			 */
			const vec4 & getSide ( void ) const
			{
				return side;
			}

			/**
			 * Returns the distance from the viewer to the far clipping plane.
			 */
			const vec4 & getUp ( void ) const
			{
				return up;
			}

			/**
			 * Returns the distance from the viewer to the far clipping plane.
			 */
			const vec4 & getView ( void ) const
			{
				return view;
			}
			
			/**
			 * Returns the size of a screen plane on unit distance from camera.
			 *
			 * @note In a returned vector only first two components are used.
			 */			
			const vec4 & getScreenSize ( void ) const;
	};
}

#endif
