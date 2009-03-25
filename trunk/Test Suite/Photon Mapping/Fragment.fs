struct SCamera
{
	vec3 Position;
	
	vec3 Side;
	
	vec3 Up;
	
	vec3 View;
	
	vec2 Scale;
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

//=================================================================================================

uniform SCamera Camera;

uniform SPlane Plane;

uniform SSphere Sphere;

varying vec2 ScreenCoords;

//=================================================================================================

SRay GenerateRay ( void )
{
	vec2 coords = ScreenCoords * Camera.Scale;
	
	vec3 direction = Camera.View - Camera.Side * coords.x + Camera.Up * coords.y;
   
	return SRay ( Camera.Position, normalize ( direction ) );
}

//=================================================================================================

vec3 ChessBoardTexture ( vec3 firstColor, vec3 secondColor, vec2 texcoords )
{
	return mix ( firstColor,
	             secondColor,
				 0.5 * sign ( ( texcoords.x - 0.5 ) * ( texcoords.y - 0.5 ) ) + 0.5 );
}

//=================================================================================================

#define DiffuseContribution 0.7

#define SpecularContribution 0.8

#define AmbientContribution 0.05

vec3 Phong ( vec3 lightpos, vec3 camerapos, vec3 point, vec3 normal, vec3 color )
{
	vec3 light = normalize ( lightpos - point );
	
	vec3 view = normalize ( camerapos - point );
	
	vec3 reflect = reflect ( -view, normal );
   
	float diffuse = abs ( dot ( light, normal ) );
	
	float specular = pow ( max ( dot ( view, reflect ), 0.0 ), 32.0 );  
	
	return DiffuseContribution * diffuse * color + vec3 ( SpecularContribution * specular ) + vec3 ( AmbientContribution );
}

//=================================================================================================

bool HitBox ( SRay ray, vec3 boxMin, vec3 boxMax, out float tmin, out float tmax )
{
   vec3 l1 = ( boxMin - ray.Origin ) / ray.Direction;
   vec3 l2 = ( boxMax - ray.Origin ) / ray.Direction;
   
   vec3 lmax = max ( l1, l2 );
   vec3 lmin = min ( l1, l2 );
  
   tmax = min ( lmax.x, min ( lmax.y, lmax.z ) );
   tmin = max ( max ( lmin.x, 0.0 ), max ( lmin.y, lmin.z ) );
  
   return tmax >= tmin;
}

//=================================================================================================

const float PlaneTexScale = 0.25;

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

//=================================================================================================

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

//=================================================================================================

void main ( void )
{
	SRay ray = GenerateRay ( );
	
	//-------------------------------------------------------------------------
	
	vec3 color = vec3 ( 0.0 );
	
	SIntersection intersect;
	
	//-------------------------------------------------------------------------
	
	if ( HitPlane ( ray, intersect ) )
	{
		SIntersection test;
		
		if ( HitSphere ( ray, test ) && ( test.Time < intersect.Time ) )
		{
			color += Phong ( Camera.Position, Camera.Position, test.Point,
			                 test.Normal, test.Color );
			
			//-----------------------------------------------------------------
			
			vec3 refractDir = refract ( ray.Direction, test.Normal, 1.0 / 1.5 );
			
			ray = SRay ( test.Point + refractDir * 0.001, refractDir );
			
			if ( HitSphere ( ray, test ) )
			{
				color += Phong ( Camera.Position, Camera.Position,
				                 test.Point, test.Normal, test.Color );	
								 
				//-------------------------------------------------------------
				
				refractDir = refract ( ray.Direction, test.Normal, 1.5 );
				
				ray = SRay ( test.Point + refractDir * 0.001, refractDir );
				
				//-------------------------------------------------------------
				
				if ( HitPlane ( ray, test ) )
				{
					color += Phong ( Camera.Position, Camera.Position,
					                 test.Point, test.Normal, test.Color );				
				}
			}
		}
		else
		{
			color += Phong ( Camera.Position, Camera.Position, intersect.Point,
							 intersect.Normal, intersect.Color );
		}
	}
	else
	{
		SIntersection test;
		
		if ( HitSphere ( ray, test ) )
		{
			color += Phong ( Camera.Position, Camera.Position, test.Point,
			                 test.Normal, test.Color );
			
			//-----------------------------------------------------------------
			
			vec3 refractDir = refract ( ray.Direction, test.Normal, 1.0 / 1.1 );
			
			ray = SRay ( test.Point + refractDir * 0.001, refractDir );
			
			if ( HitSphere ( ray, test ) )
			{
				color += Phong ( Camera.Position, Camera.Position,
				                 test.Point, test.Normal, test.Color );	
								 
				//-------------------------------------------------------------
				
				refractDir = refract ( ray.Direction, test.Normal, 1.1 );
				
				ray = SRay ( test.Point + refractDir * 0.001, refractDir );
				
				//-------------------------------------------------------------
				
				if ( HitPlane ( ray, test ) )
				{
					color += Phong ( Camera.Position, Camera.Position,
					                 test.Point, test.Normal, test.Color );				
				}
			}
		}
	}
	
	//-------------------------------------------------------------------------
	
	gl_FragColor = vec4 ( color, 1.0 );
}