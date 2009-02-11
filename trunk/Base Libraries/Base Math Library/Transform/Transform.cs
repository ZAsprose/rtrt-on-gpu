/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */

using System;
using Base.MathTools;

namespace Base.MathTools
{
    [Serializable]
    public class Transform
    {
        #region Private Fields
        
        #region Параметры преобразования        
        
        private Vector3 translation = Vector3.Empty;

        private Vector3 orientation = Vector3.Empty;
        
        private Vector3 scale = Vector3.Unit;   

        #endregion
        
        #region Матрицы прямого преобразования

        private Matrix3 forwardRotation = Matrix3.Identity;
        
        private Matrix3 forwardScaling = Matrix3.Identity;
        
        #endregion
        
        #region Матрицы обратного преобразования        
        
        private Matrix3 backwardRotation = Matrix3.Identity;
        
        private Matrix3 backwardScaling = Matrix3.Identity;          

        #endregion
        
        #endregion

        #region Constructor

        public Transform() { }
        
        public Transform(Vector3 translation)
        {
        	Translation = translation;
        }
        
        public Transform(Vector3 translation, Vector3 orientation)
        	: this(translation)
        {
        	Orientation = orientation;
        }
        
        public Transform(Vector3 translation, Vector3 orientation, Vector3 scale)
        	: this(translation, orientation)
        {
        	Scale = scale;
        }        

        #endregion
        
        #region Public Methods
        
        public Vector3 ForwardPoint(Vector3 position)
        {
        	return forwardRotation * forwardScaling * position + translation;
        }
        
        public Vector3 ForwardVector(Vector3 direction)
        {
        	return forwardRotation * forwardScaling * direction;
        }
        
        public Vector3 ForwardNormal(Vector3 normal)
        {        	
        	return forwardRotation * backwardScaling * normal;
        }         
        
        public Vector3 BackwardPoint(Vector3 position)
        {
        	return backwardScaling * backwardRotation * (position - translation);
        }
        
        public Vector3 BackwardVector(Vector3 direction)
        {
        	return backwardScaling * backwardRotation * direction;
        }               
        
        #endregion
        
        #region Properties
        
        public Vector3 Translation
        {
        	get
        	{
        		return translation;
        	}
        	
        	set
        	{
        		translation = value;
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

	        	forwardRotation = Matrix3.Rotate(orientation);
	        	
	        	backwardRotation = Matrix3.Inverse(forwardRotation);
            }
        }
        
        public Vector3 Scale
        {
            get
            {
                return scale;
            }

            set
            {
                scale = value;

	        	forwardScaling = Matrix3.Scale(scale);
	        	
	        	backwardScaling = Matrix3.Inverse(forwardScaling);                             
            }
        }        

        #endregion        
    }
}