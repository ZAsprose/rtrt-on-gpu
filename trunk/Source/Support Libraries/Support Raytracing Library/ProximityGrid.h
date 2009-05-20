#pragma once

#include "UniformGrid.h"

namespace Raytracing
{
	class ProximityGrid : public UniformGrid
	{
		private:
			
			//----------------------- Proximity Grid Saito Distance Maps --------------------------

			int ***saitoFirstMap;

			int ***saitoSecondMap;

			int ***saitoDistanceMap;

			//----------------------- Calculating Saito Distance Maps --------------------------			
			
			void CalculateFirstSaitoMap();

			void CalculateSecondSaitoMap();

			void CalculateDistanceSaitoMap();

			void NormolizeAndApllyDistanceMap();

		public:

			//---------------------------- Constructor and Destructor -----------------------------
			
			ProximityGrid ( int = 16, int = 16, int = 16 );
		
			~ProximityGrid ();

			//------------------------------- Building Proximity Grid -------------------------------

			virtual void BuildGrid ( Volume *, vector < Triangle * > );
	};
}