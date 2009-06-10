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

struct SLight
{
	vec3 Position;

	vec3 Intens;
	
};

//=================================================================================================

uniform SCamera Camera;

uniform SPlane Plane;

uniform SSphere Sphere;

uniform SLight Light;

uniform sampler2DRect PositionTexture;

const vec2 Size = vec2 ( 256.0 );

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

#define DiffuseContribution 0.8

#define SpecularContribution 0.8

#define AmbientContribution 0.2

#define Epsilon	0.5

vec3 Phong ( vec3 lightpos, vec3 camerapos, vec3 point, vec3 normal, vec3 color, float shadow)
{

	vec3 light = normalize ( lightpos - point );
	
	vec3 view = normalize ( camerapos - point );
	
	vec3 reflect = reflect ( -view, normal );
   
	float diffuse = abs ( dot ( light, normal ) );
	
	float specular = pow ( max ( dot ( view, reflect ), 0.0 ), 32.0 );  
	
	return DiffuseContribution * diffuse * color * shadow + shadow * vec3 ( SpecularContribution * specular ) + vec3 ( AmbientContribution ) * color;
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
		
		intersection.Time = mix ( tmin, tmax, step ( tmin, 0.0 ) );
			
		intersection.Point = ray.Origin + intersection.Time * ray.Direction;
			
		intersection.Normal = normalize ( intersection.Point - Sphere.Center );
			
		intersection.Color = vec3 ( 0.2 );
			
		return tmax > 0.0;
	}
	
	return false;
}

bool HitSphereEasy ( in SRay ray, out SIntersection intersection )
{
	vec4 abc;
	
	abc.x = dot ( ray.Direction, ray.Direction );
	
	abc.y = dot ( ray.Direction, ray.Origin ) - dot ( ray.Direction, Sphere.Center );
	
	abc.z = dot ( ray.Origin, ray.Origin ) - 2.0 * dot ( ray.Origin, Sphere.Center ) +
	          dot ( Sphere.Center, Sphere.Center ) - Sphere.Radius * Sphere.Radius;
	
	abc.w = abc.y * abc.y - abc.x * abc.z;
	
	if ( abc.w > 0.0 )
	{
		abc.w = sqrt ( abc.w );
		
		vec2 tmin_max;
		
		tmin_max.x = ( -abc.y - abc.w ) / abc.x;
		
		tmin_max.y = ( -abc.y + abc.w ) / abc.x;
		
		intersection.Time = mix ( tmin_max.x, tmin_max.y, step ( tmin_max.x, 0.0 ) );
		
		return tmin_max.y > 0.0;
	}
	
	return false;
}

bool Compare(vec3 a, vec3 b)
{
	bvec3 c = greaterThan ( a, b );
	
	bvec3 d = equal ( a, b );
	
	return c.x || ( d.x && c.y ) || ( d.x && d.y && c.z );
}

float BinSearch ( vec3 x )
{
	vec3 comp;
	
	comp.x = 0.0;
	
	comp.y = Size.x * Size.x;
    
    while ( comp.x < comp.y )
	{
        comp.z  = floor ( ( comp.x + comp.y ) / 2.0 );
        
        vec3 position = vec3 ( texture2DRect ( PositionTexture,
                                               vec2 ( mod ( comp.z, Size.x ), floor ( comp.z / Size.x ) ) ) ); 

        if ( Compare ( x, position ) )
    
            comp.x = comp.z + 1;
        
        else  comp.y = comp.z - 1;
        
     }
	
	return comp.z;
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
			color += Phong ( Light.Position, Camera.Position, test.Point,
			                 test.Normal, test.Color, 1.0 );
			
			//-----------------------------------------------------------------
			
			vec3 refractDir = refract ( ray.Direction, test.Normal, 1.0 / 1.5 );
			
			ray = SRay ( test.Point + refractDir * 0.001, refractDir );
			
			if ( HitSphere ( ray, test ) )
			{
				color += Phong ( Light.Position, Camera.Position,
				                 test.Point, test.Normal, test.Color, 1.0 );	
								 
				//-------------------------------------------------------------
				
				refractDir = refract ( ray.Direction, -test.Normal, 1.5 );
				
				ray = SRay ( test.Point + refractDir * 0.001, refractDir );
				
				//-------------------------------------------------------------
				
				if ( HitPlane ( ray, test ) )
				{
					vec3 l = Light.Position - test.Point;
					
					float distance = length(l);
					
					l /= distance;
					
					SRay shadowRay = SRay(test.Point + 0.001 * l, l);
					
					SIntersection testShadow;

					if ( HitSphereEasy ( shadowRay, testShadow ) && ( testShadow.Time < distance ) )
					{
						color += Phong(Light.Position, Camera.Position, test.Point, test.Normal,test.Color,0.0);
					}
					else
					{
					
						color += Phong ( Light.Position, Camera.Position,
					                 test.Point, test.Normal, test.Color,1.0 );		
					}	
				}
			}
		}
		else
		{
			vec3 l = Light.Position - intersect.Point;
					
			float distance = length(l);
					
			l /= distance;
					
			SRay shadowRay = SRay(intersect.Point + 0.001 * l, l);
					
			SIntersection testShadow;

			if ( HitSphereEasy(shadowRay,testShadow) && (testShadow.Time < distance))
			{
				color += Phong(Light.Position, Camera.Position, intersect.Point, intersect.Normal,intersect.Color,0.0);
			}
			else
			{	
				color += Phong ( Light.Position, Camera.Position, intersect.Point,
							 intersect.Normal, intersect.Color,1.0 );
			}
			
			float eps = 0.3;
			
			/*
			for ( float i = 0.0; i < Size.x; ++i )
			{
				for ( float j = 0.0; j < Size.x; ++j )
				{
					vec4 position = texture2DRect ( PositionTexture, vec2 ( i, j ) );
									
					color += max ( 0.0, 1.0 - length ( vec3 ( position ) - intersect.Point ) / eps ) * vec3 ( position.w );
				}
			}
			*/
					
			vec2 coords;
					
			coords.y = BinSearch ( intersect.Point + vec3 ( eps ) );
						
			coords.x = BinSearch ( intersect.Point - vec3 ( eps ) );

			for ( float j = coords.x; j < coords.y; ++j )
			{
				vec4 position = texture2DRect ( PositionTexture, vec2 ( mod ( j, Size.x ), floor ( j / Size.x ) ) );
								
				color += max ( 0.0, 1.0 - length ( vec3 ( position ) - intersect.Point ) / eps ) * vec3 ( position.w );
			}
		}
	}
	else
	{
		SIntersection test;
		
		if ( HitSphere ( ray, test ) )
		{
			color += Phong ( Light.Position, Camera.Position, test.Point,
			                 test.Normal, test.Color,1.0 );
			
			//-----------------------------------------------------------------
			
			vec3 refractDir = refract ( ray.Direction, test.Normal, 1.0 / 1.5 );
			
			ray = SRay ( test.Point + refractDir * 0.001, refractDir );
			
			if ( HitSphere ( ray, test ) )
			{
				color += Phong ( Camera.Position, Camera.Position,
				                 test.Point, test.Normal, test.Color,1.0 );	
								 
				//-------------------------------------------------------------
				
				refractDir = refract ( ray.Direction, -test.Normal, 1.5 );
				
				ray = SRay ( test.Point + refractDir * 0.001, refractDir );
				
				//-------------------------------------------------------------
				
				if ( HitPlane ( ray, test ) )
				{
					vec3 l = Light.Position - test.Point;
					
					float distance = length(l);
					
					l /= distance;
					
					SRay shadowRay = SRay(test.Point + 0.001 * l, l);
					
					SIntersection testShadow;
					
					if ( HitSphereEasy ( shadowRay, testShadow ) && ( testShadow.Time < distance ) )
					{
						color += Phong ( Light.Position, Camera.Position, test.Point, test.Normal, test.Color, 0.0 );
					}
					else
					{
					
						color += Phong ( Light.Position, Camera.Position,
										 test.Point, test.Normal, test.Color,1.0 );
					}				
				}
			}
		}
	}
	
	//-------------------------------------------------------------------------
	
	gl_FragColor = vec4 ( color, 1.0 );
}