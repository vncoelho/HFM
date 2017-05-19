// ===================================
// Main.cpp file generated by OptFrame
// Project EFP
// ===================================

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <numeric>
#include "../../../OptFrame/RandGen.hpp"
#include "../../../OptFrame/Util/RandGenMersenneTwister.hpp"

using namespace std;
using namespace optframe;
using namespace EFP;

int microGridLiuAppliedEnergy(int argc, char **argv)
{
	cout << "Welcome to Micro Grid Liu Applied Energy Dataset Calibration Mode..." << endl;
	RandGenMersenneTwister rg;
	//long  1412730737
	long seed = time(NULL); //CalibrationMode
	seed = 9;
	cout << "Seed = " << seed << endl;
	srand(seed);
	rg.setSeed(seed);

	if (argc != 5)
	{
		cout << "Parametros incorretos!" << endl;
		cout << "Os parametros esperados sao: nomeOutput targetTS construtiveNRulesACF timeES" << endl;
		exit(1);
	}

	const char* caminhoOutput = argv[1];
	int argvTargetTimeSeries = atoi(argv[2]);
	int argvNumberOfRules = atoi(argv[3]);
	int argvTimeES = atoi(argv[4]);
	argvTimeES = 100;

	//double argvAlphaACF = atof(argv[4]);

//
//	const char* caminho = argv[1];
//	const char* caminhoValidation = argv[2];
//	const char* caminhoOutput = argv[3];
//	const char* caminhoParameters = argv[4];
//	int instN = atoi(argv[5]);
//	int stepsAhead = atoi(argv[6]);
//	int mu = atoi(argv[7]);

	string nomeOutput = caminhoOutput;

	//===================================
	cout << "Parametros:" << endl;
	cout << "nomeOutput=" << nomeOutput << endl;
	cout << "argvTargetTimeSeries=" << argvTargetTimeSeries << endl;
	cout << "argvNumberOfRules=" << argvNumberOfRules << endl;
	//cout << "argvAlphaACF=" << argvAlphaACF << endl;
//	getchar();
//	cout << "instN=" << instN << endl;
//	cout << "stepsAhead=" << stepsAhead << endl;
//	cout << "mu=" << mu << endl;
	//===================================

	//CONFIG FILES FOR CONSTRUTIVE 0 AND 1

	vector<string> explanatoryVariables;

	//DATA FROM LIU
	string fileMicroGridA = "./MyProjects/HFM/Instance/microGridLiuAE/A";
	string fileMicroGridB = "./MyProjects/HFM/Instance/microGridLiuAE/B";
	string fileMicroGridC = "./MyProjects/HFM/Instance/microGridLiuAE/C";
	string fileMicroGridD = "./MyProjects/HFM/Instance/microGridLiuAE/D";

	//TREATED DATA FROM LIU
	string fileMicroGridAT = "./MyProjects/HFM/Instance/microGridLiuAETreated/A";
	string fileMicroGridBT = "./MyProjects/HFM/Instance/microGridLiuAETreated/B";
	string fileMicroGridCT = "./MyProjects/HFM/Instance/microGridLiuAETreated/C";
	string fileMicroGridDT = "./MyProjects/HFM/Instance/microGridLiuAETreated/D";
	vector<string> vInstances;
	vInstances.push_back(fileMicroGridAT);
	vInstances.push_back(fileMicroGridBT);
	vInstances.push_back(fileMicroGridCT);
	vInstances.push_back(fileMicroGridDT);

	explanatoryVariables.push_back(vInstances[argvTargetTimeSeries]);

	treatForecasts rF(explanatoryVariables);

	//vector<vector<double> > batchOfBlindResults; //vector with the blind results of each batch

	/*int beginValidationSet = 0;
	 int nTrainningRoundsValidation = 50;
	 int nValidationSamples = problemParam.getNotUsedForTest() + nTrainningRoundsValidation * stepsAhead;
	 cout << "nValidationSamples = " << nValidationSamples << endl;
	 int nTotalForecastingsValidationSet = nValidationSamples;

	 vector<vector<double> > validationSet; //validation set for calibration
	 validationSet.push_back(rF.getPartsForecastsEndToBegin(0, beginValidationSet, nTotalForecastingsValidationSet));
	 validationSet.push_back(rF.getPartsForecastsEndToBegin(1, beginValidationSet, nTotalForecastingsValidationSet + stepsAhead));
	 validationSet.push_back(rF.getPartsForecastsEndToBegin(2, beginValidationSet, nTotalForecastingsValidationSet + stepsAhead));
	 */

//	int maxMu = 100;
//	int maxInitialDesv = 10;
//	int maxMutationDesv = 30;
//	int maxPrecision = 300;
	int nBatches = 1;

	vector<vector<double> > vfoIndicatorCalibration; //vector with the FO of each batch

	vector<SolutionEFP> vSolutionsBatches; //vector with the solution of each batch

	for (int n = 0; n < nBatches; n++)
	{
//		int contructiveNumberOfRules = rg.rand(maxPrecision) + 10;
//		int evalFOMinimizer = rg.rand(NMETRICS); //tree is the number of possible objetive function index minimizers
//		int evalAprox = rg.rand(2); //Enayatifar aproximation using previous values
//		int construtive = rg.rand(3);
//		double initialDesv = rg.rand(maxInitialDesv) + 1;
//		double mutationDesv = rg.rand(maxMutationDesv) + 1;
//		int mu = rg.rand(maxMu) + 1;
//		int lambda = mu * 6;

		//limit ACF for construtive ACF
//		double alphaACF = rg.rand01();
//		int alphaSign = rg.rand(2);
//		if (alphaSign == 0)
//			alphaACF = alphaACF * -1;

		// ============ FORCES ======================
//		initialDesv = 10;
//		mutationDesv = 20;
		int mu = 100;
		int lambda = mu * 6;
		int evalFOMinimizer = MAPE_INDEX;
		int contructiveNumberOfRules = 100;
		int evalAprox = 2;
		double alphaACF = 0.5;
		int construtive = 2;
		// ============ END FORCES ======================

		// ============= METHOD PARAMETERS=================
		methodParameters methodParam;
		//seting up Continous ES params
		methodParam.setESInitialDesv(10);
		methodParam.setESMutationDesv(20);
		methodParam.setESMaxG(100000);

		//seting up ES params
		methodParam.setESMU(mu);
		methodParam.setESLambda(lambda);

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
		int nVR = 168;
		int nSA = 24;
		problemParam.setStepsAhead(nSA);
		int stepsAhead = problemParam.getStepsAhead();
		//========SET PROBLEM MAXIMUM LAG ===============
		problemParam.setMaxLag(672);
		int maxLag = problemParam.getMaxLag();

		//If maxUpperLag is greater than 0 model uses predicted data
		problemParam.setMaxUpperLag(0);
		int maxUpperLag = problemParam.getMaxUpperLag();
		//=================================================

		int maxTrainningRounds = 20;
		int nTrainningRounds = rg.rand(maxTrainningRounds) + 1;
		nTrainningRounds = 20;
		int nTotalForecastingsTrainningSet = maxLag + nTrainningRounds * stepsAhead;

		int beginTrainingSet = 672;

		cout << "BeginTrainninningSet: " << beginTrainingSet;
		cout << "\t #SamplesTrainningSet: " << nTotalForecastingsTrainningSet << endl;
		cout << "#sizeTrainingSet: " << rF.getForecastsSize(0) << endl;
		cout << "maxNotUsed: " << problemParam.getMaxLag() << endl;
		cout << "#StepsAhead: " << stepsAhead << endl << endl;

		//else
		//beginTrainingSet = nTotalForecastingsValidationSet + n * (nTotalForecastingsTrainningSet / 10);

		/*
		 while (beginTrainingSet + nTotalForecastingsTrainningSet > rF.getForecastsDataSize())
		 {
		 beginTrainingSet = 744;
		 nTrainningRounds = rg.rand(maxTrainningRounds) + 1;
		 nTotalForecastingsTrainningSet = maxNotUsedForTest + nTrainningRounds * stepsAhead;
		 }*/

		vector<vector<double> > trainningSet; // trainningSetVector
		trainningSet.push_back(rF.getPartsForecastsEndToBegin(0, beginTrainingSet, nTotalForecastingsTrainningSet));

		ForecastClass forecastObject(trainningSet, problemParam, rg, methodParam);

		pair<Solution<RepEFP>&, Evaluation&>* sol;

		int timeES = argvTimeES;
		int timeVND = 0;
		int timeILS = 0;
		int timeGRASP = 0;
		int nIterGRASP = 1000;
		int optMethod = 0;
		if (optMethod == 0)
			sol = forecastObject.run(timeES, timeVND, timeILS);
		if (optMethod == 1)
			sol = forecastObject.runGILS(timeGRASP, timeILS); // GRASP + ILS
		if (optMethod == 2)
			sol = forecastObject.runGRASP(timeGRASP, nIterGRASP); // GRASP

		vector<double> foIndicatorCalibration;
		int nValidationSamples = maxLag + nVR * stepsAhead;
//		vector<vector<double> > validationSet; //validation set for calibration
//		validationSet.push_back(rF.getPartsForecastsEndToBegin(0, beginTrainingSet + stepsAhead, nValidationSamples));
//		vector<double> foIndicatorsWeeks;
//		foIndicatorsWeeks = forecastObject.returnErrors(sol, validationSet);
//		foIndicatorCalibration.push_back(foIndicatorsWeeks[MAPE_INDEX]);
//		foIndicatorCalibration.push_back(foIndicatorsWeeks[RMSE_INDEX]);

		cout << "maxLag:" << maxLag << endl;
		cout << "nValidationSamples:" << nValidationSamples << endl;
		//int beginValidationSet = 0;
		for (int w = 3; w >= 0; w--)
		{
			vector<vector<double> > validationSet; //validation set for calibration
			validationSet.push_back(rF.getPartsForecastsEndToBegin(0, w * 168, nValidationSamples));
			vector<double> foIndicatorsWeeks;
			foIndicatorsWeeks = forecastObject.returnErrors(sol->first.getR(), validationSet);
			foIndicatorCalibration.push_back(foIndicatorsWeeks[MAPE_INDEX]);
			//foIndicatorCalibration.push_back(foIndicatorsWeeks[RMSE_INDEX]);
		}
		int finalNRules = sol->first.getR().singleIndex.size();
//		cout<<"nRules:"<<finalNRules<<"/"<<contructiveNumberOfRules<<endl;
//		getchar();
		double intervalOfBeginTrainningSet = double(beginTrainingSet) / double(rF.getForecastsDataSize());
		foIndicatorCalibration.push_back(sol->second.evaluation());
		foIndicatorCalibration.push_back(construtive);
		foIndicatorCalibration.push_back(alphaACF);
		foIndicatorCalibration.push_back(contructiveNumberOfRules);
		foIndicatorCalibration.push_back(finalNRules);
		foIndicatorCalibration.push_back(mu);
		foIndicatorCalibration.push_back(lambda);
		foIndicatorCalibration.push_back(evalAprox);
		foIndicatorCalibration.push_back(timeES);
		foIndicatorCalibration.push_back(evalFOMinimizer);
		foIndicatorCalibration.push_back(nTrainningRounds);
		foIndicatorCalibration.push_back(beginTrainingSet);
		foIndicatorCalibration.push_back(intervalOfBeginTrainningSet);
		foIndicatorCalibration.push_back(nTotalForecastingsTrainningSet);
//		foIndicatorCalibration.push_back(initialDesv);
//		foIndicatorCalibration.push_back(mutationDesv);
		foIndicatorCalibration.push_back(timeVND);
		foIndicatorCalibration.push_back(timeILS);
		foIndicatorCalibration.push_back(timeGRASP);
		foIndicatorCalibration.push_back(optMethod);
		foIndicatorCalibration.push_back(argvTargetTimeSeries);
		foIndicatorCalibration.push_back(nIterGRASP);
		foIndicatorCalibration.push_back(seed);
		//getchar();
		//cout << foIndicatorCalibration << endl;
		vfoIndicatorCalibration.push_back(foIndicatorCalibration);
		vSolutionsBatches.push_back(sol->first);

	}

	// =================== PRINTING RESULTS ========================
	for (int n = 0; n < nBatches; n++)
	{

		for (int i = 0; i < vfoIndicatorCalibration[n].size(); i++)
			cout << vfoIndicatorCalibration[n][i] << "\t";

		cout << endl;
	}
	// =======================================================

	// =================== PRINTING RESULTS ON FILE ========================
	string calibrationFile = "./microGridCalibration";

	FILE* fResults = fopen(calibrationFile.c_str(), "a");
	for (int n = 0; n < nBatches; n++)
	{
		for (int i = 0; i < vfoIndicatorCalibration[n].size(); i++)
			fprintf(fResults, "%.7f\t", vfoIndicatorCalibration[n][i]);
		fprintf(fResults, "\n");
	}

	fclose(fResults);
	// =======================================================

	return 0;
}

//
//for (int w = 4; w >= 1; w--)
//	{
//		vector<double> foIndicatorsMAPE;
//		vector<double> foIndicatorsRMSE;
//
//		for (int day = 1; day <= 7; day++)
//		{
//			vector<vector<double> > validationSet; //validation set for calibration
//			validationSet.push_back(rF.getPartsForecastsEndToBegin(0, w * 168 - stepsAhead * day, nValidationSamples));
//			vector<double> foIndicators;
//			foIndicators = forecastObject.returnErrors(sol, validationSet);
//			foIndicatorsMAPE.push_back(foIndicators[MAPE_INDEX]);
//			foIndicatorsRMSE.push_back(foIndicators[RMSE_INDEX]);
//		}
//		double sumMAPE = accumulate(foIndicatorsMAPE.begin(), foIndicatorsMAPE.end(), 0.0);
//		double sumRMSE = accumulate(foIndicatorsRMSE.begin(), foIndicatorsRMSE.end(), 0.0);
//
//		foIndicatorCalibration.push_back(sumMAPE/foIndicatorsMAPE.size());
//		foIndicatorCalibration.push_back(sumRMSE/foIndicatorsRMSE.size());
//	}
