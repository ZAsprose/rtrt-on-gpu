#pragma once

#ifndef _RENDER_BUFFER_

#define _RENDER_BUFFER_

#include <GLee.h>

namespace RenderTools
{
	class RenderBuffer
	{
		private:

			//----------------------------------- Buffer Handle -----------------------------------

			unsigned int Handle;

		public:

			//------------------------------------ Buffer Size ------------------------------------
			
			unsigned int Width;
			
			unsigned int Height;

			//------------------------------- Buffer Type and Format ------------------------------

			unsigned int Attachment;
			
			unsigned int InternalFormat;

			//--------------------------------- OpenGL Identifier ---------------------------------
			
			static const unsigned int Target = GL_RENDERBUFFER;

			//----------------------------- Constructor and Destructor ----------------------------

			RenderBuffer ( unsigned int = 512,
				           unsigned int = 512,
						   unsigned int = GL_DEPTH_ATTACHMENT,
						   unsigned int = GL_DEPTH_COMPONENT16 );

			~RenderBuffer ( void );

			//----------------------------------- Setup Buffer ------------------------------------
			
			void Setup ( void );

			//------------------------------------ Texture Info -----------------------------------
			
			unsigned int GetHandle ( void ) const { return Handle; }
	};
}

#endif