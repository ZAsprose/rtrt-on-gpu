/*
 * Author: Denis Bogolepov  ( denisbogol@sandy.ru )
 */

#pragma once

#include <Texture1D.h>

#include <Texture2D.h>

#include <Texture3D.h>

#include "UniformGrid.h"

#include "Scene.h"

using namespace Render;

namespace Raytracing
{
	class Data
	{
		public:

			//-------------------------------- Constant Parameters --------------------------------

			static const unsigned VertexSize;

			static const unsigned MaterialSize;

			static const unsigned VoxelUnit;

			static const unsigned PositionUnit;

			static const unsigned NormalUnit;

			static const unsigned MaterialUnit;

			//----------------------------------- Data Textures -----------------------------------

			Texture3D * VoxelTexture;

			Texture2D * PositionTexture;

			Texture2D * NormalTexture;

			Texture1D * MaterialTexture;

			//---------------------------- Constructor and Destructor -----------------------------

			Data ( void );

			~Data ( void );

			//----------------------------- Load Static Texture Data ------------------------------

			void SetupTextures ( Scene * );

			//----------------------------------- Apply Settings ----------------------------------

			void SetShaderData ( ShaderManager * );
	};
}