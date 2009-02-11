/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */

using System;
using System.Collections.Generic;

namespace Base.MathTools
{
    public static class Parser
    {
    	#region Public Fields
    	
    	public static string[] Variables = new string[] { "U", "V", "X", "Y", "Z", "T" };
    	
    	#endregion    	
    	
    	#region Public Methods
    	
    	public static BaseFunction ParseString(string formula)
        {
            int index = 0;

            return Expression(formula.Replace(" ", String.Empty).ToUpper() + " ", ref index);
        }
        
        #endregion
        
        #region Private Methods
        
        private static BaseFunction Expression(string formula, ref int position)
        {
            BaseFunction funct = Addend(formula, ref position);

            while (formula[position] == '+' || formula[position] == '-')
            {
            	if (formula[position] == '+')
            	{
            		position++;
            		
            		funct += Addend(formula, ref position);
            	}
            	else
            	{
            		if (formula[position] == '-')
            		{
            			position++;
            			
            			funct -= Addend(formula, ref position);
            		}
            	}
            }
           
            return funct;
        }

        private static BaseFunction Addend(string formula, ref int position)
        {
            BaseFunction funct = Multiplier(formula, ref position);

            while (formula[position] == '*' || formula[position] == '/')
            {
            	if (formula[position] == '*')
            	{
            		position++;
            		
            		funct *= Multiplier(formula, ref position);
            	}
            	else
            	{
            		if (formula[position] == '/')
            		{
            			position++;
            			
            			funct /= Multiplier(formula, ref position);
            		}
            	}
            }

            return funct;
        }

        private static BaseFunction Multiplier(string formula, ref int position)
        {
            BaseFunction funct = null;
            
            if (Char.IsDigit(formula[position]))
            {
                int start = position;
                
                position++;

                while (Char.IsDigit(formula[position]) || (formula[position] == ','))
                {
                	position++;
                }

                string substr = formula.Substring(start, position - start);
                
                funct = new ConstFunction(Convert.ToSingle(substr));
            }
            else
                if (formula[position] == '-')
                {
                    position++;
                    
                    funct = -Multiplier(formula, ref position);
                }
                else
                    if (formula[position] == '(')
                    {
                        position++;
                    
                        funct = Expression(formula, ref position);

                        if (formula[position] == ')')
                        {
                            position++;
                        }
                        else
                        {
                        	throw new Exception("Ошибка! Отсутствует закрывающаяся скобка.");
                        }
                    }
                    else
                        if (Char.IsLetter(formula[position]))
                        {
                            funct = Function(formula, ref position);
                        }
            
            return funct;
        }

        private static BaseFunction Function(string formula, ref int position)
        {
            BaseFunction funct = null;

            int start = position;
            
            while (Char.IsLetter(formula[position]))
            {
            	position++;
            }

            string name = formula.Substring(start, position - start);

            if (formula[position] == '(')
            {
                position++;

                if (name == "SIN")
                    funct = new SinFunction(Expression(formula, ref position));
                else
                    if (name == "COS")
                        funct = new CosFunction(Expression(formula, ref position));
                    else
                        if (name == "LOG")
                            funct = new LogFunction(Expression(formula, ref position));
                        else
                            if (name == "EXP")
                                funct = new ExpFunction(Expression(formula, ref position));
                            else
                                if (name == "SQRT")
                                    funct = new SqrtFunction(Expression(formula, ref position));
                                else
                                    if (name == "ABS")
                                        funct = new AbsFunction(Expression(formula, ref position));
                                	else
                                		throw new Exception("Ошибка! Неизвестная функция. ");
            
                if (formula[position] == ')')
                {
                	position++;
                }
                else
                {
                	throw new Exception("Ошибка! Отсутствует закрывающаяся скобка.");
                }
            }
            else
            {
            	if (name == "PI")
            		funct = ConstFunction.PI;
            	else
            	{
	            	if (name == "E")
	            		funct = ConstFunction.E;
	            	else
	            	{
	            		foreach (string var in Variables)
	            		{
	            			if (name == var)
	            			{
	            				funct = new EqualFunction(var);
	            			}
	            		}
	            		
	            		if (null == funct)
	            		{
	            			throw new Exception("Ошибка! Неизвестная переменная.");
	            		}
	            	}
            	}                        
            }            
            
            return funct;
        }
    
    	#endregion
    }
}