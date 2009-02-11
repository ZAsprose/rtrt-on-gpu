/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */
 
using System;
using System.Runtime.InteropServices;
using Tao.OpenGl;

namespace Base.Graphics
{
	[Serializable]
	public class ByteData : TextureData
	{
		#region Private Fields
		
		public byte[] pixels = null;
		
		#endregion
		
		#region Constructor
		
		public ByteData() { }
		
		public ByteData(int length)
		{
			pixels = new byte[length];
		}		

		public ByteData(byte[] data)
		{
			pixels = data;
		}
			
		public ByteData(IntPtr pointer, int length)
			: this(length)
		{
			Marshal.Copy(pointer, pixels, 0, length);
		}			
		
		#endregion

        #region Operators

        public static implicit operator byte[](ByteData data)
        {
            return data.pixels;
        }

        #endregion

        #region Indexer

        public byte this[int index]
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
					fixed (byte * pointer = pixels)
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
				return Gl.GL_UNSIGNED_BYTE;
			}
		}		
		
		#endregion
	}	
}