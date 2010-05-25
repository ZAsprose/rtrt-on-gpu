/*
   ---------------------------------------------------------------------------
   |          I M P L I C I T   S U R F A C E S   D E M O   ( OCL )          |
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

/* nbody_kern.cl */

__kernel void Update(
        float dt1, float eps,
        __global float4* pos_old,
        __global float4* pos_new,
        __global float4* vel,
        __local float4* pblock
) {       

        const float4 dt = (float4)(dt1,dt1,dt1,0.0f);

        int gti = get_global_id(0);
        int ti = get_local_id(0);

        int n = get_global_size(0);
        int nt = get_local_size(0);
        int nb = n/nt;

        float4 p = pos_old[gti];
        float4 v = vel[gti];

        float4 a = (float4)(0.0f,0.0f,0.0f,0.0f);

        for(int jb=0; jb < nb; jb++) {

                pblock[ti] = pos_old[jb*nt+ti];
                barrier(CLK_LOCAL_MEM_FENCE);

                for(int j=0; j<nt; j++) {

                        float4 p2 = pblock[j];
                        float4 d = p2 - p;
                        float invr = rsqrt(d.x*d.x + d.y*d.y + d.z*d.z + eps);
                        float f = p2.w*invr*invr*invr;
                        a += f*d;

                }

                barrier(CLK_LOCAL_MEM_FENCE);

        }      
        p += dt*v + 0.5f*dt*dt*a;
        v += dt*a;

        pos_new[gti] = p;
        vel[gti] = v;

}