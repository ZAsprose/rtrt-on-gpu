/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */
 
using System;

namespace Base.MathTools
{
    [Serializable]
    public class Vector3
    {
        #region Private Fields

        private static readonly int size = 3;

        private float[] values = new float[size];

        #endregion

        #region Constructor
        
        public Vector3(float x, float y, float z)
        {
        	values = new float[] { x, y, z };
        }        
        
        public Vector3(Vector2 vector, float z)
        {
        	values = new float[] { vector.X, vector.Y, z };
        }

        public Vector3(Vector3 vector)
        {
        	values = new float[] { vector.X, vector.Y, vector.Z };
        }
        
        public Vector3(Vector4 vector)
        {
        	values = new float[] { vector.X, vector.Y, vector.Z };
        }        

        public Vector3(float[] vector)
        {
        	for (int i = 0; i < size; i++)
        	{
        		values[i] = vector[i];
        	}
        }
        
        #endregion

        #region Operators

        public static Vector3 operator -(Vector3 source)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = -source[i];
        	}
        	
        	return result;
        }

        public static Vector3 operator +(Vector3 left, Vector3 right)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] + right[i];
        	}
        	
        	return result;
        }

        public static Vector3 operator -(Vector3 left, Vector3 right)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] - right[i];
        	}
        	
        	return result;
        }

        public static Vector3 operator *(float left, Vector3 right)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left * right[i];
        	}
        	
        	return result;
        }

        public static Vector3 operator *(Vector3 left, float right)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] * right;
        	}
        	
        	return result;
        }

        public static Vector3 operator *(Vector3 left, Vector3 right)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] * right[i];
        	}
        	
        	return result;
        }

        public static Vector3 operator /(Vector3 left, float right)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] / right;
        	}
        	
        	return result;
        }

        public static Vector3 operator /(Vector3 left, Vector3 right)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] / right[i];
        	}
        	
        	return result;
        }

        public static bool operator <(Vector3 left, Vector3 right)
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
        
        public static bool operator <(Vector3 left, float right)
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

        public static bool operator >(Vector3 left, Vector3 right)
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
        
        public static bool operator >(Vector3 left, float right)
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

        public static bool operator <=(Vector3 left, Vector3 right)
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
        
        public static bool operator <=(Vector3 left, float right)
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

        public static bool operator >=(Vector3 left, Vector3 right)
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
        
        public static bool operator >=(Vector3 left, float right)
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

        public static bool operator !=(Vector3 left, Vector3 right)
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

        public static bool operator ==(Vector3 left, Vector3 right)
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

        public static implicit operator float[](Vector3 source)
        {
            return source.values;
        }       
        
        #endregion

        #region Public Methods

        #region Общие функции

        public static Vector3 Abs(Vector3 source)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Math.Abs(source[i]);
        	}        	
        	
            return result;
        }

        public static Vector3 Sign(Vector3 source)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Math.Sign(source[i]);
        	}        	
        	
            return result;
        }

        public static Vector3 Floor(Vector3 source)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Floor(source[i]);
        	}        	
        	
            return result;
        }

        public static Vector3 Fract(Vector3 source)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = source[i] - (float) Math.Floor(source[i]);
        	}        	
        	
            return result;
        }

        public static Vector3 Ceiling(Vector3 source)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Ceiling(source[i]);
        	}        	
        	
            return result;
        }

        public static Vector3 Mod(Vector3 source, float value)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = source[i] - value * (float) Math.Floor(source[i] / value);
        	}
        	
        	return result;
        }

        public static Vector3 Mod(Vector3 left, Vector3 right)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] - right[i] * (float) Math.Floor(left[i] / right[i]);
        	}        	
        	
            return result;
        }

        public static Vector3 Min(Vector3 source, float value)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Math.Min(source[i], value);
        	}
        	
        	return result;
        }

        public static Vector3 Min(Vector3 left, Vector3 right)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Math.Min(left[i], right[i]);
        	}
        	
        	return result;
        }

        public static Vector3 Max(Vector3 source, float value)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Math.Max(source[i], value);
        	}
        	
        	return result;
        }

        public static Vector3 Max(Vector3 left, Vector3 right)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Math.Max(left[i], right[i]);
        	}
        	
        	return result;
        }

        public static Vector3 Clamp(Vector3 source, float min, float max)
        {
        	Vector3 result = new Vector3(source);
        	
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
        
        public static Vector3 Clamp(Vector3 source, Vector3 min, Vector3 max)
        {
        	Vector3 result = new Vector3(source);
        	
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

        public static Vector3 Mix(Vector3 left, Vector3 right, float value)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] * (1.0f - value) + right[i] * value;
        	}
        	
        	return result;
        }

        public static Vector3 Mix(Vector3 left, Vector3 right, Vector3 value)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] * (1.0f - value[i]) + right[i] * value[i];
        	}
        	
        	return result;
        }
        
        public static Vector3 Step(Vector3 source, float value)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Step(source[i], value);
        	}
        	
        	return result;
        }

        public static Vector3 Step(Vector3 source, Vector3 value)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Step(source[i], value[i]);
        	}
        	
        	return result;
        }
        
        public static Vector3 Smooth(Vector3 source, float left, float right)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Smooth(source[i], left, right);
        	}
        	
        	return result;
        }

        public static Vector3 Smooth(Vector3 source, Vector3 left, Vector3 right)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Smooth(source[i], left[i], right[i]);
        	}
        	
        	return result;
        }
        
        #endregion

        #region Геометрические функции

        public static float Length(Vector3 source)
        {
        	float result = 0.0f;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result += source[i] * source[i];
        	}
        	
        	return (float) Math.Sqrt(result);
        }

        public static float Square(Vector3 source)
        {
        	float result = 0.0f;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result += source[i] * source[i];
        	}
        	
        	return result;
        }

        public static float Distance(Vector3 left, Vector3 right)
        {
            return Length(left - right);
        }

        public static float Dot(Vector3 left, Vector3 right)
        {
        	float result = 0.0f;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result += left[i] * right[i];
        	}
        	
        	return result;
        }        
        
        public static Vector3 Cross(Vector3 left, Vector3 right)
        {
            return new Vector3(left[1] * right[2] - left[2] * right[1],
        	                   left[2] * right[0] - left[0] * right[2],
        	                   left[0] * right[1] - left[1] * right[0]);
        }        
        
        public static Vector3 Normalize(Vector3 source)
        {
            return source / Length(source);
        }

        public static Vector3 Reflect(Vector3 incident, Vector3 normal)
        {
            return incident - 2.0f * Dot(normal, incident) * normal;
        }

        public static Vector3 Refract(Vector3 incident, Vector3 normal, float index)
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

        public static Vector3 Radians(Vector3 source)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Radians(source[i]);
        	}
        	
        	return result;
        }
        
        public static Vector3 Degrees(Vector3 source)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Degrees(source[i]);
        	}
        	
        	return result;
        }        
        
        public static Vector3 Sin(Vector3 source)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Sin(source[i]);
        	}
        	
        	return result;
        }

        public static Vector3 Cos(Vector3 source)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Cos(source[i]);
        	}
        	
        	return result;
        }

        public static Vector3 Tan(Vector3 source)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Tan(source[i]);
        	}
        	
        	return result;
        }

        public static Vector3 Asin(Vector3 source)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Asin(source[i]);
        	}
        	
        	return result;
        }

        public static Vector3 Acos(Vector3 source)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Acos(source[i]);
        	}
        	
        	return result;
        }

        public static Vector3 Atan(Vector3 source)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Atan(source[i]);
        	}
        	
        	return result;
        }

        #endregion

        #region Экспоненциальные функции

        public static Vector3 Pow(Vector3 left, Vector3 right)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Pow(left[i], right[i]);
        	}
        	
        	return result;
        }

        public static Vector3 Exp(Vector3 source)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Exp(source[i]);
        	}
        	
        	return result;
        }

        public static Vector3 Log(Vector3 source)
        {
        	Vector3 result = Vector3.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Log(source[i]);
        	}
        	
        	return result;
        }

        public static Vector3 Sqrt(Vector3 source)
        {
        	Vector3 result = Vector3.Empty;
        	
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
        	return values[0].GetHashCode() ^ values[1].GetHashCode() ^ values[2].GetHashCode();
        }

        public override bool Equals(object o)
        {
            if (o is Vector3)
            {
                return this == o as Vector3;
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
        	return new float[] { values[0], values[1], values[2] };
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
        
        public float Z
        {
        	get
        	{
        		return values[2];
        	}
        	
        	set
        	{
        		values[2] = value;
        	}
        }                

        public Vector2 XY
        {
        	get
        	{
        		return new Vector2(values[0], values[1]);
        	}
        }
        
        public Vector2 XZ
        {
        	get
        	{
        		return new Vector2(values[0], values[2]);
        	}
        }
        
        public Vector2 YX
        {
        	get
        	{
        		return new Vector2(values[1], values[0]);
        	}
        }
        
        public Vector2 YZ
        {
        	get
        	{
        		return new Vector2(values[1], values[2]);
        	}
        } 
        
        public Vector2 ZX
        {
        	get
        	{
        		return new Vector2(values[2], values[0]);
        	}
        }
        
        public Vector2 ZY
        {
        	get
        	{
        		return new Vector2(values[2], values[1]);
        	}
        }
        
        public static Vector3 Empty
        {
        	get
        	{
        		return new Vector3(0.0f, 0.0f, 0.0f);
        	}
        }
        
        public static Vector3 Epsilon
        {
        	get
        	{
        		return new Vector3(Scalar.Epsilon, Scalar.Epsilon, Scalar.Epsilon);
        	}
        }
        
        public static Vector3 Unit
        {
        	get
        	{
        		return new Vector3(1.0f, 1.0f, 1.0f);
        	}
        }

        public static Vector3 UnitX
        {
        	get
        	{
        		return new Vector3(1.0f, 0.0f, 0.0f);
        	}
        }

        public static Vector3 UnitY
        {
        	get
        	{
        		return new Vector3(0.0f, 1.0f, 0.0f);
        	}
        }   
        
        public static Vector3 UnitZ
        {
        	get
        	{
        		return new Vector3(0.0f, 0.0f, 1.0f);
        	}
        }         
        
        #endregion
    }
}
