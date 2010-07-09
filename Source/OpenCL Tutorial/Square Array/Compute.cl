/*
   ---------------------------------------------------------------------------
   |                    S Q U A R E   A R R A Y   ( OCL )                    |
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

__kernel
void square ( __global float * input,
			  __global float * output,
			  const unsigned int count )
{
	int i = get_global_id ( 0 );
	
	if ( i < count )
		output [i] = input [i] * input [i];
}