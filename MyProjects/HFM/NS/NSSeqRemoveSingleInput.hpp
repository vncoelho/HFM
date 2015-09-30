#ifndef EFP_NSSEQREMOVESINGLEINPUT_HPP_
#define EFP_NSSEQREMOVESINGLEINPUT_HPP_

// Framework includes
#include "../../../OptFrame/NSSeq.hpp"
#include "../../../OptFrame/RandGen.hpp"

// Own includes
#include "../ProblemInstance.hpp"
#include "../Solution.h"

using namespace std;
namespace EFP
{

class MoveNEIGHRemoveSingleInput: public Move<RepEFP, OPTFRAME_DEFAULT_ADS>
{
private:
	int rule;
	bool reverse;
public:

	using Move<RepEFP, OPTFRAME_DEFAULT_ADS>::apply; // prevents name hiding
	using Move<RepEFP, OPTFRAME_DEFAULT_ADS>::canBeApplied; // prevents name hiding

	MoveNEIGHRemoveSingleInput(int _rule, bool _reverse) :
			rule(_rule), reverse(_reverse)
	{

	}

	virtual ~MoveNEIGHRemoveSingleInput()
	{
	}

	bool canBeApplied(const RepEFP& rep, const OPTFRAME_DEFAULT_ADS&)
	{

		return (rule >= 0);
	}

	Move<RepEFP, OPTFRAME_DEFAULT_ADS>* apply(RepEFP& rep, OPTFRAME_DEFAULT_ADS&)
	{

		if (reverse == false)
		{
			if (rep.singleIndex.size() >= 0)
			{
				rep.singleIndex.erase(rep.singleIndex.begin() + rule);
				rep.singleFuzzyRS.erase(rep.singleFuzzyRS.begin() + rule);
			}
		}

		return new MoveNEIGHRemoveSingleInput(rule, !reverse);
	}

	virtual bool operator==(const Move<RepEFP, OPTFRAME_DEFAULT_ADS>& _m) const
	{
		const MoveNEIGHRemoveSingleInput& m = (const MoveNEIGHRemoveSingleInput&) _m;
		return ((m.rule == rule) && (m.reverse == reverse));
	}

	void print() const
	{
		cout << "MoveNEIGHRemoveSingleInput( vector:  rule " << rule << " <=>  reverse " << reverse << " )";
		cout << endl;
	}
}
;

class NSIteratorNEIGHRemoveSingleInput: public NSIterator<RepEFP, OPTFRAME_DEFAULT_ADS>
{
private:
	MoveNEIGHRemoveSingleInput* m;
	vector<MoveNEIGHRemoveSingleInput*> moves;
	int index;
	const RepEFP& rep;

public:
	NSIteratorNEIGHRemoveSingleInput(const RepEFP& _rep) :
			rep(_rep)
	{
		index = 0;
		m = NULL;
	}

	virtual ~NSIteratorNEIGHRemoveSingleInput()
	{
		for (int i = index + 1; i < moves.size(); i++)
			delete moves[i];
	}

	virtual void first()
	{

		for (int rule = 0; rule < rep.singleIndex.size(); rule++)
		{
			moves.push_back(new MoveNEIGHRemoveSingleInput(rule, false));
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
			cout << "NSIteratorNEIGHRemoveSingleInput. Aborting." << endl;
			exit(1);
		}

		return *m;
	}

};

class NSSeqNEIGHRemoveSingleInput: public NSSeq<RepEFP>
{
private:
	RandGen& rg;

public:

	using NSSeq<RepEFP>::move; // prevents name hiding

	NSSeqNEIGHRemoveSingleInput(RandGen& _rg) :
			rg(_rg)
	{
	}

	virtual ~NSSeqNEIGHRemoveSingleInput()
	{
	}

	virtual Move<RepEFP, OPTFRAME_DEFAULT_ADS>& move(const RepEFP& rep, const OPTFRAME_DEFAULT_ADS&)
	{

		int rule = -1;
		if (rep.singleIndex.size() > 0)
			rule = rg.rand(rep.singleIndex.size());

		return *new MoveNEIGHRemoveSingleInput(rule, false); // return a random move
	}

	virtual NSIterator<RepEFP, OPTFRAME_DEFAULT_ADS>& getIterator(const RepEFP& rep, const OPTFRAME_DEFAULT_ADS&)
	{
		return *new NSIteratorNEIGHRemoveSingleInput(rep); // return an iterator to the neighbors of 'rep'
	}

	virtual string toString() const
	{
		stringstream ss;
		ss << "NSSeqNEIGHRemoveSingleInput";
		return ss.str();
	}
};

}
#endif /*EFP_NSSEQREMOVESINGLEINPUT_HPP_*/

