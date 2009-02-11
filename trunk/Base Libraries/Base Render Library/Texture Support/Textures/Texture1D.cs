/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */

using System;
using Tao.OpenGl;

namespace Base.Graphics
{
	public class Texture1D
	{		
		#region Public Fields
		
		public readonly int Handle = 0;		
		
		public int Width = 512;
		
		public int PixelFormat = Gl.GL_RGB;

        public int InternalFormat = Gl.GL_RGB32F_ARB;                
		
		public int Unit = 0;

		public string Name = "Texture";
		
		public TextureData Data = new SingleData();
		
		public WrappingMode Wrapping = new WrappingMode();
		
		public FilteringMode Filtering = new FilteringMode();
		
		public static readonly int Target = Gl.GL_TEXTURE_1D;

        #endregion
	
		#region Constructor
		
		public Texture1D()
		{
			unsafe
		    {
		    	fixed (int* pointer = &Handle)
		    	{
		    		Gl.glGenTextures(1, new IntPtr(pointer));
		    	}
		    }
		}		
		
		public Texture1D(int width)
			: this()
		{
			Width = width;
		}
					
		public Texture1D(int width, int unit)
			: this(width)
		{
			Unit = unit;
		}		
		
		public Texture1D(int width, int unit, string name)
			: this(width, unit)
		{
			Name = name;
		}		
		
		public Texture1D(int width, int unit, int pixelFormat)
			: this(width, unit)
		{
			PixelFormat = pixelFormat;
		}		

		public Texture1D(int width, int unit, int pixelFormat, string name)
			: this(width, unit, pixelFormat)
		{
			Name = name;
		}
					
		public Texture1D(int width, int unit, int pixelFormat, int internalFormat)
			: this(width, unit, pixelFormat)
		{
			InternalFormat = internalFormat;
		}									
		
		public Texture1D(int width, int unit, int pixelFormat, int internalFormat, string name)
			: this(width, unit, pixelFormat, internalFormat)
		{
			Name = name;
		}
				
		public Texture1D(TextureData data, int width)
			: this(width)
		{
			Data = data;
		}			
		
		public Texture1D(TextureData data, int width, int unit)
			: this(data, width)
		{
			Unit = unit;
		}		
		
		public Texture1D(TextureData data, int width, int unit, string name)
			: this(data, width, unit)
		{
			Name = name;
		}		
		
		public Texture1D(TextureData data, int width, int unit, int pixelFormat)
			: this(data, width, unit)
		{
			PixelFormat = pixelFormat;
		}		
		
		public Texture1D(TextureData data, int width, int unit, int pixelFormat, string name)
			: this(data, width, unit, pixelFormat)
		{
			Name = name;
		}		
						
		public Texture1D(TextureData data, int width, int unit, int pixelFormat, int internalFormat)
			: this(data, width, unit, pixelFormat)
		{
			InternalFormat = internalFormat;
		}									
		
		public Texture1D(TextureData data, int width, int unit, int pixelFormat, int internalFormat, string name)
			: this(data, width, unit, pixelFormat, internalFormat)
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
				
				Gl.glTexImage1D(Target, 0, InternalFormat, Width, 0, PixelFormat, Data.Type, Data.Pointer);
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