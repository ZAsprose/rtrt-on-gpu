/**********************************************************************************************************************/
/************************************************* SUPPORT FUNCTIONS **************************************************/
/**********************************************************************************************************************/

SRay GenerateRay ( void )
{
	vec3 direction = AxisX * gl_TexCoord[0].x * Light.Radius.x +
	                 AxisY * gl_TexCoord[0].y * Light.Radius.y -
	                 AxisZ * Light.Distance;
   
	return SRay ( Light.Position, normalize ( direction ) );
}

/**********************************************************************************************************************/
/**************************************************** ENTRY POINT *****************************************************/
/**********************************************************************************************************************/

void main ( void )
{
	SRay ray = GenerateRay ( );
		
	vec3 point = vec3 ( BIG );
	
	//-----------------------------------------------------------------------------------------------------------------
	
	float start, final, current, time = BIG;
    
	if ( IntersectBox ( ray, BoxMinimum, BoxMaximum, start, final ) )
	{
		if ( IntersectSurface ( ray, start, final, current ) && current < time )
		{
			time = current;
			
			if ( IntersectPlane ( ray, start, final, AxisZ, BoxMinimum.z, current ) && current < time )
			{
				time = current;
				
				point = ray.Origin + time * ray.Direction;
			}
			else
			{
				point = ray.Origin + time * ray.Direction;

				vec3 noise = vec3 ( texture2D ( NoiseTexture, fract ( point.xy ) ).a );
				
				vec3 normal = normalize ( CalcNormal ( point ) + noise * 0.2 );
				                  
				//-----------------------------------------------------------------------------------------------------
				                  
				vec3 refract = refract ( ray.Direction,
				                         normal,
				                         1.0 / OpticalDensity );
				
				ray = SRay ( point + refract * EPSILON, refract );
				
				final = IntersectBox ( ray, BoxMinimum, BoxMaximum );
				
				time = BIG;
				
				if ( IntersectPlane ( ray, 0.0, final, AxisZ, BoxMinimum.z, current ) && current < time )
				{
					time = current;
					
					point = ray.Origin + time * ray.Direction;
				}				               
			}			
		}
		else
		{
			if ( IntersectPlane ( ray, start, final, AxisZ, BoxMinimum.z, current ) && current < time )
			{
				time = current;
				
				point = ray.Origin + time * ray.Direction;
			}		
		}
	}
	
	//-----------------------------------------------------------------------------------------------------------------
	
	gl_FragColor = vec4 ( point, 0.05 );
}