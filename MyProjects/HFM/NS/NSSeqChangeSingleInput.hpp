#ifndef EFP_NSSEQCHANGESINGLEINPUT_HPP_
#define EFP_NSSEQCHANGESINGLEINPUT_HPP_

// Framework includes
#include "../../../OptFrame/NSSeq.hpp"
#include "../../../OptFrame/RandGen.hpp"

// Own includes
#include "../ProblemInstance.hpp"
#include "../Solution.h"

using namespace std;
namespace EFP
{

class MoveNEIGHChangeSingleInput: public Move<RepEFP, OPTFRAME_DEFAULT_ADS>
{
private:
	int rule;
	bool sign;
	int X;
	int maxLag, maxUpperLag;

public:

	using Move<RepEFP, OPTFRAME_DEFAULT_ADS>::apply; // prevents name hiding
	using Move<RepEFP, OPTFRAME_DEFAULT_ADS>::canBeApplied; // prevents name hiding

	MoveNEIGHChangeSingleInput(int _rule, bool _sign, int _maxLag, int _maxUpperLag, int _X) :
			rule(_rule), sign(_sign), maxLag(_maxLag), maxUpperLag(_maxUpperLag), X(_X)
	{

	}

	virtual ~MoveNEIGHChangeSingleInput()
	{
	}

	bool canBeApplied(const RepEFP& rep, const OPTFRAME_DEFAULT_ADS&)
	{
		bool minimumLag = false;
		bool maxLagCheck = false;
		bool notNull1 = false;
		bool notNull2 = false;
		if ((rule >= 0) && (rule < rep.singleIndex.size()))
		{
			maxLagCheck = ((rep.singleIndex[rule].second + X) <= maxLag);
			minimumLag = ((rep.singleIndex[rule].second - X) > maxUpperLag);
			notNull1 = ((rep.singleIndex[rule].second - X) != 0); //TODO -- strategy to allow moves when samples are being learning
			notNull2 = ((rep.singleIndex[rule].second + X) != 0);
		}

		return minimumLag && maxLagCheck && (rule >= 0) && (rule < rep.singleIndex.size()) && notNull1 && notNull2;
	}

	Move<RepEFP, OPTFRAME_DEFAULT_ADS>* apply(RepEFP& rep, OPTFRAME_DEFAULT_ADS&)
	{

		if (sign == false)
			rep.singleIndex[rule].second += X;
		else
			rep.singleIndex[rule].second -= X;

//		if (rep.singleIndex[rule].second > rep.earliestInput)
//			rep.earliestInput = rep.singleIndex[rule].second;

		return new MoveNEIGHChangeSingleInput(rule, !sign, maxLag, maxUpperLag, X);
	}

	virtual bool operator==(const Move<RepEFP, OPTFRAME_DEFAULT_ADS>& _m) const
	{
		const MoveNEIGHChangeSingleInput& m = (const MoveNEIGHChangeSingleInput&) _m;
		return ((m.rule == rule) && (m.sign == sign) && (m.maxLag == maxLag) && (m.maxUpperLag == maxUpperLag) && (m.X == X));
	}

	void print() const
	{
		cout << "MoveNEIGHChangeSingleInput( vector:  rule " << rule << " <=>  sign " << sign << "\t maxLag" << maxLag << "\t maxUpperLag" << maxUpperLag << "\t X" << X << " )";
		cout << endl;
	}
}
;

class NSIteratorNEIGHChangeSingleInput: public NSIterator<RepEFP, OPTFRAME_DEFAULT_ADS>
{
private:
	MoveNEIGHChangeSingleInput* m;
	vector<MoveNEIGHChangeSingleInput*> moves;
	int index;
	const RepEFP& rep;
	int maxLag, maxUpperLag;

public:
	NSIteratorNEIGHChangeSingleInput(const RepEFP& _rep, int _maxLag, int _maxUpperLag) :
			rep(_rep), maxLag(_maxLag), maxUpperLag(_maxUpperLag)
	{
		index = 0;
		m = NULL;
	}

	virtual ~NSIteratorNEIGHChangeSingleInput()
	{
		for (int i = index + 1; i < moves.size(); i++)
			delete moves[i];
	}

	virtual void first()
	{
		int X = 1;
		for (int rule = 0; rule < rep.singleIndex.size(); rule++)
		{
			moves.push_back(new MoveNEIGHChangeSingleInput(rule, false, maxLag, maxUpperLag, X));
			moves.push_back(new MoveNEIGHChangeSingleInput(rule, true, maxLag, maxUpperLag, X));
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

class NSSeqNEIGHChangeSingleInput: public NSSeq<RepEFP>
{
private:
	RandGen& rg;
	ProblemInstance& pEFP;
	int maxLag, maxUpperLag;

public:

	using NSSeq<RepEFP>::move; // prevents name hiding

	NSSeqNEIGHChangeSingleInput(ProblemInstance& _pEFP, RandGen& _rg, int _maxLag, int _maxUpperLag) :
			pEFP(_pEFP), rg(_rg), maxLag(_maxLag), maxUpperLag(_maxUpperLag)
	{
	}

	virtual ~NSSeqNEIGHChangeSingleInput()
	{
	}

	virtual Move<RepEFP, OPTFRAME_DEFAULT_ADS>& move(const RepEFP& rep, const OPTFRAME_DEFAULT_ADS&)
	{
		int MAXCHANGE = 5;
		int X = rg.rand(MAXCHANGE) + 1;
		int rule = -1;
		if (rep.singleIndex.size() > 0)
			rule = rg.rand(rep.singleIndex.size());

		int sign = rg.rand(2);

		return *new MoveNEIGHChangeSingleInput(rule, sign, maxLag, maxUpperLag, X); // return a random move
	}

	virtual NSIterator<RepEFP, OPTFRAME_DEFAULT_ADS>& getIterator(const RepEFP& rep, const OPTFRAME_DEFAULT_ADS&)
	{
		return *new NSIteratorNEIGHChangeSingleInput(rep, maxLag, maxUpperLag); // return an iterator to the neighbors of 'rep'
	}

	virtual string toString() const
	{
		stringstream ss;
		ss << "NSSeqNEIGHChangeSingleInput";
		return ss.str();
	}
};

}
#endif /*EFP_NSSEQCHANGESINGLEINPUT_HPP_*/

