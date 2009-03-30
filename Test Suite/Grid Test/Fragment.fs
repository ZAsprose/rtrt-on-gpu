#extension GL_ARB_texture_rectangle : enable

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

uniform vec3 Position;

uniform SCamera Camera;

uniform sampler3D VoxelTexture;

uniform sampler2DRect VertexTexture;

//=================================================================================================

const vec3 BoundingBoxMin = vec3 ( -5.0, -5.0, -5.0 );

const vec3 BoundingBoxMax = vec3 ( 5.0, 5.0, 5.0 );

const float HorizontalScale = 0.414214;

const float VerticalScale = 0.414214;

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

bool IntersectTriangle ( in SRay ray, in STriangle triangle, out float time )
{
	vec3 E1 = triangle.B - triangle.A;  // 3 FLOPS
	
	vec3 E2 = triangle.C - triangle.A;  // 3 FLOPS
	
	vec3 T = ray.Origin - triangle.A;  // 3 FLOPS
	
	//----------------------------------
	
	vec3 P = cross ( ray.Direction, E2 );  // 9 FLOPS

	vec3 Q = cross ( T, E1 );  // 9 FLOPS
	
	vec3 result = vec3 ( dot ( Q, E2 ), dot ( P, T ), dot ( Q, ray.Direction ) ) / dot ( P, E1 );   // 18 FLOPS
	
	time = result.x;
	
	return ( result.y > 0.0 && result.z > 0.0 && ( result.y + result.z ) < 1.0 );
}  // ~51 FLOPS


//=================================================================================================

void main ( void )
{
	SRay ray = GenerateRay ( );
		
	float time = 100000.0;
	
	vec3 color = vec3 ( 0.0 );
	
	//-------------------------------------------------------------------------
	
	vec3 voxel = Position / 16.0;
	
	vec2 voxparams = vec2 ( texture3D ( VoxelTexture, voxel ) );
	
	float count = voxparams.x;
	
	float tc = voxparams.y;
	
	for ( float i = 0; i < count; i ++ )
	{
		vec3 A = vec3 ( texture2DRect ( VertexTexture, vec2 ( mod ( tc, 4096.0 ), floor ( tc / 4096.0 ) ) ) );
		tc ++;
		
		vec3 B =  vec3 ( texture2DRect ( VertexTexture, vec2 ( mod ( tc, 4096.0 ), floor ( tc / 4096.0 ) ) ) );
		tc ++;
		
		vec3 C =  vec3 ( texture2DRect ( VertexTexture, vec2 ( mod ( tc, 4096.0 ), floor ( tc / 4096.0 ) ) ) );
		tc ++;
		
		STriangle triangle = STriangle ( A, B, C );
				
		float test_time = 0.0;
		
		if ( IntersectTriangle ( ray, triangle, test_time ) && ( test_time < time ) )
		{
			time = test_time;
			
			color = abs ( A );
		}
	}
	
	gl_FragColor = vec4 ( color, 1.0 );
}
