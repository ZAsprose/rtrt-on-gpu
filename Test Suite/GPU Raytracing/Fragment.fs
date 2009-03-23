//=================================================================================================

struct SCamera
{
	vec3 Position;
	
	vec3 Side;
	
	vec3 Up;
	
	vec3 View;
};

struct SRay
{
	vec3 Origin;
	
	vec3 Direction;
};

struct STriangle
{
	vec3 A;        
	vec3 B;        
	vec3 C;        
};

struct SIntersection
{
	float Time;
	
	vec3 Point;
	
	vec3 Normal;
	
	vec3 Color;
};

//=================================================================================================

varying vec2 ScreenCoords;

uniform SCamera Camera;

//=================================================================================================

const vec3 BoundingBoxMin = vec3 ( -5.0, -5.0, -5.0 );

const vec3 BoundingBoxMax = vec3 ( 5.0, 5.0, 5.0 );

const vec3 RoomSize = vec3 ( 20.0, 20.0, 20.0 );

const float HorizontalScale = 0.6;

const float VerticalScale = 0.6;

const int Intervals = 50;

//=================================================================================================

float CalcFunction ( vec3 point )
{
	return sin ( point.x ) + sin ( point.y ) + sin ( point.z );
}

vec3 CalcNormal ( vec3 point )
{
	return vec3 ( cos ( point.x ), cos ( point.y ), cos ( point.z ) );
}

//=================================================================================================

SRay GenerateRay ( )
{
   vec3 direction = Camera.View -
                    Camera.Side * ScreenCoords.x * HorizontalScale +
                    Camera.Up * ScreenCoords.y * VerticalScale;
   
   return SRay ( Camera.Position, normalize ( direction ) );
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

bool HitBottom ( in SRay ray, out SIntersection intersect )
{
	intersect.Time = ( -RoomSize.z - ray.Origin.z ) / ray.Direction.z;
	
	intersect.Point = ray.Origin + intersect.Time * ray.Direction;
	
	intersect.Normal = vec3 ( 0.0, 0.0, 1.0 );
		
	vec2 texcoord = fract ( 0.25 * intersect.Point.xy ) - vec2 ( 0.5 );
	
	intersect.Color = vec3 ( 0.8, 0.8, 0.0 ) * mix ( vec3 (1.0), vec3 (0.0), 
	                                                 0.5 * sign ( texcoord.x * texcoord.y ) + 0.5 );
	
	return ( intersect.Time >= 0.0 ) &&
	       ( abs ( intersect.Point.x ) <= RoomSize.x ) &&
	       ( abs ( intersect.Point.y ) <= RoomSize.y );
}

bool HitTop ( in SRay ray, out SIntersection intersect )
{
	intersect.Time = ( RoomSize.z - ray.Origin.z ) / ray.Direction.z;
	
	intersect.Point = ray.Origin + intersect.Time * ray.Direction;
	
	intersect.Normal = vec3 ( 0.0, 0.0, -1.0 );
	
	vec2 texcoord = fract ( 0.25 * intersect.Point.xy ) - vec2 ( 0.5 );
	
	intersect.Color = vec3 ( 0.0, 0.8, 0.8 ) * mix ( vec3 (1.0), vec3 (0.0), 
	                                                 0.5 * sign ( texcoord.x * texcoord.y ) + 0.5 );
	
	return ( intersect.Time >= 0.0 ) &&
	       ( abs ( intersect.Point.x ) <= RoomSize.x ) &&
	       ( abs ( intersect.Point.y ) <= RoomSize.y );
}

bool HitLeft ( in SRay ray, out SIntersection intersect )
{
	intersect.Time = ( -RoomSize.x - ray.Origin.x ) / ray.Direction.x;
	
	intersect.Point = ray.Origin + intersect.Time * ray.Direction;
	
	intersect.Normal = vec3 ( 1.0, 0.0, 0.0 );
	
	vec2 texcoord = fract ( 0.25 * intersect.Point.yz ) - vec2 ( 0.5 );
	
	intersect.Color = vec3 ( 1.0, 0.0, 0.0 ) *
	                             mix ( vec3 (0.0), vec3 (1.0),
	                                   0.5 * sign ( texcoord.x * texcoord.x + texcoord.y * texcoord.y - 0.1 ) + 0.5 );
	
	return ( intersect.Time >= 0.0 ) &&
	       ( abs ( intersect.Point.y ) <= RoomSize.y ) &&
	       ( abs ( intersect.Point.z ) <= RoomSize.z );
}

bool HitRight ( in SRay ray, out SIntersection intersect )
{
	intersect.Time = ( RoomSize.x - ray.Origin.x ) / ray.Direction.x;
	
	intersect.Point = ray.Origin + intersect.Time * ray.Direction;
	
	intersect.Normal = vec3 ( -1.0, 0.0, 0.0 );
	
	vec2 texcoord = fract ( 0.25 * intersect.Point.yz ) - vec2 ( 0.5 );
	
	intersect.Color = vec3 ( 0.0, 1.0, 0.0 ) *
	                             mix ( vec3 (0.0), vec3 (1.0),
	                                   0.5 * sign ( texcoord.x * texcoord.x + texcoord.y * texcoord.y - 0.1 ) + 0.5 );
	
	return ( intersect.Time >= 0.0 ) &&
	       ( abs ( intersect.Point.y ) <= RoomSize.y ) &&
	       ( abs ( intersect.Point.z ) <= RoomSize.z );
}

bool HitNear ( in SRay ray, out SIntersection intersect )
{
	intersect.Time = ( RoomSize.y - ray.Origin.y ) / ray.Direction.y;
	
	intersect.Point = ray.Origin + intersect.Time * ray.Direction;
	
	intersect.Normal = vec3 ( 0.0, -1.0, 0.0 );
	
	vec2 texcoord = fract ( 0.25 * intersect.Point.xz ) - vec2 ( 0.5 );
	
	intersect.Color = vec3 ( 0.5, 1.0, 0.5 ) *
	 mix ( vec3 (0.0), vec3 (1.0),
	 0.5 * sign ( texcoord.x * texcoord.x * texcoord.x * texcoord.x +
	              texcoord.y * texcoord.y * texcoord.y * texcoord.y - 0.008 ) + 0.5 );
	
	return ( intersect.Time >= 0.0 ) &&
	       ( abs ( intersect.Point.x ) <= RoomSize.x ) &&
	       ( abs ( intersect.Point.z ) <= RoomSize.z );
}

bool HitFar ( in SRay ray, out SIntersection intersect )
{
	intersect.Time = ( -RoomSize.y - ray.Origin.y ) / ray.Direction.y;
	
	intersect.Point = ray.Origin + intersect.Time * ray.Direction;
	
	intersect.Normal = vec3 ( 0.0, 1.0, 0.0 );
	
	vec2 texcoord = fract ( 0.25 * intersect.Point.xz ) - vec2 ( 0.5 );
	
	intersect.Color = vec3 ( 1.0, 0.5, 0.5 ) *
	 mix ( vec3 (0.0), vec3 (1.0),
	 0.5 * sign ( texcoord.x * texcoord.x * texcoord.x * texcoord.x +
	              texcoord.y * texcoord.y * texcoord.y * texcoord.y - 0.008 ) + 0.5 );
	
	return ( intersect.Time >= 0.0 ) &&
	       ( abs ( intersect.Point.x ) <= RoomSize.x ) &&
	       ( abs ( intersect.Point.z ) <= RoomSize.z );
}

bool HitSurface ( in SRay ray, in float tmin, in float tmax, out SIntersection intersect )
{ 
	float step = ( tmax - tmin ) / Intervals;
	
	float left = CalcFunction ( ray.Origin + tmin * ray.Direction );

	float right = 0.0;	
	
	for ( int i = 0; i < Intervals; i++ )
	{
		float t = tmin + i * step;
		
		vec3 point = ray.Origin + t * ray.Direction;
		
		right = CalcFunction ( point );
		
		if ( left * right < 0.0 )
		{
			intersect.Time = t + ( right * step ) / ( left - right );
			
			intersect.Point = ray.Origin + intersect.Time * ray.Direction;
			
			intersect.Normal = normalize ( CalcNormal ( intersect.Point ) );
			
			/*
			intersect.Color = ( intersect.Point - BoundingBoxMin ) / ( BoundingBoxMax - BoundingBoxMin );
				
			intersect.Color.r = max ( intersect.Color.r, 0.5 );
			intersect.Color.g = max ( 1.0 - intersect.Color.g, 0.5 );
			intersect.Color.b = max ( intersect.Color.b, 0.5 );
			*/
			
			intersect.Color = vec3 ( 0.0 );
			
			return true;
		}
		
		left = right;
	}
	

	return false;
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

bool Raytrace ( SRay ray, out SIntersection intersect, out bool refl )
{
	intersect.Time = 1000.0;
	
	bool result = false;
	
	SIntersection test;
	
	refl = false;
	
	float tmin, tmax;
		
	if ( HitBottom ( ray, test ) )
	{
		if ( test.Time < intersect.Time )
		{
			intersect = test;
		}
		
		result = true;
	}
	
	if ( HitTop ( ray, test ) )
	{
		if ( test.Time < intersect.Time )
		{
			intersect = test;
		}
		
		result = true;
	}
	
	if ( HitLeft ( ray, test ) )
	{
		if ( test.Time < intersect.Time )
		{
			intersect = test;
		}
		
		result = true;
	}
	
	if ( HitRight ( ray, test ) )
	{
		if ( test.Time < intersect.Time )
		{
			intersect = test;
		}
		
		result = true;
	}
	
	if ( HitNear ( ray, test ) )
	{
		if ( test.Time < intersect.Time )
		{
			intersect = test;
		}
		
		result = true;
	}
	
	if ( HitFar ( ray, test ) )
	{
		if ( test.Time < intersect.Time )
		{
			intersect = test;
		}
		
		result = true;
	}
	
	if ( HitBox ( ray, BoundingBoxMin, BoundingBoxMax, tmin, tmax ) )
	{
		if ( HitSurface ( ray, tmin, tmax, test ) )
		{
			if ( test.Time < intersect.Time )
			{
				intersect = test;
				
				refl = true;
			}
			
			result = true;
		}
	}
	
	return result;
}

//=================================================================================================

bool ProcessTriangle(SRay ray, STriangle triangle, out vec3 point)
{
	vec3 ab = triangle.B - triangle.A; // 3 FLOPS
	vec3 ac = triangle.C - triangle.A; // 3 FLOPS
	
	vec3 normal = normalize(cross(ab, ac));  // 9 + 9 = 18 FLOPS
	
	float time = dot(normal, triangle.A - ray.Origin) / dot(normal, ray.Direction);  // 8 + 5 = 13 FLOPS
	
	point = ray.Origin + time * ray.Direction; // 6 FLOPS
	
	vec3 ap = point - triangle.A; // 3 FLOPS
	
	vec3 nac = cross(normal, ac);  // 9 FLOPS
	vec3 nab = cross(normal, ab);  // 9 FLOPS
		
	vec2 interpolants = vec2(dot(ap, nac) / dot(ab, nac), dot(ap, nab) / dot(ac, nab));  // 26 FLOPS

	if (interpolants.x > 0.0  &&  interpolants.y > 0.0  &&  (interpolants.x + interpolants.y) <= 1.0)
	{
		return true;//(interpolants.x > 0.0  &&  interpolants.y > 0.0  &&  (interpolants.x + interpolants.y) <= 1.0);
	}
	else
	{
		return false;
	}
} // ~90 FLOPS

bool IntersectTriangle ( in SRay ray, in STriangle triangle, out vec3 point )
{
	vec3 E1 = triangle.B - triangle.A;  // 3 FLOPS
	
	vec3 E2 = triangle.C - triangle.A;  // 3 FLOPS
	
	vec3 T = ray.Origin - triangle.A;  // 3 FLOPS
	
	//----------------------------------
	
	vec3 P = cross ( ray.Direction, E2 );  // 9 FLOPS

	vec3 Q = cross ( T, E1 );  // 9 FLOPS
	
	vec3 result = vec3 ( dot ( Q, E2 ), dot ( P, T ), dot ( Q, ray.Direction ) ) / dot ( P, E1 );   // 18 FLOPS
	
	point = ray.Origin + result.x * ray.Direction;  // 6 FLOPS
	
	return ( result.y > 0 && result.z > 0 && ( result.y + result.z ) < 1.0 );
}  // ~51 FLOPS

void main(void)
{
	SRay ray = GenerateRay();
	
	vec3 point;	

	STriangle triangle = STriangle(vec3(0.0, -5.0, -5.0), vec3(0.0, 5.0, -5.0), vec3(0.0, 0.0, 5.0));
	 
	vec3 color = vec3(0.0);
	 
	bool bbb = false;
	
	for (int iii = 0; iii < 5000; iii++)
	{
		//bbb = ProcessTriangle (ray, triangle, point);
		bbb = IntersectTriangle (ray, triangle, point);
	}

	if (bbb) gl_FragColor = vec4(abs(point), 1.0);
}
