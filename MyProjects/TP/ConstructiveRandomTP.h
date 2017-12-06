#ifndef TP_CONSTRUCTIVE_Random_H_
#define TP_CONSTRUCTIVE_Random_H_

#include "../../OptFrame/RandGen.hpp"

#include "../../OptFrame/Constructive.hpp"
#include "ProblemInstance.h"

#include "Representation.h"
#include "Solution.h"

#include "Evaluator.h"

//#include "../../OptFrame/Heuristics/GRASP/GRConstructive.hpp"

#include <list>

#include <algorithm>
#include <stdlib.h>

using namespace std;

using namespace optframe;

namespace TP
{

class TPConstructiveRandom: public Constructive<RepTP, MY_ADS>
{
private:
	ProblemInstance& pTP;
	RandGen& rg;
	int nodeMaxChildren;
	int treeMaxDepth;

	// Your private vars

public:

	TPConstructiveRandom(ProblemInstance& _pTP, RandGen& _rg, int _nodeMaxChildren, int _treeMaxDepth);

	virtual ~TPConstructiveRandom();

    Solution<RepTP, MY_ADS>* generateSolution(double timelimit) = 0;

	void fillNode(Node& previousNode, int nOperators, int nVariables, int nodeMaxChildren, int treeMaxDepth);
};

}

#endif /*TP_CONTRUCTIVE_Random_H_*/
