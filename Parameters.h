/***********************************************************************************************
 * Parameters.h
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised May 25 2010
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#ifndef PARAMETERS_H
#define PARAMETERS_H

class Parameters {

public:
	char*  _OPMODE;		// operational mode (experiment or calibration)
	char*  _EXPVAR;		// variable analyzed in experiment
	char*  _SNSVAR;		// variable analyzed in sensitivity
  char*  _IMPULS;   // description of impulse
	int    _NTHRDS;		// number of simulations
	int    _NSRUNS;		// number of runs (R)
	int    _NWEEKS;		// number of weeks (T)
	int    _ADJLAG;		// number of initial years to ignore in calibration
	int    _LRNLAG;		// number of initial years after which central bank learning begins
	int    _IMPLAG;		// number of initial years before shock is applied in impulse mode
	int    _NGOODS;		// number of goods (n)
	int    _WKSPMN;		// weeks per month
	int    _WKSPYR;		// weeks per year
	int    _RNSEED;		// RNG seed for initial run
	int    _RNDMRK;		// markup randomization flag
	int    _TRNOVR;		// turnover flag
	int    _FLEXWG;		// downward wage flexibility flag
	int    _ZEROLB;		// zero nominal interest rate bound flag
	int    _SHOCKS;		// match breakup flag
	int    _DISEQU;		// flag for not simulating no-shock equilibrium
	int    _USEEND;		// flag for using the endogenous parameters in experiment
	int    _USECOR;		// flag for using the endogenous correlations in experiment
	int    _THREAD;		// thread ID number in {0, . . . _NTHRDS - 1}
	int    _INIPRD;		// length of initiation period for new banks
	int    _NBANKS;		// number of banking sectors 
  int    _NOBANK;   // Flag for the no-bank economy
  int    _NCROOK;   // number of crooks per good (in {0,...,NGOODS-3})

	int    _MAXSLS;		// maximal target sales (xbar)
	int    _CNTPRD;		// length of contract period in weeks (Delta)
	double _MARKUP;		// markup factor over variable cost (mu)
	double _STDMKP;		// standard deviation of markup
	double _FAILRT;		// weekly failure rate of unprofitable shops (phi)
	double _FXDCST;		// fixed cost (F)
	double _INVSPD;		// weekly inventory adjustment speed (lambda_I)
	double _WAGSPD;		// annual wage adjustment coefficient (beta)
	double _INFREQ;		// weekly frequency of innovations (theta)
	double _DMDELS;		// demand elasticity parameter (epsilon)
	double _INCSPD;		// weekly permanent income adjustment speed (lambda_y)
	double _TMPRFY;		// annual time preference factor (1 + rho)
	double _BRKFRC;		// weekly quit rate (delta)
	double _TRIGGR;		// critical inventory/sales ratio triggering a price cut
	double _LTRIGG;		// critical inventory/sales ratio triggering a layoff
	double _PDELTA;		// size of price cut (oldPrice/newPrice)

	double _EXPINI;		// initial value of variable analyzed in experiment
	double _EXPINC;		// incremental value of variable analyzed in experiment
	double _SNSDEV;		// percentage deviation from baseline of sensitivity variable

	double _BNDINC;		// target debt-gdp ratio (bstar)
	double _FSCSPD;		// annual fiscal adjustment speed (lambda_tau)
	double _INFCOR;		// annual inflation autocorrelation factor (lambda_pi)
	double _GAPCOR;		// output gap autocorrelation factor (lambda_q)
	double _INFTYR;		// inflation coefficient in Taylor rule (sigma_pi)
	double _GAPTYR;		// output gap coefficient in Taylor rule (sigma_q)
	double _GAPTRG;		// initial target logarithmic output gap in Taylor rule (qstar)
	double _GDPINT;   // initial estimate of the intercept in the gdp equation
	double _RRTTRG;		// initial target real interest factor in Taylor rule (1 + rstar)
	double _PITRGY;		// annual target inflation factor in calibration (1 + pistar)
	double _ADJRRT;		// adjustment speed of evolving real rate target
	double _GAPLMB;		// adjustment speed of evolving monthly output gap autocorrelation coefficient
	double _INFLMB;		// adjustment speed of evolving monthly inflation autocorrelation coefficient
	double _SRCHPP;		// job search propensity
	double _CARATE;		// average required capital adequacy ratio
	double _SPREAD;		// normal loan premium over deposit rate
	double _DISCPR;		// discount premium for central bank borrowing
	double _FSDISC;		// fire sale discount
  double _LVALRT;   // average loan to value ratio
  double _MXBAIL;   // maximal deficit that the governmnent will bail out
  double _CASLOP;   // slope of the capital adequacy ratio wrt the output gap
  double _LVSLOP;   // slope of the loan-to-value ratio wrt the output gap
  double _FDICPR;   // probability of getting the richest new owner by FDIC
  double _BRKPRC;   // fraction of agents who break all matches in impulse mode when IMPULS=BRKUPS
  double _EQUFRC;   // fraction of equity that a bank owner can withdraw any week
  double _FREQTR;   // frequency of trading (in weeks)
  double _BKRCST;   // bankruptcy cost (fraction of inventories lost in the process of repossession)
  double _SETUPC;   // total capital required to set up shop
  double _PSLOPE;   // slope parameter in bank's loan demand function

	Parameters(char* inFile);	// constructor
	~Parameters();				    // destructor
	friend ostream& operator <<(ostream &stream, Parameters* object);	// printer
  double get_expVal();			// returns the current value of the experiment variable
	void   adj_endPrm();			// adjuster for endogenous parameters
	void   adj_endPrm(double PITRGY);	// adjuster for endogenous parameters with interpolation
  int    adj_EXPVAR();			// adjuster for variable analyzed in experiment
	int    adj_SNSVAR(int snsDir);		// adjuster for variable analyzed in sensitivity
};

#endif // PARAMETERS_H