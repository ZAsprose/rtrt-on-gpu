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

//=================================================================================================

varying vec2 ScreenCoords;

uniform SCamera Camera;

const vec3 BoundingBoxMin = vec3 ( -5.0, -5.0, -5.0 );

const vec3 BoundingBoxMax = vec3 ( 5.0, 5.0, 5.0 );

const float HorizontalScale = 0.466307;

const float VerticalScale = 0.466307;

//=================================================================================================

SRay GenerateRay ( )
{
   vec3 direction = Camera.View -
                    Camera.Side * ScreenCoords.x * HorizontalScale +
                    Camera.Up * ScreenCoords.y * VerticalScale;
   
   return SRay ( Camera.Position, normalize ( direction ) );
}

bool InBox ( vec3 point, vec3 boxMin, vec3 boxMax )
{
   return (point.x >= boxMin.x) && (point.y >= boxMin.y) && (point.z >= boxMin.z) &&
          (point.x <= boxMax.x) && (point.y <= boxMax.y) && (point.z <= boxMax.z);
}

bool HitBox ( SRay ray, vec3 boxMin, vec3 boxMax, out float tmin, out float tmax )
{
   float txmin, txmax;
   float tymin, tymax;
   float tzmin, tzmax;

   if (ray.Direction.x >= 0.0) 
   {
      txmin = (boxMin.x - ray.Origin.x) / ray.Direction.x;
      txmax = (boxMax.x - ray.Origin.x) / ray.Direction.x;
   }
   else 
   {
      txmin = (boxMax.x - ray.Origin.x) / ray.Direction.x;
      txmax = (boxMin.x - ray.Origin.x) / ray.Direction.x;
   }
   
   if (ray.Direction.y >= 0.0) 
   {
      tymin = (boxMin.y - ray.Origin.y) / ray.Direction.y;
      tymax = (boxMax.y - ray.Origin.y) / ray.Direction.y;
   }
   else 
   {
      tymin = (boxMax.y - ray.Origin.y) / ray.Direction.y;
      tymax = (boxMin.y - ray.Origin.y) / ray.Direction.y;
   }
         
   if (ray.Direction.z >= 0.0) 
   {
      tzmin = (boxMin.z - ray.Origin.z) / ray.Direction.z;
      tzmax = (boxMax.z - ray.Origin.z) / ray.Direction.z;
   }
   else 
   {
      tzmin = (boxMax.z - ray.Origin.z) / ray.Direction.z;
      tzmax = (boxMin.z - ray.Origin.z) / ray.Direction.z;
   }
   
   bool flag = true;
   
   if (InBox(ray.Origin, boxMin, boxMax))
   {		
		tmax = min(max(txmin, txmax), max(tymin, tymax));
		tmax = min(tmax, max(tzmin, tzmax));
		
		tmin = 0.0;
   }
   else
   {
	   if (txmin > tymax || tymin > txmax) 
	   {
	      flag = false;
	   }
	   
	   tmin = max(txmin, tymin);
	   tmax = min(txmax, tymax);
	   
	   if (tmin > tzmax || tzmin > tmax)
	   {
	      flag = false;
	   }
	   
	   if (tzmin > tmin) tmin = tzmin;
	   if (tzmax < tmax) tmax = tzmax;   
   }
   
   return flag;   
}

float CalcFunction ( vec3 point )
{
	return sin ( point.x ) + sin ( point.y ) + sin ( point.z );
}

vec3 CalcNormal ( vec3 point )
{
	return vec3 ( cos ( point.x ), cos ( point.y ), cos ( point.z ) );
}

#define intervals 50

bool HitSurface ( in SRay ray, in float tmin, in float tmax, out float val )
{ 
	float step = ( tmax - tmin ) / intervals;
	
	float left = CalcFunction ( ray.Origin + tmin * ray.Direction );

	float right = 0.0;
	
	
	for ( int i = 0; i < intervals; i++ )
	{
		float t = tmin + i * step;
		
		vec3 point = ray.Origin + t * ray.Direction;
		
		right = CalcFunction ( point );
		
		if ( left * right < 0.0 )
		{
			val = t + ( right * step ) / ( left - right );
			
			return true;
		}
		
		left = right;
	}
	

	return false;
}

#define DiffuseContribution 0.7

#define SpecularContribution 0.8

#define AmbientContribution 0.3

float Phong ( vec3 lightpos, vec3 camerapos, vec3 point, vec3 normal )
{
	vec3 light = normalize ( lightpos - point );
	
	vec3 view = normalize ( camerapos - point );
	
	vec3 reflect = reflect ( -view, normal );
   
	float diffuse = abs ( dot ( light, normal ) );
	
	float specular = pow ( max ( dot ( view, reflect ), 0.0 ), 32.0 );  
	
	return DiffuseContribution * diffuse + SpecularContribution * specular + AmbientContribution;
}

//=================================================================================================

void main ( void )
{
	SRay ray = GenerateRay ( );
	
	float tmin;
	float tmax;   
    
	if ( HitBox ( ray, BoundingBoxMin, BoundingBoxMax, tmin, tmax ) )
	{
		if ( tmin >= 0.0 )
		{
			float val = 0.0;
				
			if ( HitSurface ( ray, tmin, tmax, val ) )
			{
				vec3 point = ray.Origin + val * ray.Direction;
					
				vec3 normal = normalize ( CalcNormal ( point ) );
					
				vec3 color = ( point - BoundingBoxMin ) / ( BoundingBoxMax - BoundingBoxMin );
				
				color.r = max ( color.r, 0.5 );
				color.g = max ( 1.0 - color.g, 0.5 );
				color.b = max ( color.b, 0.5 );
				
				gl_FragColor = vec4 ( color, 1.0 ) * Phong ( Camera.Position, Camera.Position, point, normal );
			}
			else
			{
				discard; 
			}
		}
		else
		{
			discard;  
		}
	}
	else
	{                               
		discard;
	}
}
