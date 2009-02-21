#pragma once

namespace Render
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

			//------------------------------------ Input Data -------------------------------------
	};
}
