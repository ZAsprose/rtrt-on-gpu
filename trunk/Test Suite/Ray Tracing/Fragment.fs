#extension GL_ARB_texture_rectangle : enable

/**********************************************************************************************************************/
/*************************************************** DATA STRUCTURES **************************************************/
/**********************************************************************************************************************/

struct SRay
{
	vec3 Origin;
	
	vec3 Direction;
	
	vec3 Intensity;
};

//---------------------------------------------------------------------------------------------------------------------

struct SVertex
{
	vec3 Position;
	
	vec3 Normal;
};

//---------------------------------------------------------------------------------------------------------------------

struct STriangle
{
	SVertex A;
	
	SVertex B;
	
	SVertex C;
};

//---------------------------------------------------------------------------------------------------------------------

struct SIntersection
{
	vec3 Parameters;
	
	STriangle Triangle;
};

//---------------------------------------------------------------------------------------------------------------------

struct SGrid
{
	vec3 Minimum;
	
	vec3 Maximum;
	
	vec3 VoxelSize;
};

//---------------------------------------------------------------------------------------------------------------------

struct SCamera
{
	vec3 Position;
	
	vec3 Side;
	
	vec3 Up;
	
	vec3 View;
	
	vec2 Scale;
};

/**********************************************************************************************************************/
/************************************************** SHADER INTERFACE **************************************************/
/**********************************************************************************************************************/

uniform SCamera Camera;

//---------------------------------------------------------------------------------------------------------------------

uniform SGrid Grid;

//---------------------------------------------------------------------------------------------------------------------

uniform sampler3D VoxelTexture;

uniform vec3 VoxelTextureStep;

//---------------------------------------------------------------------------------------------------------------------

uniform sampler2DRect PositionTexture;

uniform sampler2DRect NormalTexture;

uniform float VertexTextureSize;

uniform float VertexTextureStep;

/**********************************************************************************************************************/
/************************************************** SHADER CONSTANTS **************************************************/
/**********************************************************************************************************************/

const vec3 Zero = vec3 ( 0.0, 0.0, 0.0 );

const vec3 Unit = vec3 ( 1.0, 1.0, 1.0 );

//---------------------------------------------------------------------------------------------------------------------

const vec3 AxisX = vec3 ( 1.0, 0.0, 0.0 );

const vec3 AxisY = vec3 ( 0.0, 1.0, 0.0 );

const vec3 AxisZ = vec3 ( 0.0, 0.0, 1.0 );

/**********************************************************************************************************************/
/*************************************************** SUPPORT MACRO ****************************************************/
/**********************************************************************************************************************/

#define BIG 1000000.0

#define EPSILON 0.0001

//---------------------------------------------------------------------------------------------------------------------

#define WorldToVoxel( point ) floor ( ( point - Grid.Minimum ) / Grid.VoxelSize )

#define VoxelToWorld( voxel ) ( Grid.Minimum + voxel * Grid.VoxelSize )

/**********************************************************************************************************************/
/************************************************* SUPPORT FUNCTIONS **************************************************/
/**********************************************************************************************************************/

// [23 FLOPS]
SRay GenerateRay ( void )
{
	vec2 scale = Camera.Scale * vec2 ( gl_TexCoord[0] ); // [2 FLOPS]
	
	vec3 direction = Camera.View - scale.x * Camera.Side + scale.y * Camera.Up; // [12 FLOPS]
	
	return SRay ( Camera.Position, normalize ( direction ), Zero ); // [9 FLOPS]
}

vec3 CalcNextStepDirection ( vec3 vec )
{
	if ( all ( lessThanEqual ( vec3 ( vec.x ), vec  ) ) )
	{
		return vec3 ( 1.0, 0.0, 0.0 );
	}
	else
	{
		if ( all ( lessThanEqual ( vec3 ( vec.y ), vec  ) ) )
		{
			return vec3 ( 0.0, 1.0, 0.0 );
		}
		else
		{
			return vec3 ( 0.0, 0.0, 1.0 );		
		}
	}
}

/**********************************************************************************************************************/
/*********************************************** INTERSECTION FUNCTIONS ***********************************************/
/**********************************************************************************************************************/

bool IntersectBox ( SRay ray, vec3 minimum, vec3 maximum, out float tmin, out float tmax )
{
   vec3 l1 = ( minimum - ray.Origin ) / ray.Direction;
   vec3 l2 = ( maximum - ray.Origin ) / ray.Direction;
   
   vec3 lmax = max ( l1, l2 );
   vec3 lmin = min ( l1, l2 );
  
   tmax = min ( lmax.x, min ( lmax.y, lmax.z ) );
   tmin = max ( max ( lmin.x, 0.0 ), max ( lmin.y, lmin.z ) );
  
   return tmax >= tmin;
}

//---------------------------------------------------------------------------------------------------------------------

// [60 FLOPS]
bool IntersectTriangle ( in SRay ray, in STriangle triangle, out vec3 result )
{
	vec3 AB = triangle.B.Position - triangle.A.Position; // [3 FLOPS]
	
	vec3 AC = triangle.C.Position - triangle.A.Position; // [3 FLOPS]
	
	vec3 T = ray.Origin - triangle.A.Position; // [3 FLOPS]
	
	vec3 P = cross ( ray.Direction, AC ); // [9 FLOPS]

	vec3 Q = cross ( T, AB ); // [9 FLOPS]
	
	result = vec3 ( dot ( Q, AC ), dot ( P, T ), dot ( Q, ray.Direction ) ) / dot ( P, AB ); // [23 FLOPS]
	
	return ( result.y > 0.0 ) && ( result.z > 0.0 ) && ( result.y + result.z ) < 1.0; // [1 FLOPS]
}

/**********************************************************************************************************************/
/************************************************* LIGHTING FUNCTIONS *************************************************/
/**********************************************************************************************************************/

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

/**********************************************************************************************************************/
/*********************************************** RAY TRACING FUNCTIONS ************************************************/
/**********************************************************************************************************************/

bool Raytrace ( SRay ray, inout SIntersection intersection )
{	
	//------------------------------------------- Initialization --------------------------------------------
	
	vec3 voxel = WorldToVoxel ( ray.Origin );
	
	vec3 delta = Grid.VoxelSize / abs ( ray.Direction );
		
	vec3 step = sign ( ray.Direction );
	
	vec3 max = delta * max ( step, Zero ) - mod ( ray.Origin - Grid.Minimum, Grid.VoxelSize ) / ray.Direction;
		
	//--------------------------------------------- Traversal -----------------------------------------------
	
	while ( voxel.x > -1.0 && voxel.y > -1.0 && voxel.z > -1.0 && voxel.x < 16.0 && voxel.y < 16.0 && voxel.z < 16.0 )
	{	
		vec3 content = vec3 ( texture3D ( VoxelTexture, voxel * VoxelTextureStep ) );
				
		float count = content.x;
				
		float offset = content.y;
		
		float start = 0.0;
		
		//-------------------------------------------------------------------------------------------------------
				
		for ( float index = 0; index < count; index++ )
		{
			start = offset;
			
			vec3 A = vec3 ( texture2DRect ( PositionTexture, vec2 ( mod ( offset, VertexTextureSize ),
																	floor ( offset * VertexTextureStep ) ) ) );
			offset++;
					
			vec3 B = vec3 ( texture2DRect ( PositionTexture, vec2 ( mod ( offset, VertexTextureSize ),
																	floor ( offset * VertexTextureStep ) ) ) );
			offset++;
					
			vec3 C = vec3 ( texture2DRect ( PositionTexture, vec2 ( mod ( offset, VertexTextureSize ),
																	floor ( offset * VertexTextureStep ) ) ) );
			offset++;
			
			//---------------------------------------------------------------------------------------------------
					
			STriangle triangle = STriangle ( SVertex ( A, Zero ), SVertex ( B, Zero ), SVertex ( C, Zero ) );
			
			vec3 test = vec3 ( 0.0 );
					
			if ( IntersectTriangle ( ray, triangle, test ) && ( test.x < intersection.Parameters.x ) )
			{
				intersection = SIntersection ( test, triangle );
			}
		}
		
		//-------------------------------------------------------------------------------------------------------
			
		if ( intersection.Parameters.x < BIG )
		{
			vec3 NA = vec3 ( texture2DRect ( NormalTexture, vec2 ( mod ( start, VertexTextureSize ),
																   floor ( start * VertexTextureStep) ) ) );
			start++;
					
			vec3 NB =  vec3 ( texture2DRect ( NormalTexture, vec2 ( mod ( start, VertexTextureSize ),
																	floor ( start * VertexTextureStep ) ) ) );
			start++;
					
			vec3 NC =  vec3 ( texture2DRect ( NormalTexture, vec2 ( mod ( start, VertexTextureSize ),
																	floor ( start * VertexTextureStep ) ) ) );
																	
			//---------------------------------------------------------------------------------------------------
					
			intersection.Triangle.A.Normal = NA;
			
			intersection.Triangle.B.Normal = NB;
			
			intersection.Triangle.C.Normal = NC;
					
			return true;
		}
		
		vec3 nd = CalcNextStepDirection ( max );
			
		max += delta * nd;
			
		voxel += step * nd;
	}
	
	return false;
}

/**********************************************************************************************************************/
/**************************************************** ENTRY POINT *****************************************************/
/**********************************************************************************************************************/

void main ( void )
{
	SRay ray = GenerateRay ( );
	
	SIntersection intersection;
		
	intersection.Parameters.x = BIG;
	
	vec3 color = vec3 ( 0.0 );
	
	bool find = false;
	
	//-------------------------------------------------------------------------
	
	float tmin = 0.0, tmax = 0.0;
	
	if ( IntersectBox ( ray, Grid.Minimum, Grid.Maximum, tmin, tmax ) )
	{
		vec3 point = ray.Origin + ( tmin + 0.001 ) * ray.Direction;
		
		vec3 voxel = WorldToVoxel ( point );
		
		ray.Origin = point;
		
		//---------------------------------------------------------------------		
		
		vec3 tDelta = Grid.VoxelSize / abs ( ray.Direction );
		
		vec3 tStep = sign ( ray.Direction );
		
		vec3 tMax = tDelta * max ( tStep, Zero ) - mod ( ray.Origin - Grid.Minimum, Grid.VoxelSize ) / ray.Direction;	
		
		//---------------------------------------------------------------------
		
		while ( voxel.x > -1.0 && voxel.y > -1.0 && voxel.z > -1.0 && voxel.x < 16.0 && voxel.y < 16.0 && voxel.z < 16.0 )
		{
			vec2 voxparams = vec2 ( texture3D ( VoxelTexture, voxel * VoxelTextureStep ) );
			
			float count = voxparams.x;
			
			float tc = voxparams.y;
			
			float btc = -1.0;			
			
			for ( float i = 0; i < count; i ++ )
			{
				float xxx = tc;
				
				vec3 A = vec3 ( texture2DRect ( PositionTexture, vec2 ( mod ( tc, 4096.0 ), floor ( tc * VertexTextureStep ) ) ) );
				tc ++;
				
				vec3 B = vec3 ( texture2DRect ( PositionTexture, vec2 ( mod ( tc, 4096.0 ), floor ( tc * VertexTextureStep ) ) ) );
				tc ++;
				
				vec3 C = vec3 ( texture2DRect ( PositionTexture, vec2 ( mod ( tc, 4096.0 ), floor ( tc * VertexTextureStep ) ) ) );
				tc ++;
				
				STriangle triangle;
				
				triangle.A.Position = A;
				triangle.B.Position = B;
				triangle.C.Position = C;				
						
				vec3 test_time = vec3 ( 0.0 );
				
				if ( IntersectTriangle ( ray, triangle, test_time ) && ( test_time.x < intersection.Parameters.x ) )
				{
					intersection = SIntersection ( test_time, triangle );
					
					btc = xxx;
				}
			}
			
			if ( btc > 0.0 )
			{
				vec3 A = vec3 ( texture2DRect ( NormalTexture, vec2 ( mod ( btc, 4096.0 ), floor ( btc / 4096.0 ) ) ) );
				btc ++;
				
				vec3 B =  vec3 ( texture2DRect ( NormalTexture, vec2 ( mod ( btc, 4096.0 ), floor ( btc / 4096.0 ) ) ) );
				btc ++;
				
				vec3 C =  vec3 ( texture2DRect ( NormalTexture, vec2 ( mod ( btc, 4096.0 ), floor ( btc / 4096.0 ) ) ) );
				
				intersection.Triangle.A.Normal = A;
				intersection.Triangle.B.Normal = B;
				intersection.Triangle.C.Normal = C;
				
				find = true;
				
				break;
			}
			
			//-----------------------------------------------------------------
			
			vec3 nd = CalcNextStepDirection ( tMax );
			
			tMax += tDelta * nd;
			
			voxel += tStep * nd;
		}
	}
	
	if ( find )
	{
		vec3 point = ray.Origin + intersection.Parameters.x * ray.Direction;
		
		vec3 normal = intersection.Triangle.A.Normal * ( 1.0 - intersection.Parameters.y - intersection.Parameters.z ) + 
					  intersection.Triangle.B.Normal * intersection.Parameters.y + 
					  intersection.Triangle.C.Normal * intersection.Parameters.z;
	
		color = Phong ( Camera.Position, Camera.Position, point, normal, abs ( point ) / 2.0 );
							 
		gl_FragColor = vec4 ( color, 1.0 );
	}
}