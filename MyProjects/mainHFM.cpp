// ===================================
// Main.cpp file generated by OptFrame
// Project EFP
// ===================================

#include <stdlib.h>
#include <math.h>
#include <iostream>

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
#include "./HFM/mainForecastingCodes/GAPSO.hpp"
#include "./HFM/mainForecastingCodes/EEGLearning.hpp"
#include "./HFM/mainForecastingCodes/hosseinData.hpp"
#include "./HFM/mainForecastingCodes/MokokoWind.hpp"
#include "./HFM/mainForecastingCodes/REW2016_REED.hpp"
#include "HFM/mainForecastingCodes/stockMarket.hpp"
#include "HFM/mainForecastingCodes/readMP3.hpp"
#include "HFM/mainForecastingCodes/usingNonDominatedModels.hpp"
#include "HFM/mainForecastingCodes/musicGen.hpp"

using namespace std;
using namespace optframe;
using namespace HFM;

//#include <tr1/random>
//using namespace std::tr1;

enum SwitchOptions
{
	Option_Invalid, APEN_DEMAND_REW2016_REED, APEN_REED_SPEEDUP, STOCKMARKET, MUSIC_GEN, MUSIC_GEN_MIDI, GENERATE_NONDOMINATED_HFM_MODELS, APEN_MICROGRID, APEN_MICROGRID_ONLINE, APEN_TWO_VARIABLES_MULTIVARIATE_FORECASTS, LOAD_COMP, PRICE_COMP, PRICE_COMP_TM, LOAD_COMP_TM, WCCI_CFI_COMP, JAMES_TAYLOR_EUROPEAN_DATASET, RAIN_FORECAST, MUSIC_GENERATION_WITH_FORECASTS, MOKOKO_WIND_FORECASTS, MOKOKO_WIND_POWER_PROBABILISTIC_FORECASTS, EEG_BIOMETRIC_SYSTEM, GAPSO_SKU_TESTS_FOR_BENCHMARK, WCCI_CFI_COMP_CALIBRATION, HOSSEIN_BLIND_FORECASTS_LOAD_FORECASTS,SMART_STORAGE
};

int main(int argc, char **argv)
{
	bool r;
	SwitchOptions optionSwitch = GENERATE_NONDOMINATED_HFM_MODELS; //Important paper on the way

	optionSwitch = STOCKMARKET;

	switch (optionSwitch)
	{

	case APEN_DEMAND_REW2016_REED:
		r = APEN_SI_DemandForecasting(argc, argv);
		cout << "Program ended successfully in MG Mode" << endl;
		return r;
		break;

	case APEN_REED_SPEEDUP:

		r = APEN_SI_SpeedUp_DemandForecasting(argc, argv);

		cout << "Program ended successfully in MG Mode for checking SpeedUp_MultiThread" << endl;
		return r;
		break;

	case PRICE_COMP: //Eletricity Price Competition Mode
		r = priceCompetitionBlind(argc, argv);
		cout << "Program ended successfully in Eletricity Price Mode" << endl;
		return r;
		break;

	case PRICE_COMP_TM: //Eletricity Price Competition Training Mode
		r = priceCompetitionCalibrationMode(argc, argv);
		cout << "Program ended successfully in Training Mode Eletricity Price" << endl;
		return r;
		break;

	case LOAD_COMP: //Load Competition Mode
		r = loadCompetitionBlind(argc, argv);
		cout << "Program ended successfully in Load Competition Mode" << endl;
		return r;
		break;

	case LOAD_COMP_TM: //Load Competition Training Mode
		r = loadCompetitionCalibrationMode(argc, argv);
		cout << "Program ended successfully in Load Competition Training Mode" << endl;
		return r;
		break;

	case APEN_MICROGRID: //Liu Mode Applied Energy, 2014
		r = microGridLiuAppliedEnergy(argc, argv);
		cout << "Program ended successfully in MG Mode" << endl;
		return r;
		break;

	case APEN_MICROGRID_ONLINE: //Liu Mode Applied Energy, 2014
		r = microGridLiuAppliedEnergyOnline(argc, argv);
		cout << "Program ended successfully in ONLINE MG Mode" << endl;
		return r;
		break;

	case JAMES_TAYLOR_EUROPEAN_DATASET: //James Taylor 2007
		r = jamesTaylorEuropeanDataset(argc, argv);
		cout << "Program ended successfully in James Taylor Eurpean Dataset Mode" << endl;
		return r;
		break;

	case RAIN_FORECAST: //James Taylor 2007
		r = rainMain(argc, argv);
		cout << "Program ended successfully in rainMode" << endl;
		return r;
		break;

	case APEN_TWO_VARIABLES_MULTIVARIATE_FORECASTS: //James Taylor 2007
		r = AETwoVariables(argc, argv);
		cout << "Program ended successfully in AE many variables final batch!" << endl;
		return r;
		break;

	case SMART_STORAGE:
		r = smartStorage(argc, argv);
		cout << "Program ended successfully in Smart Storage" << endl;
		return r;
		break;

	case WCCI_CFI_COMP_CALIBRATION:
		r = CIFWCCICalibration(argc, argv);
		cout << "Program ended successfully in CIF calibration WCCI Conference!" << endl;
		return r;
		break;

	case WCCI_CFI_COMP: //CIFWCCIGeneratingForecasts
		r = CIFWCCIGeneratingForecasts(argc, argv);
		cout << "Program ended successfully in CIF calibration WCCI Conference!" << endl;
		return r;
		break;

	case GAPSO_SKU_TESTS_FOR_BENCHMARK:
		r = GAPSO_SKU(argc, argv);
		cout << "Program ended successfully in GAPSO-SKU forecasting!" << endl;
		return r;
		break;

	case EEG_BIOMETRIC_SYSTEM:
		r = EEGBiometricSystem(argc, argv);
		cout << "Program ended successfully in EEG learning!" << endl;
		return r;
		break;

	case HOSSEIN_BLIND_FORECASTS_LOAD_FORECASTS:
		r = hosseinBlindForecasts(argc, argv);
		cout << "Program ended successfully in Hossein Model Blind !" << endl;
		return r;
		break;

	case MOKOKO_WIND_FORECASTS: //James Taylor 2007
		r = mokokoWindSotavento(argc, argv);
		cout << "Program ended successfully in Mokoko 2 !" << endl;
		return r;
		break;

	case MOKOKO_WIND_POWER_PROBABILISTIC_FORECASTS:
		r = mokokoProbabilisticForecastWindPower(argc, argv);
		cout << "Program ended successfully in Mokoko's place configuration! \n Ziiz Gud ! \n A naice to meet you ! \n Godbye" << endl;
		return r;
		break;

	case STOCKMARKET: //James Taylor 2007
		r = stockMarketForecasting(argc, argv);
		cout << "Parabens! Previsao da bolsa terminada" << endl;
		return r;
		break;

	case MUSIC_GENERATION_WITH_FORECASTS: //James Taylor 2007
		r = readWAV(argc, argv);
		cout << "Parabens! WAV lido com sucessos" << endl;
		return r;
		break;

	case GENERATE_NONDOMINATED_HFM_MODELS: //James Taylor 2007
		r = usingNonDominatedHFMModels(argc, argv);
		cout << "Parabens! Non-dominated solutions successfully have been generated." << endl;
		return r;
		break;

	case MUSIC_GEN:
		r = musicGen(argc, argv);
		cout << "Parabens! Music has been created." << endl;
		return r;
		break;

	case MUSIC_GEN_MIDI:
		r = musicGenMidiCSV(argc, argv);
		cout << "Parabens! Music MIDI has been created." << endl;
		return r;
		break;

	default:
		int r = HFM_API(argc, argv);
		cout << "Program ended unsuccessfully in DEFAULT Mode! Something went wrong...." << endl;
		return r;
	}

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
}

