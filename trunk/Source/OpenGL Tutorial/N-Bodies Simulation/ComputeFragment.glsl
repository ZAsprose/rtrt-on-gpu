/*
   ---------------------------------------------------------------------------
   |             N - B O D I E S   S I M U L A T I O N   D E M O             |
   ---------------------------------------------------------------------------
                              
   Copyright (c) 2009 - 2010 Denis Bogolepov ( denisbogol @ gmail.com )

   This program is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation, either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License along
   with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#extension GL_ARB_texture_rectangle : enable

#define RUNGE_KUTTA_1

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

uniform sampler2DRect CurrentPositionTexture;

uniform sampler2DRect CurrentVelocityTexture;

//-----------------------------------------------------------------------------------------------------------

varying vec2 Particle;

//-----------------------------------------------------------------------------------------------------------

const int CountX = 128;

const int CountY = 128;

const float TimeStep = 1e-3;

const float HalfStep = TimeStep / 2.0;

const float Epsilon = 1e-2;

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

void PartialAcceleration ( vec3 current, vec2 texcoords, inout vec3 acceleration )
{	
	vec4 position = texture2DRect ( CurrentPositionTexture, texcoords );
							
	vec3 direction = vec3 ( position ) - current;
					
	float squaredist = dot ( direction, direction ) + Epsilon;
					
	float sixthdist = squaredist * squaredist * squaredist;			
					
	acceleration += position.w * direction * inversesqrt ( sixthdist );
}

#define UNROLL_4

vec3 Acceleration ( vec3 current )
{	
	vec3 acceleration = vec3 ( 0.0 );
	
	#if defined ( UNROLL_1 )
	
	for ( float x = 0.0; x < CountX; x ++ )
	{
		for ( float y = 0.0; y < CountY; y ++ )
		{
			PartialAcceleration ( current, vec2 ( x, y ), acceleration );
		}			
	}
	
	#elif defined ( UNROLL_2 )
	
	for ( float x = 0.0; x < CountX; x += 2.0 )
	{
		for ( float y = 0.0; y < CountY; y += 2.0 )
		{
			PartialAcceleration ( current, vec2 ( x       ,       y ), acceleration );			
			PartialAcceleration ( current, vec2 ( x + 1.0 ,       y ), acceleration );			
			PartialAcceleration ( current, vec2 ( x       , y + 1.0 ), acceleration );			
			PartialAcceleration ( current, vec2 ( x + 1.0 , y + 1.0 ), acceleration );
		}			
	}
	
	#elif defined ( UNROLL_4 )
	
	for ( float x = 0.0; x < CountX; x += 4.0 )
	{
		for ( float y = 0.0; y < CountY; y += 4.0 )
		{
			PartialAcceleration ( current, vec2 ( x       ,       y ), acceleration );			
			PartialAcceleration ( current, vec2 ( x + 1.0 ,       y ), acceleration );			
			PartialAcceleration ( current, vec2 ( x + 2.0 ,       y ), acceleration );			
			PartialAcceleration ( current, vec2 ( x + 3.0 ,       y ), acceleration );
			
			PartialAcceleration ( current, vec2 ( x       ,       y + 1.0 ), acceleration );			
			PartialAcceleration ( current, vec2 ( x + 1.0 ,       y + 1.0 ), acceleration );			
			PartialAcceleration ( current, vec2 ( x + 2.0 ,       y + 1.0 ), acceleration );			
			PartialAcceleration ( current, vec2 ( x + 3.0 ,       y + 1.0 ), acceleration );	
			
			PartialAcceleration ( current, vec2 ( x       ,       y + 2.0 ), acceleration );			
			PartialAcceleration ( current, vec2 ( x + 1.0 ,       y + 2.0 ), acceleration );			
			PartialAcceleration ( current, vec2 ( x + 2.0 ,       y + 2.0 ), acceleration );			
			PartialAcceleration ( current, vec2 ( x + 3.0 ,       y + 2.0 ), acceleration );	
			
			PartialAcceleration ( current, vec2 ( x       ,       y + 3.0 ), acceleration );			
			PartialAcceleration ( current, vec2 ( x + 1.0 ,       y + 3.0 ), acceleration );			
			PartialAcceleration ( current, vec2 ( x + 2.0 ,       y + 3.0 ), acceleration );			
			PartialAcceleration ( current, vec2 ( x + 3.0 ,       y + 3.0 ), acceleration );	
		}			
	}
	
	#endif
	
	return acceleration;
}

void UpdateState ( inout State Y )
{
#if defined ( RUNGE_KUTTA_2 )

	Derivative K1 = Derivative ( Y.Velocity,
	                             Acceleration ( Y.Position ) );
	                                          
	State Y2 = State ( Y.Position + HalfStep * K1.Velocity,
	                   Y.Velocity + HalfStep * K1.Acceleration );
	                                
	Derivative K2 = Derivative ( Y2.Velocity,
	                             Acceleration ( Y2.Position ) );
                               
	Y = State ( Y.Position + TimeStep * K2.Velocity,
	            Y.Velocity + TimeStep * K2.Acceleration );	                                
	
#elif defined ( RUNGE_KUTTA_4 )
	
	Derivative K1 = Derivative ( Y.Velocity,
	                             Acceleration ( Y.Position ) );
	                                
	State Y2 = State ( Y.Position + HalfStep * K1.Velocity,
	                   Y.Velocity + HalfStep * K1.Acceleration );
	               
	Derivative K2 = Derivative ( Y2.Velocity,
	                             Acceleration ( Y2.Position ) );
	                                
	State Y3 = State ( Y.Position + HalfStep * K2.Velocity,
	                   Y.Velocity + HalfStep * K2.Acceleration );
                  
	Derivative K3 = Derivative ( Y3.Velocity,
	                             Acceleration ( Y3.Position ) );	         
	                                
	State Y4 = State ( Y.Position + TimeStep * K3.Velocity,
	                   Y.Velocity + TimeStep * K3.Acceleration );
                  
	Derivative K4 = Derivative ( Y4.Velocity,
	                             Acceleration ( Y4.Position ) );
                                
	Derivative D = Derivative ( K1.Velocity + 2.0 * K2.Velocity + 2.0 * K3.Velocity + K4.Velocity,
	                            K1.Acceleration + 2.0 * K2.Acceleration + 2.0 * K3.Acceleration + K4.Acceleration );
                                
	Y = State ( Y.Position + TimeStep * D.Velocity / 6.0,
	            Y.Velocity + TimeStep * D.Acceleration / 6.0 );
	
#endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

void main ( void )
{
	vec4 position = texture2DRect ( CurrentPositionTexture, vec2 ( gl_TexCoord[0] ) );
	
	vec4 velocity = texture2DRect ( CurrentVelocityTexture, vec2 ( gl_TexCoord[0] ) );
		
#if defined ( RUNGE_KUTTA_1 )

	State Y = State ( vec3 ( position + TimeStep * velocity ),
	                  vec3 ( velocity ) + TimeStep * Acceleration ( vec3 ( position ) ) );
	            
#else

	State Y = State ( vec3 ( position ), vec3 ( velocity ) );
	
	UpdateState ( Y );

#endif	
	
	gl_FragData[0] = vec4 ( Y.Position, position.w );
	
	gl_FragData[1] = vec4 ( Y.Velocity, 0.0 );
}