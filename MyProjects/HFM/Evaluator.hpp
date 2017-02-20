#ifndef EFP_EVALUATOR_HPP_
#define EFP_EVALUATOR_HPP_

//#include <cmath>
#include <stdlib.h>
#include <math.h>

#include "../../OptFrame/Evaluation.hpp"
#include "../../OptFrame/Evaluator.hpp"

#include "Representation.h"
#include "Solution.h"
#include "Evaluation.h"

#include "ProblemInstance.hpp"
#include "treatForecasts.hpp"

#define EPSILON_EFP 0.0001
extern int nThreads;

namespace EFP
{

const int MAPE_INDEX = 0;
const int PINBALL_INDEX = 1;
const int MSE_INDEX = 2;
const int RMSE_INDEX = 3;
const int PINBALL_ERROR_INDEX = 4;
const int SMAPE_INDEX = 5;
const int WMAPE_INDEX = 6;
const int MMAPE_INDEX = 7;
const int NMETRICS = 8;

float sigmoid(float x)
{
	float exp_value;
	float return_value;

	/*** Exponential calculation ***/
	exp_value = exp((double) -x);

	/*** Final sigmoid value ***/
	return_value = 1 / (1 + exp_value);

	return return_value;
}

static bool compara(double d1, double d2)
{
	return d1 < d2;
}

class EFPEvaluator: public Evaluator<RepEFP>
{
private:
	ProblemInstance& pEFP;
	ProblemParameters& problemParam;

	//int optionsP;
	int nForecastings;
	int stepsAhead;
	int targetFile;

	int evalFO;

	int aprox;
	int nEvalForSpeedUp;
	bool roundToZero;
	double timeInMS;
public:

	EFPEvaluator(ProblemInstance& _pEFP, ProblemParameters& _problemParam, int _evalFO, int _aprox) : // If necessary, add more parameters
			pEFP(_pEFP), problemParam(_problemParam), evalFO(_evalFO), aprox(_aprox)
	{
		roundToZero = false; // If true, forecasts are all rounded to 0! TODO
		targetFile = 0; //this is the file to be learned

		stepsAhead = problemParam.getStepsAhead();

		nForecastings = pEFP.getForecatingsSizeFile(0); //all files have the same size

		nEvalForSpeedUp = 0;
		timeInMS = 0;
		//calculating the oldest step used as input of the model

	}

	virtual ~EFPEvaluator()
	{

	}

	int getKValue(const int K, const int file, const int i, const int pa, const vector<vector<double> >& vForecastings, const vector<double>& predicteds)
	{
		double value = 0;

//		if ((K == 0) || ((i + pa - K) < 0) || ((i + pa - K) >= int(vForecastings[file].size())) )
		if ((K == 0) || ((i + pa - K) < 0))
		{
			cout << "BUG Evaluator K (function getKValue) = " << K << endl;
			cout << "BUG Evaluator K (function getKValue): (i + pa - K) = " << i + pa - K << endl;
			cout << "BUG Evaluator K (function getKValue): vForecastings[file].size() = " << vForecastings[file].size() << endl;
			getchar();
		}

		if ((pa >= K) && (file == targetFile) && (K > 0))
		{
			value = predicteds[pa - K];
		}
		else
		{
			if ((i + pa - K) < int(vForecastings[file].size()))
				value = vForecastings[file][i + pa - K];
			else
				value = 0;
		}

		return value;
	}

	void defuzzification(double ruleGreater, double greaterWeight, double ruleLower, double lowerWeight, double ruleEpsilon, FuzzyFunction fuzzyFunc, double value, double& estimation, double& greaterAccepeted, double& lowerAccepted)
	{

		if (fuzzyFunc == Heavisde)
		{
			if (value > ruleGreater)
			{
				estimation += greaterWeight;
				greaterAccepeted += 1;
			}

			if (value < ruleLower)
			{
				estimation += lowerWeight;
				lowerAccepted += 1;
			}

		}

		double epsilon = ruleEpsilon;
		//Trapezoid Function
		if (fuzzyFunc == Trapezoid)
		{
			double est = 0;
			double a = ruleGreater;
			double mu = 0;
			if (value <= (a - epsilon))
				mu = 0;
			if (value > a)
				mu = 1;
			if ((value > (a - epsilon)) && value <= a)
			{
				double K1 = 1 / epsilon;
				double K2 = 1 - a * K1;
				mu = value * K1 + K2;
			}

			est = greaterWeight * mu;
			estimation += est;

			greaterAccepeted += mu;
		}

		if (fuzzyFunc == Trapezoid)
		{
			double b = ruleLower;
			double est = 0;
			double mu = 0;
			if (value >= (b + epsilon))
				mu = 0;
			if (value < b)
				mu = 1;
			if (value >= b && value < b + epsilon)
			{
				double K1 = 1 / epsilon;
				double K2 = 1 - b * K1;
				mu = value * K1 + K2;
			}
			est = lowerWeight * mu;
			estimation += est;

			lowerAccepted += mu;
		}

	}

	void approximationsEnayatifar(const int& aprox, const double& alpha, const vector<double>& vAlpha, const vector<double>& vIndexAlphas, vector<int>& vIndex, double& estimation, const int i, const int pa, const vector<vector<double> >& vForecastings, const vector<double>& predicteds, const int& maxLag)
	{
		if (aprox == 1)
		{
			double ajusts = 0;
			vector<int> adjustPrevValue;
			adjustPrevValue.push_back(1);
			adjustPrevValue.push_back(2);
			adjustPrevValue.push_back(24);
			adjustPrevValue.push_back(25);
			adjustPrevValue.push_back(167);
			adjustPrevValue.push_back(168);
			adjustPrevValue.push_back(169);

			for (int ind = 0; ind < int(adjustPrevValue.size()); ind++)
			{
				int indexK = adjustPrevValue[ind];
				double valueIndexK = 0;
				if (indexK <= i)
				{
					if (pa >= indexK)
						valueIndexK += predicteds[pa - indexK];
					else
						valueIndexK += vForecastings[targetFile][i + pa - indexK];

					ajusts += alpha * (estimation - valueIndexK);
				}
			}
			estimation = estimation + ajusts;
		}

		if (aprox == 2)
		{
			double ajusts = 0;
			vector<int> adjustPrevValue;
			adjustPrevValue.push_back(1);
			adjustPrevValue.push_back(2);
			adjustPrevValue.push_back(3);
			adjustPrevValue.push_back(23);
			adjustPrevValue.push_back(24);
			adjustPrevValue.push_back(25);
			for (int ind = 0; ind < int(adjustPrevValue.size()); ind++)
			{
				int indexK = adjustPrevValue[ind];
				double valueIndexK = 0;
				if (indexK <= (i + pa))
				{
					if (pa >= indexK)
						valueIndexK += predicteds[pa - indexK];
					else
					{
						valueIndexK += vForecastings[targetFile][i + pa - indexK];
					}
					ajusts += vAlpha[ind] * (estimation - valueIndexK);
				}

			}

			estimation = estimation + ajusts;

		}

		if (aprox == 3)
		{
			double ajusts = 0;

			for (int ind = 0; ind < int(vIndex.size()); ind++)
			{
				int indexK = vIndex[ind];
				double valueIndexK = 0;
				if (indexK <= i)
				{
					if (pa >= indexK)
						valueIndexK += predicteds[pa - indexK];
					else
						valueIndexK += vForecastings[targetFile][i + pa - indexK];

					ajusts += vIndexAlphas[ind] * (estimation - valueIndexK);
					if ((vIndex[ind] <= 0) || (vIndex[ind] > maxLag))
					{
						cout << "vIndex[ind]:" << vIndex[ind] << "maxLag:" << maxLag << endl;
						cout << "Erro on INDEX of the adjustment";
						getchar();
					}
				}

			}

			estimation = estimation + ajusts;
		}

		if (aprox == 4)
		{
			double ajusts = 0;
			vector<int> adjustPrevValue;
			adjustPrevValue.push_back(10);
			adjustPrevValue.push_back(50);
			adjustPrevValue.push_back(15);
			adjustPrevValue.push_back(25);
			adjustPrevValue.push_back(38);
			adjustPrevValue.push_back(100);

			for (int ind = 0; ind < int(adjustPrevValue.size()); ind++)
			{

				int indexK = adjustPrevValue[ind];
				double valueIndexK = 0;
				if (indexK <= (i + pa))
				{
					if (pa >= indexK)
						valueIndexK += predicteds[pa - indexK];
					else
						valueIndexK += vForecastings[targetFile][i + pa - indexK];

					ajusts += vAlpha[ind] * (estimation - valueIndexK);
				}
			}

			estimation = estimation + ajusts;

		}

		if (aprox == 5)
		{
			double ajusts = 0;
			vector<int> adjustPrevValue;
			adjustPrevValue.push_back(1);
			adjustPrevValue.push_back(24);
			adjustPrevValue.push_back(48);
			adjustPrevValue.push_back(168);
			adjustPrevValue.push_back(168 * 2);
			adjustPrevValue.push_back(168 * 3);
			for (int ind = 0; ind < int(adjustPrevValue.size()); ind++)
			{
				int indexK = adjustPrevValue[ind];
				double valueIndexK = 0;
				if (indexK <= (i + pa))
				{
					if (pa >= indexK)
						valueIndexK += predicteds[pa - indexK];
					else
					{
						valueIndexK += vForecastings[targetFile][i + pa - indexK];
					}
					ajusts += vAlpha[ind] * (estimation - valueIndexK);
				}

			}

			estimation = estimation + ajusts;

		}
	}

	vector<double> returnForecasts(const RepEFP& rep, const vector<vector<double> >& vForecastings, int begin)
	{
		int sizeSP = rep.singleIndex.size();
		int sizeMP = rep.averageIndex.size();
		int sizeDP = rep.derivativeIndex.size();

		int maxLag = problemParam.getMaxLag();

		vector<double> predicteds;

		for (int pa = 0; pa < stepsAhead; pa++)			// passos a frente
		//for (int pa = 0; ((pa < stepsAhead) && (pa + begin < nForTargetFile)); pa++) //auxiliar loop for steps ahead
		{
			//vector<double> fuzzyWeights;
			//forecasting estimation
			double estimation = 0;
			double greaterAccepeted = 0;
			double lowerAccepted = 0;

			for (int nSP = 0; nSP < sizeSP; nSP++)
			{
				int file = rep.singleIndex[nSP].first;
				int K = rep.singleIndex[nSP].second;

				double singleValue = getKValue(K, file, begin, pa, vForecastings, predicteds);

				double ruleGreater = rep.singleFuzzyRS[nSP][GREATER];
				double greaterWeight = rep.singleFuzzyRS[nSP][GREATER_WEIGHT];
				double ruleLower = rep.singleFuzzyRS[nSP][LOWER];
				double lowerWeight = rep.singleFuzzyRS[nSP][LOWER_WEIGHT];
				double ruleEpsilon = rep.singleFuzzyRS[nSP][EPSILON];
				FuzzyFunction repFuzzyPertinenceFunc = FuzzyFunction(rep.singleFuzzyRS[nSP][PERTINENCEFUNC]);

				defuzzification(ruleGreater, greaterWeight, ruleLower, lowerWeight, ruleEpsilon, repFuzzyPertinenceFunc, singleValue, estimation, greaterAccepeted, lowerAccepted);

				//fuzzyWeights.push_back(value);
			}

			for (int nMP = 0; nMP < sizeMP; nMP++)
			{
				vector<pair<int, int> > meansK = rep.averageIndex[nMP];
				int nAveragePoints = meansK.size();

				double mean = 0;
				for (int mK = 0; mK < nAveragePoints; mK++)
				{
					int file = meansK[mK].first;
					int K = meansK[mK].second;

					mean += getKValue(K, file, begin, pa, vForecastings, predicteds);
				}

				mean = mean / nAveragePoints;

				double ruleGreater = rep.averageFuzzyRS[nMP][GREATER];
				double greaterWeight = rep.averageFuzzyRS[nMP][GREATER_WEIGHT];
				double ruleLower = rep.averageFuzzyRS[nMP][LOWER];
				double lowerWeight = rep.averageFuzzyRS[nMP][LOWER_WEIGHT];
				double ruleEpsilon = rep.averageFuzzyRS[nMP][EPSILON];
				FuzzyFunction repFuzzyPertinenceFunc = FuzzyFunction(rep.averageFuzzyRS[nMP][PERTINENCEFUNC]);

				defuzzification(ruleGreater, greaterWeight, ruleLower, lowerWeight, ruleEpsilon, repFuzzyPertinenceFunc, mean, estimation, greaterAccepeted, lowerAccepted);

				//fuzzyWeights.push_back(mean);
			}

			for (int nDP = 0; nDP < sizeDP; nDP++)
			{
				vector<pair<int, int> > derivateK = rep.derivativeIndex[nDP];

				double d = 0;
				for (int dK = 0; dK < derivateK.size(); dK++)
				{
					int file = derivateK[dK].first;
					int K = derivateK[dK].second;

					double value = getKValue(K, file, begin, pa, vForecastings, predicteds);

					if (dK == 0)
						d += value;
					else
						d -= value;
				}

				//fuzzyWeights.push_back(d);

				double ruleGreater = rep.derivativeFuzzyRS[nDP][GREATER];
				double greaterWeight = rep.derivativeFuzzyRS[nDP][GREATER_WEIGHT];
				double ruleLower = rep.derivativeFuzzyRS[nDP][LOWER];
				double lowerWeight = rep.derivativeFuzzyRS[nDP][LOWER_WEIGHT];
				double ruleEpsilon = rep.derivativeFuzzyRS[nDP][EPSILON];
				FuzzyFunction repFuzzyPertinenceFunc = FuzzyFunction(rep.derivativeFuzzyRS[nDP][PERTINENCEFUNC]);

				defuzzification(ruleGreater, greaterWeight, ruleLower, lowerWeight, ruleEpsilon, repFuzzyPertinenceFunc, d, estimation, greaterAccepeted, lowerAccepted);
			}

			//cout << "EstimationAntes:" << estimation << endl;
			double accepted = greaterAccepeted + lowerAccepted;
			if (accepted > 0)
				estimation /= accepted;

			if (aprox != 0)
			{
				double alpha = rep.alpha;
				vector<double> vAlpha = rep.vAlpha;
				vector<double> vIndexAlphas = rep.vIndexAlphas;
				vector<int> vIndex = rep.vIndex;

				approximationsEnayatifar(aprox, alpha, vAlpha, vIndexAlphas, vIndex, estimation, begin, pa, vForecastings, predicteds, maxLag);
			}

			//				Remove this for other forecast problem -- rain forecast
			if (roundToZero == true)
			{
				if (estimation < 0)
					estimation = 0;
			}

			predicteds.push_back(estimation);

		} //End of current iteration ... steps head

		return predicteds;
	}

	vector<double> returnTrainingSetForecasts(const RepEFP& rep, const vector<vector<double> >& vForecastings)
	{
		int nForTargetFile = vForecastings[targetFile].size();
		int maxLag = problemParam.getMaxLag();
		int nSamples = nForTargetFile - maxLag;

		vector<double> allForecasts;

		double effectNumberSamples = (nForTargetFile - maxLag);
		vector<vector<double> > allForecastsVectors(ceil(effectNumberSamples / stepsAhead));

		Timer t;

		//for (int begin = maxLag; begin < nForTargetFile; begin ++) // main loop that varries all the time series
		int beginParallel, fParallel;
//		omp_set_num_threads(4);
		//			int np = omp_get_num_threads();
		//					cout << "number of threads: " << np << endl;
		//							getchar();

////		#pragma omp parallel for ordered
//		for (beginParallel = maxLag; beginParallel < nForTargetFile; beginParallel += stepsAhead) // main loop that varries all the time series
//		{
////			omp_set_dynamic(0);     // Explicitly disable dynamic teams
////			omp_set_num_threads(4); // Use 4 threads for all consecutive parallel regions
//
//			vector<double> predicteds = returnForecasts(rep, vForecastings, beginParallel);
//
////			#pragma omp ordered
//			allForecasts.insert(allForecasts.end(), predicteds.begin(), predicteds.end());
//
//
//
////			for (fParallel = 0; fParallel < predicteds.size(); fParallel++)
////			{
////				allForecasts.push_back(predicteds[fParallel]);
////			}
//		}

//		cout<<"nThreads:"<<nThreads<<endl;
//		getchar();

		//#pragma omp parallel for num_threads(nThreads)
		for (beginParallel = maxLag; beginParallel < nForTargetFile; beginParallel += stepsAhead) // main loop that varries all the time series
		{
			//			omp_set_dynamic(0);     // Explicitly disable dynamic teams
			//			omp_set_num_threads(4); // Use 4 threads for all consecutive parallel regions

			//vector<double> predicteds = returnForecasts(rep, vForecastings, beginParallel);

			//			#pragma omp ordered

			int index = (beginParallel - maxLag) / stepsAhead;

			allForecastsVectors[index] = std::move(returnForecasts(rep, vForecastings, beginParallel));			//predicteds;
//			allForecasts[beginParallel-maxLag]
//			allForecasts.insert(allForecasts.end(), predicteds.begin(), predicteds.end());

			//			for (fParallel = 0; fParallel < predicteds.size(); fParallel++)
			//			{
			//				allForecasts.push_back(predicteds[fParallel]);
			//			}
		}

		timeInMS += t.inMilliSecs();

		for (int aV = 0; aV < allForecastsVectors.size(); aV++)
			for (int k = 0; k < stepsAhead; k++)
				allForecasts.push_back(allForecastsVectors[aV][k]);


		//TODO - Parallel Test and report values
		/*
		nEvalForSpeedUp++;
		if (nEvalForSpeedUp == 1000)
		{
			cout << "Total time: " << timeInMS / 1000 << endl;

			string speedUpFile = "./apen_SI_speedUpFile";
			FILE* fResults = fopen(speedUpFile.c_str(), "a");
			fprintf(fResults, "%.3f\n", timeInMS / 1000);
			fclose(fResults);

			exit(1);
		}
		*/

		//TODO do it in a better style
		if (allForecasts.size() > nSamples)
		{
			int nExtraForecasts = allForecasts.size() - nSamples;
			allForecasts.erase(allForecasts.begin() + allForecasts.size() - nExtraForecasts, allForecasts.end());
		}

		return allForecasts;
	}

	vector<double> blindForecasting(const RepEFP& rep, const vector<vector<double> >& vForecastings)
	{
		vector<double> predicteds = returnForecasts(rep, vForecastings, vForecastings[targetFile].size());
		return predicteds;
	}

	//TODO -- Check if the above blindForecasting provides the same results as this one down here
//	vector<double> blindForecasting(const RepEFP& rep, const vector<vector<double> > vForecastings)
//	{
//
//		//cout<<"availableForecasting = "<<availableForecasting<<endl;
//
//		int sizeSP = rep.singleIndex.size();
//		int sizeMP = rep.averageIndex.size();
//		int sizeDP = rep.derivativeIndex.size();
//
//		int nForecastingValidations = vForecastings[0].size();
//
//		int i = problemParam.getMaxLag();
//		int maxLag = problemParam.getMaxLag();
//		vector<double> predicteds;
//
//		for (int pa = 0; pa < stepsAhead; pa++)			// passos a frente
//		{
//			//forecasting estimation
//			double estimation = 0;
//			double greaterAccepeted = 0;
//			double lowerAccepted = 0;
//
//			for (int nSP = 0; nSP < sizeSP; nSP++)
//			{
//				int file = rep.singleIndex[nSP].first;
//				int K = rep.singleIndex[nSP].second;
//
//				double value = 0;
//
//				if (file == 0)
//				{
//					if (pa >= K)
//						value = predicteds[pa - K];
//					else
//						value = vForecastings[file][i + pa - K];
//
//				}
//				else
//				{
//					if ((i + pa - K) > vForecastings[file].size())
//					{
//						//cout << nForecastings << endl;
//						//cout << pEFP.getForecastings(file, i + pa - K) << endl;
//						//cout << i + pa - K << "\t" << i << "\t" << pa << "\t" << K << endl;
//						//getchar();
//						value = 0;
//					}
//					else
//						value = vForecastings[file][i + pa - K];
//				}
//
//				double ruleGreater = rep.singleFuzzyRS[nSP][GREATER];
//				double greaterWeight = rep.singleFuzzyRS[nSP][GREATER_WEIGHT];
//				double ruleLower = rep.singleFuzzyRS[nSP][LOWER];
//				double lowerWeight = rep.singleFuzzyRS[nSP][LOWER_WEIGHT];
//				double ruleEpsilon = rep.singleFuzzyRS[nSP][EPSILON];
//				FuzzyFunction repFuzzyPertinenceFunc = FuzzyFunction(rep.singleFuzzyRS[nSP][PERTINENCEFUNC]);
//
//				defuzzification(ruleGreater, greaterWeight, ruleLower, lowerWeight, ruleEpsilon, repFuzzyPertinenceFunc, value, estimation, greaterAccepeted, lowerAccepted);
//
//			}
//
//			for (int nMP = 0; nMP < sizeMP; nMP++)
//			{
//				vector<pair<int, int> > meansK = rep.averageIndex[nMP];
//				//cout << meansK << endl;
//				//getchar();
//				double mean = 0;
//				for (int mK = 0; mK < meansK.size(); mK++)
//				{
//					int file = meansK[mK].first;
//					int K = meansK[mK].second;
//
//					//cout << "K = " << K << endl;
//					if (file == 0)
//					{
//						if (pa >= K)
//						{
//							mean += predicteds[pa - K];
//
//						}
//						else
//						{
//							mean += vForecastings[file][i + pa - K];
//
//						}
//					}
//					else
//					{
//						mean += vForecastings[file][i + pa - K];
//					}
//				}
//
//				mean = mean / meansK.size();
//				//cout << mean << endl;
//
//				double ruleGreater = rep.averageFuzzyRS[nMP][GREATER];
//				double greaterWeight = rep.averageFuzzyRS[nMP][GREATER_WEIGHT];
//				double ruleLower = rep.averageFuzzyRS[nMP][LOWER];
//				double lowerWeight = rep.averageFuzzyRS[nMP][LOWER_WEIGHT];
//				double ruleEpsilon = rep.averageFuzzyRS[nMP][EPSILON];
//				FuzzyFunction repFuzzyPertinenceFunc = FuzzyFunction(rep.averageFuzzyRS[nMP][PERTINENCEFUNC]);
//
//				defuzzification(ruleGreater, greaterWeight, ruleLower, lowerWeight, ruleEpsilon, repFuzzyPertinenceFunc, mean, estimation, greaterAccepeted, lowerAccepted);
//
//			}
//
//			for (int nDP = 0; nDP < sizeDP; nDP++)
//			{
//				vector<pair<int, int> > derivateK = rep.derivativeIndex[nDP];
//				//cout << derivateK << endl;
//				//getchar();
//
//				double d = 0;
//				for (int dK = 0; dK < derivateK.size(); dK++)
//				{
//					int file = derivateK[dK].first;
//					int K = derivateK[dK].second;
//					//cout << "K = " << K << endl;
//
//					if (file == 0)
//					{
//						if (dK == 0)
//						{
//							if (pa >= K)
//								d += predicteds[pa - K];
//							else
//								d += vForecastings[file][i + pa - K];
//						}
//						else
//						{
//							if (pa >= K)
//								d -= predicteds[pa - K];
//							else
//								d -= vForecastings[file][i + pa - K];
//						}
//					}
//					else
//					{
//						if (dK == 0)
//						{
//							d += vForecastings[file][i + pa - K];
//						}
//						else
//						{
//							d -= vForecastings[file][i + pa - K];
//						}
//					}
//				}
//
//				double ruleGreater = rep.derivativeFuzzyRS[nDP][GREATER];
//				double greaterWeight = rep.derivativeFuzzyRS[nDP][GREATER_WEIGHT];
//				double ruleLower = rep.derivativeFuzzyRS[nDP][LOWER];
//				double lowerWeight = rep.derivativeFuzzyRS[nDP][LOWER_WEIGHT];
//				double ruleEpsilon = rep.derivativeFuzzyRS[nDP][EPSILON];
//				FuzzyFunction repFuzzyPertinenceFunc = FuzzyFunction(rep.derivativeFuzzyRS[nDP][PERTINENCEFUNC]);
//
//				defuzzification(ruleGreater, greaterWeight, ruleLower, lowerWeight, ruleEpsilon, repFuzzyPertinenceFunc, d, estimation, greaterAccepeted, lowerAccepted);
//
//			}
//
//			double accepted = greaterAccepeted + lowerAccepted;
//			if (accepted > 0)
//				estimation /= accepted;
//
//			if (aprox != 0)
//			{
//				double alpha = rep.alpha;
//				vector<double> vAlpha = rep.vAlpha;
//				vector<double> vIndexAlphas = rep.vIndexAlphas;
//				vector<int> vIndex = rep.vIndex;
//
//				approximationsEnayatifar(aprox, alpha, vAlpha, vIndexAlphas, vIndex, estimation, i, pa, vForecastings, predicteds, maxLag);
//			}
//
//			predicteds.push_back(estimation);
//
//		}
//
//		cout << "Returning blind forecasts..." << endl;
//
//		return predicteds;
//	}

	EvaluationEFP& evaluate(const RepEFP& rep)
	{
		//Fo vector with different metrics calculations
		vector<double> foIndicator(NMETRICS + 1, 0); //nMetrics + ErrorPinball

		/*
		 vector<vector<double> > foIndicatorStepsAhead(NMETRICS + 1);
		 for (int i = 0; i < foIndicatorStepsAhead.size(); i++)
		 foIndicatorStepsAhead[i].resize(stepsAhead);*/

		vector<vector<double> >& vForecastings = pEFP.getForecastingsVector();

		//validation mode = false | returnforecasts = false
		vector<double> predictedValues = returnTrainingSetForecasts(rep, vForecastings);

		int maxLag = problemParam.getMaxLag();
		vector<double> targetValues;
		for (int i = maxLag; i < vForecastings[targetFile].size(); i++)
			targetValues.push_back(vForecastings[targetFile][i]);

		foIndicator = getAccuracy(targetValues, predictedValues, false);

		//foIndicator = returnForecastingsFO(rep, vForecastings, false, false);

		double fo = foIndicator[evalFO];		// Evaluation Function Value

		return *new EvaluationEFP(fo);
	}

	const vector<double> getAccuracy(const vector<double>& targetValues, const vector<double>& estimatedValues, bool validationMode)
	{

		int nSamples = targetValues.size();
		if (nSamples != estimatedValues.size())
		{
			cout << "ERROR on GetAccuracy! Different sizes!" << endl;
			cout << "targetValues.size() = " << targetValues.size() << "\t";
			cout << "estimatedValues.size() = " << estimatedValues.size() << endl;
		}
		vector<double> foIndicator(NMETRICS, 0);
		vector<pair<double, double> > vPinballFunctions(20, make_pair(0, 0));

		double sumTarget = 0;
		double avgTarget = 0;
		if (evalFO == WMAPE_INDEX || evalFO == MMAPE_INDEX || validationMode == true)
		{
			for (int i = 0; i < nSamples; i++)
				sumTarget += targetValues[i];

			avgTarget = sumTarget / nSamples;
		}

		for (int i = 0; i < nSamples; i++)
		{
			double estimation = estimatedValues[i];
			double forecastingTarget = targetValues[i];
			double forecastingTargetNotNull = forecastingTarget;
			if (forecastingTargetNotNull == 0)
				forecastingTargetNotNull = 0.0001;

			double absDiff = abs(estimation - forecastingTarget);

			if (evalFO == MAPE_INDEX || validationMode == true)
				foIndicator[MAPE_INDEX] += (absDiff / abs(forecastingTargetNotNull));

			if (evalFO == SMAPE_INDEX || validationMode == true)
				foIndicator[SMAPE_INDEX] += (absDiff / (abs(forecastingTargetNotNull) + abs(estimation)) / 2);

			if (evalFO == WMAPE_INDEX || validationMode == true)
				foIndicator[WMAPE_INDEX] += (absDiff / sumTarget);

			if (evalFO == MMAPE_INDEX || validationMode == true)
				foIndicator[MMAPE_INDEX] += (absDiff / avgTarget);

			if (evalFO == MSE_INDEX || evalFO == RMSE_INDEX || validationMode == true)
				foIndicator[MSE_INDEX] += pow(absDiff, 2);

			//PinballFunction
			if (evalFO == PINBALL_INDEX || validationMode == true)
			{
				double quantilError = 0.5;
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
				foIndicator[PINBALL_INDEX] += pinballFunctionQuantils;
			}

		}

		//MAPE FINAL CALC
		if (evalFO == MAPE_INDEX || validationMode == true)
		{
			foIndicator[MAPE_INDEX] *= 100;
			foIndicator[MAPE_INDEX] /= nSamples;
		}

		if (evalFO == SMAPE_INDEX || validationMode == true)
		{
			foIndicator[SMAPE_INDEX] *= 100;
			foIndicator[SMAPE_INDEX] /= nSamples;
		}

		if (evalFO == WMAPE_INDEX || validationMode == true)
		{
			foIndicator[WMAPE_INDEX] *= 100;
			foIndicator[WMAPE_INDEX] /= nSamples;
		}

		if (evalFO == MMAPE_INDEX || validationMode == true)
		{
			foIndicator[MMAPE_INDEX] *= 100;
			foIndicator[MMAPE_INDEX] /= nSamples;
		}

		if (evalFO == PINBALL_INDEX || validationMode == true)
		{
			foIndicator[PINBALL_INDEX] /= nSamples;
		}

		if (evalFO == MSE_INDEX || evalFO == RMSE_INDEX || validationMode == true)
		{
			foIndicator[MSE_INDEX] /= nSamples;

			if (evalFO == RMSE_INDEX || validationMode == true)
				foIndicator[RMSE_INDEX] = sqrt(foIndicator[MSE_INDEX]);
		}

		// ===================  MAPE FOR EACH STEP AHEAD ========================
		/*for (int mp = 0; mp < stepsAhead; mp++)
		 {
		 foIndicatorStepsAhead[MAPE_INDEX][mp] = foIndicatorStepsAhead[0][mp] * 100;

		 foIndicatorStepsAhead[MAPE_INDEX][mp] = foIndicatorStepsAhead[0][mp] / ((nForecastingValidations - notUsedForTest) / stepsAhead);
		 }
		 */
		// ===================  MAPE FOR EACH STEP AHEAD ========================
		//cout << foIndicator[MAPE_INDEX] << endl;
		//MSE AND RSME FINAL CALC
		//PINBALL FINAL CALC
		/*
		 double minPinball = 1000000;
		 double pinbalError;
		 for (int pF = 0; pF < 20; pF++)
		 {
		 vPinballFunctions[pF].first /= (nForTargetFile - earliestInput);
		 if (vPinballFunctions[pF].first < minPinball)
		 {
		 minPinball = vPinballFunctions[pF].first;
		 pinbalError = vPinballFunctions[pF].second;
		 }
		 }

		 foIndicator[PINBALL_INDEX] = minPinball;
		 foIndicator[PINBALL_ERROR_INDEX] = pinbalError;*/

		//cout << "Fo Indicator:";
		//cout << foIndicator << endl;
		//cout << "steps Ahead:";
		//cout << foIndicatorStepsAhead << endl;
		return foIndicator;
	}
	virtual bool betterThan(double a, double b)
	{
		// MINIMIZATION
		return (a < (b - EPSILON_EFP));
	}

	virtual bool isMinimization() const
	{
		return true;
	}

	static string idComponent()
	{
		return "OptFrame:Evaluator:EFPEvaluator";
	}

	virtual string id() const
	{
		return idComponent();
	}

};

}

#endif /*EFP_EVALUATOR_HPP_*/

