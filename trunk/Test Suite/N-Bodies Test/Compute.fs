#define RUNGE_KUTTA_1

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

uniform sampler2D CurrentPositionTexture;

uniform sampler2D CurrentVelocityTexture;

//-----------------------------------------------------------------------------------------------------------

varying vec2 Particle;

//-----------------------------------------------------------------------------------------------------------

const int Count = 128;

const float TimeStep = 1e-4;

const float HalfStep = TimeStep / 2.0;

const float Epsilon = 1e-2;

const vec4 TextureStep = vec4( 1.0, 2.0, 3.0, 4.0 ) / Count;

const vec4 DoubleTextureStep = vec4( 5.0, 6.0, 7.0, 8.0 ) / Count;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

struct State
{
	vec3 Position;
	
	vec3 Velocity;
};

struct Derivative
{
	vec3 Velocity;
	
	vec3 Acceleration;
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

void PartialAcceleration( vec3 current, vec2 texcoords, inout vec3 acceleration )
{	
	vec4 position = texture2D( CurrentPositionTexture, texcoords );
							
	vec3 direction = vec3( position ) - current;
					
	float squaredist = dot( direction, direction ) + Epsilon;
					
	float sixthdist = squaredist * squaredist * squaredist;			
					
	acceleration += position.w * direction * inversesqrt( sixthdist );
}

#define UNROLL_4

vec3 Acceleration( vec3 current )
{	
	vec3 acceleration = vec3( 0.0 ); 
	
	#if defined(UNROLL_8)

	for ( float x = 0.0; x <= 0.9375; x += DoubleTextureStep[3] )
	{
		for ( float y = 0.0; y <= 0.9375; y += DoubleTextureStep[3] )
		{
			PartialAcceleration( current, vec2( x                 ,       y ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[0],       y ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[1],       y ), acceleration );				
			PartialAcceleration( current, vec2( x + TextureStep[2],       y ), acceleration );		
			PartialAcceleration( current, vec2( x + TextureStep[3],       y ), acceleration );
			PartialAcceleration( current, vec2( x + DoubleTextureStep[0], y ), acceleration );			
			PartialAcceleration( current, vec2( x + DoubleTextureStep[1], y ), acceleration );				
			PartialAcceleration( current, vec2( x + DoubleTextureStep[2], y ), acceleration );			
			
			//--------------------------------------------------------------------------------------------
						
			PartialAcceleration( current, vec2( x                 ,       y + TextureStep[0] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[0],       y + TextureStep[0] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[1],       y + TextureStep[0] ), acceleration );				
			PartialAcceleration( current, vec2( x + TextureStep[2],       y + TextureStep[0] ), acceleration );		
			PartialAcceleration( current, vec2( x + TextureStep[3],       y + TextureStep[0] ), acceleration );
			PartialAcceleration( current, vec2( x + DoubleTextureStep[0], y + TextureStep[0] ), acceleration );			
			PartialAcceleration( current, vec2( x + DoubleTextureStep[1], y + TextureStep[0] ), acceleration );				
			PartialAcceleration( current, vec2( x + DoubleTextureStep[2], y + TextureStep[0] ), acceleration );									
			
			//--------------------------------------------------------------------------------------------
			
			PartialAcceleration( current, vec2( x                 ,       y + TextureStep[1] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[0],       y + TextureStep[1] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[1],       y + TextureStep[1] ), acceleration );				
			PartialAcceleration( current, vec2( x + TextureStep[2],       y + TextureStep[1] ), acceleration );		
			PartialAcceleration( current, vec2( x + TextureStep[3],       y + TextureStep[1] ), acceleration );
			PartialAcceleration( current, vec2( x + DoubleTextureStep[0], y + TextureStep[1] ), acceleration );			
			PartialAcceleration( current, vec2( x + DoubleTextureStep[1], y + TextureStep[1] ), acceleration );				
			PartialAcceleration( current, vec2( x + DoubleTextureStep[2], y + TextureStep[1] ), acceleration );	
					
			//--------------------------------------------------------------------------------------------
			
			PartialAcceleration( current, vec2( x                 ,       y + TextureStep[2] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[0],       y + TextureStep[2] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[1],       y + TextureStep[2] ), acceleration );				
			PartialAcceleration( current, vec2( x + TextureStep[2],       y + TextureStep[2] ), acceleration );		
			PartialAcceleration( current, vec2( x + TextureStep[3],       y + TextureStep[2] ), acceleration );
			PartialAcceleration( current, vec2( x + DoubleTextureStep[0], y + TextureStep[2] ), acceleration );			
			PartialAcceleration( current, vec2( x + DoubleTextureStep[1], y + TextureStep[2] ), acceleration );				
			PartialAcceleration( current, vec2( x + DoubleTextureStep[2], y + TextureStep[2] ), acceleration );		
			
			//--------------------------------------------------------------------------------------------
			
			PartialAcceleration( current, vec2( x                 ,       y + TextureStep[3] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[0],       y + TextureStep[3] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[1],       y + TextureStep[3] ), acceleration );				
			PartialAcceleration( current, vec2( x + TextureStep[2],       y + TextureStep[3] ), acceleration );		
			PartialAcceleration( current, vec2( x + TextureStep[3],       y + TextureStep[3] ), acceleration );
			PartialAcceleration( current, vec2( x + DoubleTextureStep[0], y + TextureStep[3] ), acceleration );			
			PartialAcceleration( current, vec2( x + DoubleTextureStep[1], y + TextureStep[3] ), acceleration );				
			PartialAcceleration( current, vec2( x + DoubleTextureStep[2], y + TextureStep[3] ), acceleration );	
			
			//--------------------------------------------------------------------------------------------
			
			PartialAcceleration( current, vec2( x                 ,       y + DoubleTextureStep[0] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[0],       y + DoubleTextureStep[0] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[1],       y + DoubleTextureStep[0] ), acceleration );				
			PartialAcceleration( current, vec2( x + TextureStep[2],       y + DoubleTextureStep[0] ), acceleration );		
			PartialAcceleration( current, vec2( x + TextureStep[3],       y + DoubleTextureStep[0] ), acceleration );
			PartialAcceleration( current, vec2( x + DoubleTextureStep[0], y + DoubleTextureStep[0] ), acceleration );			
			PartialAcceleration( current, vec2( x + DoubleTextureStep[1], y + DoubleTextureStep[0] ), acceleration );				
			PartialAcceleration( current, vec2( x + DoubleTextureStep[2], y + DoubleTextureStep[0] ), acceleration );						
			
			//--------------------------------------------------------------------------------------------
			
			PartialAcceleration( current, vec2( x                 ,       y + DoubleTextureStep[1] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[0],       y + DoubleTextureStep[1] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[1],       y + DoubleTextureStep[1] ), acceleration );				
			PartialAcceleration( current, vec2( x + TextureStep[2],       y + DoubleTextureStep[1] ), acceleration );		
			PartialAcceleration( current, vec2( x + TextureStep[3],       y + DoubleTextureStep[1] ), acceleration );
			PartialAcceleration( current, vec2( x + DoubleTextureStep[0], y + DoubleTextureStep[1] ), acceleration );			
			PartialAcceleration( current, vec2( x + DoubleTextureStep[1], y + DoubleTextureStep[1] ), acceleration );				
			PartialAcceleration( current, vec2( x + DoubleTextureStep[2], y + DoubleTextureStep[1] ), acceleration );	
					
			//--------------------------------------------------------------------------------------------
			
			PartialAcceleration( current, vec2( x                 ,       y + DoubleTextureStep[2] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[0],       y + DoubleTextureStep[2] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[1],       y + DoubleTextureStep[2] ), acceleration );				
			PartialAcceleration( current, vec2( x + TextureStep[2],       y + DoubleTextureStep[2] ), acceleration );		
			PartialAcceleration( current, vec2( x + TextureStep[3],       y + DoubleTextureStep[2] ), acceleration );
			PartialAcceleration( current, vec2( x + DoubleTextureStep[0], y + DoubleTextureStep[2] ), acceleration );			
			PartialAcceleration( current, vec2( x + DoubleTextureStep[1], y + DoubleTextureStep[2] ), acceleration );				
			PartialAcceleration( current, vec2( x + DoubleTextureStep[2], y + DoubleTextureStep[2] ), acceleration );													
		}			
	}
	
	#elif defined(UNROLL_4)

	for ( float x = 0.0; x <= 0.96875; x += TextureStep[3] )
	{
		for ( float y = 0.0; y <= 0.96875; y += TextureStep[3] )
		{
			PartialAcceleration( current, vec2( x                 , y ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[0], y ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[1], y ), acceleration );				
			PartialAcceleration( current, vec2( x + TextureStep[2], y ), acceleration );
			
			//--------------------------------------------------------------------------------------------
			
			PartialAcceleration( current, vec2( x                 , y + TextureStep[0] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[0], y + TextureStep[0] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[1], y + TextureStep[0] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[2], y + TextureStep[0] ), acceleration );					
			
			//--------------------------------------------------------------------------------------------
			
			PartialAcceleration( current, vec2( x                 , y + TextureStep[1] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[0], y + TextureStep[1] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[1], y + TextureStep[1] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[2], y + TextureStep[1] ), acceleration );
					
			//--------------------------------------------------------------------------------------------
			
			PartialAcceleration( current, vec2( x                 , y + TextureStep[2] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[0], y + TextureStep[2] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[1], y + TextureStep[2] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[2], y + TextureStep[2] ), acceleration );											
		}			
	}
	
	#elif defined(UNROLL_2)

	for ( float x = 0.0; x <= 0.984375; x += TextureStep[1] )
	{
		for ( float y = 0.0; y <= 0.984375; y += TextureStep[1] )
		{
			PartialAcceleration( current, vec2( x                 , y ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[0], y ), acceleration );
			
			//--------------------------------------------------------------------------------------------
			
			PartialAcceleration( current, vec2( x                 , y + TextureStep[0] ), acceleration );			
			PartialAcceleration( current, vec2( x + TextureStep[0], y + TextureStep[0] ), acceleration );											
		}			
	}	
	
	#else
	
	for ( float x = 0.0; x < 1.0; x += TextureStep[0] )
	{
		for ( float y = 0.0; y < 1.0; y += TextureStep[0] )
		{
			PartialAcceleration( current, vec2( x, y ), acceleration );
		}			
	}
	
	#endif		
	
	return acceleration;
}

void UpdateState(inout State Y)
{

// [4 + 20 * N FLOPS]
#ifdef RUNGE_KUTTA_1

	// [20 * N FLOPS]
	Derivative D = Derivative( Y.Velocity,
	                           Acceleration( Y.Position ) );
	// [4 FLOPS]
	Y = State( Y.Position + TimeStep * D.Velocity,
	           Y.Velocity + TimeStep * D.Acceleration );
	           		
#endif

// [4 + 40 * N FLOPS]
#ifdef RUNGE_KUTTA_2

	// [20 * N FLOPS]
	Derivative K1 = Derivative( Y.Velocity,
	                            Acceleration( Y.Position ) );
	                                
	// [4 FLOPS]              
	State Y2 = State( Y.Position + HalfStep * K1.Velocity,
	                  Y.Velocity + HalfStep * K1.Acceleration );
	              
	// [20 * N FLOPS]	                  
	Derivative K2 = Derivative( Y2.Velocity,
	                            Acceleration( Y2.Position ) );
	// [4 FLOPS]	                               
	Y = State( Y.Position + TimeStep * K2.Velocity,
	           Y.Velocity + TimeStep * K2.Acceleration );	                                
	
#endif

// [28 + 80 * N FLOPS]
#ifdef RUNGE_KUTTA_4
	
	// [20 * N FLOPS]
	Derivative K1 = Derivative( Y.Velocity,
	                            Acceleration( Y.Position ) );
	                                
	// [4 FLOPS]             
	State Y2 = State( Y.Position + HalfStep * K1.Velocity,
	                  Y.Velocity + HalfStep * K1.Acceleration );
	               
	// [20 * N FLOPS]	                  
	Derivative K2 = Derivative( Y2.Velocity,
	                            Acceleration( Y2.Position ) );
	                                
	// [4 FLOPS]	                                
	State Y3 = State( Y.Position + HalfStep * K2.Velocity,
	                  Y.Velocity + HalfStep * K2.Acceleration );
	                  
	// [20 * N FLOPS]	                  
	Derivative K3 = Derivative( Y3.Velocity,
	                            Acceleration( Y3.Position ) );	         
	                                
	// [4 FLOPS]	                                
	State Y4 = State( Y.Position + TimeStep * K3.Velocity,
	                  Y.Velocity + TimeStep * K3.Acceleration );
	                
	// [20 * N FLOPS]	                  
	Derivative K4 = Derivative( Y4.Velocity,
	                            Acceleration( Y4.Position ) );
	                                
	                  
	// [10 FLOPS]	                                
	Derivative D = Derivative( K1.Velocity + 2.0 * K2.Velocity + 2.0 * K3.Velocity + K4.Velocity,
	                           K1.Acceleration + 2.0 * K2.Acceleration + 2.0 * K3.Acceleration + K4.Acceleration );
	             
	// [6 FLOPS]	                                
	Y = State( Y.Position + TimeStep * D.Velocity / 6.0,
	           Y.Velocity + TimeStep * D.Acceleration / 6.0 );
	
#endif

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

void main()
{
	vec4 position = texture2D( CurrentPositionTexture, Particle );
	
	vec4 velocity = texture2D( CurrentVelocityTexture, Particle );
	
	
	State Y = State( position, velocity );
		
	UpdateState( Y );
	
	
	gl_FragData[0] = vec4( Y.Position, position.w );
	
	gl_FragData[1] = vec4( Y.Velocity, 0.0 );
}