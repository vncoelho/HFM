#ifndef TP_EVALUATOR_H_
#define TP_EVALUATOR_H_

//#include <cmath>
#include <stdlib.h>
#include <math.h>

#include "../../OptFrame/Evaluation.hpp"
#include "../../OptFrame/Evaluator.hpp"

#include "Representation.h"
#include "Solution.h"
#include "Evaluation.h"

#include "ProblemInstance.h"


namespace TP
{

class TPEvaluator: public Evaluator<RepTP, MY_ADS>
{
private:
	ProblemInstance& pTP;


public:
	TPEvaluator(ProblemInstance& _pTP);
	
	virtual ~TPEvaluator();

	EvaluationTP evaluate(const RepTP& rep, const MY_ADS*);

	
	double getNodeValue(const Node* root, int s);

	virtual bool isMinimization() const;
};

}

#endif /*TP_EVALUATOR_H_*/

