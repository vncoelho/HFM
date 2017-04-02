#include "ConstructiveRandom.h"

using namespace BBV;

ConstructiveRandomBBV::ConstructiveRandomBBV(ProblemInstanceBBV& _pBBV):
        pBBV(_pBBV)
{
}
	
ConstructiveRandomBBV::~ConstructiveRandomBBV()
{
}

Solution<RepBBV, MY_ADS>& ConstructiveRandomBBV::generateSolution()
{
    RepBBV newRep;

    return * new TestSolution<RepBBV, MY_ADS>(newRep);			
}
