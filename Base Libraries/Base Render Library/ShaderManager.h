#pragma once

#ifndef _SHADER_MANAGER_

#define _SHADER_MANAGER_

#include <Vector2D.h>

#include <Vector3D.h>

#include <Vector4D.h>

#include <Matrix2D.h>

#include <Matrix3D.h>

#include <Matrix4D.h>

#include "Texture2D.h"

using namespace Math;

namespace RenderTools
{
	class ShaderManager
	{
		private:

			//---------------------------------- Private Fields -----------------------------------

			int Vertex;

			int Fragment;

			int Program;

			//---------------------------------- Private Methods ----------------------------------

			bool Load ( int, const char **, int );

			bool Compile ( int );

			bool Attach ( int );

		public:

			//----------------------------- Constructor and Destructor ----------------------------

			ShaderManager( void );
			
			~ShaderManager( void );

			//--------------------------------- Shaders Management --------------------------------

			bool LoadVertexShader ( const char * );

			bool LoadFragmentShader ( const char * );

			bool LoadVertexShader ( const char **, int );

			bool LoadFragmentShader ( const char **, int );

			bool BuildProgram ( void );

			void Bind ( void );
			
			void Unbind ( void );

			//------------------------------- Input and Output Data -------------------------------

			int GetUniformLocation ( char * );
			
			int GetAttributeLocation ( char * );
			
			Vector4D GetUniformVector ( char * );
			
			Vector4D GetUniformVector ( int );
			
			Vector4D GetAttributeVector ( char * );
			
			Vector4D GetAttributeVector ( int );
        
			bool SetUniformInteger ( const char *, int );

			bool SetUniformInteger ( int, int );

			bool SetUniformFloat ( const char *, float );

			bool SetUniformFloat ( int, float );

			bool SetUniformVector ( const char *, const Vector2D& );

			bool SetUniformVector ( int, const Vector2D& );

			bool SetUniformVector ( const char *, const Vector3D& );

			bool SetUniformVector ( int, const Vector3D& );

			bool SetUniformVector ( const char *, const Vector4D& );

			bool SetUniformVector ( int, const Vector4D& );

			bool SetUniformMatrix ( const char *, const Matrix2D& );

			bool SetUniformMatrix ( int, const Matrix2D& );

			bool SetUniformMatrix ( const char *, const Matrix3D& );

			bool SetUniformMatrix ( int, const Matrix3D& );

			bool SetUniformMatrix ( const char *, const Matrix4D& );

			bool SetUniformMatrix ( int, const Matrix4D& );
			
			bool SetTexture ( const Texture2D& );

			bool SetAttributeName ( int, const char * );

			bool SetAttributeFloat ( const char *, float );

			bool SetAttributeFloat ( int, float );

			bool SetAttributeVector ( const char *, const Vector2D& );

			bool SetAttributeVector ( int, const Vector2D& );

			bool SetAttributeVector ( const char *, const Vector3D& );

			bool SetAttributeVector ( int, const Vector3D& );

			bool SetAttributeVector ( const char *, const Vector4D& );

			bool SetAttributeVector ( int, const Vector4D& );
	};
}

#endif