/*
 * Author: Denis Bogolepov  ( bogdencmc@inbox.ru )
 */

#pragma once

#ifndef _RENDER_BUFFER_

#define _RENDER_BUFFER_

#include <GLee.h>

namespace Render
{
	class RenderBuffer
	{
		private:

			//----------------------------------- Buffer Handle -----------------------------------

			unsigned Handle;

		public:

			//------------------------------------ Buffer Size ------------------------------------
			
			unsigned Width;
			
			unsigned Height;

			//------------------------------- Buffer Type and Format ------------------------------

			unsigned Attachment;
			
			unsigned InternalFormat;

			//--------------------------------- OpenGL Identifier ---------------------------------
			
			static const unsigned Target = GL_RENDERBUFFER;

			//----------------------------- Constructor and Destructor ----------------------------

			RenderBuffer ( unsigned = 512,
				           unsigned = 512,
						   unsigned = GL_DEPTH_ATTACHMENT,
						   unsigned = GL_DEPTH_COMPONENT16 );

			~RenderBuffer ( void );

			//--------------------------------- Buffer Management ---------------------------------
			
			void Setup ( void );

			void Bind ( void );
			
			void Unbind ( void );

			//------------------------------------ Buffer Info ------------------------------------
			
			int GetHandle ( void ) const { return Handle; }
	};
}

#endif