/***********************************************************************************************
 * Statistics.h
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised July 25 2016
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

#include <stdlib.h>
#include <math.h>

class Economy;	// forward declaration of Economy Class

// qsort comparator function
int qst_cmpFnc(const void* argOne, const void* argTwo);

// array index sorter
void srt_idxArr(double* dblArr, int* idxArr, int arrDim);

#ifndef STATISTICS_H
#define STATISTICS_H

class Statistics {
public:
	int _lastCn;	// number of active shop locations recorded in the last quinquennial census
	int _nCrash;	// number of bad runs
	int _tlLfSp;	// run-specific total of the lifespan of all entrants that exit
	int _tlEnEx;	// run-specific total of the number of all entrants that exit

	int _lmShFl;	// number of shop failures observed at the end of last month
	int _lmBkFl;	// number of bank failures observed at the end of last month
	int _lmBkTr;	// number of troubled banks observed at the end of last month

	double _tlShop;		// run-specific total of the number of shops observed weekly after market entry
	double _tlUnEp;		// run-specific total of the weekly number of completed unemployment episodes from all unemployment causes
	double _tlLfEp;		// run-specific total of the weekly number of completed unemployment episodes from "Layoff" state
	double _tlExEp;		// run-specific total of the weekly number of completed unemployment episodes from "Exit" state
	double _tlQtEp;		// run-specific total of the weekly number of completed unemployment episodes from "Quit" state
	double _tlBkEp;		// run-specific total of the weekly number of completed unemployment episodes from "Broke" state
	double _tlUnDr;		// run-specific total of the total weekly duration of completed unemployment episodes from all unemployment causes
	double _tlLfDr;		// run-specific total of the total weekly duration of completed unemployment episodes from "Layoff" state
	double _tlExDr;		// run-specific total of the total weekly duration of completed unemployment episodes from "Exit" state
	double _tlQtDr;		// run-specific total of the total weekly duration of completed unemployment episodes from "Quit" state
	double _tlBkDr;		// run-specific total of the total weekly duration of completed unemployment episodes from "Broke" state
	double _avMkup;		// yearly cross-week average of cross-firm average markup
	double _wtMkup;		// yearly cross-week average of cross-firm sales-weighted average markup
	double _stdMkp;		// yearly cross-week average of cross-firm sales-weighted stdev of markup
	double _wtPrft;		// yearly cross-week average of cross-firm sales-weighted average profit
	double _wtRAge;		// yearly cross-week average of cross-firm sales-weighted average relative age
	double _debtRt;		// debt/GDP ratio at year end
	double _unempl;		// number unemployed at week's end
	double _unLoff;		// number unemployed due to layoffs
	double _unExit;		// number unemployed due to employer exits
	double _unQuit;		// number unemployed due to quits
	double _unBrok;		// number unemployed due to broke employer
	double _woEmpl;		// number without employer at week's end
	double _nwEmpl;		// number with job at new (< 1 year old) shop	

	double* _yeBnds;	// storage for annual observations of the year-end bond supply (overwritten per run)
	double* _yeLoan;    // storage for annual observations of the average weekly book value of bank loans (overwritten per run)
	double* _yeBkrp;	// storage for annual observations of the average weekly hazard rate of bankruptcy (overwritten per run)
	double* _yeFCon;	// storage for annual observations of the average weekly hazard rate of failing the financial viability test at market entry (overwritten per run)
	double* _yeIncm;	// storage for annual observations of average weekly real gdp (overwritten per run)
	double* _yeNInc;	// storage for annual observations of average weekly nominal gdp (overwritten per run)
	double* _yeLnGp;	// storage for annual observations of average weekly logarithmic output gdp (overwritten per run)
	double* _yeCons;	// storage for annual observations of aggregate consumption per year (overwritten per run)
	double* _yeInvn;	// storage for annual observations of year-end inventories (overwritten per run)
	double* _yeDInv;	// storage for annual observations of the year-to-year change in year-end inventories (overwritten per run)
	double* _yeEmpl;	// storage for annual observations of average weekly economy-wide employment (overwritten per run)
	double* _yeUnRt;	// storage for annual observations of average weekly unemployment rate (overwritten per run)
	double* _yeVcRt;	// storage for annual observations of average weekly job vacancy rate (overwritten per run)
	double* _yeRlWg;	// storage for annual observations of average weekly real wage rate (overwritten per run)
	double* _yePIdx;	// storage for annual observations of average weekly gdp deflator (overwritten per run)
	double* _yeInfl;	// storage for annual observations of the year-end simple annual inflation factor (overwritten per run)
	double* _yeCCRR;	// storage for annual observations of monthly average annualized continuously compounded real interest rates (overwritten per run)
	double* _yeCCNR;	// storage for annual observations of monthly average annualized continuously compounded nominal interest rates (overwritten per run)
	double* _yeTxRt;	// storage for annual observations of the year-end tax rate (overwritten per run)

	double* _CRLnGp;	// storage for cross-run average year-end log output gap (per year)
	double* _CRFail;	// storage for cross-run average year-end shop failures (per Year)
	double* _CRAvMk;	// storage for cross-run average of cross-firm average markup (per year)
	double* _CRWtMk;	// storage for cross-run average of cross-firm sales-weighted average markup (per year)
	double* _CRStMk;	// storage for cross-run average of cross-firm sales-weighted stdev of markup (per year)
	double* _CRWtPf;	// storage for cross-run average of cross-firm sales-weighted average profit (per year)
	double* _CRRAge;	// storage for cross-run average of cross-firm sales-weighted average relative age (per year)
	double* _CRDbtR;	// storage for cross-run average of year-end debt/GDP ratio (per year)
	double* _CRTxRt;	// storage for cross-run average of year-end tax rate (per year)
	double* _CRCCRR;	// storage for cross-run average of monthly average of annualized continuously compounded real interest rate (per year)
	double* _CRCCPi;	// storage for cross-run average of year-end year-to-year continuously compounded annual inflation rate (per year)
	double* _CRGpTg;	// storage for cross-run average of year-end evolving target for logarithmic output gap
	double* _CRRtTg;	// storage for cross-run average of year-end evolving target real interest factor
	double* _CRGpRm;	// storage for cross-run average of year-end evolving output gap autocorrelation coefficient
	double* _CRPiRm;	// storage for cross-run average of year-end evolving inflation autocorrelation coefficient
	double* _CRGdPt;	// storage for cross-run average of year-end evolving estimate of log potential GDP

	double** _RYLnGp;	// storage for year-end average output gap (per run per year)
	double** _RYFail;	// storage for year-end shop failures (per run per year)
	double** _RYAvMk;	// storage for cross-firm average markup (per run per year)
	double** _RYWtMk;	// storage for cross-firm sales-weighted average markup (per run per year)
	double** _RYStMk;	// storage for cross-firm sales-weighted stdev of markup (per run per year)
	double** _RYWtPf;	// storage for cross-firm sales-weighted average profit (per run per year)
	double** _RYRAge;	// storage for cross-firm sales-weighted average relative age (per run per year)
	double** _RYDbtR;	// storage for year-end debt/GDP ratio (per run per year)
	double** _RYTxRt;	// storage for year-end tax rate (per run per year)
	double** _RYCCRR;	// storage for monthly average of annualized continuously compounded real interest rate (per run per year)
	double** _RYCCPi;	// storage for year-end year-to-year continuously compounded annual inflation rate (per run per year)
	double** _RYGpTg;	// storage for year-end evolving target for logarithmic output gap (per run per year)
	double** _RYRtTg;	// storage for year-end evolving target real interest factor (per run per year)
	double** _RYGpRm;	// storage for year-end evolving output gap autocorrelation coefficient(per run per year)
	double** _RYPiRm;	// storage for year-end evolving inflation autocorrelation coefficient (per run per year)
	double** _RYGdPt;	// storage for year-end evolving estimate of log potential GDP (per run per year)
	double** _RYBkFl;	// storage for average yearly hazard of bank failures (per run per year)
	double** _RYBkTr;	// storage for average weekly fraction of troubled banks (per run per year)

	double** _RMLnGp;	// storage for average output gap (per run per month)
	double** _RMFail;	// storage for average shop failures (per run per month)
	double** _RMBkFl;	// storage for average yearly hazard of bank failures (per run per month)
	double** _RMBkTr;	// storage for average weekly fraction of troubled banks (per run per month)
	double** _RMEntr;	// storage for average monthly number of new entrants (per run per month)

	double* _CYIncm;	// storage for cross-year average year-end income (per run)
	double* _CYLnGp;	// storage for cross-year average of monthly average of logarithmic output gap (per run)
	double* _CYFail;	// storage for cross-year average year-end shop failures (per run)
	double* _CYWtFl;	// storage for cross-year average year-end employment-weighted shop failures (per run)
	double* _CYHpls;	// storage for cross-year average year-end shop failures due to hopelessness (per run)
	double* _CYZbRt;	// storage for cross-year average year-end incidence of hitting zero lower bound (per run)
	double* _CYInfl;	// storage for cross-year average year-end year-to-year simple annual inflation rate (per run)
	double* _CYCCPi;	// storage for cross-year average year-end year-to-year continuously compounded annual inflation rate (per run)
	double* _CYCCRR;	// storage for cross-year average of monthly average of annualized continuously compounded real interest rate (per run)
	double* _CYExit;	// storage for cross-year average 5-yr exit rate (per run)
	double* _erWtMk;	// storage for final-year weighted average markup (per run)
	double* _CYBkFl;	// storage for cross-year average hazard of bank failures (per run)
	double* _CYBkTr;	// storage for cross-year average fraction of troubled banks (per run)
	bool*   _badRns;	// storage for indicator of crash (per run)

	double* _vncyRt;	// list of average weekly vacancy rate for all runs
	double* _entrFl;	// list of average weekly flow of entrants for all runs
	double* _entrRt;	// list of average weekly entry rate (fraction of nActSh) for all runs
	double* _nwJCRt;	// list of average weekly new shop (<1yr) jobs as fraction of employment for all runs
	double* _loan2Y;	// list of average weekly credit to GDP ratio for all runs
	double* _rlLoan;	// list of average weekly real value of loans for all runs
	double* _enRlLn;	// list of average weekly real value of loans given to entrants for all runs
	double* _lnVlRt;	// list of average monthly loan-to-value ratio for all runs
	double* _bkrpRt;	// list of average weekly bankruptcy hazard (expressed as per annum) for all runs
	double* _fConRt;	// list of average weekly financial constraint rate (fraction of tested entrepreneurs) for all runs
	double* _jobMch;	// list of average weekly number of job matches for all runs
	double* _job2Jb;	// list of average weekly number of job to job moves for all runs
	double* _strMch;	// list of average weekly number of store matches for all runs
	double* _str2St;	// list of average weekly number of store to store moves for all runs
	double* _exitRt;	// list of average weekly shop exit rate for all runs
	double* _jbLsRt;	// list of average weekly job loss rate for all runs
	double* _lsLfRt;	// list of average weekly job loss rate due to layoffs for all runs
	double* _lsQtRt;	// list of average weekly job loss rate due to quits for all runs
	double* _lsExRt;	// list of average weekly job loss rate due to employer exits for all runs
	double* _uempDr;	// list of average weekly unemployment duration for all runs
	double* _unLfDr;	// list of average weekly unemployment duration due to layoffs for all runs
	double* _unQtDr;	// list of average weekly unemployment duration due to quits for all runs
	double* _unExDr;	// list of average weekly unemployment duration due to exits for all runs
	double* _unBkDr;	// list of average weekly unemployment duration due to broke employer for all runs
	double* _uempRt;	// list of average weekly unemployment for all runs
	double* _unLfRt;	// list of average weekly unemployment due to layoffs for all runs
	double* _unQtRt;	// list of average weekly unemployment due to quits for all runs
	double* _unExRt;	// list of average weekly unemployment due to employer exits for all runs
	double* _unBkRt;	// list of average weekly unemployment due to broke employer for all runs
	double* _prftRt;	// list of average profit rate for all runs
	double* _pFreqy;	// list of average yearly price change frequencies for all runs
	double* _prcDsp;	// list of average weekly price dispersion for all runs
	double* _wprDsp;	// list of average weekly sales-weighted price dispersion for all runs
	double* _pftDsp;	// list of average weekly profit dispersion for all runs
	double* _salDsp;	// list of average weekly sales dispersion for all runs
	double* _wpfDsp;	// list of average weekly sales-weighted profit dispersion for all runs
	double* _lifeEx;	// list of average entrant's life expectancy for all runs
	double* _shPrWk;	// list of average weekly number of active shops for all runs
	double* _failHz;	// list of average weekly hazard rates for shop failures for all runs
	double* _hplsHz;	// list of average weekly hazard rates for shop failures from hopelessness for all runs
	double* _gapVol;	// list of year-end output gap variance for all runs
	double* _gapCor;	// list of year-end output gap AR1 coefficient for all runs
	double* _infVol;	// list of year-end inflation variance for all runs
	double* _infCor;	// list of year-end inflation rate AR1 coefficient for all runs
    double* _YLCorD;    // list of year-end correlation coeff of differenced, logged, loans and nominal gdp
	
	double** _pftMrk;	// storage for the profit-markup relationship
	Economy* _abEcon;	// access to the economy

	double _avLnGp;		// cross-run sample monthly average logarithmic output gap
	double _avGpVl;		// cross-run sample average output gap volatility
	double _avGpCr;		// cross-run sample average output gap AR1 coefficient
	double _avPiRt;		// cross-run sample average inflation rate
	double _avCCPi;		// cross-run sample average continuously compounded inflation rate
	double _avPiVl;		// cross-run sample average inflation rate volatility
	double _avPiCr;		// cross-run sample average inflation rate AR1 coefficient
	double _avRlRt;		// cross-run sample average annualized continuously compounded real interest rate
	double _avRlVl;		// cross-run sample average real interest rate volatility
	double _avPrDs;		// cross-run sample average price dispersion
	double _avWPrD;		// cross-run sample average sales-weighted price dispersion
	double _avPfDs;		// cross-run sample average profit dispersion
	double _avWPfD;		// cross-run sample average sales-weighted profit dispersion
	double _avSlDs;		// cross-run sample average sales dispersion
	double _avNmFl;		// cross-run sample average number of failures
	double _avWtFl;		// cross-run sample average employment-weighted failures
	double _avHpls;		// cross-run sample average number of failures due to hopelessness
	double _avZbRt;		// cross-run sample average zero lower bound incidence
	double _avUnDr;		// cross-run sample average unemployment duration
	double _avLfDr;		// cross-run sample average unemployment duration due to layoffs
	double _avQtDr;		// cross-run sample average unemployment duration due to quits
	double _avExDr;		// cross-run sample average unemployment duration due to exits
	double _avBkDr;		// cross-run sample average unemployment duration due to broke employer
	double _avUnRt;		// cross-run sample average unemployment rate
	double _avUnLf;		// cross-run sample average unemployment rate due to layoffs
	double _avUnQt;		// cross-run sample average unemployment rate due to quits
	double _avUnEx;		// cross-run sample average unemployment rate due to employer exits
	double _avUnBk;		// cross-run sample average unemployment rate due to broke employer
	double _avPfRt;		// cross-run sample average profit rate
	double _avPrFq;		// cross-run sample average yearly price change frequency
	double _avVnRt;		// cross-run sample average vacancy rate
	double _avEnFl;		// cross-run sample average weekly flow of entrants
	double _avEnRt;		// cross-run sample average weekly entry flow (as fraction of nActSh)
	double _avNJCR;		// cross-run sample average weekly new-shop job creation rate
	double _avLn2Y;		// cross-run sample average weekly credit to GDP ratio
	double _avRlLn;		// cross-run sample average weekly real value of loans
	double _avEnLn;		// cross-run sample average weekly real value of loans given to entrants
	double _avLVRt;		// cross-run sample average loan-to-value ratio
	double _avBkrp;		// cross-run sample average weekly bankruptcy rate (annualized hazard)
	double _avFCon;		// cross-run sample average weekly financial failure rate (prob of failing test)
	double _avJbMt;		// cross-run sample average number of job matches
	double _avJ2Jb;		// cross-run sample average number of job to job moves
	double _avStMt;		// cross-run sample average number of store matches
	double _avS2St;		// cross-run sample average number of store to store moves
	double _avLfEx;		// cross-run sample average life expectancy of entrants
	double _avWkSh;		// cross-run sample average number of active shops
	double _avFlHz;		// cross-run sample average weekly hazard rate of shop failures
	double _avHpHz;		// cross-run sample average hazard rate of failures from hopelessness
	double _avExRt;		// cross-run sample average 5-yr exit rate
	double _avJbLs;		// cross-run sample average job loss rate
	double _avLsLf;		// cross-run sample average job loss rate due to layoffs
	double _avLsQt;		// cross-run sample average job loss rate due to quits
	double _avLsEx;		// cross-run sample average job loss rate due to employer exits
	double _avWtMk;		// cross-run sample average weighted avg mkup in last year
	double _avBkFl;		// cross-run sample average hazard of bank failures
	double _avBkTr;		// cross-run sample average fraction of troubled banks

	double _mdLnGp;		// cross-run sample median logarithmic output gap
	double _mdGpVl;		// cross-run sample median output gap volatility
	double _mdCCPi;		// cross-run sample median continuously compounded inflation rate
	double _mdPiVl;		// cross-run sample median inflation rate volatility
	double _mdRlRt;		// cross-run sample median real interest rate
	double _mdRlVl;		// cross-run sample median real interest rate volatility
	double _mdUnRt;		// cross-run sample median unemployment rate
	double _mdUnDr;		// cross-run sample median unemployment duration
	double _mdWkSh;		// cross-run sample median number of active shops
	double _mdJbLs;		// cross-run sample median weekly job loss rate
	double _mdLsLf;		// cross-run sample median job loss rate due to layoffs
	double _mdLsQt;		// cross-run sample median job loss rate due to quits
	double _mdLsEx;		// cross-run sample median job loss rate due to employer exits
	double _mdExRt;		// cross-run sample median 5-yr exit rate
	double _mdFlHz;		// cross-run sample median (weekly) hazard of shop failure
	double _mdHpHz;		// cross-run sample median hazard rate of failures from hopelessness
	double _mdEnFl;		// cross-run sample median weekly flow of entrants
	double _mdEnRt;		// cross-run sample median weekly entry flow (as fraction of nActSh)
	double _mdNJCR;		// cross-run sample median weekly new-shop job creation rate
	double _mdLn2Y;		// cross-run sample median weekly credit to GDP ratio
	double _mdRlLn;		// cross-run sample median weekly real value of loans
	double _mdEnLn;		// cross-run sample median weekly real value of loans given to entrants
	double _mdLVRt;		// cross-run sample median loan-to-value ratio
	double _mdBkrp;		// cross-run sample median weekly bankruptcy rate (annualized hazard)
	double _mdFCon;		// cross-run sample median weekly financial failure rate (prob of failing test)
	double _mdZbRt;		// cross-run sample median zero lower bound incidence
	double _mdSlDs;		// cross-run sample median sales dispersion
	double _mdWtMk;		// cross-run sample median weighted avg mkup in last year
	double _mdPfRt;		// cross-run sample median profit rate
	double _mdPrFq;		// cross-run sample median yearly price change frequency
	double _mdBkFl;		// cross-run sample median hazard of bank failures
	double _mdBkTr;		// cross-run sample median fraction of troubled banks
	double _mdGpCr;		// cross-run sample median output gap autocorrelation
	double _mdPiCr;		// cross-run sample median inflation autocorrelation
	double _lwOpGp;		// cross-run sample 95% CI lower output gap
	double _upOpGp;		// cross-run sample 95% CI upper output gap

	// storage for cross-run averages by decile of the cross-run log output gap distribution
	double* _dcLnGp;	// cross-run average log output gap
	double* _dcUnRt;	// cross-run average unemployment rate
	double* _dcUnDr;	// cross-run average unemployment duration
	double* _dcWkSh;	// cross-run average number of active shops
	double* _dcGpVl;	// cross-run average gap volatility
	double* _dcPiVl;	// cross-run average inflation volatility
	double* _dcCCPi;	// cross-run average contin compounded inflation rate
	double* _dcBkFl;	// cross-run average bank failure hazard
	double* _dcBkTr;	// cross-run average fraction of troubled banks
	double* _dcJbLs;	// cross-run average job loss rate
	double* _dcLsLf;	// cross-run average job loss rate due to layoffs
	double* _dcLsQt;	// cross-run average job loss rate due to quits
	double* _dcLsEx;	// cross-run average job loss rate due to employer exits
	double* _dcFlHz;	// cross-run average shop failure hazard
	double* _dcHpHz;	// cross-run average hazard rate of failures from hopelessness
	double* _dcEnFl;	// cross-run average weekly flow of entrants
	double* _dcEnRt;	// cross-run average weekly entry flow (as fraction of nActSh)
	double* _dcNJCR;	// cross-run average weekly new-shop job creation rate
	double* _dcLn2Y;	// cross-run average weekly credit to GDP ratio
	double* _dcRlLn;	// cross-run average weekly real value of loans
	double* _dcEnLn;	// cross-run average weekly real value of loans given to entrants
	double* _dcLVRt;	// cross-run average loan-to-value ratio
	double* _dcBkrp;	// cross-run average weekly bankruptcy rate (annualized hazard)
	double* _dcFCon;	// cross-run average weekly financial failure rate (prob of failing test)
	double* _dcZbRt;	// cross-run average zero lower bound incidence
	double* _dcSlDs;	// cross-run average sales dispersion
	
	double* _avPfMk;	// cross-run sample average profit-markup relationship

	Statistics(Economy* abEcon);	// constructor
	~Statistics();					// destructor

	double get_avPfMk(int pcIndx) const;	// returns _avPfMk[pcIndx]

	void col_wkStat();	// collect end-of-week statistics
	void col_mnStat();	// collect end-of-month statistics
	void col_yrStat();	// collect end-of-year statistics
	void col_cnStat();	// collect census statistics
	void col_rnStat();	// collect end-of-run statistics
	void cal_CRStat();	// calculate cross-run sample statistics

private:
	double _AVGYRS;     // average time variable (for calculating GDP time trend)
	double _MOMENT;     // moment matrix used in calculating GDP time trend
	void cal_pftMrk();	// perform weekly calculation of the profit-markup relationship
};

#endif // STATISTICS_H
