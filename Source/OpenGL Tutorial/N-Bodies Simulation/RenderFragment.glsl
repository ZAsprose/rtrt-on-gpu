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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

varying vec4 FragmentColor;

varying vec2 FragmentPosition;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

void main()
{
	vec2 posA = abs( FragmentPosition );
	
	vec2 posB = abs( vec2( FragmentPosition.x + FragmentPosition.y, FragmentPosition.x - FragmentPosition.y ) );
			
	gl_FragColor = mix( FragmentColor, vec4( 0.0 ),
	                    0.3 * ( pow( posA.x, 0.3 ) + pow( posA.y, 0.3 ) + pow( posB.x, 0.3 ) + pow( posB.y, 0.3 ) ) );
}