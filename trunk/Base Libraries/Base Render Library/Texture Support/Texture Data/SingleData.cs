/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */
 
using System;
using System.Runtime.InteropServices;
using Tao.OpenGl;

namespace Base.Graphics
{
	[Serializable]
	public class SingleData : TextureData
	{
		#region Private Fields
		
		public float[] pixels = null;
		
		#endregion
		
		#region Constructor
		
		public SingleData() { }
		
		public SingleData(int length)
		{
			pixels = new float[length];
		}		

		public SingleData(float[] data)
		{
			pixels = data;
		}
			
		public SingleData(IntPtr pointer, int length)
			: this(length)
		{
			Marshal.Copy(pointer, pixels, 0, length);
		}			
				
		#endregion

        #region Operators

        public static implicit operator float[](SingleData data)
        {
            return data.pixels;
        }

        #endregion

		#region Indexer
		
		public float this[int index]
		{
			get
			{
				return pixels[index];
			}
			
			set
			{
				pixels[index] = value;
			}
		}
		
		#endregion
		
		#region Properties
		
		public override IntPtr Pointer
		{
			get
			{
				unsafe
				{
					fixed (float * pointer = pixels)
					{
						return new IntPtr(pointer);
					}
				}
			}
		}
		
		public override int Type
		{
			get
			{
				return Gl.GL_FLOAT;
			}
		}
		
		#endregion
	}
}