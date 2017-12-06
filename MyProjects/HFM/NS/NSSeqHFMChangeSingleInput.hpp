#ifndef HFM_NSSEQCHANGESINGLEINPUT_HPP_
#define HFM_NSSEQCHANGESINGLEINPUT_HPP_

// Framework includes
#include "../../../OptFrame/NSSeq.hpp"
#include "../../../OptFrame/RandGen.hpp"

// Own includes
#include "../ProblemInstance.hpp"
#include "../Solution.h"

using namespace std;
namespace HFM
{

class MoveHFMChangeSingleInput: public Move<RepEFP, OPTFRAME_DEFAULT_ADS>
{
private:
	int rule;
	bool sign;
	int maxLag, maxUpperLag;
	int X;


public:


	MoveHFMChangeSingleInput(int _rule, bool _sign, int _maxLag, int _maxUpperLag, int _X) :
			rule(_rule), sign(_sign), maxLag(_maxLag), maxUpperLag(_maxUpperLag), X(_X)
	{

	}

	virtual ~MoveHFMChangeSingleInput()
	{
	}

	bool canBeApplied(const RepEFP& rep, const OPTFRAME_DEFAULT_ADS*)
	{
		bool minimumLag = false;
		bool maxLagCheck = false;
		bool notNull1 = false;
		bool notNull2 = false;
		if ((rule >= 0) && (rule < (int) rep.singleIndex.size()))
		{
			maxLagCheck = ((rep.singleIndex[rule].second + X) <= maxLag);
			minimumLag = ((rep.singleIndex[rule].second - X) > maxUpperLag);
			notNull1 = ((rep.singleIndex[rule].second - X) != 0); //TODO -- strategy to allow moves when samples are being learning
			notNull2 = ((rep.singleIndex[rule].second + X) != 0);
		}

		return minimumLag && maxLagCheck && notNull1 && notNull2;
	}

	Move<RepEFP, OPTFRAME_DEFAULT_ADS>* apply(RepEFP& rep, OPTFRAME_DEFAULT_ADS*)
	{

		if (!sign)
			rep.singleIndex[rule].second += X;
		else
			rep.singleIndex[rule].second -= X;

//		if (rep.singleIndex[rule].second > rep.earliestInput)
//			rep.earliestInput = rep.singleIndex[rule].second;

		return new MoveHFMChangeSingleInput(rule, !sign, maxLag, maxUpperLag, X);
	}

	virtual bool operator==(const Move<RepEFP, OPTFRAME_DEFAULT_ADS>& _m) const
	{
		const MoveHFMChangeSingleInput& m = (const MoveHFMChangeSingleInput&) _m;
		return ((m.rule == rule) && (m.sign == sign) && (m.maxLag == maxLag) && (m.maxUpperLag == maxUpperLag) && (m.X == X));
	}

	void print() const
	{
		cout << "MoveNEIGHChangeSingleInput( vector:  rule " << rule << " <=>  sign " << sign << "\t X:" << X << "\t maxLag " << maxLag << "\t maxUpperLag " << maxUpperLag <<  " )";
		cout << endl;
	}
}
;

class NSIteratorHFMChangeSingleInput: public NSIterator<RepEFP, OPTFRAME_DEFAULT_ADS>
{
private:
	MoveHFMChangeSingleInput* m;
	vector<MoveHFMChangeSingleInput*> moves;
	int index;
	const RepEFP& rep;
	int maxLag, maxUpperLag;

public:
	NSIteratorHFMChangeSingleInput(const RepEFP& _rep, int _maxLag, int _maxUpperLag) :
			rep(_rep), maxLag(_maxLag), maxUpperLag(_maxUpperLag)
	{
		index = 0;
		m = NULL;
	}

	virtual ~NSIteratorHFMChangeSingleInput()
	{
		for (int i = index + 1; i < (int) moves.size(); i++)
			delete moves[i];
	}

	virtual void first()
	{
		int X = 1;
		for (int rule = 0; rule < (int) rep.singleIndex.size(); rule++)
		{
			bool sign = false;
			moves.push_back(new MoveHFMChangeSingleInput(rule, sign, maxLag, maxUpperLag, X));
			moves.push_back(new MoveHFMChangeSingleInput(rule, !sign, maxLag, maxUpperLag, X));
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
			cout << "NSIteratorHFMChangeSingleInput. Aborting." << endl;
			exit(1);
		}

		return m;
	}

};

class NSSeqHFMChangeSingleInput: public NSSeq<RepEFP>
{
private:
	ProblemInstance& pEFP;
	RandGen& rg;
	int maxLag, maxUpperLag;

public:

	NSSeqHFMChangeSingleInput(ProblemInstance& _pEFP, RandGen& _rg, int _maxLag, int _maxUpperLag) :
			pEFP(_pEFP), rg(_rg), maxLag(_maxLag), maxUpperLag(_maxUpperLag)
	{
	}

	virtual ~NSSeqHFMChangeSingleInput()
	{
	}

	virtual Move<RepEFP, OPTFRAME_DEFAULT_ADS>* randomMove(const RepEFP& rep, const OPTFRAME_DEFAULT_ADS*)
	{
		int MAXCHANGE = 5;
		int X = rg.rand(MAXCHANGE) + 1;
		int rule = -1;
		if (rep.singleIndex.size() > 0)
			rule = rg.rand(rep.singleIndex.size());

		int sign = rg.rand(2);

		return new MoveHFMChangeSingleInput(rule, sign, maxLag, maxUpperLag, X); // return a random move
	}

	virtual NSIterator<RepEFP, OPTFRAME_DEFAULT_ADS>* getIterator(const RepEFP& rep, const OPTFRAME_DEFAULT_ADS*)
	{
		return new NSIteratorHFMChangeSingleInput(rep, maxLag, maxUpperLag); // return an iterator to the neighbors of 'rep'
	}

	virtual string toString() const
	{
		stringstream ss;
		ss << "NSSeqHFMChangeSingleInput";
		return ss.str();
	}
};

}
#endif /*HFM_NSSEQCHANGESINGLEINPUT_HPP_*/

