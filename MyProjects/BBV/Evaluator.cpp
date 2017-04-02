#include "Evaluator.h"

using namespace BBV;

EvalutorBBV::EvalutorBBV(ProblemInstanceBBV& _pBBV): // If necessary, add more parameters
pBBV(_pBBV)
{
	// Put the rest of your code here
}
	
EvalutorBBV::~EvalutorBBV()
{
}

Evaluation& EvalutorBBV::evaluate(const RepBBV& rep)
{
    // 'rep' is the representation of the solution

    double fo = 0; // Evaluation Function Value

    return * new Evaluation(fo);
}


bool EvalutorBBV::isMinimization() const
{
	// MAXIMIZATION
	return false;
}
