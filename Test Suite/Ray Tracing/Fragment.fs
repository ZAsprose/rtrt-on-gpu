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

struct SMaterial
{
	vec3 Ambiant;
    
	vec3 Diffuse;
    
	vec3 Specular;
    
	vec3 Color;
	
	vec3 Reflective;
    
	vec3 Refractive;
	
	float Shininess;
	
	float RefractIndex;
};

//---------------------------------------------------------------------------------------------------------------------

struct STriangle
{
	SVertex A;
	
	SVertex B;
	
	SVertex C;
    
    float Offset;
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

//---------------------------------------------------------------------------------------------------------------------

struct SLight
{
	vec3 Ambient;
	
	vec3 Diffuse;
	
	vec3 Specular;
	
	vec3 Position;
};

/**********************************************************************************************************************/
/************************************************** SHADER INTERFACE **************************************************/
/**********************************************************************************************************************/

uniform SCamera Camera;

//---------------------------------------------------------------------------------------------------------------------

uniform SGrid Grid;

//---------------------------------------------------------------------------------------------------------------------

uniform int LightsCount;

uniform SLight Lights[8];

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

#define SHADOWS

//---------------------------------------------------------------------------------------------------------------------

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
/*********************************************** RAY TRACING FUNCTIONS ************************************************/
/**********************************************************************************************************************/

bool Raytrace ( SRay ray, inout SIntersection intersection, float final )
{	
	//------------------------------------------------ Initialization -------------------------------------------------
	
	vec3 voxel = WorldToVoxel ( ray.Origin );
	
	vec3 delta = Grid.VoxelSize / abs ( ray.Direction );
		
	vec3 step = sign ( ray.Direction );
	
	vec3 max = delta * max ( step, Zero ) - mod ( ray.Origin - Grid.Minimum, Grid.VoxelSize ) / ray.Direction;
		
	//--------------------------------------------------- Traversal ---------------------------------------------------
	
	float min;
	
	do
	{
		//---------------------------------- Calc next direction and voxel out time -----------------------------------
		
		vec3 next = NextVoxel ( max );
		
		min = dot ( max, next );
		
		//---------------------------- Reading voxel content ( triange count and offset ) -----------------------------
		
		vec3 content = vec3 ( texture3D ( VoxelTexture, voxel * VoxelTextureStep ) );
				
		float count = content.x;
				
		float offset = content.y;
				
		//-------------------------------- Testing all triangles for ray intersection ---------------------------------
						
		for ( float index = 0.0; index < count; index++ )
		{
            //------------------------------------- Reading triangle vertices -----------------------------------------
            
			vec3 PA = vec3 ( texture2DRect ( PositionTexture, vec2 ( mod ( offset, VertexTextureSize ),
																	 floor ( offset * VertexTextureStep ) ) ) );
			offset++;
					
			vec3 PB = vec3 ( texture2DRect ( PositionTexture, vec2 ( mod ( offset, VertexTextureSize ),
																	 floor ( offset * VertexTextureStep ) ) ) );
			offset++;
					
			vec3 PC = vec3 ( texture2DRect ( PositionTexture, vec2 ( mod ( offset, VertexTextureSize ),
																	 floor ( offset * VertexTextureStep ) ) ) );
			
			STriangle triangle;
            
            triangle.A.Position = PA;
            
            triangle.B.Position = PB;
            
			triangle.C.Position = PC;
			
			triangle.Offset = offset;
            
            //------------------------------- Testing triangle for ray intersection -----------------------------------
			
			vec3 test = vec3 ( 0.0 );
			
			if ( IntersectTriangle ( ray, triangle, test ) && test.x < intersection.Parameters.x && test.x < min )
			{
				intersection = SIntersection ( test, triangle );
			}
			
			offset++;
		}
		
		//----------------------------------- Reading parameters of founded triangle ----------------------------------
			
		if ( intersection.Parameters.x < BIG )
		{
			//-------------------------------------- Reading triangle normals -----------------------------------------
            
			float start = intersection.Triangle.offset;
            
			vec4 NC = texture2DRect ( NormalTexture, vec2 ( mod ( start, VertexTextureSize ),
                                                            floor ( start * VertexTextureStep ) ) );
			start--;
						
			vec4 NB = texture2DRect ( NormalTexture, vec2 ( mod ( start, VertexTextureSize ),
                                                            floor ( start * VertexTextureStep ) ) );
			start--;
						
			vec4 NA = texture2DRect ( NormalTexture, vec2 ( mod ( start, VertexTextureSize ),
                                                            floor ( start * VertexTextureStep ) ) );                                                
						
			intersection.Triangle.A.Normal = vec3 ( NA ); 
				
			intersection.Triangle.B.Normal = vec3 ( NB );
				
			intersection.Triangle.C.Normal = vec3 ( NC );
            
            //-------------------------------- Reading triangle material properties -----------------------------------
            
            float material = NC.w;
            
 			vec3 ambient = vec3 ( texture1D ( MaterialTexture, material * MaterialTextureStep ) );
            
			material++;
            
 			vec3 diffuse = vec3 ( texture1D ( MaterialTexture, material * MaterialTextureStep ) );
            
			material++;
            
 			vec3 specular = vec3 ( texture1D ( MaterialTexture, material * MaterialTextureStep ) );
            
			material++;
            
 			vec3 color = vec3 ( texture1D ( MaterialTexture, material * MaterialTextureStep ) );
            
			material++;
            
 			vec4 reflective = texture1D ( MaterialTexture, material * MaterialTextureStep );
            
			material++;
            
 			vec4 refractive = texture1D ( MaterialTexture, material * MaterialTextureStep );
            
            intersection.Triangle.Properties = SMaterial ( ambient,
                                                           diffuse,
                                                           specular,
                                                           color,
                                                           reflective.xyz,
                                                           refractive.xyz,
                                                           reflective.w,
                                                           refractive.w );
            
			return true;
		}
		
		//---------------------------------------------- Go to next voxel ---------------------------------------------
			
		max += delta * next;
			
		voxel += step * next;
	}	
	while ( min < final );
	
	return false;
}

/**********************************************************************************************************************/
/************************************************* LIGHTING FUNCTIONS *************************************************/
/**********************************************************************************************************************/

void Lighting ( SRay ray, SIntersection intersection, out vec3 color )
{			              
	SIntersection test;
		
	test.Parameters.x = BIG;
	
	//-----------------------------------------------------------------------------------------------------------------
	
	vec3 point = ray.Origin + intersection.Parameters.x * ray.Direction;
			
	vec3 normal = Interpolate ( intersection.Triangle.A.Normal,
	                            intersection.Triangle.B.Normal,
								intersection.Triangle.C.Normal,
								intersection.Parameters.yz );
								
	//-----------------------------------------------------------------------------------------------------------------

	vec3 reflection = reflect ( ray.Direction, normal );

	//-----------------------------------------------------------------------------------------------------------------
	
	for ( int index = 0; index < LightsCount; index++ )
	{
		vec3 light = normalize ( Lights[index].Position - point );
		
		float shadow = 1.0;

		#ifdef SHADOWS__

		ray = SRay ( point + light * EPSILON, light );
		
		float start = 0.0, final = 0.0;
		
		if ( IntersectBox ( ray, Grid.Minimum, Grid.Maximum, start, final ) )
		{
			ray.Origin += ( start + 0.9 ) * ray.Direction;
			
			if ( Raytrace ( ray, test, final - start ) && test.Parameters.x < length ( Lights[index].Position - point ) )
			{
				shadow = 0.0;
			}
		}
		
		#endif
		
		SMaterial properties = intersection.Triangle.Properties;
		
		//-------------------------------------------------------------------------------------------------------------
		
		color += properties.Ambiant * Lights[index].Ambient;		

		//-------------------------------------------------------------------------------------------------------------
		
		float diffuse = abs ( dot ( light, normal) );
		
		color += properties.Diffuse * properties.Color * Lights[index].Diffuse * diffuse * shadow;
		
		//-------------------------------------------------------------------------------------------------------------			
				
		float specular = abs ( dot ( light, reflection ) );
		
		color += properties.Specular * Lights[index].Specular * pow ( specular, properties.Shininess ) * shadow;
	}
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
	
	//-------------------------------------------------------------------------
	
	float start = 0.0, final = 0.0;
	
	if ( IntersectBox ( ray, Grid.Minimum, Grid.Maximum, start, final ) )
	{
		ray.Origin += ( start + 0.001 ) * ray.Direction;
				
		//---------------------------------------------------------------------
		
		if ( Raytrace ( ray, intersection, final - start ) )
		{
			Lighting ( ray, intersection, color );
		}
	}
							 
	gl_FragColor = vec4 ( color, 1.0 );
}