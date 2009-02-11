/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */
 
using System;
using Base.MathTools;
using Tao.OpenGl;

namespace Base.Graphics
{
    [Serializable]
    public class Camera
    {
        #region Public Fields       
        
        public int Width = 512;

        public int Height = 512;

        public float FieldOfView = 45.0f;  

        #endregion
        
        #region Private Fields
        
        #region Положение и ориентация камеры

        private Vector3 position = new Vector3(20.0f, 0.0f, 0.0f);
        
        private Vector3 orientation = Vector3.Empty;

        #endregion        
        
        #region Стандартные направления камеры
        
        private Vector3 upDirection = Vector3.UnitZ;

        private Vector3 rightDirection = Vector3.UnitY;

        private Vector3 viewDirection = -Vector3.UnitX;

        #endregion
        
        #endregion

        #region Constructor

        public Camera() { }

        public Camera(Vector3 position)
        {
            Position = position;
        }

        public Camera(Vector3 position, Vector3 orientation)
            : this(position)
        {
            Orientation = orientation;
        }

        #endregion

        #region Private Methods

        private void UpdateDirections()
        {
            Matrix3 rotation = Matrix3.Rotate(orientation);

            viewDirection = -rotation * Vector3.UnitX;
            
            upDirection = rotation * Vector3.UnitZ;
            
            rightDirection = rotation * Vector3.UnitY;
        }

        #endregion

        #region Public Methods

        public void MoveStraight(float delta)
        {
            Position += viewDirection * delta;
        }

        public void MoveHorizontal(float delta)
        {
            Position += rightDirection * delta;
        }

        public void MoveVertical(float delta)
        {
            Position += upDirection * delta;
        }
        
        public void Rotate(Vector3 delta)
        {
            Orientation += delta;
        }
        
        public void Setup()
        {
            Gl.glMatrixMode(Gl.GL_PROJECTION); 
            
            Gl.glLoadIdentity(); 
            
            Glu.gluPerspective(FieldOfView, Aspect, 1.0f, 1000.0f);
            
         	Glu.gluLookAt(position.X, position.Y, position.Z,
                          position.X + ViewDirection.X,
                          position.Y + ViewDirection.Y,
                          position.Z + ViewDirection.Z,
                          UpDirection.X, UpDirection.Y, UpDirection.Z);
            
            Gl.glMatrixMode(Gl.GL_MODELVIEW);
        }
        
        public void SetShaderData(ShaderProgram program)
        {
        	program.SetUniformVector("Camera.Position", position);
        	
        	program.SetUniformVector("Camera.View", viewDirection);

        	program.SetUniformVector("Camera.Right", rightDirection * ScreenScale.X);
        	
        	program.SetUniformVector("Camera.Up", upDirection * ScreenScale.Y);        	        	
        	
        	program.SetUniformVector("Camera.PixelSize", PixelSize);
        }       
        
        #endregion

        #region Properties
        
        public float Aspect
        {
        	get
        	{
        		return Width / (float) Height;
        	}
        }

        public Vector2 ScreenScale
        {
        	get
        	{
        		return new Vector2((float) Math.Tan(Aspect * FieldOfView / 2.0), (float) Math.Tan(FieldOfView / 2.0));
        	}
        }
        
        public Vector2 PixelSize
        {
        	get        		
        	{
        		return new Vector2(1.0f / Width, 1.0f / Height);
        	}
        }        
        
        public Vector3 Position
        {
        	get
        	{
        		return position;
        	}
        	
        	set
        	{
        		position = value;
        	}
        }

        public Vector3 Orientation
        {
            get
            {
                return orientation;
            }
            
            set
            {
            	orientation = value;
            	
            	UpdateDirections();
            }
        }

        public Vector3 ViewDirection
        {
            get
            {
                return viewDirection;
            }
        }

        public Vector3 UpDirection
        {
            get
            {
                return upDirection;
            }
        }

        public Vector3 RightDirection
        {
            get
            {
                return rightDirection;
            }
        }
        
        #endregion
    }
}