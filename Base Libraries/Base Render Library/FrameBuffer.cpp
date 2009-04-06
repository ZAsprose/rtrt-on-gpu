/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "FrameBuffer.h"

namespace Render
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	FrameBuffer :: FrameBuffer ( void )
	{
		glGenFramebuffers ( 1, &Handle );
	}

	FrameBuffer :: ~FrameBuffer ( void )
	{
		glDeleteFramebuffers ( 1, &Handle );
	}

	//------------------------------------- Buffer Management -------------------------------------
			
	void FrameBuffer :: Setup ( void )
	{
		if ( Handle == 0 )
		{
			return;
		}

		//------------------------------------------------------------------------

		int current = 0;

        glGetIntegerv ( GL_FRAMEBUFFER_BINDING, &current );
		
		if ( current != Handle )
		{
			glBindFramebuffer ( GL_FRAMEBUFFER, Handle );
		}

		//------------------------------------------------------------------------
		
		{
			unsigned * buffers = new unsigned [ColorBuffers.size ( )];
            	
			for ( unsigned index = 0; index < ColorBuffers.size ( ); index++ )
			{
				glFramebufferTexture2D ( Target,
					                     GL_COLOR_ATTACHMENT0 + index,
										 ColorBuffers [index]->GetTarget ( ),
										 ColorBuffers [index]->GetHandle ( ),
										 0 );

				buffers[index] = GL_COLOR_ATTACHMENT0 + index;
			}

			glDrawBuffers ( ColorBuffers.size ( ), buffers );

			delete [] buffers;
		}

		//------------------------------------------------------------------------
		
		{
			for ( unsigned index = 0; index < RenderBuffers.size ( ); index++ )
			{
				glFramebufferRenderbuffer ( Target,
					                        RenderBuffers [index]->Attachment,
											RenderBuffers [index]->Target,
											RenderBuffers [index]->GetHandle ( ) );
			}
		}

		//------------------------------------------------------------------------
		
		if ( current != Handle )
		{
			glBindFramebuffer ( Target, current );
		}
	}
	
	void FrameBuffer :: Bind ( void )
	{
		glBindFramebuffer ( Target, Handle );
	}
	
	void FrameBuffer :: Unbind ( void )
	{
		glBindFramebuffer ( Target, 0 );
	}
	
	void FrameBuffer :: FetchOutcome ( void )
	{
		for ( unsigned index = 0; index < ColorBuffers.size ( ); index++ )
		{
			glReadBuffer ( GL_COLOR_ATTACHMENT0 + index );
				
			glReadPixels ( 0,
				           0,
				           ColorBuffers [index]->Data->GetWidth ( ),
						   ColorBuffers [index]->Data->GetHeight ( ),
				           ColorBuffers [index]->Data->GetPixelFormat ( ),
						   ColorBuffers [index]->Data->GetType ( ),
						   * ( ColorBuffers [index]->Data ) );
		}	
	}
}