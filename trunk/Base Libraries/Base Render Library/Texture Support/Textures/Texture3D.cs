/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */

using System;
using Tao.OpenGl;

namespace Base.Graphics
{
	public class Texture3D
	{		
		#region Public Fields
		
		public readonly int Handle = 0;		
		
		public int Width = 128;
		
		public int Height = 128;	
		
		public int Depth = 128;
		
		public int PixelFormat = Gl.GL_RGB;

        public int InternalFormat = Gl.GL_RGB32F_ARB;                
		
		public int Unit = 0;

		public string Name = "Texture";
		
		public TextureData Data = new SingleData();
		
		public WrappingMode Wrapping = new WrappingMode();
		
		public FilteringMode Filtering = new FilteringMode();
		
		public static readonly int Target = Gl.GL_TEXTURE_3D;

        #endregion
	
		#region Constructor
		
		public Texture3D()
		{
			unsafe
		    {
		    	fixed (int* pointer = &Handle)
		    	{
		    		Gl.glGenTextures(1, new IntPtr(pointer));
		    	}
		    }
		}		
		
		public Texture3D(int width, int height, int depth)
			: this()
		{
			Width = width;			
			Height = height;
			Depth = depth;
		}
					
		public Texture3D(int width, int height, int depth, int unit)
			: this(width, height, depth)
		{
			Unit = unit;
		}		
		
		public Texture3D(int width, int height, int depth, int unit, string name)
			: this(width, height, depth, unit)
		{
			Name = name;
		}		
		
		public Texture3D(int width, int height, int depth, int unit, int pixelFormat)
			: this(width, height, depth, unit)
		{
			PixelFormat = pixelFormat;
		}		

		public Texture3D(int width, int height, int depth, int unit, int pixelFormat, string name)
			: this(width, height, depth, unit, pixelFormat)
		{
			Name = name;
		}
					
		public Texture3D(int width, int height, int depth, int unit, int pixelFormat, int internalFormat)
			: this(width, height, depth, unit, pixelFormat)
		{
			InternalFormat = internalFormat;
		}									
		
		public Texture3D(int width, int height, int depth, int unit, int pixelFormat, int internalFormat, string name)
			: this(width, height, depth, unit, pixelFormat, internalFormat)
		{
			Name = name;
		}
				
		public Texture3D(TextureData data, int width, int height, int depth)
			: this(width, height, depth)
		{
			Data = data;
		}			
		
		public Texture3D(TextureData data, int width, int height, int depth, int unit)
			: this(data, width, height, depth)
		{
			Unit = unit;
		}		
		
		public Texture3D(TextureData data, int width, int height, int depth, int unit, string name)
			: this(data, width, height, depth, unit)
		{
			Name = name;
		}		
		
		public Texture3D(TextureData data, int width, int height, int depth, int unit, int pixelFormat)
			: this(data, width, height, depth, unit)
		{
			PixelFormat = pixelFormat;
		}		
		
		public Texture3D(TextureData data, int width, int height, int depth, int unit, int pixelFormat, string name)
			: this(data, width, height, depth, unit, pixelFormat)
		{
			Name = name;
		}		
						
		public Texture3D(TextureData data, int width, int height, int depth, int unit, int pixelFormat, int internalFormat)
			: this(data, width, height, depth, unit, pixelFormat)
		{
			InternalFormat = internalFormat;
		}									
		
		public Texture3D(TextureData data, int width, int height, int depth, int unit, int pixelFormat, int internalFormat, string name)
			: this(data, width, height, depth, unit, pixelFormat, internalFormat)
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
				
				Gl.glTexImage3D(Target, 0, InternalFormat, Width, Height, Depth, 0, PixelFormat, Data.Type, Data.Pointer);
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