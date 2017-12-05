/***********************************************************************************************
 * Firesale.cpp
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised Dec 30 2014
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

#include "Firesale.h"
#include "Agent.h"

Firesale::Firesale(int gdIndx) {
    _gIndex = gdIndx;
}

Firesale::~Firesale(){
}

void Firesale::add_seller(Agent* seller) {
    fQueue.push(seller);
}

void Firesale::drop_sellr(Agent* seller) {
    int nSllrs = static_cast<int> (fQueue.size());
    for(int i = 0; i < nSllrs; i++) {
        if (seller != fQueue.front())
            fQueue.push(fQueue.front());
        fQueue.pop();
    }
}

double Firesale::run_frsale(double fOrder) {
    if (fQueue.size() == 0)
        return 0;
    double unflld = fOrder;
    while (unflld > 0 && fQueue.size() > 0)
        unflld -= fQueue.front()->liquidate(_gIndex, unflld);
    return fOrder - unflld;
}

void Firesale::ini_frsale() {
    while(fQueue.size())
        fQueue.pop();
}

const double Firesale::get_supply() {
    int nSllrs = static_cast<int> (fQueue.size());
    if(nSllrs == 0) return 0;
    double supply = 0;
    for(int i = 0; i < nSllrs; i++) {
        supply += fQueue.front()->get_fOffer(_gIndex);
        fQueue.push(fQueue.front());
        fQueue.pop();
    }
    return supply;
}

Agent* Firesale::dbg_chkDup() {
    int nSllrs = static_cast<int> (fQueue.size());
    if(nSllrs < 2) return NULL;
    for (int i = 0; i < nSllrs; i++) { 
        Agent* nxSllr = fQueue.front();
        fQueue.pop();
        drop_sellr(nxSllr);
        if(static_cast<int>(fQueue.size()) < nSllrs-1) return nxSllr;
        fQueue.push(nxSllr);
    }
    return NULL;
}

ostream& operator <<(ostream &stream, Firesale* object) {
    stream << "gdIndx: " << object->_gIndex << "\n";
    stream << "fQueue.size: " << object->fQueue.size() << "\n";
    stream << "\n";
    return stream;
}