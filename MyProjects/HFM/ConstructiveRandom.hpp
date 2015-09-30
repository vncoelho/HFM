#ifndef EFP_CONTRUCTIVE_RANDOM_HPP_
#define EFP_CONTRUCTIVE_RANDOM_HPP_

#include "../../OptFrame/Constructive.h"
#include "../../OptFrame/Util/TestSolution.hpp"
#include "../../OptFrame/Heuristics/GRASP/GRConstructive.h"

#include "ProblemInstance.hpp"

#include "Representation.h"
#include "Solution.h"

#include "Evaluator.hpp"

#include <list>

#include <algorithm>
#include <stdlib.h>
#include <set>

using namespace std;

namespace EFP
{

class ConstructiveRandom: public GRConstructive<RepEFP>
{
private:
	ProblemInstance& pEFP;
	RandGen& rg;

	int precisionSP;
	int precisionMP;
	int precisionDP;
	int precision;
	// Your private vars
	ProblemParameters problemParam;

	int maxLag, maxUpperLag;

public:

	ConstructiveRandom(ProblemInstance& _pEFP, ProblemParameters& _problemParam, RandGen& _rg, int _precision) : // If necessary, add more parameters
			pEFP(_pEFP), problemParam(_problemParam), rg(_rg), precision(_precision)
	{

		if (precision == 0)
		{
			cerr << "Precision should be greater than 0 :" << precision << endl;
			getchar();
		}

		maxLag = problemParam.getMaxLag();
		maxUpperLag = problemParam.getMaxUpperLag();
		precisionSP = precision;
		precisionMP = precision;
		precisionDP = precision;
		precisionMP = 1; //forcing to not have average rules
		precisionDP = 1;//forcing to do not have derivative rules
	}

	virtual ~ConstructiveRandom()
	{
	}

	Solution<RepEFP>& generateSolution()
	{
		return generateSolution(0);
	}

	Solution<RepEFP>& generateSolution(float notUsed)
	{

		//cout << "ACF generating solution.." << endl;

		vector<vector<double> > data = pEFP.getForecastingsVector();

		int numberExplanatoryVariables = data.size();

		int earliestInput = 0;
		vector<pair<int, int> > singleIndex;
		vector<vector<double> > singleFuzzyRS;
		vector<vector<pair<int, int> > > averageIndex;
		vector<vector<double> > averageFuzzyRS;
		vector<vector<pair<int, int> > > derivativeIndex;
		vector<vector<double> > derivativeFuzzyRS;

		for (int nEXV = 0; nEXV < numberExplanatoryVariables; nEXV++)
		{
			int mean = pEFP.getMean(nEXV);
			int stdDesv = pEFP.getStdDesv(nEXV);
			double meanWeight = pEFP.getMean(0); //File 0 is the target file
			double stdDesvWeight = pEFP.getStdDesv(0);

			int pSP = rg.rand(precisionSP);
			for (int p = 0; p < pSP; p++)
			{
				int K;
				if (maxUpperLag > 0)
				{
					int sign = rg.rand(2);
					if (sign == 0)
						K = rg.rand(maxLag) + 1;
					else
						K = (rg.rand(maxUpperLag) + 1) * -1;
				}
				else
					K = rg.rand(maxLag) + 1;

				singleIndex.push_back(make_pair(nEXV, K));

				if (K > earliestInput)
					earliestInput = K;

				double greater = rg.randG(mean, stdDesv);
				double lower = rg.randG(mean, stdDesv);
				double greaterWeight = rg.randG(meanWeight, stdDesvWeight);
				double lowerWeight = rg.randG(meanWeight, stdDesvWeight);

				int fuzzyFunction = rg.rand(NFUZZYFUNCTIONS);

				vector<double> fuzzyRules;
				fuzzyRules.resize(NCOLUMNATRIBUTES);
				fuzzyRules[GREATER] = greater;
				fuzzyRules[GREATER_WEIGHT] = greaterWeight;
				fuzzyRules[LOWER] = lower;
				fuzzyRules[LOWER_WEIGHT] = lowerWeight;
				fuzzyRules[EPSILON] = 1; //TODO TEST FOR TRAPEZOID
				fuzzyRules[PERTINENCEFUNC] = fuzzyFunction; //PERTINENCE FUNCTION

				singleFuzzyRS.push_back(fuzzyRules);
			}

			int pMP = rg.rand(precisionMP);
			for (int p = 0; p < pMP; p++)
			{
				int nAveragePoints = rg.rand(5) + 2;
				vector<pair<int, int> > aInputs;
				for (int aI = 0; aI < nAveragePoints; aI++)
				{
					int K;
					if (maxUpperLag > 0)
					{
						int sign = rg.rand(2);
						if (sign == 0)
							K = rg.rand(maxLag) + 1;
						else
							K = (rg.rand(maxUpperLag) + 1) * -1;
					}
					else
						K = rg.rand(maxLag) + 1;

					aInputs.push_back(make_pair(nEXV, K));
					if (K > earliestInput)
						earliestInput = K;
				}
				averageIndex.push_back(aInputs);

				double greater = rg.randG(mean, stdDesv);
				double lower = rg.randG(mean, stdDesv);
				double greaterWeight = rg.randG(meanWeight, stdDesvWeight);
				double lowerWeight = rg.randG(meanWeight, stdDesvWeight);

				int fuzzyFunction = rg.rand(NFUZZYFUNCTIONS);

				vector<double> fuzzyRules;
				fuzzyRules.resize(NCOLUMNATRIBUTES);
				fuzzyRules[GREATER] = greater;
				fuzzyRules[GREATER_WEIGHT] = greaterWeight;
				fuzzyRules[LOWER] = lower;
				fuzzyRules[LOWER_WEIGHT] = lowerWeight;
				fuzzyRules[EPSILON] = 1; //TODO TEST FOR TRAPEZOID
				fuzzyRules[PERTINENCEFUNC] = fuzzyFunction; //PERTINENCE FUNCTION

				averageFuzzyRS.push_back(fuzzyRules);
			}

			//cout << pMP << endl;
			//cout << averageFuzzyRS << endl;
			//cout << averageIndex << endl;
			//getchar();

			int pDP = rg.rand(precisionDP);
			for (int p = 0; p < pDP; p++)
			{
				int nDerivativePoints = 2;
				vector<pair<int, int> > aInputs;
				for (int aI = 0; aI < nDerivativePoints; aI++)
				{
					int K;
					if (maxUpperLag > 0)
					{
						int sign = rg.rand(2);
						if (sign == 0)
							K = rg.rand(maxLag) + 1;
						else
							K = (rg.rand(maxUpperLag) + 1) * -1;
					}
					else
						K = rg.rand(maxLag) + 1;

					aInputs.push_back(make_pair(nEXV, K));
					if (K > earliestInput)
						earliestInput = K;
				}
				derivativeIndex.push_back(aInputs);

				double greater = rg.randG(mean, stdDesv);
				double lower = rg.randG(mean, stdDesv);
				double greaterWeight = rg.randG(meanWeight, stdDesvWeight);
				double lowerWeight = rg.randG(meanWeight, stdDesvWeight);

				int fuzzyFunction = rg.rand(NFUZZYFUNCTIONS);

				vector<double> fuzzyRules;
				fuzzyRules.resize(NCOLUMNATRIBUTES);
				fuzzyRules[GREATER] = greater;
				fuzzyRules[GREATER_WEIGHT] = greaterWeight;
				fuzzyRules[LOWER] = lower;
				fuzzyRules[LOWER_WEIGHT] = lowerWeight;
				fuzzyRules[EPSILON] = 1; //TODO TEST FOR TRAPEZOID
				fuzzyRules[PERTINENCEFUNC] = fuzzyFunction; //PERTINENCE FUNCTION

				derivativeFuzzyRS.push_back(fuzzyRules);
			}

			//cout << pDP << endl;
			//cout << derivativeFuzzyRS << endl;
			//cout << derivativeIndex << endl;
			//getchar();
		}

		RepEFP newRep;
		newRep.singleIndex = singleIndex;
		newRep.singleFuzzyRS = singleFuzzyRS;
		newRep.averageIndex = averageIndex;
		newRep.averageFuzzyRS = averageFuzzyRS;
		newRep.derivativeIndex = derivativeIndex;
		newRep.derivativeFuzzyRS = derivativeFuzzyRS;
		//newRep.earliestInput = earliestInput;

		vector<double> vAlpha(NAJUSTS, rg.randG(0, 0.1));
		newRep.alpha = rg.rand(100) / 10000.0;
		newRep.vAlpha = vAlpha;

		vector<int> vIndex;
		vector<double> vIndexAlphas;

		int nAdjust = rg.rand(1) + 1;
		for (int nA = 0; nA <= nAdjust; nA++)
		{
			int K = rg.rand(maxLag);

			vIndex.push_back(K);
			double std = rg.rand(300) / 1000;
			vIndexAlphas.push_back(rg.randG(0, 0.1));
		}

		newRep.vIndex = vIndex;
		newRep.vIndexAlphas = vIndexAlphas;

		/*		cout << newRep.earliestInput << endl;
		 cout << newRep.singleIndex << endl;
		 cout << newRep.singleFuzzyRS << endl;
		 cout << newRep.averageIndex << endl;
		 cout << newRep.averageFuzzyRS << endl;
		 cout << newRep.derivativeIndex << endl;
		 cout << newRep.derivativeFuzzyRS << endl;
		 getchar();*/

//		cout << "End of Random Hybrid Fuzzy Model Sol generation!" << endl;
//		getchar();
		return *new TestSolution<RepEFP>(newRep);
	}

};

}

#endif /*EFP_CONTRUCTIVE_RANDOM_HPP_*/
