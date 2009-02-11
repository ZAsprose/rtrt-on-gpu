/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */
 
using System;
using System.Collections.Generic;
using Tao.OpenGl;

namespace Base.Graphics
{
    public class FrameBuffer
	{		
		#region Public Fields
		
		public readonly int Handle = 0;
		
		public List<Texture2D> ColorBuffers = new List<Texture2D>();

		public List<RenderBuffer> RenderBuffers = new List<RenderBuffer>();
		
		public readonly static int Target = Gl.GL_FRAMEBUFFER_EXT;
		
		#endregion
		
		#region Construtor
		
		public FrameBuffer()
		{
			unsafe
			{
				fixed (int* pointer = &Handle)
				{
					Gl.glGenFramebuffersEXT(1, new IntPtr(pointer));
				}
			}
		}
				
		#endregion
		
		#region Public Methods
		
		public void Setup()
		{
			if (Handle == 0)
			{
				return;
			}

            int current = 0;

            unsafe
            {
            	Gl.glGetIntegerv(Gl.GL_FRAMEBUFFER_BINDING_EXT, new IntPtr(&current));
            }

            if (current != Handle)
            {
                Gl.glBindFramebufferEXT(Gl.GL_FRAMEBUFFER_EXT, Handle);
            }
            
            {
            	int[] buffers = new int[ColorBuffers.Count];
            	
	            for (int index = 0; index < ColorBuffers.Count; index++)
	            {
	            	Texture2D texture = ColorBuffers[index];
	            	
	            	ColorBuffers[index].Bind();
	            	
					Gl.glFramebufferTexture2DEXT(FrameBuffer.Target,
	            	                             Gl.GL_COLOR_ATTACHMENT0_EXT + index,
	            	                             Texture2D.Target,
	            	                             texture.Handle, 0);
	            	
	            	buffers[index] = Gl.GL_COLOR_ATTACHMENT0_EXT + index;
	            }
	            
	            Gl.glDrawBuffers(ColorBuffers.Count, buffers);
            }
            
            {
            	for (int index = 0; index < RenderBuffers.Count; index++)
	            {
	            	RenderBuffer buffer = RenderBuffers[index];
	            	
	            	Gl.glFramebufferRenderbufferEXT(FrameBuffer.Target,
	            	                                buffer.Attachment,
	            	                                RenderBuffer.Target,
	            	                                buffer.Handle);
	            }
            }            
            
            if (current != Handle)
            {
                Gl.glBindFramebufferEXT(Gl.GL_FRAMEBUFFER_EXT, current);
            }
		}
			
		public void Destroy()
		{
			Gl.glDeleteFramebuffersEXT(1, new int[] { Handle });
		}
		
		public void Bind()
		{		
			Gl.glBindFramebufferEXT(Gl.GL_FRAMEBUFFER_EXT, Handle);
		}
		
		public void	Unbind()
		{		
			Gl.glBindFramebufferEXT(Gl.GL_FRAMEBUFFER_EXT, 0);
		}
	
		public void FetchOutcome()
		{
			for (int index = 0; index < ColorBuffers.Count; index++)
			{
				Texture2D texture = ColorBuffers[index];
				
				Gl.glReadBuffer(Gl.GL_COLOR_ATTACHMENT0_EXT + index);
				
				Gl.glReadPixels(0, 0, texture.Width, texture.Height,
				                texture.PixelFormat,
				                texture.Data.Type,
				                texture.Data.Pointer);
			}	
		}
		
		#endregion
		
		#region Properties
		
		public bool	Status
		{
			get
			{
				int	currentBuffer = 0;
				
				unsafe
				{
					Gl.glGetIntegerv(Gl.GL_FRAMEBUFFER_BINDING_EXT, new IntPtr(&currentBuffer));
				}

				if (currentBuffer != Handle)
				{
					Gl.glBindFramebufferEXT(Gl.GL_FRAMEBUFFER_EXT, Handle);
				}

				int status = Gl.glCheckFramebufferStatusEXT(Gl.GL_FRAMEBUFFER_EXT);

				if (currentBuffer != Handle)
				{
					Gl.glBindFramebufferEXT (Gl.GL_FRAMEBUFFER_EXT, currentBuffer);
				}

				return status == Gl.GL_FRAMEBUFFER_COMPLETE_EXT;
			}
		}
		
		public static int MaximumAttachements
		{
			get
			{
			    int result = 0;
			
			    unsafe
			    {
			    	Gl.glGetIntegerv(Gl.GL_MAX_COLOR_ATTACHMENTS_EXT, new IntPtr(&result));
			    }
			    
				return result;
			}
		}
		
		public static int MaximumSize
		{
			get
			{
			    int result = 0;
			
			    unsafe
			    {
			    	Gl.glGetIntegerv(Gl.GL_MAX_RENDERBUFFER_SIZE_EXT, new IntPtr(&result));
			    }
			    
				return result;				
			}
		}
		
		#endregion
	}
}