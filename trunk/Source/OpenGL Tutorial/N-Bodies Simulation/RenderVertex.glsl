/*
   ---------------------------------------------------------------------------
   |             N - B O D I E S   S I M U L A T I O N   D E M O             |
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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

uniform sampler2DRect CurrentPositionTexture;

uniform vec3 UpDirection;

uniform vec3 RightDirection;

varying vec4 FragmentColor;

varying vec2 FragmentPosition;

const float SizeScale = 0.005;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

void main ( void )
{
	vec4 position = texture2DRect ( CurrentPositionTexture, gl_MultiTexCoord0.xy );
	
	float size = position.w * SizeScale;
	
	vec3 vertex = ( -gl_Vertex.x * RightDirection + gl_Vertex.y * UpDirection ) * size;
	
	vertex += position.xyz;
	
	FragmentColor = gl_Color;	
   
	FragmentPosition = gl_Vertex.xy;
	
	gl_Position =  gl_ModelViewProjectionMatrix * vec4 ( vertex, 1.0 );
}
