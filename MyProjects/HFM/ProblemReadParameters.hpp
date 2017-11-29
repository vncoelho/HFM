#ifndef EFP_PROBLEMREADPARAMETERS_HPP_
#define EFP_PROBLEMREADPARAMETERS_HPP_

#include <iostream>

#include "../../OptFrame/Scanner++/Scanner.h"

using namespace std;
using namespace scannerpp;

namespace EFP
{

struct ForecastingOptions
{
	bool roundNegative;
	bool round;
	bool binary;
	bool forceSampleLearningWithEndogenous;

	ForecastingOptions(bool _roundNegative = false, bool _round = false, bool _binary = false, bool _forceSampleLearningWithEndogenous = false) :
			roundNegative(_roundNegative), round(_round), binary(_binary), forceSampleLearningWithEndogenous(_forceSampleLearningWithEndogenous)
	{

	}

	friend ostream & operator<<(ostream & os, const ForecastingOptions& fOptions)
	{

		os << "=======================================" << endl;
		os << "==========PRINTING PATH ===============" << endl;
		os << "=======================================" << endl;
		os << " allowNegative:" << fOptions.roundNegative << " ; round:" << fOptions.round << " ; binary:" << fOptions.binary << " ; forceSampleLearningWithEndogenous:" << fOptions.forceSampleLearningWithEndogenous << endl;
		os << "=======================================" << endl;

		return os;
	}

};

class ProblemParameters
{
private:
	// Your private vars
	//int options;
	int precision;
	int stepsAhead;
	int function;
	vector<int> vNotUsedForTests;
	int maxLag, maxUpperLag;
	string instancePath;
	string validationPath;
	int nFiles;
	ForecastingOptions fOptions;

	vector<pair<int, int> > oForesSP; //Options to Forecasting with single points
	vector<pair<int, vector<int> > > oForesMP; //Options to Forecasting with mean points
	vector<pair<int, vector<int> > > oForesDP; //Options to Forecasting with derivate points

public:
	ProblemParameters(string parametersFile)
	{
		readFile(parametersFile);

	}

	ProblemParameters()
	{

	}

	//the target file is always the first time series
	int getTargetFile()
	{
		return 0;
	}

	void readFile(string parametersFile)
	{
		vNotUsedForTests.clear();
		oForesSP.clear();
		oForesMP.clear();
		oForesDP.clear();
		File* file;
		try
		{
			file = new File(parametersFile);
		} catch (FileNotFound& f)
		{
			cout << "Parameters File '" << parametersFile << "' not found" << endl;
			exit(1);
		}
		Scanner scanner(file);

		cout << scanner.nextLine() << endl;
		cout << scanner.nextLine() << endl;
		precision = scanner.nextInt();
		cout << "precision = " << precision << endl;
		cout << scanner.nextLine() << endl;
		stepsAhead = scanner.nextInt();
		cout << "stepsAhead = " << stepsAhead << endl;
		cout << scanner.nextLine() << endl;
		function = scanner.nextInt();
		cout << "function = " << function << endl;

		cout << scanner.nextLine() << endl;
		cout << scanner.nextLine() << endl;
		cout << scanner.nextLine() << endl;

		int nSP = scanner.nextInt();
		cout << "nSP = " << nSP << endl;

		cout << scanner.nextLine() << endl;

		for (int i = 0; i < nSP; i++)
		{
			int file = scanner.nextInt();
			int K = scanner.nextInt();
			oForesSP.push_back(make_pair(file, K));
		}

		cout << oForesSP << endl;

		cout << scanner.nextLine() << endl;
		cout << scanner.nextLine() << endl;

		int nMP = scanner.nextInt();
		cout << "nMP = " << nMP << endl;

		cout << scanner.nextLine() << endl;

		for (int i = 0; i < nMP; i++)
		{
			int file = scanner.nextInt();
			int nAverageP = scanner.nextInt();

			vector<int> meansP;
			for (int n = 0; n < nAverageP; n++)
			{
				int K = scanner.nextInt();
				meansP.push_back(K);
			}
			oForesMP.push_back(make_pair(file, meansP));
		}

		cout << oForesMP << endl;

		cout << scanner.nextLine() << endl;
		cout << scanner.nextLine() << endl;

		int nDP = scanner.nextInt();
		cout << "nDP = " << nDP << endl;

		cout << scanner.nextLine() << endl;

		for (int i = 0; i < nDP; i++)
		{
			int file = scanner.nextInt();
			int K1 = scanner.nextInt();
			int K2 = scanner.nextInt();

			vector<int> derivativeP;
			derivativeP.push_back(K1);
			derivativeP.push_back(K2);

			oForesDP.push_back(make_pair(file, derivativeP));
		}

		cout << oForesDP << endl;

		cout << scanner.nextLine() << endl;
		cout << scanner.nextLine() << endl;

		//cout << "===== options = nSP + nMP + nDP =====" << endl;
		//options = nSP + nMP + nDP;
		//cout << "options = " << options << endl;

		cout << scanner.nextLine() << endl;
		cout << scanner.nextLine() << endl;

		//number of Single Points + Average Points + Derivative and Integral Ones

		//Checking number of files
		//Checking the minimum number of points to perform a forecast #notUsedForTest
		int oldestStep = 0;
		nFiles = 0;

		for (int i = 0; i < (int) oForesSP.size(); i++)
		{
			if (oForesSP[i].first > nFiles)
				nFiles = oForesSP[i].first;

			if (oForesSP[i].second > oldestStep)
				oldestStep = oForesSP[i].second;
		}

		for (int i = 0; i < (int) oForesMP.size(); i++)
		{
			vector<int> means = oForesMP[i].second;

			if (oForesMP[i].first > nFiles)
				nFiles = oForesMP[i].first;

			for (int m = 0; m < (int) means.size(); m++)
			{
				if (means[m] > oldestStep)
					oldestStep = means[m];
			}
		}

		for (int i = 0; i < (int) oForesDP.size(); i++)
		{
			vector<int> means = oForesDP[i].second;

			if (oForesDP[i].first > nFiles)
				nFiles = oForesDP[i].first;

			for (int m = 0; m < (int) means.size(); m++)
			{
				if (means[m] > oldestStep)
					oldestStep = means[m];
			}

		}

		maxLag = oldestStep;
		//cout << "nFiles =" << nFiles << endl;
		//cout << "notUsedForTest = " << notUsedForTest << endl;
		vNotUsedForTests.resize(nFiles);

		for (int i = 0; i < (int) oForesSP.size(); i++)
		{
			vNotUsedForTests[oForesSP[i].first - 1] = max(oForesSP[i].second, vNotUsedForTests[oForesSP[i].first - 1]);
		}

		for (int i = 0; i < (int) oForesMP.size(); i++)
		{
			vector<int> means = oForesMP[i].second;

			for (int m = 0; m < (int) means.size(); m++)
			{
				vNotUsedForTests[oForesMP[i].first - 1] = max(means[m], vNotUsedForTests[oForesMP[i].first - 1]);
			}
		}

		for (int i = 0; i < (int) oForesDP.size(); i++)
		{
			vector<int> means = oForesDP[i].second;

			for (int m = 0; m < (int) means.size(); m++)
			{
				vNotUsedForTests[oForesDP[i].first - 1] = max(means[m], vNotUsedForTests[oForesDP[i].first - 1]);
			}

		}
	}

	virtual ~ProblemParameters()
	{
	}

	int getStepsAhead()
	{
		return stepsAhead;
	}

	int getMaxLag()
	{
		return maxLag;
	}

	void setMaxLag(int _maxLag)
	{
		maxLag = _maxLag;
	}

	void setMaxUpperLag(int _maxLag)
	{
		maxUpperLag = _maxLag;
	}

	bool getRoundingToInteger()
	{
		return fOptions.round;
	}

	bool getRoundingNegative()
	{
		return fOptions.roundNegative;
	}

	bool getForceSampleLearningWithEndogenous()
	{
		return fOptions.forceSampleLearningWithEndogenous;
	}

	bool getBinary()
	{
		return fOptions.binary;
	}

	void setRounding(bool desiredRounding)
	{
		fOptions.round = desiredRounding;
	}

	void setRoundingNegative(bool desiredRounding)
	{
		fOptions.roundNegative = desiredRounding;
	}

	void setBinary(bool desiredRounding)
	{
		fOptions.binary = desiredRounding;
	}

	int getMaxUpperLag()
	{
		return maxUpperLag;
	}

	void setStepsAhead(int _stepsAhead)
	{
		stepsAhead = _stepsAhead;
	}

	void setInstancePath(string _instancePath)
	{
		instancePath = _instancePath;
	}

	void setValidationPath(string _validationPath)
	{
		validationPath = _validationPath;
	}

	string getInstancePath()
	{
		return instancePath;
	}

	string getValidationPath()
	{
		return validationPath;
	}

//	vector<pair<int, int> > getOForesSP()
//	{
//		return oForesSP;
//	}
//
//	vector<pair<int, vector<int> > > getOForesMP()
//	{
//		return oForesMP;
//	}
//
//	vector<pair<int, vector<int> > > getOForesDP()
//	{
//		return oForesDP;
//	}

};

}

#endif /*EFP_PROBLEMREADPARAMETERS_HPP_*/

