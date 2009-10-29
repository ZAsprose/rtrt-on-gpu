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

#pragma once

#ifndef _BVH_TREE_

#define _BVH_TREE_

#include "Volume.h"

#include "Intersector.h"

#include "Primitive.h"

#include <ShaderManager.h>

namespace Raytracing
{
	struct BVHNode
	{
		//------------------------ Identifiers for Node and Leaf Elements -------------------------
		
		static const unsigned Node;

		static const unsigned Leaf;

		static const unsigned None;

		//-------------------------------------- Node Index ---------------------------------------

		int Index;

		//------------------------ Minimum and Maximum Node Points ( AABB ) -----------------------

		Vector3D Minimum;

		Vector3D Maximum;

		//--------------------------------- Child and Parent Nodes --------------------------------

		BVHNode * Left;

		BVHNode * Right;
		
		BVHNode * Parent;

		//----------------------------- List of Intersected Triangles -----------------------------

		vector <Triangle *> Triangles;

		//------------------------------- Constructor and Destructor ------------------------------

		BVHNode ( void );

		~BVHNode ( void );
	};

	//---------------------------------------------------------------------------------------------

	class BVHTree
	{
		public:

			//---------------------- Building Subdivision for Specified Node ----------------------

			void BuildNode ( BVHNode * );

			//------------------------------ Drawing Specified Node -------------------------------

			void DrawNode ( BVHNode * );

			//----------------------- Setup Textures for Using from Shaders -----------------------

			void SetupNode ( BVHNode * );

			int VertexOffset;

			int NodeOffset;

			int Index;

		public:

			//------------------------ Texture Units for Storing BVH Data -------------------------

			static const unsigned VertexSize;

			static const unsigned MaterialSize;

			static const unsigned VolumeUnit;

			static const unsigned PositionUnit;

			static const unsigned NormalUnit;

			static const unsigned TexCoordUnit;

			//----------------------------------- Data Textures -----------------------------------

			Texture2D * VolumeTexture;

			Texture2D * PositionTexture;

			Texture2D * NormalTexture;

			Texture2D * TexCoordTexture;

			//------------------------- Tree of Bounding Volumes for Scene ------------------------

			BVHNode * Root;			

			//----------------------------- Constructor and Destructor ----------------------------
		
			BVHTree ( );
			
			~BVHTree ( void );

			//-------------------- Building Bounding Volumes Hierarchy ( BVH ) --------------------

			void Build ( vector <Primitive *>& );

			//----------------------------- Drawing Bounding Volumes ------------------------------

			void Draw ( void );

			//----------------------- Setup Textures for Using from Shaders -----------------------

			void SetupTextures ( void );

			void SetShaderData ( ShaderManager * );
	};
}

#endif