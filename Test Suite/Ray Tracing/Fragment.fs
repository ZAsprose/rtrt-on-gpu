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

uniform sampler1D TestTexture;

uniform int Count;

//=================================================================================================

const vec3 BoundingBoxMin = vec3 ( -5.0, -5.0, -5.0 );

const vec3 BoundingBoxMax = vec3 ( 5.0, 5.0, 5.0 );

const float HorizontalScale = 0.6;

const float VerticalScale = 0.6;

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

//=================================================================================================

bool ProcessTriangle(SRay ray, STriangle triangle, out vec3 point, out float time)
{
	vec3 ab = triangle.B - triangle.A; // 3 FLOPS
	vec3 ac = triangle.C - triangle.A; // 3 FLOPS
	
	vec3 normal = normalize(cross(ab, ac));  // 9 + 9 = 18 FLOPS
	
	time = dot(normal, triangle.A - ray.Origin) / dot(normal, ray.Direction);  // 8 + 5 = 13 FLOPS
	
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

bool IntersectTriangle ( in SRay ray, in STriangle triangle, out float time )
{
	vec3 E1 = triangle.B - triangle.A;  // 3 FLOPS
	
	vec3 E2 = triangle.C - triangle.A;  // 3 FLOPS
	
	vec3 T = ray.Origin - triangle.A;  // 3 FLOPS
	
	//----------------------------------
	
	vec3 P = cross ( ray.Direction, E2 );  // 9 FLOPS

	vec3 Q = cross ( T, E1 );  // 9 FLOPS
	
	vec3 result = vec3 ( dot ( Q, E2 ), dot ( P, T ), dot ( Q, ray.Direction ) ) / dot ( P, E1 );   // 18 FLOPS
	
	//point = ray.Origin + result.x * ray.Direction;  // 6 FLOPS
	
	time = result.x;
	
	return ( result.y > 0.0 && result.z > 0.0 && ( result.y + result.z ) < 1.0 );
}  // ~51 FLOPS


//=================================================================================================

const float step = 1.0 / 4096.0;

void main ( void )
{
	SRay ray = GenerateRay ( );
		
	float time = 100000.0;
	
	vec3 point = vec3 ( 0.0 );	
	
	float tc = 0.0;
	
	for ( int i = 0; i < Count; i ++ )
	{
		vec3 A = vec3 ( texture1D ( TestTexture, tc ) );
		tc += step;
		
		vec3 B =  vec3 ( texture1D ( TestTexture, tc ) );
		tc += step;
		
		vec3 C =  vec3 ( texture1D ( TestTexture, tc ) );
		tc += step;
		
		STriangle triangle = STriangle ( A, B, C );
				
		float test_time = 0.0;
		
		/*
		vec3 test_point = vec3 ( 0.0 );
		
		if ( ProcessTriangle ( ray, triangle, test_point, test_time ) && ( test_time < time ) )
		{
			time = test_time;
		}
		*/
		
		if ( IntersectTriangle ( ray, triangle, test_time ) && ( test_time < time ) )
		{
			time = test_time;
		}
	}
	
	if ( time < 10000.0 )
	{
		point = ray.Origin + time * ray.Direction;
		
		gl_FragColor = vec4 ( abs ( point ), 1.0 );
	}
}
