//#ifndef HFM_USINGNONDOMINATEDMODELS_HPP_
//#define HFM_USINGNONDOMINATEDMODELS_HPP_
//#endif /*HFM_USINGNONDOMINATEDMODELS_HPP_*/

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

int smartStorage(int argc, char **argv)
{

	cout << "Welcome to SmartStorage prob. forecast generation wind, solar, price" << endl;
	RandGenMersenneTwister rg;
	//long
	long seed = time(nullptr); //CalibrationMode
	//seed = 1;
	cout << "Seed = " << seed << endl;
	srand(seed);
	rg.setSeed(seed);

	if (argc != 3)
	{
		cout << "Parametros incorretos!" << endl;
		cout << "Os parametros esperados sao: data output" << endl;
		exit(1);
	}

	const char* caminho = argv[1];
	const char* caminhoOutput = argv[2];
	string nome = caminho;
	string nomeOutput = caminhoOutput;

	vector<string> vParametersFiles;
	vParametersFiles.push_back("./MyProjects/ConfigParameters/JTEuropean/1Column_124Points");

	//parametersFile = "./MyProjects/configParametersBestPrice3ColunaseForward";

	vector<string> explanatoryVariables;

	//string datasetWind = "./MyProjects/EFP/Instance/SmartStorage/windProdComplete";
	//string datasetSolar = "./MyProjects/EFP/Instance/SmartStorage/solarProdComplete";
	//string datasetConsumption = "./MyProjects/EFP/Instance/SmartStorage/consumptionComplete";

	stringstream inputFile;
	inputFile << "./MyProjects/EFP/Instance/SmartStorage/" << nome;

	explanatoryVariables.push_back(inputFile.str());
	//explanatoryVariables.push_back(datasetSolar);
	//explanatoryVariables.push_back(datasetConsumption);

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

	int maxPrecision = 20;
	int maxTrainningRounds = 1000;

	double initialDesv;
	double mutationDesv;

	int maxMu = 100;
	int maxInitialDesv = 10;
	int maxMutationDesv = 30;

	int nBatches = 200;

	int timeES = 30;
	int timeVND = 0;
	int timeILS = 0;
	int timeGRASP = 0;

	int stepsAheadR = 24;
	vector<vector<double> > batchOfResults;
	vector<vector<double> > validationSet; //validation set for calibration
	for (int n = 0; n < nBatches; n++)
	{
		int randomPrecision = rg.rand(maxPrecision) + 10;
		int randomParametersFiles = rg.rand(vParametersFiles.size());
		int evalFOMinimizer = MAE_INDEX;
		int evalAprox = rg.rand(2); //Enayatifar aproximation using previous values
		int construtive = rg.rand(2);
		initialDesv = rg.rand(maxInitialDesv) + 1;
		mutationDesv = rg.rand(maxMutationDesv) + 1;
		int mu = rg.rand(maxMu) + 1;
		int lambda = mu * 6;
		double alphaACF = rg.rand01(); //limit ACF for construtive ACF
		int alphaSign = rg.rand(2);
		if (alphaSign == 0)
			alphaACF = alphaACF * -1;

		// ============ FORCES ======================
		mu = 50;
		lambda = mu * 6;
		initialDesv = 10;
		mutationDesv = 20;
		//randomPrecision = 100;
		randomParametersFiles = 0;
		evalFOMinimizer = MAPE_INDEX;
		evalAprox = 2;
		construtive = 2;
		alphaACF = 0;
		// ============ END FORCES ======================

		// ============= METHOD PARAMETERS=================
		HFMParams methodParam;

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

		problemParam.setStepsAhead(stepsAheadR);
		int stepsAhead = problemParam.getStepsAhead();
		//stepsAhead = 1;
		// =========================================== CONSTRUTIVE 0 AND 1
		//========ADAPTATION FOR CONSTRUTIVE 2 ===============
		if (construtive == 2) //ACF construtive
			problemParam.setMaxLag(500);

		int maxNotUsedForTest = problemParam.getMaxLag();

		cout << "maxNotUsedForTest: " << maxNotUsedForTest << endl;

		//getchar();
		//int randomObjFunc = rg.rand(2);
		//cout<<"randomObjFunc = "<<randomObjFunc<<endl;
		//problemParam.setFunction(randomObjFunc);

		//validationBlindForecastings.clear();

		int nTrainningRounds = rg.rand(maxTrainningRounds) + 1;
		nTrainningRounds = 3;
		int nTotalForecastingsTrainningSet = maxNotUsedForTest + nTrainningRounds * stepsAhead;
		cout << "nTrainningRounds: " << nTrainningRounds << endl;
		cout << "nTotalForecastingsTrainningSet: " << nTotalForecastingsTrainningSet << endl;

		vector<vector<double> > trainningSet; // trainningSetVector
		int beginTrainingSet = 672;
		trainningSet.push_back(rF.getPartsForecastsEndToBegin(0, beginTrainingSet, nTotalForecastingsTrainningSet));

		ForecastClass forecastObject(trainningSet, problemParam, rg, methodParam);

		pair<Solution<RepEFP>, Evaluation>* sol;

		int optMethod = rg.rand(2);
		optMethod = 0;
		if (optMethod == 0)
			sol = forecastObject.run(timeES, timeVND, timeILS);
		else
			sol = forecastObject.runGILS(timeGRASP, timeILS); // GRASP + ILS

		//int needInputs = sol->first.getR().earliestInput + stepsAhead;
		int needInputs = problemParam.getMaxLag();

		validationSet.clear();
		validationSet.push_back(rF.getPartsForecastsEndToBegin(0, beginTrainingSet-stepsAhead, needInputs));
		//validationSet.push_back(validationSetPure);
		batchOfResults.push_back(*forecastObject.returnForecasts(sol, validationSet));

	}

	vector<vector<double> > finalResultQuantis;

	for (int sa = 0; sa < stepsAheadR; sa++)
	{
		vector<double> distribution;
		for (int n = 0; n < nBatches; n++)
		{
			distribution.push_back(batchOfResults[n][sa]);
		}
		vector<double> quantis;
		quantis = rF.generateQuantis(distribution, 1, 99);
		finalResultQuantis.push_back(quantis);
	}

	stringstream ss;
	ss << "./MyProjects/EFP/Instance/SmartStorage/ResultsProb/" << nomeOutput;

	rF.exportQuantisVector(finalResultQuantis, ss.str());

	double pinball = rF.getPinball(finalResultQuantis, validationSet[0]);
	cout << "Pinball error: " << pinball << endl;

	FILE* fResults = fopen(ss.str().c_str(), "a");
	fprintf(fResults, "\n\nPinball error:%f\n", pinball);
	fclose(fResults);


	return 0;
}
