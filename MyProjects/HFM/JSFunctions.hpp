/*
 * EFPESContinous.h
 *
 *  Created on: 01/09/2014
 *      Author: vitor
 */

#ifndef HFM_JS_FUNCTIONS_HPP_
#define HFM_JS_FUNCTIONS_HPP_

#include <vector>
#include <iostream>

using namespace std;

namespace HFM
{

vector<vector<double>> ts;

extern "C"
{


void loadTS(char* argv, int size, int var)
{
	cout << "Function called..." << endl;
	cout << argv << endl;
	char* sample = argv;
	cout << "sample is:" << sample << endl;
	string sampleString = sample;
	double sampleValue = stod(sampleString);
	if (var < ts.size())
	{
		ts[var].push_back(sampleValue);
	}
	else
	{
		int tsToAdd = var - ts.size() + 1;
		for (int i = 0; i < tsToAdd; i++)
		{
			vector<double> newTS;
			ts.push_back(newTS);
		}

		ts[var].push_back(sampleValue);
	}

	cout << "sample string is:" << sampleString << endl;
	cout << "vector is:" << ts << endl;

	return;

//	vector<vector<string>> TS;
//	for (int i = 0; i < size; i++)
//	{
//		vector<string> sample;
//		for (int v = 0; v < nVar; v++)
//		{
//			sample.push_back(string(argv[i][v]));
//		}
//
//		TS.push_back(sample);
//	}
//
//	cout << TS << endl;
//	return;
}

const char* callTSForecasting(int argc)
{
	char **argv;

	string result = onlineHFM(argc, argv, &ts);

	const char* charToReturn = result.c_str();
//	if(result == 0)
//		cout<<"Finished with sucess!"<<endl;

	cout<<"printing string:"<<endl;
	cout << result <<endl;
//	cout << charToReturn <<endl;
	cout<<"bye bye"<<endl;
	return charToReturn;
}

int getNTimeSeries()
{
	return ts.size();
}

int getTimeSeriesSize(int var)
{
	return ts[var].size();
}


}

}
#endif /* HFM_JS_FUNCTIONS_HPP_ */
