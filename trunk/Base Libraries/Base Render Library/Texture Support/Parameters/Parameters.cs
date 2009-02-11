/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */
 
using System;
using Tao.OpenGl;

namespace Base.Graphics
{
	public class FilteringMode
	{
		#region Public Fields
		
		public int Min = Gl.GL_NEAREST;
		
		public int Mag = Gl.GL_NEAREST;
		
		#endregion
		
		#region Constructor
				
		public FilteringMode() { }
		
		public FilteringMode(int min, int mag)
		{
			Min = min;			
			Mag =  mag;
		}	
		
		#endregion
		
		#region Public Methods
		
		public void Setup(int target)
		{
			Gl.glTexParameteri(target, Gl.GL_TEXTURE_MIN_FILTER, Min);
			
			Gl.glTexParameteri(target, Gl.GL_TEXTURE_MAG_FILTER, Mag);
		}
		
		#endregion
	}
	
	public class WrappingMode
	{
		#region Public Fields
		
		public int S = Gl.GL_REPEAT;
		
		public int T = Gl.GL_REPEAT;
		
		public int R = Gl.GL_REPEAT;
		
		#endregion
		
		#region Constructor
				
		public WrappingMode() { }
		
		public WrappingMode(int s)
		{
			S = s;
		}
		
		public WrappingMode(int s, int t)
			: this(s)
		{
			T = t;
		}
		
		public WrappingMode(int s, int t, int r)
			: this(s, t)
		{
			R = r;
		}		
		
		#endregion
		
		#region Public Methods
		
		public void Setup(int target)
		{
			Gl.glTexParameteri(target, Gl.GL_TEXTURE_WRAP_S, S);
			
			Gl.glTexParameteri(target, Gl.GL_TEXTURE_WRAP_T, T);
			
			Gl.glTexParameteri(target, Gl.GL_TEXTURE_WRAP_R, R);
		}
		
		#endregion
	}
}