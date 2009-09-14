#pragma once

#include "UniformGrid.h"

namespace Raytracing
{
	class ProximityGrid : public UniformGrid
	{
		private:
			
			//----------------------- Proximity Grid Saito Distance Maps --------------------------

			int *** saitoFirstMap;

			int *** saitoSecondMap;

			int *** saitoDistanceMap;

			int *** simpleDistanceMap;

			//----------------------- Calculating Saito Distance Maps --------------------------			
			
			void CalculateFirstSaitoMap ( void );

			void CalculateSecondSaitoMap ( void );

			void CalculateDistanceSaitoMap ( void );

			void NormolizeAndApllyDistanceMap ( void );

			//--------------------------------- Test Subroutines ----------------------------------			
			
			bool CheckVoxelProximity ( int, int, int, int );

			void BuildDistanceMap ( void );

		public:

			//---------------------------- Constructor and Destructor -----------------------------
			
			ProximityGrid ( int = 16, int = 16, int = 16, Volume * = NULL );
		
			~ProximityGrid ();

			//------------------------------- Building Proximity Grid -------------------------------

			virtual void BuildGrid ( vector <Primitive *>& );
	};
}