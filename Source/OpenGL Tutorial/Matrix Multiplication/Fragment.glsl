/*
   ---------------------------------------------------------------------------
   |           M A T R I X   M U L T I P L I C A T I O N   D E M O           |
   ---------------------------------------------------------------------------
                              
   Copyright (c) 2009 - 2010 Denis Bogolepov ( denisbogol @ gmail.com )

   This program is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation, either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License along
   with this program. If not, see <http://www.gnu.org/licenses/>.
 */
 
#extension GL_ARB_texture_rectangle : enable
 
///////////////////////////////////////////////////////////////////////////////
// Shader interface

uniform sampler2DRect MatrixA;

uniform sampler2DRect MatrixB;

varying vec2 Element;

///////////////////////////////////////////////////////////////////////////////
// Shader entry point

void main ( void )
{
    float result = 0;
    
    for ( int i = 0; i < N; i++ )
    {
        float a = texture2DRect ( MatrixA, vec2 ( i, Element.y ) ).w;
        
        float b = texture2DRect ( MatrixB, vec2 ( i, Element.x ) ).w;
        
        result += a * b;
    }
    
    gl_FragColor = vec4 ( result );
}