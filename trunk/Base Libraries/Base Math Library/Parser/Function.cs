/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */

using System;
using System.Collections.Generic;

namespace Base.MathTools
{
    [Serializable]
    public abstract class BaseFunction
    {
    	#region Operators
        
        public static BaseFunction operator +(BaseFunction left, BaseFunction right)
        {
            return new AddFunction(left, right);
        }

        public static BaseFunction operator -(BaseFunction left, BaseFunction right)
        {
            return new SubFunction(left, right);
        }

        public static BaseFunction operator *(BaseFunction left, BaseFunction right)
        {
            return new MulFunction(left, right);
        }

        public static BaseFunction operator /(BaseFunction left, BaseFunction right)
        {
            return new DivFunction(left, right);
        }

        public static BaseFunction operator -(BaseFunction funct)
        {
            return new NegativeFunction(funct);
        }
        
        #endregion        
        
    	#region Public Methods
    	
        public abstract float CalcFunction(Dictionary<string, float> arg);

        #endregion       
    }
    
    [Serializable]
    public class AddFunction : BaseFunction
    {
    	#region Public Fields
    	
        public BaseFunction Left = null;
        
        public BaseFunction Right = null;

        #endregion
        
        #region Constructor
        
        public AddFunction(BaseFunction left, BaseFunction right)
        {
            Left = left;
            Right = right;
        }
        
        #endregion
        
        #region Public Methods

        public override float CalcFunction(Dictionary<string, float> arg)
        {
            return Left.CalcFunction(arg) + Right.CalcFunction(arg);
        }
        
        #endregion
    }

    [Serializable]
    public class SubFunction : BaseFunction
    {
    	#region Public Fields
    	
        public BaseFunction Left = null;
        
        public BaseFunction Right = null;

        #endregion
        
        #region Constructor
        
        public SubFunction(BaseFunction left, BaseFunction right)
        {
            Left = left;
            Right = right;
        }
        
        #endregion

        #region Public Methods
        
        public override float CalcFunction(Dictionary<string, float> arg)
        {
            return Left.CalcFunction(arg) - Right.CalcFunction(arg);
        }       
        
        #endregion
    }

    [Serializable]
    public class MulFunction : BaseFunction
    {
    	#region Public Fields
    	
        public BaseFunction Left = null;
        
        public BaseFunction Right = null;

        #endregion
        
        #region Constructor
        
        public MulFunction(BaseFunction left, BaseFunction right)
        {
            Left = left;
            Right = right;
        }

        #endregion
        
        #region Public Methods
        
        public override float CalcFunction(Dictionary<string, float> arg)
        {
            return Left.CalcFunction(arg) * Right.CalcFunction(arg);
        }
        
        #endregion
    }
    
    [Serializable]
    public class DivFunction : BaseFunction
    {
        #region Public Fields
        
    	public BaseFunction Left = null;
        
        public BaseFunction Right = null;

        #endregion
        
        #region Constructor
                
        public DivFunction(BaseFunction left, BaseFunction right)
        {
            Left = left;
            Right = right;
        }
        
        #endregion

        #region Public Methods
        
        public override float CalcFunction(Dictionary<string, float> arg)
        {
            return Left.CalcFunction(arg) / Right.CalcFunction(arg);
        }
        
        #endregion
    }

    [Serializable]
    public class NegativeFunction : BaseFunction
    {
    	#region Public Fields
    	
        public BaseFunction Source = null;
        
        #endregion

        #region Constructor
        
        public NegativeFunction(BaseFunction source)
        {
            Source = source;
        }
        
        #endregion

        #region Public Methods
        
        public override float CalcFunction(Dictionary<string, float> arg)
        {
            return -Source.CalcFunction(arg);
        }
        
        #endregion
    }

    [Serializable]
    public class ReverseFunction : BaseFunction
    {
    	#region Public Fields
    	   	
        public BaseFunction Source = null;
        
        #endregion 

        #region Constructor
              
        public ReverseFunction(BaseFunction source)
        {
            Source = source;
        }
        
        #endregion  
        
        #region Public Methods
        
        public override float CalcFunction(Dictionary<string, float> arg)
        {
            return 1.0f / Source.CalcFunction(arg);
        }
        
        #endregion
    }

    [Serializable]
    public class SinFunction : BaseFunction
    {
    	#region Public Fields
    	   	
        public BaseFunction Source = null;
        
        #endregion 

        #region Constructor
              
        public SinFunction(BaseFunction source)
        {
            Source = source;
        }
        
        #endregion  

        #region Public Methods
        
        public override float CalcFunction(Dictionary<string, float> arg)
        {
            return Convert.ToSingle(Math.Sin(Source.CalcFunction(arg)));
        }
        
        #endregion
    }

    [Serializable]
    public class CosFunction : BaseFunction
    {
    	#region Public Fields
    	  	
        public BaseFunction Source = null;
        
        #endregion  

        #region Constructor
              
        public CosFunction(BaseFunction source)
        {
            Source = source;
        }
        
        #endregion  
        
        #region Public Methods       

        public override float CalcFunction(Dictionary<string, float> arg)
        {
        	return Convert.ToSingle(Math.Cos(Source.CalcFunction(arg)));
        }
        
        #endregion
    }

    [Serializable]
    public class ExpFunction : BaseFunction
    {
    	#region Public Fields
    	   	
        public BaseFunction Source = null;
        
        #endregion 

        #region Constructor
               
        public ExpFunction(BaseFunction source)
        {
            Source = source;
        }
        
        #endregion 

        #region Public Methods
        
        public override float CalcFunction(Dictionary<string, float> arg)
        {
        	return Convert.ToSingle(Math.Exp(Source.CalcFunction(arg)));
        }
        
        #endregion
    }

    [Serializable]
    public class LogFunction : BaseFunction
    {
    	#region Public Fields
    	   	
        public BaseFunction Source = null;
        
        #endregion 

        #region Constructor
                
        public LogFunction(BaseFunction source)
        {
            Source = source;
        }
        
        #endregion

        #region Public Methods
        
        public override float CalcFunction(Dictionary<string, float> arg)
        {
        	return Convert.ToSingle(Math.Log(Source.CalcFunction(arg)));
        }
        
        #endregion
    }

    [Serializable]
    public class SqrtFunction : BaseFunction
    {
    	#region Public Fields
    	   	
        public BaseFunction Source = null;
        
        #endregion 

        #region Constructor
               
        public SqrtFunction(BaseFunction source)
        {
            Source = source;
        }
        
        #endregion 

        #region Public Methods
        
        public override float CalcFunction(Dictionary<string, float> arg)
        {
        	return Convert.ToSingle(Math.Sqrt(Source.CalcFunction(arg)));
        }
        
        #endregion
    }

    [Serializable]
    public class AbsFunction : BaseFunction
    {
    	#region Public Fields
    	   	
        public BaseFunction Source = null;
        
        #endregion 

        #region Constructor
             
        public AbsFunction(BaseFunction source)
        {
            Source = source;
        }
        
        #endregion   

        #region Public Methods
        
        public override float CalcFunction(Dictionary<string, float> arg)
        {
            return Math.Abs(Source.CalcFunction(arg));
        }
        
        #endregion
    }

    [Serializable]
    public class ConstFunction : BaseFunction
    {
    	#region Public Fields
    	   	
        public float Value = 0.0f;
        
        public static BaseFunction PI = new ConstFunction(Scalar.OnePI);
        
        public static BaseFunction E = new ConstFunction(Scalar.Exponent);
        
        #endregion 

        #region Constructor
               
        public ConstFunction(float value)
        {
            Value = value;
        }
        
        #endregion
        
        #region Public Methods
        
        public override float CalcFunction(Dictionary<string, float> arg)
        {
            return Value;
        }
        
        #endregion
    }

    [Serializable]
    public class EqualFunction : BaseFunction
    {
    	#region Public Fields
    	   	
        public string Name = "X";
        
        #endregion 

        #region Constructor
              
        public EqualFunction(string name)
        {
            Name = name;
        }
        
        #endregion
        
        #region Public Methods
        
        public override float CalcFunction(Dictionary<string, float> arg)
        {
        	return arg[Name];
        }
        
        #endregion
    }
}
