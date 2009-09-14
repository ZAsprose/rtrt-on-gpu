/*
   Support Raytracing Library  
   Copyright (C) 2009  Denis Bogolepov ( bogdencmc@inbox.ru )

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see http://www.gnu.org/licenses.
 */

#include "Intersector.h"

#include <math.h>

//----------------------------------------- Useful Macro ------------------------------------------

#define CrossX(source) Vector2D ( -source.Z, source.Y )

#define CrossY(source) Vector2D ( source.Z, -source.X ) 

#define CrossZ(source) Vector2D ( -source.Y, source.X )

#define DotXY(left, right) ( left.X * right.X + left.Y * right.Y )

#define DotXZ(left, right) ( left.X * right.X + left.Y * right.Z )

#define DotYZ(left, right) ( left.X * right.Y + left.Y * right.Z )

#define AbsDotXY(left, right) ( fabs( left.X ) * right.X + fabs( left.Y ) * right.Y )

#define AbsDotXZ(left, right) ( fabs( left.X ) * right.X + fabs( left.Y ) * right.Z )

#define AbsDotYZ(left, right) ( fabs( left.X ) * right.Y + fabs( left.Y ) * right.Z )

//-------------------------------------------------------------------------------------------------

namespace Raytracing
{
	//-------------------------------------- Triangle edges ---------------------------------------

	Vector3D Intersector :: MovedA ( 0.0F, 0.0F, 0.0F );
				
	Vector3D Intersector :: MovedB ( 0.0F, 0.0F, 0.0F );
				
	Vector3D Intersector :: MovedC ( 0.0F, 0.0F, 0.0F );
			
	//-------------------------------------- Triangle edges ---------------------------------------
				
	Vector3D Intersector :: EdgeAB ( 0.0F, 0.0F, 0.0F );
						
	Vector3D Intersector :: EdgeBC ( 0.0F, 0.0F, 0.0F );
						
	Vector3D Intersector :: EdgeCA ( 0.0F, 0.0F, 0.0F );		
						
	//------------------------------------- Voxel parameters --------------------------------------
				
	Vector3D Intersector :: Radius ( 0.0F, 0.0F, 0.0F );

	//------------------------------- Support Tests from SAT Theorem ------------------------------

	bool Intersector :: PlaneVoxelOverlap ( const Vector3D& normal, float distance )
	{
		Vector3D minimum = -Sign ( normal ) * Radius;						
		
		if ( Dot ( normal, minimum ) + distance <= 0.0F )
		{
			Vector3D maximum = Sign ( normal ) * Radius;
			
			if ( Dot ( normal, maximum ) + distance >= 0.0F )
			{
				return true;
			}
		}

		return false;
	}
	
	bool Intersector :: SeparateAxisX ( void )
	{
		//---------------------------- Testing Axis [X x AB] ----------------------------
			
		{
			Vector2D axis = CrossX ( EdgeAB );
				
			Vector2D projection ( DotYZ ( axis, MovedA ), DotYZ ( axis, MovedC ) );
				
			float radius = AbsDotYZ ( axis, Radius );
				
			if ( projection < -radius || projection > radius ) return true;
		}
		
		//---------------------------- Testing Axis [X x BC] ----------------------------
			
		{
			Vector2D axis = CrossX ( EdgeBC );
				
			Vector2D projection ( DotYZ ( axis, MovedA ), DotYZ ( axis, MovedC ) );
				
			float radius = AbsDotYZ ( axis, Radius );
				
			if ( projection < -radius || projection > radius ) return true;
		}
		
		//---------------------------- Testing Axis [X x CA] ----------------------------
			
		{
			Vector2D axis = CrossX ( EdgeCA );
				
			Vector2D projection ( DotYZ ( axis, MovedA ), DotYZ ( axis, MovedB ) );
				
			float radius = AbsDotYZ ( axis, Radius );
				
			if ( projection < -radius || projection > radius ) return true;
		}
		
		return false;
	}

	bool Intersector :: SeparateAxisY ( void )
	{
		//---------------------------- Testing Axis [Y x AB] ----------------------------
			
		{
			Vector2D axis = CrossY ( EdgeAB );
				
			Vector2D projection ( DotXZ ( axis, MovedA ), DotXZ ( axis, MovedC ) );
				
			float radius = AbsDotXZ ( axis, Radius );
				
			if ( projection < -radius || projection > radius ) return true;
		}
		
		//---------------------------- Testing Axis [Y x BC] ----------------------------
			
		{
			Vector2D axis = CrossY ( EdgeBC );
				
			Vector2D projection ( DotXZ ( axis, MovedA ), DotXZ ( axis, MovedC ) );
				
			float radius = AbsDotXZ ( axis, Radius );
				
			if ( projection < -radius || projection > radius ) return true;
		}
		
		//---------------------------- Testing Axis [Y x CA] ----------------------------
			
		{
			Vector2D axis = CrossY ( EdgeCA );
				
			Vector2D projection ( DotXZ ( axis, MovedA ), DotXZ ( axis, MovedB ) );
				
			float radius = AbsDotXZ ( axis, Radius );
				
			if ( projection < -radius || projection > radius ) return true;
		}
		
		return false;
	}

	bool Intersector :: SeparateAxisZ ( void )
	{
		//---------------------------- Testing Axis [Z x AB] ----------------------------
			
		{
			Vector2D axis = CrossZ ( EdgeAB );
				
			Vector2D projection ( DotXY ( axis, MovedB ), DotXY ( axis, MovedC ) );
				
			float radius = AbsDotXY ( axis, Radius );
				
			if ( projection < -radius || projection > radius ) return true;
		}
		
		//---------------------------- Testing Axis [Z x BC] ----------------------------
			
		{
			Vector2D axis = CrossZ ( EdgeBC );
				
			Vector2D projection ( DotXY ( axis, MovedA ), DotXY ( axis, MovedB ) );
				
			float radius = AbsDotXY ( axis, Radius );
				
			if ( projection < -radius || projection > radius ) return true;
		}
		
		//---------------------------- Testing Axis [Z x CA] ----------------------------
			
		{
			Vector2D axis = CrossZ ( EdgeCA );
				
			Vector2D projection ( DotXY ( axis, MovedB ), DotXY ( axis, MovedC ) );
				
			float radius = AbsDotXY ( axis, Radius );
				
			if ( projection < -radius || projection > radius ) return true;
		}
		
		return false;
	}

	//------------------------------ Testing for Triangle-Box Overlap -----------------------------

	bool Intersector :: TriangleVoxelOverlap ( const Triangle * triangle,
		                                       Vector3D position,
											   Vector3D radius )
	{
		//----------------------------- Prepare input data ------------------------------
			
		{
			MovedA = triangle->VertexA->Position - position;
				
			MovedB = triangle->VertexB->Position - position;
			
			MovedC = triangle->VertexC->Position - position;
			
			EdgeAB = MovedB - MovedA;
			
			EdgeBC = MovedC - MovedB;
			
			EdgeCA = MovedA - MovedC;
			
			Radius = radius;
		}
		
		//-------------- Execute 9 tests for every possible pairs of edges --------------

		{
			if ( SeparateAxisX ( ) ) return false;
				
			if ( SeparateAxisY ( ) ) return false;
										
			if ( SeparateAxisZ ( ) ) return false;
		}
		
		//----------------- Execute test for bounding boxes intersection ----------------
		
		{
			Vector3D minimum = Min ( Min ( MovedA, MovedB ), MovedC );

			Vector3D maximum = Max ( Max ( MovedA, MovedB ), MovedC );

			if ( minimum.X > Radius.X || maximum.X < -Radius.X ) return false;

			if ( minimum.Y > Radius.Y || maximum.Y < -Radius.Y ) return false;

			if ( minimum.Z > Radius.Z || maximum.Z < -Radius.Z ) return false;
		}
		
		//------------------ Execute test for plane-voxel intersection ------------------
			
		{
			Vector3D normal = Cross ( EdgeAB, EdgeBC );
				
			float distance = -Dot ( normal, MovedA );
				
			if ( !PlaneVoxelOverlap ( normal, distance ) ) return false;
		}

		return true;
	}
}