/*
 * Author: Bogolepov Denis (denisbogol@sandy.ru)
 */
 
using System;
using System.IO;
using System.Text;

using Base.MathTools;
using Tao.OpenGl;

namespace Base.Graphics
{
	public class ShaderProgram
	{
		#region Public Fields
		
		public string Information = String.Empty;
		
		#endregion
		
        #region Private Fields

        private int vertex = 0;

        private int fragment = 0;

        private int program = 0;

        #endregion

        #region Constructor

        public ShaderProgram()
        {
            vertex = Gl.glCreateShader(Gl.GL_VERTEX_SHADER);
            
            fragment = Gl.glCreateShader(Gl.GL_FRAGMENT_SHADER);

            program = Gl.glCreateProgram();
        }

        #endregion

        #region Private Methods

        private bool LoadShader(int shader, string[] fileNames)
        {
            Information += "Загрузка исходного кода шейдера...\n";
            
            string[] lines = new string[fileNames.Length];

            for (int index = 0; index < fileNames.Length; index++)
            {
                Information += "Файл: " + Path.GetFileName(fileNames[index]) + "...";

                StreamReader streamReader = new StreamReader(fileNames[index]);

                try
                {
                    lines[index] = streamReader.ReadToEnd();

                    streamReader.Close();

                    Information += "Успех!\n";
                }
                catch
                {
                    streamReader.Close();

                    Information += "Неудача!\n";

                    return false;
                }
            }
            
            return CompileShader(lines, shader);
        }
        
        private bool CompileShader(string[] lines, int shader)
        {
        	{
	            Information += "Компиляция исходного кода шейдера...";
	            
	            Gl.glShaderSource(shader, lines.Length, lines, null);
	
	            Gl.glCompileShader(shader);
        	}
        	
        	int status = 0;
        	
        	{	            	
	            unsafe
	            {
	            	Gl.glGetShaderiv(shader, Gl.GL_COMPILE_STATUS, new IntPtr(&status));
	            }
	
	            if (0 != status)
	            {
	            	Information += "Успех!\n";
	            }
	            else
	            {
	            	Information += "Неудача!\n";
	            }
        	}

        	{
	            int capacity = 0;
	
	            unsafe
	            {
	            	Gl.glGetShaderiv(shader, Gl.GL_INFO_LOG_LENGTH, new IntPtr(&capacity));
	            }
	
	            StringBuilder info = new StringBuilder(capacity);
	
	            unsafe
	            {
	            	Gl.glGetShaderInfoLog(shader, Int32.MaxValue, null, info);
	            }
	
	            if (0 == info.Length)
	            {
	            	Information += "Информационный журнал пуст\n";
	            }
	            else
	            {
	            	Information += info;
	            }
        	}
            
            if (0 == status)
            {
            	return false;
            }
            
            {            
	            Information += "Присоединение шейдера к программному объекту...";
	
	            Gl.glAttachShader(program, shader);
	            
	            Information += "Успех!\n";
            }

            return true;
        }

        #endregion

        #region Public Methods
        
        #region Методы для управления шейдерными объектами
        
        public bool LoadVertexShader(string fileName)
        {        
        	return LoadVertexShader(new string[] { fileName });
        }

        public bool LoadFragmentShader(string fileName)
        {
        	return LoadFragmentShader(new string[] { fileName });
        }

        public bool LoadVertexShader(string[] fileNames)
        {
            Information += "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
            Information += "+++                   ВЕРШИННЫЙ ШЕЙДЕР                    +++\n";
            Information += "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
            
            return LoadShader(vertex, fileNames);
        }

        public bool LoadFragmentShader(string[] fileNames)
        {
            Information += "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
            Information += "+++                  ФРАГМЕНТНЫЙ ШЕЙДЕР                   +++\n";
            Information += "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
            
            return LoadShader(fragment, fileNames);
        }

        public bool BuildProgram()
        {
            Information += "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
            Information += "+++                  ПРОГРАММНЫЙ ОБЪЕКТ                   +++\n";
            Information += "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
            
            {
	            Information += "Сборка программного объекта...";
	            
	            Gl.glLinkProgram(program);
            }
            
            int status = 0;

            {
	            unsafe
	            {
	            	Gl.glGetProgramiv(program, Gl.GL_LINK_STATUS, new IntPtr(&status));
	            }
	
	            if (0 != status)
	            {
	            	Information += "Успех!\n";
	            }
	            else
	            {
	            	Information += "Неудача!\n";
	            }
            }
            
            {
	            int capacity = 0;
	            
	            unsafe
	            {
	            	Gl.glGetProgramiv(program, Gl.GL_INFO_LOG_LENGTH, new IntPtr(&capacity));
	            }
	
	            StringBuilder info = new StringBuilder(capacity);
	
	            unsafe
	            {
	            	Gl.glGetProgramInfoLog(program, Int32.MaxValue, null, info);
	            }
	
	            if (0 == info.Length)
	            {
	            	Information += "Информационный журнал пуст\n";
	            }
	            else
	            {
	            	Information += info;
	            }
            }
            
            return 0 != status;
        }

        public void Destroy()
        {
            unsafe
            {
                Gl.glDeleteShader(vertex);
                
                Gl.glDeleteShader(fragment);

                Gl.glDeleteProgram(program);
            }
        }

        public void Bind()
        {
            Gl.glUseProgram(program);
        }

        public void Unbind()
        {
            Gl.glUseProgram(0);
        }

        #endregion
        
        #region Методы для ввода / вывода данных шейдернх объектов

        public int GetUniformLocation(string name)
        {
            return Gl.glGetUniformLocation(program, name);
        }

        public int GetAttributeLocation(string name)
        {
            return Gl.glGetAttribLocation(program, name);
        }

        public Vector4 GetUniformVector(string name)
        {
            float[] values = new float[4];

            int location = Gl.glGetUniformLocation(program, name);

            if (location < 0)
                return null;

            Gl.glGetUniformfv(program, location, values);

            return new Vector4(values);
        }

        public Vector4 GetUniformVector(int location)
        {
            float[] values = new float[4];

            Gl.glGetUniformfv(program, location, values);

            return new Vector4(values);
        }

        public Vector4 GetAttributeVector(string name)
        {
            int location = Gl.glGetAttribLocation(program, name);

            if (location < 0)
                return null;

            float[] values = new float[4];

            Gl.glGetVertexAttribfv(location, Gl.GL_CURRENT_VERTEX_ATTRIB, values);

            return new Vector4(values);
        }

        public Vector4 GetAttributeVector(int location)
        {
            float[] values = new float[4];

            Gl.glGetVertexAttribfv(location, Gl.GL_CURRENT_VERTEX_ATTRIB, values);

            return new Vector4(values);
        }
        
        public bool SetUniformInteger(string name, int value)
        {
            int location = Gl.glGetUniformLocation(program, name);

            if (location < 0)
                return false;

            Gl.glUniform1i(location, value);

            return true;
        }

        public bool SetUniformInteger(int location, int value)
        {
            Gl.glUniform1i(location, value);

            return true;
        }

        public bool SetUniformFloat(string name, float value)
        {
            int location = Gl.glGetUniformLocation(program, name);

            if (location < 0)
                return false;

            Gl.glUniform1f(location, value);

            return true;
        }

        public bool SetUniformFloat(int location, float value)
        {
            Gl.glUniform1f(location, value);

            return true;
        }

        public bool SetUniformVector(string name, Vector2 value)
        {
            int location = Gl.glGetUniformLocation(program, name);

            if (location < 0)
                return false;

            Gl.glUniform2fv(location, 1, value.ToArray());

            return true;
        }

        public bool SetUniformVector(int location, Vector2 value)
        {
            Gl.glUniform2fv(location, 1, value.ToArray());

            return true;
        }

        public bool SetUniformVector(string name, Vector3 value)
        {
            int location = Gl.glGetUniformLocation(program, name);

            if (location < 0)
                return false;

            Gl.glUniform3fv(location, 1, value.ToArray());

            return true;
        }

        public bool SetUniformVector(int location, Vector3 value)
        {
            Gl.glUniform3fv(location, 1, value.ToArray());

            return true;
        }

        public bool SetUniformVector(string name, Vector4 value)
        {
            int location = Gl.glGetUniformLocation(program, name);

            if (location < 0)
                return false;

            Gl.glUniform4fv(location, 1, value.ToArray());

            return true;
        }

        public bool SetUniformVector(int location, Vector4 value)
        {
            Gl.glUniform4fv(location, 1, value.ToArray());

            return true;
        }      
        
        public bool SetUniformMatrix(string name, Matrix2 value)
        {
            int location = Gl.glGetUniformLocation(program, name);

            if (location < 0)
                return false;

            Gl.glUniformMatrix2fv(location, 1, Gl.GL_TRUE, value.ToArray());

            return true;
        }

        public bool SetUniformMatrix(int location, Matrix2 value)
        {
            Gl.glUniformMatrix2fv(location, 1, Gl.GL_TRUE, value.ToArray());

            return true;
        }

        public bool SetUniformMatrix(string name, Matrix3 value)
        {
            int location = Gl.glGetUniformLocation(program, name);

            if (location < 0)
                return false;

            Gl.glUniformMatrix3fv(location, 1, Gl.GL_TRUE, value.ToArray());

            return true;
        }

        public bool SetUniformMatrix(int location, Matrix3 value)
        {
            Gl.glUniformMatrix3fv(location, 1, Gl.GL_TRUE, value.ToArray());

            return true;
        }

        public bool SetUniformMatrix(string name, Matrix4 value)
        {
            int location = Gl.glGetUniformLocation(program, name);

            if (location < 0)
                return false;

            Gl.glUniformMatrix4fv(location, 1, Gl.GL_TRUE, value.ToArray());

            return true;
        }

        public bool SetUniformMatrix(int location, Matrix4 value)
        {
            Gl.glUniformMatrix4fv(location, 1, Gl.GL_TRUE, value.ToArray());

            return true;
        }

        public bool SetTexture(Texture1D texture)
        {
            int location = Gl.glGetUniformLocation(program, texture.Name);

            if (location < 0)
                return false;

            Gl.glUniform1i(location, texture.Unit);

            return true;
        }
        
        public bool SetTexture(Texture2D texture)
        {
            int location = Gl.glGetUniformLocation(program, texture.Name);

            if (location < 0)
                return false;

            Gl.glUniform1i(location, texture.Unit);

            return true;
        }
        
        public bool SetTexture(Texture3D texture)
        {
            int location = Gl.glGetUniformLocation(program, texture.Name);

            if (location < 0)
                return false;

            Gl.glUniform1i(location, texture.Unit);

            return true;
        }        

        public bool SetAttributeName(int location, string name)
        {
            Gl.glBindAttribLocation(program, location, name);

            return true;
        }

        public bool SetAttributeFloat(string name, float value)
        {
            int index = Gl.glGetAttribLocation(program, name);

            if (index < 0)
                return false;

            Gl.glVertexAttrib1f(index, value);

            return true;
        }

        public bool SetAttributeFloat(int location, float value)
        {
            Gl.glVertexAttrib1f(location, value);

            return true;
        }

        public bool SetAttributeVector(string name, Vector2 value)
        {
            int index = Gl.glGetAttribLocation(program, name);

            if (index < 0)
                return false;

            Gl.glVertexAttrib2fv(index, value.ToArray());

            return true;
        }

        public bool SetAttributeVector(int location, Vector2 value)
        {
            Gl.glVertexAttrib2fv(location, value.ToArray());

            return true;
        }

        public bool SetAttributeVector(string name, Vector3 value)
        {
            int index = Gl.glGetAttribLocation(program, name);

            if (index < 0)
                return false;

            Gl.glVertexAttrib3fv(index, value.ToArray());

            return true;
        }

        public bool SetAttributeVector(int location, Vector3 value)
        {
            Gl.glVertexAttrib3fv(location, value.ToArray());

            return true;
        }

        public bool SetAttributeVector(string name, Vector4 value)
        {
            int index = Gl.glGetAttribLocation(program, name);

            if (index < 0)
                return false;

            Gl.glVertexAttrib4fv(index, value.ToArray());

            return true;
        }

        public bool SetAttributeVector(int location, Vector4 value)
        {
            Gl.glVertexAttrib4fv(location, value.ToArray());

            return true;
        }        
        
        #endregion

        #endregion

        #region Properties

        public int VertexShader
        {
            get
            {
                return vertex;
            }
        }

        public int FragmentShader
        {
            get
            {
                return fragment;
            }
        }

        public int ProgramObject
        {
            get
            {
                return program;
            }
        }
        
        #endregion
	}
}
