/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */

using System;

namespace Base.MathTools
{
	[Serializable]
    public class Scalar
    {
    	#region Public Fields
    	
    	public static readonly float OnePI = Convert.ToSingle(Math.PI);
		
		public static readonly float TwoPI = Convert.ToSingle(2.0 * Math.PI);
    	
    	public static readonly float Exponent = Convert.ToSingle(Math.E);
    	
        public static readonly float Epsilon = 0.0001f;

    	#endregion
    	
    	#region Public Methods
    	
    	#region Общие функции
    	
        public static float Fract(float source)
        {
        	return Convert.ToSingle(source - Math.Floor(source));
        }

        public static float Mod(float source, float value)
        {
        	return Convert.ToSingle(source - value * Math.Floor(source / value));
        }

     	public static float Clamp(float source, float min, float max)
        {
            if (source < min)
            {
                return min;
            }
            else
            {
                if (source > max)
                {
                    return max;
                }
                else
                {
                    return source;
                }
            }
        }
        
     	public static float Mix(float left, float right, float value)
        {
        	return left * (1.0f - value) + right * value;
        }
     	
     	public static float Step(float source, float value)
        {
            if (source < value)
            {
                return 0.0f;
            }
            else
            {
                return 1.0f;
            }
        }

        public static float Smooth(float source, float left, float right)        	
        {
            float result = Clamp((source - left) / (right - left), 0.0f, 1.0f);

            return result * result * (3.0f - 2.0f * result);
        }           	
    	
    	#endregion
    	
    	#region Угловые функции    	
    	
    	public static float Degrees(float radians)
    	{
    		return 180.0f * radians / OnePI;
    	}
    	
    	public static float Radians(float degrees)
    	{
    		return OnePI * degrees / 180.0f;
    	}    	
    	
    	#endregion
    	
        #endregion
    }
}
