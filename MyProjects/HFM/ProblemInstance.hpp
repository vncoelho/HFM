#ifndef EFP_PROBLEMINSTANCE_HPP_
#define EFP_PROBLEMINSTANCE_HPP_

#include <iostream>

#include "../../OptFrame/Scanner++/Scanner.h"

using namespace std;
using namespace scannerpp;

namespace EFP
{

class ProblemInstance
{
private:
	// Your private vars
	vector<double> means;
	vector<double> stdDesvs;
	//int precision;
	//int options;
	//int function;
	int stepsHead;
	ProblemParameters problemParam;
	vector<vector<double> > forecastings;

public:
	ProblemInstance(vector<vector<double> > _forecastings, ProblemParameters& _problemParam) :
		forecastings(_forecastings),problemParam(_problemParam)
	{
		//precision = problemParam.getPrecision();
		//options = problemParam.getOptions();
		//function = problemParam.getFunction();
		stepsHead = problemParam.getStepsAhead();

		// Put here your code
		// You can read the input data from the 'scanner' object

		//number of different explanatory variables
		int nExVar = forecastings.size();

		vector<int> nForecastings;

		nForecastings.resize(nExVar);
		means.resize(nExVar);
		stdDesvs.resize(nExVar);

		//vector with the number of forecasting in each file

		for (int exVar = 0; exVar < nExVar; exVar++)
		{
			//Scanner scanner = *scannerFiles[exVar];
			nForecastings[exVar] = forecastings[exVar].size();
			//cout << "nForecastings[" << exVar << "]: " << nForecastings[exVar] << endl;

			/*for (int i = 0; i < nForecastings[exVar]; i++)
			{
				cout<< forecastings[exVar][i]<<endl;
			}*/

			means[exVar] = 0;
			stdDesvs[exVar] = 0;
		}

		//getchar();
		//Average Calc
		for (int exVar = 0; exVar < nExVar; exVar++)
		{
			for (int i = 0; i < nForecastings[exVar]; i++)
			{
				means[exVar] += getForecastings(exVar, i);

			}

			means[exVar] = means[exVar] / nForecastings[exVar];

			//cout << "Media = " << mean << endl;

			for (int i = 0; i < nForecastings[exVar]; i++)
			{
				double desv = getForecastings(exVar, i) - means[exVar];
				stdDesvs[exVar] += desv * desv;
			}

			stdDesvs[exVar] = stdDesvs[exVar] / nForecastings[exVar];
			stdDesvs[exVar] = sqrt(stdDesvs[exVar]);
		}

		//cout <<"File means:"<< means << endl;
		//cout <<"File std:"<< stdDesvs << endl;
		//getchar();

		// ==================== Check if #stepsAhead are possible to be done

		/*
		 stringstream ss;
		 ss << problemParam.getValidationPath();
		 File* file;

		 try
		 {
		 file = new File(ss.str());
		 } catch (FileNotFound& f)
		 {
		 cout << "File '" << ss.str() << "' not found" << endl;
		 }

		 Scanner scannerBlind(file);

		 int nForecastingsBlind = scannerBlind.nextInt();
		 cout << "nBlind: " << nForecastingsBlind << endl;
		 */

		//================================================
	}

	double getMean(int file)
	{
		return means[file];
	}

	vector<double> getMeans()
	{
		return means;
	}

	vector<double> getStdDesvs()
	{
		return stdDesvs;
	}

	double getStdDesv(int file)
	{
		return stdDesvs[file];
	}

	int getForecatingsSizeFile(int file)
	{
		return forecastings[file].size();
	}

	int getNumberExplanatoryVariables()
	{
		return forecastings.size();
	}

	inline vector<vector<double> > getForecastingsVector()
	{
		return forecastings;
	}

	inline double getForecastings(int file, int i)
	{
		return forecastings[file][i];
	}

	virtual ~ProblemInstance()
	{
	}

};

}

#endif /*EFP_PROBLEMINSTANCE_HPP_*/

