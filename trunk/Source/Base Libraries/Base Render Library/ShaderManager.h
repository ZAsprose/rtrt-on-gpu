/*
 * B A S E   R E N D E R   L I B R A R Y
 *
 * Copyright (C) 2009 Denis Bogolepov, Andrey Morozov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses.
 */

#ifndef _SHADER_MANAGER_HPP_
#define	_SHADER_MANAGER_HPP_

#include <GLee.h>

namespace render
{
    class ShaderManager
    {
        protected:

            /**
             * Handle of the vertex shader.
             */
            unsigned Vertex;

            /**
             * Handle of the fragment shader.
             */
            unsigned Fragment;

            /**
             * Handle of the program object.
             */
            unsigned Program;
            
            /**
             * Loads and compiles shader with specified handle from several files.
             * @param shader handle of the shader object whose source code is to be replaced.
             * @param files array of strings containing paths to vertex shader files.
             * @param count length of strings array.
             * @param prefix string for addition in the source beginning.
             * @return Result of loading.
             */
            bool Load ( unsigned shader,
                        const char ** files,
                        int count,
                        const char * prefix = "" );

            /**
             * Loads and compiles shader with specified handle from several files.
             * @param shader handle of the shader object to be compiled.
             * @return Result of compiling.
             */
            bool Compile ( unsigned shader );

            bool Attach ( unsigned shader );

        public:
            
            ShaderManager ( void );

            ~ShaderManager ( void );
        
            /**
             * Loads and compiles a vertex shader from file.
             * @param file string containing paths to vertex shader file.
             * @param prefix string for addition in the source beginning.
             * @return Result of loading and compiling.
             */
            bool LoadVertexShader ( const char * file, const char * prefix = "" );

            /**
             * Loads and compiles a fragment shader from file.
             * @param file string containing path to fragment shader file.
             * @param prefix string for addition in the source beginning.
             * @return Result of loading and compiling.
             */
            bool LoadFragmentShader ( const char * file, const char * prefix = "" );

            /**
             * Loads and compiles a vertex shader from several files.
             * @param files array of strings containing paths to vertex shader files.
             * @param count length of strings array.
             * @param prefix string for addition in the source beginning.
             * @return Result of loading and compiling.
             */
            bool LoadVertexShader ( const char ** files, int count, const char * prefix = "" );

            /**
             * Loads and compiles a fragment shader from several files.
             * @param files array of strings containing paths to fragment shader files.
             * @param count length of strings array.
             * @param prefix string for addition in the source beginning.
             * @return Result of loading and compiling.
             */
            bool LoadFragmentShader ( const char ** files, int count, const char * prefix = "" );

            /**
             * Links the program object from vertex and fragment shaders.
             * @return Result of link operation.
             */
            bool BuildProgram ( void );

            /**
             * Installs a program object as part of current rendering state.
             */
            void Bind ( void ) const;

            /**
             * Disables the programmable processors.
             */
            static void Unbind ( void );

            unsigned GetVertexHandle ( void ) const { return Vertex; }

            unsigned GetFragmentHandle ( void ) const { return Fragment; }

            unsigned GetProgramHandle ( void ) const { return Program; }
    };
}

#endif

