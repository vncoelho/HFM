#ifndef BBV_PROBLEMINSTANCE_H_
#define BBV_PROBLEMINSTANCE_H_

#include <iostream>

#include "../../OptFrame/Scanner++/Scanner.h"
#include "../../OptFrame/Component.hpp"

using namespace std;
using namespace scannerpp;
using namespace optframe;

namespace BBV
{

class ProblemInstanceBBV : public Component
{
private:
	// Your private vars

public:
	ProblemInstanceBBV(Scanner& scanner);
	
	virtual ~ProblemInstanceBBV();
};

}

#endif /*BBV_PROBLEMINSTANCE_H_*/

