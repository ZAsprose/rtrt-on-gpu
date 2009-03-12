#include "RenderBuffer.h"

namespace RenderTools
{
	//-------------------------------- Constructor and Destructor ---------------------------------

	RenderBuffer :: RenderBuffer ( unsigned int width,
		                           unsigned int height,
								   unsigned int attachment,
								   unsigned int internalFormat )
	{
		Width = width;
		
		Height = height;
		
		Attachment = attachment;
		
		InternalFormat = internalFormat;
		
		glGenRenderbuffers ( 1, &Handle );
	}

	RenderBuffer :: ~RenderBuffer ( void )
	{
		glDeleteRenderbuffers ( 1, &Handle );
	}

	//--------------------------------------- Setup Buffer ----------------------------------------
			
	void RenderBuffer :: Setup ( void )
	{
		glBindRenderbuffer ( Target, Handle );
		
		glRenderbufferStorage ( Target, InternalFormat, Width, Height );	
	}
}
