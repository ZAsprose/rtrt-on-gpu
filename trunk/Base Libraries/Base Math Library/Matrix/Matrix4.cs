/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */
 
using System;

namespace Base.MathTools
{
    [Serializable]
    public class Matrix4
    {
        #region Private Fields

        private static int size = 4;

        private float[,] values = new float[size, size];

        #endregion

        #region Constructor

        public Matrix4(float value)
        {
            for (int i = 0; i < size; i++)
            {
                values[i, i] = value;
            }
        }

        public Matrix4(float[] diagonal)
        {
            for (int i = 0; i < size; i++)
            {
                values[i, i] = diagonal[i];
            }
        }

        public Matrix4(Vector4 diagonal)
        {
            for (int i = 0; i < size; i++)
            {
                values[i, i] = diagonal[i];
            }
        }        
        
        public Matrix4(float[,] matrix)
        {
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    values[i, j] = matrix[i, j];
                }
            }
        }

        public Matrix4(Matrix4 matrix)
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

        public static Matrix4 operator -(Matrix4 source)
        {
            float[,] result = new float[size, size];

            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    result[i, j] = -source[i, j];
                }
            }

            return new Matrix4(result);
        }

        public static Matrix4 operator -(Matrix4 left, Matrix4 right)
        {
            float[,] result = new float[size, size];

            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    result[i, j] = left[i, j] - right[i, j];
                }
            }

            return new Matrix4(result);
        }

        public static Matrix4 operator +(Matrix4 left, Matrix4 right)
        {
            float[,] result = new float[size, size];

            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    result[i, j] = left[i, j] + right[i, j];
                }
            }

            return new Matrix4(result);
        }

        public static Matrix4 operator *(Matrix4 left, float right)
        {
            float[,] result = new float[size, size];

            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    result[i, j] = left[i, j] * right;
                }
            }

            return new Matrix4(result);
        }

        public static Matrix4 operator *(Matrix4 left, Matrix4 right)
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

            return new Matrix4(result);
        }

        public static Vector4 operator *(Matrix4 left, Vector4 right)
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

            return new Vector4(result);
        }

        public static Matrix4 operator /(Matrix4 left, float right)
        {
            float[,] result = new float[size, size];

            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    result[i, j] = left[i, j] / right;
                }
            }

            return new Matrix4(result);
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
        
        public static Matrix4 Inverse(Matrix4 source)
        {
            Matrix4 result = Matrix4.Empty;

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

        public static Matrix4 Transpose(Matrix4 source)
        {
            Matrix4 matrix = Matrix4.Empty;

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

        public static Matrix4 Empty
        {
            get
            {
                return new Matrix4(0.0f);
            }
        }

        public static Matrix4 Identity
        {
            get
            {
                return new Matrix4(1.0f);
            }
        }

        #endregion
    }
}
