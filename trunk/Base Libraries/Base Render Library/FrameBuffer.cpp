#include "FrameBuffer.h"

namespace RenderTools
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

	//--------------------------------------- Setup Buffer ----------------------------------------
			
	void FrameBuffer :: Setup ( void )
	{
		if ( Handle == 0 )
		{
			return;
		}

		//---------------------------------------------------------------------

		int current = 0;

        glGetIntegerv ( GL_FRAMEBUFFER_BINDING, &current );
		
		if ( current != Handle )
		{
			glBindFramebuffer ( GL_FRAMEBUFFER, Handle );
		}

		//---------------------------------------------------------------------
		
		{
			unsigned int * buffers = new unsigned int [ ColorBuffers.size ( ) ];
            	
			for ( int index = 0; index < ColorBuffers.size ( ); index++ )
			{
				ColorBuffers[index]->Bind ( );
	            	
				glFramebufferTexture2D ( Target,
					                     GL_COLOR_ATTACHMENT0 + index,
										 ColorBuffers[index]->GetTarget ( ),
										 ColorBuffers[index]->GetHandle ( ),
										 0 );

				buffers[index] = GL_COLOR_ATTACHMENT0 + index;
			}

			glDrawBuffers ( ColorBuffers.size ( ), buffers );
		}

		//---------------------------------------------------------------------
		
		{
			for ( int index = 0; index < RenderBuffers.size ( ); index++ )
			{
				glFramebufferRenderbuffer ( Target,
					                        RenderBuffers[index]->Attachment,
											RenderBuffers[index]->Target,
											RenderBuffers[index]->GetHandle ( ) );
			}
		}

		//---------------------------------------------------------------------
		
		if ( current != Handle )
		{
			glBindFramebuffer ( Target, current );
		}
	}
	
	void FrameBuffer :: Bind ( )
	{
		glBindFramebuffer ( Target, Handle );
	}
	
	void FrameBuffer :: Unbind ( )
	{
		glBindFramebuffer ( Target, 0 );
	}
	
	void FrameBuffer :: FetchOutcome ( )
	{
		for ( int index = 0; index < ColorBuffers.size ( ); index++ )
		{
			glReadBuffer ( GL_COLOR_ATTACHMENT0_EXT + index );
				
			glReadPixels ( 0, 0,
				           ColorBuffers[index]->Data->GetWidth ( ),
						   ColorBuffers[index]->Data->GetHeight ( ),
				           ColorBuffers[index]->Data->GetPixelFormat ( ),
						   GL_FLOAT,
						   * ColorBuffers[index]->Data);
		}	
	}
}