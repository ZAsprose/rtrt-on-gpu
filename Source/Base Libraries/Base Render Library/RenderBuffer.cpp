/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#include "RenderBuffer.h"

namespace Render
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

	//------------------------------------- Buffer Management -------------------------------------
			
	void RenderBuffer :: Setup ( void )
	{
		Bind ( );
		
		glRenderbufferStorage ( Target, InternalFormat, Width, Height );	
	}

	void RenderBuffer :: Bind ( void )
	{
		glBindRenderbuffer ( Target, Handle );
	}
	
	void RenderBuffer :: Unbind ( void )
	{
		glBindRenderbuffer ( Target, 0 );
	}
}