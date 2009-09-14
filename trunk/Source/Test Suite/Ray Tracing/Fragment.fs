#extension GL_ARB_texture_rectangle : enable

#extension GL_EXT_texture_array: enable

/**********************************************************************************************************************/
/************************************************** RENDERING CONFIG **************************************************/
/**********************************************************************************************************************/

#define RENDER_SHADOWS

#define RENDER_REFLECTIONS

#define RENDER_REFRACTIONS

#define RENDER_DISSOLVE

#define RENDER_TEXTURES

//---------------------------------------------------------------------------------------------------------------------

#define USE_TRIANGLE_NORMALS

#define USE_PROXIMITY_GRID_

//---------------------------------------------------------------------------------------------------------------------

#define LIGHTING_ATTENUATION_

//---------------------------------------------------------------------------------------------------------------------

#define SHOW_TRAVERSAL_DEPTH_

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
	
	vec2 TexCoord;
};

//---------------------------------------------------------------------------------------------------------------------

struct STriangle
{
	SVertex A;
	
	SVertex B;
	
	SVertex C;

	vec3 Normal;
    
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
	vec3 Ambient;
    
	vec3 Diffuse;
    
	vec3 Specular;
	
	float Shininess;
	
	vec3 Reflection;
	
	vec3 Refraction;
    
	float Density;
	
	float Dissolve;

	vec2 Scale;
	
	int Texture;
};

//---------------------------------------------------------------------------------------------------------------------

struct SLight
{
	vec3 Position;

	vec3 Ambient;
	
	vec3 Diffuse;
	
	vec3 Specular;
	
	vec3 Attenuation;
};

/**********************************************************************************************************************/
/************************************************** SHADER INTERFACE **************************************************/
/**********************************************************************************************************************/

uniform SCamera Camera;

//---------------------------------------------------------------------------------------------------------------------

uniform SGrid Grid;

//---------------------------------------------------------------------------------------------------------------------

uniform int LightsCount;

uniform SLight Lights [8];

//---------------------------------------------------------------------------------------------------------------------

uniform sampler3D VoxelTexture;

uniform vec3 VoxelTextureStep;

//---------------------------------------------------------------------------------------------------------------------

uniform sampler2DRect PositionTexture;

uniform sampler2DRect NormalTexture;

uniform sampler2DRect TexCoordTexture;

uniform float VertexTextureSize;

uniform float VertexTextureStep;

//---------------------------------------------------------------------------------------------------------------------

uniform sampler1D MaterialTexture;

uniform float MaterialTextureStep;

//---------------------------------------------------------------------------------------------------------------------

uniform sampler2DArray ImageTextures;

//---------------------------------------------------------------------------------------------------------------------

#ifdef SHOW_TRAVERSAL_DEPTH

float TraversalDepth;

#endif

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

//---------------------------------------------------------------------------------------------------------------------

#define FirstVoxelTime( ray, dir ) ( Grid.Minimum + max ( dir, Zero ) * Grid.VoxelSize - ray.Origin ) / ray.Direction

#define FirstVoxel( ray ) ( ray.Origin + EPSILON * ray.Direction - Grid.Minimum ) / Grid.VoxelSize

//---------------------------------------------------------------------------------------------------------------------

#define ClearIntersection( intersection ) intersection.Parameters.x = BIG

//---------------------------------------------------------------------------------------------------------------------

#define Interpolate( A, B, C, value ) ( A * ( 1.0 - value.x - value.y ) + B * value.x + C * value.y )		

/**********************************************************************************************************************/
/************************************************* SUPPORT FUNCTIONS **************************************************/
/**********************************************************************************************************************/

SRay GenerateRay ( void )
{
	vec2 scale = Camera.Scale * vec2 ( gl_TexCoord[0] );
	
	vec3 direction = Camera.View - scale.x * Camera.Side + scale.y * Camera.Up;
	
	return SRay ( Camera.Position, normalize ( direction ) );
}

#ifdef RENDER_TEXTURES

void InterpolatePoint ( SRay ray, SIntersection intersection, out vec3 point,
                        out vec3 normal, out vec3 reflection, out vec2 texcoord )

#else

void InterpolatePoint ( SRay ray, SIntersection intersection, out vec3 point,
                        out vec3 normal, out vec3 reflection )

#endif
{
	point = ray.Origin + intersection.Parameters.x * ray.Direction;
							
	normal = Interpolate ( intersection.Triangle.A.Normal,
	                       intersection.Triangle.B.Normal,
	                       intersection.Triangle.C.Normal,
	                       intersection.Parameters.yz );
	                       
	reflection = reflect ( ray.Direction, normal );
										   
	#ifdef RENDER_TEXTURES

	texcoord = Interpolate ( intersection.Triangle.A.TexCoord,
	                         intersection.Triangle.B.TexCoord,
	                         intersection.Triangle.C.TexCoord,
	                         intersection.Parameters.yz );					
							
	#endif
}

/**********************************************************************************************************************/
/*********************************************** DATA LOADING FUNCTIONS ***********************************************/
/**********************************************************************************************************************/

float LoadTriangle ( inout STriangle triangle )
{
	float offset = triangle.Offset;
            
	vec4 NC = texture2DRect ( NormalTexture, vec2 ( mod ( offset, VertexTextureSize ),
	                                                floor ( offset * VertexTextureStep ) ) );
	                                                
	#ifdef RENDER_TEXTURES
	
	vec2 TC = vec2 ( texture2DRect ( TexCoordTexture, vec2 ( mod ( offset, VertexTextureSize ),
	                                                         floor ( offset * VertexTextureStep ) ) ) );	
	
	#endif
	
	offset--;
	                                                
	vec4 NB = texture2DRect ( NormalTexture, vec2 ( mod ( offset, VertexTextureSize ),
	                                                floor ( offset * VertexTextureStep ) ) );
	                                                
	#ifdef RENDER_TEXTURES
	
	vec2 TB = vec2 ( texture2DRect ( TexCoordTexture, vec2 ( mod ( offset, VertexTextureSize ),
	                                                         floor ( offset * VertexTextureStep ) ) ) );	
	
	#endif
	
	offset--;
	                                                       
	vec4 NA = texture2DRect ( NormalTexture, vec2 ( mod ( offset, VertexTextureSize ),
	                                                floor ( offset * VertexTextureStep ) ) );      
	                                                
	#ifdef RENDER_TEXTURES
	
	vec2 TA = vec2 ( texture2DRect ( TexCoordTexture, vec2 ( mod ( offset, VertexTextureSize ),
	                                                         floor ( offset * VertexTextureStep ) ) ) );	
	
	#endif
						
	triangle.A.Normal = vec3 ( NA );
					
	triangle.B.Normal = vec3 ( NB );
				
	triangle.C.Normal = vec3 ( NC );
	
	#ifdef RENDER_TEXTURES
	
	triangle.A.TexCoord = TA; 
	
	triangle.B.TexCoord = TB;
	
	triangle.C.TexCoord = TC;
	
	#endif
	
	return NC.w;
}

//---------------------------------------------------------------------------------------------------------------------

void LoadTriangle ( inout STriangle triangle, out SMaterial properties )
{	
	float offset = LoadTriangle ( triangle );
	
	vec4 ambient =  texture1D ( MaterialTexture, offset++ * MaterialTextureStep );
	
	vec4 diffuse = texture1D ( MaterialTexture, offset++ * MaterialTextureStep );
	
	vec4 specular = texture1D ( MaterialTexture, offset++ * MaterialTextureStep );
	
	vec4 reflection = texture1D ( MaterialTexture, offset++ * MaterialTextureStep );
	
	vec4 refraction = texture1D ( MaterialTexture, offset++ * MaterialTextureStep );
	
	vec4 scale = texture1D ( MaterialTexture, offset++ * MaterialTextureStep );
	
	properties = SMaterial ( ambient.xyz,
	                         diffuse.xyz,
	                         specular.xyz,
	                         specular.w,
	                         reflection.xyz,
	                         refraction.xyz,
	                         refraction.w,
	                         reflection.w,
	                         scale.xy,
	                         int ( scale.z ) );
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
	vec3 OMAX = ( Grid.Minimum - ray.Origin ) / ray.Direction;
           
	vec3 OMIN = ( Grid.Maximum - ray.Origin ) / ray.Direction;
           
	vec3 MAX = max ( OMAX, OMIN );
           
	vec3 MIN = min ( OMAX, OMIN );
          
	final = min ( MAX.x, min ( MAX.y, MAX.z ) );
           
	start = max ( max ( MIN.x, 0.0 ), max ( MIN.y, MIN.z ) ); 
	
	return final > start;
}

//---------------------------------------------------------------------------------------------------------------------

bool IntersectTriangle ( in SRay ray, in STriangle triangle, out vec3 result )
{
	#ifdef USE_TRIANGLE_NORMALS

	vec3 AB = triangle.B.Position - triangle.A.Position;
	
	vec3 CA = triangle.A.Position - triangle.C.Position;
	
	vec3 OA = triangle.A.Position - ray.Origin;
	
	//-----------------------------------------------------------------------------------------------------------------
	
	vec3 S = cross ( ray.Direction, OA );
	
	result = vec3 ( dot ( OA, triangle.Normal ),
		            dot ( CA, S ),
		            dot ( AB, S ) ) / dot ( ray.Direction, triangle.Normal );

	#else

	//-----------------------------------------------------------------------------------------------------------------

	vec3 AB = triangle.B.Position - triangle.A.Position;
	
	vec3 AC = triangle.C.Position - triangle.A.Position;
	
	vec3 AO = ray.Origin - triangle.A.Position;
	
	//-----------------------------------------------------------------------------------------------------------------
	
	vec3 P = cross ( ray.Direction, AC );

	vec3 Q = cross ( AO, AB );
	
	result = vec3 ( dot ( Q, AC ), dot ( P, AO ), dot ( Q, ray.Direction ) ) / dot ( P, AB );

	#endif

	//-----------------------------------------------------------------------------------------------------------------
	
	return result.x > 0.0 && result.y > 0.0 && result.z > 0.0 && result.y + result.z < 1.0;
}

/**********************************************************************************************************************/
/*********************************************** RAY TRACING FUNCTIONS ************************************************/
/**********************************************************************************************************************/

bool Raytrace ( SRay ray, inout SIntersection intersection, float final )
{	
	//------------------------------------------------ Initialization -------------------------------------------------
	
	vec3 VOXEL = WorldToVoxel ( ray.Origin );
	
	vec3 DELTA = Grid.VoxelSize / abs ( ray.Direction );	
		
	vec3 DIR = sign ( ray.Direction );

	#ifndef USE_PROXIMITY_GRID
	
	vec3 MAX = ( VoxelToWorld ( VOXEL ) + max ( DIR, Zero ) * Grid.VoxelSize - ray.Origin ) / ray.Direction;

	#else

	vec3 A = FirstVoxel ( ray );

	vec3 B = FirstVoxelTime ( ray, DIR );

	vec3 C = Grid.VoxelSize / ray.Direction;

	vec3 D = ray.Direction / Grid.VoxelSize;

	vec3 MAX = VOXEL * C + B;

	float STEP = min ( DELTA.x, min ( DELTA.y, DELTA.z ) );

	#endif

	//--------------------------------------------------- Traversal ---------------------------------------------------
	
	for (int i = 0; i < 256; i++)
	{
		#ifndef USE_PROXIMITY_GRID

		vec3 next = AxisZ;

		float MIN = MAX.z;
			
		if ( MAX.x < MAX.y )
		{
			if ( MAX.x < MAX.z )
			{
				next = AxisX; MIN = MAX.x;
			}
		}
		else
		{
			if ( MAX.y < MAX.z )
			{
				next = AxisY; MIN = MAX.y;
			}
		}

		#else

		float MIN = min ( MAX.x, min ( MAX.y, MAX.z ) );

		#endif
		
		if ( MIN > final ) break;
		
		//---------------------------- Reading voxel content ( triange count and offset ) -----------------------------
		
		vec3 data = vec3 ( texture3D ( VoxelTexture, VOXEL * VoxelTextureStep ) );

		float count = data.x;
				
		float offset = data.y;

		#ifdef USE_PROXIMITY_GRID
		
		float proximity = data.z;
		
		#endif
				
		//-------------------------------- Testing all triangles for ray intersection ---------------------------------

		for ( float index = 0.0; index < count; index++ )
		{
            //------------------------------------- Reading triangle vertices -----------------------------------------
            
			vec4 PA = texture2DRect ( PositionTexture, vec2 ( mod ( offset, VertexTextureSize ),
															  floor ( offset * VertexTextureStep ) ) );
			offset++;
					
			vec4 PB = texture2DRect ( PositionTexture, vec2 ( mod ( offset, VertexTextureSize ),
															  floor ( offset * VertexTextureStep ) ) );
			offset++;
					
			vec4 PC = texture2DRect ( PositionTexture, vec2 ( mod ( offset, VertexTextureSize ),
															  floor ( offset * VertexTextureStep ) ) );
			
			STriangle triangle;
			
			triangle.A.Position = vec3 ( PA );
			
			triangle.B.Position = vec3 ( PB );
			
			triangle.C.Position = vec3 ( PC );
			
			triangle.Normal = vec3 ( PA.w, PB.w, PC.w );
			
			triangle.Offset = offset;
            
            //------------------------------- Testing triangle for ray intersection -----------------------------------
			
			vec3 test = Zero;
			
			if ( IntersectTriangle ( ray, triangle, test ) && test.x < intersection.Parameters.x )
			{
				intersection = SIntersection ( test, triangle );
			}
			
			offset++;
		}
		
		if ( intersection.Parameters.x < MIN )
		{            
			return true;
		}

		//---------------------------------------------- Go to next voxel ---------------------------------------------

		#ifndef USE_PROXIMITY_GRID

		VOXEL += DIR * next;

		MAX += DELTA * next;

		#else

		VOXEL = floor ( A + ( MIN + proximity * STEP ) * D );

		MAX = VOXEL * C + B;

		#endif

		#ifdef SHOW_TRAVERSAL_DEPTH

		TraversalDepth++;

		#endif
	}
	
	return false;
}

/**********************************************************************************************************************/
/************************************************* LIGHTING FUNCTIONS *************************************************/
/**********************************************************************************************************************/

#ifdef RENDER_TEXTURES
		
vec3 Lighting ( vec3 point, vec3 normal, vec3 reflection, vec2 texcoord, SMaterial material )
		
#else
		
vec3 Lighting ( vec3 point, vec3 normal, vec3 reflection, SMaterial material )
		
#endif
{
	vec3 color = Zero;
	
	for ( int index = 0; index < LightsCount; index++ )
	{
		//------------------------------ Calculating direction and distance to the ligth ------------------------------
		
		vec3 light = Lights [index].Position - point;
		
		float distance = length ( light );
		
		light = normalize ( light );
		
		//-------------------------------- Calculating shadowing and light attenuation --------------------------------
		
		float coefficient = 1.0;

		//-------------------------------------------------------------------------------------------------------------
		
		#ifdef RENDER_SHADOWS
		
		SIntersection intersection;
		
		ClearIntersection ( intersection );

		SRay ray = SRay ( point + light * EPSILON, light );
		
		float final = IntersectBox ( ray );
		
		if ( Raytrace ( ray, intersection, final + EPSILON ) && intersection.Parameters.x < distance )
		{
			coefficient = 0.0;
		}
		
		#endif

		//-------------------------------------------------------------------------------------------------------------

		#ifdef LIGHTING_ATTENUATION
		
		coefficient /= Lights [index].Attenuation.x +
			           ( Lights [index].Attenuation.y + Lights [index].Attenuation.z * distance ) * distance;

		#endif
		
		//------------------------------------- Calculating ambient contribution --------------------------------------
		
		color += material.Ambient * Lights [index].Ambient;		

		//------------------------------------- Calculating diffuse contribution --------------------------------------
		
		float diffuse = max ( 0.0, dot ( light, normal ) );
		
		#ifdef RENDER_TEXTURES

		vec3 texture = vec3 ( texture2DArray ( ImageTextures,
			                                   vec3 ( fract ( texcoord * material.Scale ), material.Texture ) ) );

		color += material.Diffuse * Lights [index].Diffuse * texture * ( diffuse * coefficient );
		
		#else
		
		color += material.Diffuse * Lights [index].Diffuse * ( diffuse * coefficient );
		
		#endif
		
		//------------------------------------- Calculating specular contribution -------------------------------------			
		
		float specular = pow ( max ( 0.0, dot ( light, reflection ) ), material.Shininess );
		
		color += material.Specular * Lights [index].Specular * ( specular * coefficient );
	}
	
	return color;
}

/**********************************************************************************************************************/
/**************************************************** ENTRY POINT *****************************************************/
/**********************************************************************************************************************/

void main ( void )
{
	//--------------------------------------------- Generating primary ray --------------------------------------------
	
	SRay ray = GenerateRay ( );
	
	vec3 color = Zero;
	
	#ifdef SHOW_TRAVERSAL_DEPTH

	TraversalDepth = 0.0;

	#endif
	
	//-------------------------------- Intersecting primary ray with scene bounding box -------------------------------
	
	float start = 0.0, final = 0.0;
	
	if ( IntersectBox ( ray, start, final ) )
	{
		ray.Origin += ( start + EPSILON ) * ray.Direction;
				
		//-------------------------- Testing primary ray for intersection with scene objects --------------------------
		
		SIntersection intersection;
			
		ClearIntersection ( intersection );
		
		if ( Raytrace ( ray, intersection, final - start + EPSILON ) )
		{
            //-------------------------- Loading vertices attributes and material properties --------------------------
			
			SMaterial primaryMaterial;
			
			LoadTriangle ( intersection.Triangle, primaryMaterial );
			
			//------------------------------- Calculating intersection point attributes -------------------------------
			
			vec3 point = Zero, normal = Zero, reflection = Zero;
					
			#ifdef RENDER_TEXTURES

			vec2 texcoord = vec2 ( Zero );
			
			InterpolatePoint ( ray, intersection, point, normal, reflection, texcoord );				
				
			#else
			
			InterpolatePoint ( ray, intersection, point, normal, reflection );
				
			#endif
			
			//------------------------------------ Calculating directional lighting -----------------------------------
			
			#ifdef RENDER_TEXTURES

			color += Lighting ( point, normal, reflection, texcoord, primaryMaterial );					
							
			#else	
								
			color += Lighting ( point, normal, reflection, primaryMaterial );
					
			#endif
			
			//=========================================================================================================
			//=============================================== DISSOLVE ================================================
			//=========================================================================================================			
	
			#ifdef RENDER_DISSOLVE 
			
			if ( primaryMaterial.Dissolve < 1.0 )
			{
				//--------------------------- Moving ray origin for a point of intersection ---------------------------
                
                ray.Origin = point + ray.Direction * EPSILON;
			                
				final -= intersection.Parameters.x;
				
				//-------------------------- Testing ray for intersection with scene objects --------------------------                                
                
                ClearIntersection ( intersection );
			                
				if ( Raytrace ( ray, intersection, final + EPSILON ) )
				{
                    //---------------------- Loading vertices attributes and material properties ----------------------
                    
					SMaterial dissolveMaterial;
					
					LoadTriangle ( intersection.Triangle, dissolveMaterial );
												
					//--------------------------- Calculating intersection point attributes ---------------------------
					
					#ifdef RENDER_TEXTURES

					InterpolatePoint ( ray, intersection, point, normal, reflection, texcoord );				
						
					#else
					
					InterpolatePoint ( ray, intersection, point, normal, reflection );
						
					#endif                 
					
					//-------------------------------- Calculating directional lighting -------------------------------
					
					#ifdef RENDER_TEXTURES

					color += ( 1.0F - primaryMaterial.Dissolve ) * Lighting ( point, normal, reflection,
					                                                          texcoord, dissolveMaterial );					
							
					#else	
								
					color += ( 1.0F - primaryMaterial.Dissolve ) * Lighting ( point, normal,
					                                                          reflection, dissolveMaterial );
					
					#endif
				}			
			}
				
			#endif
			
			//=========================================================================================================
			//============================================== REFLECTIONS ==============================================
			//=========================================================================================================
			
			#ifdef RENDER_REFLECTIONS
			
			if ( any ( greaterThan ( primaryMaterial.Reflection, Zero ) ) )
			{
                //------------------------------------- Generating reflection ray -------------------------------------
                
                ray = SRay ( point + reflection * EPSILON, reflection );
			                
				final = IntersectBox ( ray );
                
                //--------------------- Testing reflection ray for intersection with scene objects --------------------
                
                ClearIntersection ( intersection );
			                
				if ( Raytrace ( ray, intersection, final + EPSILON ) )
				{
                    //---------------------- Loading vertices attributes and material properties ----------------------
                    
					SMaterial reflectMaterial;
					
					LoadTriangle ( intersection.Triangle, reflectMaterial );
												
					//--------------------------- Calculating intersection point attributes ---------------------------
					
					#ifdef RENDER_TEXTURES

					InterpolatePoint ( ray, intersection, point, normal, reflection, texcoord );				
						
					#else
					
					InterpolatePoint ( ray, intersection, point, normal, reflection );
						
					#endif                 
					
					//-------------------------------- Calculating directional lighting -------------------------------
					
					#ifdef RENDER_TEXTURES

					color += primaryMaterial.Reflection * Lighting ( point, normal, reflection,
					                                                 texcoord, reflectMaterial );					
							
					#else	
								
					color += primaryMaterial.Reflection * Lighting ( point, normal,
					                                                 reflection, reflectMaterial );
					
					#endif
				}			
			}
				
			#endif
			
			//=========================================================================================================
			//============================================== REFRACTIONS ==============================================
			//=========================================================================================================
			
			#ifdef RENDER_REFRACTIONS
			
			if ( any ( greaterThan ( primaryMaterial.Refraction, Zero ) ) )
			{
                //---------------------------------- Generating first refraction ray ----------------------------------
				
                vec3 refraction = refract ( ray.Direction, normal, 1.0 / primaryMaterial.Density );

				ray = SRay ( point + refraction * EPSILON, refraction );
			                
				final = IntersectBox ( ray );
				
                //------------------ Testing first refraction ray for intersection with scene objects -----------------
                
                ClearIntersection ( intersection );
			                
				if ( Raytrace ( ray, intersection, final + EPSILON ) )
				{
                    //---------------------------------- Loading vertices attributes ----------------------------------
				
					LoadTriangle ( intersection.Triangle );
					
					//--------------------------- Calculating intersection point attributes ---------------------------
					
					#ifdef RENDER_TEXTURES

					InterpolatePoint ( ray, intersection, point, normal, reflection, texcoord );				
						
					#else
					
					InterpolatePoint ( ray, intersection, point, normal, reflection );
						
					#endif
                
					//-------------------------------- Calculating directional lighting -------------------------------
					
					#ifdef RENDER_TEXTURES

					color += primaryMaterial.Refraction * Lighting ( point, normal, reflection,
					                                                 texcoord, primaryMaterial );					
							
					#else				
					
					color += primaryMaterial.Refraction * Lighting ( point, normal,
					                                                 reflection, primaryMaterial );
					
					#endif
					                                                   
					//-------------------------------- Generating second refraction ray -------------------------------
					
					refraction = refract ( ray.Direction, -normal, primaryMaterial.Density );

					ray = SRay ( point + refraction * EPSILON, refraction );
				                
					final = IntersectBox ( ray );
					
                    //--------------- Testing second refraction ray for intersection with scene objects ---------------
                    
                    ClearIntersection ( intersection );
				                
					if ( Raytrace ( ray, intersection, final + EPSILON ) )
					{
                        //-------------------- Loading vertices attributes and material properties --------------------
                        
						SMaterial refractMaterial;
						
						LoadTriangle ( intersection.Triangle, refractMaterial );
						
						//------------------------- Calculating intersection point attributes -------------------------
						
						#ifdef RENDER_TEXTURES

						InterpolatePoint ( ray, intersection, point, normal, reflection, texcoord );				
							
						#else
						
						InterpolatePoint ( ray, intersection, point, normal, reflection );
							
						#endif
						
						//------------------------------ Calculating directional lighting -----------------------------
						
						#ifdef RENDER_TEXTURES

						color += primaryMaterial.Refraction * Lighting ( point, normal, reflection,
						                                                 texcoord, refractMaterial );				
							
						#else
						
						color += primaryMaterial.Refraction * Lighting ( point, normal,
						                                                 reflection, refractMaterial );
								
						#endif											
					}
				}			
			}
				
			#endif
		}
	}
	
	#ifdef SHOW_TRAVERSAL_DEPTH

	gl_FragColor = vec4 ( TraversalDepth / length ( Grid.VoxelCount ) );

	#else
	
	gl_FragColor = vec4 ( color, 1.0 );
	
	#endif
}