/*
   S U P P O R T   R A Y   T R A C I N G   L I B R A R Y

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

#include "BVHTree.h"

#include <algorithm>

#include <time.h>

#define BUILD_MEDIAN_TREE_

#define BUILD_BEST_AXIS_TREE

namespace Raytracing
{
	//==================================== BVHNode Class Subroutines ===================================

	const unsigned BVHNode :: Node = 1;

	const unsigned BVHNode :: Leaf = 2;

	const unsigned BVHNode :: None = -1;

	//----------------------------------- Constructor and Destructor -----------------------------------

	BVHNode :: BVHNode ( void )
	{
		Left = NULL;
		
		Right = NULL;

		Parent = NULL;
	}

	BVHNode :: ~BVHNode ( void )
	{
		delete Left;
		
		delete Right;
	}

	//==================================== BVHTree Class Subroutines ===================================

	const unsigned BVHTree :: VertexSize = 2048;

	const unsigned BVHTree :: MaterialSize = 8192;

	const unsigned BVHTree :: VolumeUnit = 0;
	
	const unsigned BVHTree :: PositionUnit = 1;
	
	const unsigned BVHTree :: NormalUnit = 2;

	const unsigned BVHTree :: TexCoordUnit = 3;

	//----------------------------------- Constructor and Destructor -----------------------------------

	BVHTree :: BVHTree ( void )
	{
		VolumeTexture = new Texture2D ( VolumeUnit, GL_TEXTURE_RECTANGLE_ARB );

		//------------------------------------------------------------------------------------
		
		PositionTexture = new Texture2D ( PositionUnit, GL_TEXTURE_RECTANGLE_ARB );
		
		NormalTexture = new Texture2D ( NormalUnit, GL_TEXTURE_RECTANGLE_ARB );

		TexCoordTexture = new Texture2D ( TexCoordUnit, GL_TEXTURE_RECTANGLE_ARB );

		//------------------------------------------------------------------------------------

		VolumeTexture->Data = new TextureData2D ( VertexSize, VertexSize, 4 );

		//------------------------------------------------------------------------------------

		PositionTexture->Data = new TextureData2D ( VertexSize, VertexSize, 4 );

		NormalTexture->Data = new TextureData2D ( VertexSize, VertexSize, 4 );

		TexCoordTexture->Data =  new TextureData2D ( VertexSize, VertexSize, 3 );
	}

	BVHTree :: ~BVHTree ( void )
	{
		delete VolumeTexture;

		delete PositionTexture;

		delete NormalTexture;

		delete TexCoordTexture;
	}

	//------------------------------ Building Sub-Tree for Specified Node ------------------------------

	bool CompareX ( const Triangle * t1, const Triangle * t2 )
	{
		float c1 = ( t1->VertexA->Position.X +
			         t1->VertexB->Position.X +
					 t1->VertexC->Position.X ) / 3.0F;

		float c2 = ( t2->VertexA->Position.X +
			         t2->VertexB->Position.X +
					 t2->VertexC->Position.X ) / 3.0F;

		return c1 < c2;
	}

	bool CompareY ( const Triangle * t1, const Triangle * t2 )
	{
		float c1 = ( t1->VertexA->Position.Y +
			         t1->VertexB->Position.Y +
					 t1->VertexC->Position.Y ) / 3.0F;

		float c2 = ( t2->VertexA->Position.Y +
			         t2->VertexB->Position.Y +
					 t2->VertexC->Position.Y ) / 3.0F;

		return c1 < c2;
	}

	bool CompareZ ( const Triangle * t1, const Triangle * t2 )
	{
		float c1 = ( t1->VertexA->Position.Z +
			         t1->VertexB->Position.Z +
					 t1->VertexC->Position.Z ) / 3.0F;

		float c2 = ( t2->VertexA->Position.Z +
			         t2->VertexB->Position.Z +
					 t2->VertexC->Position.Z ) / 3.0F;

		return c1 < c2;
	}

	void BVHTree :: BuildNode ( BVHNode * node )
	{
		vector <Triangle *> :: iterator triangle = node->Triangles.begin ( );

		Vector3D minimum = ( *triangle )->GetMinimum ( );

		Vector3D maximum = ( *triangle )->GetMaximum ( );

		while ( ++triangle != node->Triangles.end ( ) )
		{
			minimum = Min ( minimum, ( *triangle )->GetMinimum ( ) );

			maximum = Max ( maximum, ( *triangle )->GetMaximum ( ) );
		}
		
		node->Index = Index++;

		node->Minimum = minimum;

		node->Maximum = maximum;

		if ( node->Triangles.size ( ) <= 10 )
		{
			return;
		}

		//====================================================================================

		#ifdef BUILD_MEDIAN_TREE_
		
		Vector3D size = maximum - minimum;
		
		if ( size.X > size.Y )
		{
			if ( size.X > size.Z )
			{
				sort ( node->Triangles.begin ( ), node->Triangles.end ( ), CompareX );
			}
			else
			{
				sort ( node->Triangles.begin ( ), node->Triangles.end ( ), CompareZ );
			}
		}
		else
		{
			if ( size.Y > size.Z )
			{
				sort ( node->Triangles.begin ( ), node->Triangles.end ( ), CompareY );
			}
			else
			{
				sort ( node->Triangles.begin ( ), node->Triangles.end ( ), CompareZ );
			}
		}

		//------------------------------------------------------------------------------------

		BVHNode * left = new BVHNode ( );

		for ( int i = 0; i < node->Triangles.size ( ) / 2; i++ )
		{
			left->Triangles.push_back ( node->Triangles [i] );
		}

		node->Left = left;

		left->Parent = node;

		BVHNode * right = new BVHNode ( );

		for ( int i = node->Triangles.size ( ) / 2; i < node->Triangles.size ( ); i++ )
		{
			right->Triangles.push_back ( node->Triangles [i] );
		}
		
		node->Right = right;

		right->Parent = node;

		#endif
		
		//====================================================================================

		#ifdef BUILD_BEST_AXIS_TREE_

		float splitX = ( minimum.X + maximum.X ) / 2.0F;

		int leftCountX = 0, rightCountX = 0, resultX;

		for ( int i = 0; i < node->Triangles.size ( ); i++ )
		{
			if ( ( node->Triangles [i]->VertexA->Position.X +
				   node->Triangles [i]->VertexB->Position.X +
				   node->Triangles [i]->VertexC->Position.X ) / 3.0F < splitX )
			{
				leftCountX++;
			}
			else
			{
				rightCountX++;
			}
		}

		resultX = abs ( leftCountX - rightCountX );

		//------------------------------------------------------------------------------------

		float splitY = ( minimum.Y + maximum.Y ) / 2.0F;

		int leftCountY = 0, rightCountY = 0, resultY; 

		for ( int i = 0; i < node->Triangles.size ( ); i++ )
		{
			if ( ( node->Triangles [i]->VertexA->Position.Y +
				   node->Triangles [i]->VertexB->Position.Y +
				   node->Triangles [i]->VertexC->Position.Y ) / 3.0F < splitY )
			{
				leftCountY++;
			}
			else
			{
				rightCountY++;
			}
		}

		resultY = abs ( leftCountY - rightCountY );

		//------------------------------------------------------------------------------------
		
		float splitZ = ( minimum.Z + maximum.Z ) / 2.0F;

		int leftCountZ = 0, rightCountZ = 0, resultZ; 

		for ( int i = 0; i < node->Triangles.size ( ); i++ )
		{
			if ( ( node->Triangles [i]->VertexA->Position.Z +
				   node->Triangles [i]->VertexB->Position.Z +
				   node->Triangles [i]->VertexC->Position.Z ) / 3.0F < splitZ )
			{
				leftCountZ++;
			}
			else
			{
				rightCountZ++;
			}
		}

		resultZ = abs ( leftCountZ - rightCountZ );

		//------------------------------------------------------------------------------------

		BVHNode * left = new BVHNode ( );

		BVHNode * right = new BVHNode ( );

		left->Parent = node;

		right->Parent = node;

		if ( resultX < resultY )
		{
			if ( resultX < resultZ )
			{
				for ( int i = 0; i < node->Triangles.size ( ); i++ )
				{
					if ( ( node->Triangles [i]->VertexA->Position.X +
						   node->Triangles [i]->VertexB->Position.X +
						   node->Triangles [i]->VertexC->Position.X ) / 3.0F < splitX )
					{
						left->Triangles.push_back ( node->Triangles [i] );
					}
					else
					{
						right->Triangles.push_back ( node->Triangles [i] );
					}
				}
			}
			else
			{
				for ( int i = 0; i < node->Triangles.size ( ); i++ )
				{
					if ( ( node->Triangles [i]->VertexA->Position.Z +
						   node->Triangles [i]->VertexB->Position.Z +
						   node->Triangles [i]->VertexC->Position.Z ) / 3.0F < splitZ )
					{
						left->Triangles.push_back ( node->Triangles [i] );
					}
					else
					{
						right->Triangles.push_back ( node->Triangles [i] );
					}
				}
			}
		}
		else
		{
			if ( resultY < resultZ  )
			{
				for ( int i = 0; i < node->Triangles.size ( ); i++ )
				{
					if ( ( node->Triangles [i]->VertexA->Position.Y +
						   node->Triangles [i]->VertexB->Position.Y +
						   node->Triangles [i]->VertexC->Position.Y ) / 3.0F < splitY )
					{
						left->Triangles.push_back ( node->Triangles [i] );
					}
					else
					{
						right->Triangles.push_back ( node->Triangles [i] );
					}
				}
			}
			else
			{
				for ( int i = 0; i < node->Triangles.size ( ); i++ )
				{
					if ( ( node->Triangles [i]->VertexA->Position.Z +
						   node->Triangles [i]->VertexB->Position.Z +
						   node->Triangles [i]->VertexC->Position.Z ) / 3.0F < splitZ )
					{
						left->Triangles.push_back ( node->Triangles [i] );
					}
					else
					{
						right->Triangles.push_back ( node->Triangles [i] );
					}
				}
			}
		}

		node->Left = left;

		node->Right = right;

		#endif

		//====================================================================================

		BuildNode ( left );
		
		BuildNode ( right );
	}

	//---------------------------------------- Building BVH Tree ---------------------------------------
	
	void BVHTree :: Build ( vector <Primitive *>& primitives )
	{
		Index = 0;

		Root = new BVHNode ( );
		
		for ( unsigned index = 0; index < primitives.size ( ); index++ )
		{
			Root->Triangles.insert ( Root->Triangles.end ( ),
				                     primitives [index]->Triangles.begin ( ),
									 primitives [index]->Triangles.end ( ) );
		}

		//------------------------------------------------------------------------------------
		
		clock_t time = clock ( );
		
		BuildNode ( Root );
		
		cout << "Building BVH Tree: " << clock ( ) - time << " ms" << endl;
	}

	//------------------------------------- Drawing Specified Node -------------------------------------

	void BVHTree :: DrawNode ( BVHNode * node )
	{glDisable ( GL_LIGHTING );
		if ( NULL != node )
		{
			glColor3f ( 0.0F, 1.0F, 0.0F );

			glBegin ( GL_LINE_LOOP );		

				glVertex3f ( node->Minimum.X, node->Minimum.Y, node->Minimum.Z );
				glVertex3f ( node->Maximum.X, node->Minimum.Y, node->Minimum.Z );
				glVertex3f ( node->Maximum.X, node->Maximum.Y, node->Minimum.Z );
				glVertex3f ( node->Minimum.X, node->Maximum.Y, node->Minimum.Z );		

			glEnd ( );

			glBegin ( GL_LINE_LOOP );	

				glVertex3f ( node->Minimum.X, node->Minimum.Y, node->Maximum.Z );
				glVertex3f ( node->Maximum.X, node->Minimum.Y, node->Maximum.Z );
				glVertex3f ( node->Maximum.X, node->Maximum.Y, node->Maximum.Z );
				glVertex3f ( node->Minimum.X, node->Maximum.Y, node->Maximum.Z );		

			glEnd ( );

			glBegin ( GL_LINES );			

				glVertex3f ( node->Minimum.X, node->Minimum.Y, node->Minimum.Z );
				glVertex3f ( node->Minimum.X, node->Minimum.Y, node->Maximum.Z );
				glVertex3f ( node->Maximum.X, node->Minimum.Y, node->Minimum.Z );
				glVertex3f ( node->Maximum.X, node->Minimum.Y, node->Maximum.Z );
				glVertex3f ( node->Maximum.X, node->Maximum.Y, node->Minimum.Z );
				glVertex3f ( node->Maximum.X, node->Maximum.Y, node->Maximum.Z );
				glVertex3f ( node->Minimum.X, node->Maximum.Y, node->Minimum.Z );
				glVertex3f ( node->Minimum.X, node->Maximum.Y, node->Maximum.Z );	

			glEnd ( );

			//DrawNode ( node->Left );

			//DrawNode ( node->Right );
		}glEnable ( GL_LIGHTING );
	}

	//------------------------------------ Drawing Bounding Volumes ------------------------------------

	void BVHTree :: Draw ( void )
	{
		glDisable ( GL_LIGHTING );
		
		DrawNode ( Root );
		
		glEnable ( GL_LIGHTING );
	}

	//------------------------------ Setup Textures for Using from Shaders -----------------------------

	void BVHTree :: SetupNode ( BVHNode * node )
	{
		if ( NULL != node )
		{
			//cout << "NODE # " << node->Index << endl;

			int escape = BVHNode :: None;

			if ( NULL != node->Parent )
			{
				//cout << "Node has parent # " << node->Parent->Index << endl;

				if ( node == node->Parent->Left )
				{
					//cout << "Node is left child" << endl;

					escape = node->Parent->Right->Index;
				}
				else
				{
					//cout << "Node is right child" << endl;

					BVHNode * next = node->Parent;

					while ( Root != next && next == next->Parent->Right )
					{
						next = next->Parent;
					}

					if ( Root != next )
					{
						escape = next->Parent->Right->Index;
					}
				}
			}



			//cout << "Escape index is " << escape << endl;

			//--------------------------------------------------------------------------------

			if ( NULL != node->Left )
			{
				//cout << "Node has children" << endl;

				VolumeTexture->Data->Pixel <Vector2D> ( NodeOffset++ ) =
					Vector2D ( BVHNode :: Node, 3.0F * escape );

				if ( escape > 0 && 3 * escape < NodeOffset-1 )
					cout << "Error!";

				VolumeTexture->Data->Pixel <Vector3D> ( NodeOffset++ ) =
					node->Minimum;

				VolumeTexture->Data->Pixel <Vector3D> ( NodeOffset++ ) =
					node->Maximum;

				//cout << Vector2D ( BVHNode :: Node, 3.0F * escape ) << endl;
				//cout << node->Minimum << endl;
				//cout << node->Maximum << endl;

				//getchar ( );

				SetupNode ( node->Left );

				SetupNode ( node->Right );
			}
			else
			{
				//cout << "Node has NO children" << endl;

				VolumeTexture->Data->Pixel <Vector4D> ( NodeOffset++ ) =
					Vector4D ( BVHNode :: Leaf,
					           3.0F * escape,
					           VertexOffset,
							   node->Triangles.size ( ) );

				if ( escape > 0 && 3 * escape < NodeOffset-1 )
					cout << "Error!";

				VolumeTexture->Data->Pixel <Vector3D> ( NodeOffset++ ) =
					node->Minimum;

				VolumeTexture->Data->Pixel <Vector3D> ( NodeOffset++ ) =
					node->Maximum;

				//cout << Vector4D ( BVHNode :: Leaf, 3.0F * escape, VertexOffset, node->Triangles.size ( ) ) << endl;
				//cout << node->Minimum << endl;
				//cout << node->Maximum << endl;

				//getchar ( );

				for ( int i = 0; i < node->Triangles.size ( ); i++ )
				{
					Vector3D normal = node->Triangles [i]->GetNormal ( );

					//------------------------------------------------------------------------
					
					PositionTexture->Data->Pixel <Vector4D> ( VertexOffset ) =
						Vector4D ( node->Triangles [i]->VertexA->Position, normal.X );

					NormalTexture->Data->Pixel <Vector3D> ( VertexOffset ) = 
						node->Triangles [i]->VertexA->Normal;

					TexCoordTexture->Data->Pixel <Vector2D> ( VertexOffset++ ) = 
						node->Triangles [i]->VertexA->TexCoord;

					//------------------------------------------------------------------------
					
					PositionTexture->Data->Pixel <Vector4D> ( VertexOffset ) =
						Vector4D ( node->Triangles [i]->VertexB->Position, normal.Y );

					NormalTexture->Data->Pixel <Vector3D> ( VertexOffset ) = 
						node->Triangles [i]->VertexB->Normal;

					TexCoordTexture->Data->Pixel <Vector2D> ( VertexOffset++ ) = 
						node->Triangles [i]->VertexB->TexCoord;

					//------------------------------------------------------------------------

					PositionTexture->Data->Pixel <Vector4D> ( VertexOffset ) =
						Vector4D ( node->Triangles [i]->VertexC->Position, normal.Z );

					NormalTexture->Data->Pixel <Vector4D> ( VertexOffset ) = Vector4D (
						node->Triangles [i]->VertexC->Normal,
						node->Triangles [i]->Properties->Identifier );

					TexCoordTexture->Data->Pixel <Vector2D> ( VertexOffset++ ) =
						node->Triangles [i]->VertexC->TexCoord;
				}
			}
		}
	}

	void BVHTree :: SetupTextures ( void )
	{
		clock_t time  = clock ( ); 

		VertexOffset = 0;

		NodeOffset = 0;

		SetupNode ( Root );

		//for ( int i = 0; i < NodeOffset; i += 3 )
		//{
		//	if ( i >  VolumeTexture->Data->Pixel <Vector3D> ( i ).Y )
		//	{
		//		cout << "OFFSET = " << i << " | " << VolumeTexture->Data->Pixel <Vector3D> ( i ) << endl;
		//	}
		//}

		cout << "Building Vertex Data: " << clock ( ) - time << " ms" << endl << endl;

		float vermem = floor ( 100.0F * VertexOffset / ( VertexSize * VertexSize ) );

		cout << "Vertex Memory Used: " << vermem << "%" << endl << endl;

		VolumeTexture->Setup ( );

		PositionTexture->Setup ( );

		NormalTexture->Setup ( );

		TexCoordTexture->Setup ( );
	}

	//------------------------------------- Applying Settings to Shaders ------------------------------------

	void BVHTree :: SetShaderData ( ShaderManager * manager )
	{
		manager->SetTexture ( "VolumeTexture", VolumeTexture );

		manager->SetTexture ( "PositionTexture", PositionTexture );

		manager->SetTexture ( "NormalTexture", NormalTexture );

		manager->SetTexture ( "TexCoordTexture", TexCoordTexture );

		//manager->SetTexture ( "MaterialTexture", MaterialTexture );

		manager->SetUniformFloat ( "VertexTextureSize", VertexSize );
		
		manager->SetUniformFloat ( "VertexTextureStep", 1.0F / VertexSize );

		//manager->SetUniformFloat ( "MaterialTextureStep", 1.0F / MaterialSize );
	}
}