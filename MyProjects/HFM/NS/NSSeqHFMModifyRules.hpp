#ifndef HFM_NSSEQNEIGHMODIFYRULES_HPP_
#define HFM_NSSEQNEIGHMODIFYRULES_HPP_

// Framework includes
#include "../../../OptFrame/NSSeq.hpp"
#include "../../../OptFrame/RandGen.hpp"

// Own includes
#include "../ProblemInstance.hpp"
#include "../Solution.h"

using namespace std;
namespace HFM
{

class MoveHFMModifyRule: public Move<RepEFP, OPTFRAME_DEFAULT_ADS>
{
private:
	int r, o;
	double applyValue;
	bool sign;
	int vectorType;
public:

	MoveHFMModifyRule(int _r, int _o, double _applyValue, bool _sign, int _vectorType) :
			r(_r), o(_o), applyValue(_applyValue), sign(_sign), vectorType(_vectorType)
	{

	}

	virtual ~MoveHFMModifyRule()
	{
	}

	bool canBeApplied(const RepEFP& rep, const OPTFRAME_DEFAULT_ADS*)
	{
		return true;
	}

	Move<RepEFP, OPTFRAME_DEFAULT_ADS>* apply(RepEFP& rep, OPTFRAME_DEFAULT_ADS*)
	{
		if (r == -1)
			return new MoveHFMModifyRule(-1, -1, -1, -1, -1);

		if (r == PERTINENCEFUNC)
		{
			if (vectorType == Single_Input)
				rep.singleFuzzyRS[o][r] = !rep.singleFuzzyRS[o][r];
			if (vectorType == Average_Inputs)
				rep.averageFuzzyRS[o][r] = !rep.averageFuzzyRS[o][r];
			if (vectorType == Derivative_Inputs)
				rep.derivativeFuzzyRS[o][r] = !rep.derivativeFuzzyRS[o][r];

			return new MoveHFMModifyRule(r, o, applyValue, !sign, vectorType);
		}

		if (vectorType == Single_Input)
		{

			if (!sign)
				rep.singleFuzzyRS[o][r] += applyValue;
			else
				rep.singleFuzzyRS[o][r] -= applyValue;
		}

		if (vectorType == Average_Inputs)
		{
			if (!sign)
				rep.averageFuzzyRS[o][r] += applyValue;
			else
				rep.averageFuzzyRS[o][r] -= applyValue;
		}

		if (vectorType == Derivative_Inputs)
		{
			if (!sign)
				rep.derivativeFuzzyRS.at(o).at(r) += applyValue;
			else
				rep.derivativeFuzzyRS[o][r] -= applyValue;
		}
		// return reverse move
		return new MoveHFMModifyRule(r, o, applyValue, !sign, vectorType);
	}

	virtual bool operator==(const Move<RepEFP, OPTFRAME_DEFAULT_ADS>& _m) const
	{
		const MoveHFMModifyRule& m = (const MoveHFMModifyRule&) _m;
		return ((m.r == r) && (m.o == o) && (m.sign == sign));
	}

	void print() const
	{
		cout << "MoveNEIGHModifyRule( vector: " << r << " : ";
		cout << " option " << o << " <=>  sign " << sign << "vectorType " << vectorType << " )";
		cout << endl;
	}
}
;

class NSIteratorHFMModifyRules: public NSIterator<RepEFP, OPTFRAME_DEFAULT_ADS>
{
private:
	MoveHFMModifyRule* m;
	vector<MoveHFMModifyRule*> moves;
	int index;
	const RepEFP& rep;
	ProblemInstance& pEFP;
	vector<double>& vUpdateValues;
public:
	NSIteratorHFMModifyRules(const RepEFP& _rep, ProblemInstance& _pEFP, vector<double>& _vUpdateValues) :
			rep(_rep), pEFP(_pEFP), vUpdateValues(_vUpdateValues)
	{
		index = 0;
		m = NULL;
	}

	virtual ~NSIteratorHFMModifyRules()
	{
		for (int i = index + 1; i < (int) moves.size(); i++)
			delete moves[i];

	}

	virtual void first()
	{
		int nShakes = vUpdateValues.size();
		int options = rep.singleFuzzyRS.size(); //rep.size() options

		for (int sign = 0; sign < 2; sign++)
			for (int r = 0; r < NCOLUMNATRIBUTES; r++)
				for (int o = 0; o < options; o++)
					for (int v = 0; v < nShakes; v++)
					{
						moves.push_back(new MoveHFMModifyRule(r, o, vUpdateValues[v], sign, 0));
					}

		options = rep.averageFuzzyRS.size(); //rep.size() options

		for (int sign = 0; sign < 2; sign++)
			for (int r = 0; r < NCOLUMNATRIBUTES; r++)
				for (int o = 0; o < options; o++)
					for (int v = 0; v < nShakes; v++)
					{
						moves.push_back(new MoveHFMModifyRule(r, o, vUpdateValues[v], sign, 1));
					}

		options = rep.derivativeFuzzyRS.size(); //rep.size() options

		for (int sign = 0; sign < 2; sign++)
			for (int r = 0; r < NCOLUMNATRIBUTES; r++)
				for (int o = 0; o < options; o++)
					for (int v = 0; v < (int) nShakes; v++)
					{
						moves.push_back(new MoveHFMModifyRule(r, o, vUpdateValues[v], sign, 2));
					}

		if (moves.size() > 0)
			m = moves[index];
		else
			m = NULL;
	}

	virtual void next()
	{
		index++;
		if (index < (int) moves.size())
			m = moves[index];
		else
			m = NULL;

	}

	virtual bool isDone()
	{
		return m == NULL;
	}

	virtual Move<RepEFP, OPTFRAME_DEFAULT_ADS>* current()
	{
		if (isDone())
		{
			cout << "There isnt any current element!" << endl;
			cout << "NSIteratorNEIGHModifyRules. Aborting." << endl;
			exit(1);
		}

		return m;
	}

};

class NSSeqHFMModifyRules: public NSSeq<RepEFP, OPTFRAME_DEFAULT_ADS>
{
private:
	ProblemInstance& pEFP;
	RandGen& rg;
	vector<double>* vUpdateValues;
public:

	NSSeqHFMModifyRules(ProblemInstance& _pEFP, RandGen& _rg, vector<double>* _vUpdateValues = NULL) :
			pEFP(_pEFP), rg(_rg), vUpdateValues(_vUpdateValues)
	{
		//TODO mean from the targetfile
		double mean = pEFP.getMean(0);

		if (!vUpdateValues)
		{
			vUpdateValues->push_back(0.01);
			vUpdateValues->push_back(0.1);
			vUpdateValues->push_back(1);
			vUpdateValues->push_back(mean / 30);
			vUpdateValues->push_back(mean / 15);
			vUpdateValues->push_back(mean / 6);
			vUpdateValues->push_back(mean / 2);
			vUpdateValues->push_back(mean);
			vUpdateValues->push_back(mean * 2);
			vUpdateValues->push_back(mean * 5);
			vUpdateValues->push_back(mean * 100);
		}
		else
		{
			cout << "Modify values given as input:" << *_vUpdateValues << endl;
			assert(_vUpdateValues->size() > 0);
		}
	}

	virtual ~NSSeqHFMModifyRules()
	{
	}

	virtual Move<RepEFP, OPTFRAME_DEFAULT_ADS>* randomMove(const RepEFP& rep, const OPTFRAME_DEFAULT_ADS*)
	{

		int vectorType = rg.rand(N_Inputs_Types);
		int o = -1;
		int r = -1;

		int maxTries = 1000;
		int tries = 1;
		while ((r == -1) && (tries < maxTries))
		{
			vectorType = rg.rand(N_Inputs_Types);

			if (vectorType == Single_Input)
			{
				if (rep.singleFuzzyRS.size() > 0)
				{
					o = rg.rand(rep.singleFuzzyRS.size()); //rep.size() options
					r = rg.rand(NCOLUMNATRIBUTES);
				}
			}

			if (vectorType == Average_Inputs)
			{
				if (rep.averageFuzzyRS.size() > 0)
				{
					o = rg.rand(rep.averageFuzzyRS.size()); //rep.size() options
					r = rg.rand(NCOLUMNATRIBUTES);
				}
			}

			if (vectorType == Derivative_Inputs)
			{
				if (rep.derivativeFuzzyRS.size() > 0)
				{
					o = rg.rand(rep.derivativeFuzzyRS.size()); //rep.size() options
					r = rg.rand(NCOLUMNATRIBUTES);
				}
			}
		}

		if (tries == maxTries)
			return new MoveHFMModifyRule(-1, -1, -1, -1, -1); // return a random move

		int applyRand = rg.rand(vUpdateValues->size());
		double applyValue = vUpdateValues->at(applyRand);
		bool sign = rg.rand(2);

		return new MoveHFMModifyRule(r, o, applyValue, sign, vectorType); // return a random move
	}

	virtual NSIterator<RepEFP, OPTFRAME_DEFAULT_ADS>* getIterator(const RepEFP& rep, const OPTFRAME_DEFAULT_ADS*)
	{
		return new NSIteratorHFMModifyRules(rep, pEFP, *vUpdateValues); // return an iterator to the neighbors of 'rep'
	}

	virtual string toString() const
	{
		stringstream ss;
		ss << "NSSeqHFMModifyRules with move: ";
		return ss.str();
	}
};

}
#endif /*HFM_NSSEQNEIGHMODIFYRULES_HPP_*/

