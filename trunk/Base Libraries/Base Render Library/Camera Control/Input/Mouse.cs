/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */
 
using System;
using Base.MathTools;

namespace Base.Graphics
{
	public class Mouse
	{
		#region Private Fields
		
		private int locationX = 0;
		
		private int locationY = 0;

		#endregion		
		
		#region Public Fields
		
		public Camera Camera = null;
		
		public float Step = 0.005f;
		
		#endregion
		
		#region Constructor
		
		public Mouse(Camera camera)
		{
			Camera = camera;
		}		
		
		public Mouse(Camera camera, float speed)
			: this(camera)
		{
			Step = speed;
		}
		
		#endregion
			
		#region Public Methods
		
		public void OnStateChange(int x, int y)
		{
			locationX = x;
			
			locationY = y;			
		}
		
		public void OnMouseMove(int x, int y)
		{
			if (null == Camera)
				return;
												
        	if (locationX != x)
        	{
        		Camera.Rotate(Vector3.UnitZ * (x - locationX) * Step);
        	}
        	
        	if (locationY != y)
        	{
        		Camera.Rotate(Vector3.UnitY * (y - locationY) * Step);
			}
			
			locationX = x;
			
			locationY = y;
		}
		
		#endregion
	}
}
