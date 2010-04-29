/*************************************************************************************************/
/*                       Fragment Shader for Implicity Surface Ray Tracing                       */	
/*************************************************************************************************/

struct SCamera
{
	vec3 Position;
	
	vec3 Side;
	
	vec3 Up;
	
	vec3 View;
	
	vec2 Scale;
};

//-------------------------------------------------------------------------------------------------

struct SRay
{
	vec3 Origin;
	
	vec3 Direction;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// Shader Interface

uniform SCamera Camera;

uniform float Time;

///////////////////////////////////////////////////////////////////////////////////////////////////
// Shader Constants

const vec3 BoxMinimum = vec3 ( -5.0 );	// Minimum point of bounding box

const vec3 BoxMaximum = vec3 ( 5.0 );	// Maximum point of bounding box

//-------------------------------------------------------------------------------------------------

const float Radius = 5.5;				// Radius of bounding sphere

//-------------------------------------------------------------------------------------------------

const vec3 Zero = vec3 ( 0.0 );

const vec3 Unit = vec3 ( 1.0 );

//-------------------------------------------------------------------------------------------------

const vec3 AxisX = vec3 ( 1.0, 0.0, 0.0 );

const vec3 AxisY = vec3 ( 0.0, 1.0, 0.0 );

const vec3 AxisZ = vec3 ( 0.0, 0.0, 1.0 );

///////////////////////////////////////////////////////////////////////////////////////////////////
// Intersection Functions

bool IntersectBox ( SRay ray, vec3 minimum, vec3 maximum, out float start, out float final )
{
	vec3 OMAX = ( minimum - ray.Origin ) / ray.Direction;
	
	vec3 OMIN = ( maximum - ray.Origin ) / ray.Direction;
	
	vec3 MAX = max ( OMAX, OMIN );
	
	vec3 MIN = min ( OMAX, OMIN );
	
	final = min ( MAX.x, min ( MAX.y, MAX.z ) );
	
	start = max ( max ( MIN.x, 0.0), max ( MIN.y, MIN.z ) );	
	
	return final > start;
}

//-------------------------------------------------------------------------------------------------

bool IntersectSphere ( SRay ray, float radius, out float start, out float final )
{
	float A = dot ( ray.Direction, ray.Direction );

	float B = dot ( ray.Direction, ray.Origin );

	float C = dot ( ray.Origin, ray.Origin ) - radius * radius;

	float D = B * B - A * C;
	
	if ( D > 0.0 )
	{
		D = sqrt ( D );

		start = max ( 0.0, ( -B - D ) / A );

		final = ( -B + D ) / A;

		return final > 0.0;
	}

	return false;
}

//-------------------------------------------------------------------------------------------------

float CalcFunction ( vec3 point )
{
	float x = point.x, y = point.y, z = point.z, T = 1.6180339887;

	return 2.0 - cos ( x + T * y ) - cos ( x - T * y ) - cos ( y + T * z ) -
		         cos ( y - T * z ) - cos ( z - T * x ) - cos ( z + T * x );

	//return sin ( x ) + sin ( y ) + sin ( z );
}

//-------------------------------------------------------------------------------------------------

#define STEP  0.01

vec3 CalcNormal ( vec3 point )
{
	float A = CalcFunction ( point - AxisX * STEP );
	float B = CalcFunction ( point + AxisX * STEP );

	float C = CalcFunction ( point - AxisY * STEP );
	float D = CalcFunction ( point + AxisY * STEP );

	float E = CalcFunction ( point - AxisZ * STEP );
	float F = CalcFunction ( point + AxisZ * STEP );

	return normalize ( vec3 ( B - A, D - C, F - E ) );
}

//-------------------------------------------------------------------------------------------------

#define INTERVALS 100

bool IntersectSurface ( SRay ray, float start, float final, out float val )
{
	float step = ( final - start ) / float ( INTERVALS );

	//-----------------------------------------------------------------------------------

	float time = start;

	vec3 point = ray.Origin + time * ray.Direction;

	//-----------------------------------------------------------------------------------

	float right, left = CalcFunction ( point );

	//-----------------------------------------------------------------------------------

	for ( float i = 0.0; i < float ( INTERVALS ); i++ )
	{
		time += step;

		point += step * ray.Direction;

		right = CalcFunction ( point );
		
		if ( left * right < 0.0 )
		{
			val = time + right * step / ( left - right );

			return true;
		}
		
		left = right;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Support Functions

SRay GenerateRay ( SCamera camera )
{
#if defined ( RENDER_TRUE_STEREO )

	float x = gl_TexCoord [0].x + mix ( -1.0, 1.0, float ( gl_TexCoord [0].x < 0.0 ) );

	vec2 coords = vec2 ( x, gl_TexCoord [0].y ) * camera.Scale;

#else

	vec2 coords = gl_TexCoord [0].xy * camera.Scale;

#endif


	
	vec3 direction = camera.View - camera.Side * coords.x + camera.Up * coords.y;
   
	return SRay ( camera.Position, normalize ( direction ) );
}

//-------------------------------------------------------------------------------------------------

#define K_A 0.2

#define K_D 0.8

#define K_S 0.8

#define P 64.0

vec3 Phong ( vec3 point, vec3 normal, vec3 color )
{
	vec3 light = normalize ( Camera.Position - point );
   
	float diffuse = abs ( dot ( light, normal ) );

	vec3 reflect = reflect ( -light, normal );

	float specular = pow ( abs ( dot ( reflect, light ) ), P );

	return K_A * Unit + diffuse * ( K_D * color + K_S * specular * Unit );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Shader Entry Point

#define USE_BOX_

#define USE_SPHERE

vec3 Raytrace ( SRay ray )
{
	vec3 result = Zero;
    
	float start, final, time;

#if defined ( USE_BOX )

	if ( IntersectBox ( ray, BoxMinimum, BoxMaximum, start, final ) )

#elif defined ( USE_SPHERE )

	if ( IntersectSphere ( ray, Radius, start, final ) )

#endif
	{
		if ( IntersectSurface ( ray, start, final, time ) )
		{
			vec3 point = ray.Origin + ray.Direction * time;
					
			vec3 normal = CalcNormal ( point );

			vec3 color = ( point - BoxMinimum ) / ( BoxMaximum - BoxMinimum );

			result = Phong ( point, normal, color );
		}
	}
	
	return result;
}

//-------------------------------------------------------------------------------------------------

#if defined ( TRUE_ANAGLYPH )

const mat3 MatrixL = mat3 ( 0.299, 0.000, 0.000,
					        0.587, 0.000, 0.000,
					        0.114, 0.000, 0.000 );

const mat3 MatrixR = mat3 ( 0.000, 0.000, 0.299,
					        0.000, 0.000, 0.587,
					        0.000, 0.000, 0.114 );

#elif defined ( COLOR_ANAGLYPH )

const mat3 MatrixL = mat3 ( 1.000, 0.000, 0.000,
					        0.000, 0.000, 0.000,
					        0.000, 0.000, 0.000 );

const mat3 MatrixR = mat3 ( 0.000, 0.000, 0.000,
					        0.000, 1.000, 0.000,
					        0.000, 0.000, 1.000 );

#elif defined ( HALF_COLOR_ANAGLYPH )

const mat3 MatrixL = mat3 ( 0.299, 0.000, 0.000,
					        0.587, 0.000, 0.000,
					        0.114, 0.000, 0.000 );

const mat3 MatrixR = mat3 ( 0.000, 0.000, 0.000,
					        0.000, 1.000, 0.000,
					        0.000, 0.000, 1.000 );

#elif defined ( GRAY_ANAGLYPH )

const mat3 MatrixL = mat3 ( 0.299, 0.000, 0.000,
					        0.587, 0.000, 0.000,
					        0.114, 0.000, 0.000 );

const mat3 MatrixR = mat3 ( 0.000, 0.299, 0.299,
					        0.000, 0.587, 0.587,
					        0.000, 0.114, 0.114 );

#elif defined ( OPTIMIZED_ANAGLYPH )

const mat3 MatrixL = mat3 ( 0.000, 0.000, 0.000,
					        0.700, 0.000, 0.000,
					        0.300, 0.000, 0.000 );

const mat3 MatrixR = mat3 ( 0.000, 0.000, 0.000,
					        0.000, 1.000, 0.000,
					        0.000, 0.000, 1.000 );

#endif

//-------------------------------------------------------------------------------------------------

void main ( void )
{
	vec3 result = Zero;

	//=============================================================================================

	#if defined ( RENDER_MONO )

	SRay ray = GenerateRay ( Camera );

	result = Raytrace ( ray );

	//=============================================================================================

	#elif defined ( RENDER_ANAGLYPH_FULL )

	SCamera camera = Camera;

	//-----------------------------------------------------------------------------------

	camera.Position = Camera.Position - Camera.Side * DELTA;

	SRay ray = GenerateRay ( camera );

	vec3 right = Raytrace ( ray );

	//-----------------------------------------------------------------------------------

	camera.Position = Camera.Position + Camera.Side * DELTA;

	ray = GenerateRay ( camera );

	vec3 left = Raytrace ( ray );

	//-----------------------------------------------------------------------------------

	result = MatrixL * left + MatrixR * right;

	#if defined ( OPTIMIZED_ANAGLYPH )

	result.r = pow ( result.r, 1.0 / 1.5) ;

	#endif

	//=============================================================================================

	#elif defined ( RENDER_ANAGLYPH_INTERLACED )

	SCamera camera = Camera;
	
	float line = mod ( floor ( gl_FragCoord.y ), 2.0 );
	
	camera.Position = Camera.Position + Camera.Side * mix ( -DELTA, DELTA, line );
	
	SRay ray = GenerateRay ( camera );
	
	result = Raytrace ( ray );
	
	result = mix ( MatrixR * result, MatrixL * result, line );

	#if defined ( OPTIMIZED_ANAGLYPH )

	result.r = pow ( result.r, 1.0 / 1.5) ;

	#endif

	//=============================================================================================

	#elif defined ( RENDER_TRUE_STEREO )

	SCamera camera = Camera;

	float side = float ( gl_TexCoord [0].x < 0.0 );
	
	camera.Position = Camera.Position + Camera.Side * mix ( -DELTA, DELTA, side );
	
	SRay ray = GenerateRay ( camera );
	
	result = Raytrace ( ray );

	#endif

	//=============================================================================================
	
	gl_FragColor = vec4 ( result, 1.0 );
}