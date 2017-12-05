/***********************************************************************************************
 * Firesale.h
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised Dec 30 2014
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

#include <queue>
using namespace std;
class Agent;          // forward declaration of Agent

#ifndef FIRESALE_H
#define FIRESALE_H

typedef queue<Agent*> agentQueue;

class Firesale {
    agentQueue fQueue;	// the queue of firesale suppliers
    int _gIndex;        // index of the good sold

public:
    Firesale(int gdIndx);
    ~Firesale();	                                           
    friend ostream& operator <<(ostream &stream, Firesale* object);
    Agent* dbg_chkDup(); // checks for duplicate seller name
    void ini_frsale(); // initialize the firesale for new run
    void add_seller(Agent* seller);	// add a seller to the queue of firesale suppliers
    void drop_sellr(Agent* seller);	// drop seller from the queue of firesale suppliers
    double run_frsale(double fOrder);	// run a firesale (returns the quantity transacted)
    const double get_supply(); // returns total supply in the queue
};

#endif // FIRESALE_H