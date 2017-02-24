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


#include <iostream>

using namespace std;
using namespace scannerpp;

namespace EFP
{

class HFMMultiEvaluator: public MultiEvaluator<RepEFP, OPTFRAME_DEFAULT_ADS>
{
	EFPEvaluator& evalEFP;
public:

	HFMMultiEvaluator(EFPEvaluator& _evalEFP) :
		evalEFP(_evalEFP)
	{
	}


	~HFMMultiEvaluator()
	{
	}

	MultiEvaluation& evaluate(const RepEFP& r)
	{
		cout<<"oi aqui"<<endl;
		MultiEvaluation* nev = new MultiEvaluation;

		vector<double> foIndicator = evalEFP.evaluateAll(r, -1);

		nev->addEvaluation(*new EvaluationEFP(foIndicator[MAPE_INDEX]));
		nev->addEvaluation(*new EvaluationEFP(foIndicator[RMSE_INDEX]));
		nev->addEvaluation(*new EvaluationEFP(foIndicator[WMAPE_INDEX]));
		nev->addEvaluation(*new EvaluationEFP(foIndicator[SMAPE_INDEX]));
		nev->addEvaluation(*new EvaluationEFP(foIndicator[MMAPE_INDEX]));


		return *nev;
	}

	MultiEvaluation& evaluate(const RepEFP& r, const OPTFRAME_DEFAULT_ADS&)
	{
		cout<<"oi correct"<<endl;
		return evaluate(r);
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
