#ifndef BBV_NSSEQBuySellPaper_H_
#define BBV_NSSEQBuySellPaper_H_

// Framework includes
#include "../../OptFrame/NSSeq.hpp"
#include "../../OptFrame/RandGen.hpp"

// Own includes
#include "ProblemInstance.h"
#include "Solution.h"

using namespace std;

namespace BBV
{

class MoveBuySellPaper: public Move< RepBBV , MY_ADS  >
{
private:
    // MOVE PARAMETERS

public:
    using Move< RepBBV , MY_ADS  >::apply; // prevents name hiding
    using Move< RepBBV , MY_ADS  >::canBeApplied; // prevents name hiding

    MoveBuySellPaper() // ADD PARAMETERS
    {
    }

    virtual ~MoveBuySellPaper()
    {
    }
    
    void print() const
    {
        cout << id() << " with params: '" << "ADD MY PARAMETERS" << "'" << endl;
    }
    
    string id() const
    {
        return Move<RepBBV , MY_ADS >::idComponent().append(":MoveBuySellPaper");
    }
    
    bool operator==(const Move< RepBBV , MY_ADS  >& _m) const
    {
        const MoveBuySellPaper& m = (const MoveBuySellPaper&) _m;
        // COMPARE PARAMETERS AND RETURN TRUE IF EQUALS
        return false;
    }
    
    // Implement these methods in the .cpp file
    
    bool canBeApplied(const RepBBV& rep, const MY_ADS&);

    Move< RepBBV , MY_ADS  >* apply(RepBBV& rep, MY_ADS&);
    
    MoveCost* cost(const Evaluation&, const RepBBV& rep, const MY_ADS& ads);
};



class NSIteratorBuySellPaper: public NSIterator< RepBBV , MY_ADS  >
{
private:
    // ITERATOR PARAMETERS

public:
    NSIteratorBuySellPaper() // ADD ITERATOR PARAMETERS
    {
    }

    virtual ~NSIteratorBuySellPaper()
    {
    }
    
    // Implement these methods in the .cpp file

    void first();
    void next();
    bool isDone();
    Move< RepBBV , MY_ADS  >& current();
};



class NSSeqBuySellPaper: public NSSeq< RepBBV , MY_ADS  >
{
private:
    // YOU MAY REMOVE THESE PARAMETERS IF YOU DON'T NEED (BUT PROBABLY WILL...)
    ProblemInstanceBBV& pBBV; // problem instance data
    RandGen& rg;                // random number generator

public:

    using NSSeq< RepBBV , MY_ADS  >::move; // prevents name hiding

    // YOU MAY REMOVE THESE PARAMETERS IF YOU DON'T NEED (BUT PROBABLY WILL...)
    NSSeqBuySellPaper(ProblemInstanceBBV& _pBBV, RandGen& _rg):
        pBBV(_pBBV), rg(_rg)
    {
    }

    virtual ~NSSeqBuySellPaper()
    {
    }
    
    void print() const
    {
        cout << "NSSeqBuySellPaper" << endl;
    }
    
    string id() const
    {
        return NSSeq<RepBBV , MY_ADS >::idComponent().append(":NSSeqBuySellPaper");
    }
    
    NSIterator<RepBBV , MY_ADS >& getIterator(const RepBBV& rep, const MY_ADS&)
    {
        // return an iterator to the neighbors of 'rep' 
        return * new NSIteratorBuySellPaper;  // ADD POSSIBLE ITERATOR PARAMETERS
    }
        
    // Implement this method in the .cpp file

    Move<RepBBV , MY_ADS >& move(const RepBBV& rep, const MY_ADS&);
};

}

#endif /*BBV_NSSEQBuySellPaper_H_*/
