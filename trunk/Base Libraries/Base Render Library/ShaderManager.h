#pragma once

#include "GLee.h"

namespace Render
{
	enum Components
	{
		Vertex,

		Fragment,

		Both
	};
	
	class ShaderManager
	{
		private:

			//----------------------------------- Public Fields -----------------------------------

			int vertex;

			int fragment;

			int program;

			//---------------------------------- Private Methods ----------------------------------

			bool Load ( int, const char **, int );

			bool Compile ( int );

		public:

			ShaderManager( int = Both );
			
			~ShaderManager( void );


			bool LoadVertexShader ( const char * );

			bool LoadFragmentShader ( const char * );

			bool LoadVertexShader ( const char **, int );

			bool LoadFragmentShader ( const char **, int );

			bool BuildProgram ( void );
	};
}
