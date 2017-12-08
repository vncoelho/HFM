#ifndef HFM_PROBLEMPARAMETERS_HPP_
#define HFM_PROBLEMPARAMETERS_HPP_

#include <iostream>

#include "../../OptFrame/Scanner++/Scanner.h"

using namespace std;
using namespace scannerpp;

namespace HFM
{

struct ForecastingOptions
{
	vector<bool> roundNegative;
	vector<bool> round;
	vector<bool> binary;
	vector<bool> forceSampleLearningWithEndogenous;

	ForecastingOptions()
	{
		roundNegative.resize(1,false);
		round.resize(1,false);
		binary.resize(1,false);
		forceSampleLearningWithEndogenous.resize(1,false);
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
	//If maxUpperLag is greater than 0 model uses predicted data: ( t - (-K) ) => (t + K) | K !=0
	vector<int> vMaxLag, vMaxUpperLag;
	string instancePath;
	string validationPath;
	int nFiles;
	ForecastingOptions fOptions;
	int targetFile;
	vector<pair<int, int> > oForesSP; //Options to Forecasting with single points
	vector<pair<int, vector<int> > > oForesMP; //Options to Forecasting with mean points
	vector<pair<int, vector<int> > > oForesDP; //Options to Forecasting with derivate points

public:
	ProblemParameters(string parametersFile)
	{
		readFile(parametersFile);
		targetFile = 0;
		vMaxLag.resize(1,0);
		vMaxUpperLag.resize(1,0);
	}

	ProblemParameters()
	{
		targetFile = 0;
		vMaxLag.resize(1,0);
		vMaxUpperLag.resize(1,0);
	}

	//the target file is always the first time series
	int getTargetFile()
	{
		return targetFile;
	}

	virtual ~ProblemParameters()
	{
	}

	int getStepsAhead()
	{
		return stepsAhead;
	}

	vector<int> getVMaxLag()
	{
		return vMaxLag;
	}

	vector<int> getVMaxUpperLag()
	{
		return vMaxUpperLag;
	}

	int getMaxLag(int _expVariable = -1)
	{
		if(_expVariable == -1)
			_expVariable = getTargetFile();
		assert((int)vMaxLag.size()>=_expVariable);
		return vMaxLag[_expVariable];
	}


	void setMaxLag(int _maxLag, int _expVariable = - 1)
	{
		if(_expVariable == -1)
			_expVariable = getTargetFile();

		assert((int)vMaxLag.size()>=_expVariable);

		vMaxLag.at(_expVariable) = _maxLag;
	}

	int getMaxUpperLag(int _expVariable)
	{
		assert((int)vMaxUpperLag.size()>=_expVariable);
		return vMaxUpperLag.at(_expVariable);
	}

	void setMaxUpperLag(int _maxLagUpperBound, int _expVariable = -1)
	{
		if(_expVariable == -1)
			_expVariable = getTargetFile();
		assert((int)vMaxUpperLag.size()>=_expVariable);
		vMaxUpperLag.at(_expVariable) = _maxLagUpperBound;
	}


	bool getRoundingToInteger(int _expVariable)
	{
		return fOptions.round[_expVariable];
	}

	bool getRoundingNegative(int _expVariable)
	{
		return fOptions.roundNegative[_expVariable];
	}

	void setBinary(bool desiredRounding,int _expVariable)
	{
		fOptions.binary[_expVariable] = desiredRounding;
	}

	bool getForceSampleLearningWithEndogenous(int _expVariable)
	{
		assert(_expVariable != getTargetFile());
		assert((int) fOptions.forceSampleLearningWithEndogenous.size()>=_expVariable);
		return fOptions.forceSampleLearningWithEndogenous[_expVariable];
	}

	bool getBinary(int _expVariable)
	{
		assert((int) fOptions.binary.size()>=_expVariable);
		return fOptions.binary[_expVariable];
	}

	void setRounding(bool desiredRounding,int _expVariable)
	{
		assert((int) fOptions.round.size()>=_expVariable);
		fOptions.round[_expVariable] = desiredRounding;
	}

	void setRoundingNegative(bool desiredRounding,int _expVariable)
	{
		assert((int) fOptions.roundNegative.size()>=_expVariable);
		fOptions.roundNegative[_expVariable] = desiredRounding;
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
	//TODO DEPRECATED
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

		vMaxLag[0] = oldestStep;
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

};

}

#endif /*HFM_PROBLEMPARAMETERS_HPP_*/

