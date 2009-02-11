/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */

using System;

namespace Base.MathTools
{
    [Serializable]
    public class Vector4
    {
        #region Private Fields

        private static readonly int size = 4;

        private float[] values = new float[size];

        #endregion

        #region Constructor
        
        public Vector4(float x, float y, float z, float w)
        {
        	values = new float[] { x, y, z, w };
        }        
        
        public Vector4(Vector2 vector, float z, float w)
        {
        	values = new float[] { vector.X, vector.Y, z, w };
        }

        public Vector4(Vector3 vector, float w)
        {
        	values = new float[] { vector.X, vector.Y, vector.Z, w };
        }
        
        public Vector4(Vector4 vector)
        {
        	values = new float[] { vector.X, vector.Y, vector.Z, vector.W };
        }        

        public Vector4(float[] vector)
        {
        	for (int i = 0; i < size; i++)
        	{
        		values[i] = vector[i];
        	}
        }
        
        #endregion

        #region Operators

        public static Vector4 operator -(Vector4 source)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = -source[i];
        	}
        	
        	return result;
        }

        public static Vector4 operator +(Vector4 left, Vector4 right)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] + right[i];
        	}
        	
        	return result;
        }

        public static Vector4 operator -(Vector4 left, Vector4 right)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] - right[i];
        	}
        	
        	return result;
        }

        public static Vector4 operator *(float left, Vector4 right)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left * right[i];
        	}
        	
        	return result;
        }

        public static Vector4 operator *(Vector4 left, float right)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] * right;
        	}
        	
        	return result;
        }

        public static Vector4 operator *(Vector4 left, Vector4 right)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] * right[i];
        	}
        	
        	return result;
        }

        public static Vector4 operator /(Vector4 left, float right)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] / right;
        	}
        	
        	return result;
        }

        public static Vector4 operator /(Vector4 left, Vector4 right)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] / right[i];
        	}
        	
        	return result;
        }

        public static bool operator <(Vector4 left, Vector4 right)
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
        
        public static bool operator <(Vector4 left, float right)
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

        public static bool operator >(Vector4 left, Vector4 right)
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
        
        public static bool operator >(Vector4 left, float right)
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

        public static bool operator <=(Vector4 left, Vector4 right)
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
        
        public static bool operator <=(Vector4 left, float right)
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

        public static bool operator >=(Vector4 left, Vector4 right)
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
        
        public static bool operator >=(Vector4 left, float right)
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

        public static bool operator !=(Vector4 left, Vector4 right)
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

        public static bool operator ==(Vector4 left, Vector4 right)
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
        
        public static implicit operator float[](Vector4 source)
        {
            return source.values;
        }
        
        #endregion

        #region Public Methods

        #region Общие функции

        public static Vector4 Abs(Vector4 source)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Math.Abs(source[i]);
        	}        	
        	
            return result;
        }

        public static Vector4 Sign(Vector4 source)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Math.Sign(source[i]);
        	}        	
        	
            return result;
        }

        public static Vector4 Floor(Vector4 source)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Floor(source[i]);
        	}        	
        	
            return result;
        }

        public static Vector4 Fract(Vector4 source)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = source[i] - (float) Math.Floor(source[i]);
        	}        	
        	
            return result;
        }

        public static Vector4 Ceiling(Vector4 source)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Ceiling(source[i]);
        	}        	
        	
            return result;
        }

        public static Vector4 Mod(Vector4 source, float value)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = source[i] - value * (float) Math.Floor(source[i] / value);
        	}
        	
        	return result;
        }

        public static Vector4 Mod(Vector4 left, Vector4 right)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] - right[i] * (float) Math.Floor(left[i] / right[i]);
        	}        	
        	
            return result;
        }

        public static Vector4 Min(Vector4 source, float value)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Math.Min(source[i], value);
        	}
        	
        	return result;
        }

        public static Vector4 Min(Vector4 left, Vector4 right)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Math.Min(left[i], right[i]);
        	}
        	
        	return result;
        }

        public static Vector4 Max(Vector4 source, float value)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Math.Max(source[i], value);
        	}
        	
        	return result;
        }

        public static Vector4 Max(Vector4 left, Vector4 right)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Math.Max(left[i], right[i]);
        	}
        	
        	return result;
        }

        public static Vector4 Clamp(Vector4 source, float min, float max)
        {
        	Vector4 result = new Vector4(source);
        	
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
        
        public static Vector4 Clamp(Vector4 source, Vector4 min, Vector4 max)
        {
        	Vector4 result = new Vector4(source);
        	
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

        public static Vector4 Mix(Vector4 left, Vector4 right, float value)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] * (1.0f - value) + right[i] * value;
        	}
        	
        	return result;
        }

        public static Vector4 Mix(Vector4 left, Vector4 right, Vector4 value)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = left[i] * (1.0f - value[i]) + right[i] * value[i];
        	}
        	
        	return result;
        }
        
        public static Vector4 Step(Vector4 source, float value)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Step(source[i], value);
        	}
        	
        	return result;
        }

        public static Vector4 Step(Vector4 source, Vector4 value)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Step(source[i], value[i]);
        	}
        	
        	return result;
        }
        
        public static Vector4 Smooth(Vector4 source, float left, float right)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Smooth(source[i], left, right);
        	}
        	
        	return result;
        }

        public static Vector4 Smooth(Vector4 source, Vector4 left, Vector4 right)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Smooth(source[i], left[i], right[i]);
        	}
        	
        	return result;
        }
        
        #endregion

        #region Геометрические функции

        public static float Length(Vector4 source)
        {
        	float result = 0.0f;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result += source[i] * source[i];
        	}
        	
        	return (float) Math.Sqrt(result);
        }

        public static float Square(Vector4 source)
        {
        	float result = 0.0f;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result += source[i] * source[i];
        	}
        	
        	return result;
        }

        public static float Distance(Vector4 left, Vector4 right)
        {
            return Length(left - right);
        }

        public static float Dot(Vector4 left, Vector4 right)
        {
        	float result = 0.0f;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result += left[i] * right[i];
        	}
        	
        	return result;
        }        
        
        public static Vector4 Normalize(Vector4 source)
        {
            return source / Length(source);
        }

        public static Vector4 Reflect(Vector4 incident, Vector4 normal)
        {
            return incident - 2.0f * Dot(normal, incident) * normal;
        }

        public static Vector4 Refract(Vector4 incident, Vector4 normal, float index)
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

        public static Vector4 Radians(Vector4 source)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Radians(source[i]);
        	}
        	
        	return result;
        }
        
        public static Vector4 Degrees(Vector4 source)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = Scalar.Degrees(source[i]);
        	}
        	
        	return result;
        }        
        
        public static Vector4 Sin(Vector4 source)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Sin(source[i]);
        	}
        	
        	return result;
        }

        public static Vector4 Cos(Vector4 source)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Cos(source[i]);
        	}
        	
        	return result;
        }

        public static Vector4 Tan(Vector4 source)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Tan(source[i]);
        	}
        	
        	return result;
        }

        public static Vector4 Asin(Vector4 source)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Asin(source[i]);
        	}
        	
        	return result;
        }

        public static Vector4 Acos(Vector4 source)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Acos(source[i]);
        	}
        	
        	return result;
        }

        public static Vector4 Atan(Vector4 source)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Atan(source[i]);
        	}
        	
        	return result;
        }

        #endregion

        #region Экспоненциальные функции

        public static Vector4 Pow(Vector4 left, Vector4 right)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Pow(left[i], right[i]);
        	}
        	
        	return result;
        }

        public static Vector4 Exp(Vector4 source)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Exp(source[i]);
        	}
        	
        	return result;
        }

        public static Vector4 Log(Vector4 source)
        {
        	Vector4 result = Vector4.Empty;
        	
        	for (int i = 0; i < size; i++)
        	{
        		result[i] = (float) Math.Log(source[i]);
        	}
        	
        	return result;
        }

        public static Vector4 Sqrt(Vector4 source)
        {
        	Vector4 result = Vector4.Empty;
        	
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
        	return values[0].GetHashCode() ^ values[1].GetHashCode() ^
        		   values[2].GetHashCode() ^ values[3].GetHashCode();
        }

        public override bool Equals(object o)
        {
            if (o is Vector4)
            {
                return this == o as Vector4;
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
        	return new float[] { values[0], values[1], values[2], values[3] };
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
        
        public float W
        {
        	get
        	{
        		return values[3];
        	}
        	
        	set
        	{
        		values[3] = value;
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
        
        public Vector2 XW
        {
        	get
        	{
        		return new Vector2(values[0], values[3]);
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
        
        public Vector2 YW
        {
        	get
        	{
        		return new Vector2(values[1], values[3]);
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
        
        public Vector2 ZW
        {
        	get
        	{
        		return new Vector2(values[2], values[3]);
        	}
        }
        
        public Vector2 WX
        {
        	get
        	{
        		return new Vector2(values[3], values[0]);
        	}
        }
        
        public Vector2 WY
        {
        	get
        	{
        		return new Vector2(values[3], values[1]);
        	}
        } 
        
        public Vector2 WZ
        {
        	get
        	{
        		return new Vector2(values[3], values[2]);
        	}
        }    
        
        public Vector3 XYZ
        {
        	get
        	{
        		return new Vector3(values[0], values[1], values[2]);
        	}
        }
        
        public Vector3 XZY
        {
        	get
        	{
        		return new Vector3(values[0], values[2], values[1]);
        	}
        }        
        
        public Vector3 YXZ
        {
        	get
        	{
        		return new Vector3(values[1], values[0], values[2]);
        	}
        } 
        
        public Vector3 YZX
        {
        	get
        	{
        		return new Vector3(values[1], values[2], values[0]);
        	}
        }        
        
        public Vector3 ZXY
        {
        	get
        	{
        		return new Vector3(values[2], values[0], values[1]);
        	}
        }     
        
        public Vector3 ZYX
        {
        	get
        	{
        		return new Vector3(values[2], values[1], values[0]);
        	}
        }         
        
        public Vector3 XYW
        {
        	get
        	{
        		return new Vector3(values[0], values[1], values[3]);
        	}
        }
        
        public Vector3 XWY
        {
        	get
        	{
        		return new Vector3(values[0], values[3], values[1]);
        	}
        }        
        
        public Vector3 YXW
        {
        	get
        	{
        		return new Vector3(values[1], values[0], values[3]);
        	}
        } 
        
        public Vector3 YWX
        {
        	get
        	{
        		return new Vector3(values[1], values[3], values[0]);
        	}
        }        
        
        public Vector3 WXY
        {
        	get
        	{
        		return new Vector3(values[3], values[0], values[1]);
        	}
        }     
        
        public Vector3 WYX
        {
        	get
        	{
        		return new Vector3(values[3], values[1], values[0]);
        	}
        }
        
        public Vector3 XWZ
        {
        	get
        	{
        		return new Vector3(values[0], values[3], values[2]);
        	}
        }
        
        public Vector3 XZW
        {
        	get
        	{
        		return new Vector3(values[0], values[2], values[3]);
        	}
        }        
        
        public Vector3 WXZ
        {
        	get
        	{
        		return new Vector3(values[3], values[0], values[2]);
        	}
        } 
        
        public Vector3 WZX
        {
        	get
        	{
        		return new Vector3(values[3], values[2], values[0]);
        	}
        }        
        
        public Vector3 ZXW
        {
        	get
        	{
        		return new Vector3(values[2], values[0], values[3]);
        	}
        }     
        
        public Vector3 ZWX
        {
        	get
        	{
        		return new Vector3(values[2], values[3], values[0]);
        	}
        }
        
        public Vector3 WYZ
        {
        	get
        	{
        		return new Vector3(values[3], values[1], values[2]);
        	}
        }
        
        public Vector3 WZY
        {
        	get
        	{
        		return new Vector3(values[3], values[2], values[1]);
        	}
        }        
        
        public Vector3 YWZ
        {
        	get
        	{
        		return new Vector3(values[1], values[3], values[2]);
        	}
        } 
        
        public Vector3 YZW
        {
        	get
        	{
        		return new Vector3(values[1], values[2], values[3]);
        	}
        }        
        
        public Vector3 ZWY
        {
        	get
        	{
        		return new Vector3(values[2], values[3], values[1]);
        	}
        }     
        
        public Vector3 ZYW
        {
        	get
        	{
        		return new Vector3(values[2], values[1], values[3]);
        	}
        }         
        
        public static Vector4 Empty
        {
        	get
        	{
        		return new Vector4(0.0f, 0.0f, 0.0f, 0.0f);
        	}
        }

        public static Vector4 Epsilon
        {
        	get
        	{
        		return new Vector4(Scalar.Epsilon, Scalar.Epsilon, Scalar.Epsilon, Scalar.Epsilon);
        	}
        }
        
        public static Vector4 Unit
        {
        	get
        	{
        		return new Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        	}
        }

        public static Vector4 UnitX
        {
        	get
        	{
        		return new Vector4(1.0f, 0.0f, 0.0f, 0.0f);
        	}
        }

        public static Vector4 UnitY
        {
        	get
        	{
        		return new Vector4(0.0f, 1.0f, 0.0f, 0.0f);
        	}
        }   
        
        public static Vector4 UnitZ
        {
        	get
        	{
        		return new Vector4(0.0f, 0.0f, 1.0f, 0.0f);
        	}
        } 
        
        public static Vector4 UnitW
        {
        	get
        	{
        		return new Vector4(0.0f, 0.0f, 0.0f, 1.0f);
        	}
        }         
        
        #endregion
    }
}