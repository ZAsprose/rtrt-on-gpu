#extension GL_ARB_texture_rectangle : enable

struct SLight
{
	vec3 Position;
	
	float distance;
	
	vec2 HalfSize;

	vec3 Intens;
	
};

struct SRay
{
	vec3 Origin;
	
	vec3 Direction;
};

struct SIntersection
{
	float Time;
	
	vec3 Point;
	
	vec3 Normal;
	
	vec3 Color;
};

struct SPlane
{
	vec3 Center;
	
	vec2 Size;
};

struct SSphere
{
	vec3 Center;
	
	float Radius;
};

//=======================================================================================================================================

uniform SLight Light;

uniform SPlane Plane;

uniform SSphere Sphere;

uniform sampler2DRect Position;

uniform sampler2DRect Color;

varying vec2 ScreenCoords;

const vec3 AxisX = vec3 ( 1.0, 0.0, 0.0 );

const vec3 AxisY = vec3 ( 0.0, 1.0, 0.0 );

const vec3 AxisZ = vec3 ( 0.0, 0.0, 1.0 );

const float PlaneTexScale = 0.25;

//=======================================================================================================================================

SRay GenerateRay ( void )
{
	vec3 direction = AxisX * ScreenCoords.x * Light.HalfSize.x + AxisZ * ScreenCoords.y * Light.HalfSize.z - AxisY * Ligth.distance;
   
	return SRay ( Light.Position, normalize ( direction ) );
}

bool HitPlane ( in SRay ray, out SIntersection intersection  )
{
	intersection.Time = ( Plane.Center.y - ray.Origin.y ) / ray.Direction.y;
	
	intersection.Point = ray.Origin + intersection.Time * ray.Direction;
	
	intersection.Normal = vec3 ( 0.0, 1.0, 0.0 );
		
	vec2 texcoords = fract ( PlaneTexScale * intersection.Point.xz );
	
	intersection.Color = ChessBoardTexture ( vec3 ( 0.8, 0.8, 0.0 ),
	                                         vec3 ( 0.0, 0.0, 0.8 ),
						 texcoords );
	
	return ( intersection.Time >= 0.0 ) &&
	       ( abs ( intersection.Point.x ) <= Plane.Size.x ) &&
	       ( abs ( intersection.Point.z ) <= Plane.Size.y );
}

bool HitSphere ( in SRay ray, out SIntersection intersection )
{
	float a = dot ( ray.Direction, ray.Direction );
	
	float b = dot ( ray.Direction, ray.Origin ) - dot ( ray.Direction, Sphere.Center );
	
	float c = dot ( ray.Origin, ray.Origin ) - 2.0 * dot ( ray.Origin, Sphere.Center ) +
	          dot ( Sphere.Center, Sphere.Center ) - Sphere.Radius * Sphere.Radius;
	
	float det = b * b - a * c;
	
	if ( det > 0.0 )
	{
		det = sqrt ( det );
		
		float tmin = ( -b - det ) / a;
		
		float tmax = ( -b + det ) / a;
		
		if ( tmax > 0.0 )
		{
			if ( tmin > 0.0 )
			{
				intersection.Time = tmin;
			}
			else
			{
				intersection.Time = tmax;
			}
			
			intersection.Point = ray.Origin + intersection.Time * ray.Direction;
			
			intersection.Normal = normalize ( intersection.Point - Sphere.Center );
			
			intersection.Color = vec3 ( 0.2 );
			
			return true;
		}
	}
	
	return false;
}

void main( void )
{
	SRay ray = GenerateRay();
	
	SIntersection intersect;
	
	vec3 intens = vec3(0.0);
	
	//-------------------------------------------------------------------------
	
	if ( HitPlane ( ray, intersect ) )
	{
		SIntersection test;
		
		if ( HitSphere ( ray, test ) && ( test.Time < intersect.Time ) )
		
		{
			intens = Light.Intens;
			
			vec3 refractDir = refract ( ray.Direction, test.Normal, 1.0 / 1.5 );
			
			ray = SRay ( test.Point + refractDir * 0.001, refractDir );
			
			if ( HitSphere ( ray, test ) )
			{
				intens = Light.Intens;
								 
				//-------------------------------------------------------------
				
				refractDir = refract ( ray.Direction, -test.Normal, 1.5 );
				
				ray = SRay ( test.Point + refractDir * 0.001, refractDir );
				
				//-------------------------------------------------------------
				
				if ( HitPlane ( ray, test ) )
				{
					intens = Light.Intens;			
				}
			}
		}
		else
		{
			intens = Light.Intens;
		}
	}
	else
	{
		SIntersection test;
		
		if ( HitSphere ( ray, test ) )
		{
			intens = Light.Intens;
			
			//-----------------------------------------------------------------
			
			vec3 refractDir = refract ( ray.Direction, test.Normal, 1.0 / 1.5 );
			
			ray = SRay ( test.Point + refractDir * 0.001, refractDir );
			
			if ( HitSphere ( ray, test ) )
			{
				intens = Light.Intens;
								 
				//-------------------------------------------------------------
				
				refractDir = refract ( ray.Direction, -test.Normal, 1.5 );
				
				ray = SRay ( test.Point + refractDir * 0.001, refractDir );
				
				//-------------------------------------------------------------
				
				if ( HitPlane ( ray, test ) )
				{
					intens = Light.Intens;		
				}
			}
		}
	}
	
	//-------------------------------------------------------------------------
	
	gl_Frag_Data[1] = vec4( intens,1.0);
	gl_Frag_Data[0] = vec4( intersection.Point, 1.0);
}	






























































