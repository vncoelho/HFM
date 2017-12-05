// ===================================
// Main.cpp file generated by OptFrame
// Project EFP
// ===================================

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "../../../OptFrame/RandGen.hpp"
#include "../../../OptFrame/Util/RandGenMersenneTwister.hpp"

using namespace std;
using namespace optframe;
using namespace HFM;

int priceCompetitionBlind(int argc, char **argv)
{
	cout << "Welcome to Price Competition Mode Blind Result Generation..." << endl;
	RandGenMersenneTwister rg;
	long seed = time(NULL);
	seed = 2;
	cout << "Seed = " << seed << endl;
	srand(seed);
	rg.setSeed(seed);

	if (argc != 10)
	{
		cout << "Parametros incorretos!" << endl;
		cout << "Os parametros esperados sao: nome nomeValidationSet saida parameters options precision" << endl;
		exit(1);
	}

	const char* caminho = argv[1];
	const char* caminhoValidation = argv[2];
	const char* caminhoOutput = argv[3];
	const char* caminhoParameters = argv[4];
	int instN = atoi(argv[5]);
	int stepsAhead = atoi(argv[6]);
	int precision = atoi(argv[7]);
	int function = atoi(argv[8]); // 0 heavisede ou 1 sigmoide
	int mu = atoi(argv[9]);

	string nome = caminho;
	string nomeValidationSet = caminhoValidation;
	string nomeOutput = caminhoOutput;
	string parametersFile = caminhoParameters;

	//===================================
	cout << "Parametros:" << endl;
	cout << "nome=" << nome << endl;
	cout << "nomeValidationSet=" << nomeValidationSet << endl;
	cout << "nomeOutput=" << nomeOutput << endl;
	cout << "nomeParameters=" << parametersFile << endl;
	cout << "instN=" << instN << endl;
	cout << "stepsAhead=" << stepsAhead << endl;
	cout << "precision=" << precision << endl;
	cout << "function=" << function << endl;
	cout << "mu=" << mu << endl;
	//===================================

	vector<string> vParametersFiles;
	vParametersFiles.push_back("./MyProjects/ConfigParameters/Price_1Column_124Points");
	vParametersFiles.push_back("./MyProjects/ConfigParameters/Price_1Column_7Points");
	vParametersFiles.push_back("./MyProjects/ConfigParameters/Price_2Column_11Points");
	vParametersFiles.push_back("./MyProjects/ConfigParameters/Price_3Column_15Points");
	vParametersFiles.push_back("./MyProjects/ConfigParameters/Price_3ColumnsForward");
	//parametersFile = "./MyProjects/configParametersBestPrice3ColunaseForward";

	vector<string> explanatoryVariables;

	string nomePriceCompetition1 = "./MyProjects/EFP/Instance/PriceForecasting/Task8/task8CompleteCol1";
	explanatoryVariables.push_back(nomePriceCompetition1);

	treatForecasts rF(explanatoryVariables);

	//vector<vector<double> > batchOfBlindResults; //vector with the blind results of each batch

	int nBatches = 1;
	//int maxPrecision = 30;
	//int maxTrainningRounds = 20;
	int timeES = 600;
	int timeVND = 600;
	int timeILS = 600;
	int timeGRASP = 600;

	vector<vector<double> > trainningSet; // trainningSetVector
	vector<vector<double> > batchOfBlindResults;

	for (int n = 0; n < nBatches; n++)
	{
		// ============ FORCES ======================
		int randomPrecision = 30; //best precision
		int randomParametersFiles = 1; //best parameters config Price_1Column_7Points
		int evalFOMinimizer = PINBALL_INDEX;
		int evalAprox = 5;
		int nTrainningRounds = 5;
		mu = 10;
		int lambda = mu * 6;
		double initialDesv = 1;
		double mutationDesv = 2;
		int construtive = 2;
		double alphaACF = 0;
		// ============ END FORCES ======================

		// ============= METHOD PARAMETERS=================
		methodParameters methodParam;
		//seting up ES params
		methodParam.setESMU(mu);
		methodParam.setESLambda(lambda);
		methodParam.setESInitialDesv(initialDesv);
		methodParam.setESMutationDesv(mutationDesv);
		methodParam.setESMaxG(100000);
		//seting up Construtive params
		methodParam.setConstrutiveMethod(construtive);
		methodParam.setConstrutivePrecision(randomPrecision);
		vector<double> vAlphaACFlimits;
		vAlphaACFlimits.push_back(alphaACF);
		methodParam.setConstrutiveLimitAlphaACF(vAlphaACFlimits);
		//seting up Eval params
		methodParam.setEvalAprox(evalAprox);
		methodParam.setEvalFOMinimizer(evalFOMinimizer);
		// ==========================================

		// ================== READ FILE ============== CONSTRUTIVE 0 AND 1
		ProblemParameters problemParam(vParametersFiles[randomParametersFiles]);
		stepsAhead = problemParam.getStepsAhead();
		// =========================================== CONSTRUTIVE 0 AND 1
		//========ADAPTATION FOR CONSTRUTIVE 2 ===============
		if (construtive == 2) //ACF construtive
			problemParam.setMaxLag(300);
		//=================================================

		//getchar();
		//int randomObjFunc = rg.rand(2);
		//cout<<"randomObjFunc = "<<randomObjFunc<<endl;
		//problemParam.setFunction(randomObjFunc);

		trainningSet.clear();
		//validationBlindForecastings.clear();
		int maxNotUsedForTest = problemParam.getMaxLag();
		int nTotalForecastingsTrainningSet = maxNotUsedForTest + nTrainningRounds * stepsAhead;

		int beginTrainingSet = 0; // best begin trainning set

		//int beginTraining = n * (nTotalForecastings / 10) + nTotalForecastings;
		//tForecastings.push_back(rF.getLastForecasts(0, nTotalForecastings)); //For the target file
		//tForecastings.push_back(rF.getLastForecasts(1, nTotalForecastings + stepsAhead)); //the auxiliary files has the stepsAhead of the next day to be forecasted
		//tForecastings.push_back(rF.getLastForecasts(2, nTotalForecastings + stepsAhead)); //the auxiliary files has the stepsAhead of the next day to be forecasted

		trainningSet.push_back(rF.getPartsForecastsEndToBegin(0, beginTrainingSet, nTotalForecastingsTrainningSet));

		//trainningSet.push_back(rF.getPartsForecastsEndToBegin(1, beginTrainingSet, nTotalForecastingsTrainningSet + stepsAhead));
		//trainningSet.push_back(rF.getPartsForecastsEndToBegin(2, beginTrainingSet, nTotalForecastingsTrainningSet + stepsAhead));

		ForecastClass forecastObject(trainningSet, problemParam, rg, methodParam);


		pair<Solution<RepEFP>, Evaluation>* sol;

		int optMethod = rg.rand(2);
		optMethod = 0;
		if (optMethod == 0)
			sol = forecastObject.run(timeES, timeVND, timeILS);
		else
			sol = forecastObject.runGILS(timeGRASP, timeILS); // GRASP + ILS

		vector<vector<double> > validationBlindForecastings; //Validation Blind for the Competition
		int maxLag = problemParam.getMaxLag();

		validationBlindForecastings.push_back(rF.getPartsForecastsEndToBegin(0, 0, maxLag));
		//validationBlindForecastings.push_back(rF.getLastForecasts(1, problemParam.getNotUsedForTest() + stepsAhead));
		//validationBlindForecastings.push_back(rF.getLastForecasts(2, problemParam.getNotUsedForTest() + stepsAhead));



		vector<double> blindResults;
		blindResults = *forecastObject.returnBlind(sol->first.getR(), validationBlindForecastings);
		batchOfBlindResults.push_back(blindResults);
		//cout << blindResults << endl;


		/*
		vector<vector<double> > validationTest; //Validation Blind for the Competition
		validationTest.push_back(rF.getPartsForecastsEndToBegin(0, 0, sol->first.getR().earliestInput + stepsAhead));
		vector<double> resultsErrors;
		resultsErrors = forecastObject.returnErrors(sol, validationTest);
		cout << resultsErrors << endl;
		getchar();*/


	}

	cout << batchOfBlindResults << endl;
	//getchar();
	vector<vector<double> > finalResultQuantis;

	for (int sa = 0; sa < stepsAhead; sa++)
	{
		vector<double> distribution;
		for (int n = 0; n < nBatches; n++)
		{
			distribution.push_back(batchOfBlindResults[n][sa]);
		}
		vector<double> quantis;
		quantis = rF.generateQuantis(distribution, 1, 99);
		finalResultQuantis.push_back(quantis);
	}

	//cout << finalResultQuantis << endl;
	rF.exportQuantisVector(finalResultQuantis, "./ProbResults/quantilFromDifferentBatches");

	double bestQuantilError = 0.5;
	vector<vector<double> > quantilBestError = rF.generateQuantisProbabilisticVectorFixedQuantilError(batchOfBlindResults[0], bestQuantilError);
	rF.exportQuantisVector(quantilBestError, "./ProbResults/finalSubmissionTask9_1_Good05");

	return 0;
}

int priceCompetitionCalibrationMode(int argc, char **argv)
{
	cout << "Welcome to Price Competition Calibration Mode..." << endl;
	RandGenMersenneTwister rg;
	//long seed = 1;
	long seed = time(NULL); //CalibrationMode
	cout << "Seed = " << seed << endl;
	srand(seed);
	rg.setSeed(seed);

	if (argc != 10)
	{
		cout << "Parametros incorretos!" << endl;
		cout << "Os parametros esperados sao: nome nomeValidationSet saida parameters options precision" << endl;
		exit(1);
	}

	const char* caminho = argv[1];
	const char* caminhoValidation = argv[2];
	const char* caminhoOutput = argv[3];
	const char* caminhoParameters = argv[4];
	int instN = atoi(argv[5]);
	int stepsAhead = atoi(argv[6]);
	int precision = atoi(argv[7]);
	int function = atoi(argv[8]); // 0 heavisede ou 1 sigmoide
	int mu = atoi(argv[9]);

	string nome = caminho;
	string nomeValidationSet = caminhoValidation;
	string nomeOutput = caminhoOutput;
	string parametersFile = caminhoParameters;
	//===================================
	cout << "Parametros:" << endl;
	cout << "nome=" << nome << endl;
	cout << "nomeValidationSet=" << nomeValidationSet << endl;
	cout << "nomeOutput=" << nomeOutput << endl;
	cout << "nomeParameters=" << parametersFile << endl;
	cout << "instN=" << instN << endl;
	cout << "stepsAhead=" << stepsAhead << endl;
	cout << "precision=" << precision << endl;
	cout << "function=" << function << endl;
	cout << "mu=" << mu << endl;
	//=======================================

	vector<string> vParametersFiles;
	vParametersFiles.push_back("./MyProjects/ConfigParameters/Price_1Column_124Points");
	vParametersFiles.push_back("./MyProjects/ConfigParameters/Price_1Column_7Points");
	vParametersFiles.push_back("./MyProjects/ConfigParameters/Price_2Column_11Points");
	vParametersFiles.push_back("./MyProjects/ConfigParameters/Price_3Column_15Points");
	vParametersFiles.push_back("./MyProjects/ConfigParameters/Price_3ColumnsForward");
	//parametersFile = "./MyProjects/configParametersBestPrice3ColunaseForward";

	vector<string> explanatoryVariables;

	string nomePriceCompetition1 = "./MyProjects/EFP/Instance/PriceForecasting/Task9/task9CompleteCol1";
	string nomePriceCompetition2 = "./MyProjects/EFP/Instance/PriceForecasting/Task9/task9CompleteCol2";
	string nomePriceCompetition3 = "./MyProjects/EFP/Instance/PriceForecasting/Task9/task9CompleteCol3";
	explanatoryVariables.push_back(nomePriceCompetition1);
	explanatoryVariables.push_back(nomePriceCompetition2);
	explanatoryVariables.push_back(nomePriceCompetition3);

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

	int maxPrecision = 50;
	int maxTrainningRounds = 10;

	int lambda = mu * 6;

	int maxMu = 100;
	int maxInitialDesv = 10;
	int maxMutationDesv = 30;

	int maxTimeES = 200;
	int maxTimeVND = 200;
	int maxTimeILS = 200;
	int timeES = rg.rand(maxTimeES) + 1;
	int timeVND = rg.rand(maxTimeVND) + 1;
	int timeILS = rg.rand(maxTimeILS) + 1;

	// ============ FORCES ======================
	timeES = 20;
	timeVND = 20;
	timeILS = 20;
	// ============ END FORCES ======================

	int nBatches = 60;

	vector<vector<double> > vfoIndicatorCalibration; //vector with the FO of each batch

	//vector<SolutionEFP*> vSolutionsBatches; //vector with the solution of each batch

	for (int n = 0; n < nBatches; n++)
	{
		int randomPrecision = rg.rand(maxPrecision) + 10;
		int randomParametersFiles = rg.rand(vParametersFiles.size());
		int function = rg.rand(2);
		int construtive = rg.rand(2);
		int initialDesv = rg.rand(maxInitialDesv) + 1;
		int mutationDesv = rg.rand(maxMutationDesv) + 1;
		mu = rg.rand(maxMu) + 1;
		lambda = mu * 6;
		int evalFOMinimizer = rg.rand(EVALUTORS_NMETRICS_ENUM_COUNT); //tree is the number of possible objetive function index minimizers
		int evalAprox = rg.rand(4); //Enayatifar aproximation using previous values

		//forces
		mu = 10;
		lambda = mu * 6;
		initialDesv = 1;
		mutationDesv = 2;
		randomPrecision = 20;

		// ============= METHOD PARAMETERS=================
		methodParameters methodParam;
		//seting up ES params
		methodParam.setESMU(mu);
		methodParam.setESLambda(lambda);
		methodParam.setESInitialDesv(initialDesv);
		methodParam.setESMutationDesv(mutationDesv);
		methodParam.setESMaxG(100000);
		//seting up Construtive params
		methodParam.setConstrutiveMethod(construtive);
		methodParam.setConstrutivePrecision(randomPrecision);
		vector<double> vAlphaACFlimits;
		vAlphaACFlimits.push_back(0.8);
		methodParam.setConstrutiveLimitAlphaACF(vAlphaACFlimits);
		//seting up Eval params
		methodParam.setEvalAprox(evalAprox);
		methodParam.setEvalFOMinimizer(evalFOMinimizer);
		// ==========================================

		// ================== READ FILE ============== CONSTRUTIVE 0 AND 1
		ProblemParameters problemParam(vParametersFiles[randomParametersFiles]);
		stepsAhead = problemParam.getStepsAhead();
		// =========================================== CONSTRUTIVE 0 AND 1
		//========ADAPTATION FOR CONSTRUTIVE 2 ===============
		if (construtive == 2) //ACF construtive
			problemParam.setMaxLag(300);
		//=================================================

		int maxNotUsedForTest = problemParam.getMaxLag();
		int nTrainningRounds = rg.rand(maxTrainningRounds) + 1;
		int nTotalForecastingsTrainningSet = maxNotUsedForTest + nTrainningRounds * stepsAhead;
		int beginTrainingSet = rg.rand(rF.getForecastsDataSize());

		//if (bRand == 0)

		//else
		//beginTrainingSet = nTotalForecastingsValidationSet + n * (nTotalForecastingsTrainningSet / 10);

		while ((beginTrainingSet + nTotalForecastingsTrainningSet > rF.getForecastsDataSize()) && (beginTrainingSet + stepsAhead < rF.getForecastsDataSize()))
		{
			beginTrainingSet = rg.rand(rF.getForecastsDataSize());

			//if (bRand == 0)
			//beginTrainingSet = startTraining;
			//else
			//beginTrainingSet = nTotalForecastingsValidationSet + n * (nTotalForecastingsTrainningSet / 10);
		}

		vector<vector<double> > trainningSet; // trainningSetVector
		trainningSet.push_back(rF.getPartsForecastsEndToBegin(0, beginTrainingSet, nTotalForecastingsTrainningSet));
		trainningSet.push_back(rF.getPartsForecastsEndToBegin(1, beginTrainingSet, nTotalForecastingsTrainningSet + stepsAhead));
		trainningSet.push_back(rF.getPartsForecastsEndToBegin(2, beginTrainingSet, nTotalForecastingsTrainningSet + stepsAhead));

		int nValidationSamples = maxNotUsedForTest + stepsAhead;
		/*Example
		 * [ 1 2 3 4 5 6 7 8 9 10 11 12 13 ]
		 * beginTrainingSet = 3
		 * stepsAhead = 2
		 * nTrainningRounds = 3
		 * noteUsed = 2
		 * nTotalForecastingsTrainningSet = 8
		 * trainningSet[0] = [3 4 5 6 7 8 9 10]
		 * validationSet[0] = [9 10 11 12]
		 */
		int beginValidationSet = beginTrainingSet - stepsAhead;
		//beginValidationSet = 0;
		vector<vector<double> > validationSet; //validation set for calibration
		validationSet.push_back(rF.getPartsForecastsEndToBegin(0, beginValidationSet, nValidationSamples));
		validationSet.push_back(rF.getPartsForecastsEndToBegin(1, beginValidationSet, nValidationSamples + stepsAhead));
		validationSet.push_back(rF.getPartsForecastsEndToBegin(2, beginValidationSet, nValidationSamples + stepsAhead));

		ForecastClass forecastObject(trainningSet, problemParam, rg, methodParam);

		pair<Solution<RepEFP>, Evaluation>* sol;

		sol = forecastObject.run(timeES, timeVND, timeILS);

		/*
		 vector<vector<double> > validationBlindForecastings; //Validation Blind for the Competition
		 validationBlindForecastings.push_back(rF.getLastForecasts(0, problemParam.getNotUsedForTest()));
		 validationBlindForecastings.push_back(rF.getLastForecasts(1, problemParam.getNotUsedForTest() + stepsAhead));
		 validationBlindForecastings.push_back(rF.getLastForecasts(2, problemParam.getNotUsedForTest() + stepsAhead));
		 vector<double> blindResults;
		 blindResults = forecastObject.returnBlind(sol, validationBlindForecastings);
		 batchOfBlindResults.push_back(blindResults);
		 */

		double intervalOfBeginTrainningSet = double(beginTrainingSet) / double(rF.getForecastsDataSize());
		//int intervalNumber = 0;
		//for (int iV = 0; iV < 10000; iV = iV + 100)
		//if (intervalOfBeginTrainningSet > iV)
		//intervalNumber++;

		vector<double> foIndicatorCalibration;
		foIndicatorCalibration = *forecastObject.returnErrors(sol->first.getR(), validationSet);
		foIndicatorCalibration.push_back(randomPrecision);
		foIndicatorCalibration.push_back(randomParametersFiles);
		foIndicatorCalibration.push_back(nTrainningRounds);
		foIndicatorCalibration.push_back(beginTrainingSet);
		foIndicatorCalibration.push_back(intervalOfBeginTrainningSet);
		foIndicatorCalibration.push_back(nTotalForecastingsTrainningSet);
		//foIndicatorCalibration.push_back(beginValidationSet);
		//foIndicatorCalibration.push_back(nTotalForecastingsValidationSet);
		foIndicatorCalibration.push_back(mu);
		foIndicatorCalibration.push_back(lambda);
		foIndicatorCalibration.push_back(initialDesv);
		foIndicatorCalibration.push_back(mutationDesv);
		foIndicatorCalibration.push_back(timeES);
		foIndicatorCalibration.push_back(timeVND);
		foIndicatorCalibration.push_back(evalFOMinimizer);
		foIndicatorCalibration.push_back(evalAprox);
		foIndicatorCalibration.push_back(function);
		foIndicatorCalibration.push_back(construtive);

		//getchar();
		//cout << foIndicatorCalibration << endl;
		vfoIndicatorCalibration.push_back(foIndicatorCalibration);
		//vSolutionsBatches.push_back(&(sol->first.clone()));
		delete &sol->first;
		delete &sol->second;
		delete sol;
	}

	cout << vfoIndicatorCalibration << endl;
	for (int n = 0; n < nBatches; n++)
	{

		for (int i = 0; i <  (int) vfoIndicatorCalibration[n].size(); i++)
			cout << vfoIndicatorCalibration[n][i] << "\t";

		cout << endl;
	}

	string calibrationFile = "./priceCalibrationTask7_Func_Construtive";

	FILE* fResults = fopen(calibrationFile.c_str(), "a");
	for (int n = 0; n < nBatches; n++)
	{

		for (int i = 0; i <  (int) vfoIndicatorCalibration[n].size(); i++)
			fprintf(fResults, "%.7f\t", vfoIndicatorCalibration[n][i]);
		fprintf(fResults, "\n");
	}

	fclose(fResults);

	return 0;
}

