/*
 * ForecastClass.hpp
 *
 *  Created on: 10/09/2014
 *      Author: vitor
 */

#ifndef FORECASTCLASS_HPP_
#define FORECASTCLASS_HPP_

#include "Evaluator.hpp"
#include "Evaluation.h"
#include "ProblemInstance.hpp"
#include "HFMESContinous.hpp"
#include "treatForecasts.hpp"
#include "../../OptFrame/Heuristics/EvolutionaryAlgorithms/ES.hpp"
#include "../../OptFrame/Heuristics/Empty.hpp"
#include "../../OptFrame/NSSeq.hpp"
#include "../../OptFrame/Heuristics/LocalSearches/VND.h"
#include "../../OptFrame/Heuristics/LocalSearches/VariableNeighborhoodDescent.hpp"
#include "../../OptFrame/Heuristics/LocalSearches/FirstImprovement.hpp"
#include "../../OptFrame/Heuristics/LocalSearches/RandomDescentMethod.hpp"
#include "../../OptFrame/Heuristics/ILS/IteratedLocalSearchLevels.hpp"
#include "../../OptFrame/Heuristics/GRASP/BasicGRASP.hpp"
#include "../../OptFrame/Heuristics/VNS/MOVNSLevels.hpp"
#include "../../OptFrame/Heuristics/2PPLS.hpp"
#include "../../OptFrame/MultiEvaluator.hpp"
#include "../../OptFrame/MultiObjSearch.hpp"
#include "../../OptFrame/Util/UnionNDSets.hpp"

namespace EFP
{

class ForecastClass
{
private:
	ProblemInstance* p;
	EFPEvaluator* eval;
	GRConstructive<RepEFP>* c;
	vector<NSSeq<RepEFP>*> vNS;
	RandGen& rg;
	EmptyLocalSearch<RepEFP> emptyLS;
	vector<NSSeq<RepEFP>*> vNSeq;

	EFPESContinous* EsCOpt;
	ES<RepEFP>* es;

	vector<vector<double> >& tForecastings;
	vector<LocalSearch<RepEFP>*> vLS;

	VariableNeighborhoodDescent<RepEFP>* vnd;
	IteratedLocalSearchLevels<RepEFP>* ils;

	vector<vector<double> > vBlindResults;
	vector<pair<Solution<RepEFP>&, Evaluation&>*> vFinalSol;

	ILSLPerturbationLPlus2<RepEFP>* ilsPert;
	vector<NS<RepEFP>*> vizPert;

	ProblemParameters& problemParam;
	methodParameters& methodParam;

	//OptimalLinearRegression* olr;

public:

	ForecastClass(vector<vector<double> >& _tForecastings, ProblemParameters& _problemParam, RandGen& _rg, methodParameters& _methodParam) :
			tForecastings(_tForecastings), problemParam(_problemParam), rg(_rg), methodParam(_methodParam)
	{

		p = new ProblemInstance(tForecastings, problemParam);
		eval = new EFPEvaluator(*p, problemParam, methodParam.getEvalFOMinimizer(), methodParam.getEvalAprox());

		int nIntervals = 5;

		NSSeqNEIGHModifyRules* nsModifyFuzzyRules = new NSSeqNEIGHModifyRules(*p, rg);
		NSSeqNEIGHVAlpha* nsVAlpha = new NSSeqNEIGHVAlpha(*p, rg, nIntervals);
		NSSeqNEIGHChangeSingleInput* nsChangeSingleInput = new NSSeqNEIGHChangeSingleInput(*p, rg, problemParam.getMaxLag(), problemParam.getMaxUpperLag());
		NSSeqNEIGHRemoveSingleInput* nsRemoveSingleInput = new NSSeqNEIGHRemoveSingleInput(rg);
		NSSeqNEIGHAddSingleInput* nsAddSingleInput = new NSSeqNEIGHAddSingleInput(*p, rg, problemParam.getMaxLag(), problemParam.getMaxUpperLag());
//		NSSeqNEIGHAddX* nsAddMean01 = new NSSeqNEIGHAddX(*p, rg, 0.1);
//		NSSeqNEIGHAddX* nsAddMean1 = new NSSeqNEIGHAddX(*p, rg, 1);
//		NSSeqNEIGHAddX* nsAddMeanD15 = new NSSeqNEIGHAddX(*p, rg, p->getMean(0) / 15);
//		NSSeqNEIGHAddX* nsAddMeanD6 = new NSSeqNEIGHAddX(*p, rg, p->getMean(0) / 6);
//		NSSeqNEIGHAddX* nsAddMeanD2 = new NSSeqNEIGHAddX(*p, rg, p->getMean(0) / 2);
//		NSSeqNEIGHAddX* nsAddMean = new NSSeqNEIGHAddX(*p, rg, p->getMean(0));
//		NSSeqNEIGHAddX* nsAddMeanM2 = new NSSeqNEIGHAddX(*p, rg, 2 * p->getMean(0));
//		NSSeqNEIGHAddX* nsAddMeanM5 = new NSSeqNEIGHAddX(*p, rg, 5 * p->getMean(0));
//		NSSeqNEIGHAddX* nsAddMeanBigM = new NSSeqNEIGHAddX(*p, rg, 100 * p->getMean(0));

		int cMethod = methodParam.getConstrutiveMethod();
		int cPre = methodParam.getConstrutivePrecision();

		if (cMethod == 0)
			c = new ConstructiveRandom(*p, problemParam, rg, cPre);
		if (cMethod == 2)
			c = new ConstructiveACF(*p, problemParam, rg, cPre, methodParam.getConstrutiveLimitAlphaACF());

		FirstImprovement<RepEFP>* fiModifyFuzzyRules = new FirstImprovement<RepEFP>(*eval, *nsModifyFuzzyRules);
		FirstImprovement<RepEFP>* fiChangeSingleInput = new FirstImprovement<RepEFP>(*eval, *nsChangeSingleInput);
		FirstImprovement<RepEFP>* fiVAlpha = new FirstImprovement<RepEFP>(*eval, *nsVAlpha);
//		int maxRDM = 100;
//		RandomDescentMethod<RepEFP>* rdm = new RandomDescentMethod<RepEFP>(*eval, *ns, maxRDM);
//		//rdm->setMessageLevel(3);
		vLS.push_back(fiVAlpha);
		vLS.push_back(fiModifyFuzzyRules);
		vLS.push_back(fiChangeSingleInput);
		vnd = new VariableNeighborhoodDescent<RepEFP>(*eval, vLS);

		vizPert.push_back(nsModifyFuzzyRules);
		vizPert.push_back(nsVAlpha);
		vizPert.push_back(nsChangeSingleInput);
		ilsPert = new ILSLPerturbationLPlus2<RepEFP>(*eval, 50, *vizPert[0], rg);
		ilsPert->add_ns(*vizPert[1]);
		ilsPert->add_ns(*vizPert[2]);
		ils = new IteratedLocalSearchLevels<RepEFP>(*eval, *c, *vnd, *ilsPert, 100, 10);

		int mu = methodParam.getESMU();
		int lambda = methodParam.getESLambda();
		int esMaxG = methodParam.getESMaxG();
		int initialDesv = methodParam.getESInitialDesv();
		int mutationDesv = methodParam.getESMutationDesv();
		//cout<<mu<<"\t"<<lambda<<"\t"<<esMaxG<<"\t"<<initialDesv<<"\t"<<mutationDesv<<endl;
		//getchar();
		EsCOpt = new EFPESContinous(*eval, *c, vNSeq, emptyLS, mu, lambda, esMaxG, rg, initialDesv, mutationDesv);

		//olr = new OptimalLinearRegression(*eval, *p);

		vNSeq.push_back(nsChangeSingleInput);
		vNSeq.push_back(nsRemoveSingleInput);
		vNSeq.push_back(nsAddSingleInput);
//		vNSeq.push_back(nsAddMean01);
//		vNSeq.push_back(nsAddMean1);
//		vNSeq.push_back(nsAddMeanD15);
//		vNSeq.push_back(nsAddMeanD6);
//		vNSeq.push_back(nsAddMeanD2);
//		vNSeq.push_back(nsAddMean);
//		vNSeq.push_back(nsAddMeanM2);
//		vNSeq.push_back(nsAddMeanM5);
//		vNSeq.push_back(nsAddMeanBigM);
//		vNSeq.push_back(nsVAlpha);
		vNSeq.push_back(nsModifyFuzzyRules);

		//TODO check why ES goes more generations some time when we do not have improvements.

		vector<int> vNSeqMax(vNSeq.size(), 1000);
		double mutationRate = 0.1;
		int selectionType = 1;
		string outputFile = "LogPopFOPlus";
		es = new ES<RepEFP>(*eval, *c, vNSeq, vNSeqMax, emptyLS, selectionType, mutationRate, rg, mu, lambda, esMaxG, outputFile, 0);
		es->setMessageLevel(3);
	}

	virtual ~ForecastClass()
	{

		tForecastings.clear();

		delete p;
		delete eval;
		delete c;

		for (int i = 0; i < vLS.size(); i++)
			delete vLS[i];
		vLS.clear();

		for (int i = 0; i < vizPert.size(); i++)
			delete vizPert[i];
		vizPert.clear();

		delete ilsPert; //todo verify
		delete ils; //todo verify
		delete vnd;
		delete EsCOpt;

	}

	void runMultiObjSearch()
	{
		GRInitialPopulation<RepEFP> bip(*c, rg, 0.2);

		vector<Evaluator<RepEFP>*> v_e;
		v_e.push_back(new EFPEvaluator(*p, problemParam, methodParam.getEvalFOMinimizer(), MAPE_INDEX));
		v_e.push_back(new EFPEvaluator(*p, problemParam, methodParam.getEvalFOMinimizer(), RMSE_INDEX));

		MultiEvaluator<RepEFP> mev(v_e);
		int initial_population_size = 10;

		MOVNSLevels<RepEFP> multiobjectvns(v_e, bip, initial_population_size, vNSeq, rg, 10, 10);
		TwoPhaseParetoLocalSearch<RepEFP> paretoSearch(mev, bip, initial_population_size, vNSeq);

		Pareto<RepEFP>* pf;
		//pf = paretoSearch.search(120, 0);
		pf = multiobjectvns.search(20, 0);

		vector<vector<EvaluationEFP*> > vEval = pf->getParetoFront();
		vector<Solution<RepEFP>*> vSolPf = pf->getParetoSet();

		int nObtainedParetoSol = vEval.size();
		for (int i = 0; i < nObtainedParetoSol; i++)
		{
			vector<double> solEvaluations;
			double foProfit = vEval[i][0]->getObjFunction();
			double foVolatility = vEval[i][1]->getObjFunction();
			solEvaluations.push_back(foProfit);
			solEvaluations.push_back(foVolatility);

			cout << foProfit << "\t" << foVolatility << "\t";
		}
		getchar();
	}

	pair<Solution<RepEFP>&, Evaluation&>* runOLR()
	{

		pair<Solution<RepEFP>&, Evaluation&>* finalSol = NULL;
		finalSol = EsCOpt->search(1);

		//olr->exec(finalSol->first, 100, 100);

		return finalSol;
	}
	pair<Solution<RepEFP>&, Evaluation&>* runGRASP(int timeGRASP, int nSol)
	{
		BasicGRASP<RepEFP> g(*eval, *c, emptyLS, 0.1, nSol);
		g.setMessageLevel(3);
		pair<Solution<RepEFP>&, Evaluation&>* finalSol;

		finalSol = g.search(timeGRASP);
		return finalSol;
	}

	pair<Solution<RepEFP>&, Evaluation&>* run(int timeES, int timeVND, int timeILS)
	{
		if (timeES == 0)
			timeES = 1;

		pair<Solution<RepEFP>&, Evaluation&>* finalSol;

		//finalSol = EsCOpt->search(timeES);
		finalSol = es->search(timeES);

//		vnd->setMessageLevel(3);
//		if (timeVND > 0)
//			vnd->exec(finalSol->first, finalSol->second, timeVND, 0);
//
//		const Solution<RepEFP> solVND = finalSol->first;
//		const Evaluation evaluationVND = finalSol->second;
//		evaluationVND.print();
//		if (timeILS > 0)
//			finalSol = ils->search(timeILS, 0, &solVND, &evaluationVND);

		return finalSol;
	}

	pair<Solution<RepEFP>&, Evaluation&>* runDiscreteES(int timeES)
	{
		pair<Solution<RepEFP>&, Evaluation&>* finalSol;
		return finalSol;
	}

	pair<Solution<RepEFP>&, Evaluation&>* runEFP(int timeGRASP, int timeILS)
	{

		BasicGRASP<RepEFP> g(*eval, *c, emptyLS, 0.1, 100000);
		g.setMessageLevel(3);
		pair<Solution<RepEFP>&, Evaluation&>* finalSol;

		finalSol = g.search(timeGRASP);
		const Solution<RepEFP> solGRASP = finalSol->first;
		const Evaluation evaluationGrasp = finalSol->second;

		ils->setMessageLevel(3);
		if (timeILS > 0)
			finalSol = ils->search(timeILS, 0, &solGRASP, &evaluationGrasp);

		return finalSol;
	}

	vector<double> returnBlind(pair<SolutionEFP&, Evaluation&>* sol, vector<vector<double> >& validationBlindForecastings)
	{
		vector<double> blindResults;
		blindResults = eval->blindForecasting(sol->first.getR(), validationBlindForecastings);

		return blindResults;
	}

	vector<double> returnErrors(pair<SolutionEFP&, Evaluation&>* sol, vector<vector<double> > vForecastingsValidation)
	{

		//OLD CALCULUSS -- TODO CHECK IF THE NEW IS RIGHT
		//vector<double> foIndicator(NMETRICS + 1, 0); //+1 is for the PinballError TODO
//		vector<double> foIndicator(NMETRICS, 0); //+1 is for the PinballError TODO
//		foIndicator = eval->returnForecastingsFO(sol->first.getR(), vForecastingsValidation, true, false);
//		cout << "insideForecastClass" << endl;
//		cout << foIndicator << endl;
//		getchar();

		//TODO CHECK IF THE NEW IS RIGHT
		vector<double> foIndicatorNew(NMETRICS, 0);
		vector<double> estimatedValues = eval->returnTrainingSetForecasts(sol->first.getR(), vForecastingsValidation);

		int maxLag = problemParam.getMaxLag();
		vector<double> targetValues;

		for (int i = maxLag; i < vForecastingsValidation[0].size(); i++)
			targetValues.push_back(vForecastingsValidation[0][i]);

//		cout << validationSetValues << endl;
//		getchar();
		foIndicatorNew = eval->getAccuracy(targetValues, estimatedValues, true);
//		cout << "insideForecastClassNew" << endl;
//		cout << foIndicatorNew << endl;

		return foIndicatorNew;
	}

	vector<double> returnErrorsCallGetAccuracy(vector<double> targetValues, vector<double> estimatedValues)
	{
		return eval->getAccuracy(targetValues, estimatedValues, true);
	}

	vector<double> returnErrorsPersistance(vector<double> targetValues, int fH)
	{

		vector<double> estimatedValues;
		int nSamples = targetValues.size();
		for (int s = 1; s < nSamples; s += fH)
			for (int k = 0; k < fH; k++)
			{
				if (s + k < nSamples)
					estimatedValues.push_back(targetValues[s - 1]);
			}

//		cout<<targetValues<<endl;
		targetValues.erase(targetValues.begin());
//		cout << estimatedValues << endl;
//		cout << targetValues << endl;
//		getchar();

		return eval->getAccuracy(targetValues,estimatedValues, true);
	}

	vector<double> returnForecasts(pair<SolutionEFP&, Evaluation&>* sol, vector<vector<double> > vForecastingsValidation)
	{
		vector<double> estimatedValues = eval->returnTrainingSetForecasts(sol->first.getR(), vForecastingsValidation);

		return estimatedValues;
	}

//	vector<double> returnForecasts(pair<SolutionEFP&, Evaluation&>* sol, vector<vector<double> > vForecastingsValidation)
//	{
//		vector<double> forecasts = eval->returnForecastingsFO(sol->first.getR(), vForecastingsValidation, true, true);
//
//		return forecasts;
//	}

	/*
	 vector<double> getFoIndicator(vector<double> vRealValues, vector<double> vForecastings)
	 {
	 vector<double> foIndicator;
	 vector < pair<double, double> > vPinballFunctions(20, make_pair(0, 0));

	 int nSamples = vRealValues.size();
	 for (int i = 0; i < nSamples; i++) // preve 24 horas a frente
	 {
	 double forecastingTarget = vRealValues[i];
	 double estimation = vForecastings[i];

	 foIndicator[MAPE_INDEX] += (abs(estimation - forecastingTarget) / abs(forecastingTarget));

	 int vQ = 0;
	 for (double quantilError = 0; quantilError <= 1; quantilError = quantilError + 0.05)
	 {
	 double pinballFunctionQuantils = 0;
	 for (int a = 1; a < 100; a++)
	 {
	 double quantilFactor = (50 - a) / 100.0;

	 double qa = estimation - estimation * quantilError * quantilFactor;

	 if (forecastingTarget < qa)
	 pinballFunctionQuantils += (1 - (a / 100.0)) * (qa - forecastingTarget);

	 if (forecastingTarget >= qa)
	 pinballFunctionQuantils += (a / 100.0) * (forecastingTarget - qa);
	 }
	 pinballFunctionQuantils /= 99;
	 vPinballFunctions[vQ].first += pinballFunctionQuantils;
	 vPinballFunctions[vQ].second = quantilError;
	 vQ++;
	 }
	 foIndicator[MSE_INDEX] += pow((estimation - forecastingTarget), 2);
	 }

	 foIndicator[MAPE_INDEX] *= 100;
	 foIndicator[MAPE_INDEX] /= nSamples;

	 double minPinball = 1000000;
	 double pinbalError;
	 for (int pF = 0; pF < 20; pF++)
	 {
	 vPinballFunctions[pF].first /= nSamples;
	 if (vPinballFunctions[pF].first < minPinball)
	 {
	 minPinball = vPinballFunctions[pF].first;
	 pinbalError = vPinballFunctions[pF].second;
	 }
	 }

	 foIndicator[PINBALL_INDEX] = minPinball;
	 foIndicator[PINBALL_ERROR] = pinbalError;
	 //foIndicator[PINBALL_INDEX] /= (nForecastingValidations - notUsedForTest);

	 foIndicator[MSE_INDEX] /= nSamples;
	 foIndicator[RMSE_INDEX] = sqrt(foIndicator[MSE_INDEX]);

	 return foIndicator;
	 }*/

	/*
	 void changeMethod(vector<vector<double> >& _tForecastings, ProblemParameters& _problemParam)
	 {
	 tForecastings.clear();
	 delete &problemParam;
	 delete &tForecastings;

	 tForecastings = _tForecastings;

	 problemParam = _problemParam;

	 delete p;
	 delete eval;
	 delete c;
	 p = new ProblemInstance(tForecastings, problemParam);
	 eval = new EFPEvaluator(*p, problemParam);

	 if (construtiveMethod == 0)
	 c = new ConstructiveRandomNorm(*p, problemParam, rg);
	 else
	 c = new ConstructiveCalcMeans(*p, problemParam, rg);


	 for (int i = 0; i < vLS.size(); i++)
	 delete vLS[i];
	 vLS.clear();
	 FirstImprovement<RepEFP>* fi = new FirstImprovement<RepEFP>(*eval, *ns);
	 vLS.push_back(fi);

	 delete vnd;
	 vnd = new VariableNeighborhoodDescent<RepEFP>(*eval, vLS);

	 ils = new IteratedLocalSearchLevels<RepEFP>(*eval, *c, *vnd, *ilsPert, 100, 10);

	 delete EsCOpt;
	 EsCOpt = new EFPESContinous(*eval, *c, vNSeq, emptyLS, mu, lambda, gmax, rg, initialDesv, mutationDesv);
	 }*/

};

} /* namespace EFP */
#endif /* FORECASTCLASS_HPP_ */
