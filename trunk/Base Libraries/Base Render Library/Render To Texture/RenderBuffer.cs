/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */

using System;
using Tao.OpenGl;

namespace Base.Graphics
{
    public class RenderBuffer
    {		
		#region Public Fields

        public readonly int Handle = 0;
		
		public int Width = 512;

        public int Height = 512;

        public int Attachment = Gl.GL_DEPTH_ATTACHMENT_EXT;
				
		public int InternalFormat = Gl.GL_DEPTH_COMPONENT16_ARB;				
        
        public readonly static int Target = Gl.GL_RENDERBUFFER_EXT;
		
		#endregion
		
		#region Constructor

        public RenderBuffer()
        {
            unsafe
            {
                fixed (int* pointer = &Handle)
                {
                    Gl.glGenRenderbuffersEXT(1, new IntPtr(pointer));
                }
            }
        }
		
		public RenderBuffer(int width, int height)
            : this()
		{
			Width = width;
			Height = height;
		}
		
		public RenderBuffer(int width, int height, int attachment, int internalFormat)
			: this(width, height)
		{
            Attachment = attachment;
			InternalFormat = internalFormat;			
		}
		
		#endregion
		
		#region Public Methods
			
		public void Setup()
		{	
			Gl.glBindRenderbufferEXT(Gl.GL_RENDERBUFFER_EXT, Handle);

			Gl.glRenderbufferStorageEXT(Target, InternalFormat, Width, Height);	
		}
		
		public void Destroy()
		{
		    Gl.glDeleteRenderbuffersEXT(1, new int[] { Handle });
		}
				
		#endregion
	}
}