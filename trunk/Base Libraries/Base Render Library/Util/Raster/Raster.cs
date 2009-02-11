/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */
 
using System;
using System.Collections.Generic;
using System.Drawing.Imaging;
using System.Drawing;

using Base.MathTools;

namespace Base.Graphics
{
    /// <summary> Простой класс для представления растра. </summary>
    [Serializable]
    public class Raster
    {
        #region Private Fields

        private Vector3[,] pixels = null;

        #endregion

        #region Constructor

        public Raster(int width, int height)
        {
            pixels = new Vector3[width, height];

            for (int j = 0; j < height; j++)
            {
                for (int i = 0; i < width; i++)
                {
                    pixels[i, j] = Vector3.Empty;
                }
            }
        }

        public Raster(string fileName)
        {
            Bitmap image = new Bitmap(fileName);
            
            pixels = new Vector3[image.Width, image.Height];

            for (int j = 0; j < image.Height; j++)
            {
                for (int i = 0; i < image.Width; i++)
                {
                    Color color = image.GetPixel(i, j);

                    pixels[i, j] = new Vector3(color.R / 255f, color.G / 255f, color.B / 255f);
                }
            }
        }

        #endregion

        #region Public Methods

        public Bitmap ToBitmap()
        {
            Bitmap bitmap = new Bitmap(Width, Height);

            for (int j = 0; j < Height; j++)
            {
                for (int i = 0; i < Width; i++)
                {
                    bitmap.SetPixel(i, j,
                                    Color.FromArgb((int)(255f * pixels[i, j].X),
                                                   (int)(255f * pixels[i, j].Y),
                                                   (int)(255f * pixels[i, j].Z)));
                }
            }

            return bitmap;
        }

        public Texture2D ToTexture()
        {
        	List<float> data = new List<float>();

            for (int j = Height-1; j > -1; j--)
            {
                for (int i = 0; i < Width; i++)
                {
	                for (int k = 0; k < 3; k++)
	                {
	                	data.Add(pixels[i, j][k]);
	                }
                }
            }
            
            Texture2D texture = new Texture2D(Width, Height);

            //texture.Create(data.ToArray());
            
            return texture;
        }
        
        public void Clamp()
        {
        	for (int j = 0; j < Height; j++)
            {
                for (int i = 0; i < Width; i++)
                {
                	pixels[i, j] =  Vector3.Clamp(pixels[i, j], 0.0f, 1.0f);
                }
            }
        }        
        
        public void Normalize()
        {
        	float length = 0.0f;
        	
            for (int j = 0; j < Height; j++)
            {
                for (int i = 0; i < Width; i++)
                {
                	length = Math.Max(Vector3.Length(pixels[i, j]), length);
                }
            }
            
            for (int j = 0; j < Height; j++)
            {
                for (int i = 0; i < Width; i++)
                {
                	pixels[i, j] /= length;
                }
            }
        }      
        
        #endregion

        #region Properties

        public int Width
        {
            get
            {
                return pixels.GetLength(0);
            }
        }

        public int Height
        {
            get
            {
                return pixels.GetLength(1);
            }
        }

        public Vector3 this[int i, int j]
        {
            get
            {
                return pixels[i, j];
            }

            set
            {
                pixels[i, j] = value;
            }
        }

        #endregion
    }
}