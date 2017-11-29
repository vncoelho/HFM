#ifndef BBV_CONSTRUCTIVE_Random_H_
#define BBV_CONSTRUCTIVE_Random_H_

#include "../../OptFrame/Constructive.hpp"

#include "ProblemInstance.h"

#include "Representation.h"
#include "Solution.h"

#include "Evaluator.h"

#include <list>

#include <algorithm>
#include <stdlib.h>

using namespace std;

using namespace optframe;

namespace BBV
{

class ConstructiveRandomBBV: public Constructive<RepBBV, MY_ADS>
{
private:
	ProblemInstanceBBV& pBBV;

   // Your private vars

public:
	
   ConstructiveRandomBBV(ProblemInstanceBBV& _pBBV);
	
	virtual ~ConstructiveRandomBBV();

	Solution<RepBBV, MY_ADS>& generateSolution();
};

}

#endif /*BBV_CONTRUCTIVE_Random_H_*/
