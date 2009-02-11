/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */
 
using System;

namespace Base.Graphics
{
	[Serializable]
	public abstract class TextureData
	{
		#region Properties
		
		public abstract IntPtr Pointer
		{
			get;
		}
		
		public abstract int Type
		{
			get;
		}
		
		#endregion
	}	
}