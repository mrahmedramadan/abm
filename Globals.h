/***********************************************************************************************
 * Globals.h
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised September 12 2015
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

// mutex handles
HANDLE CSLMTX;		// mutex for printing to the console
HANDLE PTRMTX;		// mutex for loading parameters 
HANDLE ARRMTX;		// mutex for accessing the global arrays
HANDLE FILMTX;		// mutex for file i/o operations

// storage for cross-thread averages (calibration and randomize only)
double CT_AVGGAP;	// mean logarithmic output gap
double CT_AVGVOL;	// output gap volatility
double CT_AVGUNR;	// mean unemployment rate
double CT_AVGUDR;	// mean unemployment duration
double CT_AVGRRT;	// mean continuously compounded real interest rate
double CT_AVGINF;	// mean continuously compounded inflation rate
double CT_AVGIVL;	// inflation volatility
double CT_AVGCOR;	// output gap autocorrelation
double CT_AVGICR;	// inflation autocorrelation
double CT_AVGYLC;	// correlation of diffed, logged, loans and nominal GDP
double CT_AVGLSS;	// weekly job loss rate
double CT_AVGEXT;	// weekly exit rate
double CT_AVGVAC;	// weekly vacancy rate
double CT_AVGMKP;	// average markup at end of run
double CT_PRFTRT;	// average profit rate over all years
double CT_AVGFQY;	// average yearly price changes per shop
double CT_AVGBKF;	// average yearly bank failure hazard rate
double CT_AVGBKT;	// average fraction of banks in trouble
int    CT_TTLBAD;   // total number of bad runs

// storage for cross-thread medians (calibration and randomize only)
double CT_MDNGAP;	// median logarithmic output gap
double CT_MDNVOL;	// median output gap volatility
double CT_MDNUNR;	// median unemployment rate
double CT_MDNUDR;	// median unemployment duration (per spell)
double CT_MDNRRT;	// median continuously compounded real interest rate
double CT_MDNINF;	// median continuously compounded inflation rate
double CT_MDNIVL;	// median inflation volatility
double CT_MDNCOR;	// median output gap autocorrelation
double CT_MDNICR;	// median inflation autocorrelation
double CT_MDNYLC;	// median correlation of diffed, logged, loans and nominal gdp
double CT_MDNLSS;	// median weekly job loss rate
double CT_MDNEXT;	// median weekly exit rate
double CT_MDNVAC;	// median weekly vacancy rate
double CT_MDNMKP;	// median markup at end of run
double CT_MDNPFT;	// median profit rate over all years
double CT_MDNFQY;	// median yearly price changes per shop
double CT_MDNBKF;	// median yearly bank failure hazard rate
double CT_MDNBKT;	// median fraction of banks in trouble

// storage for cross-thread averages by decile of the cross-thread log output gap distribution (calibration and randomize only)
double* CT_DCLGAP;	// average logarithmic output gap
double* CT_DCLVOL;	// average output gap volatility
double* CT_DCLUNR;	// average unemployment rate
double* CT_DCLUDR;	// average unemployment duration (per spell)
double* CT_DCLRRT;	// average continuously compounded real interest rate
double* CT_DCLINF;	// average continuously compounded inflation rate
double* CT_DCLIVL;	// average inflation volatility
double* CT_DCLCOR;	// average output gap autocorrelation
double* CT_DCLICR;	// average inflation autocorrelation
double* CT_DCLYLC;	// average correlation of diffed, logged, loans and nominal GDP
double* CT_DCLLSS;	// average weekly job loss rate
double* CT_DCLEXT;	// average weekly exit rate
double* CT_DCLVAC;	// average weekly vacancy rate
double* CT_DCLMKP;	// average markup at end of run
double* CT_DCLPFT;	// average profit rate over all years
double* CT_DCLFQY;	// average yearly price changes per shop
double* CT_DCLBKF;	// average yearly bank failure hazard rate
double* CT_DCLBKT;	// average fraction of banks in trouble

double** TS_DCLGAP; // average time series of logarithmic output gap
double** TS_DCLVOL; // time series of cross-run variance of log output gap

// storage for cross-thread average time paths (calibration and randomize only)
double* CT_TSRGAP;	// time-series average output gap
double* CT_TSRMKP;	// time-series average markup
double* CT_TSRSTM;	// time-series stdev of markup
double* CT_TSRDTR;	// time-series average debt-GDP ratio
double* CT_TSRTXR;	// time-series average tax rate
double* CT_TSRINF;	// time-series simplified average of continuously compounded inflation rate
double* CT_TSRINT;	// time-series simplified average of continuously compounded real interest rate
double* CT_TSRGTG;	// time-series average evolving target of logarithmic output gap
double* CT_TSRRTG;	// time-series average evolving target real interest factor
double* CT_TSRGRM;	// time-series average evolving output gap autocorrelation coefficient
double* CT_TSRIRM;	// time-series average evolving inflation autocorrelation coefficient
double* CT_TSRGPT;  // time-series average evolving estimate of log potential GDP

double** TS_SRTGAP; // time-series of output gap (not averaged across threads)

// storage for all annual observations (each vector has the sample for one year)
vector<double>* CT_SMPGAP; // the output gap sample
vector<double>* CT_SMPINT; // the real interest rate (continuously compounded) sample
vector<double>* CT_SMPINF; // the inflation rate (continuously compounded) sample

double*** TS_SMPGAP;       // array for storing cross-thread time-series of log output gap

// storage for raw data used to calculate cross-thread averages and medians
double** CT_LOGGAP;	// array for storing cross-thread logarithmic output gap
double** CT_GAPVOL;	// array for storing cross-thread gap volatility
double** CT_UNEMPL;	// array for storing cross-thread unemployment rate
double** CT_UNDURN;	// array for storing cross-thread unemployment duration
double** CT_CCRLRT;	// array for storing cross-thread continuously compounded real interest rate
double** CT_CCPIRT;	// array for storing cross-thread continuously compounded inflation rate
double** CT_INFVOL;	// array for storing cross-thread inflation volatility
double** CT_GAPCOR;	// array for storing cross-thread gap autocorrelation
double** CT_INFCOR;	// array for storing cross-thread inflation autocorrelation
double** CT_YLCORD;	// array for storing cross-thread correlation of diffed, logged, loans and nominal gdp
double** CT_JBLSRT;	// array for storing cross-thread weekly job loss rate
double** CT_EXITRT;	// array for storing cross-thread weekly exit rate
double** CT_VNCYRT;	// array for storing cross-thread weekly vacancy rate
double** CT_MARKUP;	// array for storing cross-thread markup at end of run
double** CT_PROFIT;	// array for storing cross-thread profit rate over all years
double** CT_PCFRQY;	// array for storing cross-thread yearly price changes per shop
double** CT_BANKFL; // array for storing cross-thread yearly bank failure hazard rate
double** CT_BANKTR; // array for storing cross-thread average fraction of banks in trouble
bool  ** CT_BADRNS;	// array for storing cross-thread bad runs

// storage for raw data used to calculate cross-thread average time paths
double** CT_TSAGAP;	// array for storing cross-thread time-series of output gap
double** CT_TSAMKP;	// array for storing cross-thread time-series of markup
double** CT_TSASTM;	// array for storing cross-thread time-series of stdev of markup
double** CT_TSADTR;	// array for storing cross-thread time-series of debt-GDP ratio
double** CT_TSATXR;	// array for storing cross-thread time-series of tax rate
double** CT_TSAINF;	// array for storing cross-thread time-series of continuously compounded inflation rate
double** CT_TSAINT;	// array for storing cross-thread time-series of continuously compounded real interest rate
double** CT_TSAGTG;	// array for storing cross-thread time-series of evolving target of logarithmic output gap
double** CT_TSARTG;	// array for storing cross-thread time-series of evolving target real interest factor
double** CT_TSAGRM;	// array for storing cross-thread time-series of evolving output gap autocorrelation coefficient
double** CT_TSAIRM;	// array for storing cross-thread time-series of evolving inflation autocorrelation coefficient
double** CT_TSAGPT;	// array for storing cross-thread time-series of evolving estimate of log potential GDP

// storage for raw data in impulse mode
double*** SH_LOGGAP; // data by thread-run-month on the logarithmic output gap (shocked)
double*** CF_LOGGAP; // data by thread-run-month on the logarithmic output gap (counterfactual)
double*** SH_SHOPFL; // data by thread-run-month on the number of shop failures (shocked)
double*** CF_SHOPFL; // data by thread-run-month on the number of shop failures (counterfactual)
double*** SH_BANKFL; // data by thread-run-month on the hazard of bank failures (shocked)
double*** CF_BANKFL; // data by thread-run-month on the hazard of bank failures (counterfactual)
double*** SH_BANKTR; // data by thread-run-month on the fraction of troubled banks (shocked)
double*** CF_BANKTR; // data by thread-run-month on the fraction of troubled banks (counterfactual)
double*** SH_NENTRY; // data by thread-run-month on the number of new entrants (shocked)
double*** CF_NENTRY; // data by thread-run-month on the number of new entrants (counterfactual)
double**  CF_CYLNGP; // data by thread-run on the cross-year average logarithmic output gap (counterfactual)
bool**    SH_BADRNS; // data by thread-run on the bad run flag (shocked)
bool**    CF_BADRNS; // data by thread-run on the bad run flag (counterfactual)

// storage for cross-run time series statistics in impulse mode
double* IM_AVLGAP;	// time-series of the average logarithmic output gap (shocked minus counterfactual)
double* IM_AVSHFL;	// time-series of the average number of shop failures (shocked minus counterfactual)
double* IM_AVBKFL;	// time-series of the average hazard of bank failures (shocked minus counterfactual)
double* IM_AVBKTR;	// time-series of the average fraction of troubled banks (shocked minus counterfactual)
double* IM_AVENTR;	// time-series of the average number of new entrants (shocked minus counterfactual)

double* IM_SDLGAP;	// time-series of the std. dev. of the logarithmic output gap (shocked minus counterfactual)
double* IM_SDSHFL;	// time-series of the std. dev. of the number of shop failures (shocked minus counterfactual)
double* IM_SDBKFL;	// time-series of the std. dev. of the hazard of bank failures (shocked minus counterfactual)
double* IM_SDBKTR;	// time-series of the std. dev. of the fraction of troubled banks (shocked minus counterfactual)

double* IM_SELGAP;	// time-series of the std. err. of the logarithmic output gap (shocked minus counterfactual)
double* IM_SESHFL;	// time-series of the std. err. of the number of shop failures (shocked minus counterfactual)
double* IM_SEBKFL;	// time-series of the std. err. of the hazard of bank failures (shocked minus counterfactual)
double* IM_SEBKTR;	// time-series of the std. err. of the fraction of troubled banks (shocked minus counterfactual)

double* IM_IQLGAP;	// time-series of the interquartile average logarithmic output gap (shocked minus counterfactual)
double* IM_IQSHFL;	// time-series of the interquartile average number of shop failures (shocked minus counterfactual)
double* IM_IQBKFL;	// time-series of the interquartile average hazard of bank failures (shocked minus counterfactual)
double* IM_IQBKTR;	// time-series of the interquartile average fraction of troubled banks (shocked minus counterfactual)
double* IM_IQENTR;	// time-series of the interquartile average number of new entrants (shocked minus counterfactual)

vector<double>* IM_SMPGAP; // entire sample of log output gap (shocked minus counterfactual) 
vector<double>* IM_SMPENT; // entire sample of number of new entrants (shocked minus counterfactual)

// stuff for generating random parameter values (randomize mode only)
const int ntries = 500;
const int numPrs = 13;
double rnPars[numPrs];
enum   {MAX, INF, BRK, FAI, DMD, TRI, PDE, FXD, RRT, GAP, MAR, COR, ICR};

ofstream HSTOUT;		// output stream for printing to overall history results file
double*  INFARR;		// array for storing inflation targets in history mode
string*  CODARR;		// array for storing country iso codes in history mode
double** AVGARR;		// array for storing average results in experiment mode
double** MDNARR;		// array for storing median results in experiment mode
double** D01ARR;		// array for storing 1st-decile results in experiment mode
double** D02ARR;		// array for storing 2nd-decile results in experiment mode
double** D03ARR;		// array for storing 3rd-decile results in experiment mode
double** D04ARR;		// array for storing 4th-decile results in experiment mode
double** D05ARR;		// array for storing 5th-decile results in experiment mode
double** D06ARR;		// array for storing 6th-decile results in experiment mode
double** D07ARR;		// array for storing 7th-decile results in experiment mode
double** D08ARR;		// array for storing 8th-decile results in experiment mode
double** D09ARR;		// array for storing 9th-decile results in experiment mode
double** D10ARR;		// array for storing worst-decile results in experiment mode
int      SNSDIR = 0;	// direction of adjustment in sensitivity analysis
char*    INICMD;		// the ini file name command argument
