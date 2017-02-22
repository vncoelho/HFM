// OptFrame - Optimization Framework

// Copyright (C) 2009-2015
// http://optframe.sourceforge.net/
//
// This file is part of the OptFrame optimization framework. This framework
// is free software; you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License v3 as published by the
// Free Software Foundation.

// This framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License v3 for more details.

// You should have received a copy of the GNU Lesser General Public License v3
// along with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
// USA.

#ifndef HFM_MULTI_EVALUATOR_HPP_
#define HFM_MULTI_EVALUATOR_HPP_

#include "../Solution.hpp"
#include "../Evaluator.hpp"
#include "../MultiEvaluation.hpp"
#include "../MultiEvaluator.hpp"

#include <iostream>

using namespace std;
using namespace scannerpp;

namespace EFP
{

class HFMMultiEvaluator: public MultiEvaluator<RepEFP>
{
	EFPEvaluator& evalEFP;
public:

	HFMMultiEvaluator(EFPEvaluator& _evalEFP) :
		evalEFP(_evalEFP)
	{
	}



public:
	// protected: not possible because of GeneralizedMultiEvaluator

	// TODO: make virtual "= 0"
	virtual MultiEvaluation& evaluate(const RepEFP& r)
	{
		MultiEvaluation* nev = new MultiEvaluation;

		vector<double> foIndicator = evalEFP.evaluateAll(r, -1);

		return *nev;
	}


protected:


	static string idComponent()
	{
		stringstream ss;
		ss << MultiDirection::idComponent() << ":MultiEvaluator:HFM";
		return ss.str();
	}

	virtual string id() const
	{
		return idComponent();
	}

};


}

#endif /*HFM_MULTI_EVALUATOR_HPP_*/
