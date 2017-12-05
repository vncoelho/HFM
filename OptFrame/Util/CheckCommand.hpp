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

#ifndef OPTFRAME_CHECK_COMMAND_HPP_
#define OPTFRAME_CHECK_COMMAND_HPP_

#include "../Timer.hpp"
#include "Matrix.hpp"

#include "../OptFrameList.hpp"

#include "../Evaluator.hpp"
#include "../Constructive.hpp"
#include "../NS.hpp"
#include "../NSSeq.hpp"
#include "../NSEnum.hpp"

namespace optframe
{

template<class R, class ADS = OPTFRAME_DEFAULT_ADS>
class CheckCommand
{

	static const int CMERR_EV_BETTERTHAN = 2001;
	static const int CMERR_EV_BETTEREQUALS = 2002;
	static const int CMERR_MOVE_EQUALS = 3001;
	static const int CMERR_MOVE_HASREVERSE = 3002;
	static const int CMERR_MOVE_REVREV_VALUE = 3004;
	static const int CMERR_MOVE_REVSIMPLE = 3005;
	static const int CMERR_MOVE_REVFASTER = 3006;
	static const int CMERR_MOVE_REALREVFASTER = 3008;
	static const int CMERR_MOVE_COST = 3007;

private:
	bool verbose;

	bool convertNS;

	// independent moves
	bool checkIndependent;

	vector<Evaluator<R, ADS>*> lEvaluator;
	vector<Constructive<R, ADS>*> lConstructive;
	vector<Solution<R, ADS>*> lSolution;
	vector<Move<R, ADS>*> lMove;
	vector<NS<R, ADS>*> lNS;
	vector<NSSeq<R, ADS>*> lNSSeq;
	vector<NSEnum<R, ADS>*> lNSEnum;

	ADSManager<R, ADS>* adsMan;

public:

	vector<ADSManager<R, ADS>*> lADSManagerComp; // optional

	CheckCommand(bool _verbose = false) :
			verbose(_verbose)
	{
		convertNS = true;
		adsMan = NULL;
		checkIndependent = true;
	}

	virtual ~CheckCommand()
	{
	}

	void add(Constructive<R, ADS>& c)
	{
		lConstructive.push_back(&c);
		if (verbose)
			cout << "checkcommand: Constructive " << lConstructive.size() << " added!" << endl;
	}

	void add(ADSManager<R, ADS>& adsMan)
	{
		lADSManagerComp.push_back(&adsMan);
		if (verbose)
			cout << "checkcommand: AdsMan " << lADSManagerComp.size() << " added!" << endl;
	}

	void add(Evaluator<R, ADS>& c)
	{
		lEvaluator.push_back(&c);
		if (verbose)
			cout << "checkcommand: Evaluator " << lEvaluator.size() << " added!" << endl;
	}

	void add(Solution<R, ADS>& c)
	{
		lSolution.push_back(&c);
		if (verbose)
			cout << "checkcommand: Solution " << lSolution.size() << " added!" << endl;
	}

	void add(Move<R, ADS>& c)
	{
		lMove.push_back(&c);
		if (verbose)
			cout << "checkcommand: Move " << lMove.size() << " added!" << endl;
	}

	void add(NS<R, ADS>& c)
	{
		lNS.push_back(&c);
		if (verbose)
			cout << "checkcommand: NS " << lNS.size() << " added!" << endl;
	}

	void add(NSSeq<R, ADS>& c)
	{
		lNSSeq.push_back(&c);
		if (verbose)
			cout << "checkcommand: NSSeq " << lNSSeq.size() << " added!" << endl;
		if (convertNS)
			add((NS<R, ADS>&) c);
	}

	void add(NSEnum<R, ADS>& c)
	{
		lNSEnum.push_back(&c);
		if (verbose)
			cout << "checkcommand: NSEnum " << lNSEnum.size() << " added!" << endl;
		if (convertNS)
			add((NSSeq<R, ADS>&) c);
	}

	void message(Component* c, int iter, string text)
	{
		message(c->id(), iter, text);
	}

	void message(string component, int iter, string text)
	{
		if (verbose)
			cout << "checkcommand iter: " << iter << " testing component '" << component << "' => " << text << endl;
	}

	void error(string text)
	{
		cout << "checkcommand error: " << text << endl;
	}

	void errormsg(string component, int code, string scode, int iter, string text)
	{
		cout << "checkcommand ERROR " << code << " (" << scode << "): iter=" << iter << " testing component '" << component << "' MSG: " << text << endl;
	}

	bool parseBool(string b)
	{
		return b == "true";
	}

	struct TimeCheckNS
	{
		vector<pair<int, double> > timeNSApply;
		vector<pair<int, double> > timeNSCostApply;
		vector<pair<int, double> > timeNSCostApplyDelta;
		vector<pair<int, double> > timeNSCostApplyRealDelta;
		vector<pair<int, double> > timeNSCost;
		vector<pair<int, double> > timeNSEstimatedCost;
		vector<pair<int, double> > errorNSEstimatedCost;
		bool overestimate, underestimate;
	};

	struct TimeCheckSol
	{
		pair<int, double> timeCloneSolution;
		vector<pair<int, double> > timeInitializeADS;
		vector<pair<int, double> > fullTimeEval;
		vector<pair<int, double> > timeReeval;
	};

	bool testMoveGeneral(int iter, NS<R, ADS>* ns, int id_ns, Solution<R, ADS>& s, int id_s, Move<R, ADS>& move, vector<vector<Evaluation*> >& evaluations, TimeCheckSol& timeSol, TimeCheckNS& timeNS)
	{
		for (unsigned ev = 0; ev < lEvaluator.size(); ev++)
		{
			message(lEvaluator.at(ev), iter, "evaluating random move (apply, revert and moveCost).");

			Evaluation e = lEvaluator[ev]->evaluateSolution(s);

			string moveFrom = "Move ";
			moveFrom.append(move.id());
			// not printing NS for single Move test
			if (ns)
			{
				moveFrom.append(" from: ");
				moveFrom.append(ns->id());
				moveFrom.append(" toString: ");
				moveFrom.append(ns->toString());
			}

			if (verbose)
				move.print();

			message(moveFrom, iter, "testing reverse.");

			Timer t_clone;
			Solution<R, ADS>& sOriginal = s.clone(); // remove if not verbose
			timeSol.timeCloneSolution.second += t_clone.inMilliSecs();
			timeSol.timeCloneSolution.first++;

			Timer tMovApply;
			Move<R, ADS>* rev = move.applySolution(s);
			if ((!move.hasReverse() && rev) || (move.hasReverse() && !rev))
			{
				errormsg(moveFrom, CMERR_MOVE_HASREVERSE, "CMERR_MOVE_HASREVERSE", iter, " conflict between apply result and hasReverse()");
				return false;
			}

			timeNS.timeNSApply[id_ns].second += tMovApply.inMilliSecs();
			timeNS.timeNSApply[id_ns].first++;

			Timer t_clone2;
			Solution<R, ADS>& sNeighbor = s.clone(); // remove if not verbose
			timeSol.timeCloneSolution.second += t_clone2.inMilliSecs();
			timeSol.timeCloneSolution.first++;

			// ===================== tests with ADSManager ======================

			// DEPRECATED ADSManager
#if 1
			if (adsMan)
			{
				message(lEvaluator.at(ev), -1, "testing ADS.");

				ADS ads(sNeighbor.getADS()); // copy
				Timer ts_ds;
				adsMan->initializeADS(sNeighbor.getR(), sNeighbor.getADS());
				timeSol.timeInitializeADS[0].second += ts_ds.inMilliSecs();
				timeSol.timeInitializeADS[0].first++;

				if (!adsMan->compareADS(ads, sNeighbor.getADS()))
				{
					cout << "checkcommand error: ADS not updated correctly! Compared brand new initializeADS with update from move => ";
					move.print();
					cout << "S (sOriginal.getADS()): " << endl;
					adsMan->printADS(sOriginal.getADS());
					cout << "WRONG: (sNeighbor.getADS())" << endl;
					adsMan->printADS(ads);
					cout << "RIGHT (re-initialized): " << endl;
					adsMan->printADS(sNeighbor.getADS());
					return false;
				}
			}
#endif

			Timer te;
			Evaluation e_rev = lEvaluator.at(ev)->evaluateSolution(s);
			timeSol.fullTimeEval[ev].second += te.inMilliSecs();
			timeSol.fullTimeEval[ev].first++;

			Timer tMovRevApply;
			Move<R, ADS>* ini = NULL;
			if (rev)
				ini = rev->applySolution(s);
			timeNS.timeNSApply[id_ns].second += tMovRevApply.inMilliSecs();
			timeNS.timeNSApply[id_ns].first++;

			// ===================== tests with ADSManager ======================

#if 1
			if (adsMan)
			{
				message(lEvaluator.at(ev), -1, "testing ADS.");

				ADS ads(s.getADS()); // copy
				Timer ts_ds2;
				adsMan->initializeADS(s.getR(), s.getADS());
				timeSol.timeInitializeADS[0].second += ts_ds2.inMilliSecs();
				timeSol.timeInitializeADS[0].first++;

				if (!adsMan->compareADS(ads, s.getADS()))
				{
					cout << "checkcommand error: ADS not updated correctly! Compared brand new initializeADS with update from reverse move => ";
					Component::safe_print(rev);
					cout << "S (sOriginal.getADS()): " << endl;
					adsMan->printADS(sOriginal.getADS());
					cout << "WRONG (s.getADS()): " << endl;
					adsMan->printADS(ads);
					cout << "RIGHT (re-initialized): " << endl;
					adsMan->printADS(s.getADS());
					return false;
				}
			}
#endif

			// go back by copy (if necessary!)
			if (!rev)
				s = sOriginal;

			delete &sOriginal;

			Timer te2;
			Evaluation e_ini = lEvaluator.at(ev)->evaluateSolution(s);
			timeSol.fullTimeEval[ev].second += te2.inMilliSecs();
			timeSol.fullTimeEval[ev].first++;

			if (ini && (*ini != move))
			{
				errormsg(moveFrom, CMERR_MOVE_EQUALS, "CMERR_MOVE_EQUALS", iter, "reverse of reverse is not the original move!");
				move.print();
				cout << "move: ";
				move.print();
				cout << "rev: ";
				Component::safe_print(rev);
				cout << "ini (reverse of rev): ";
				Component::safe_print(ini);

				return false;
			}

			message(lEvaluator.at(ev), iter, "testing reverse value.");

			if (EVALUATION_ABS(e_ini.evaluation() - e.evaluation()) >= EVALUATION_ZERO)
			{
				errormsg(moveFrom, CMERR_MOVE_REVREV_VALUE, "CMERR_MOVE_REVREV_VALUE", iter, "reverse of reverse has a different evaluation value!");
				move.print();
				cout << "move: ";
				move.print();
				cout << "original: ";
				e.print();
				cout << "reverse of reverse:";
				e_ini.print();

				return false;
			}

			// =============================================================

			message(lEvaluator.at(ev), iter, "testing move cost.");

			// ==============

			evtype revCost = e_rev.evaluation() - e.evaluation();
			message(lEvaluator.at(ev), iter, "revCost calculated!");

			Timer tMoveCostApply;
			MoveCost* mcSimpleCost = lEvaluator[ev]->moveCostComplete(move, s);
			evtype simpleCost = mcSimpleCost->cost();
			delete mcSimpleCost;
			message(lEvaluator.at(ev), iter, "simpleCost calculated!");
			timeNS.timeNSCostApply[id_ns].second += tMoveCostApply.inMilliSecs();
			timeNS.timeNSCostApply[id_ns].first++;

			if (EVALUATION_ABS(revCost - simpleCost) >= EVALUATION_ZERO)
			{
				errormsg(moveFrom, CMERR_MOVE_REVSIMPLE, "CMERR_MOVE_REVSIMPLE", iter, "difference between revCost and simpleCost");
				cout << "move: ";
				move.print();
				cout << "revCost = " << revCost << endl;
				cout << "simpleCost = " << simpleCost << endl;
				return false;
			}

			// ==============
			// fasterCost
			Timer tMoveCostApplyDelta;
			Move<R, ADS>& rev1 = *lEvaluator[ev]->applyMoveReevaluate(e, move, s);
			evtype e_end1 = e.evaluation();
			// TODO: check outdated status
			// TODO: if(e.outdated), it means that user did not implement Move::applyMoveReevaluate(e,R,ADS)!
			Move<R, ADS>& ini1 = *lEvaluator[ev]->applyMoveReevaluate(e, rev1, s);
			evtype e_ini1 = e.evaluation();
			timeNS.timeNSCostApplyDelta[id_ns].second += tMoveCostApplyDelta.inMilliSecs();
			timeNS.timeNSCostApplyDelta[id_ns].first++;

			delete &rev1;
			delete &ini1;

			evtype fasterCost = e_end1 - e_ini1;
			message(lEvaluator.at(ev), iter, "fasterCost calculated!");

			if (EVALUATION_ABS(revCost - fasterCost) >= EVALUATION_ZERO)
			{
				errormsg(moveFrom, CMERR_MOVE_REVFASTER, "CMERR_MOVE_REVFASTER", iter, "difference between revCost and fasterCost!");
				cout << "move: ";
				move.print();
				cout << "revCost = " << revCost << endl;
				cout << "fasterCost = " << fasterCost << endl;
				cout << "e = " << e.evaluation() << endl;
				cout << "e_rev = " << e_rev.evaluation() << endl;
				return false;
			}

			// ==============

			// ==============
			// real fasterCost
			Timer tMoveCostApplyRealDelta;
			bool oldAllowCostsStatus = lEvaluator[ev]->getAllowCosts();
			lEvaluator[ev]->setAllowCosts(false);
			MoveCost* mcRealFasterCost = lEvaluator[ev]->moveCost(e, move, s);
			lEvaluator[ev]->setAllowCosts(oldAllowCostsStatus);
			evtype realFasterCost = mcRealFasterCost->cost();
			delete mcRealFasterCost;
			message(lEvaluator.at(ev), iter, "realFasterCost calculated!");
			timeNS.timeNSCostApplyRealDelta[id_ns].second += tMoveCostApplyRealDelta.inMilliSecs();
			timeNS.timeNSCostApplyRealDelta[id_ns].first++;
			message(lEvaluator.at(ev), iter, "realfasterCost calculated!");

			if (EVALUATION_ABS(revCost - realFasterCost) >= EVALUATION_ZERO)
			{
				errormsg(moveFrom, CMERR_MOVE_REALREVFASTER, "CMERR_MOVE_REALREVFASTER", iter, "difference between revCost and realfasterCost!");
				cout << "move: ";
				move.print();
				cout << "revCost = " << revCost << endl;
				cout << "simpleCost = " << realFasterCost << endl;
				return false;
			}
			// ==============

			Timer tMoveCost;
			MoveCost* cost = NULL;

			if (lEvaluator[ev]->getAllowCosts())
				cost = move.cost(e, s.getR(), s.getADSptr(), false);

			message(lEvaluator.at(ev), iter, "cost() calculated!");

			if (cost && !cost->isEstimated())
			{
				timeNS.timeNSCost[id_ns].second += tMoveCost.inMilliSecs();
				timeNS.timeNSCost[id_ns].first++;
			}

			if (cost && cost->isEstimated())
			{
				timeNS.timeNSEstimatedCost[id_ns].second += tMoveCost.inMilliSecs();
				timeNS.timeNSEstimatedCost[id_ns].first++;
				if (cost->cost() > revCost)
					timeNS.overestimate = true;
				if (cost->cost() < revCost)
					timeNS.underestimate = true;
			}

			if (cost && !cost->isEstimated())
			{
				double cValue = cost->cost();
				if (EVALUATION_ABS(revCost - cValue) >= EVALUATION_ZERO)
				{
					errormsg(moveFrom, CMERR_MOVE_COST, "CMERR_MOVE_COST", iter, "difference between expected cost and cost()");
					cout << "move: ";
					move.print();
					cout << "expected cost: (e' - e) =\t" << revCost << endl;
					cout << "cost() =\t" << cValue << endl;
					cout << "e: \t obj:" << e.getObjFunction() << "\t inf: " << e.getInfMeasure() << " \t total:" << e.evaluation() << endl;
					cout << "e':\t obj:" << e_rev.getObjFunction() << "\t inf:" << e_rev.getInfMeasure() << " \t total:" << e_rev.evaluation() << endl;
					cout << "e+cost():\t obj:" << e.getObjFunction() + cost->getObjFunctionCost() << "\t inf:" << e.getInfMeasure() + cost->getInfMeasureCost() << "\t total:" << e.evaluation() + cost->cost() << endl;
					cout << "s: ";
					s.print();
					cout << "s': ";
					sNeighbor.print();
					cout << "move: ";
					move.print();
					cout << "==============" << endl;
					cout << "  GOOD LUCK!  " << endl;
					cout << "==============" << endl;
					return false;
				}

				// testing double move costs! (for MoveCost betterThan)
				if (ns) // not testing for single Move
				{
					// TODO: consider that randomMove can be null
					Move<R, ADS>& move2 = *ns->randomMoveSolution(s);
					if (verbose)
					{
						cout << "testing double move!" << endl;
						move2.print();
					}

					if (!move2.canBeAppliedToSolution(s))
					{
						if (verbose)
						{
							cout << "double move cannot be applied: ";
							move2.print();
						}
					}
					else
					{
						MoveCost* cost2 = NULL;
						if (lEvaluator[ev]->getAllowCosts())
						{
							cost2 = move2.cost(e, s.getR(), s.getADSptr(), false);
							if (cost2)
							{
								lEvaluator[ev]->betterThan(*cost, *cost2);
								delete cost2;
							}
						}
					}

					delete &move2;
				}
				// finish double cost test

				delete cost;
			}

			message(lEvaluator.at(ev), iter, "all move costs okay!");

			if (rev)
				delete rev;
			delete &sNeighbor;
			if (ini)
				delete ini;
		}

		//delete &move;  // ONLY IF NECESSARY! DO IT OUTSIDE...

		return true;
	}

	bool run(int iterMax, int nSolNSSeq)
	{
		// ======================================
		//           BEGIN TESTS
		// ======================================

		cout << "---------------------------------------" << endl;
		cout << "tests=" << iterMax << " tests(NSSeq)=" << nSolNSSeq << endl;
		cout << "---------------------------------------" << endl;
		cout << "evaluators=" << lEvaluator.size() << endl;
		cout << "constructives=" << lConstructive.size() << endl;
		cout << "moves=" << lMove.size() << endl;
		cout << "ns=" << lNS.size() << endl;
		cout << "nsseq=" << lNSSeq.size() << endl;
		cout << "nsenum=" << lNSEnum.size() << endl;
		// ADSManager is deprecated!
		cout << "DEPRECATED:adsmanager=" << lADSManagerComp.size() << endl;
		cout << "---------------------------------------" << endl << endl;

		// ----------------
		// read evaluators
		// ----------------

		vector<Evaluator<R, ADS>*> evaluators;
		for (unsigned ev = 0; ev < lEvaluator.size(); ev++)
			evaluators.push_back(lEvaluator[ev]);

		TimeCheckSol timeSol;
		// time to clone a solution
		timeSol.timeCloneSolution = pair<int, double>(0, 0.0);

		adsMan = NULL;
		if (lADSManagerComp.size() > 0)
		{
			adsMan = lADSManagerComp[0];

			if (lADSManagerComp.size() > 1)
				cout << " checkcommand warning: more than 1 ADSManager (" << lADSManagerComp.size() << ")" << endl;
		}

		timeSol.timeInitializeADS = vector<pair<int, double> >(1, make_pair(0, 0.0));
		timeSol.fullTimeEval = vector<pair<int, double> >(evaluators.size(), make_pair(0, 0.0));
		timeSol.timeReeval = vector<pair<int, double> >(evaluators.size(), make_pair(0, 0.0));

		TimeCheckNS timeNS;
		timeNS.timeNSApply = vector<pair<int, double> >(lNS.size(), make_pair(0, 0.0));
		timeNS.timeNSCostApply = vector<pair<int, double> >(lNS.size(), make_pair(0, 0.0));
		timeNS.timeNSCostApplyDelta = vector<pair<int, double> >(lNS.size(), make_pair(0, 0.0));
		timeNS.timeNSCostApplyRealDelta = vector<pair<int, double> >(lNS.size(), make_pair(0, 0.0));
		timeNS.timeNSCost = vector<pair<int, double> >(lNS.size(), make_pair(0, 0.0));
		timeNS.timeNSEstimatedCost = vector<pair<int, double> >(lNS.size(), make_pair(0, 0.0));
		timeNS.errorNSEstimatedCost = vector<pair<int, double> >(lNS.size(), make_pair(0, 0.0));
		timeNS.overestimate = timeNS.underestimate = false;

		// ----------------------------------------------------------------------------------------
		// generate 'iterMax' OptFrame:Solution for each OptFrame:Constructive and store evaluation
		// ----------------------------------------------------------------------------------------

		vector<Solution<R, ADS>*> solutions;
		vector<vector<Evaluation*> > evaluations(evaluators.size());

		vector<pair<int, double> > timeConstructive(lConstructive.size(), make_pair(0, 0.0));

		if (lConstructive.size() > 0)
			cout << "checkcommand  will test " << lConstructive.size() << " constructive components (iterMax=" << iterMax << ")" << endl;
		for (unsigned c = 0; c < lConstructive.size(); c++)
		{
			Constructive<R, ADS>* constructive = lConstructive.at(c);

			cout << "checkcommand: testing Constructive " << c << " => " << constructive->toString();
			cout << endl;

			for (int iter = 1; iter <= iterMax; iter++)
			{
				message(lConstructive.at(c), iter, "generating solution.");

				Timer ts;
				Solution<R, ADS> s = *constructive->generateSolution(10000000);
				timeConstructive[c].second += ts.inMilliSecs();
				timeConstructive[c].first++;

				if (adsMan)
				{
					Timer ts2;
					adsMan->initializeADS(s.getR(), s.getADS());
					timeSol.timeInitializeADS[0].second += ts2.inMilliSecs();
					timeSol.timeInitializeADS[0].first++;
				}

				solutions.push_back(new Solution<R, ADS>(s));

				for (unsigned ev = 0; ev < evaluators.size(); ev++)
				{
					message(lEvaluator.at(ev), iter, "evaluating solution.");
					Timer te;
					Evaluation e = evaluators.at(ev)->evaluateSolution(s);
					timeSol.fullTimeEval[ev].second += te.inMilliSecs();
					timeSol.fullTimeEval[ev].first++;

					evaluations.at(ev).push_back(new Evaluation(e));

					if (lEvaluator.at(ev)->betterThan(e, e))
					{
						errormsg(lEvaluator.at(ev)->toString(), CMERR_EV_BETTERTHAN, "CMERR_EV_BETTERTHAN", iter, "error in betterThan(X,X)=true");
						return false;
					}

					if (!lEvaluator.at(ev)->betterOrEquals(e, e))
					{
						errormsg(lEvaluator.at(ev)->toString(), CMERR_EV_BETTEREQUALS, "CMERR_EV_BETTEREQUALS", iter, "error in betterOrEquals(X,X)=false");
						return false;
					}
				}
			}

			cout << "checkcommand: Constructive " << c << " => " << lConstructive.at(c)->id() << " finished." << endl;
			if (verbose)
				cout << endl << endl;
		}

		if (verbose)
			cout << endl << endl;

		// ====================================================================
		// testing Move
		// ====================================================================

		if (lMove.size() > 0)
			cout << "checkcommand  will test given Move components (|Move|=" << lMove.size() << "; numSolutions=" << solutions.size() << ")" << endl;

		for (unsigned p = 0; p < lSolution.size(); p++)
		{
			solutions.push_back(&lSolution[p]->clone());

			for (unsigned ev = 0; ev < evaluators.size(); ev++)
			{
				message(lEvaluator.at(ev), p, "evaluating input solution.");
				Timer te;
				Evaluation e = evaluators.at(ev)->evaluateSolution(*lSolution[p]);
				timeSol.fullTimeEval[ev].second += te.inMilliSecs();
				timeSol.fullTimeEval[ev].first++;

				evaluations.at(ev).push_back(new Evaluation(e));
			}
		}

		for (unsigned id_move = 0; id_move < lMove.size(); id_move++)
		{
			Move<R, ADS>* pmove = lMove.at(id_move);

			cout << "checkcommand: testing Move " << id_move << " => " << pmove->toString() << endl;

			for (unsigned id_s = 0; id_s < solutions.size(); id_s++)
			{
				if (verbose)
					cout << endl;
				message(lMove.at(id_move), -1, "working on move.");

				Solution<R, ADS>& s = *solutions.at(id_s);

				Move<R, ADS>& move = *pmove;

				if (!move.canBeAppliedToSolution(s))
				{
					if (verbose)
					{
						cout << "move cannot be applied: ";
						move.print();
					}
					continue;
				}

				// 	bool testMoveGeneral(int iter, NS<R,ADS>* ns, int id_ns, Solution<R, ADS>& s, int id_s, Move<R, ADS>& move, vector<vector<Evaluation*> >& evaluations, TimeCheckSol& timeSol, TimeNS& timeNS)

				if (!testMoveGeneral(-1, NULL, -1, s, id_s, move, evaluations, timeSol, timeNS))
					return false;

				/////delete& move; // TODO NEVER DESTROY THIS OptFrame:Move!
			}

		}

		if (verbose)
			cout << endl << endl;

		// ====================================================================
		// testing NS
		// ====================================================================

		if (lNS.size() > 0)
			cout << "checkcommand  will test " << lNS.size() << " NS components (iterMax=" << iterMax << " Solutions=" << solutions.size() << ")" << endl;

		for (unsigned id_ns = 0; id_ns < lNS.size(); id_ns++)
		{
			NS<R, ADS>* ns = lNS.at(id_ns);

			cout << "checkcommand: testing NS " << id_ns << " => " << ns->toString();
			cout << endl;

			for (int iter = 1; iter <= iterMax; iter++)
			{
				message(lNS.at(id_ns), iter, "starting tests!");

				for (unsigned id_s = 0; id_s < solutions.size(); id_s++)
				{
					if (verbose)
						cout << endl;

					stringstream ss_msg1;
					ss_msg1 << "generating random move for solution id=" << id_s;
					message(lNS.at(id_ns), iter, ss_msg1.str());

					Solution<R, ADS>& s = *solutions.at(id_s);

					// TODO: consider that randomMove can be null
					Move<R, ADS>& move = *ns->randomMoveSolution(s);
					if (verbose)
						move.print();

					if (!move.canBeAppliedToSolution(s))
					{
						if (verbose)
						{
							cout << "move cannot be applied: ";
							move.print();
						}

						delete &move;
						continue;
					}

					// EXEC TESTS HERE

					// bool testMoveNS(int iter, NS<R,ADS>* ns, int id_ns, Solution<R, ADS>& s, int id_s, Move<R, ADS>& move, vector<vector<Evaluation*> >& evaluations, pair<int, double>& timeCloneSolution, vector<pair<int, double> >& timeInitializeADS, vector<pair<int, double> >& fullTimeEval, vector<pair<int, double> >& timeReeval, TimeNS& timeNS)
					// bool testMoveGeneral(int iter, NS<R,ADS>* ns, int id_ns, Solution<R, ADS>& s, int id_s, Move<R, ADS>& move, vector<vector<Evaluation*> >& evaluations, TimeCheckSol& timeSol, TimeNS& timeNS)

					if (!testMoveGeneral(iter, ns, id_ns, s, id_s, move, evaluations, timeSol, timeNS))
					{
						delete &move;
						return false;
					}

					delete &move;
				}
			}

			cout << "checkcommand: " << lNS.at(id_ns)->id() << " finished." << endl;
			if (verbose)
				cout << endl << endl;
		}

		// ====================================================================
		// testing NSSeq
		// ====================================================================

		if (lNSSeq.size() > 0)
			cout << "checkcommand  will test " << lNSSeq.size() << " NSSeq components (nSolNSSeq=" << nSolNSSeq << " of numSolutions=" << solutions.size() << ")" << endl;

		vector<int> vCountMoves(lNSSeq.size());
		vector<int> vCountValidMoves(lNSSeq.size());

		for (unsigned id_nsseq = 0; id_nsseq < lNSSeq.size(); id_nsseq++)
		{
			NSSeq<R, ADS>* nsseq = lNSSeq.at(id_nsseq);

			cout << "checkcommand: testing NSSeq " << id_nsseq << " => " << nsseq->toString();
			cout << endl;

			int countMoves = 0;
			int countValidMoves = 0;

			for (int nqs = 1; nqs <= nSolNSSeq; nqs++)
			{
				message(lNSSeq.at(id_nsseq), nqs, "starting tests!");

				int randomIndex = rand() % solutions.size();
				Solution<R, ADS>& s = *solutions.at(randomIndex);
				int id_s = randomIndex;

				// ===================
				// apply a single move
				// ===================

				// TODO: consider that iterator can be null!
				NSIterator<R, ADS>& it = *nsseq->getIteratorSolution(s);

				for (it.first(); !it.isDone(); it.next())
				{
					if (verbose)
						cout << endl;
					message(lNSSeq.at(id_nsseq), nqs, "getting current move (NSSeq tests).");

					// TODO: verify if it's not null!
					Move<R, ADS>& move = *it.current();
					countMoves++;

					if (!move.canBeAppliedToSolution(s))
					{
						if (verbose)
						{
							cout << "move cannot be applied (NSSeq tests): ";
							move.print();
						}
						delete &move;
						continue;
					}

					countValidMoves++;

					// EXEC MOVES HERE

					//	bool testMoveNSSeq(int iter, NSSeq<R,ADS>* nsseq, int nqs, int id_nsseq, Solution<R, ADS>& s, int id_s, Move<R, ADS>& move, vector<vector<Evaluation*> >& evaluations, pair<int, double>& timeCloneSolution, vector<pair<int, double> >& timeInitializeADS, vector<pair<int, double> >& fullTimeEval, vector<pair<int, double> >& timeReeval, TimeNS& timeNS)
					// 	bool testMoveGeneral(int iter, NS<R,ADS>* ns, int id_ns, Solution<R, ADS>& s, int id_s, Move<R, ADS>& move, vector<vector<Evaluation*> >& evaluations, TimeCheckSol& timeSol, TimeNS& timeNS)

					if (!testMoveGeneral(nqs, nsseq, id_nsseq, s, id_s, move, evaluations, timeSol, timeNS))
					{
						delete &move;
						return false;
					}

					delete &move;
				}

				delete &it;
			}

			vCountMoves[id_nsseq] += countMoves;
			vCountValidMoves[id_nsseq] += countValidMoves;

			cout << "checkcommand: " << lNSSeq.at(id_nsseq)->id() << " finished." << endl;
			if (verbose)
				cout << endl << endl;
		}

		// ====================================================================
		// testing NSEnum
		// ====================================================================

		if (lNSEnum.size() > 0)
			cout << "checkcommand  will test " << lNSEnum.size() << " NSEnum components (nSolNSSeq=" << nSolNSSeq << " of numSolutions=" << solutions.size() << ")" << endl;

		vector<int> vCountMovesEnum(lNSEnum.size());
		vector<int> vCountValidMovesEnum(lNSEnum.size());

		vector<int> vCountIndependentEnum(lNSEnum.size());
		vector<int> vCountMovePairsEnum(lNSEnum.size());

		for (unsigned id_nsenum = 0; id_nsenum < lNSEnum.size(); id_nsenum++)
		{
			NSEnum<R, ADS>* nsenum = lNSEnum.at(id_nsenum);

			cout << "checkcommand: testing NSEnum " << id_nsenum << " => " << nsenum->toString();
			cout << endl;

			int countMoves = 0;
			int countValidMoves = 0;

			for (int nqs = 1; nqs <= nSolNSSeq; nqs++)
			{
				message(lNSEnum.at(id_nsenum), nqs, "starting tests!");

				int randomIndex = rand() % solutions.size();
				Solution<R, ADS>& s = *solutions.at(randomIndex);
				int id_s = randomIndex;

				// ===================
				// apply a single move
				// ===================

				// TODO: consider that iterator can be null!
				NSIterator<R, ADS>& it = *nsenum->getIteratorSolution(s);

				for (it.first(); !it.isDone(); it.next())
				{
					if (verbose)
						cout << endl;
					message(lNSEnum.at(id_nsenum), nqs, "getting current move (NSEnum tests).");

					// TODO: verify if it's not null!
					Move<R, ADS>& move = *it.current();
					countMoves++;

					if (!move.canBeAppliedToSolution(s))
					{
						if (verbose)
						{
							cout << "move cannot be applied (NSEnum tests): ";
							move.print();
						}
						delete &move;
						continue;
					}

					countValidMoves++;

					// EXEC MOVES HERE

					//	bool testMoveNSSeq(int iter, NSSeq<R,ADS>* nsseq, int nqs, int id_nsseq, Solution<R, ADS>& s, int id_s, Move<R, ADS>& move, vector<vector<Evaluation*> >& evaluations, pair<int, double>& timeCloneSolution, vector<pair<int, double> >& timeInitializeADS, vector<pair<int, double> >& fullTimeEval, vector<pair<int, double> >& timeReeval, TimeNS& timeNS)
					// 	bool testMoveGeneral(int iter, NS<R,ADS>* ns, int id_ns, Solution<R, ADS>& s, int id_s, Move<R, ADS>& move, vector<vector<Evaluation*> >& evaluations, TimeCheckSol& timeSol, TimeNS& timeNS)

					if (!testMoveGeneral(nqs, nsenum, id_nsenum, s, id_s, move, evaluations, timeSol, timeNS))
					{
						delete &move;
						return false;
					}

					delete &move;
				}

				delete &it;
			}

			vCountMovesEnum[id_nsenum] += countMoves;
			vCountValidMovesEnum[id_nsenum] += countValidMoves;

			if (checkIndependent)
			{
				cout << "checkcommand: will try to identify independent moves (can take some time... deactivate with 'checkIndependent=false')" << endl;
				// indicate possible independent moves

				// adopting Evaluator 0...
				Evaluator<R, ADS>& ev = *evaluators.at(0);

				// check for independence between moves m1 and m2
				for (int m1 = 0; m1 < int(nsenum->size()) - 1; m1++)
				{
					// slow...
					cout << "checkcommand: independence test for move " << m1 << " / " << nsenum->size() << endl;
					int count_ind_m1 = 0;
					for (int m2 = m1 + 1; m2 < int(nsenum->size()); m2++)
					{
						bool conflict = false;
						// compute another move pair
						vCountMovePairsEnum[id_nsenum]++;

						if (!conflict)
						{
							// TODO: increase performance of this method
							for (unsigned sol = 0; sol < solutions.size(); sol++)
							{
								Solution<R, ADS>& s = *solutions.at(sol);
								// TODO: verify if return is return is not null!
								Move<R, ADS>& move1 = *nsenum->indexMove(m1);
								Move<R, ADS>& move2 = *nsenum->indexMove(m2);
								// moves must be valid
								if (!(move1.canBeAppliedToSolution(s) && move2.canBeAppliedToSolution(s)))
									break;
								// move 1 must have reverse
								if (!move1.hasReverse())
								{
									cout << "checkcommand: NSEnum independent check expected reverse move... (deactivate with 'checkIndependent=false')" << endl;
									break;
								}

								// calculate cost for move 2
								MoveCost* cost_m2 = ev.moveCostComplete(move2, s);

								// apply move 1 (consider reverse is not NULL)
								Move<R, ADS>& rev_m1 = *move1.applySolution(s);

								// calculate new cost for move 2
								MoveCost* cost2_m2 = ev.moveCostComplete(move2, s);

								// return solution to original value and free
								delete rev_m1.applySolution(s);
								delete &rev_m1;
								delete &move1;
								delete &move2;

								// if costs differ, moves are not independent
								if (!ev.equals(*cost2_m2, *cost_m2))
								{
									// mark conflict between m1 and m2
									conflict = true;
									delete cost2_m2;
									delete cost_m2;
									break;
								}
								delete cost2_m2;
								delete cost_m2;
							}

							if (!conflict)
							{
								// if here, m1 'could' be independent from m2
								vCountIndependentEnum[id_nsenum]++;
								count_ind_m1++;
								if (verbose)
								{
									cout << "independent(m1=" << m1 << ";m2=" << m2 << ")" << endl;
									nsenum->indexMove(m1)->print(); // TODO: fix leak
									nsenum->indexMove(m2)->print(); // TODO: fix leak
									cout << endl;
								}
							}

						}
					}
					cout << "checkcommand: found " << count_ind_m1 << " independent move pairs." << endl;
				}

			}

			cout << "checkcommand: " << lNSEnum.at(id_nsenum)->id() << " finished." << endl;
			if (verbose)
				cout << endl << endl;
		}

		for (unsigned i = 0; i < solutions.size(); i++)
			delete solutions[i];

		for (unsigned i = 0; i < evaluations.size(); i++)
			for (unsigned j = 0; j < evaluations[i].size(); j++)
				delete evaluations[i][j];

		cout << "===============================" << endl;
		cout << "           SUMMARY             " << endl;
		cout << "===============================" << endl << endl;

		printSingleSummary("Solution", timeSol.timeCloneSolution, "Time to clone a solution");

		printSummary(convertVector(lConstructive), timeConstructive, "Constructive", "testing construction of initial solution");

		if (adsMan)
			printSummary(convertVector(lADSManagerComp), timeSol.timeInitializeADS, "ADSManager::initializeADS()", "testing lazy initializeADS in solutions");
		else
			cout << endl << "No ADSManager was tested." << endl << endl;

		printSummary(convertVector(lEvaluator), timeSol.fullTimeEval, "Evaluators", "testing full evaluate(s) of a solution");

		printSummary(convertVector(lNS), timeNS.timeNSApply, "NS", "testing time of move apply(s)");

		printSummary(convertVector(lNS), timeNS.timeNSCostApply, "NS", "testing time of cost based on move apply(s)");

		printSummary(convertVector(lNS), timeNS.timeNSCostApplyDelta, "NS", "testing time of cost based on move apply(e, s)");

		printSummary(convertVector(lNS), timeNS.timeNSCostApplyRealDelta, "NS", "testing time of real cost based on move apply(e, s)");

		printSummary(convertVector(lNS), timeNS.timeNSCost, "NS", "testing time of move cost()");

		printSummary(convertVector(lNS), timeNS.timeNSEstimatedCost, "NS", "testing time of move estimatedCost()");

		printSummary(convertVector(lNS), timeNS.errorNSEstimatedCost, "NS", "testing error (%) of move estimatedCost()");

		printSummarySimple(convertVector(lNSSeq), vCountMoves, nSolNSSeq, "NSSeq", "counting moves of NSSeq iterator");

		printSummarySimple(convertVector(lNSSeq), vCountValidMoves, nSolNSSeq, "NSSeq", "counting valid moves of NSSeq iterator");

		printSummarySimple(convertVector(lNSEnum), vCountMovesEnum, nSolNSSeq, "NSEnum", "counting moves of NSEnum iterator");

		printSummarySimple(convertVector(lNSEnum), vCountValidMovesEnum, nSolNSSeq, "NSEnum", "counting valid moves of NSEnum iterator");

		printSummarySimple(convertVector(lNSEnum), vCountMovePairsEnum, 1, "NSEnum", "counting general move pairs for NSEnum");

		printSummarySimple(convertVector(lNSEnum), vCountIndependentEnum, 1, "NSEnum", "counting independent move pairs for NSEnum");

		cout << "checkcommand: tests finished successfully!" << endl;
		return true;
	}

	template<class T>
	vector<T*> assignVector(const vector<string> lComponents, T* type, HeuristicFactory<R, ADS>& factory)
	{
		vector<T*> vComp;
		for (unsigned i = 0; i < lComponents.size(); i++)
		{
			Scanner scan(lComponents.at(i));
			type = NULL;
			factory.assign(type, scan.nextInt(), scan.next()); // reversed!

			if (!type)
			{
				cout << "checkcommand  warning: NULL component " << lComponents[i] << "!" << endl;
			}
			else
				vComp.push_back(type);
		}

		return vComp;
	}

	template<class T>
	vector<Component*> convertVector(const vector<T*>& v)
	{
		vector<Component*> vcomp;
		for (unsigned c = 0; c < v.size(); c++)
			vcomp.push_back((Component*) (v[c]));

		return vcomp;
	}

	void printSingleSummary(string component, const pair<int, double>& value, string title)
	{
		printf("---------------------------------\n");
		cout << component << "\t" << title << endl;
		printf("---------------------------------\n");
		printf("title\t#tests\tavg(ms)\tsum(ms)\n");
		double avg = 0;
		int validValues = 0;
		if (value.first > 0)
		{
			printf("%s\t%d\t%.4f\t%.4f\n", component.c_str(), value.first, (value.second / value.first), value.second);
			avg += (value.second / value.first);
			validValues++;
		}
		else
			printf("%s\t%d\tUNTESTED OR UNIMPLEMENTED\n", component.c_str(), 0);
		printf("---------------------------------\n");
		printf("all\t-\t%.4f\t-\n", (avg / validValues));
		cout << endl;
	}

	void printSummary(const vector<Component*>& vcomp, const vector<pair<int, double> >& values, string type, string title)
	{
		printf("---------------------------------\n");
		cout << "|" << type << "|=" << values.size() << "\t" << title << endl;
		printf("---------------------------------\n");
		printf("#id\ttitle\t#tests\tavg(ms)\tsum(ms)\n");
		double avg = 0;
		int validValues = 0;
		for (unsigned id = 0; id < values.size(); id++)
		{
			if (values[id].first > 0)
			{
				printf("#%d\t%s\t%d\t%.4f\t%.4f\n", ((int) id), vcomp[id]->toString().c_str(), values[id].first, (values[id].second / values[id].first), values[id].second);
				avg += (values[id].second / values[id].first);
				validValues++;
			}
			else
				printf("#%d\t%s\t%d\tUNTESTED OR UNIMPLEMENTED\n", ((int) id), vcomp[id]->toString().c_str(), 0);
		}
		printf("---------------------------------\n");
		printf("all\t*\t-\t%.4f\t-\n", (avg / validValues));
		cout << endl;
	}

	void printSummarySimple(const vector<Component*>& vcomp, const vector<int>& values, int numTests, string type, string title)
	{
		printf("---------------------------------\n");
		cout << "|" << type << "|=" << values.size() << "\t" << title << endl;
		printf("---------------------------------\n");
		printf("#id\ttitle\t#tests\tavg of %d tests\n", numTests);
		double avg = 0;
		int validValues = 0;
		for (unsigned id = 0; id < values.size(); id++)
		{
			if (values[id] > 0)
			{
				printf("#%d\t%s\t%d\t%d\n", ((int) id), vcomp[id]->toString().c_str(), values[id], (values[id] / numTests));
				avg += (values[id] / numTests);
				validValues++;
			}
			else
				printf("#%d\t%s\t%d\tUNTESTED OR UNIMPLEMENTED\n", ((int) id), vcomp[id]->toString().c_str(), 0);
		}
		printf("---------------------------------\n");
		printf("all\t*\t-\t%.4f\t-\n", (avg / validValues));
		cout << endl;
	}

};

}

#endif /* OPTFRAME_CHECK_COMMAND_HPP_ */
