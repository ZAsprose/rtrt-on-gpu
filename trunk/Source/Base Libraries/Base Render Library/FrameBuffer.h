/*
 * Author: Denis Bogolepov  ( bogdencmc@inbox.ru )
 */

#pragma once

#ifndef _FRAME_BUFFER_

#define _FRAME_BUFFER_

#include "RenderBuffer.h"

#include "Texture2D.h"

#include <GLee.h>

#include <vector>

using namespace std;

namespace Render
{
	class FrameBuffer
	{
		private:

			//----------------------------------- Buffer Handle -----------------------------------

			unsigned int Handle;

		public:

			//--------------------------------- Buffer Attachments --------------------------------
			
			vector < Texture2D * > ColorBuffers;

			vector < RenderBuffer * > RenderBuffers;

			//--------------------------------- OpenGL Identifier ---------------------------------
			
			static const unsigned int Target = GL_FRAMEBUFFER_EXT;

			//----------------------------- Constructor and Destructor ----------------------------

			FrameBuffer ( void );

			~FrameBuffer ( void );

			//--------------------------------- Buffer Management ---------------------------------
			
			void Setup ( void );
			
			void Bind ( void );
			
			void Unbind ( void );

			void FetchOutcome ( void );

			//------------------------------------ Buffer Info ------------------------------------
			
			int GetHandle ( void ) const { return Handle; }
	};
}

#endif