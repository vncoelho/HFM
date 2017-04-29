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
using namespace EFP;
extern int nThreads;

int musicGen(int argc, char **argv)
{
	nThreads = 2;
	cout << "Welcome to automatic music learning and forecasting and generation" << endl;
	RandGenMersenneTwister rg;
	//long  1412730737
	long seed = time(NULL); //CalibrationMode
	seed = 1;
	cout << "Seed = " << seed << endl;
	srand(seed);
	rg.setSeed(seed);
	cout << "nThreads = " << nThreads << endl;

//	if (argc != 2)
//	{
//		cout << "Parametros incorretos!" << endl;
//		cout << "Os parametros esperados sao: stockMarketTimeSeries" << endl;
//		exit(1);
//	}

	const char* caminhoOutput = argv[1];

//	string inputTimeSeries = caminhoOutput;
//	inputTimeSeries = "MyProjects/HFM/Instance/mp3/bluesMonoChannel_10_20";
//	inputTimeSeries = "MyProjects/HFM/Instance/mp3/jazzMonoChannel_10_20";

	//===================================
	cout << "Parametros:" << endl;
//	cout << "nomeOutput=" << inputTimeSeries << endl;

	vector<string> explanatoryVariables;
	explanatoryVariables.push_back("MyProjects/HFM/Instance/mp3/bluesMonoChannel_10_20");
	explanatoryVariables.push_back("MyProjects/HFM/Instance/mp3/jazzMonoChannel_10_20");

	treatForecasts rF(explanatoryVariables);

	vector<string> timeSeriesToBeForecasted;
	timeSeriesToBeForecasted.push_back("MyProjects/HFM/Instance/mp3/bluesMonoChannel_10_20");
	timeSeriesToBeForecasted.push_back("MyProjects/HFM/Instance/mp3/jazzMonoChannel_10_20");
	treatForecasts tFToBeForecasted(timeSeriesToBeForecasted);

	//Parametros do metodo
	int mu = 10;
	int lambda = mu * 6;
	int evalFOMinimizer = MAPE_INDEX;
	int contructiveNumberOfRules = 100;
	int evalAprox = 0;
	double alphaACF = -1;
	int construtive = 2;
	// ============ END FORCES ======================

	// ============= METHOD PARAMETERS=================
	methodParameters methodParam;
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
	int timeES = 21 * 60;
	double fs = 44100;
	double nSecondsOfTraining = 0.1;
	double nSecondsForFeeding = 0.2;
	int maxLag = nSecondsForFeeding * fs;
	double nSecondsOfForecasts = 0.1;
	int fh = nSecondsOfForecasts * fs;

	//forcing values
	fh = 1;
	maxLag = 150;

	problemParam.setStepsAhead(fh);
	problemParam.setMaxLag(maxLag);

	//If maxUpperLag is greater than 0 model uses predicted data
	problemParam.setMaxUpperLag(0);
	//=================================================

	cout << std::setprecision(9);
	cout << std::fixed;
	int nTotalForecastingsTrainningSet = nSecondsOfTraining * fs + maxLag;
	int beginTrainingSet = fs;
	double NTRaprox = (nTotalForecastingsTrainningSet - maxLag) / double(fh);
	cout << "#timeSeriesSize: " << rF.getForecastsSize(0) << endl;
	cout << "#nTotalForecastingsTrainningSet: " << nTotalForecastingsTrainningSet << endl;
	cout << "#~NTR: " << NTRaprox << endl;
	cout << "#maxNotUsed: " << maxLag << endl;
	cout << "#StepsAhead: " << fh << endl << endl;
	cout << "#trainingTimeWithES: " << timeES << endl << endl;

	vector<vector<double> > trainningSet; // trainningSetVector
	trainningSet.push_back(rF.getPartsForecastsBeginToEnd(0, beginTrainingSet, nTotalForecastingsTrainningSet));

	int multiplier = 10000;

	for (int n = 0; n < trainningSet[0].size(); n++)
		trainningSet[0][n] *= multiplier;

	ForecastClass* forecastObject;

	forecastObject = new ForecastClass(trainningSet, problemParam, rg, methodParam);
	pair<Solution<RepEFP>&, Evaluation&>* sol = forecastObject->run(timeES, 0, 0);

	for (int m = 0; m < timeSeriesToBeForecasted.size(); m++)
	{
		vector<vector<double> > validationSet; // validationSetVector
		validationSet.push_back(tFToBeForecasted.getPartsForecastsBeginToEnd(m, 0, tFToBeForecasted.getForecastsSize(m)));

		for (int n = 0; n < validationSet[0].size(); n++)
			validationSet[0][n] *= multiplier;

		pair<vector<double>, vector<double> > forecastsAndTargets = forecastObject->returnForecastsAndTargets(sol->first.getR(), validationSet);

		for (int n = 0; n < forecastsAndTargets.first.size(); n++)
		{
			forecastsAndTargets.first[n] /= multiplier;
			forecastsAndTargets.second[n] /= multiplier;
		}

		stringstream ss;
		ss << "forecast_Params_FH" << fh << "_max" << maxLag << "_Tr" << timeES << "_Times" << nSecondsOfTraining << "_" << nSecondsForFeeding << "_" << nSecondsOfForecasts << "_TS" << "_TS" << m << ".txt";
		forecastObject->exportForecasts(forecastsAndTargets.second, ss.str());
	}

//	forecastObject->exportForecasts(forecastsAndTargets.first, "./targetSmall.txt");

	cout << "Music has been created." << endl;

	return 0;
}

