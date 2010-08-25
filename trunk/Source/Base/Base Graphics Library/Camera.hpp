/*
   -----------------------------------------------------------------------------
   |                 B A S E   G R A P H I C S   L I B R A R Y                 |
   -----------------------------------------------------------------------------
                              
   Copyright (c) 2009 - 2010 Denis Bogolepov ( denisbogol @ gmail.com )

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

#ifndef _CAMERA_
#define _CAMERA_

#include "ShaderProgram.hpp"

namespace graphics
{
    class Camera
    {
        public:
            
            /************************************************************************/
            /*                      CONSTRUCTOR AND DESTRUCTOR                      */
            /************************************************************************/
            
            Camera ( Vector3f position = Vector3f :: Zero ( ),
                     Vector3f orientation = Vector3f :: Zero ( ) );
            
            /************************************************************************/
            /*                            PUBLIC METHODS                            */
            /************************************************************************/
            
            void Setup ( void );
            
            //------------------------------------------------------------------------
            
            Vector3f Position ( void ) const;
            
            Vector3f Orientation ( void ) const;
            
            //------------------------------------------------------------------------
            
            Vector3f View ( void ) const;
            
            Vector3f Up ( void ) const;
            
            Vector3f Side ( void ) const;

            Vector2f Scale ( void ) const;
            
            //------------------------------------------------------------------------
            
            GLfloat NearPlane ( void ) const;
            
            GLfloat FarPlane ( void ) const;
            
            GLfloat FieldOfView ( void ) const;
            
            //------------------------------------------------------------------------
            
            GLuint Width ( void ) const;
            
            GLuint Height ( void ) const;
            
            GLuint Left ( void ) const;
            
            GLuint Top ( void ) const;
            
            GLfloat Aspect ( void ) const;
            
            //------------------------------------------------------------------------
            
            /* Here position and orientation must be defined in WORLD space */

            void SetPosition ( Vector3f position );
            
            void SetOrientation ( Vector3f orientation );
            
            //------------------------------------------------------------------------
            
            void SetViewport ( GLuint width = 512,
                               GLuint height = 512,
                               GLuint left = 0,
                               GLuint top = 0 );
            
            //------------------------------------------------------------------------
            
            void SetFrustum ( GLfloat fieldOfView = 45.0F,
                                  GLfloat nearPlane = 1E-2F,
                                  GLfloat farPlane = 1E3F );

            //------------------------------------------------------------------------
            
            /*
             * Sets camera data to the specified shader program object.
             *
             * This method sets the following camera attributes:
             * 
             *   Position - camera position in world space,
             *   Side - side vector ( left or right ) of the camera,
             *   Up - up vector of the camera,
             *   View - view vector of the camera,
             *   Scale - size of a screen plane on unit distance from camera.
             *
             * In the shader source the camera object should be described by the
             * following structure ( structure name is not important ):
             *
             * struct SCamera
             * {
             *     vec3 Position;
             *     vec3 Side;
             *     vec3 Up;
             *     vec3 View;
             *     vec2 Scale;
             * };
             *
             * In the shader interface there should be a following declaration:
             *
             * uniform SCamera Camera;
             *
             * You can change implementation of this method to support another
             * shader interface.
             */
            void SetShaderData ( ShaderProgram * program );
    
        private:
            
            /************************************************************************/
            /*                            PRIVATE FIELDS                            */
            /************************************************************************/
            
            Vector3f fPosition;
            
            Vector3f fOrientation;
            
            //------------------------------------------------------------------------
            
            Vector3f fView;
            
            Vector3f fUp;
            
            Vector3f fSide;
            
            //------------------------------------------------------------------------
            
            GLfloat fNearPlane;
            
            GLfloat fFarPlane;
            
            GLfloat fFieldOfView;
            
            //------------------------------------------------------------------------
            
            GLuint fWidth;
            
            GLuint fHeight;
            
            GLuint fLeft;
            
            GLuint fTop;
            
            GLfloat fAspect;
            
            /************************************************************************/
            /*                           PRIVATE METHODS                            */
            /************************************************************************/
            
            void RotateAxes ( void );
    };
}

#endif