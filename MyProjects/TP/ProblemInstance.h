#ifndef TP_PROBLEMINSTANCE_H_
#define TP_PROBLEMINSTANCE_H_

#include <iostream>

#include "../../OptFrame/Scanner++/Scanner.h"
#include "../../OptFrame/Component.hpp"
#include "../../OptFrame/Util/printable.h"

using namespace std;
using namespace scannerpp;
using namespace optframe;


class ProblemInstance : public Component
{
public:
	vector<vector<double> > inputs;
	vector<string> operators;

public:
	ProblemInstance();
	
	virtual ~ProblemInstance();

	int getNSamples();
};



#endif /*TP_PROBLEMINSTANCE_H_*/

