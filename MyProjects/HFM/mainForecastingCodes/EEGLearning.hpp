// ===================================
// Main.cpp file generated by OptFrame
// Project EFP
// ===================================

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <numeric>
#include <iomanip>
#include "../../../OptFrame/RandGen.hpp"
#include "../../../OptFrame/Util/RandGenMersenneTwister.hpp"

using namespace std;
using namespace optframe;
using namespace HFM;

struct HFMModelAndParam
{
	pair<Solution<RepEFP>, Evaluation>* HFMModel;
	vector<double> forecastingErrors;
	int fH;
	int v; //volunteer which was used to train the data
	int channel; //volunteer channel which was used to train the data

	HFMModelAndParam(pair<Solution<RepEFP>, Evaluation>* _HFMModel, vector<double> _forecastingErrors, int _fH, int _v, int _channel) :
			HFMModel(_HFMModel), forecastingErrors(_forecastingErrors), fH(_fH), v(_v), channel(_channel)
	{
	}
};

static bool comparaVolunteersPoints(pair<int, int> d1, pair<int, int> d2)
{
	return d1.second > d2.second;
}

static bool comparaVolunteersPoints2(pair<int, int> d1, pair<int, int> d2)
{
	return d1.second < d2.second;
}

vector<pair<int, int> > classificationRuleLessMax(vector<pair<double, int> > betterAVG, vector<pair<double, int> > betterMin, vector<pair<double, int> > betterMax, vector<pair<double, int> > betterGAP, vector<int> metricWeights, int nVolunteers)
{
	int nIQ = metricWeights.size(); //number of indicators of quality
	int avgWeight = 4;
	int minWeight = 1;
	int maxWeight = 1;
	int gapWeight = 3;
	vector<pair<int, int> > volunteersPoints(nVolunteers);
	for (int v = 0; v < nVolunteers; v++)
		volunteersPoints[v] = make_pair(v, 0);

	for (int m = 0; m < nIQ; m++)
	{
		volunteersPoints[betterAVG[m].second].second += metricWeights[m] * avgWeight;
		volunteersPoints[betterMin[m].second].second += metricWeights[m] * minWeight;
		volunteersPoints[betterMax[m].second].second -= metricWeights[m] * maxWeight;
		volunteersPoints[betterGAP[m].second].second += metricWeights[m] * gapWeight;
	}

	sort(volunteersPoints.begin(), volunteersPoints.end(), comparaVolunteersPoints); // ordena com QuickSort

//	cout << volunteersPoints << endl;
	return volunteersPoints;
}

vector<pair<int, int> > classificationRulePlusMax(vector<pair<double, int> > betterAVG, vector<pair<double, int> > betterMin, vector<pair<double, int> > betterMax, vector<pair<double, int> > betterGAP, vector<int> metricWeights, int nVolunteers)
{
	int nIQ = metricWeights.size(); //number of indicators of quality
	int avgWeight = 4;
	int minWeight = 1;
	int maxWeight = 1;
	int gapWeight = 3;
	vector<pair<int, int> > volunteersPoints(nVolunteers);
	for (int v = 0; v < nVolunteers; v++)
		volunteersPoints[v] = make_pair(v, 0);

	for (int m = 0; m < nIQ; m++)
	{
		volunteersPoints[betterAVG[m].second].second += metricWeights[m] * avgWeight;
		volunteersPoints[betterMin[m].second].second += metricWeights[m] * minWeight;
		volunteersPoints[betterMax[m].second].second += metricWeights[m] * maxWeight;
		volunteersPoints[betterGAP[m].second].second += metricWeights[m] * gapWeight;
	}

	sort(volunteersPoints.begin(), volunteersPoints.end(), comparaVolunteersPoints); // ordena com QuickSort

//	cout << volunteersPoints << endl;
	return volunteersPoints;
}

vector<pair<int, int> > classificationRuleNoGapSumMax(vector<pair<double, int> > betterAVG, vector<pair<double, int> > betterMin, vector<pair<double, int> > betterMax, vector<pair<double, int> > betterGAP, vector<int> metricWeights, int nVolunteers)
{
	int nIQ = metricWeights.size(); //number of indicators of quality
	int avgWeight = 4;
	int minWeight = 1;
	int maxWeight = 1;
	vector<pair<int, int> > volunteersPoints(nVolunteers);
	for (int v = 0; v < nVolunteers; v++)
		volunteersPoints[v] = make_pair(v, 0);

	for (int m = 0; m < nIQ; m++)
	{
		volunteersPoints[betterAVG[m].second].second += metricWeights[m] * avgWeight;
		volunteersPoints[betterMin[m].second].second += metricWeights[m] * minWeight;
		volunteersPoints[betterMax[m].second].second += metricWeights[m] * maxWeight;
	}

	sort(volunteersPoints.begin(), volunteersPoints.end(), comparaVolunteersPoints); // ordena com QuickSort

//	cout << volunteersPoints << endl;
	return volunteersPoints;
}

vector<pair<int, int> > classificationRuleNoGapLessMax(vector<pair<double, int> > betterAVG, vector<pair<double, int> > betterMin, vector<pair<double, int> > betterMax, vector<pair<double, int> > betterGAP, vector<int> metricWeights, int nVolunteers)
{
	int nIQ = metricWeights.size(); //number of indicators of quality
	int avgWeight = 4;
	int minWeight = 1;
	int maxWeight = 1;
	vector<pair<int, int> > volunteersPoints(nVolunteers);
	for (int v = 0; v < nVolunteers; v++)
		volunteersPoints[v] = make_pair(v, 0);

	for (int m = 0; m < nIQ; m++)
	{
		volunteersPoints[betterAVG[m].second].second += metricWeights[m] * avgWeight;
		volunteersPoints[betterMin[m].second].second += metricWeights[m] * minWeight;
		volunteersPoints[betterMax[m].second].second -= metricWeights[m] * maxWeight;
	}

	sort(volunteersPoints.begin(), volunteersPoints.end(), comparaVolunteersPoints); // ordena com QuickSort

//	cout << volunteersPoints << endl;
	return volunteersPoints;
}
vector<pair<int, int> > classificationRuleOrderLessMax(Matrix<double> resultsAVG, Matrix<double> resultsMIN, Matrix<double> resultsMAX, Matrix<double> resultsGAP, vector<int> metricWeights)
{
	int nVolunteers = resultsAVG.getNumCols();
	int nIQ = metricWeights.size(); //number of indicators of quality
	int avgWeight = 4;
	int minWeight = 1;
	int maxWeight = 1;
	int gapWeight = 3;

	vector<pair<int, int> > volunteersPoints(nVolunteers);
	for (int v = 0; v < nVolunteers; v++)
		volunteersPoints[v] = make_pair(v, 0);

	for (int m = 0; m < nIQ; m++)
	{
		vector<pair<int, double> > avgOrder(nVolunteers);
		vector<pair<int, double> > minOrder(nVolunteers);
		vector<pair<int, double> > maxOrder(nVolunteers);
		vector<pair<int, double> > gapOrder(nVolunteers);
		for (int v = 0; v < nVolunteers; v++)
		{
			avgOrder[v] = make_pair(v, resultsAVG(m, v));
			minOrder[v] = make_pair(v, resultsMIN(m, v));
			maxOrder[v] = make_pair(v, resultsMAX(m, v));
			gapOrder[v] = make_pair(v, resultsGAP(m, v));
		}
		sort(avgOrder.begin(), avgOrder.end(), comparaVolunteersPoints); // ordena com QuickSort
		sort(minOrder.begin(), minOrder.end(), comparaVolunteersPoints); // ordena com QuickSort
		sort(maxOrder.begin(), maxOrder.end(), comparaVolunteersPoints2); // ordena com QuickSort
		sort(gapOrder.begin(), gapOrder.end(), comparaVolunteersPoints); // ordena com QuickSort

//		cout << "ordered values for different IQ:" << m << endl;
//		cout << avgOrder << endl;
//		cout << minOrder << endl;
//		cout << maxOrder << endl;
//		cout << gapOrder << endl << endl;
//		getchar();
		for (int v = 0; v < nVolunteers; v++)
		{
			volunteersPoints[avgOrder[v].first].second += (metricWeights[m] * avgWeight * (v + 1));
			volunteersPoints[minOrder[v].first].second += (metricWeights[m] * minWeight * (v + 1));
			volunteersPoints[maxOrder[v].first].second -= (metricWeights[m] * maxWeight * (v + 1));
			volunteersPoints[gapOrder[v].first].second += (metricWeights[m] * gapWeight * (v + 1));
		}

	}

	sort(volunteersPoints.begin(), volunteersPoints.end(), comparaVolunteersPoints); // ordena com QuickSort

//	cout << volunteersPoints << endl;

	return volunteersPoints;
}

vector<pair<int, int> > classificationRuleOrderPlusMax(Matrix<double> resultsAVG, Matrix<double> resultsMIN, Matrix<double> resultsMAX, Matrix<double> resultsGAP, vector<int> metricWeights)
{
	int nVolunteers = resultsAVG.getNumCols();
	int nIQ = metricWeights.size(); //number of indicators of quality
	int avgWeight = 4;
	int minWeight = 1;
	int maxWeight = 1;
	int gapWeight = 3;

	vector<pair<int, int> > volunteersPoints(nVolunteers);
	for (int v = 0; v < nVolunteers; v++)
		volunteersPoints[v] = make_pair(v, 0);

	for (int m = 0; m < nIQ; m++)
	{
		vector<pair<int, double> > avgOrder(nVolunteers);
		vector<pair<int, double> > minOrder(nVolunteers);
		vector<pair<int, double> > maxOrder(nVolunteers);
		vector<pair<int, double> > gapOrder(nVolunteers);
		for (int v = 0; v < nVolunteers; v++)
		{
			avgOrder[v] = make_pair(v, resultsAVG(m, v));
			minOrder[v] = make_pair(v, resultsMIN(m, v));
			maxOrder[v] = make_pair(v, resultsMAX(m, v));
			gapOrder[v] = make_pair(v, resultsGAP(m, v));
		}
		sort(avgOrder.begin(), avgOrder.end(), comparaVolunteersPoints); // ordena com QuickSort
		sort(minOrder.begin(), minOrder.end(), comparaVolunteersPoints); // ordena com QuickSort
		sort(maxOrder.begin(), maxOrder.end(), comparaVolunteersPoints); // ordena com QuickSort
		sort(gapOrder.begin(), gapOrder.end(), comparaVolunteersPoints); // ordena com QuickSort

//		cout << "ordered values for different IQ:" << m << endl;
//		cout << avgOrder << endl;
//		cout << minOrder << endl;
//		cout << maxOrder << endl;
//		cout << gapOrder << endl << endl;
//		getchar();
		for (int v = 0; v < nVolunteers; v++)
		{
			volunteersPoints[avgOrder[v].first].second += (metricWeights[m] * avgWeight * (v + 1));
			volunteersPoints[minOrder[v].first].second += (metricWeights[m] * minWeight * (v + 1));
			volunteersPoints[maxOrder[v].first].second += (metricWeights[m] * maxWeight * (v + 1));
			volunteersPoints[gapOrder[v].first].second += (metricWeights[m] * gapWeight * (v + 1));
		}

	}

	sort(volunteersPoints.begin(), volunteersPoints.end(), comparaVolunteersPoints); // ordena com QuickSort

//	cout << volunteersPoints << endl;

	return volunteersPoints;
}

vector<pair<double, int> > findBestPairsValuesByMetric(Matrix<double> results, bool minOrMax)
{
	int nIQ = results.getNumRows(); //number of indicators of quality
	vector<pair<double, int> > bestPairs(nIQ);

	double bigM = 10000000000;
	for (int m = 0; m < nIQ; m++)
	{
		double minMaxError;
		if (minOrMax)
		{
			minMaxError = bigM;
		}
		else
		{
			minMaxError = -bigM;
		}

		int minColIndex = -1;

		for (int v = 0; v <  (int) results.getNumCols(); v++)
		{
			if (minOrMax)
			{
				if (results(m, v) < minMaxError)
				{
					minMaxError = results(m, v);
					minColIndex = v;
				}
			}
			else
			{
				if (results(m, v) > minMaxError)
				{
					minMaxError = results(m, v);
					minColIndex = v;
				}
			}

		}

		bestPairs[m] = make_pair(minMaxError, minColIndex);

	}

	return bestPairs;
}

pair<Solution<RepEFP>, Evaluation>* learnModel(treatForecasts& tFTraining, int argvMaxLagRate, int argvTimeES, long seed, RandGen& rg, int evalFO, int fh)
{

	int evalFOMinimizer = evalFO;
	int mu = 100;
	int lambda = mu * 6;
	int contructiveNumberOfRules = 100;
	int evalAprox = 0;
	double alphaACF = -1;
	int construtive = 2;

	// ============ END FORCES ======================

	// ============= METHOD PARAMETERS=================
	HFMParams methodParam;
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

	problemParam.setStepsAhead(fh);

	int nTotalForecastingsTrainningSet = tFTraining.getForecastsSize(0);

	cout << "ContructiveACF HFM maximum number of Rules: " << contructiveNumberOfRules << endl;
	//========SET PROBLEM MAXIMUM LAG ===============
	cout << "argvMaxLagRate: " << argvMaxLagRate << endl;

	int iterationMaxLag = ((nTotalForecastingsTrainningSet - fh) * argvMaxLagRate) / 100.0;
	iterationMaxLag = ceil(iterationMaxLag);
	if (iterationMaxLag > (nTotalForecastingsTrainningSet - fh))
		iterationMaxLag--;
	if (iterationMaxLag <= 0)
		iterationMaxLag = 1;

	problemParam.setMaxLag(iterationMaxLag);
	int maxLag = problemParam.getMaxLag();

	//If maxUpperLag is greater than 0 model uses predicted data
	problemParam.setMaxUpperLag(-maxLag);
	problemParam.setMaxUpperLag(0);
	//int maxUpperLag = problemParam.getMaxUpperLag();
	//=================================================

	int timeES = argvTimeES; // online training time

	vector<double> foIndicators;

	int beginTrainingSet = 0;
	//int nTrainningRounds = 3;
	//int nTotalForecastingsTrainningSet = maxLag + nTrainningRounds * stepsAhead;

	cout << std::setprecision(3);
	cout << std::fixed;
	double NTRaprox = (nTotalForecastingsTrainningSet - maxLag) / double(fh);
	cout << "BeginTrainninningSet: " << beginTrainingSet << endl;
	cout << "#nTotalForecastingsTrainningSet: " << nTotalForecastingsTrainningSet << endl;
	cout << "#~NTR: " << NTRaprox << endl;
	cout << "#sizeTrainingSet: " << tFTraining.getForecastsSize(0) << endl;
	cout << "#maxNotUsed: " << maxLag << endl;
	cout << "#StepsAhead: " << fh << endl << endl;

	vector<vector<double> > trainningSet = tFTraining.getTS();
	; // trainningSetVector
	  //trainningSet.push_back(rF.getPartsForecastsEndToBegin(0, 0, nTotalForecastingsTrainningSet));

//	trainningSet.push_back(tFTraining.getPartsForecastsEndToBegin(0, 0, nTotalForecastingsTrainningSet));

	ForecastClass forecastObject(trainningSet, problemParam, rg, methodParam);

	//		forecastObject.runMultiObjSearch();
	//		getchar();
	pair<Solution<RepEFP>, Evaluation>* sol;
	sol = forecastObject.run(timeES, 0, 0);
	return sol;
}

vector<double> checkLearningAbility(treatForecasts& tFValidation, pair<Solution<RepEFP>, Evaluation>* sol, RandGen& rg, int fh)
{

	int mu = 100;
	int lambda = mu * 6;
	int evalFOMinimizer = SMAPE_INDEX;
	int contructiveNumberOfRules = 100;
	int evalAprox = 0;
	double alphaACF = -1;
	int construtive = 2;
	// ============ END FORCES ======================

	// ============= METHOD PARAMETERS=================
	HFMParams methodParam;
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

	problemParam.setStepsAhead(fh);

	int nTotalForecastingsTrainningSet = tFValidation.getForecastsSize(0);

	//========SET PROBLEM MAXIMUM LAG ===============
	problemParam.setMaxLag(sol->first.getR().earliestInput);
	int maxLag = problemParam.getMaxLag();

	if (nTotalForecastingsTrainningSet < (maxLag + fh))
	{
		cout << "ERROR on learnModel -- small TS size " << endl;
		cout << "nTotalForecastingsTrainningSet:" << nTotalForecastingsTrainningSet << "\tmaxLag + stepsAhead:" << maxLag + fh << endl;
	}

	//If maxUpperLag is greater than 0 model uses predicted data
	problemParam.setMaxUpperLag(0);

	//=================================================

	vector<vector<double> > validationSet = tFValidation.getTS();

	ForecastClass forecastObject(validationSet, problemParam, rg, methodParam);
	vector<double> errors = *forecastObject.returnErrors(sol->first.getR(), validationSet);

	return errors;
}

int EEGBiometricSystem(int argc, char **argv)
{
	cout << "Welcome to EEG-Based Biometric system!" << endl;
	RandGenMersenneTwister rg;
	//long  1412730737
	long seed = time(nullptr); //CalibrationMode
//	seed = 1;
	cout << "Seed = " << seed << endl;
	srand(seed);
	rg.setSeed(seed);

	//0 50 5 10 10

	if (argc != 6)
	{
		cout << "Parametros incorretos!" << endl;
		cout << "Os parametros esperados sao: exp fh nModels nTests timeES" << endl;
		exit(1);
	}

	int argEXP = atoi(argv[1]);
	int argvFH = atoi(argv[2]);
	int maxNM = atoi(argv[3]);
	int argvTimeES = atoi(argv[4]);
	int nVolunters = atoi(argv[5]);

	//Forcing some values TODO
	argEXP = 2;
	argvFH = 80; //Max value for fh (if not random , this is the value)
	maxNM = 10;
	argvTimeES = 10;
	nVolunters = 4;
	cout<<"Parameters [EXP,FH,maxNM,timeES,nVol]:"<<argEXP<<"\t"<<argvFH<<"\t"<<maxNM<<"\t"<<argvTimeES<<"\t"<<nVolunters<<endl;
	int argvMaxLagRate = 3; // percentage of ts to be used
	bool randomFH = true;
	bool randomChannel = true;
	bool randomQI = true; //random indicator of quality (MAPE,SMAPE
	//=============================================
	//Learning instance and validation

	double trainingSetPercentage = 0.7;
	double validationSetPercentage = 1 - trainingSetPercentage;
	trainingSetPercentage = 0.3;
	validationSetPercentage = 0.3;

	//Training experiment - 1 to 14
	int expT = 1;
	int expV = 2;
//	expT = argEXP;
//	expV = argEXP + 1;

	//Channel to be trained and channel to be validated
//	int fixedChannelT = 30;
//	int fixedChannelV = 30;

	//cout << "expT: " << expT << " -- channel: " << fixedChannelT << " with " << trainingSetPercentage * 100 << "%" << endl;
	cout << "EEG-BASED BIOMETRIC SYSTEM GOING TO RUN" << endl;
	cout << maxNM << " models from " << nVolunters << " volunteers" << " expT " << expT << " with " << trainingSetPercentage * 100 << "%" << endl;
	cout << "expV: " << expV << " with " << validationSetPercentage * 100 << "%" << endl;

	//=============================================

	//===================================
	cout << "===========================" << endl;
	cout << "Input parameters:" << endl;
	cout << "nVolunters:" << nVolunters << endl;
	cout << "number of models per volunteer:" << maxNM << endl;
	cout << "argvMaxLagRate:" << argvMaxLagRate << endl;
	cout << "argvTimeES:" << argvTimeES << endl;
	cout << "argvFH:" << argvFH << endl;
	cout << "===========================" << endl << endl;

	//vector that carries each model that were trained and its characteristics and parameters
	vector<HFMModelAndParam*> setOfHFMLearningModels;

	vector<int> listOfIndicatorOfQuality;
	// this next, listOfIndicatorOfQualityWeights, vector is used in the decision making
	// it indicates the importance of each indicator of quality in order to analyze which is the most suitable model for that time series
	vector<int> listOfIndicatorOfQualityWeights;

	listOfIndicatorOfQuality.push_back(MAPE_INDEX);
	listOfIndicatorOfQualityWeights.push_back(1);

	listOfIndicatorOfQuality.push_back(MSE_INDEX);
	listOfIndicatorOfQualityWeights.push_back(1);

	listOfIndicatorOfQuality.push_back(RMSE_INDEX);
	listOfIndicatorOfQualityWeights.push_back(1);

	listOfIndicatorOfQuality.push_back(SMAPE_INDEX);
	listOfIndicatorOfQualityWeights.push_back(1);

	listOfIndicatorOfQuality.push_back(WMAPE_INDEX);
	listOfIndicatorOfQualityWeights.push_back(1);

	//TODO
	//new indicators of quality, intrisic looking at the predictes values and comparing them to the real EEG should be developed

	int nIndicatorsOfQuality = listOfIndicatorOfQuality.size();

	vector<int> channelsToBeLearned;
	vector<int> fhToBeLearned;
	vector<int> iQToBeLearned;
	for (int c = 0; c < maxNM; c++)
	{
		channelsToBeLearned.push_back(rg.rand(64) + 1);
		fhToBeLearned.push_back(rg.rand(argvFH) + 1);
		iQToBeLearned.push_back(listOfIndicatorOfQuality[rg.rand(nIndicatorsOfQuality)]);
		//channelsToBeLearned.push_back(c+1); // this is for the case where all channels should be read
	}

	for (int v = 0; v < nVolunters; v++)
	{
		for (int nM = 0; nM < maxNM; nM++)
		{
			int variableChannelT = channelsToBeLearned[nM];
			int fH = fhToBeLearned[nM]; //forecasting horizon
			int evalFO = iQToBeLearned[nM];
			int trainingTime = argvTimeES;

			if (!randomFH)
			{
				cout << "forcing FH in the learning phase!" << endl;
				fH = argvFH;
			}

			if (!randomChannel)
			{
				cout << "forcing random channel in the learning phase!" << endl;
				//int randomChannel = rg.rand(64) + 1;
				variableChannelT = 10;
			}
			if (!randomQI)
			{
				cout << "forcing indicator of quality in the learning phase!" << endl;
				evalFO = SMAPE_INDEX;
			}

			cout << "\nTraining model " << nM + 1 << "/" << maxNM << " of Volunteer " << v + 1 << " -- EEG Channel " << variableChannelT << " -- exp" << expT << endl;

			stringstream EEGTimeSeriesToBeLearned;
			stringstream sName;
			stringstream expName;
//			EEGTimeSeriesToBeLearned << "./MyProjects/HFM/Instance/Physionet/S00" << v + 1 << "R01/Channel-1";
			if (v + 1 <= 9)
				sName << "S00" << v + 1;
			else if (v + 1 <= 99)
				sName << "S0" << v + 1;
			else
				sName << "S" << v + 1;
			if (expT <= 9)
				expName << "R0" << expT;
			else
				expName << "R" << expT;
			EEGTimeSeriesToBeLearned << "./MyProjects/HFM/Instance/Physionet/" << sName.str() << expName.str() << "/Channel-" << variableChannelT;

			vector<string> explanatoryVariables;
			explanatoryVariables.push_back(EEGTimeSeriesToBeLearned.str());

//			int trainingTime = rg.rand(argvTimeES) + minTime;

			treatForecasts tFTraining(explanatoryVariables);
			tFTraining.setTSFile(tFTraining.getPercentageFromBeginToEnd(0, 0, trainingSetPercentage), 0);

			pair<Solution<RepEFP>, Evaluation>* HFMmodel = learnModel(tFTraining, argvMaxLagRate, trainingTime, seed, rg, evalFO, fH);
//			cout << "sol->first.getR().earliestInput: " << HFMmodel->first.getR().earliestInput << endl;
//			cout<<HFMmodel->first.getR()<<endl;
//			getchar();

			vector<double> allErrors = checkLearningAbility(tFTraining, HFMmodel, rg, fH);
			vector<double> biometricSystemMeasures;
			for (int iq = 0; iq <  (int) listOfIndicatorOfQuality.size(); iq++)
				biometricSystemMeasures.push_back(allErrors[listOfIndicatorOfQuality[iq]]);
//			cout << allErrors << endl;
//			cout << biometricSystemMeasures << endl;
//			getchar();

			HFMModelAndParam* HFMCurrentModelAndParams = new HFMModelAndParam(HFMmodel, biometricSystemMeasures, fH, v, variableChannelT);
			setOfHFMLearningModels.push_back(HFMCurrentModelAndParams);
		}
	}

	cout << "The different time series has been learned with success!\n" << endl;
	cout << "Time to check their performance...\n" << endl;

	//Checking the performance of each HFM model in different EEG channels of different inviduals
	// A different experiment is verified

//	int nChannels = 1;
	int nClassificationRules = 6;
	vector<int> truePositivesPerMetric(nClassificationRules, 0);
	for (int hiddenV = 0; hiddenV < nVolunters; hiddenV++) // how is volunter v?
	{
		cout << "===================================================\n" << endl;

		Matrix<double> resultsAVG(nIndicatorsOfQuality, nVolunters);
		Matrix<double> resultsMin(nIndicatorsOfQuality, nVolunters);
		Matrix<double> resultsMax(nIndicatorsOfQuality, nVolunters);
		Matrix<double> resultsGAP(nIndicatorsOfQuality, nVolunters);
		vector<int> resultsCounter(nVolunters, 0); // can be removed, since allValidations[v].size() gives the number
		vector<vector<vector<double> > > allValidations(nVolunters);

		float bigM = 10000000000;
		for (int v = 0; v < nVolunters; v++)
			for (int m = 0; m < nIndicatorsOfQuality; m++)
			{
				resultsAVG(m, v) = 0;
				resultsGAP(m, v) = 0;
				resultsMax(m, v) = -bigM;
				resultsMin(m, v) = bigM;
			}

		for (int nM = 0; nM <  (int) setOfHFMLearningModels.size(); nM++)
		{
			pair<Solution<RepEFP>, Evaluation>* HFMmodel = setOfHFMLearningModels[nM]->HFMModel;
			vector<double> modelStandardErrors = setOfHFMLearningModels[nM]->forecastingErrors;
			int modelFH = setOfHFMLearningModels[nM]->fH;
			int variableChannelV = setOfHFMLearningModels[nM]->channel;
			int modelV = setOfHFMLearningModels[nM]->v;

			stringstream validationTSFile;
			stringstream sVName;
			stringstream expVName;
			if (hiddenV + 1 <= 9)
				sVName << "S00" << hiddenV + 1;
			else if (hiddenV + 1 <= 99)
				sVName << "S0" << hiddenV + 1;
			else
				sVName << "S" << hiddenV + 1;

			if (expV <= 9)
				expVName << "R0" << expV;
			else
				expVName << "R" << expV;
			validationTSFile << "./MyProjects/HFM/Instance/Physionet/" << sVName.str() << expVName.str() << "/Channel-" << variableChannelV;

			vector<string> validationExplanotoryVariables;
			validationExplanotoryVariables.push_back(validationTSFile.str());
//				cout << "checking performance on " << checkError.str() << endl;

			treatForecasts tFValidation(validationExplanotoryVariables);
			tFValidation.setTSFile(tFValidation.getPercentageFromEndToBegin(0, 0, validationSetPercentage), 0);
//			cout << HFMmodel->first.getR() << endl;
//			getchar();
			vector<double> allErrors = checkLearningAbility(tFValidation, HFMmodel, rg, modelFH);
			vector<double> currentErrors;

			for (int iq = 0; iq < (int) listOfIndicatorOfQuality.size(); iq++)
				currentErrors.push_back(allErrors[listOfIndicatorOfQuality[iq]]);

			for (int m = 0; m < nIndicatorsOfQuality; m++)
			{
				resultsAVG(m, modelV) += currentErrors[m];

				if (currentErrors[m] < resultsMin(m, modelV))
					resultsMin(m, modelV) = currentErrors[m];

				if (currentErrors[m] > resultsMax(m, modelV))
					resultsMax(m, modelV) = currentErrors[m];

				resultsGAP(m, modelV) += abs(modelStandardErrors[m] - currentErrors[m]);
				resultsCounter[modelV]++;
			}
			allValidations[modelV].push_back(currentErrors);

		}

		for (int v = 0; v < nVolunters; v++)
			for (int m = 0; m < nIndicatorsOfQuality; m++)
			{
				resultsAVG(m, v) = resultsAVG(m, v) / resultsCounter[v];
				resultsGAP(m, v) = resultsGAP(m, v) / resultsCounter[v];
			}

//		cout << maxNM << " models from volunteer:" << v << "-->" << hiddenV << endl;

//			cout << "Average errors" << endl;
//			//<< " applied over all  errors vollunter " << v2 + 1 << " -- All channels from exp 2" << endl;
//			cout << sumOfErrors << endl;
//
//			cout << "Min errors" << endl;
//			//<< " applied over all  errors vollunter " << v2 + 1 << " -- All channels from exp 2" << endl;
//			cout << minErrors << endl;
//
//			cout << "Max errors" << endl;
//			cout << maxErrors << endl;
//
//			cout << "GAPS against standard errors" << endl;
//			cout << sumOfGAP << endl << endl;
//			getchar();

		vector<pair<double, int> > lowerPairAVG = findBestPairsValuesByMetric(resultsAVG, true);
		vector<pair<double, int> > lowerPairMIN = findBestPairsValuesByMetric(resultsMin, true);
		vector<pair<double, int> > highestPairMAX = findBestPairsValuesByMetric(resultsMax, false);
		vector<pair<double, int> > lowestPairMAX = findBestPairsValuesByMetric(resultsMax, true);
		vector<pair<double, int> > lowerPairGAP = findBestPairsValuesByMetric(resultsGAP, true);

//		cout << "AVG\n";
//		cout << lowerPairAVG << endl;
//		cout << results << endl;
//
//		cout << "MIN\n";
//		cout << lowerPairMIN << endl;
//		cout << resultsMin << endl;
//
//		cout << "MAX\n";
//		cout << highestPairMAX << endl;
//		cout << lowestPairMAX << endl;
//		cout << resultsMax << endl;
//
//		cout << "GAP\n";
//		cout << lowerPairGAP << endl;
//		cout << resultsGAP << endl;
//		getchar();

		vector<vector<pair<int, int> > > volunteerRankPerMetric;
		volunteerRankPerMetric.push_back(classificationRuleLessMax(lowerPairAVG, lowerPairMIN, highestPairMAX, lowerPairGAP, listOfIndicatorOfQualityWeights, nVolunters));
		volunteerRankPerMetric.push_back(classificationRulePlusMax(lowerPairAVG, lowerPairMIN, lowestPairMAX, lowerPairGAP, listOfIndicatorOfQualityWeights, nVolunters));
		volunteerRankPerMetric.push_back(classificationRuleOrderLessMax(resultsAVG, resultsMin, resultsMax, resultsGAP, listOfIndicatorOfQualityWeights));
		volunteerRankPerMetric.push_back(classificationRuleOrderPlusMax(resultsAVG, resultsMin, resultsMax, resultsGAP, listOfIndicatorOfQualityWeights));
		volunteerRankPerMetric.push_back(classificationRuleNoGapSumMax(lowerPairAVG, lowerPairMIN, lowestPairMAX, lowerPairGAP, listOfIndicatorOfQualityWeights, nVolunters));
		volunteerRankPerMetric.push_back(classificationRuleNoGapLessMax(lowerPairAVG, lowerPairMIN, highestPairMAX, lowerPairGAP, listOfIndicatorOfQualityWeights, nVolunters));

		for (int vPM = 0; vPM < nClassificationRules; vPM++)
		{
			cout << "\nM" << vPM << "-- Model from volunteer: " << volunteerRankPerMetric[vPM][0].first << " is the most suitable for ts: " << hiddenV << endl;
			cout << volunteerRankPerMetric[vPM] << endl;

			//Checking true positives
			if (volunteerRankPerMetric[vPM][0].first == hiddenV)
				truePositivesPerMetric[vPM]++;
		}

		cout << "===================================================\n" << endl;

	}
	cout << "===================================================\n" << endl;
	cout << "True positives are:\n";
	for (int vPM = 0; vPM < nClassificationRules; vPM++)
	{
		cout << "M" << vPM << ":" << truePositivesPerMetric[vPM] << "\t";
	}
	cout << "\n";
	cout << "True percentage are:\n";
	for (int vPM = 0; vPM < nClassificationRules; vPM++)
	{
		double tempPercentage = 100.0 * truePositivesPerMetric[vPM] / nVolunters;
		cout << "M" << vPM << ":" << tempPercentage << "\t";
	}
	cout << "\n";
	cout << "===================================================\n" << endl;

	cout << maxNM << " models from " << nVolunters << " volunteers" << " expT " << expT << " with " << trainingSetPercentage * 100 << "%" << endl;
	cout << "expV: " << expV << " with " << validationSetPercentage * 100 << "%" << endl;

	// =================== PRINTING RESULTS ON FILE ========================
	string calibrationFile = "./EEGLearningAccuracy";

	FILE* fResults = fopen(calibrationFile.c_str(), "a");

	for (int vPM = 0; vPM < nClassificationRules; vPM++)
	{
		fprintf(fResults, "%d\t", truePositivesPerMetric[vPM]);
	}

	fprintf(fResults, "%d\t%d\t%d\t%d\t%d\t%f\t%f\t%d\t%d\t\n", nVolunters, maxNM, argvMaxLagRate, argvTimeES, argvFH, trainingSetPercentage, validationSetPercentage, expT, expV);

	fclose(fResults);
	// =======================================================

	return 0;
}

