/*
 * ForecastClass.hpp
 *
 *  Created on: 10/09/2014
 *      Author: vitor
 */

#ifndef FORECASTCLASS_HPP_
#define FORECASTCLASS_HPP_

#include "Evaluator.hpp"
#include "MultiEvaluatorHFM.hpp"
#include "Evaluation.h"
#include "ProblemInstance.hpp"
#include "HFMESContinous.hpp"
#include "treatForecasts.hpp"
#include "../../OptFrame/Heuristics/Empty.hpp"
#include "../../OptFrame/NSSeq.hpp"
#include "../../OptFrame/Heuristics/LocalSearches/VND.h"
#include "../../OptFrame/Heuristics/LocalSearches/VariableNeighborhoodDescent.hpp"
#include "../../OptFrame/Heuristics/LocalSearches/FirstImprovement.hpp"
#include "../../OptFrame/Heuristics/LocalSearches/RandomDescentMethod.hpp"
#include "../../OptFrame/Heuristics/ILS/IteratedLocalSearchLevels.hpp"
#include "../../OptFrame/Heuristics/ILS/ILSLPerturbation.hpp"
#include "../../OptFrame/Heuristics/GRASP/BasicGRASP.hpp"
//#include "../../OptFrame/Heuristics/VNS/MOVNSLevels.hpp"
#include "../../OptFrame/Heuristics/2PPLS.hpp"
#include "../../OptFrame/Heuristics/EvolutionaryAlgorithms/NGES.hpp"
#include "../../OptFrame/MultiEvaluator.hpp"
#include "../../OptFrame/MultiObjSearch.hpp"
#include "../../OptFrame/Util/UnionNDSets.hpp"
#include "../../OptFrame/Heuristics/MOLocalSearches/MOBestImprovement.hpp"
#include "../../OptFrame/Heuristics/MOLocalSearches/MORandomImprovement.hpp"
#include "../../OptFrame/Heuristics/MOLocalSearches/GPLS.hpp"

namespace EFP
{

class ForecastClass
{
private:
	vector<vector<double> >& tForecastings;
	ProblemParameters& problemParam;
	RandGen& rg;
	methodParameters& methodParam;

	ProblemInstance* p;
	EFPEvaluator* eval;
	Constructive<RepEFP, OPTFRAME_DEFAULT_ADS>* c;
	vector<NSSeq<RepEFP, OPTFRAME_DEFAULT_ADS>*> vNS;

	EmptyLocalSearch<RepEFP, OPTFRAME_DEFAULT_ADS> emptyLS;
	vector<NSSeq<RepEFP, OPTFRAME_DEFAULT_ADS>*> vNSeq;

//	EFPESContinous* EsCOpt;
	ES<RepEFP, OPTFRAME_DEFAULT_ADS>* es;
	NGESParams* ngesParams;

	vector<LocalSearch<RepEFP, OPTFRAME_DEFAULT_ADS>*> vLS;

	VariableNeighborhoodDescent<RepEFP, OPTFRAME_DEFAULT_ADS>* vnd;
	IteratedLocalSearchLevels<RepEFP, OPTFRAME_DEFAULT_ADS>* ils;

	vector<vector<double> > vBlindResults;
	vector<pair<Solution<RepEFP, OPTFRAME_DEFAULT_ADS>, Evaluation>*> vFinalSol;

	ILSLPerturbationLPlus2<RepEFP, OPTFRAME_DEFAULT_ADS>* ilsPert;

	//OptimalLinearRegression* olr;
//	MultiEvaluator<RepEFP, OPTFRAME_DEFAULT_ADS>* mev;
	HFMMultiEvaluator* mev;
public:

	ForecastClass(vector<vector<double> >& _tForecastings, ProblemParameters& _problemParam, RandGen& _rg, methodParameters& _methodParam) :
			tForecastings(_tForecastings), problemParam(_problemParam), rg(_rg), methodParam(_methodParam)
	{

		p = new ProblemInstance(tForecastings, problemParam);
		eval = new EFPEvaluator(*p, problemParam, methodParam.getEvalFOMinimizer(), methodParam.getEvalAprox());

		NSSeqNEIGHModifyRules* nsModifyFuzzyRules = new NSSeqNEIGHModifyRules(*p, rg);
		NSSeqNEIGHChangeSingleInput* nsChangeSingleInput = new NSSeqNEIGHChangeSingleInput(*p, rg, problemParam.getMaxLag(), problemParam.getMaxUpperLag());
		NSSeqNEIGHRemoveSingleInput* nsRemoveSingleInput = new NSSeqNEIGHRemoveSingleInput(rg);
		NSSeqNEIGHAddSingleInput* nsAddSingleInput = new NSSeqNEIGHAddSingleInput(*p, rg, problemParam.getMaxLag(), problemParam.getMaxUpperLag());
//		NSSeqNEIGHVAlpha* nsVAlpha = new NSSeqNEIGHVAlpha(*p, rg, 5);

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

		FirstImprovement<RepEFP, OPTFRAME_DEFAULT_ADS>* fiModifyFuzzyRules = new FirstImprovement<RepEFP, OPTFRAME_DEFAULT_ADS>(*eval, *nsModifyFuzzyRules);
		FirstImprovement<RepEFP, OPTFRAME_DEFAULT_ADS>* fiChangeSingleInput = new FirstImprovement<RepEFP, OPTFRAME_DEFAULT_ADS>(*eval, *nsChangeSingleInput);
//		FirstImprovement<RepEFP>* fiVAlpha = new FirstImprovement<RepEFP>(*eval, *nsVAlpha);
//		int maxRDM = 100;
//		RandomDescentMethod<RepEFP>* rdm = new RandomDescentMethod<RepEFP>(*eval, *ns, maxRDM);
//		//rdm->setMessageLevel(3);
//		vLS.push_back(fiVAlpha);
		vLS.push_back(fiModifyFuzzyRules);
		vLS.push_back(fiChangeSingleInput);
		vnd = new VariableNeighborhoodDescent<RepEFP, OPTFRAME_DEFAULT_ADS>(*eval, vLS);

//		ilsPert = new ILSLPerturbationLPlus2<RepEFP,OPTFRAME_DEFAULT_ADS>(*eval, 50, *nsModifyFuzzyRules, rg); //TODO check why 50 was removed
		ilsPert = new ILSLPerturbationLPlus2<RepEFP, OPTFRAME_DEFAULT_ADS>(*eval, *nsModifyFuzzyRules, rg);
		ilsPert->add_ns(*nsChangeSingleInput);
//		nsVAlpha
		ils = new IteratedLocalSearchLevels<RepEFP, OPTFRAME_DEFAULT_ADS>(*eval, *c, *vnd, *ilsPert, 100, 10);

		int mu = methodParam.getESMU();
		int lambda = methodParam.getESLambda();
		int esGMaxWithoutImp = methodParam.getESMaxG();
//		int initialDesv = methodParam.getESInitialDesv(); //Parameter for ESContinous - Used in Clemson's Paper
//		int mutationDesv = methodParam.getESMutationDesv(); //Parameter for ESContinous - Used in Clemson's Paper

//cout<<mu<<"\t"<<lambda<<"\t"<<esMaxG<<"\t"<<initialDesv<<"\t"<<mutationDesv<<endl;
//getchar();

//Old continous Evolution Strategy - Deprecated
//		EsCOpt = new EFPESContinous(*eval, *c, vNSeq, emptyLS, mu, lambda, esMaxG, rg, initialDesv, mutationDesv);

		//olr = new OptimalLinearRegression(*eval, *p);

		vNSeq.push_back(nsModifyFuzzyRules);
		vNSeq.push_back(nsChangeSingleInput);
		vNSeq.push_back(nsRemoveSingleInput);
		vNSeq.push_back(nsAddSingleInput);
//		vNSeq.push_back(nsVAlpha);
//		vNSeq.push_back(nsAddMean01);
//		vNSeq.push_back(nsAddMean1);
//		vNSeq.push_back(nsAddMeanD15);
//		vNSeq.push_back(nsAddMeanD6);
//		vNSeq.push_back(nsAddMeanD2);
//		vNSeq.push_back(nsAddMean);
//		vNSeq.push_back(nsAddMeanM2);
//		vNSeq.push_back(nsAddMeanM5);
//		vNSeq.push_back(nsAddMeanBigM);

		//TODO check why ES goes more generations some time when we do not have improvements.

		vector<int> vNSeqMax(vNSeq.size(), 1000);
		double mutationRate = 0.1;
		int selectionType = 1;
		string outputFile = "LogPopFOPlus";
		ngesParams = new NGESParams(vNSeqMax, selectionType, mutationRate, mu, lambda, esGMaxWithoutImp, outputFile, 0);
		es = new ES<RepEFP>(*eval, *c, vNSeq, emptyLS, rg, *ngesParams);
		es->setMessageLevel(3);

		//MO -- HFM MULTI IS ABLE TO ONLY "EVALUATE" once
//		vector<Evaluator<RepEFP>*> v_e;
//		v_e.push_back(new EFPEvaluator(*p, problemParam, MAPE_INDEX, 0));
//		v_e.push_back(new EFPEvaluator(*p, problemParam, MAPE_INV_INDEX, 0));
//		v_e.push_back(new EFPEvaluator(*p, problemParam, RMSE_INDEX, 0));
//		v_e.push_back(new EFPEvaluator(*p, problemParam, SMAPE_INDEX, 0));
////		v_e.push_back(new EFPEvaluator(*p, problemParam, WMAPE_INDEX, 0));
////		v_e.push_back(new EFPEvaluator(*p, problemParam, MMAPE_INDEX, 0));
//		mev = new MultiEvaluator<RepEFP>(v_e);
		mev = new HFMMultiEvaluator(*eval);

	}

	virtual ~ForecastClass()
	{
//		tForecastings.clear();

		delete p;
		delete eval;
		delete c;

		for (int i = 0; i < (int) vLS.size(); i++)
			delete vLS[i];
		vLS.clear();

		for (int i = 0; i < (int) vNSeq.size(); i++)
			delete vNSeq[i];
		vNSeq.clear();

		delete ilsPert; //todo verify
		delete ils; //todo verify
		delete vnd;
//		delete EsCOpt;

//		mev->clear();
		delete mev;
		delete es;
		delete ngesParams;
	}

//	//add solution to pareto front evaluating with forecasting class evaluators
//	void addSolToParetoWithFCMEV(Solution<RepEFP>& s, Pareto<RepEFP>& pf)
//	{
//		pf.push_back(s, *mev);
//	}

	//add solution to pareto front evaluating with forecasting class evaluators
	void addSolToParetoWithParetoManager(Pareto<RepEFP, OPTFRAME_DEFAULT_ADS>& pf, const Solution<RepEFP, OPTFRAME_DEFAULT_ADS>& candidateS)
	{
		paretoManager<RepEFP, OPTFRAME_DEFAULT_ADS> paretoMan(*mev);
		paretoMan.addSolution(pf, candidateS);
	}

	//add solution to pareto front evaluating with forecasting class evaluators
	void addSolWithMevToParetoWithParetoManager(Pareto<RepEFP, OPTFRAME_DEFAULT_ADS>& pf, const Solution<RepEFP, OPTFRAME_DEFAULT_ADS>& candidateS, const MultiEvaluation& candidateMev)
	{
		paretoManager<RepEFP> paretoMan(*mev);
		paretoMan.addSolutionWithMEV(pf, candidateS, candidateMev);
	}

	Pareto<RepEFP>* runMultiObjSearch(double timeGPLS, Pareto<RepEFP, OPTFRAME_DEFAULT_ADS>* _pf = NULL)
	{
		Pareto<RepEFP, OPTFRAME_DEFAULT_ADS>* pf = new Pareto<RepEFP, OPTFRAME_DEFAULT_ADS>();


//		if (vS != NULL)
//		{
//			if (pf == NULL)
//			{
//				pf = new Pareto<RepEFP>();
//
//				for (int ns = 0; ns < vS.size(); ns++)
//					pf->push_back(*vS[ns], mev);
//			}
//			else
//			{
//				for (int ns = 0; ns < vS.size(); ns++)
//					pf->push_back(*vS[ns], mev);
//			}
//		}

		int initial_population_size = 30;
//		GRInitialPopulation<RepEFP,OPTFRAME_DEFAULT_ADS> bip(*c, rg, 1);
		BasicInitialPopulation<RepEFP, OPTFRAME_DEFAULT_ADS> bip(*c);
//		MOVNSLevels<RepEFP> multiobjectvns(v_e, bip, initial_population_size, vNSeq, rg, 10, 10);
//		GRInitialPareto<RepEFP,OPTFRAME_DEFAULT_ADS> grIP(*c, rg, 1, *mev);
		BasicInitialPareto<RepEFP, OPTFRAME_DEFAULT_ADS> grIP(*c, *mev);
		int maxTriesRI = 100;
		MORandomImprovement<RepEFP, OPTFRAME_DEFAULT_ADS> moriMFR(*mev, *vNSeq[0], maxTriesRI);
		MORandomImprovement<RepEFP, OPTFRAME_DEFAULT_ADS> moriCSI(*mev, *vNSeq[1], maxTriesRI);
		MORandomImprovement<RepEFP, OPTFRAME_DEFAULT_ADS> moriRSI(*mev, *vNSeq[2], maxTriesRI);
		MORandomImprovement<RepEFP, OPTFRAME_DEFAULT_ADS> moriASI(*mev, *vNSeq[3], maxTriesRI);

		vector<MOLocalSearch<RepEFP, OPTFRAME_DEFAULT_ADS>*> vMOLS;
		vMOLS.push_back(&moriASI);
		vMOLS.push_back(&moriRSI);
		vMOLS.push_back(&moriMFR);
		vMOLS.push_back(&moriCSI);

		GeneralParetoLocalSearch<RepEFP, OPTFRAME_DEFAULT_ADS> generalPLS(*mev, grIP, initial_population_size, vMOLS);
		MOSC moStopCriteriaGPLS;
		moStopCriteriaGPLS.timelimit = timeGPLS;
		if (_pf == NULL)
		{
			delete pf;
			pf = generalPLS.search(moStopCriteriaGPLS);
		}
		else
		{
			delete pf;
			pf = generalPLS.search(moStopCriteriaGPLS, _pf);
		}

//		vector<MultiEvaluation*> vEval = pf->getParetoFront();
//		vector<Solution<RepEFP>*> vSolPf = pf->getParetoSet();
//
//		int nObtainedParetoSol = vEval.size();
//		for (int i = 0; i < nObtainedParetoSol; i++)
//		{
//			//vector<double> solEvaluations;
//			for (int e = 0; e < vEval[i]->size(); e++)
//				cout << vEval[i]->at(e).getObjFunction() << "\t";
//			cout << endl;
//
//		}

//		mev->clear();
		return pf;
	}

	//Mathematical model for finding optimal weights between models, ensemble forecasting TODO
//	pair<Solution<RepEFP>&, Evaluation&>* runOLR()
//	{
//
//		pair<Solution<RepEFP>&, Evaluation&>* finalSol = NULL;
//		//olr->exec(finalSol->first, 100, 100);
//		return finalSol;
//	}

	pair<Solution<RepEFP>, Evaluation>* runGRASP(int timeGRASP, int nSol)
	{
		SOSC* stopCriteria = new SOSC(timeGRASP);
		pair<Solution<RepEFP>, Evaluation>* finalSol;
//		BasicGRASP<RepEFP> g(*eval, *c, emptyLS, 0.1, nSol);
//		g.setMessageLevel(3);

//		finalSol = g.search(stopCriteria);
		return finalSol;
	}

	pair<Solution<RepEFP>, Evaluation>* run(int timeES, int timeVND, int timeILS)
	{
		if (timeES == 0)
			timeES = 1;

		pair<Solution<RepEFP>, Evaluation>* finalSol;

		double targetValue = 3.879748973;
		targetValue = 0.123;

		SOSC* stopCriteria = new SOSC(timeES, targetValue);
		finalSol = es->search(*stopCriteria);
		delete stopCriteria;
		//finalSol = EsCOpt->search(timeES); //Continous ES -- Deprecated

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

	pair<Solution<RepEFP>, Evaluation>* runGILS(int timeGRASP, int timeILS)
	{
		SOSC stopCriteria;
//		BasicGRASP<RepEFP> g(*eval, *c, emptyLS, 0.1, 100000);
//		g.setMessageLevel(3);
		pair<Solution<RepEFP>, Evaluation>* finalSol;

//		stopCriteria.timelimit=timeGRASP;
//		finalSol = g.search(stopCriteria);
//		const Solution<RepEFP> solGRASP = finalSol.first;
//		const Evaluation evaluationGrasp = finalSol.second;

		stopCriteria.timelimit = timeILS;
		stopCriteria.target_f = 0;
		ils->setMessageLevel(3);
		if (timeILS > 0)
			finalSol = ils->search(stopCriteria);
//		finalSol = ils->search(stopCriteria, &solGRASP, &evaluationGrasp);

		return finalSol;
	}

	//return blind forecasts for the required steps ahead of problemParam class
	//this function feed a trainedModel with the last samples from the vector of TimeSeries
	vector<double>* returnBlind(const RepEFP& trainedModel, vector<vector<double> >& vTimeSeries)
	{
		return eval->returnForecasts(trainedModel, vTimeSeries, vTimeSeries[problemParam.getTargetFile()].size(), problemParam.getStepsAhead());
	}

	vector<double>* returnErrors(const RepEFP& rep, vector<vector<double> >& vForecastingsValidation)
	{
		return eval->evaluateAll(rep, ALL_EVALUATIONS, &vForecastingsValidation);

//		(NMETRICS, 0);
//		vector<double> estimatedValues = returnForecasts(sol, vForecastingsValidation);
//
//		int maxLag = problemParam.getMaxLag();
//		vector<double> targetValues;
//
//		for (int i = maxLag; i < vForecastingsValidation[0].size(); i++)
//			targetValues.push_back(vForecastingsValidation[0][i]);
//
////		cout << validationSetValues << endl;
////		getchar();
//		foIndicatorNew = returnErrorsCallGetAccuracy(targetValues, estimatedValues);
////		cout << "insideForecastClassNew" << endl;
////		cout << foIndicatorNew << endl;

//		return foIndicatorNew;
	}

	//return all possible forecasting measures
	vector<double>* callEvalGetAccuracy(vector<double> targetValues, vector<double> estimatedValues)
	{
		return eval->getAccuracy(targetValues, estimatedValues, ALL_EVALUATIONS);
	}
//

	//Return forecasts with pre-defined sliding window strategy with FH
	vector<double>* returnForecasts(pair<SolutionEFP, Evaluation>* sol, vector<vector<double> > vForecastingsValidation)
	{
		pair<vector<double>*, vector<double>* >* targetAndForecasts  = eval->generateSWMultiRoundForecasts(sol->first.getR(), vForecastingsValidation, problemParam.getStepsAhead());
		return targetAndForecasts->second;
	}

	/*Target and forecasts
	 Return forecasts with pre-defined sliding window strategy with FH*/
	pair<vector<double>*, vector<double>* >* returnForecastsAndTargets(const RepEFP& rep, const vector<vector<double> > vForecastingsValidation)
	{
		return eval->generateSWMultiRoundForecasts(rep, vForecastingsValidation, problemParam.getStepsAhead());
	}

	vector<double>* returnErrorsPersistance(vector<double> targetValues, int fH)
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

		return eval->getAccuracy(targetValues, estimatedValues, -1);
	}

	void exportForecasts(const vector<double> forecasts, string output)
	{
		FILE* fResults = fopen(output.c_str(), "w");
		for (int n = 0; n < (int) forecasts.size(); n++)
		{
//			cout<< forecasts[n]<<endl;
//			getchar();
			fprintf(fResults, "%.10f\n", forecasts[n]);
		}
		fclose(fResults);
	}

};

} /* namespace EFP */
#endif /* FORECASTCLASS_HPP_ */
