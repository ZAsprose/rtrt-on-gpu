#extension GL_ARB_texture_rectangle : enable

/**********************************************************************************************************************/
/************************************************** RENDERING CONFIG **************************************************/
/**********************************************************************************************************************/

#define RENDER_SHADOWS_

#define RENDER_REFLECTIONS_

#define RENDER_REFRACTIONS_

#define RENDER_DISSOLVE_

#define USE_TEXTURES_

#define LIGHTING_TWO_SIDED

#define USE_PROXIMITY_GRID_

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

uniform sampler2DRect TexCoordTexture;

uniform float VertexTextureSize;

uniform float VertexTextureStep;

//---------------------------------------------------------------------------------------------------------------------

uniform sampler1D MaterialTexture;

uniform float MaterialTextureStep;

//---------------------------------------------------------------------------------------------------------------------

uniform sampler2D ImageTexture0;

uniform sampler2D ImageTexture1;

uniform sampler2D ImageTexture2;

uniform sampler2D ImageTexture3;

uniform sampler2D ImageTexture4;

uniform sampler2D ImageTexture5;

uniform sampler2D ImageTexture6;

uniform sampler2D ImageTexture7;

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

#define InsideBox( point ) ( all ( lessThan ( point, Grid.Maximum ) ) && all ( greaterThan ( point, Grid.Minimum ) ) )

//---------------------------------------------------------------------------------------------------------------------

#define WorldToVoxel( point ) floor ( ( point - Grid.Minimum ) / Grid.VoxelSize )

#define VoxelToWorld( voxel ) ( Grid.Minimum + voxel * Grid.VoxelSize )

//---------------------------------------------------------------------------------------------------------------------

#define ClearIntersection( intersection ) intersection.Parameters.x = BIG

//---------------------------------------------------------------------------------------------------------------------

#define Interpolate( A, B, C, coords ) ( A * ( 1.0 - coords.x - coords.y ) + B * coords.x + C * coords.y )		

/**********************************************************************************************************************/
/************************************************* SUPPORT FUNCTIONS **************************************************/
/**********************************************************************************************************************/

SRay GenerateRay ( void )
{
	vec2 scale = Camera.Scale * vec2 ( gl_TexCoord[0] );
	
	vec3 direction = Camera.View - scale.x * Camera.Side + scale.y * Camera.Up;
	
	return SRay ( Camera.Position, normalize ( direction ) );
}

#ifdef USE_TEXTURES

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
										   
	#ifdef USE_TEXTURES

	texcoord = Interpolate ( intersection.Triangle.A.TexCoord,
	                         intersection.Triangle.B.TexCoord,
	                         intersection.Triangle.C.TexCoord,
	                         intersection.Parameters.yz );					
							
	#endif
}

/**********************************************************************************************************************/
/*********************************************** DATA LOADING FUNCTIONS ***********************************************/
/**********************************************************************************************************************/

float LoadTriangle1 ( inout STriangle triangle )
{
	//---------------------------------- Reading triangle normals and texture coords ----------------------------------
            
	float offset = triangle.Offset;
            
	vec4 NC = texture2DRect ( NormalTexture, vec2 ( mod ( offset, VertexTextureSize ),
	                                                floor ( offset * VertexTextureStep ) ) );
	                                                
	#ifdef USE_TEXTURES
	
	vec2 TC = vec2 ( texture2DRect ( TexCoordTexture, vec2 ( mod ( offset, VertexTextureSize ),
	                                                         floor ( offset * VertexTextureStep ) ) ) );	
	
	#endif
	
	offset--;
	                                                
	vec4 NB = texture2DRect ( NormalTexture, vec2 ( mod ( offset, VertexTextureSize ),
	                                                floor ( offset * VertexTextureStep ) ) );
	                                                
	#ifdef USE_TEXTURES
	
	vec2 TB = vec2 ( texture2DRect ( TexCoordTexture, vec2 ( mod ( offset, VertexTextureSize ),
	                                                         floor ( offset * VertexTextureStep ) ) ) );	
	
	#endif
	
	offset--;
	                                                       
	vec4 NA = texture2DRect ( NormalTexture, vec2 ( mod ( offset, VertexTextureSize ),
	                                                floor ( offset * VertexTextureStep ) ) );      
	                                                
	#ifdef USE_TEXTURES
	
	vec2 TA = vec2 ( texture2DRect ( TexCoordTexture, vec2 ( mod ( offset, VertexTextureSize ),
	                                                         floor ( offset * VertexTextureStep ) ) ) );	
	
	#endif
						
	triangle.A.Normal = vec3 ( NA );
					
	triangle.B.Normal = vec3 ( NB );
				
	triangle.C.Normal = vec3 ( NC );
	
	#ifdef USE_TEXTURES
	
	triangle.A.TexCoord = TA; 
	
	triangle.B.TexCoord = TB;
	
	triangle.C.TexCoord = TC;
	
	#endif
	
	return NC.w;
}

//---------------------------------------------------------------------------------------------------------------------

void LoadTriangle ( inout STriangle triangle, out SMaterial properties )
{	
	//------------------------------------- Reading triangle material properties --------------------------------------
	
	float offset = LoadTriangle1 ( triangle );
	
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
	//-----------------------------------------------------------------------------------------------------------------
	
	vec3 AB = triangle.B.Position - triangle.A.Position;
	
	vec3 AC = triangle.C.Position - triangle.A.Position;
	
	vec3 AO = ray.Origin - triangle.A.Position;
	
	//-----------------------------------------------------------------------------------------------------------------
	
	vec3 P = cross ( ray.Direction, AC );

	vec3 Q = cross ( AO, AB );
	
	result = vec3 ( dot ( Q, AC ), dot ( P, AO ), dot ( Q, ray.Direction ) ) / dot ( P, AB );
	
	//-----------------------------------------------------------------------------------------------------------------
	
	return ( result.x > 0.0 ) && ( result.y > 0.0 ) && ( result.z > 0.0 ) && ( result.y + result.z ) < 1.0;
}

bool IntersectTriangleS ( SRay ray, STriangle triangle, out vec3 result )
{
	//-----------------------------------------------------------------------------------------------------------------
	
	vec3 AB = triangle.B.Position - triangle.A.Position;
	
	vec3 AC = triangle.A.Position - triangle.C.Position;
	
	vec3 AO = triangle.A.Position - ray.Origin;
	
	//-----------------------------------------------------------------------------------------------------------------
	
	//vec3 P = cross ( ray.Direction, AC );

	vec3 XXX = cross ( ray.Direction, AO );
	
	result = vec3 (   dot ( AO, triangle.Normal ),
	                  dot ( AC, XXX ),
	                  dot ( AB, XXX ) ) / dot ( ray.Direction, triangle.Normal );
	
	//-----------------------------------------------------------------------------------------------------------------
	
	return ( result.x > 0.0 ) && ( result.y > 0.0 ) && ( result.z > 0.0 ) && ( result.y + result.z ) < 1.0;
}

/**********************************************************************************************************************/
/*********************************************** TRAVERSAL FUNCTIONS **************************************************/
/**********************************************************************************************************************/

#ifdef USE_PROXIMITY_GRID
	
vec3 NextVoxel( vec3 next, vec3 max, vec3 delta )
{
	vec3 result;
	
	result = AxisZ;
	
	if ( max.x + delta.x * next.x < max.y + delta.y * next.y )
	{
		if ( max.x + delta.x * next.x < max.z + delta.z * next.z )
		{
			result = AxisX;
		}
	}
	else
	{
		if ( max.y + delta.y * next.y < max.z + delta.z * next.z )
		{
			result = AxisY;
		}
	}
	
	return result;
}
	
#endif

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
	
	vec3 next;
	
	float min;
	
	while ( true )
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
		
		if ( min > final ) break;
		
		//---------------------------- Reading voxel content ( triange count and offset ) -----------------------------
		
		vec3 content = vec3 ( texture3D ( VoxelTexture, voxel * VoxelTextureStep ) );
				
		float count = content.x;
				
		float offset = content.y;
		
		#ifdef USE_PROXIMITY_GRID
		
		float emptyRadius = content.z;
		
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
			
			if ( IntersectTriangleS ( ray, triangle, test ) && test.x < intersection.Parameters.x && test.x < min )
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
			
		#ifdef USE_PROXIMITY_GRID
		
		if ( emptyRadius > 0 )
		{
			next += NextVoxel ( next, max, delta );
		}
		
		#endif
		
		max += delta * next;
			
		voxel += step * next;
		
		#ifdef SHOW_TRAVERSAL_DEPTH

		TraversalDepth++;

		#endif
	}	
	while ( min < final );
	
	return false;
}

/**********************************************************************************************************************/
/************************************************* LIGHTING FUNCTIONS *************************************************/
/**********************************************************************************************************************/

#ifdef USE_TEXTURES
		
vec3 Lighting ( vec3 point, vec3 normal, vec3 reflection, vec2 texcoord, SMaterial material )
		
#else
		
vec3 Lighting ( vec3 point, vec3 normal, vec3 reflection, SMaterial material )
		
#endif
{
	vec3 color = Zero;
	
	for ( int index = 0; index < LightsCount; index++ )
	{
		//-------------------------------- Calculating ligth vector and ligth distance --------------------------------
		
		vec3 light = Lights [index].Position - point;
		
		float distance = length ( light );
		
		light /= distance;
		
		//------------------------------------ Checking if point is in the shadow -------------------------------------
		
		float shadow = 1.0;
		
		#ifdef RENDER_SHADOWS
		
		SIntersection intersection;
		
		ClearIntersection ( intersection );

		SRay ray = SRay ( point + light * EPSILON, light );
		
		float final = IntersectBox ( ray );
		
		if ( Raytrace ( ray, intersection, final ) && intersection.Parameters.x < distance )
		{
			shadow = 0.0;
		}
		
		#endif
		
		//------------------------------------- Calculating ambient contribution --------------------------------------
		
		color += material.Ambient * Lights [index].Ambient;		

		//------------------------------------- Calculating diffuse contribution --------------------------------------
		
		#ifdef LIGHTING_TWO_SIDED
		
		float diffuse = abs ( dot ( light, normal ) );
		
		#else
		
		float diffuse = max ( dot ( light, normal ), 0.0 );
		
		#endif
		
		#ifdef USE_TEXTURES

		vec3 texture = Unit;
		
		if ( 0 != material.Texture )
		{
			if ( 1 == material.Texture )
				texture = vec3 ( texture2D ( ImageTexture0,
				                             fract ( texcoord * material.Scale ) ) );
			else
				if ( 2 == material.Texture )
					texture = vec3 ( texture2D ( ImageTexture1,
					                             fract ( texcoord * material.Scale ) ) );
				else
					if ( 3 == material.Texture )
						texture = vec3 ( texture2D ( ImageTexture2,
						                             fract ( texcoord * material.Scale ) ) );
					else
						if ( 4 == material.Texture )
							texture = vec3 ( texture2D ( ImageTexture3,
							                             fract ( texcoord * material.Scale ) ) );
						else
							if ( 5 == material.Texture )
								texture = vec3 ( texture2D ( ImageTexture4,
															 fract ( texcoord * material.Scale ) ) );
							else
								if ( 6 == material.Texture )
									texture = vec3 ( texture2D ( ImageTexture5,
																 fract ( texcoord * material.Scale ) ) );
								else
									if ( 7 == material.Texture )
										texture = vec3 ( texture2D ( ImageTexture6,
																	 fract ( texcoord * material.Scale ) ) );
									else
										if ( 8 == material.Texture )
											texture = vec3 ( texture2D ( ImageTexture7,
																		 fract ( texcoord * material.Scale ) ) );							                             
		}
		
		color += material.Diffuse * texture * Lights [index].Diffuse * diffuse * shadow;
		
		#else
		
		color += material.Diffuse * Lights [index].Diffuse * diffuse * shadow;
		
		#endif
		
		//------------------------------------- Calculating specular contribution -------------------------------------			
		
		#ifdef LIGHTING_TWO_SIDED
		
		float specular = pow ( abs ( dot ( light, reflection ) ), material.Shininess );
		
		#else
		
		float specular = pow ( max ( dot ( light, reflection ), 0.0 ), material.Shininess );
		
		#endif
		
		color += material.Specular * Lights [index].Specular * specular * shadow;
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
		
		if ( Raytrace ( ray, intersection, final - start ) )
		{
            //-------------------------- Loading vertices attributes and material properties --------------------------
			
			SMaterial primaryMaterial;
			
			LoadTriangle ( intersection.Triangle, primaryMaterial );
			
			//------------------------------- Calculating intersection point attributes -------------------------------
			
			vec3 point = Zero, normal = Zero, reflection = Zero;
					
			#ifdef USE_TEXTURES

			vec2 texcoord = vec2 ( Zero );
			
			InterpolatePoint ( ray, intersection, point, normal, reflection, texcoord );				
				
			#else
			
			InterpolatePoint ( ray, intersection, point, normal, reflection );
				
			#endif
			
			//------------------------------------ Calculating directional lighting -----------------------------------
			
			#ifdef USE_TEXTURES

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
			                
				if ( Raytrace ( ray, intersection, final ) )
				{
                    //---------------------- Loading vertices attributes and material properties ----------------------
                    
					SMaterial dissolveMaterial;
					
					LoadTriangle ( intersection.Triangle, dissolveMaterial );
												
					//--------------------------- Calculating intersection point attributes ---------------------------
					
					#ifdef USE_TEXTURES

					InterpolatePoint ( ray, intersection, point, normal, reflection, texcoord );				
						
					#else
					
					InterpolatePoint ( ray, intersection, point, normal, reflection );
						
					#endif                 
					
					//-------------------------------- Calculating directional lighting -------------------------------
					
					#ifdef USE_TEXTURES

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
			                
				if ( Raytrace ( ray, intersection, final ) )
				{
                    //---------------------- Loading vertices attributes and material properties ----------------------
                    
					SMaterial reflectMaterial;
					
					LoadTriangle ( intersection.Triangle, reflectMaterial );
												
					//--------------------------- Calculating intersection point attributes ---------------------------
					
					#ifdef USE_TEXTURES

					InterpolatePoint ( ray, intersection, point, normal, reflection, texcoord );				
						
					#else
					
					InterpolatePoint ( ray, intersection, point, normal, reflection );
						
					#endif                 
					
					//-------------------------------- Calculating directional lighting -------------------------------
					
					#ifdef USE_TEXTURES

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
			                
				if ( Raytrace ( ray, intersection, final ) )
				{
                    //---------------------------------- Loading vertices attributes ----------------------------------
				
					LoadTriangle1 ( intersection.Triangle );
					
					//--------------------------- Calculating intersection point attributes ---------------------------
					
					#ifdef USE_TEXTURES

					InterpolatePoint ( ray, intersection, point, normal, reflection, texcoord );				
						
					#else
					
					InterpolatePoint ( ray, intersection, point, normal, reflection );
						
					#endif
                
					//-------------------------------- Calculating directional lighting -------------------------------
					
					#ifdef USE_TEXTURES

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
				                
					if ( Raytrace ( ray, intersection, final ) )
					{
                        //-------------------- Loading vertices attributes and material properties --------------------
                        
						SMaterial refractMaterial;
						
						LoadTriangle ( intersection.Triangle, refractMaterial );
						
						//------------------------- Calculating intersection point attributes -------------------------
						
						#ifdef USE_TEXTURES

						InterpolatePoint ( ray, intersection, point, normal, reflection, texcoord );				
							
						#else
						
						InterpolatePoint ( ray, intersection, point, normal, reflection );
							
						#endif
						
						//------------------------------ Calculating directional lighting -----------------------------
						
						#ifdef USE_TEXTURES

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