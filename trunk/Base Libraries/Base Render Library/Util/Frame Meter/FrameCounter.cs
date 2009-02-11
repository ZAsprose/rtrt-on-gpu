/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */
 
using System;

namespace Base.Graphics
{
	public class FrameMeter
	{
		#region Private Fields
		
		private static int lastTick;
        
		private static float lastFrameRate;
        
		private static float frameRate;
		
		private static int interval = 3000;		

		#endregion
		
		#region Public Methods
		
		public static float CalculateFrameRate()
        {
            if (System.Environment.TickCount - lastTick >= interval)
            {
                lastFrameRate = frameRate;
                
                lastTick = System.Environment.TickCount;
                
                frameRate = 0;
                
                Console.WriteLine("FPS: " + 1000 * lastFrameRate / interval);
            }
            
            frameRate++;
            
            return 1000 * lastFrameRate / interval;
        }
        
        #endregion
	}
}
