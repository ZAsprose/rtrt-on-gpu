/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */

using System;

namespace Base.MathTools
{
    [Serializable]
    public class Matrix3
    {
        #region Private Fields

        private static int size = 3;

        private float[,] values = new float[size, size];

        #endregion

        #region Constructor

        public Matrix3(float diagonal)
        {
            for (int i = 0; i < size; i++)
            {
                values[i, i] = diagonal;
            }
        }

        public Matrix3(float[] diagonal)
        {
            for (int i = 0; i < size; i++)
            {
                values[i, i] = diagonal[i];
            }
        }

        public Matrix3(Vector3 diagonal)
        {
            for (int i = 0; i < size; i++)
            {
                values[i, i] = diagonal[i];
            }
        }
        
        public Matrix3(float[,] matrix)
        {
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    values[i, j] = matrix[i, j];
                }
            }
        }

        public Matrix3(Matrix3 matrix)
        {
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    values[i, j] = matrix[i, j];
                }
            }
        }

        #endregion

        #region Operators

        public static Matrix3 operator -(Matrix3 source)
        {
            float[,] result = new float[size, size];

            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    result[i, j] = -source[i, j];
                }
            }

            return new Matrix3(result);
        }

        public static Matrix3 operator -(Matrix3 left, Matrix3 right)
        {
            float[,] result = new float[size, size];

            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    result[i, j] = left[i, j] - right[i, j];
                }
            }

            return new Matrix3(result);
        }

        public static Matrix3 operator +(Matrix3 left, Matrix3 right)
        {
            float[,] result = new float[size, size];

            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    result[i, j] = left[i, j] + right[i, j];
                }
            }

            return new Matrix3(result);
        }

        public static Matrix3 operator *(Matrix3 left, float right)
        {
            float[,] result = new float[size, size];

            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    result[i, j] = left[i, j] * right;
                }
            }

            return new Matrix3(result);
        }

        public static Matrix3 operator *(Matrix3 left, Matrix3 right)
        {
            float[,] result = new float[size, size];

            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    result[i, j] = 0;

                    for (int k = 0; k < size; k++)
                    {
                        result[i, j] += left[i, k] * right[k, j];
                    }
                }
            }

            return new Matrix3(result);
        }

        public static Vector3 operator *(Matrix3 left, Vector3 right)
        {
            float[] result = new float[size];

            for (int i = 0; i < size; i++)
            {
                result[i] = 0;

                for (int k = 0; k < size; k++)
                {
                    result[i] += left[i, k] * right[k];
                }
            }

            return new Vector3(result);
        }

        public static Matrix3 operator /(Matrix3 left, float right)
        {
            float[,] result = new float[size, size];

            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    result[i, j] = left[i, j] / right;
                }
            }

            return new Matrix3(result);
        }

        public float this[int i, int j]
        {
            get
            {
                return values[i, j];
            }

            set
            {
                values[i, j] = value;
            }
        }        
        
        #endregion

        #region Public Methods
        
        #region Общие функции        
        
        public static Matrix3 Inverse(Matrix3 source)
        {
            Matrix3 result = Matrix3.Empty;

            for (int k = 0; k < size; k++)
            {
                for (int i = 0; i < size; i++)
                {
                    for (int j = 0; j < size; j++)
                    {
                        if (i == k)
                        {
                            if (j == k)
                            {
                                result[i, j] = 1.0f / source[i, j];
                            }
                            else
                            {
                                result[i, j] = -source[i, j] / source[k, k];
                            }
                        }
                        else
                        {
                            if (j == k)
                            {
                                result[i, j] = source[i, k] / source[k, k];
                            }
                            else
                            {
                                result[i, j] = source[i, j] - source[k, j] * source[i, k] / source[k, k];
                            }
                        }
                    }
                }
            }

            return result;
        }

        public static Matrix3 Transpose(Matrix3 source)
        {
            Matrix3 matrix = Matrix3.Identity;

            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    matrix[i, j] = source[j, i];
                }
            }

            return matrix;
        }

        #endregion
        
        #region Функции для преобразования координат
        
        public static Matrix3 Scale(float factor)
        {
            return new Matrix3(factor);
        }
        
        public static Matrix3 Scale(Vector3 factors)
        {
        	return new Matrix3(factors);
        }

        public static Matrix3 Rotate(Vector3 orientation)
        {
            return RotateAroundZ(orientation.Z) * RotateAroundY(orientation.Y) * RotateAroundX(orientation.X);
        }
        
        public static Matrix3 RotateAroundX(float angle)
        {
            Matrix3 matrix = Matrix3.Identity;

            float cos = (float) Math.Cos(angle);
            float sin = (float) Math.Sin(angle);

            matrix[1, 1] = cos;    matrix[1, 2] = sin;
            matrix[2, 1] = -sin;   matrix[2, 2] = cos;

            return matrix;
        }

        public static Matrix3 RotateAroundY(float angle)
        {
            Matrix3 matrix = Matrix3.Identity;

            float cos = (float) Math.Cos(angle);
            float sin = (float) Math.Sin(angle);

            matrix[0, 0] = cos;   matrix[0, 2] = -sin;
            matrix[2, 0] = sin;   matrix[2, 2] = cos;

            return matrix;
        }

        public static Matrix3 RotateAroundZ(float angle)
        {
            Matrix3 matrix = Matrix3.Identity;

            float cos = (float) Math.Cos(angle);
            float sin = (float) Math.Sin(angle);

            matrix[0, 0] = cos;    matrix[0, 1] = sin;
            matrix[1, 0] = -sin;   matrix[1, 1] = cos;

            return matrix;
        }

        #endregion
        
        #region Дополнительные функции
        
        public float[] ToArray()
        {
        	float[] result = new float[values.Length];  

            for (int i = 0; i < size; i++)
            {
            	for (int j = 0; j < size; j++)
                {
                    result[i * size + j] = values[i, j];
                }
            }

            return result;
        }
        
        public override string ToString()
		{
			string result = "[ ";
			
			for (int row = 0; row < size; row++)
			{
				result += "( ";
				
				for (int col = 0; col < size; col++)
				{
					result += values[row, col] + " ";
				}
				
				result += ") ";
			}
			
			result += "]";
			
			return result;
		}

        #endregion
        
        #endregion
        
        #region Properties

        public static Matrix3 Empty
        {
            get
            {
                return new Matrix3(0.0f);
            }
        }

        public static Matrix3 Identity
        {
            get
            {
                return new Matrix3(1.0f);
            }
        }

        #endregion
    }
}
