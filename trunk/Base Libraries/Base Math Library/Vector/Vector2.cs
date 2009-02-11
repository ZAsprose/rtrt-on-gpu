/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */

using System;

namespace Base.MathTools
{
    [Serializable]
    public class Vector2
    {
        #region Private Fields

        private static readonly int size = 2;

        private float[] values = new float[size];

        #endregion

        #region Constructor
        
        public Vector2(float x, float y)
        {
        	values = new float[] { x, y };
        }        
        
        public Vector2(Vector2 vector)
        {
        	values = new float[] { vector.X, vector.Y };
        }

        public Vector2(Vector3 vector)
        {
        	values = new float[] { vector.X, vector.Y };
        }
        
        public Vector2(Vector4 vector)
        {
        	values = new float[] { vector.X, vector.Y };
        }        

        public Vector2(float[] vector)
        {
        	for (int i = 0; i < size; i++)
        	{
        		values[i] = vector[i];
        	}
        }
        
        #endregion

        #region Operators

        public static Vector2 operator -(Vector2 source)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = -source[i];
        	}
        	
        	return result;
        }

        public static Vector2 operator +(Vector2 left, Vector2 right)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] + right[i];
        	}
        	
        	return result;
        }

        public static Vector2 operator -(Vector2 left, Vector2 right)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] - right[i];
        	}
        	
        	return result;
        }

        public static Vector2 operator *(float left, Vector2 right)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left * right[i];
        	}
        	
        	return result;
        }

        public static Vector2 operator *(Vector2 left, float right)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] * right;
        	}
        	
        	return result;
        }

        public static Vector2 operator *(Vector2 left, Vector2 right)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] * right[i];
        	}
        	
        	return result;
        }

        public static Vector2 operator /(Vector2 left, float right)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] / right;
        	}
        	
        	return result;
        }

        public static Vector2 operator /(Vector2 left, Vector2 right)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] / right[i];
        	}
        	
        	return result;
        }

        public static bool operator <(Vector2 left, Vector2 right)
        {
        	for (int i = 0; i < size; i++)
        	{
        		if (left[i] >= right[i])
        		{
        			return false;
        		}
        	}
        	
        	return true;
        }
        
        public static bool operator <(Vector2 left, float right)
        {
        	for (int i = 0; i < size; i++)
        	{
        		if (left[i] >= right)
        		{
        			return false;
        		}
        	}
        	
        	return true;
        }        

        public static bool operator >(Vector2 left, Vector2 right)
        {
        	for (int i = 0; i < size; i++)
        	{
        		if (left[i] <= right[i])
        		{
        			return false;
        		}
        	}
        	
        	return true;
        }
        
        public static bool operator >(Vector2 left, float right)
        {
        	for (int i = 0; i < size; i++)
        	{
        		if (left[i] <= right)
        		{
        			return false;
        		}
        	}
        	
        	return true;
        }        

        public static bool operator <=(Vector2 left, Vector2 right)
        {
        	for (int i = 0; i < size; i++)
        	{
        		if (left[i] > right[i])
        		{
        			return false;
        		}
        	}
        	
        	return true;
        }
        
        public static bool operator <=(Vector2 left, float right)
        {
        	for (int i = 0; i < size; i++)
        	{
        		if (left[i] > right)
        		{
        			return false;
        		}
        	}
        	
        	return true;
        }        

        public static bool operator >=(Vector2 left, Vector2 right)
        {
        	for (int i = 0; i < size; i++)
        	{
        		if (left[i] < right[i])
        		{
        			return false;
        		}
        	}
        	
        	return true;
        }
        
        public static bool operator >=(Vector2 left, float right)
        {
        	for (int i = 0; i < size; i++)
        	{
        		if (left[i] < right)
        		{
        			return false;
        		}
        	}
        	
        	return true;
        }        

        public static bool operator !=(Vector2 left, Vector2 right)
        {
        	for (int i = 0; i < size; i++)
        	{
        		if (left[i] != right[i])
        		{
        			return true;
        		}
        	}
        	
        	return false;
        }

        public static bool operator ==(Vector2 left, Vector2 right)
        {
        	for (int i = 0; i < size; i++)
        	{
        		if (left[i] != right[i])
        		{
        			return false;
        		}
        	}
        	
        	return true;
        }
        
        public static implicit operator float[](Vector2 source)
        {
            return source.values;
        }          
        
        #endregion

        #region Public Methods

        #region Общие функции

        public static Vector2 Abs(Vector2 source)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Math.Abs(source[i]);
        	}        	
        	
            return result;
        }

        public static Vector2 Sign(Vector2 source)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Math.Sign(source[i]);
        	}        	
        	
            return result;
        }

        public static Vector2 Floor(Vector2 source)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Floor(source[i]);
        	}        	
        	
            return result;
        }

        public static Vector2 Fract(Vector2 source)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Fract(source[i]);
        	}        	
        	
            return result;
        }

        public static Vector2 Ceiling(Vector2 source)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Ceiling(source[i]);
        	}        	
        	
            return result;
        }

        public static Vector2 Mod(Vector2 source, float value)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Mod(source[i], value);
        	}
        	
        	return result;
        }

        public static Vector2 Mod(Vector2 left, Vector2 right)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Mod(left[i], right[i]);
        	}        	
        	
            return result;
        }

        public static Vector2 Min(Vector2 source, float value)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Math.Min(source[i], value);
        	}
        	
        	return result;
        }

        public static Vector2 Min(Vector2 left, Vector2 right)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Math.Min(left[i], right[i]);
        	}
        	
        	return result;
        }

        public static Vector2 Max(Vector2 source, float value)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Math.Max(source[i], value);
        	}
        	
        	return result;
        }

        public static Vector2 Max(Vector2 left, Vector2 right)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Math.Max(left[i], right[i]);
        	}
        	
        	return result;
        }

        public static Vector2 Clamp(Vector2 source, float min, float max)
        {
        	Vector2 result = new Vector2(source);
        	
        	for (int i = 0; i < size; i++)
        	{
        		if (result[i] < min)
        		{
        			result[i] = min;
        		}
        		
        		if (result[i] > max)
        		{
        			result[i] = max;
        		}
        	}
        	
        	return result;
        }
        
        public static Vector2 Clamp(Vector2 source, Vector2 min, Vector2 max)
        {
        	Vector2 result = new Vector2(source);
        	
        	for (int i = 0; i < size; i++)
        	{
        		if (result[i] < min[i])
        		{
        			result[i] = min[i];
        		}
        		
        		if (result[i] > max[i])
        		{
        			result[i] = max[i];
        		}
        	}
        	
        	return result;
        }        

        public static Vector2 Mix(Vector2 left, Vector2 right, float value)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Mix(left[i], right[i], value);
        	}
        	
        	return result;
        }

        public static Vector2 Mix(Vector2 left, Vector2 right, Vector2 value)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Mix(left[i], right[i], value[i]);
        	}
        	
        	return result;
        }
        
        public static Vector2 Step(Vector2 source, float value)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Step(source[i], value);
        	}
        	
        	return result;
        }

        public static Vector2 Step(Vector2 source, Vector2 value)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Step(source[i], value[i]);
        	}
        	
        	return result;
        }
        
        public static Vector2 Smooth(Vector2 source, float left, float right)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Smooth(source[i], left, right);
        	}
        	
        	return result;
        }

        public static Vector2 Smooth(Vector2 source, Vector2 left, Vector2 right)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Smooth(source[i], left[i], right[i]);
        	}
        	
        	return result;
        }
        
        #endregion

        #region Геометрические функции

        public static float Length(Vector2 source)
        {
        	float result = 0.0f;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result += source[i] * source[i];
        	}
        	
        	return (float) Math.Sqrt(result);
        }

        public static float Square(Vector2 source)
        {
        	float result = 0.0f;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result += source[i] * source[i];
        	}
        	
        	return result;
        }

        public static float Distance(Vector2 left, Vector2 right)
        {
            return Length(left - right);
        }

        public static float Dot(Vector2 left, Vector2 right)
        {
        	float result = 0.0f;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result += left[i] * right[i];
        	}
        	
        	return result;
        }        
        
        public static Vector2 Normalize(Vector2 source)
        {
            return source / Length(source);
        }

        public static Vector2 Reflect(Vector2 incident, Vector2 normal)
        {
            return incident - 2.0f * Dot(normal, incident) * normal;
        }

        public static Vector2 Refract(Vector2 incident, Vector2 normal, float index)
        {
            float dot = Dot(incident, normal);

            float square = 1.0f - index * index * (1.0f - dot * dot);

            if (square < 0.0f)
            {
                return Reflect(incident, normal);
            }
            else
            {
                float cos = (float) Math.Sqrt(square);

                return index * incident - (cos + index * dot) * normal;
            }
        }

        #endregion

        #region Угловые и тригонометрические функции

        public static Vector2 Radians(Vector2 source)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Radians(source[i]);
        	}
        	
        	return result;
        }
        
        public static Vector2 Degrees(Vector2 source)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Degrees(source[i]);
        	}
        	
        	return result;
        }        
        
        public static Vector2 Sin(Vector2 source)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Sin(source[i]);
        	}
        	
        	return result;
        }

        public static Vector2 Cos(Vector2 source)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Cos(source[i]);
        	}
        	
        	return result;
        }

        public static Vector2 Tan(Vector2 source)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Tan(source[i]);
        	}
        	
        	return result;
        }

        public static Vector2 Asin(Vector2 source)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Asin(source[i]);
        	}
        	
        	return result;
        }

        public static Vector2 Acos(Vector2 source)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Acos(source[i]);
        	}
        	
        	return result;
        }

        public static Vector2 Atan(Vector2 source)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Atan(source[i]);
        	}
        	
        	return result;
        }

        #endregion

        #region Экспоненциальные функции

        public static Vector2 Pow(Vector2 left, Vector2 right)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Pow(left[i], right[i]);
        	}
        	
        	return result;
        }

        public static Vector2 Exp(Vector2 source)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Exp(source[i]);
        	}
        	
        	return result;
        }

        public static Vector2 Log(Vector2 source)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Log(source[i]);
        	}
        	
        	return result;
        }

        public static Vector2 Sqrt(Vector2 source)
        {
        	Vector2 result = Vector2.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Sqrt(source[i]);
        	}
        	
        	return result;
        }

        #endregion

        #region Дополнительные функции

        public override int GetHashCode()
        {
        	return values[0].GetHashCode() ^ values[1].GetHashCode();
        }

        public override bool Equals(object o)
        {
            if (o is Vector2)
            {
                return this == o as Vector2;
            }

            return false;
        }

		public override string ToString()
		{
			string result = "[ ";
			
			for (int i = 0; i < size; i++)
			{
				result += String.Format("{0:0.00}", values[i]) + " ";
			}
			
			result += "]";
			
			return result;
		}       
		
		public float[] ToArray()
		{
			return new float[] { values[0], values[1] };
		}

        public void CopyTo(float[] array, int index)
        {
            values.CopyTo(array, index);
        }

        #endregion

        #endregion
        
        #region Indexer
        
        public float this[int index]
        {
            get
            {
            	return values[index];
            }

            set
            {
                values[index] = value;
            }
        }     
        
        #endregion        
        
        #region Properties
        
        public float X
        {
        	get
        	{
        		return values[0];
        	}
        	
        	set
        	{
        		values[0] = value;
        	}
        }
        
        public float Y
        {
        	get
        	{
        		return values[1];
        	}
        	
        	set
        	{
        		values[1] = value;
        	}
        }        

        public static Vector2 Empty
        {
        	get
        	{
        		return new Vector2(0.0f, 0.0f);
        	}
        }

        public static Vector2 Epsilon
        {
        	get
        	{
        		return new Vector2(Scalar.Epsilon, Scalar.Epsilon);
        	}
        }                
        
        public static Vector2 Unit
        {
        	get
        	{
        		return new Vector2(1.0f, 1.0f);
        	}
        }

        public static Vector2 UnitX
        {
        	get
        	{
        		return new Vector2(1.0f, 0.0f);
        	}
        }

        public static Vector2 UnitY
        {
        	get
        	{
        		return new Vector2(0.0f, 1.0f);
        	}
        }       
        
        #endregion
    }
}