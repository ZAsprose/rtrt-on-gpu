#pragma once

#include "Vector2D.h"

#include "Vector3D.h"

#include "Vector4D.h"

#include "Matrix2D.h"

#include "Matrix3D.h"

#include "Matrix4D.h"

using namespace Math;

namespace RenderTools
{
	class ShaderManager
	{
		private:

			//---------------------------------- Private Fields -----------------------------------

			int vertex;

			int fragment;

			int program;

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