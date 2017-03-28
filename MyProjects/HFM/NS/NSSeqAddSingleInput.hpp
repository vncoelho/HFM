#ifndef EFP_NSSEQADDSINGLEINPUT_HPP_
#define EFP_NSSEQADDSINGLEINPUT_HPP_

// Framework includes
#include "../../../OptFrame/NSSeq.hpp"
#include "../../../OptFrame/RandGen.hpp"

// Own includes
#include "../ProblemInstance.hpp"
#include "../Solution.h"

using namespace std;
namespace EFP
{

class MoveNEIGHAddSingleInput: public Move<RepEFP, OPTFRAME_DEFAULT_ADS>
{
private:
	int file, K;
	bool reverse;
	ProblemInstance& pEFP;
	RandGen& rg;

public:

	using Move<RepEFP, OPTFRAME_DEFAULT_ADS>::apply; // prevents name hiding
	using Move<RepEFP, OPTFRAME_DEFAULT_ADS>::canBeApplied; // prevents name hiding

	MoveNEIGHAddSingleInput(int _file, int _K, bool _reverse, ProblemInstance& _pEFP, RandGen& _rg) :
			file(_file), K(_K), reverse(_reverse), pEFP(_pEFP), rg(_rg)
	{

	}

	virtual ~MoveNEIGHAddSingleInput()
	{
	}

	bool canBeApplied(const RepEFP& rep, const OPTFRAME_DEFAULT_ADS&)
	{
		return true;
	}

	Move<RepEFP, OPTFRAME_DEFAULT_ADS>* apply(RepEFP& rep, OPTFRAME_DEFAULT_ADS&)
	{
		if (!reverse)
		{
			rep.singleIndex.push_back(make_pair(file, K));
			if (K > rep.earliestInput)
				rep.earliestInput = K;

			int nEXV = file;
			int mean = pEFP.getMean(nEXV);
			int stdDesv = pEFP.getStdDesv(nEXV);
			double meanWeight = pEFP.getMean(0); //File 0 is the target file
			double stdDesvWeight = pEFP.getStdDesv(0);

			double greater = rg.randG(mean, stdDesv);
			double lower = rg.randG(mean, stdDesv);
			double greaterWeight = rg.randG(meanWeight, stdDesvWeight);
			double lowerWeight = rg.randG(meanWeight, stdDesvWeight);

			vector<double> fuzzyRules;
			int fuzzyFunction = rg.rand(NFUZZYFUNCTIONS);
			fuzzyRules.resize(NCOLUMNATRIBUTES);
			fuzzyRules[GREATER] = greater;
			fuzzyRules[GREATER_WEIGHT] = greaterWeight;
			fuzzyRules[LOWER] = lower;
			fuzzyRules[LOWER_WEIGHT] = lowerWeight;
			fuzzyRules[EPSILON] = 1; //TODO TEST FOR TRAPEZOID
			fuzzyRules[PERTINENCEFUNC] = fuzzyFunction; //PERTINENCE FUNCTION

			rep.singleFuzzyRS.push_back(fuzzyRules);
		}
		else
		{
			rep.singleIndex.erase(rep.singleIndex.begin() + rep.singleIndex.size());
			rep.singleFuzzyRS.erase(rep.singleFuzzyRS.begin() + rep.singleFuzzyRS.size());
		}
		return new MoveNEIGHAddSingleInput(file, K, !reverse, pEFP, rg);
	}

	virtual bool operator==(const Move<RepEFP, OPTFRAME_DEFAULT_ADS>& _m) const
	{
		const MoveNEIGHAddSingleInput& m = (const MoveNEIGHAddSingleInput&) _m;
		return ((m.file == file) && (m.K == K));
	}

	void print() const
	{
		cout << "MoveNEIGHAddSingleInput( vector:  explatonary variable " << file << " <=>  k " << K << " )";
		cout << endl;
	}
}
;

class NSIteratorNEIGHAddSingleInput: public NSIterator<RepEFP, OPTFRAME_DEFAULT_ADS>
{
private:
	MoveNEIGHAddSingleInput* m;
	vector<MoveNEIGHAddSingleInput*> moves;
	int index;
	const RepEFP& rep;
	int maxLag, maxUpperLag;
	RandGen& rg;
	ProblemInstance& pEFP;

public:
	NSIteratorNEIGHAddSingleInput(const RepEFP& _rep, int _maxLag, int _maxUpperLag, ProblemInstance& _pEFP, RandGen& _rg) :
			rep(_rep), maxLag(_maxLag), maxUpperLag(_maxUpperLag), pEFP(_pEFP), rg(_rg)
	{
		index = 0;
		m = NULL;
	}

	virtual ~NSIteratorNEIGHAddSingleInput()
	{
		for (int i = index + 1; i < moves.size(); i++)
			delete moves[i];
	}

	virtual void first()
	{

		for (int lag = 1; lag <= maxLag; lag++)
		{
			moves.push_back(new MoveNEIGHAddSingleInput(0, lag, false, pEFP, rg));
		}

		if (moves.size() > 0)
		{
			m = moves[index];
		}
		else
			m = NULL;
	}

	virtual void next()
	{
		index++;
		if (index < moves.size())
		{
			m = moves[index];
		}
		else
			m = NULL;
	}

	virtual bool isDone()
	{
		return m == NULL;
	}

	virtual Move<RepEFP, OPTFRAME_DEFAULT_ADS>& current()
	{
		if (isDone())
		{
			cout << "There isnt any current element!" << endl;
			cout << "NSIteratorNEIGHChangeSingleInput. Aborting." << endl;
			exit(1);
		}

		return *m;
	}

};

class NSSeqNEIGHAddSingleInput: public NSSeq<RepEFP>
{
private:
	RandGen& rg;
	ProblemInstance& pEFP;
	int maxLag, maxUpperLag;

public:

	using NSSeq<RepEFP>::move; // prevents name hiding

	NSSeqNEIGHAddSingleInput(ProblemInstance& _pEFP, RandGen& _rg, int _maxLag, int _maxUpperLag) :
			pEFP(_pEFP), rg(_rg), maxLag(_maxLag), maxUpperLag(_maxUpperLag)
	{
	}

	virtual ~NSSeqNEIGHAddSingleInput()
	{
	}

	virtual Move<RepEFP, OPTFRAME_DEFAULT_ADS>& move(const RepEFP& rep, const OPTFRAME_DEFAULT_ADS&)
	{
		//TODO - Check the possibility of add negative K values
		int K = rg.rand(maxLag) + 1; // because the values 0 can not be an input
		int file = rg.rand(pEFP.getNumberExplanatoryVariables());

		return *new MoveNEIGHAddSingleInput(file, K, false, pEFP, rg); // return a random move
	}

	virtual NSIterator<RepEFP, OPTFRAME_DEFAULT_ADS>& getIterator(const RepEFP& rep, const OPTFRAME_DEFAULT_ADS&)
	{
		return *new NSIteratorNEIGHAddSingleInput(rep, maxLag, maxUpperLag, pEFP, rg); // return an iterator to the neighbors of 'rep'
	}

	virtual string toString() const
	{
		stringstream ss;
		ss << "NSSeqNEIGHAddSingleInput";
		return ss.str();
	}
};

}
#endif /*EFP_NSSEQADDSINGLEINPUT_HPP_*/

