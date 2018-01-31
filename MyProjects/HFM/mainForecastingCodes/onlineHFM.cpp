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
//#include "../../BBV/BBVTolls.hpp"

using namespace std;
using namespace optframe;
using namespace HFM;
extern int nThreads;

string onlineHFM(int argc, char **argv, vector<vector<double>>* forecastings = nullptr)
{
	nThreads = 1;
	cout << "Welcome to our online HFM!" << endl;
	cout << "Let's optimize with " << nThreads << " nThreads\n" << endl;

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

//	const char* caminhoOutput = argv[1];
//	string nomeOutput = caminhoOutput;

	//===================================
	cout << "Parametros:" << endl;

	//Numero de passos a frente - Horizonte de previsao
	int fh = 5;
	//O valor mais antigo que pode ser utilizado como entrada do modelo de previsao [100]
	double argvMaxLagRate = 10;

	vector<string> explanatoryVariables;

//	explanatoryVariables.push_back("./HFM/Instance/dadosBovespa/bovespa");
//	explanatoryVariables.push_back("./HFM/Instance/dadosBovespa/test.txt");
	explanatoryVariables.push_back("./HFM/Instance/dadosBovespa/loanFunctionTarget");
	explanatoryVariables.push_back("./HFM/Instance/dadosBovespa/loanFunctiontExp1");
	explanatoryVariables.push_back("./HFM/Instance/dadosBovespa/loanFunctiontExp3");
	explanatoryVariables.push_back("./HFM/Instance/dadosBovespa/loanFunctiontExp4");
//	explanatoryVariables.push_back("./HFM/Instance/dadosBovespa/loanFunctiontExp2"); //Amort should not be considered

	cout << "Variables and explanation:" << explanatoryVariables << endl;

	treatForecasts rF;
	if (forecastings)
	{
		//Forcing TS for .js
//	vector<double>  forcingVector;
//	for (int i=1;i<1000;i++)
//		forcingVector.push_back(i);
//	vector<vector<double>> forcedTS;
//	forcedTS.push_back(forcingVector);
		rF = treatForecasts(*forecastings);
		cout << "loaded forecasts:" << *forecastings << endl;
		explanatoryVariables.resize(forecastings->size());
	}
	else
	{
		rF = treatForecasts(explanatoryVariables);
	}

	//Parametros do metodo
	int mu = 100;
	int lambda = mu * 6;
	int evalFOMinimizer = MAPE_INDEX;
	int contructiveNumberOfColumns = 10;
	cout << "contructiveNumberOfColumns:" << contructiveNumberOfColumns << endl;
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
	methodParam.setConstrutivePrecision(contructiveNumberOfColumns);
	vector<double> vAlphaACFlimits;
	for (int expVar = 0; expVar < (int) explanatoryVariables.size(); expVar++)
		vAlphaACFlimits.push_back(alphaACF);
	methodParam.setConstrutiveLimitAlphaACF(vAlphaACFlimits);

	//seting up Eval params
	methodParam.setEvalAprox(evalAprox);
	methodParam.setEvalFOMinimizer(evalFOMinimizer);
	// ==========================================

	// ================== READ FILE ============== CONSTRUTIVE 0 AND 1
	ProblemParameters problemParam(explanatoryVariables.size());
	//ProblemParameters problemParam(vParametersFiles[randomParametersFiles]);

	problemParam.setStepsAhead(fh);

	int nTotalForecastingsTrainningSet = rF.getForecastsSize(0) - fh;

	//========SET PROBLEM MAXIMUM LAG ===============
	cout << "argvMaxLagRate = " << argvMaxLagRate << endl;

	int maxLargAccordingToRate = ((nTotalForecastingsTrainningSet - fh) * argvMaxLagRate) / 100.0;
	maxLargAccordingToRate = ceil(maxLargAccordingToRate);
	if (maxLargAccordingToRate > (nTotalForecastingsTrainningSet - fh))
		maxLargAccordingToRate--;
	if (maxLargAccordingToRate <= 0)
		maxLargAccordingToRate = 1;

	cout << "maxLargAccordingToRate:" << maxLargAccordingToRate << endl;
	for (int expVar = 0; expVar < (int) explanatoryVariables.size(); expVar++)
		problemParam.setMaxLag(maxLargAccordingToRate, expVar);

	//If maxUpperLag is greater than 0 model uses predicted data: ( t - (-K) ) => (t + K) | K !=0
	for (int expVar = 0; expVar < (int) explanatoryVariables.size(); expVar++)
		problemParam.setMaxUpperLag(expVar, expVar);

//	problemParam.setForceSampleLearningWithEndogenous(true, 1);
//	problemParam.setForceSampleLearningWithEndogenous(true, 2);
//	problemParam.setForceSampleLearningWithEndogenous(true, 3);
	problemParam.setForceSampleLearningWithEndogenous(true);

//	problemParam.setForceSampleLearningWithEndogenous(true, 4);

	int maxLag = problemParam.getMaxLag(0);

	//int maxUpperLag = problemParam.getMaxUpperLag();
	//=================================================

//	problemParam.setToRoundedForecasts(true);
//	problemParam.setToNonNegativeForecasts(true);
//	problemParam.setToBinaryForecasts(true);

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
	for (int expVar = 0; expVar < (int) explanatoryVariables.size(); expVar++)
		trainningSet.push_back(rF.getPartsForecastsEndToBegin(expVar, fh, nTotalForecastingsTrainningSet));

	Pareto < RepHFM > *pf = new Pareto<RepHFM>();

	ForecastClass* forecastObject;
	int timeES = 10;
	int timeGPLS = 10;
	int nBatches = 1;
	for (int b = 0; b < nBatches; b++)
	{
		if (b == 1)
			methodParam.setEvalFOMinimizer(MAPE_INV_INDEX);
		forecastObject = new ForecastClass(trainningSet, problemParam, rg, methodParam);
		pair<Solution<RepHFM, OPTFRAME_DEFAULT_ADS>, Evaluation>* sol = forecastObject->run(timeES, 0, 0);
//		pair<Solution<RepEFP, OPTFRAME_DEFAULT_ADS>, Evaluation>* sol = forecastObject->runGILS(0, timeES);
//		cout << "Bye bye..see u soon." << endl;
//		exit(1);

		forecastObject->addSolToParetoWithParetoManager(*pf, sol->first);
		Pareto<RepHFM, OPTFRAME_DEFAULT_ADS>* pfNew = forecastObject->runMultiObjSearch(timeGPLS, pf);
		delete pf;
		pf = pfNew;
		delete forecastObject;
		delete sol;

//		getchar();
	}
	forecastObject = new ForecastClass(trainningSet, problemParam, rg, methodParam);

	vector<MultiEvaluation*> vEvalPF = pf->getParetoFront();
	vector<Solution<RepHFM>*> vSolPF = pf->getParetoSet();
	int nObtainedParetoSol = vEvalPF.size();

	vector<vector<double> > dataForFeedingValidationTest;
	for (int expVar = 0; expVar < (int) explanatoryVariables.size(); expVar++)
		dataForFeedingValidationTest.push_back(rF.getPartsForecastsEndToBegin(expVar, fh, maxLag));

	int targetFile = problemParam.getTargetFile();
	vector<double> targetValidationSet;
	targetValidationSet = rF.getPartsForecastsEndToBegin(targetFile, 0, fh);

	//Using Multi Round forecasting for obtaining validation and tests
//	vector<vector<double> > validationSet;
//	for (int expVar = 0; expVar < (int) explanatoryVariables.size(); expVar++)
//		validationSet.push_back(rF.getPartsForecastsEndToBegin(expVar, 0, fh + maxLag));

	string obtainedForecastsString;
	vector<vector<double>*> ensembleBlindForecasts;
	cout << "\nPrinting obtained sets of predicted values..." << endl;
	stringstream ss;

	for (int i = 0; i < nObtainedParetoSol; i++)
	{
		vector<double>* blindForecasts = forecastObject->returnBlind(vSolPF[i]->getR(), dataForFeedingValidationTest);
		cout << setprecision(2);
		//Print CSV labels in the first iter
		if (i == 0)
		{
			ss << "Non-dominated HFM,";
			for (int f = 0; f < (int) blindForecasts->size(); f++)
			{
				ss << "K(" << f << ")";
//				if (f < ((int) blindForecasts->size() - 1))
				ss << ",";
			}

			for (int e = 0; e < (int) vEvalPF[i]->size(); e++)
				ss << "Eval(" << e << "),";

			ss << "\n";
		}
		ss << i << ",";
		for (int f = 0; f < (int) blindForecasts->size(); f++)
		{
			cout << blindForecasts->at(f) << "/" << targetValidationSet.at(f) << "/" << (targetValidationSet.at(f) - blindForecasts->at(f)) << "\t";
			ss << "[" << blindForecasts->at(f) << "/" << targetValidationSet.at(f) << "/" << (targetValidationSet.at(f) - blindForecasts->at(f)) << "]";
//			if (f < ((int) blindForecasts->size() - 1))
			ss << ",";
		}

		for (int e = 0; e < (int) vEvalPF[i]->size(); e++)
			ss << vEvalPF[i]->at(e).getObjFunction() << ",";
		ss << "\n";
//		cout << forecastObject->returnForecastsAndTargets(vSolPF[i]->getR(), validationSet) << endl;
//		getchar();
		ensembleBlindForecasts.push_back(blindForecasts);
//getchar();
	}
	obtainedForecastsString = ss.str();

	cout << "\nPrinting pareto front forecast accuracy measures..." << endl;
	for (int i = 0; i < nObtainedParetoSol; i++)
	{
		cout << setprecision(5);
		for (int e = 0; e < (int) vEvalPF[i]->size(); e++)
			cout << vEvalPF[i]->at(e).getObjFunction() << "\t\t";
		cout << endl;
	}

//	pf->exportParetoFront("./Outputs/paretoFrontGPLS.txt", "w");

	delete pf;
	delete forecastObject;

	for (int i = 0; i < nObtainedParetoSol; i++)
		delete ensembleBlindForecasts[i];

	cout << "ONLINE forecasting finished!" << endl;

	return obtainedForecastsString;
}