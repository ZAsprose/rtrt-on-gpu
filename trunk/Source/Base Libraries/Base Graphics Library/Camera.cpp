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

#include "Camera.hpp"

namespace graphics
{
    /************************************************************************/
    /*                      CONSTRUCTOR AND DESTRUCTOR                      */
    /************************************************************************/

    Camera :: Camera ( Vector3f position,
                       Vector3f orientation )
    {
        fPosition = position;

        fOrientation = orientation;

        RotateAxes ( );
    }

    /************************************************************************/
    /*                           PRIVATE METHODS                            */
    /************************************************************************/

    void Camera :: RotateAxes ( void )
    {
        Matrix3f rotation;
        
        rotation =
            AngleAxisf ( fOrientation.z ( ), Vector3f :: UnitZ ( ) ) *
            AngleAxisf ( fOrientation.y ( ), Vector3f :: UnitY ( ) ) *
            AngleAxisf ( fOrientation.x ( ), Vector3f :: UnitX ( ) );

        //----------------------------------------------------------------

        fView = rotation * Vector3f :: UnitZ ( );

        fUp = rotation * Vector3f :: UnitY ( );

        fSide = rotation * Vector3f :: UnitX ( );

        //----------------------------------------------------------------

        fView.normalize ( );

        fUp.normalize ( );

        fSide.normalize ( );
    }

    /************************************************************************/
    /*                            PUBLIC METHODS                            */
    /************************************************************************/

    void Camera :: Setup ( void )
    {
        glMatrixMode ( GL_PROJECTION );

        glLoadIdentity ( );

        gluPerspective (
            fFieldOfView,
            fAspect,
            fNearPlane,
            fFarPlane );
        
        //-------------------------------------------------

        glMatrixMode ( GL_MODELVIEW );

        glLoadIdentity ( );

        gluLookAt (
            fPosition.x ( ),
            fPosition.y ( ),
            fPosition.z ( ),
            fPosition.x ( ) + fView.x ( ),
            fPosition.y ( ) + fView.y ( ),
            fPosition.z ( ) + fView.z ( ),
            fUp.x ( ),
            fUp.y ( ),
            fUp.z ( ) );
    }
    
    /************************************************************************/

    Vector3f Camera :: Position ( void ) const
    {
        return fPosition;
    }

    /************************************************************************/

    Vector3f Camera :: Orientation ( void ) const
    {
        return fOrientation;
    }

    /************************************************************************/

    Vector3f Camera :: View ( void ) const
    {
        return fView;
    }

    /************************************************************************/

    Vector3f Camera :: Up ( void ) const
    {
        return fUp;
    }

    /************************************************************************/

    Vector3f Camera :: Side ( void ) const
    {
        return fSide;
    }

    /************************************************************************/
    
    Vector2f Camera :: Scale ( void ) const
    {
        float tangent = tanf ( fFieldOfView / 2.0F );
        
        return Vector2f ( fAspect * tangent, tangent );
    }

    /************************************************************************/

    GLfloat Camera :: NearPlane ( void ) const
    {
        return fNearPlane;
    }

    /************************************************************************/

    GLfloat Camera :: FarPlane ( void ) const
    {
        return fFarPlane;
    }

    /************************************************************************/

    GLfloat Camera :: FieldOfView ( void ) const
    {
        return fFieldOfView;
    }

    /************************************************************************/

    GLuint Camera :: Width ( void ) const
    {
        return fWidth;
    }

    /************************************************************************/

    GLuint Camera :: Height ( void ) const
    {
        return fHeight;
    }

    /************************************************************************/

    GLuint Camera :: Left ( void ) const
    {
        return fLeft;
    }

    /************************************************************************/

    GLuint Camera :: Top ( void ) const
    {
        return fTop;
    }

    /************************************************************************/

    GLfloat Camera :: Aspect ( void ) const
    {
        return fAspect;
    }

    /************************************************************************/

    void Camera :: SetPosition ( Vector3f position )
    {
        fPosition = position;
    }

    /************************************************************************/

    void Camera :: SetOrientation ( Vector3f orientation )
    {
        fOrientation = orientation;

        RotateAxes ( );
    }

    /************************************************************************/

    void Camera :: SetViewport ( GLuint width,
                                 GLuint height,
                                 GLuint left,
                                 GLuint top )
    {
        fWidth = width;

        fHeight = height;

        fLeft = left;

        fTop = top;

        fAspect = width / ( GLfloat ) height;

        //-------------------------------------------------

        glViewport (
            fLeft,
            fTop,
            fWidth,
            fHeight );
    }

    /************************************************************************/

    void Camera :: SetViewFrustum ( GLfloat fieldOfView,
                                    GLfloat nearPlane,
                                    GLfloat farPlane )
    {
        fFieldOfView = fieldOfView;

        fNearPlane = nearPlane;

        fFarPlane = farPlane;
    }

    /************************************************************************/

    void Camera :: SetShaderData ( ShaderProgram * program )
    {
        program->SetUniformVector ( "Camera.Position", fPosition );

        program->SetUniformVector ( "Camera.Side", fSide );

        program->SetUniformVector ( "Camera.Up", fUp );

        program->SetUniformVector ( "Camera.View", fView );

        program->SetUniformVector ( "Camera.Scale", Scale ( ) );
    }
}