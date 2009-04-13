#extension GL_ARB_texture_rectangle : enable

/**********************************************************************************************************************/
/************************************************** RENDERING CONFIG **************************************************/
/**********************************************************************************************************************/

#define LIGHTING_SHADOWS

#define LIGHTING_TWO_SIDED

#define RENDER_REFLECTIONS

#define RENDER_REFRACTIONS

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

#define BIG 1000000.0

#define EPSILON 0.001

//---------------------------------------------------------------------------------------------------------------------

#define WorldToVoxel( point ) floor ( ( point - Grid.Minimum ) / Grid.VoxelSize )

#define VoxelToWorld( voxel ) ( Grid.Minimum + voxel * Grid.VoxelSize )

#define Interpolate( A, B, C, coords ) ( A * ( 1.0 - coords.x - coords.y ) + B * coords.x + C * coords.y )

#define InsideBox( point ) ( all ( lessThanEqual ( point, Grid.Maximum ) ) && \
                             all ( greaterThanEqual ( point, Grid.Minimum ) ) )					

/**********************************************************************************************************************/
/************************************************* SUPPORT FUNCTIONS **************************************************/
/**********************************************************************************************************************/

SRay GenerateRay ( void )
{
	vec2 scale = Camera.Scale * vec2 ( gl_TexCoord[0] );
	
	vec3 direction = Camera.View - scale.x * Camera.Side + scale.y * Camera.Up;
	
	return SRay ( Camera.Position, normalize ( direction ) );
}

/**********************************************************************************************************************/
/*********************************************** DATA LOADING FUNCTIONS ***********************************************/
/**********************************************************************************************************************/

void LoadTriangle ( inout STriangle triangle, out SMaterial properties )
{
	//------------------------------------------- Reading triangle normals --------------------------------------------
            
	float offset = triangle.Offset;
            
	vec4 NC = texture2DRect ( NormalTexture, vec2 ( mod ( offset, VertexTextureSize ),
	                                                floor ( offset * VertexTextureStep ) ) );
	offset--;
						
	vec4 NB = texture2DRect ( NormalTexture, vec2 ( mod ( offset, VertexTextureSize ),
                                                    floor ( offset * VertexTextureStep ) ) );
	offset--;
						
	vec4 NA = texture2DRect ( NormalTexture, vec2 ( mod ( offset, VertexTextureSize ),
                                                    floor ( offset * VertexTextureStep ) ) );                                                
						
	triangle.A.Normal = vec3 ( NA ); 
				
	triangle.B.Normal = vec3 ( NB );
				
	triangle.C.Normal = vec3 ( NC );
	
	//------------------------------------- Reading triangle material properties --------------------------------------
	
	offset = NC.w;
	
	vec3 ambient = vec3 ( texture1D ( MaterialTexture, offset * MaterialTextureStep ) );
	
	offset++;
	
	vec3 diffuse = vec3 ( texture1D ( MaterialTexture, offset * MaterialTextureStep ) );
	
	offset++;
	
	vec3 specular = vec3 ( texture1D ( MaterialTexture, offset * MaterialTextureStep ) );
	
	offset++;
	
	vec3 color = vec3 ( texture1D ( MaterialTexture, offset * MaterialTextureStep ) );
	
	offset++;
	
	vec4 reflective = texture1D ( MaterialTexture, offset * MaterialTextureStep );
	
	offset++;
	
	vec4 refractive = texture1D ( MaterialTexture, offset * MaterialTextureStep );
	
	properties = SMaterial ( ambient,
	                         diffuse,
	                         specular,
	                         color,
	                         reflective.xyz,
	                         refractive.xyz,
	                         reflective.w,
	                         refractive.w );
}

/**********************************************************************************************************************/
/*********************************************** INTERSECTION FUNCTIONS ***********************************************/
/**********************************************************************************************************************/

float IntersectBox ( SRay ray )
{
	vec3 OMAX = ( Grid.Minimum - ray.Origin ) / ray.Direction;
   
	vec3 OMIN = ( Grid.Maximum - ray.Origin ) / ray.Direction;
	
	vec3 MAX = max ( OMAX, OMIN );
	
	return min ( MAX.x, min ( MAX.y, MAX.z ) );
}

//---------------------------------------------------------------------------------------------------------------------

bool IntersectBox ( SRay ray, out float start, out float final )
{
	if ( InsideBox ( ray.Origin ) )
	{
		final = IntersectBox ( ray );
	}
	else
	{
	   vec3 OMAX = ( Grid.Minimum - ray.Origin ) / ray.Direction;
	   
	   vec3 OMIN = ( Grid.Maximum - ray.Origin ) / ray.Direction;
	   
	   vec3 MAX = max ( OMAX, OMIN );
	   
	   vec3 MIN = min ( OMAX, OMIN );
	  
	   final = min ( MAX.x, min ( MAX.y, MAX.z ) );
	   
	   start = max ( MIN.x, max ( MIN.y, MIN.z ) );	
	}
	
	return final > start;
}

//---------------------------------------------------------------------------------------------------------------------

bool IntersectTriangle ( in SRay ray, in STriangle triangle, out vec3 result )
{
	//------------------------------------------------ Initialization -------------------------------------------------
	
	vec3 AB = triangle.B.Position - triangle.A.Position;
	
	vec3 AC = triangle.C.Position - triangle.A.Position;
	
	vec3 AO = ray.Origin - triangle.A.Position;
	
	//------------------------------------------------ Initialization -------------------------------------------------
	
	vec3 P = cross ( ray.Direction, AC );

	vec3 Q = cross ( AO, AB );
	
	result = vec3 ( dot ( Q, AC ), dot ( P, AO ), dot ( Q, ray.Direction ) ) / dot ( P, AB );
	
	//------------------------------------------------ Initialization -------------------------------------------------
	
	return ( result.x > 0.0 ) && ( result.y > 0.0 ) && ( result.z > 0.0 ) && ( result.y + result.z ) < 1.0;
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
	
	vec3 next = vec3 ( 0.0 );
	
	float min = 0.0;
	
	do
	{
		//---------------------------------- Calc next direction and voxel out time -----------------------------------
		
		next = AxisZ;
		
		min = max.z;
		
		if ( max.x < max.y )
		{
			if ( max.x < max.z )
			{
				next = AxisX;
				
				min = max.x;
			}
		}
		else
		{
			if ( max.y < max.z )
			{
				next = AxisY;
				
				min = max.y;
			}
		}
		
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
			
			STriangle triangle = STriangle ( SVertex ( PA, Zero ),
			                                 SVertex ( PB, Zero ),
			                                 SVertex ( PC, Zero ),
			                                 offset );
            
            //------------------------------- Testing triangle for ray intersection -----------------------------------
			
			vec3 test = vec3 ( 0.0 );
			
			if ( IntersectTriangle ( ray, triangle, test ) && test.x < intersection.Parameters.x && test.x < min )
			{
				intersection = SIntersection ( test, triangle );
			}
			
			offset++;
		}
		
		if ( intersection.Parameters.x < BIG )
		{            
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

vec3 Lighting ( vec3 point, vec3 normal, vec3 reflection, SMaterial properties )
{
	vec3 color = vec3 ( 0.0 );
	
	for ( int index = 0; index < LightsCount; index++ )
	{
		//-------------------------------- Calculating ligth vector and ligth distance --------------------------------
		
		vec3 light = Lights [index].Position - point;
		
		float distance = length ( light );
		
		light /= distance;
		
		//------------------------------------ Checking if point is in the shadow -------------------------------------
		
		float shadow = 1.0;
		
		#ifdef LIGHTING_SHADOWS
		
		SIntersection intersection;
		
		intersection.Parameters.x = BIG;

		SRay ray = SRay ( point + light * EPSILON, light );
		
		float final = IntersectBox ( ray );
		
		if ( Raytrace ( ray, intersection, final ) && intersection.Parameters.x < distance )
		{
			shadow = 0.0;
		}
		
		#endif
		
		//------------------------------------- Calculating ambient contribution --------------------------------------
		
		color += properties.Ambiant * Lights [index].Ambient;		

		//------------------------------------- Calculating diffuse contribution --------------------------------------
		
		#ifdef LIGHTING_TWO_SIDED
		
		float diffuse = abs ( dot ( light, normal ) );
		
		#else
		
		float diffuse = max ( dot ( light, normal ), 0.0 );
		
		#endif
		
		color += properties.Diffuse * properties.Color * Lights [index].Diffuse * diffuse * shadow;
		
		//------------------------------------- Calculating specular contribution -------------------------------------			
		
		#ifdef LIGHTING_TWO_SIDED
		
		float specular = pow ( abs ( dot ( light, reflection ) ), properties.Shininess );
		
		#else
		
		float specular = pow ( max ( dot ( light, reflection ), 0.0 ), properties.Shininess );
		
		#endif
		
		color += properties.Specular * Lights [index].Specular * specular * shadow;
	}
	
	return color;
}

/**********************************************************************************************************************/
/**************************************************** ENTRY POINT *****************************************************/
/**********************************************************************************************************************/

void main ( void )
{
	//-----------------------------------------------------------------------------------------------------------------
	
	SRay ray = GenerateRay ( );
	
	SIntersection intersection;
		
	intersection.Parameters.x = BIG;
	
	vec3 color = vec3 ( 0.0 );
	
	//-----------------------------------------------------------------------------------------------------------------
	
	float start = 0.0, final = 0.0;
	
	if ( IntersectBox ( ray, start, final ) )
	{
		ray.Origin += ( start + 0.001 ) * ray.Direction;
				
		//-------------------------------------------------------------------------------------------------------------
		
		if ( Raytrace ( ray, intersection, final - start ) )
		{
			SMaterial primaryProperties;
			
			LoadTriangle ( intersection.Triangle, primaryProperties );
			
			//------------------------------- Calculating intersection point attributes -------------------------------
			
			vec3 point = ray.Origin + intersection.Parameters.x * ray.Direction;
					
			vec3 normal = Interpolate ( intersection.Triangle.A.Normal,
										intersection.Triangle.B.Normal,
										intersection.Triangle.C.Normal,
										intersection.Parameters.yz );
										
			vec3 reflection = reflect ( ray.Direction, normal );
			
			//------------------------------------ Calculating directional lighting -----------------------------------
			
			color += Lighting ( point, normal, reflection, primaryProperties );
			
			//------------------------------------------- Render reflections ------------------------------------------
					
			#ifdef RENDER_REFLECTIONS
			
			if ( any ( greaterThan ( primaryProperties.Reflective, Zero ) ) )
			{
				intersection.Parameters.x = BIG;

				ray = SRay ( point + reflection * EPSILON, reflection );
			                
				final = IntersectBox ( ray );
			                
				if ( Raytrace ( ray, intersection, final ) )
				{
					SMaterial reflectionProperties;
					
					LoadTriangle ( intersection.Triangle, reflectionProperties );
					
					//--------------------------- Calculating intersection point attributes ---------------------------
					
					point = ray.Origin + intersection.Parameters.x * ray.Direction;
							
					normal = Interpolate ( intersection.Triangle.A.Normal,
										   intersection.Triangle.B.Normal,
										   intersection.Triangle.C.Normal,
										   intersection.Parameters.yz );
												
					reflection = reflect ( ray.Direction, normal );
					
					//-------------------------------- Calculating directional lighting -------------------------------
					
					color += primaryProperties.Reflective * Lighting ( point,
					                                                   normal,
					                                                   reflection,
					                                                   reflectionProperties );
				}			
			}
				
			#endif
			
			#ifdef RENDER_REFRACTIONS
			
			if ( any ( greaterThan ( primaryProperties.Refractive, Zero ) ) )
			{
				intersection.Parameters.x = BIG;
				
				vec3 refraction = refract ( ray.Direction, normal, 1.0 / primaryProperties.RefractIndex );

				ray = SRay ( point + refraction * EPSILON, refraction );
			                
				final = IntersectBox ( ray );
			                
				if ( Raytrace ( ray, intersection, final ) )
				{
					LoadTriangle ( intersection.Triangle, primaryProperties );
					
					//--------------------------- Calculating intersection point attributes ---------------------------
					
					point = ray.Origin + intersection.Parameters.x * ray.Direction;
							
					normal = Interpolate ( intersection.Triangle.A.Normal,
										   intersection.Triangle.B.Normal,
										   intersection.Triangle.C.Normal,
										   intersection.Parameters.yz );
												
					reflection = reflect ( ray.Direction, normal );
					
					//-------------------------------- Calculating directional lighting -------------------------------
					
					color += primaryProperties.Refractive * Lighting ( point,
					                                                   normal,
					                                                   reflection,
					                                                   primaryProperties );
					                                                   
					//-------------------------------------------------------------------------------------------------
					
					refraction = refract ( ray.Direction, -normal, primaryProperties.RefractIndex );

					ray = SRay ( point + refraction * EPSILON, refraction );
				                
					final = IntersectBox ( ray );
					
					intersection.Parameters.x = BIG;
				                
					if ( Raytrace ( ray, intersection, final ) )
					{
						SMaterial refractionProperties;
						
						LoadTriangle ( intersection.Triangle, refractionProperties );
						
						//--------------------------- Calculating intersection point attributes ---------------------------
						
						point = ray.Origin + intersection.Parameters.x * ray.Direction;
								
						normal = Interpolate ( intersection.Triangle.A.Normal,
											   intersection.Triangle.B.Normal,
											   intersection.Triangle.C.Normal,
											   intersection.Parameters.yz );
													
						reflection = reflect ( ray.Direction, normal );
						
						//-------------------------------- Calculating directional lighting -------------------------------
						
						color += primaryProperties.Refractive * Lighting ( point,
																		   normal,
																		   reflection,
																		   refractionProperties );
					}
				}			
			}
				
			#endif
		}
	}
							 
	gl_FragColor = vec4 ( color, 1.0 );
}