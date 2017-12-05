#ifndef EFP_PROBLEMINSTANCE_HPP_
#define EFP_PROBLEMINSTANCE_HPP_

#include <iostream>

#include "../../OptFrame/Scanner++/Scanner.h"

using namespace std;
using namespace scannerpp;

namespace HFM
{

class ProblemInstance
{
private:
	// Your private vars
	vector<double> means;
	vector<double> vMax;
	vector<double> stdDesvs;
	//int precision;
	//int options;
	//int function;
	int stepsHead;
	vector<vector<double> > forecastings;
	ProblemParameters problemParam;

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

		vector<int> tsNumberOfSamples(nExVar);
		means.resize(nExVar,0);
		stdDesvs.resize(nExVar,0);
		vMax.resize(nExVar,0);
		//vector with the number of forecasting in each file

		for (int exVar = 0; exVar < nExVar; exVar++)
			tsNumberOfSamples[exVar] = forecastings[exVar].size();


		//getchar();
		//Average Calc
		for (int exVar = 0; exVar < nExVar; exVar++)
		{
			for (int i = 0; i < tsNumberOfSamples[exVar]; i++)
			{
				means[exVar] += getForecastings(exVar, i);
				if(getForecastings(exVar, i)>vMax[exVar])
					vMax[exVar] = getForecastings(exVar, i);
			}

			means[exVar] = means[exVar] / tsNumberOfSamples[exVar];

			//cout << "Media = " << mean << endl;

			for (int i = 0; i < tsNumberOfSamples[exVar]; i++)
			{
				double desv = getForecastings(exVar, i) - means[exVar];
				stdDesvs[exVar] += desv * desv;
			}

			stdDesvs[exVar] = stdDesvs[exVar] / tsNumberOfSamples[exVar];
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

	double getMax(int file)
	{
		return vMax[file];
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

	inline vector<vector<double> >& getForecastingsVector()
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

