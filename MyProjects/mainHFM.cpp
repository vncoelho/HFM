// ===================================
// Main.cpp file generated by OptFrame
// Project EFP
// ===================================

#include <stdlib.h>
#include <math.h>
#include <iostream>

#include "../OptFrame/Loader.hpp"
#include "HFM.h"
#include "./HFM/mainForecastingCodes/priceMain.cpp"
#include "./HFM/mainForecastingCodes/loadMain.cpp"
#include "./HFM/mainForecastingCodes/microGrid.cpp"
#include "./HFM/mainForecastingCodes/microGridOnline.cpp"
#include "./HFM/mainForecastingCodes/selfUseMode.cpp"
#include "./HFM/mainForecastingCodes/jamesTaylor.cpp"
#include "./HFM/mainForecastingCodes/rainMain.cpp"
#include "./HFM/mainForecastingCodes/mokoko.cpp"
#include "./HFM/mainForecastingCodes/AETwoVariables.cpp"
#include "./HFM/mainForecastingCodes/smartStorage.cpp"
#include "./HFM/mainForecastingCodes/HFModel.cpp"
#include "./HFM/mainForecastingCodes/CIF_WCCI.hpp"

using namespace std;
using namespace optframe;
using namespace EFP;

//#include <tr1/random>
//using namespace std::tr1;

int main(int argc, char **argv)
{
//	variate_generator<mt19937, uniform_real<> > rngU(mt19937(123), uniform_real<>());
//	variate_generator<mt19937, normal_distribution<> > rngN(mt19937(123), normal_distribution<>(2, .5));
//	variate_generator<mt19937, binomial_distribution<> > rngB(mt19937(123), binomial_distribution<>(3, .7));
//	variate_generator<mt19937, poisson_distribution<> > rngP(mt19937(123), poisson_distribution<>(20));
//	variate_generator<mt19937, gamma_distribution<> > rngG(mt19937(123), gamma_distribution<>(3));
//
//	for (int i = 0; i < 5; ++i)
//		std::cout << rngU() << " " << rngN() << " " << rngB() << " " << rngP() << " " << 2 * rngG() << std::endl;
//	getchar();

//	if (argc != 10)
//	{
//		cout << "Wrong parameters!" << endl;
//		cout << "Expected parameters are: nome nomeValidationSet saida parameters options precision" << endl;
//		selfUseMode();
//		exit(1);
//	}
//
//	if (argc != 10)
//	{
//		cout << "Wrong parameters!" << endl;
//		cout << "Expected parameters are: nome nomeValidationSet saida parameters options precision" << endl;
//		selfUseMode();
//		exit(1);
//	}

// ==============================================
//Running know forecasting problems

	int type = 0; // price
	//type = 1; // load
	type = 2; // MG
	type = 3; // JAMES TAYLOR 2007 European Dataset
	type = 4; // rain forecast
	type = 69; // mokoko forecast

	type = 9999; // colocar o numero desejado
	int r;

	int trainningMode = 0; //calibration mode active if value is 1

	switch (type)
	{
	case 0: //Eletricity Price Competition Mode
		if (trainningMode == 0)
			r = priceCompetitionBlind(argc, argv);
		else
			r = priceCompetitionCalibrationMode(argc, argv);

		cout << "Program ended successfully in Eletricity Price Mode" << endl;
		return r;
		break;

	case 1: //Load Competition Mode
		if (trainningMode == 0)
			r = loadCompetitionBlind(argc, argv);
		else
			r = loadCompetitionCalibrationMode(argc, argv);

		cout << "Program ended successfully in Load Competition Mode" << endl;
		return r;
		break;

	case 2: //Liu Mode Applied Energy, 2014
		r = microGridLiuAppliedEnergy(argc, argv);
		cout << "Program ended successfully in MG Mode" << endl;
		return r;
		break;

	case 21: //Liu Mode Applied Energy, 2014
		r = microGridLiuAppliedEnergyOnline(argc, argv);
		cout << "Program ended successfully in ONLINE MG Mode" << endl;
		return r;
		break;

	case 3: //James Taylor 2007
		r = jamesTaylorEuropeanDataset(argc, argv);
		cout << "Program ended successfully in James Taylor Eurpean Dataset Mode" << endl;
		return r;
		break;

	case 4: //James Taylor 2007
		r = rainMain(argc, argv);
		cout << "Program ended successfully in James Taylor Eurpean Dataset Mode" << endl;
		return r;
		break;

	case 69: //James Taylor 2007
		r = mokoko(argc, argv);
		cout << "Program ended successfully in James Taylor Eurpean Dataset Mode" << endl;
		return r;
		break;

	case 100: //James Taylor 2007
		r = AETwoVariables(argc, argv);
		cout << "Program ended successfully in AE many variables final batch!" << endl;
		return r;
		break;

	case 1112: //James Taylor 2007
		r = smartStorage(argc, argv);
		cout << "Program ended successfully in Smart Storage" << endl;
		return r;
		break;

	case 9999: //James Taylor 2007
		r = CIFWCCICalibration(argc, argv);
		cout << "Program ended successfully in CIF calibration WCCI Conference!" << endl;
		return r;
		break;


	default:
		int r = HFModel(argc, argv);
		cout << "Program ended unsuccessfully in DEFAULT Mode! Something went wrong...." << endl;
		return r;
	}

}


