#ifndef BBVTOOLS_Random_H_
#define BBVTOOLS_Random_H_
// ===================================
// Main.cpp file generated by OptFrame
// Project BBV
// ===================================

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "Evaluator.h"
#include "ConstructiveRandom.h"
#include "NSSeqBuySellPaper.h"
#include "ProblemInstance.h"
// LOADER IS NOT DEFAULT ANYMORE. USE ONLY IF YOU NEED...
//#include "../OptFrame/Loader.hpp"

#include "../../OptFrame/Util/CheckCommand.hpp"

using namespace std;
using namespace optframe;
using namespace BBV;

//function for calling Brincando com a Bolsa de Valores

class BBVTools
{
private:

public:
	BBVTools(){

	}

	virtual ~BBVTools(){

	}

	void createBBV()
	{
		RandGen rg;

		// Initialize here all your OptFrame components
		// (ProblemInstance, Evaluator, Constructive, ...)

		Scanner scanner("");
		ProblemInstanceBBV p(scanner);

		EvalutorBBV ev(p);

		NSSeqBuySellPaper ns1(p, rg);

		ConstructiveRandomBBV c1(p);

		CheckCommand<RepBBV> check;
		check.add(ev);
		check.add(c1);
		check.add(ns1);

		check.run(10, 10);

		cout << "Program ended successfully" << endl;
	}

};

#endif /*BBVTOOLS_Random_H_*/

