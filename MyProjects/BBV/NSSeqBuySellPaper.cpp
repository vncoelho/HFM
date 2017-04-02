#include "NSSeqBuySellPaper.h"

using namespace BBV;

// ============ MoveBuySellPaper ==============

bool MoveBuySellPaper::canBeApplied(const RepBBV& rep, const MY_ADS&)
{
    return true;
}

Move< RepBBV , MY_ADS  >* MoveBuySellPaper::apply(RepBBV& rep, MY_ADS&)
{
    // apply this move to 'rep'
    // rep. (...) = (...)
    // return reverse move
    return new MoveBuySellPaper; 
}

MoveCost* MoveBuySellPaper::cost(const Evaluation&, const RepBBV& rep, const MY_ADS& ads)
{
   return NULL;
   // Implement if a more efficient evaluation of the move is available
   //double move_cost = ...;
   //double move_cost_infeasible = ...;
   //return new MoveCost(move_cost, move_cost_infeasible);
}



// ============ NSIteratorBuySellPaper ==============

void NSIteratorBuySellPaper::first(){};

void NSIteratorBuySellPaper::next(){};
	
bool NSIteratorBuySellPaper::isDone(){};
	
Move< RepBBV , MY_ADS  >& NSIteratorBuySellPaper::current(){};




// ============ NSSeqBuySellPaper ==============


Move<RepBBV , MY_ADS >& NSSeqBuySellPaper::move(const RepBBV& rep, const MY_ADS&)
{
   // return a random move (that is, a move operator that generates a neighbor solution of 'rep')
   // you may need to use the random number generator 'rg'
   
   return * new MoveBuySellPaper; 
}
