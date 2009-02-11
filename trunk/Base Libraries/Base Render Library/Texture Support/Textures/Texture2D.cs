/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */

using System;
using Tao.OpenGl;

namespace Base.Graphics
{
	public class Texture2D
	{		
		#region Public Fields
		
		public readonly int Handle = 0;		
		
		public int Width = 512;
		
		public int Height = 512;		
		
		public int PixelFormat = Gl.GL_RGB;

        public int InternalFormat = Gl.GL_RGB32F_ARB;                
		
		public int Unit = 0;

		public string Name = "Texture";
		
		public TextureData Data = new SingleData();
		
		public WrappingMode Wrapping = new WrappingMode();
		
		public FilteringMode Filtering = new FilteringMode();
		
		public static readonly int Target = Gl.GL_TEXTURE_2D;

        #endregion
	
		#region Constructor
		
		public Texture2D()
		{
			unsafe
		    {
		    	fixed (int* pointer = &Handle)
		    	{
		    		Gl.glGenTextures(1, new IntPtr(pointer));
		    	}
		    }
		}		
		
		public Texture2D(int width, int height)
			: this()
		{
			Width = width;			
			Height = height;
		}
					
		public Texture2D(int width, int height, int unit)
			: this(width, height)
		{
			Unit = unit;
		}		
		
		public Texture2D(int width, int height, int unit, string name)
			: this(width, height, unit)
		{
			Name = name;
		}		
		
		public Texture2D(int width, int height, int unit, int pixelFormat)
			: this(width, height, unit)
		{
			PixelFormat = pixelFormat;
		}		

		public Texture2D(int width, int height, int unit, int pixelFormat, string name)
			: this(width, height, unit, pixelFormat)
		{
			Name = name;
		}
					
		public Texture2D(int width, int height, int unit, int pixelFormat, int internalFormat)
			: this(width, height, unit, pixelFormat)
		{
			InternalFormat = internalFormat;
		}									
		
		public Texture2D(int width, int height, int unit, int pixelFormat, int internalFormat, string name)
			: this(width, height, unit, pixelFormat, internalFormat)
		{
			Name = name;
		}
				
		public Texture2D(TextureData data, int width, int height)
			: this(width, height)
		{
			Data = data;
		}			
		
		public Texture2D(TextureData data, int width, int height, int unit)
			: this(data, width, height)
		{
			Unit = unit;
		}		
		
		public Texture2D(TextureData data, int width, int height, int unit, string name)
			: this(data, width, height, unit)
		{
			Name = name;
		}		
		
		public Texture2D(TextureData data, int width, int height, int unit, int pixelFormat)
			: this(data, width, height, unit)
		{
			PixelFormat = pixelFormat;
		}		
		
		public Texture2D(TextureData data, int width, int height, int unit, int pixelFormat, string name)
			: this(data, width, height, unit, pixelFormat)
		{
			Name = name;
		}		
						
		public Texture2D(TextureData data, int width, int height, int unit, int pixelFormat, int internalFormat)
			: this(data, width, height, unit, pixelFormat)
		{
			InternalFormat = internalFormat;
		}									
		
		public Texture2D(TextureData data, int width, int height, int unit, int pixelFormat, int internalFormat, string name)
			: this(data, width, height, unit, pixelFormat, internalFormat)
		{
			Name = name;
		}
		
		#endregion
		
		#region Public Methods
		
		public void Setup()
		{
			//logger.Trace("Загрузка данных в текстуру [ " + Name + " @ " + Unit + " ]");
			
			if (null != Data)
			{
                Bind();

                Wrapping.Setup(Target);
                
                Filtering.Setup(Target);
				
				Gl.glTexImage2D(Target, 0, InternalFormat, Width, Height, 0, PixelFormat, Data.Type, Data.Pointer);
			}
			else
			{
				//logger.Error("Ошибка! Данные отсутствуют (null)!");
			}			                
		}
		
		public void Destroy()
		{
			//logger.Trace("Удаление ресурсов текстуры [" + Name + " @ " + Unit + " ]");
			
		    Gl.glDeleteTextures(1, new int[] { Handle });
		}		

		public void Bind()
		{
            Gl.glActiveTexture(Gl.GL_TEXTURE0 + Unit);
			
			Gl.glBindTexture(Target, Handle);
		}
		
		public void Unbind()
		{
			Gl.glActiveTexture(Unit);
			
			Gl.glBindTexture(Target, 0);			
		}
		
		#endregion
	}
}