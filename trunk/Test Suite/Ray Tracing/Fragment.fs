#extension GL_ARB_texture_rectangle : enable

/**********************************************************************************************************************/
/*************************************************** DATA STRUCTURES **************************************************/
/**********************************************************************************************************************/

struct SRay
{
	vec3 Origin;
	
	vec3 Direction;
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
	
	vec3 VoxelCount;
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

//---------------------------------------------------------------------------------------------------------------------

uniform sampler1D MaterialTexture;

uniform float MaterialTextureStep;

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

#define Interpolate( A, B, C, coords ) ( A * ( 1.0 - coords.x - coords.y ) + B * coords.x + C * coords.y )

#define NextVoxel( maximum ) vec3 ( all ( lessThanEqual ( vec2 ( maximum.x ), maximum.yz ) ), \
                                    all ( lessThanEqual ( vec2 ( maximum.y ), maximum.xz ) ), \
									all ( lessThanEqual ( vec2 ( maximum.z ), maximum.xy ) ) )							

/**********************************************************************************************************************/
/************************************************* SUPPORT FUNCTIONS **************************************************/
/**********************************************************************************************************************/

// [23 FLOPS]
SRay GenerateRay ( void )
{
	vec2 scale = Camera.Scale * vec2 ( gl_TexCoord[0] ); // [2 FLOPS]
	
	vec3 direction = Camera.View - scale.x * Camera.Side + scale.y * Camera.Up; // [12 FLOPS]
	
	return SRay ( Camera.Position, normalize ( direction ) ); // [9 FLOPS]
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
	//---------------------------------------------- Initialization ----------------------------------------------
	
	vec3 voxel = WorldToVoxel ( ray.Origin );
	
	vec3 delta = Grid.VoxelSize / abs ( ray.Direction );
		
	vec3 step = sign ( ray.Direction );
	
	vec3 max = delta * max ( step, Zero ) - mod ( ray.Origin - Grid.Minimum, Grid.VoxelSize ) / ray.Direction;
		
	//------------------------------------------------ Traversal -------------------------------------------------
	
	while ( all ( greaterThanEqual ( voxel, Zero ) ) && all ( lessThan ( voxel, Grid.VoxelCount ) ) )
	{
		vec3 content = vec3 ( texture3D ( VoxelTexture, voxel * VoxelTextureStep ) );
				
		float count = content.x;
				
		float offset = content.y;
		
		float start = 0.0;
		
		//--------------------------------------------------------------------------------------------------------
				
		for ( float index = 0; index < count; index++ )
		{
			vec3 PA = vec3 ( texture2DRect ( PositionTexture, vec2 ( mod ( offset, VertexTextureSize ),
																	 floor ( offset * VertexTextureStep ) ) ) );
			offset++;
					
			vec3 PB = vec3 ( texture2DRect ( PositionTexture, vec2 ( mod ( offset, VertexTextureSize ),
																	 floor ( offset * VertexTextureStep ) ) ) );
			offset++;
					
			vec3 PC = vec3 ( texture2DRect ( PositionTexture, vec2 ( mod ( offset, VertexTextureSize ),
																	 floor ( offset * VertexTextureStep ) ) ) );
			
			//----------------------------------------------------------------------------------------------------
					
			STriangle triangle;

			triangle.A.Position = PA;
			
			triangle.B.Position = PB;
			
			triangle.C.Position = PC;
			
			vec3 test = vec3 ( 0.0 );
					
			if ( IntersectTriangle ( ray, triangle, test ) && test.x < intersection.Parameters.x )
			{
				if ( all ( lessThan ( vec3 ( test.x ), max ) ) )
				{
					intersection = SIntersection ( test, triangle );
					
					start = offset;
				}
			}
			
			//----------------------------------------------------------------------------------------------------
			
			offset++;
		}
		
		//--------------------------------------------------------------------------------------------------------
				
		if ( intersection.Parameters.x < BIG )
		{
			vec3 NC = vec3 ( texture2DRect ( NormalTexture, vec2 ( mod ( start, VertexTextureSize ),
																   floor ( start * VertexTextureStep ) ) ) );
			start--;
						
			vec3 NB =  vec3 ( texture2DRect ( NormalTexture, vec2 ( mod ( start, VertexTextureSize ),
																	floor ( start * VertexTextureStep ) ) ) );
			start--;
						
			vec3 NA =  vec3 ( texture2DRect ( NormalTexture, vec2 ( mod ( start, VertexTextureSize ),
																	floor ( start * VertexTextureStep ) ) ) );
																		
			//----------------------------------------------------------------------------------------------------
						
			intersection.Triangle.A.Normal = NA;
				
			intersection.Triangle.B.Normal = NB;
				
			intersection.Triangle.C.Normal = NC;
						
			return true;
		}
		
		//--------------------------------------------------------------------------------------------------------
		
		vec3 next = NextVoxel ( max );
			
		max += delta * next;
			
		voxel += step * next;
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
		
		ray.Origin = point;
		
		//---------------------------------------------------------------------
		
		if ( Raytrace ( ray, intersection ) )
		{
			vec3 point = ray.Origin + intersection.Parameters.x * ray.Direction;
			
			vec3 normal = Interpolate ( intersection.Triangle.A.Normal,
			                            intersection.Triangle.B.Normal,
										intersection.Triangle.C.Normal,
										intersection.Parameters.yz );
		
			color += 0.8 * Phong ( Camera.Position, Camera.Position, point, normal, abs ( point ) / 2.0 );
								 
			gl_FragColor = vec4 ( color, 1.0 );
		}
	}
}