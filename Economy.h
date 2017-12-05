/***********************************************************************************************
 * Economy.h
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised August 2 2016
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

#include <iostream>
#include <iomanip>
#include <math.h>
#include <windows.h>
#include "Agent.h"

using namespace std;
class Agent;			// forward declarations
class Parameters;
class Shop;				
class Bank;       
class Firesale;   
class Crook;      
class WorkerMan;  
class ShopOwner;  
class BankOwner;  

extern HANDLE CSLMTX;

#ifndef ECONOMY_H
#define ECONOMY_H

class Economy {
    friend class Statistics;
    Shop** _shList;	// ordered list of shop locations - _shList[i] contains shop location (i + 1)  
    Bank** _bkList; // ordered list of banks - _bkList[i] points to the bank in sector i
    Agent** _agList; // ordered list of agents - _agList[i] contains agent (i + 1)
    Agent** _rnList; // random list of agents
    Agent*** _crList; // list of crooks - _crList[i] contains agents in bank sector i
    Agent*** _prList;	// list of producers - _prList[i] contains agents with supply good (i + 1)
    Agent*** _cnList;	// list of consumers - _cnList[i] contains agents with primary demand good (i + 1)
    Firesale** _frList; // list of firesales - _frList[i] is used by sellers of good (i + 1)
    WorkerMan* _worker; // pointer to a worker mode
    ShopOwner* _sOwner; // pointer to a shopowner mode
    BankOwner* _bOwner; // pointer to a bankowner mode
    Statistics* _ecStat; // statistics collector and reporter

    int _RNSEED; // RNG seed for initial run
    int _NWEEKS; // number of weeks (T)
    int _NGOODS; // number of goods (n)
    int _NSRUNS; // number of runs (R)
    int _WKSPMN; // weeks per month
    int _WKSPYR; // weeks per year
    int _MNSPYR; // number of months per year
    int _NYEARS; // number of years
    int _NMNTHS; // number of months
    int _MAXSLS; // maximal target sales (xbar)
    int _THREAD; // thread ID number in {0, . . . _NTHRDS - 1}
    int _NSHOPS; // initial number of shop locations
    int _TRNOVR; // flag for turnover
    int _SHOCKS; // flag for shocks
    int _ZEROLB; // zero nominal interest rate bound flag
    int _NCROOK; // number of crooks per good
    int _ADJLAG; // number of initial years to ignore in calibration
    int _LRNLAG; // number of initial years after which central bank learning begins
    int _IMPLAG; // number of initial years before shock is applied in impulse mode
    int _NBANKS; // number of bank sectors
    int _NAGNTS; // number of agents
    int _NOBANK; // Flag for the no-bank economy
    int _RNDMRK; // markup randomization flag
    int _CNTPRD; // length of contract period in weeks (Delta)
    int _INIPRD; // length of initiation period for new bank loan customers
    int _wkCntr; // week counter in {0,...,NWEEKS-1}
    int _nShops; // actual number of shop locations
    int _nActSh; // number of active shop locations
    int _mnCntr; // month counter (the 12 months in a year only)
    int _yrCntr; // year counter (all years)
    int _cnCntr; // census counter (5 yearly)
    int _zbCntr; // zero lower bound on nominal interest binding counter (yearly)
    int _rnCntr; // run counter
    int _nAlive; // number of surviving initial shops
    int _nFails; // number of active shop failures
    int _nPrDlt; // number of price changes
    int _nPrObs; // number of observed price change opportunities
    int _nBkFlr; // number of bank failures
    int _nBkTrb; // number of troubled banks
    int _enLfSp; // life span of entrants during market exit
    int _nEntEx; // number of entrants that exit
    int _nUn2Em; // number of completed unemployment episodes from all unemployment causes
    int _nLf2Em; // number of completed unemployment episodes from "Layoff" state
    int _nEx2Em; // number of completed unemployment episodes from "Exit" state
    int _nQt2Em; // number of completed unemployment episodes from "Quit" state
    int _nBk2Em; // number of completed unemployment episodes from "Broke" state
    int _unDurn; // total duration of completed unemployment episodes from all unemployment causes
    int _lfDurn; // total duration of completed unemployment episodes from "Layoff" state
    int _exDurn; // total duration of completed unemployment episodes from "Exit" state
    int _qtDurn; // total duration of completed unemployment episodes from "Quit" state
    int _bkDurn; // total duration of completed unemployment episodes from "Broke" state
    int _nHplss; // number of shop failures due to hopelessness
    int _anvCnt; // total (within thread) number of iterations in get_annVal

    int* _numPrs; // number of producers - _numPrs[i] contains number of agents with supply good (i + 1)
    int* _numCns; // number of consumers - _numCns[i] contains number of agents with primary demand good (i + 1)
    int* _nSllrs; // number of active shops - nSllrs[i] contains the number selling good (i + 1)

    bool _crashd; // indicates a run that has crashed
    bool _shopFl; // indicates that the first active shop will be forced to exit
    bool _bankFl; // indicates that the first sector bank will be forced to exit
    bool _brkups; // indicates that the fraction _BRKPRC of individuals will breakup all matches this week
    bool _bankTr; // indicates that the first sector bank will be forced into trouble (gets reset in inc_tmCntr())
    bool _implse; // indicates that the economy is in impulse mode
                        // NB: _implse, _shopFl and _bankFl should NOT be reset in ini_forRun

    char* _IMPULS; // description of impulse
    char* _OPMODE; // operational mode
    char* _RSFILE; // prefix of file names for storing results
	FILE* _rystrm; // file handle for storing yearly results for all runs

    double _FSCSPD; // annual fiscal adjustment speed (lambda_tau)
    double _INFREQ; // weekly frequency of innovations (theta)
    double _INFCOR; // annual inflation autocorrelation factor (lambda_pi)
    double _GAPCOR; // output gap autocorrelation factor (lambda_q)
    double _INFTYR; // inflation coefficient in Taylor rule (sigma_pi)
    double _GAPTYR; // output gap coefficient in Taylor rule (sigma_q)
    double _GAPTRG; // initial target logarithmic output gap (qstar)
    double _TMPRFY; // annual time preference factor (1 + rho)
    double _BRKFRC; // weekly quit rate (delta)
    double _BNDINC; // target debt-gdp ratio (bstar)
    double _PITRGY; // annual target inflation factor (1 + pistar)
    double _SRCHPP; // job search propensity
    double _CARATE; // average required capital adequacy ratio
    double _GDPINT; // initial estimate of the intercept in the gdp equation
    double _GDPPOT; // initial estimate of potential GDP
    double _RRTTRG; // initial target real interest factor in Taylor rule (1 + rstar)
    double _ADJRRT; // adjustment speed of evolving real rate target
    double _GAPLMB; // adjustment speed of evolving monthly output gap autocorrelation coefficient
    double _INFLMB; // adjustment speed of evolving monthly inflation autocorrelation coefficient
    double _CASLOP; // slope of the capital adequacy ratio wrt the output gap
    double _LVSLOP; // slope of the loan-to-value ratio wrt the output gap
    double _BRKPRC; // fraction of agents who break all matches in impulse mode when IMPULS=BRKUPS
    double _CAPGDP; // capacity gdp
    double _INPCAP; // input to capacity gdp ratio
    double _EQMTAX; // equilibrium tax rate in no-shock steady state
    double _GDPRYR; // initial annual output gdp autocorrelation coefficient 
    double _INFRYR; // initial annual inflation autocorrelation coefficient
    double _MARKUP; // markup factor over variable cost (mu)
    double _STDMKP; // standard deviation of markup
    double _FXDCST; // fixed cost (F)
    double _DMDELS; // demand elasticity parameter (epsilon)
    double _INCSPD; // weekly permanent income adjustment speed (lambda_y)
    double _DISCPR; // premium on central bank borrowing
    double _FSDISC; // firesale discount
    double _LVALRT; // average loan to value ratio
    double _MXBAIL; // maximal fraction of GDP that the governmnent will use to bail out banks
    double _FDICPR; // probability of getting the richest new owner by FDIC
    double _SETUPC; // total capital required to set up shop
    double _TMPRFW; // weekly time preference factor
    double _PITRGW; // weekly target inflation factor
    double _SPNDPP; // propensity to spend out of wealth
    double _FREQTR; // Frequency of trading (in weeks)
    double _initPm; // initial equilibrium value of money
    double _bndStk; // aggregate stock of bonds
    double _rlWage; // real wage per week (calculated weekly right after trading)
    double _annVal; // value of a real perpetuity
    double _txRate; // sales tax rate
    double _wgRate; // average weekly wage rate (calculated weekly right after trading)
    double _fsaleP; // firesale price
    double _hcutPr; // haircut price
    double _wkIntr; // nominal interest factor per week
    double _caRate; // evolving capital adequacy rate
    double _lValRt; // evolving loan-to-value ratio
    double _cpIndx; // consumer price index (calculated weekly right after trading)
    double _monStk; // aggregate stock of money
    double _rlDebt; // real government debt
    double _rlIncm; // real gdp per week (calculated weekly right after trading)
    double _nmIncm; // nomimal gdp per week (calculated weekly right after trading)
    double _cmRInc; // monthly average real gdp per week
    double _cmNInc; // monthly average nominal gdp per week
    double _gdpint; // evolving estimate of the intercept in the gdp equation
    double _gdppot; // evolving estimate of the log potential output
    double _rrttrg; // evolving target real interest factor in Taylor rule (1 + rstar)
    double _csttyr; // evolving constant in the Taylor rule
    double _gdpssq; // sum of squared log gdps
    double _gdpsum; // sum of log gdps
    double _gdpryr; // evolving yearly output gdp autocorrelation coefficient
    double _infssq; // weighted sum of squared inflation deviations
    double _infryr; // evolving monthly inflation autocorrelation coefficient
    double _ilggap; // last period's log GDP to be used in updating a/c coefficient
    double _ilginf; // last period's inflation to be used in updating a/c coefficient
    double _entVac; // weekly vacancy rate after market entry
    double _nEnter; // weekly number of actual entrants
    double _entrFr; // weekly number of entrants as a fraction of active shops
    double _entShp; // weekly number of active shops after market entry
    double _extShp; // weekly number of active shops after market exit
    double _jobLss; // weekly number of jobs lost due to breakups, exits and layoffs
    double _jbLsLf; // weekly number of jobs lost due to layoffs
    double _jbLsQt; // weekly number of jobs lost due to quits (breakups)
    double _jbLsEx; // weekly number of jobs lost due to employer exits
    double _nJbMch; // weekly number of job matches (during entry or matching)
    double _nJb2Jb; // weekly number of job to job moves (during entry or matching)
    double _nStMch; // weekly number of store matches during market matching
    double _nSt2St; // weekly number of store to store moves during market matching
    double _bkrpFr; // weekly fraction of bankrupt shops
    double _fConFr; // weekly fraction of entrepreneurs failing financial viability test
	double _entrLn; // weekly amount of nominal loans given to entrants
    double _fscInv; // yearly aggregate inventory after fiscal policy
	double _iniInv; // aggregate inventory just after a run is initialized
	double _aggCon; // yearly aggregate real consumption
	double _avgLVR; // yearly average loan-to-value ratio
    double _wtFail; // yearly employment-weighted active shop failures
    double _surMkp; // end of run average markup of survivors
    double _agPrft; // yearly aggregate profit (incremented weekly right after trading)
    double _nomGdp; // yearly nominal GDP
    double _crProb; // probability that a random "entrepreneur" is a crook

    double* _lagdPc; // storage for last 12 months' price levels
    double* _lagdPi; // storage for last 12 months' (backward) simple year-to-year inflation factors
    double* _lagdRI; // storage for last 12 months' (backward) real (per week) gdp
    double* _lagdNI; // storage for last 12 months' (backward) nominal (per week) gdp
    double* _lag3RI; // storage for last 12 months' (backward) 3-month moving average real (per week) gdp
    double* _lgCCRR; // storage for last 12 months' (backward) continuously compounded annualized real interest rate
	double* _lgCCNR; // storage for last 12 months' (backward) continuously compounded annualized nominal interest rate

    void ini_forRun(); // initialize economy for a new run
    void sim_implse(); // simulate impulse
    void sim_census(); // simulate market census
    void sim_mktEnt(); // simulate market entry
    void sim_mktMch(); // simulate market matching
    void sim_finMkt(); // simulate bond market activities
    void sim_mktTrd(); // simulate market trading
    void sim_monPol(); // simulate monetary policy (monthly)
    void sim_mktBrk(); // simulate market break-ups
    void sim_fisPol(); // simulate fiscal policy (yearly)
    void sim_mktExt(); // simulate market exit
    void sim_shpUpd(); // simulate store updates
    void inc_tmCntr(); // increment the week, month and year counters
    void prt_ryStat(); // print yearly statistics by run
    void inc_rnCntr(); // increment the run counter
    void dbg_chkNeg(); // check for negative values
    void inc_shList(); // dynamically increase the list of shop locations
    void rnd_rnList(); // randomize the random list of agents
    void adj_shPars(Shop* dxShop); // allow customers to adjust to new prices
    double get_taylor(double yrCCPi, double logGDP); // returns a simple weekly nominal interest factor from a taylor rule
    double get_annVal(double yrCCPi, double logGDP); // returns an annuity value
    double get_fiscal(double rlDebt);	// returns tax rate from the fiscal rule
    double get_agEmpl(); // returns the aggregate level of employment
    double get_agVncy(); // returns the aggregate level of vacancies
    double get_agInvn(); // returns the aggregate level of inventories
    double do_squash(double change, double absLim, double equLim); // returns squashed change within +- absLim

public:
    Economy(Parameters* PARAMS, char* RSFILE);
    ~Economy();
    friend ostream& operator <<(ostream &stream, Economy* object);
    Shop*  get_nextSh();
    Agent* get_aCrook(int sector) const; // get a Crook with untroubled bank, best not in sector
    Shop* get_shList(int number) const;
    Bank* get_bkList(int sector) const;
    Agent* get_agList(int number) const;
    Agent* get_prList(int gIndex, int number) const;
    Agent* get_cnList(int gIndex, int number) const;
    Firesale** get_frList() const;
    WorkerMan* get_worker() const; 
    ShopOwner* get_sOwner() const; 
    BankOwner* get_bOwner() const; 
    Statistics* get_ecStat() const;
    void sim_ABECON(); // called by executor to start the simulation
    void prt_smStat(); // print simulation statistics
    void inc_jMchSt(Shop* sh); // increment job match statistics
    void inc_sMchSt(Shop* sh); // increment store match statistics
    void inc_nBkFlr();
    void inc_nBkTrb();
    void inc_drStat(UnType unType, int unWeek);	// increment unemployment duration statistics
    void inc_nSllrs(int gIndex); // add one seller
	void inc_entrLn(double amount); // increment weekly amount of nominal loans given to entrants
	void inc_aggCon(double amount); // increment yearly aggregate real consumption
    void set_implse(bool truFal);
    const int get_NGOODS();
    const int get_TRNOVR();
    const int get_MAXSLS();
    const int get_NCROOK();
    const int get_NYEARS();
    const int get_NMNTHS();
    const int get_WKSPMN();
    const int get_MNSPYR();
    const int get_NAGNTS();
    const int get_NBANKS();
    const int get_NOBANK();
    const int get_RNDMRK();
    const int get_wkCntr();
    const int get_nShops();
    const int get_numPrs(int gIndex);
    const int get_numCns(int gIndex);
    const double get_GAPTRG();
    const double get_MARKUP();
    const double get_DMDELS();
    const double get_FXDCST();
    const double get_SRCHPP();
    const double get_PITRGW();
    const double get_FREQTR();
    const double get_SPNDPP();
    const double get_FDICPR();
    const double get_INCSPD();
    const double get_SETUPC();
    const double get_PITRGY();
    const double get_STDMKP();
    const double get_CAPGDP();
    const double get_annVal();
    const double get_txRate();
    const double get_wgRate();
    const double get_fsaleP();
    const double get_hcutPr();
    const double get_wkIntr();
    const double get_caRate();
    const double get_lValRt();
    const double get_bndStk();
    const double get_initPm();
    const double get_anvCnt();
    const double get_crProb();
};

#endif // ECONOMY_H
