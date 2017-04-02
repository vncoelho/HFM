#ifndef BBV_EVALUATOR_H_
#define BBV_EVALUATOR_H_

//#include <cmath>
#include <stdlib.h>
#include <math.h>

#include "../../OptFrame/Evaluation.hpp"
#include "../../OptFrame/Evaluator.hpp"

#include "Representation.h"
#include "Solution.h"

#include "ProblemInstance.h"

#define EPSILON_BBV 0.0001


namespace BBV
{

class EvalutorBBV: public Evaluator<RepBBV, MY_ADS >
{
private:
	ProblemInstanceBBV& pBBV;

	// Your private vars

public:
	EvalutorBBV(ProblemInstanceBBV& _pBBV);
	
	virtual ~EvalutorBBV();

	Evaluation& evaluate(const RepBBV& rep);
	
	//Evaluation& evaluate(const RepBBV& rep, const MY_ADS&);
	
	virtual bool isMinimization() const;
};

}

#endif /*BBV_EVALUATOR_H_*/

