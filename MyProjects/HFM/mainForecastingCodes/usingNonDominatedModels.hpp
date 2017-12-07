#ifndef HFM_USINGNONDOMINATEDMODELS_HPP_
#define HFM_USINGNONDOMINATEDMODELS_HPP_

// ===================================
// Main.cpp file generated by OptFrame
// Project EFP
// ===================================

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <numeric>
#include "../../../OptFrame/RandGen.hpp"
#include "../../../OptFrame/Util/RandGenMersenneTwister.hpp"

using namespace std;
using namespace optframe;
using namespace HFM;
extern int nThreads;

int usingNonDominatedHFMModels(int argc, char **argv)
{
	nThreads = 1;
	cout << "Welcome to MO forecasting, let's generate a couple of non-dominated HFM models!" << endl;
	RandGenMersenneTwister rg;
	//long  1412730737
	long seed = time(nullptr); //CalibrationMode
	seed = 1;
	cout << "Seed = " << seed << endl;
	srand(seed);
	rg.setSeed(seed);

//	if (argc != 2)
//	{
//		cout << "Parametros incorretos!" << endl;
//		cout << "Os parametros esperados sao: stockMarketTimeSeries" << endl;
//		exit(1);
//	}

	const char* caminhoOutput = argv[1];

	string nomeOutput = caminhoOutput;
//	nomeOutput = "MyProjects/HFM/Instance/dadosBovespa/bovespa";
	nomeOutput = "MyProjects/HFM/Instance/Physionet/S001R01/Channel-30";

	//===================================
	cout << "Parametros:" << endl;
	cout << "nomeOutput=" << nomeOutput << endl;

	//Numero de passos a frente - Horizonte de previsao
	int fh = 10;
	//O valor mais antigo que pode ser utilizado como entrada do modelo de previsao [100]
	int argvMaxLagRate = 25;

	vector<string> explanatoryVariables;

	explanatoryVariables.push_back(nomeOutput);

	treatForecasts rF(explanatoryVariables);

	//Parametros do metodo
	int mu = 100;
	int lambda = mu * 6;
	int evalFOMinimizer = MAPE_INDEX;
	int contructiveNumberOfRules = 100;
	int evalAprox = 0;
	double alphaACF = -1;
	int construtive = 2;
	// ============ END FORCES ======================

	// ============= METHOD PARAMETERS=================
	HFMParams methodParam;
	//seting up ES params
	methodParam.setESMU(mu);
	methodParam.setESLambda(lambda);
	methodParam.setESMaxG(10000);

	//seting up ACF construtive params
	methodParam.setConstrutiveMethod(construtive);
	methodParam.setConstrutivePrecision(contructiveNumberOfRules);
	vector<double> vAlphaACFlimits;
	vAlphaACFlimits.push_back(alphaACF);
	methodParam.setConstrutiveLimitAlphaACF(vAlphaACFlimits);

	//seting up Eval params
	methodParam.setEvalAprox(evalAprox);
	methodParam.setEvalFOMinimizer(evalFOMinimizer);
	// ==========================================

	// ================== READ FILE ============== CONSTRUTIVE 0 AND 1
	ProblemParameters problemParam;
	//ProblemParameters problemParam(vParametersFiles[randomParametersFiles]);

	problemParam.setStepsAhead(fh);

	int nTotalForecastingsTrainningSet = rF.getForecastsSize(0) - fh;

	//========SET PROBLEM MAXIMUM LAG ===============
	cout << "argvMaxLagRate = " << argvMaxLagRate << endl;

	int iterationMaxLag = ((nTotalForecastingsTrainningSet - fh) * argvMaxLagRate) / 100.0;
	iterationMaxLag = ceil(iterationMaxLag);
	if (iterationMaxLag > (nTotalForecastingsTrainningSet - fh))
		iterationMaxLag--;
	if (iterationMaxLag <= 0)
		iterationMaxLag = 1;

	problemParam.setMaxLag(iterationMaxLag);
	int maxLag = problemParam.getMaxLag();

	//If maxUpperLag is greater than 0 model uses predicted data
	problemParam.setMaxUpperLag(0);
	//int maxUpperLag = problemParam.getMaxUpperLag();
	//=================================================

	vector<double> foIndicators;
	int beginTrainingSet = 0;
	cout << std::setprecision(9);
	cout << std::fixed;
	double NTRaprox = (nTotalForecastingsTrainningSet - maxLag) / double(fh);
	cout << "#timeSeriesSize: " << rF.getForecastsSize(0) << endl;
	cout << "#nTotalForecastingsTrainningSet: " << nTotalForecastingsTrainningSet << endl;
	cout << "BeginTrainninningSet: " << beginTrainingSet << endl;
	cout << "#~NTR: " << NTRaprox << endl;
	cout << "#maxNotUsed: " << maxLag << endl;
	cout << "#StepsAhead: " << fh << endl << endl;

	vector<vector<double> > trainningSet; // trainningSetVector
	trainningSet.push_back(rF.getPartsForecastsEndToBegin(0, fh, nTotalForecastingsTrainningSet));

//		forecastObject.runMultiObjSearch();
//		getchar();

	Pareto<RepEFP>* pf = new Pareto<RepEFP>();

	ForecastClass* forecastObject;
	int timeES = 10;
	int timeGPLS = 20;
	for (int b = 0; b < 2; b++)
	{
		if (b == 1)
			methodParam.setEvalFOMinimizer(MAPE_INV_INDEX);
		forecastObject = new ForecastClass(trainningSet, problemParam, rg, methodParam);
		pair<Solution<RepEFP>, Evaluation>* sol = forecastObject->run(timeES, 0, 0);
		forecastObject->addSolToParetoWithParetoManager(*pf, sol->first);
		Pareto<RepEFP>* pfNew = forecastObject->runMultiObjSearch(timeGPLS, pf);
		delete pf;
		pf = pfNew;
		delete &sol->first;
		delete &sol->second;
		delete sol;
		delete forecastObject;
	}
	forecastObject = new ForecastClass(trainningSet, problemParam, rg, methodParam);

	vector<MultiEvaluation*> vEvalPF = pf->getParetoFront();
	vector<Solution<RepEFP>*> vSolPF = pf->getParetoSet();
	int nObtainedParetoSol = vEvalPF.size();

	int targetFile = problemParam.getTargetFile();
	vector<vector<double> > dataForFeedingValidationTest;
	dataForFeedingValidationTest.push_back(rF.getPartsForecastsEndToBegin(0, fh, maxLag));
	vector<vector<double> > targetValidationSet;
	targetValidationSet.push_back(rF.getPartsForecastsEndToBegin(0, 0, fh));
	//Using Multi Round forescasting for obtaining validation and tests
//	vector<vector<double> > validationSet;
//	validationSet.push_back(rF.getPartsForecastsEndToBegin(0, 0, fh + maxLag));

	vector<vector<double> > ensembleBlindForecasts;
	cout << "\nPrinting obtained sets of predicted values..." << endl;
	for (int i = 0; i < nObtainedParetoSol; i++)
	{
		cout << setprecision(2);
		vector<double> blindForecasts = *forecastObject->returnBlind(vSolPF[i]->getR(), dataForFeedingValidationTest);
		for (int f = 0; f < (int) blindForecasts.size(); f++)
			cout << blindForecasts[f] << "/" << targetValidationSet[targetFile][f] << "/" << (targetValidationSet[targetFile][f] - blindForecasts[f]) << "\t";

		cout << endl;

//		cout << forecastObject->returnForecastsAndTargets(vSolPF[i]->getR(), validationSet) << endl;
//		getchar();
		ensembleBlindForecasts.push_back(blindForecasts);
//getchar();
	}

	cout << "\nPrinting pareto front forecast accuracy measures..." << endl;
	for (int i = 0; i < nObtainedParetoSol; i++)
	{
		cout << setprecision(5);
		for (int e = 0; e < (int) vEvalPF[i]->size(); e++)
			cout << vEvalPF[i]->at(e).getObjFunction() << "\t\t";
		cout << endl;
	}

	pf->exportParetoFront("../Outputs/paretoFrontGPLS.txt", "w");

	delete pf;
	delete forecastObject;

	cout << "MO Stock Market forecasting finished!" << endl;

//	//	===========================================
//	//	TIME FOR BRINCANDO COM A BOLSA DE VALORES (BBV)
//	BBVTools bbvTools;

	return 0;
}

#endif /*HFM_USINGNONDOMINATEDMODELS_HPP_*/
