/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */
 
using System;
using System.Windows.Forms;

namespace Base.Graphics
{
	public class Keyboard
	{
		#region Public Fields
		
		public Camera Camera = null;
		
		public float Step = 0.2f;	
		
		#endregion
		
		#region Constructor
				
		public Keyboard(Camera camera)
		{
			Camera = camera;
		}
		
		public Keyboard(Camera camera, float speed)
			: this(camera)
		{			
			Step = speed;
		}		
		
		#endregion
		
		#region Public Methods
		
		public void OnKeyPressed(byte key)
		{
			if (null == Camera)
				return;
			
			switch (key)
			{
			    case (byte) 'W': case (byte) 'w':
				    Camera.MoveStraight(Step); break;
				
			    case (byte) 'S': case (byte) 's':
				    Camera.MoveStraight(-Step); break;
				
			    case (byte) 'A': case (byte) 'a':
				    Camera.MoveHorizontal(-Step); break;
				
			    case (byte) 'D': case (byte) 'd':
				    Camera.MoveHorizontal(Step); break;
				
			    case (byte) 'Z': case (byte) 'z':
				    Camera.MoveVertical(-Step); break;
				
			    case (byte) ' ':
				    Camera.MoveVertical(Step); break;					
			}
		}
		
		#endregion
	}
}
