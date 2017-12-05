/***********************************************************************************************
 * BankFail.cpp
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised July 13 2016
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

#undef PRINT_DIAGNOSTICS // flag for printing unemployment details

#include "Economy.h"
#include "Parameters.h"
#include "Statistics.h"
#include "ExpResults.h"
#include "require.h"
#include "RandUtils.h"
#include "Globals.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include <process.h>
#include <windows.h>
#include <time.h>

using namespace std;

/***********************************************************************************************
 * GLOBAL DECLARATIONS
 ***********************************************************************************************/

// global variables are declared and defined in Globals.h

// forward declarations
void do_calib(Parameters* params);
void do_clsim(void* thread);

void do_exper(Parameters* params);
void do_exsim(void* thread);

void do_randm(Parameters* params);
void do_rnsim(void* thread);

void do_impls(Parameters* params);
void do_shsim(void* thread);
void do_cfsim(void* thread);

void do_chist(void);
void do_chsim(void* thread);

void prt_expres(Parameters* params, ofstream& expout, const string hdrarr[], double** resarr);

void ini_ctarrs(Parameters* params);
void cal_ctstat(Parameters* params);
void del_ctarrs(Parameters* params);

void ini_imarrs(Parameters* params);
void cal_imstat(Parameters* params);
void del_imarrs(Parameters* params);

double median(vector<double> &v);

/***********************************************************************************************
 * MAIN FUNCTION
 ***********************************************************************************************/

int main(int argc, char* argv[]) {
    INICMD = argv[1];

    Parameters* PARAMS = new Parameters(INICMD);

    if(strcmp(PARAMS->_OPMODE, "experiment" ) == 0)
        do_exper(PARAMS);
    if(strcmp(PARAMS->_OPMODE, "calibration") == 0)
        do_calib(PARAMS);
    if(strcmp(PARAMS->_OPMODE, "impulse") == 0)
        do_impls(PARAMS);
    if(strcmp(PARAMS->_OPMODE, "sensitivity" ) == 0)
        for (SNSDIR=-1; SNSDIR<2; SNSDIR+=2)
            do_exper(PARAMS);
    if(strcmp(PARAMS->_OPMODE, "randomize") == 0)
        do_randm(PARAMS);
    if(strcmp(PARAMS->_OPMODE, "history") == 0)
        do_chist();

    delete PARAMS;
    PARAMS = NULL;
    return 0;
}

/***********************************************************************************************
 * MANAGER FUNCTION FOR CALIBRATION
 ***********************************************************************************************/

void do_calib(Parameters* params) {
    // start timer
    clock_t  BGNCAL = 0;
    clock_t  ENDCAL = 0;
    BGNCAL = clock();

    // create cross-thread arrays
    ini_ctarrs(params);

    // create mutexes
    CSLMTX = CreateMutex(NULL, FALSE, NULL);
    PTRMTX = CreateMutex(NULL, FALSE, NULL);
    ARRMTX = CreateMutex(NULL, FALSE, NULL);

    HANDLE* THDLST = new HANDLE[params->_NTHRDS];	// thread handles (one for each simulation)
    int*    IDLIST = new int   [params->_NTHRDS];	// thread identifiers (one for each simulation)

    // start the simulation threads
    for (int i = 0; i < params->_NTHRDS; i++) {
        IDLIST[i] = i;
        THDLST[i] = (HANDLE) _beginthread(do_clsim, 0, &IDLIST[i]);
    }

    // wait for all simulations to complete and then print results
    WaitForMultipleObjects(params->_NTHRDS, THDLST, TRUE, INFINITE);

    // calculate cross-thread statistics
    cal_ctstat(params);

    ofstream CALOUT("CAL_RESULTS.TXT", ios_base::out | ios_base::trunc);

    char datbuf[128], timbuf[128];
    _strdate_s( datbuf, 128 ); _strtime_s( timbuf, 128 );
    CALOUT << datbuf << " " << timbuf << endl;

    CALOUT.precision(6);
    CALOUT << fixed;
    CALOUT << "       "
           << setw(10) << "average"
           << setw(12) << "d10avg"
           << setw(12) << "median"
           << setw(12) << "target"
           << setw(12) << "% dev" << endl;

    CALOUT << "  INF: ";
    CALOUT << setw(10) << exp(CT_AVGINF) - 1
           << setw(12) << exp(CT_DCLINF[9]) - 1
           << setw(12) << exp(CT_MDNINF) - 1
           << setw(12) << params->_PITRGY - 1
           << setw(12) << 100*(exp(CT_MDNINF) - params->_PITRGY) << endl;
    CALOUT << "  GAP: ";
    CALOUT << setw(10) << CT_AVGGAP
           << setw(12) << CT_DCLGAP[9]
           << setw(12) << CT_MDNGAP
           << setw(12) << params->_GAPTRG
           << setw(12) << 100*(CT_MDNGAP - params->_GAPTRG) << endl;
    CALOUT << "  UNR: ";
    CALOUT << setw(10) << CT_AVGUNR
           << setw(12) << CT_DCLUNR[9]
           << setw(12) << CT_MDNUNR
           << setw(12) << 0.059
           << setw(12) << 100*(CT_MDNUNR - 0.059) << endl;
    CALOUT << "  VOL: ";
    CALOUT << setw(10) << CT_AVGVOL
           << setw(12) << CT_DCLVOL[9]
           << setw(12) << CT_MDNVOL
           << setw(12) << 0.032
           << setw(12) << 100*(CT_MDNVOL - 0.032) << endl;
    CALOUT << "  IVL: ";
    CALOUT << setw(10) << CT_AVGIVL
           << setw(12) << CT_DCLIVL[9]
           << setw(12) << CT_MDNIVL
           << setw(12) << 0.013
           << setw(12) << 100*(CT_MDNIVL - 0.013) << endl;
    CALOUT << "  BKF: ";
    CALOUT << setw(10) << CT_AVGBKF
           << setw(12) << CT_DCLBKF[9]
           << setw(12) << CT_MDNBKF
           << setw(12) << 0.005 
           << setw(12) << 100*(CT_MDNBKF - 0.005) << endl;
    CALOUT << "  BKT: ";
    CALOUT << setw(10) << CT_AVGBKT
           << setw(12) << CT_DCLBKT[9]
           << setw(12) << CT_MDNBKT << endl;
    CALOUT << "  UDR: ";
    CALOUT << setw(10) << CT_AVGUDR
           << setw(12) << CT_DCLUDR[9]
           << setw(12) << CT_MDNUDR
           << setw(12) << 14.0
           << setw(12) << 100*(CT_MDNUDR - 14.0) / 14.0 << endl;
    CALOUT << "  LSS: ";
    CALOUT << setw(10) << CT_AVGLSS * 100
           << setw(12) << CT_DCLLSS[9] * 100
           << setw(12) << CT_MDNLSS * 100
           << setw(12) << 0.688
           << setw(12) << 100*(CT_MDNLSS * 100 - 0.688) << endl;
    CALOUT << "  EXT: ";
    CALOUT << setw(10) << CT_AVGEXT
           << setw(12) << CT_DCLEXT[9]
           << setw(12) << CT_MDNEXT
           << setw(12) << 0.464
           << setw(12) << 100*(CT_MDNEXT - 0.464) << endl;
    CALOUT << endl;
    CALOUT << "  FQY: ";
    CALOUT << setw(10) << CT_AVGFQY
           << setw(12) << CT_DCLFQY[9]
           << setw(12) << CT_MDNFQY
           << setw(12) << 4.0
           << setw(12) << 100*(CT_MDNFQY - 4.0) / 4.0 << endl;
    CALOUT << "  RRT: ";
    CALOUT << setw(10) << exp(CT_AVGRRT) - 1
           << setw(12) << exp(CT_DCLRRT[9]) - 1
           << setw(12) << exp(CT_MDNRRT) - 1
           << setw(12) << params->_RRTTRG - 1
           << setw(12) << 100*(exp(CT_MDNRRT)-params->_RRTTRG) << endl;
    CALOUT << "  MKP: ";
    CALOUT << setw(10) << CT_AVGMKP - 1
           << setw(12) << CT_DCLMKP[9] - 1
           << setw(12) << CT_MDNMKP - 1
           << setw(12) << params->_MARKUP - 1
           << setw(12) << 100*(CT_MDNMKP - params->_MARKUP) << endl;
    CALOUT << "  COR: ";
    CALOUT << setw(10) << CT_AVGCOR
           << setw(12) << CT_DCLCOR[9]
           << setw(12) << CT_MDNCOR
           << setw(12) << params->_GAPCOR
           << setw(12) << 100*(CT_MDNCOR-params->_GAPCOR) << endl;
    CALOUT << "  ICR: ";
    CALOUT << setw(10) << CT_AVGICR
           << setw(12) << CT_DCLICR[9]
           << setw(12) << CT_MDNICR
           << setw(12) << params->_INFCOR
           << setw(12) << 100*(CT_MDNICR - params->_INFCOR) << endl;
    CALOUT << "  YLC: ";
    CALOUT << setw(10) << CT_AVGYLC
           << setw(12) << CT_DCLYLC[9]
           << setw(12) << CT_MDNYLC
           << setw(12) << .403
           << setw(12) << 100*(CT_MDNYLC - .403) << endl;
    CALOUT << endl;
    CALOUT << "  TOTAL BAD RUNS: "
           << CT_TTLBAD
           << endl
           << endl;

    // print parameters to results file
    ifstream INIFIL(INICMD, ios_base::in);
    string nxline;
    while(getline(INIFIL, nxline)) {
        if (size_t i = nxline.find("//")) {
            nxline = nxline.assign(nxline, 0, i);
            CALOUT << nxline << endl;
        }
    }

    CALOUT.close();
    INIFIL.close();

    // print calibration parameters and cross-thread average statistics
    for (;;) {
        ofstream TSAOUT("CAL_RESULTS_TIMESERIES.CSV", ios_base::out | ios_base::trunc);
        if(!TSAOUT) {
            cout << endl
                 << "Problem writing to CAL_RESULTS_TIMESERIES.CSV" << endl
                 << " Try closing it and hitting [Enter]"           << endl
                 << " or if you don't want this file, just type n"  << endl << endl;
            if (cin.get() == 'n') {
                cout << "Cannot print simulation results to CAL_RESULTS_TIMESERIES.CSV" << endl;
                break;
            }
        }
        else {
            TSAOUT << "year, GAP, MARKUP, DEBT/GDP, TAXRATE, INFLATION, REALINTR, STDEVMKP, GAPTARGET, RRTTARGET, GAPRYR, INFRYR, GDPPOT,"
                << " MDNGAP, MDNRRATE, MDNINF, GAP_D1, GAP_D2, GAP_D3, GAP_D4, GAP_D5, GAP_D6, GAP_D7, GAP_D8, GAP_D9, GAP_D10,"
                << " VOL_D1, VOL_D2, VOL_D3, VOL_D4, VOL_D5, VOL_D6, VOL_D7, VOL_D8, VOL_D9, VOL_D10" << endl;
            for (int i = 0; i < params->_NWEEKS / params->_WKSPYR; i++) {
                TSAOUT << i+1 << ", " << CT_TSRGAP[i] << ", " << CT_TSRMKP[i]-1 
                              << ", " << CT_TSRDTR[i] << ", " << CT_TSRTXR[i] 
                              << ", " << CT_TSRINF[i] << ", " << CT_TSRINT[i] 
                              << ", " << CT_TSRSTM[i] << ", " << CT_TSRGTG[i]
                              << ", " << CT_TSRRTG[i] << ", " << CT_TSRGRM[i]
                              << ", " << CT_TSRIRM[i] << ", " << CT_TSRGPT[i]
                              << ", " << median(CT_SMPGAP[i])
                              << ", " << median(CT_SMPINT[i])
                              << ", " << median(CT_SMPINF[i]);
                for (int d=0; d<10; d++) {
                    TSAOUT << ", " << TS_DCLGAP[d][i];
                }
                for (int d=0; d<10; d++) {
                    TSAOUT << ", " << TS_DCLVOL[d][i];
                }
                TSAOUT << endl;
            }
            TSAOUT.close();
            break;
        }
    }

    // end timer
    ENDCAL = clock();
    double calsec = (double)(ENDCAL - BGNCAL) / CLOCKS_PER_SEC;

    cout << "COMPLETED " << params->_NTHRDS*params->_NSRUNS << " RUNS IN "
         << (int) calsec/60   << " MINUTES "
         << fmod(calsec,60.0) << " SECONDS" << endl;

    // clean up handles
    CloseHandle(CSLMTX);
    CloseHandle(PTRMTX);
    CloseHandle(ARRMTX);

    // clean up dynamically allocated memory
    delete[] THDLST; THDLST = NULL;
    delete[] IDLIST; IDLIST = NULL;

    del_ctarrs(params);
}

/***********************************************************************************************
 * SIMULATION FUNCTION FOR CALIBRATION
 ***********************************************************************************************/

void do_clsim(void* thread) { 
    char BUFFER[50] = "";
    char RSFILE[50] = "";
    int  THREAD = *((int*) thread);

    _itoa_s (THREAD, BUFFER, 65, 10);
    strcat_s(RSFILE, "CAL_RESULTS_THREAD_");
    strcat_s(RSFILE, BUFFER);
    strcat_s(RSFILE, ".csv");

    // print to console one thread at a time
    WaitForSingleObject(CSLMTX, INFINITE);
    cout << "STARTING SIMULATION FOR THREAD " << THREAD << "\n";
    ReleaseMutex(CSLMTX);

    //load parameters one thread at a time
    WaitForSingleObject(PTRMTX, INFINITE);
    Parameters* PARAMS = new Parameters(INICMD);
    ReleaseMutex(PTRMTX);

    // here we make all the remaining changes needed in the Economy's
    // parameters for the rest of the simulation.
    PARAMS->_RNSEED += THREAD * PARAMS->_NSRUNS;
    PARAMS->_THREAD = THREAD;

    // create economy and simulate it
    Economy* ABECON = new Economy(PARAMS, RSFILE);
  Statistics* ECSTAT = ABECON->get_ecStat();
    ABECON->sim_ABECON();

    // print simulation results and compute cross-run stats
    ECSTAT->cal_CRStat();
    ABECON->prt_smStat();

    // store data into cross-thread arrays one thread at a time
    WaitForSingleObject(ARRMTX, INFINITE);
    for (int i = 0; i < PARAMS->_NSRUNS; i++) {
        CT_LOGGAP[THREAD][i] = ECSTAT->_CYLnGp[i];
        CT_GAPVOL[THREAD][i] = ECSTAT->_gapVol[i];
        CT_UNEMPL[THREAD][i] = ECSTAT->_uempRt[i];
        CT_UNDURN[THREAD][i] = ECSTAT->_uempDr[i];
        CT_CCRLRT[THREAD][i] = ECSTAT->_CYCCRR[i];
        CT_CCPIRT[THREAD][i] = ECSTAT->_CYCCPi[i];
        CT_INFVOL[THREAD][i] = ECSTAT->_infVol[i];
        CT_GAPCOR[THREAD][i] = ECSTAT->_gapCor[i];
        CT_INFCOR[THREAD][i] = ECSTAT->_infCor[i];
        CT_YLCORD[THREAD][i] = ECSTAT->_YLCorD[i];
        CT_JBLSRT[THREAD][i] = ECSTAT->_jbLsRt[i];
        CT_EXITRT[THREAD][i] = ECSTAT->_CYExit[i];
        CT_VNCYRT[THREAD][i] = ECSTAT->_vncyRt[i];
        CT_MARKUP[THREAD][i] = ECSTAT->_erWtMk[i];
        CT_PROFIT[THREAD][i] = ECSTAT->_prftRt[i];
        CT_PCFRQY[THREAD][i] = ECSTAT->_pFreqy[i];
        CT_BANKFL[THREAD][i] = ECSTAT->_CYBkFl[i];
        CT_BANKTR[THREAD][i] = ECSTAT->_CYBkTr[i];
        CT_BADRNS[THREAD][i] = ECSTAT->_badRns[i];
        if (!ECSTAT->_badRns[i]) {
            for (int y=0; y < PARAMS->_NWEEKS / PARAMS->_WKSPYR; ++y) {
                CT_SMPGAP[y].push_back(ECSTAT->_RYLnGp[i][y]);
                CT_SMPINT[y].push_back(ECSTAT->_RYCCRR[i][y]);
                CT_SMPINF[y].push_back(ECSTAT->_RYCCPi[i][y]);
                TS_SMPGAP[THREAD][i][y] = ECSTAT->_RYLnGp[i][y];
            }
        }
    }

    for (int i = 0; i < ABECON->get_NYEARS(); i++) {
        CT_TSAGAP[THREAD][i] = ECSTAT->_CRLnGp[i];
        CT_TSAMKP[THREAD][i] = ECSTAT->_CRWtMk[i];
        CT_TSASTM[THREAD][i] = ECSTAT->_CRStMk[i];
        CT_TSADTR[THREAD][i] = ECSTAT->_CRDbtR[i];
        CT_TSATXR[THREAD][i] = ECSTAT->_CRTxRt[i];
        CT_TSAINF[THREAD][i] = exp(ECSTAT->_CRCCPi[i]) - 1.0;
        CT_TSAINT[THREAD][i] = exp(ECSTAT->_CRCCRR[i]) - 1.0;
        CT_TSAGTG[THREAD][i] = ECSTAT->_CRGpTg[i];
        CT_TSARTG[THREAD][i] = ECSTAT->_CRRtTg[i] - 1.0;
        CT_TSAGRM[THREAD][i] = ECSTAT->_CRGpRm[i];
        CT_TSAIRM[THREAD][i] = ECSTAT->_CRPiRm[i];
        CT_TSAGPT[THREAD][i] = ECSTAT->_CRGdPt[i];
    }
    ReleaseMutex(ARRMTX);

    // print to console one thread at a time
    WaitForSingleObject(CSLMTX, INFINITE);
    cout << "COMPLETED SIMULATION FOR THREAD " << THREAD 
         << ". Avg # iterations = "
         << (double) ABECON->get_anvCnt() / (PARAMS->_NSRUNS*ABECON->get_MNSPYR()*ABECON->get_NYEARS())
         << endl;
    ReleaseMutex(CSLMTX);

    // clean up dynamically allocated memory
    delete PARAMS; PARAMS = NULL;
    delete ABECON; ABECON = NULL;

    _endthread(); // kill simulation thread
}

/***********************************************************************************************
 * MANAGER FUNCTION FOR EXPERIMENT
 ***********************************************************************************************/

void do_exper(Parameters* params) {
    // create mutexes
    CSLMTX = CreateMutex(NULL, FALSE, NULL);
    PTRMTX = CreateMutex(NULL, FALSE, NULL);
    ARRMTX = CreateMutex(NULL, FALSE, NULL);

    char RSFILE[50] = "";
    if(strcmp(params->_OPMODE, "experiment" ) == 0) {
        strcat_s(RSFILE, "EXP_RESULTS_");
        strcat_s(RSFILE, params->_EXPVAR);
        strcat_s(RSFILE, ".txt");
    }
    if(strcmp(params->_OPMODE, "sensitivity" ) == 0) {
        strcat_s(RSFILE, "SNS_");
        strcat_s(RSFILE, params->_SNSVAR);
        if(SNSDIR == -1)
            strcat_s(RSFILE, "_L.txt");
        else
            strcat_s(RSFILE, "_H.txt");
    }
  
    // open results file and write header
    // NSTATS and exName[] are defined in ExpResults.h
    ofstream EXPOUT(RSFILE, ios_base::out | ios_base::trunc);
    char datbuf[128], timbuf[128];
    _strdate_s( datbuf, 128 ); _strtime_s( timbuf, 128 );
    EXPOUT << datbuf << " " << timbuf << endl;

    AVGARR = new double*[params->_NTHRDS];
    MDNARR = new double*[params->_NTHRDS];
    D01ARR = new double*[params->_NTHRDS];
    D02ARR = new double*[params->_NTHRDS];
    D03ARR = new double*[params->_NTHRDS];
    D04ARR = new double*[params->_NTHRDS];
    D05ARR = new double*[params->_NTHRDS];
    D06ARR = new double*[params->_NTHRDS];
    D07ARR = new double*[params->_NTHRDS];
    D08ARR = new double*[params->_NTHRDS];
    D09ARR = new double*[params->_NTHRDS];
    D10ARR = new double*[params->_NTHRDS];

    for (int i = 0; i < params->_NTHRDS; i++) {
        AVGARR[i] = new double[NSTATS];
        MDNARR[i] = new double[NSTATS];
        D01ARR[i] = new double[NSTATS];
        D02ARR[i] = new double[NSTATS];
        D03ARR[i] = new double[NSTATS];
        D04ARR[i] = new double[NSTATS];
        D05ARR[i] = new double[NSTATS];
        D06ARR[i] = new double[NSTATS];
        D07ARR[i] = new double[NSTATS];
        D08ARR[i] = new double[NSTATS];
        D09ARR[i] = new double[NSTATS];
        D10ARR[i] = new double[NSTATS];

        for (int j = 0; j < NSTATS; j++) {
            AVGARR[i][j] = -1;
            MDNARR[i][j] = -1;
            D01ARR[i][j] = -1;
            D02ARR[i][j] = -1;
            D03ARR[i][j] = -1;
            D04ARR[i][j] = -1;
            D05ARR[i][j] = -1;
            D06ARR[i][j] = -1;
            D07ARR[i][j] = -1;
            D08ARR[i][j] = -1;
            D09ARR[i][j] = -1;
            D10ARR[i][j] = -1;
        }
    }

    HANDLE* THDLST = new HANDLE[params->_NTHRDS];	// thread handles (one for each simulation)
    int*    IDLIST = new int   [params->_NTHRDS];	// thread identifiers (one for each simulation)

    for (int i = 0; i < params->_NTHRDS; i++) {
        IDLIST[i] = i;
        THDLST[i] = (HANDLE) _beginthread(do_exsim, 0, &IDLIST[i]);
    }

    // wait for all simulations to complete before continuing
    WaitForMultipleObjects(params->_NTHRDS, THDLST, TRUE, INFINITE);

    // print results to results file
    prt_expres(params, EXPOUT, avgVar, AVGARR);
    prt_expres(params, EXPOUT, mdnVar, MDNARR);
    prt_expres(params, EXPOUT, d10Var, D10ARR);
    prt_expres(params, EXPOUT, d09Var, D09ARR);
    prt_expres(params, EXPOUT, d08Var, D08ARR);
    prt_expres(params, EXPOUT, d07Var, D07ARR);
    prt_expres(params, EXPOUT, d06Var, D06ARR);
    prt_expres(params, EXPOUT, d05Var, D05ARR);
    prt_expres(params, EXPOUT, d04Var, D04ARR);
    prt_expres(params, EXPOUT, d03Var, D03ARR);
    prt_expres(params, EXPOUT, d02Var, D02ARR);
    prt_expres(params, EXPOUT, d01Var, D01ARR);

    // print parameters to results file
    ifstream INIFIL(INICMD, ios_base::in);
    string nxline;
    while(getline(INIFIL, nxline)) {
        if (size_t i = nxline.find("//")) {
            nxline = nxline.assign(nxline, 0, i);
            EXPOUT << nxline << endl;
        }
    }

    // close files
    INIFIL.close();
    EXPOUT.close();

    // clean up handles
    CloseHandle(CSLMTX);
    CloseHandle(PTRMTX);
    CloseHandle(ARRMTX);

    // clean up dynamically allocated memory
    delete[] THDLST; THDLST = NULL;
    delete[] IDLIST; IDLIST = NULL;

    for (int i = 0; i < params->_NTHRDS; i++) {
        delete[] AVGARR[i]; AVGARR[i] = NULL;
        delete[] MDNARR[i]; MDNARR[i] = NULL;
        delete[] D01ARR[i]; D01ARR[i] = NULL;
        delete[] D02ARR[i]; D02ARR[i] = NULL;
        delete[] D03ARR[i]; D03ARR[i] = NULL;
        delete[] D04ARR[i]; D04ARR[i] = NULL;
        delete[] D05ARR[i]; D05ARR[i] = NULL;
        delete[] D06ARR[i]; D06ARR[i] = NULL;
        delete[] D07ARR[i]; D07ARR[i] = NULL;
        delete[] D08ARR[i]; D08ARR[i] = NULL;
        delete[] D09ARR[i]; D09ARR[i] = NULL;
        delete[] D10ARR[i]; D10ARR[i] = NULL;
    }
    delete[] AVGARR; AVGARR = NULL;
    delete[] MDNARR; MDNARR = NULL;
    delete[] D01ARR; D01ARR = NULL;
    delete[] D02ARR; D02ARR = NULL;
    delete[] D03ARR; D03ARR = NULL;
    delete[] D04ARR; D04ARR = NULL;
    delete[] D05ARR; D05ARR = NULL;
    delete[] D06ARR; D06ARR = NULL;
    delete[] D07ARR; D07ARR = NULL;
    delete[] D08ARR; D08ARR = NULL;
    delete[] D09ARR; D09ARR = NULL;
    delete[] D10ARR; D10ARR = NULL;
}

/***********************************************************************************************
 * SIMULATION FUNCTION FOR EXPERIMENT
 ***********************************************************************************************/

void do_exsim(void* thread) {
    int THREAD = *((int*) thread);

    // load parameters one thread at a time and make adjustments
    WaitForSingleObject(PTRMTX, INFINITE);
    Parameters* PARAMS = new Parameters(INICMD);
    PARAMS->_THREAD = THREAD;
    if (!PARAMS->adj_EXPVAR()) {
        delete PARAMS;
        cout << "ERROR in Parameters::adj_EXPVAR(). Thread "
             << THREAD << " exiting..." << endl << endl;
        _endthread();
    }
    if(strcmp(PARAMS->_OPMODE, "sensitivity") == 0)
        PARAMS->adj_SNSVAR(SNSDIR);
    if(PARAMS->_SHOCKS && (PARAMS->_USEEND || PARAMS->_USECOR))
        PARAMS->adj_endPrm();
    ReleaseMutex(PTRMTX);

    // create results filename string
    int  FLTDEC = -1;
    int  FLTSGN = -1;
    char BUFFER[50] = "";
    char RSFILE[50] = "";

    _fcvt_s (BUFFER, PARAMS->get_expVal(), 4, &FLTDEC, &FLTSGN);
    strcat_s(RSFILE, "EXP_RESULTS_");
    strcat_s(RSFILE, PARAMS->_EXPVAR);
    if (FLTSGN == 1)
        strcat_s(RSFILE, "_-");
    else
        strcat_s(RSFILE, "_");
    strcat_s(RSFILE, BUFFER);
    strcat_s(RSFILE, ".csv");

    // print to console
    WaitForSingleObject(CSLMTX, INFINITE);
    cout.precision(4);
    cout.setf(ios::fixed | ios::showpoint);
    cout << "STARTING SIMULATION FOR " << PARAMS->_EXPVAR << " = "
         << PARAMS->get_expVal() << "\n";
    ReleaseMutex(CSLMTX);

    // create economy and simulate
    Economy* ABECON = new Economy(PARAMS, RSFILE);
  Statistics* ECSTAT = ABECON->get_ecStat();
    ABECON->sim_ABECON();
  
    // print simulation results and compute cross-run stats
    ECSTAT->cal_CRStat();
    ABECON->prt_smStat();	

    WaitForSingleObject(ARRMTX, INFINITE);
    for (int i = 0; i < NSTATS; i++) {
        AVGARR[THREAD][i] = get_avgRes(PARAMS, ECSTAT, i);
        MDNARR[THREAD][i] = get_mdnRes(PARAMS, ECSTAT, i);
        D01ARR[THREAD][i] = get_d01Res(PARAMS, ECSTAT, i);
        D02ARR[THREAD][i] = get_d02Res(PARAMS, ECSTAT, i);
        D03ARR[THREAD][i] = get_d03Res(PARAMS, ECSTAT, i);
        D04ARR[THREAD][i] = get_d04Res(PARAMS, ECSTAT, i);
        D05ARR[THREAD][i] = get_d05Res(PARAMS, ECSTAT, i);
        D06ARR[THREAD][i] = get_d06Res(PARAMS, ECSTAT, i);
        D07ARR[THREAD][i] = get_d07Res(PARAMS, ECSTAT, i);
        D08ARR[THREAD][i] = get_d08Res(PARAMS, ECSTAT, i);
        D09ARR[THREAD][i] = get_d09Res(PARAMS, ECSTAT, i);
        D10ARR[THREAD][i] = get_d10Res(PARAMS, ECSTAT, i);
    }
    ReleaseMutex(ARRMTX);

    // print to console
    WaitForSingleObject(CSLMTX, INFINITE);
    cout << "COMPLETED SIMULATION FOR " << PARAMS->_EXPVAR << " = "
         << PARAMS->get_expVal() << ". Avg # iterations = "
         << (double) ABECON->get_anvCnt() / (PARAMS->_NSRUNS * ABECON->get_MNSPYR() * ABECON->get_NYEARS())
         << endl;
    ReleaseMutex(CSLMTX);

    // clean up dynamically allocated memory
    delete PARAMS; PARAMS = NULL;
    delete ABECON; ABECON = NULL;

    _endthread(); // kill simulation thread
}

/***********************************************************************************************
 * MANAGER FUNCTION FOR RANDOMIZE
 ***********************************************************************************************/

void do_randm(Parameters* params) {
    // create cross-thread arrays
    ini_ctarrs(params);

    // create mutexes
    CSLMTX = CreateMutex(NULL, FALSE, NULL);
    PTRMTX = CreateMutex(NULL, FALSE, NULL);
    ARRMTX = CreateMutex(NULL, FALSE, NULL);

    // create output file and print headers and targets
    ofstream RNDOUT("RND_RESULTS.CSV", ios_base::out | ios_base::trunc);
    char datbuf[128], timbuf[128];
    _strdate_s( datbuf, 128 ); _strtime_s( timbuf, 128 );
    RNDOUT << datbuf << "," << timbuf << endl;

    RNDOUT  << "   ,"
            << "INF,"
            /*<< "UNR,"
            << "VOL,"
            << "LSS,"
            << "EXT,"
            << "FQY,"
            << "RRT," 
            << "GAP,"
            << "MKP," 
            << "COR,"
            << "ICR,"
            << "MAXSLS,"
            << "INFREQ,"
            << "BRKFRC,"
            << "FAILRT,"
            << "DMDELS,"
            << "TRIGGR,"
            << "PDELTA,"
            << "FXDCST,"*/
            << "RRTTRG,"
            << "GAPTRG,"
            << "MARKUP,"
            << "GAPCOR,"
            << "INFCOR," << endl;

    RNDOUT  << "Targets,"
            << params->_PITRGY - 1 << ","
            /*<< .05               << ","
            << .016258             << ","
            << .688                << ","
            << .464                << ","
            << 4                   << ","
            << " , " << " , " << " , " << " , " << " , "
            << params->_MAXSLS << ","
            << params->_INFREQ << ","
            << params->_BRKFRC << ","
            << params->_FAILRT << ","
            << params->_DMDELS << ","
            << params->_TRIGGR << ","
            << params->_PDELTA << ","
            << params->_FXDCST << ","*/
            << params->_RRTTRG << ","
            << params->_GAPTRG << ","
            << params->_MARKUP << ","
            << params->_GAPCOR << ","
            << params->_INFCOR << "," << endl;

    // create thread handles and IDlist
    HANDLE* THDLST = new HANDLE[params->_NTHRDS];	
    int*    IDLIST = new int   [params->_NTHRDS];

    for (int i = 0; i < params->_NTHRDS; i++)
        IDLIST[i] = i;

    // initialize the stDevs
    double stDevs[numPrs];
    stDevs[MAX] = 0; //0.15;
    stDevs[INF] = 0; //0.15;
    stDevs[BRK] = 0; //0.20;
    stDevs[FAI] = 0; //0.20;
    stDevs[DMD] = 0; //0.11/10;
    stDevs[TRI] = 0; //0.05;
    stDevs[PDE] = 0; //0.05;
    stDevs[FXD] = 0; //0.06/10;
    stDevs[RRT] = 0.00005; //0.003;
    stDevs[GAP] = 0.0001; //0.01;
    stDevs[MAR] = 0; //0.01;
    stDevs[COR] = 0; //0.000025; //0.03;
    stDevs[ICR] = 0; //0.000025; //0.03;
    /*stDevs[MAX] = 0;
    stDevs[INF] = 0;
    stDevs[BRK] = 0;
    stDevs[FAI] = 0;
    stDevs[DMD] = 0;
    stDevs[TRI] = 0;
    stDevs[PDE] = 0;
    stDevs[FXD] = 0;
    stDevs[RRT] = 0;
    stDevs[GAP] = 0;
    stDevs[MAR] = 0;
    stDevs[COR] = 0;
    stDevs[ICR] = 0;*/

    clock_t  BEGTRL = 0;
    clock_t  ENDTRL = 0;

    // start trials
    for (int trial = 0; trial < ntries; trial++) {
        BEGTRL = clock();
        cout << "STARTING RANDOM TRIAL " << trial + 1 << " of " << ntries << endl;

        // generate new random parameter values
        rnPars[MAX] = params->_MAXSLS*(1.0 + stDevs[MAX]*(2.0*RandUtils::get_RnFrac() - 1.0));
        rnPars[INF] = params->_INFREQ*(1.0 + stDevs[INF]*(2.0*RandUtils::get_RnFrac() - 1.0));
        rnPars[BRK] = params->_BRKFRC*(1.0 + stDevs[BRK]*(2.0*RandUtils::get_RnFrac() - 1.0));
        rnPars[FAI] = params->_FAILRT*(1.0 + stDevs[FAI]*(2.0*RandUtils::get_RnFrac() - 1.0));
        rnPars[DMD] = params->_DMDELS*(1.0 + stDevs[DMD]*(2.0*RandUtils::get_RnFrac() - 1.0));
        rnPars[TRI] = params->_TRIGGR*(1.0 + stDevs[TRI]*(2.0*RandUtils::get_RnFrac() - 1.0));
        rnPars[PDE] = params->_PDELTA*(1.0 + stDevs[PDE]*(2.0*RandUtils::get_RnFrac() - 1.0));
        rnPars[FXD] = params->_FXDCST*(1.0 + stDevs[FXD]*(2.0*RandUtils::get_RnFrac() - 1.0));
        rnPars[RRT] = params->_RRTTRG*(1.0 + stDevs[RRT]*(2.0*RandUtils::get_RnFrac() - 1.0));
        rnPars[GAP] = params->_GAPTRG*(1.0 + stDevs[GAP]*(2.0*RandUtils::get_RnFrac() - 1.0));
        rnPars[MAR] = params->_MARKUP*(1.0 + stDevs[MAR]*(2.0*RandUtils::get_RnFrac() - 1.0));
        rnPars[COR] = params->_GAPCOR*(1.0 + stDevs[COR]*(2.0*RandUtils::get_RnFrac() - 1.0));
        rnPars[ICR] = params->_INFCOR*(1.0 + stDevs[ICR]*(2.0*RandUtils::get_RnFrac() - 1.0));
        rnPars[RRT] = params->_RRTTRG + stDevs[RRT]*(2.0*RandUtils::get_RnFrac() - 1.0);
        rnPars[GAP] = params->_GAPTRG + stDevs[GAP]*(2.0*RandUtils::get_RnFrac() - 1.0);

        // start the simulation threads
        for (int i = 0; i < params->_NTHRDS; i++)
            THDLST[i] = (HANDLE) _beginthread(do_rnsim, 0, &IDLIST[i]);

        // wait for all simulations to complete and then print results
        WaitForMultipleObjects(params->_NTHRDS, THDLST, TRUE, INFINITE);

        // calculate cross-thread statistics
        cal_ctstat(params);

        RNDOUT  << trial + 1			<< ","
                << exp(CT_AVGINF)-1		<< ","
                /*<< CT_AVGUNR			<< ","
                << CT_AVGVOL			<< ","
                << CT_AVGLSS*100		<< ","
                << CT_AVGEXT			<< ","
                << CT_AVGFQY			<< ","*/
                << setprecision(7)
                << exp(CT_AVGRRT)-1.0	<< ","
                << setprecision(7)
                << CT_AVGGAP			<< ","
                << CT_AVGMKP - 1		<< ","
                << CT_AVGCOR			<< ","
                << CT_AVGICR			<< ","
                /*<< rnPars[MAX]		<< ","*/
                << rnPars[INF]			<< ","
                /*<< rnPars[BRK]		<< ","
                << rnPars[FAI]			<< ","
                << rnPars[DMD]			<< ","
                << rnPars[TRI]			<< ","
                << rnPars[PDE]			<< ","
                << rnPars[FXD]			<< ","*/
                << rnPars[RRT]			<< ","
                << rnPars[GAP]			<< ","
                << rnPars[MAR]			<< ","
                << rnPars[COR]			<< ","
                << rnPars[ICR]			<< "," << endl;

        ENDTRL = clock();
        cout << "COMPLETED RANDOM TRIAL " << trial + 1 
             << " in " << (double)(ENDTRL - BEGTRL) / CLOCKS_PER_SEC
             << " seconds" << endl << endl;
    }

    RNDOUT.close();

    // clean up handles
    CloseHandle(CSLMTX);
    CloseHandle(PTRMTX);
    CloseHandle(ARRMTX);

    // clean up dynamically allocated memory
    delete[] THDLST; THDLST = NULL;
    delete[] IDLIST; IDLIST = NULL;

    del_ctarrs(params);
}

/***********************************************************************************************
 * SIMULATION FUNCTION FOR RANDOMIZE
 ***********************************************************************************************/

void do_rnsim(void* thread) {
    char BUFFER[50] = "";
    char RSFILE[50] = "";
    int  THREAD = *((int*) thread);

    _itoa_s (THREAD, BUFFER, 65, 10);
    strcat_s(RSFILE, "CAL_RESULTS_THREAD_");
    strcat_s(RSFILE, BUFFER);
    strcat_s(RSFILE, ".csv");

    // print to console one thread at a time
    WaitForSingleObject(CSLMTX, INFINITE);
    cout << "STARTING SIMULATION FOR THREAD " << THREAD << "\n";
    ReleaseMutex(CSLMTX);

    //load parameters one thread at a time
    WaitForSingleObject(PTRMTX, INFINITE);
    Parameters* PARAMS = new Parameters(INICMD);

    // here we make all the remaining changes needed in the Economy's
    // parameters for the rest of the simulation.
    PARAMS->_RNSEED += THREAD * PARAMS->_NSRUNS;
    PARAMS->_THREAD = THREAD;
    PARAMS->_MAXSLS = (int) rnPars[MAX];
    PARAMS->_INFREQ = rnPars[INF];
    PARAMS->_BRKFRC = rnPars[BRK];
    PARAMS->_FAILRT = rnPars[FAI];
    PARAMS->_DMDELS = rnPars[DMD];
    PARAMS->_TRIGGR = rnPars[TRI];
    PARAMS->_PDELTA = rnPars[PDE];
    PARAMS->_FXDCST = rnPars[FXD];
    PARAMS->_RRTTRG = rnPars[RRT];
    PARAMS->_GAPTRG = rnPars[GAP];
    PARAMS->_MARKUP = rnPars[MAR];
    PARAMS->_GAPCOR = rnPars[COR];
    PARAMS->_INFCOR = rnPars[ICR];
    ReleaseMutex(PTRMTX);

    // create economy and simulate it
    Economy* ABECON = new Economy(PARAMS, RSFILE);
  Statistics* ECSTAT = ABECON->get_ecStat();
    ABECON->sim_ABECON();

    // print simulation results and compute cross-run stats
    ECSTAT->cal_CRStat();
    ABECON->prt_smStat();	

    // increment cross-thread averages one thread at a time
    WaitForSingleObject(ARRMTX, INFINITE);
    for (int i = 0; i < PARAMS->_NSRUNS; i++) {
        CT_LOGGAP[THREAD][i] = ECSTAT->_CYLnGp[i];
        CT_GAPVOL[THREAD][i] = ECSTAT->_gapVol[i];
        CT_UNEMPL[THREAD][i] = ECSTAT->_uempRt[i];
        CT_UNDURN[THREAD][i] = ECSTAT->_uempDr[i];
        CT_CCRLRT[THREAD][i] = ECSTAT->_CYCCRR[i];
        CT_CCPIRT[THREAD][i] = ECSTAT->_CYCCPi[i];
        CT_INFVOL[THREAD][i] = ECSTAT->_infVol[i];
        CT_GAPCOR[THREAD][i] = ECSTAT->_gapCor[i];
        CT_INFCOR[THREAD][i] = ECSTAT->_infCor[i];
        CT_YLCORD[THREAD][i] = ECSTAT->_YLCorD[i];
        CT_JBLSRT[THREAD][i] = ECSTAT->_jbLsRt[i];
        CT_EXITRT[THREAD][i] = ECSTAT->_CYExit[i];
        CT_VNCYRT[THREAD][i] = ECSTAT->_vncyRt[i];
        CT_MARKUP[THREAD][i] = ECSTAT->_erWtMk[i];
        CT_PROFIT[THREAD][i] = ECSTAT->_prftRt[i];
        CT_PCFRQY[THREAD][i] = ECSTAT->_pFreqy[i];
        CT_BANKFL[THREAD][i] = ECSTAT->_CYBkFl[i];
        CT_BANKTR[THREAD][i] = ECSTAT->_CYBkTr[i];
        CT_BADRNS[THREAD][i] = ECSTAT->_badRns[i];
    }
    for (int i = 0; i < ABECON->get_NYEARS(); i++) {
        CT_TSAGAP[THREAD][i] = ECSTAT->_CRLnGp[i];
        CT_TSAMKP[THREAD][i] = ECSTAT->_CRWtMk[i];
        CT_TSASTM[THREAD][i] = ECSTAT->_CRStMk[i];
        CT_TSADTR[THREAD][i] = ECSTAT->_CRDbtR[i];
        CT_TSATXR[THREAD][i] = ECSTAT->_CRTxRt[i];
        CT_TSAINF[THREAD][i] = exp(ECSTAT->_CRCCPi[i]) - 1.0;
        CT_TSAINT[THREAD][i] = exp(ECSTAT->_CRCCRR[i]) - 1.0;
        CT_TSAGTG[THREAD][i] = ECSTAT->_CRGpTg[i];
        CT_TSARTG[THREAD][i] = ECSTAT->_CRRtTg[i] - 1.0;
        CT_TSAGRM[THREAD][i] = ECSTAT->_CRGpRm[i];
        CT_TSAIRM[THREAD][i] = ECSTAT->_CRPiRm[i];
        CT_TSAGPT[THREAD][i] = ECSTAT->_CRGdPt[i];
    }
    ReleaseMutex(ARRMTX);

    // clean up dynamically allocated memory
    delete PARAMS; PARAMS = NULL;
    delete ABECON; ABECON = NULL;

    // print to console one thread at a time
    WaitForSingleObject(CSLMTX, INFINITE);
    cout << "COMPLETED SIMULATION FOR THREAD " << THREAD << "\n";
    ReleaseMutex(CSLMTX);

    _endthread(); // kill simulation thread
}

/***********************************************************************************************
 * MANAGER FUNCTION FOR IMPULSE
 ***********************************************************************************************/

void do_impls(Parameters* params) {
    // create global arrays
    ini_imarrs(params);

    // create mutexes
    CSLMTX = CreateMutex(NULL, FALSE, NULL);
    PTRMTX = CreateMutex(NULL, FALSE, NULL);
    ARRMTX = CreateMutex(NULL, FALSE, NULL);

    HANDLE* SHLIST = new HANDLE[params->_NTHRDS];	// thread handles for shocked simulations
    HANDLE* CFLIST = new HANDLE[params->_NTHRDS];	// thread handles for counterfactual simulations
    int*    IDLIST = new int   [params->_NTHRDS];	// thread identifiers

    // start the shocked simulation threads
    for (int i = 0; i < params->_NTHRDS; i++) {
        IDLIST[i] = i;
        SHLIST[i] = (HANDLE) _beginthread(do_shsim, 0, &IDLIST[i]);
    }

    // wait for all shocked simulations to complete before proceeding
    WaitForMultipleObjects(params->_NTHRDS, SHLIST, TRUE, INFINITE);

    // start the counterfactual simulation threads
    for (int i = 0; i < params->_NTHRDS; i++)
        CFLIST[i] = (HANDLE) _beginthread(do_cfsim, 0, &IDLIST[i]);

    // wait for all counterfactual simulations to complete before proceeding
    WaitForMultipleObjects(params->_NTHRDS, CFLIST, TRUE, INFINITE);

    // calculate impulse statistics
    cal_imstat(params);

    // print impulse statistics
    for (;;) {
        ofstream TSAOUT("IMP_RESULTS_TIMESERIES.CSV", ios_base::out | ios_base::trunc);
        if (!TSAOUT) {
            cout << endl
                 << "Problem writing to IMP_RESULTS_TIMESERIES.CSV" << endl
                 << " Try closing it and hitting [Enter]"           << endl
                 << " or if you don't want this file, just type n"  << endl << endl;
            if (cin.get() == 'n') {
                cout << "Cannot print simulation results to CAL_RESULTS_TIMESERIES.CSV" << endl;
                break;
            }
        }
        else {
            TSAOUT << "MONTH, "
                   << "AVLGAP, IQLGAP, SDLGAP, SELGAP, AVSHFL, IQSHFL, SDSHFL, SESHFL, "
                   << "AVBKFL, IQBKFL, SDBKFL, SEBKFL, AVBKTR, IQBKTR, SDBKTR, SEBKTR, AVENTR, IQENTR, MDNGAP, MDENTR" << endl;
      for (int i = params->_IMPLAG * params->_WKSPYR/params->_WKSPMN - 1;
               i < params->_NWEEKS / params->_WKSPMN; i++)
                TSAOUT << i + 1 << ", " << IM_AVLGAP[i] << ", " << IM_IQLGAP[i] << ", " << IM_SDLGAP[i] << ", " << IM_SELGAP[i]
                                << ", " << IM_AVSHFL[i] << ", " << IM_IQSHFL[i] << ", " << IM_SDSHFL[i] << ", " << IM_SESHFL[i]
                              << ", " << IM_AVBKFL[i] << ", " << IM_IQBKFL[i] << ", " << IM_SDBKFL[i] << ", " << IM_SEBKFL[i]
                                        << ", " << IM_AVBKTR[i] << ", " << IM_IQBKTR[i] << ", " << IM_SDBKTR[i] << ", " << IM_SEBKTR[i]
                        << ", " << IM_AVENTR[i] << ", " << IM_IQENTR[i] << ", " << median(IM_SMPGAP[i]) << ", " << median(IM_SMPENT[i]) << endl;
            TSAOUT.close();
            break;
        }
    }

    // clean up handles
    CloseHandle(CSLMTX);
    CloseHandle(PTRMTX);
    CloseHandle(ARRMTX);

    // clean up dynamically allocated memory
    delete[] CFLIST; CFLIST = NULL;
    delete[] SHLIST; SHLIST = NULL;
    delete[] IDLIST; IDLIST = NULL;

    del_imarrs(params);
}

/***********************************************************************************************
 * SIMULATION FUNCTION FOR IMPULSE (SHOCKED)
 ***********************************************************************************************/

void do_shsim(void* thread) {
    char BUFFER[50] = "";
    char RSFILE[50] = "";
    int  THREAD = *((int*) thread);

    _itoa_s (THREAD, BUFFER, 65, 10);
    strcat_s(RSFILE, "IMP_RESULTS_SHK_THREAD_");
    strcat_s(RSFILE, BUFFER);
    strcat_s(RSFILE, ".csv");

    // print to console one thread at a time
    WaitForSingleObject(CSLMTX, INFINITE);
    cout << "STARTING SHOCKED SIMULATION ON THREAD " << THREAD << "\n";
    ReleaseMutex(CSLMTX);

    //load parameters one thread at a time
    WaitForSingleObject(PTRMTX, INFINITE);
    Parameters* PARAMS = new Parameters(INICMD);
    ReleaseMutex(PTRMTX);

    // here we make all the remaining changes needed in the Economy's
    // parameters for the rest of the simulation.
    PARAMS->_RNSEED += THREAD * PARAMS->_NSRUNS;
    PARAMS->_THREAD  = THREAD;

    // create economy and simulate it
    Economy* ABECON = new Economy(PARAMS, RSFILE);
  Statistics* ECSTAT = ABECON->get_ecStat();
    ABECON->set_implse(true);
    ABECON->sim_ABECON();

    // print simulation results and compute cross-run stats
    ECSTAT->cal_CRStat();
    ABECON->prt_smStat();

    // store data into global arrays one thread at a time
    WaitForSingleObject(ARRMTX, INFINITE);
    for (int i = 0; i < PARAMS->_NSRUNS; i++) {
        for (int j = 0; j < ABECON->get_NMNTHS(); j++) {
            SH_LOGGAP[THREAD][i][j] = ECSTAT->_RMLnGp[i][j];
            SH_SHOPFL[THREAD][i][j] = ECSTAT->_RMFail[i][j];
            SH_BANKFL[THREAD][i][j] = ECSTAT->_RMBkFl[i][j];
            SH_BANKTR[THREAD][i][j] = ECSTAT->_RMBkTr[i][j];
            SH_NENTRY[THREAD][i][j] = ECSTAT->_RMEntr[i][j];
        }
        SH_BADRNS[THREAD][i] = ECSTAT->_badRns[i];
    }
    ReleaseMutex(ARRMTX);

    // print to console one thread at a time
    WaitForSingleObject(CSLMTX, INFINITE);
    cout << "COMPLETED SHOCKED SIMULATION ON THREAD " << THREAD 
         << ". Avg # iterations = "
         << (double) ABECON->get_anvCnt() / (PARAMS->_NSRUNS * ABECON->get_MNSPYR() * ABECON->get_NYEARS())
         << endl;
    ReleaseMutex(CSLMTX);

    // clean up dynamically allocated memory
    delete PARAMS; PARAMS = NULL;
    delete ABECON; ABECON = NULL;

    _endthread(); // kill simulation thread
}

/***********************************************************************************************
 * SIMULATION FUNCTION FOR IMPULSE (COUNTERFACTUAL)
 ***********************************************************************************************/

void do_cfsim(void* thread) {
    char BUFFER[50] = "";
    char RSFILE[50] = "";
    int  THREAD = *((int*) thread);

    _itoa_s (THREAD, BUFFER, 65, 10);
    strcat_s(RSFILE, "IMP_RESULTS_CFL_THREAD_");
    strcat_s(RSFILE, BUFFER);
    strcat_s(RSFILE, ".csv");

    // print to console one thread at a time
    WaitForSingleObject(CSLMTX, INFINITE);
    cout << "STARTING COUNTERFACTUAL SIMULATION ON THREAD " << THREAD << "\n";
    ReleaseMutex(CSLMTX);

    //load parameters one thread at a time
    WaitForSingleObject(PTRMTX, INFINITE);
    Parameters* PARAMS = new Parameters(INICMD);
    ReleaseMutex(PTRMTX);

    // here we make all the remaining changes needed in the Economy's
    // parameters for the rest of the simulation.
    PARAMS->_RNSEED += THREAD * PARAMS->_NSRUNS;
    PARAMS->_THREAD  = THREAD;

    // create economy and simulate it
    Economy* ABECON = new Economy(PARAMS, RSFILE);
  Statistics* ECSTAT = ABECON->get_ecStat();
    ABECON->set_implse(false);
    ABECON->sim_ABECON();

    // print simulation results and compute cross-run stats
    ECSTAT->cal_CRStat();
    ABECON->prt_smStat();

    // store data into global arrays one thread at a time
    WaitForSingleObject(ARRMTX, INFINITE);
    for (int i = 0; i < PARAMS->_NSRUNS; i++) {
        for (int j = 0; j < ABECON->get_NMNTHS(); j++) {
            CF_LOGGAP[THREAD][i][j] = ECSTAT->_RMLnGp[i][j];
            CF_SHOPFL[THREAD][i][j] = ECSTAT->_RMFail[i][j];
            CF_BANKFL[THREAD][i][j] = ECSTAT->_RMBkFl[i][j];
            CF_BANKTR[THREAD][i][j] = ECSTAT->_RMBkTr[i][j];
            CF_NENTRY[THREAD][i][j] = ECSTAT->_RMEntr[i][j];
        }
        CF_CYLNGP[THREAD][i] = ECSTAT->_CYLnGp[i];
        CF_BADRNS[THREAD][i] = ECSTAT->_badRns[i];
    }
    ReleaseMutex(ARRMTX);

    // print to console one thread at a time
    WaitForSingleObject(CSLMTX, INFINITE);
    cout << "COMPLETED COUNTERFACTUAL SIMULATION ON THREAD " << THREAD 
         << ". Avg # iterations = "
         << (double) ABECON->get_anvCnt() / (PARAMS->_NSRUNS * ABECON->get_MNSPYR() * ABECON->get_NYEARS())
         << endl;
    ReleaseMutex(CSLMTX);

    // clean up dynamically allocated memory
    delete PARAMS; PARAMS = NULL;
    delete ABECON; ABECON = NULL;

    _endthread(); // kill simulation thread
}

/***********************************************************************************************
 * MANAGER FUNCTION FOR HISTORY
 ***********************************************************************************************/

void do_chist(void) {
    int   NTHRDS = 0;
    int   ARRCTR = 0;

    FILE* flStrm;
    char  flLine[256];
    char* cdTokn;
    char* piTokn;
    char* contxt;

    if (fopen_s(&flStrm, "CTYINF.ini", "r") == 0) {
        while (fgets(flLine, 256, flStrm) != NULL) NTHRDS++;
        fclose(flStrm);
    }
    else {
        fprintf(stderr, "Could not open file CTYINF.ini\n");
        exit(1);
    }

    INFARR = new double[NTHRDS];
    CODARR = new string[NTHRDS];

    if (fopen_s(&flStrm, "CTYINF.ini", "r") == 0) {    
        while (fgets(flLine, 256, flStrm) != NULL) {
            cdTokn = strtok_s(flLine, "\t"  , &contxt);
            piTokn = strtok_s(NULL  , "\t\n", &contxt);

            INFARR[ARRCTR] = atof(piTokn);
            CODARR[ARRCTR] = cdTokn;
            ARRCTR++;
        }
        fclose(flStrm);
    }
    else {
        fprintf(stderr, "Could not open file CTYINF.ini\n");
        exit(1);
    }

    CSLMTX = CreateMutex(NULL, FALSE, NULL);
    PTRMTX = CreateMutex(NULL, FALSE, NULL);
    FILMTX = CreateMutex(NULL, FALSE, NULL);

    HSTOUT.open("HST_RESULTS.txt", ios_base::out | ios_base::trunc);
    HSTOUT << setw(6 ) << "WBCODE" << setw(12) << "HSTNUM"
                                   << setw(16) << "HSTGAP"
                                   << setw(16) << "HSTINF" << endl;

    HANDLE* THDLST = new HANDLE[NTHRDS];
    int*    IDLIST = new int   [NTHRDS];

    for (int i = 0; i < NTHRDS; i++) {
        IDLIST[i] = i;
        THDLST[i] = (HANDLE) _beginthread(do_chsim, 0, &IDLIST[i]);
    }

    WaitForMultipleObjects(NTHRDS, THDLST, TRUE, INFINITE);

    HSTOUT.close();

    CloseHandle(CSLMTX);
    CloseHandle(PTRMTX);
    CloseHandle(FILMTX);

    delete[] THDLST; THDLST = NULL;
    delete[] IDLIST; IDLIST = NULL;
    delete[] INFARR; INFARR = NULL;
    delete[] CODARR; CODARR = NULL;
}

/***********************************************************************************************
 * SIMULATION FUNCTION FOR HISTORY
 ***********************************************************************************************/

void do_chsim(void* thread) {
    int THREAD = *((int*) thread);

    WaitForSingleObject(PTRMTX, INFINITE);
    Parameters* PARAMS = new Parameters(INICMD);
    PARAMS->_THREAD = THREAD;
    PARAMS->_PITRGY = INFARR[THREAD];
    if(PARAMS->_SHOCKS && (PARAMS->_USEEND || PARAMS->_USECOR))
        PARAMS->adj_endPrm(INFARR[THREAD]);
    ReleaseMutex(PTRMTX);

    WaitForSingleObject(FILMTX, INFINITE);
    char RSFILE[50] = "HST_RESULTS_COUNTRY_";
    strcat_s(RSFILE, CODARR[THREAD].c_str());
    strcat_s(RSFILE, ".csv");
    ReleaseMutex(FILMTX);

    WaitForSingleObject(CSLMTX, INFINITE);
    cout << "STARTING SIMULATION FOR COUNTRY \"" << CODARR[THREAD]
         << "\" ON THREAD " << THREAD << "\n";
    ReleaseMutex(CSLMTX);

    Economy* ABECON = new Economy(PARAMS, RSFILE);
  Statistics* ECSTAT = ABECON->get_ecStat();
    ABECON->sim_ABECON();
    ECSTAT->cal_CRStat();
    ABECON->prt_smStat();

    WaitForSingleObject(FILMTX, INFINITE);
    for (int i = 0; i < PARAMS->_NSRUNS; i++) {
        HSTOUT << setw(6 ) << CODARR[THREAD]
               << setw(12) << i + 1
               << setw(16) << setprecision(8) << setiosflags(ios::fixed | ios::showpoint)
                           << 1.0 - ECSTAT->_CYIncm[i]/ABECON->get_CAPGDP()
               << setw(16) << setprecision(8) << setiosflags(ios::fixed | ios::showpoint)
                           << ECSTAT->_CYInfl[i] << endl;
    }
    ReleaseMutex(FILMTX);

    WaitForSingleObject(CSLMTX, INFINITE);
    cout << "COMPLETED SIMULATION FOR COUNTRY \"" << CODARR[THREAD]
         << "\" ON THREAD " << THREAD << "\n";
    ReleaseMutex(CSLMTX);

    delete PARAMS; PARAMS = NULL;
    delete ABECON; ABECON = NULL;

    _endthread();
}

/***********************************************************************************************
 * FUNCTION FOR PRINTING EXPERIMENT RESULTS
 ***********************************************************************************************/

void prt_expres(Parameters* params, ofstream& expout, const string hdrarr[], double** resarr) {
    expout << setiosflags(ios::right) << setw(12) << params->_EXPVAR;
    for (int i = 1; i < NSTATS; i++)
        expout << setw(12) << hdrarr[i];
    expout << endl;
    expout << setiosflags(ios::fixed);
    for (int i = 0; i < params->_NTHRDS; i++) {
        for (int j = 0; j < NSTATS; j++) {
            if (j == 0)
                expout << setprecision(6) << setw(12) << resarr[i][j];
            else
                expout << setprecision(6) << setw(12) << resarr[i][j];
        }
        expout << endl;
    }
    expout << endl;
}

/***********************************************************************************************
 * FUNCTION FOR CREATING CROSS-THREAD ARRAYS
 ***********************************************************************************************/

void ini_ctarrs(Parameters* params) {
    CT_LOGGAP = new double*[params->_NTHRDS];
    CT_GAPVOL = new double*[params->_NTHRDS];
    CT_UNEMPL = new double*[params->_NTHRDS];
    CT_UNDURN = new double*[params->_NTHRDS];
    CT_CCRLRT = new double*[params->_NTHRDS];
    CT_CCPIRT = new double*[params->_NTHRDS];
    CT_INFVOL = new double*[params->_NTHRDS];
    CT_GAPCOR = new double*[params->_NTHRDS];
    CT_INFCOR = new double*[params->_NTHRDS];
    CT_YLCORD = new double*[params->_NTHRDS];
    CT_JBLSRT = new double*[params->_NTHRDS];
    CT_EXITRT = new double*[params->_NTHRDS];
    CT_VNCYRT = new double*[params->_NTHRDS];
    CT_MARKUP = new double*[params->_NTHRDS];
    CT_PROFIT = new double*[params->_NTHRDS];
    CT_PCFRQY = new double*[params->_NTHRDS];
    CT_BANKFL = new double*[params->_NTHRDS];
    CT_BANKTR = new double*[params->_NTHRDS];
    CT_BADRNS = new bool  *[params->_NTHRDS];

    for (int i = 0; i < params->_NTHRDS; i++) {
        CT_LOGGAP[i] = new double[params->_NSRUNS];
        CT_GAPVOL[i] = new double[params->_NSRUNS];
        CT_UNEMPL[i] = new double[params->_NSRUNS];
        CT_UNDURN[i] = new double[params->_NSRUNS];
        CT_CCRLRT[i] = new double[params->_NSRUNS];
        CT_CCPIRT[i] = new double[params->_NSRUNS];
        CT_INFVOL[i] = new double[params->_NSRUNS];
        CT_GAPCOR[i] = new double[params->_NSRUNS];
        CT_INFCOR[i] = new double[params->_NSRUNS];
        CT_YLCORD[i] = new double[params->_NSRUNS];
        CT_JBLSRT[i] = new double[params->_NSRUNS];
        CT_EXITRT[i] = new double[params->_NSRUNS];
        CT_VNCYRT[i] = new double[params->_NSRUNS];
        CT_MARKUP[i] = new double[params->_NSRUNS];
        CT_PROFIT[i] = new double[params->_NSRUNS];
        CT_PCFRQY[i] = new double[params->_NSRUNS];
        CT_BANKFL[i] = new double[params->_NSRUNS];
        CT_BANKTR[i] = new double[params->_NSRUNS];
        CT_BADRNS[i] = new bool  [params->_NSRUNS];

        for (int j = 0; j < params->_NSRUNS; j++) {
            CT_LOGGAP[i][j] = 0.0; CT_GAPVOL[i][j] = 0.0; CT_UNEMPL[i][j] = 0.0;
            CT_CCRLRT[i][j] = 0.0; CT_CCPIRT[i][j] = 0.0; CT_INFVOL[i][j] = 0.0;
            CT_GAPCOR[i][j] = 0.0; CT_INFCOR[i][j] = 0.0; CT_JBLSRT[i][j] = 0.0;
            CT_EXITRT[i][j] = 0.0; CT_VNCYRT[i][j] = 0.0; CT_MARKUP[i][j] = 0.0;
            CT_PROFIT[i][j] = 0.0; CT_PCFRQY[i][j] = 0.0; CT_UNDURN[i][j] = 0.0;
            CT_BANKFL[i][j] = 0.0; CT_BANKTR[i][j] = 0.0; CT_YLCORD[i][j] = 0.0;
            CT_BADRNS[i][j] = false;
        }
    }

    CT_TSAGAP = new double*[params->_NTHRDS];
    CT_TSAMKP = new double*[params->_NTHRDS];
    CT_TSASTM = new double*[params->_NTHRDS];
    CT_TSADTR = new double*[params->_NTHRDS];
    CT_TSATXR = new double*[params->_NTHRDS];
    CT_TSAINF = new double*[params->_NTHRDS];
    CT_TSAINT = new double*[params->_NTHRDS];
    CT_TSAGTG = new double*[params->_NTHRDS];
    CT_TSARTG = new double*[params->_NTHRDS];
    CT_TSAGRM = new double*[params->_NTHRDS];
    CT_TSAIRM = new double*[params->_NTHRDS];
    CT_TSAGPT = new double*[params->_NTHRDS];

    for (int i = 0; i < params->_NTHRDS; i++) {
        CT_TSAGAP[i] = new double[params->_NWEEKS / params->_WKSPYR];
        CT_TSAMKP[i] = new double[params->_NWEEKS / params->_WKSPYR];
        CT_TSASTM[i] = new double[params->_NWEEKS / params->_WKSPYR];
        CT_TSADTR[i] = new double[params->_NWEEKS / params->_WKSPYR];
        CT_TSATXR[i] = new double[params->_NWEEKS / params->_WKSPYR];
        CT_TSAINF[i] = new double[params->_NWEEKS / params->_WKSPYR];
        CT_TSAINT[i] = new double[params->_NWEEKS / params->_WKSPYR];
        CT_TSAGTG[i] = new double[params->_NWEEKS / params->_WKSPYR];
        CT_TSARTG[i] = new double[params->_NWEEKS / params->_WKSPYR];
        CT_TSAGRM[i] = new double[params->_NWEEKS / params->_WKSPYR];
        CT_TSAIRM[i] = new double[params->_NWEEKS / params->_WKSPYR];
        CT_TSAGPT[i] = new double[params->_NWEEKS / params->_WKSPYR];

        for (int j = 0; j < params->_NWEEKS / params->_WKSPYR; j++) {
            CT_TSAGAP[i][j] = 0.0; CT_TSAMKP[i][j] = 0.0;
            CT_TSASTM[i][j] = 0.0; CT_TSADTR[i][j] = 0.0;
            CT_TSATXR[i][j] = 0.0; CT_TSAINF[i][j] = 0.0;
            CT_TSAINT[i][j] = 0.0; CT_TSAGTG[i][j] = 0.0;
            CT_TSARTG[i][j] = 0.0; CT_TSAGRM[i][j] = 0.0;
            CT_TSAIRM[i][j] = 0.0; CT_TSAGPT[i][j] = 0.0;
        }
    }

    CT_TSRGAP = new double[params->_NWEEKS / params->_WKSPYR];
    CT_TSRMKP = new double[params->_NWEEKS / params->_WKSPYR];
    CT_TSRSTM = new double[params->_NWEEKS / params->_WKSPYR];
    CT_TSRDTR = new double[params->_NWEEKS / params->_WKSPYR];
    CT_TSRTXR = new double[params->_NWEEKS / params->_WKSPYR];
    CT_TSRINF = new double[params->_NWEEKS / params->_WKSPYR];
    CT_TSRINT = new double[params->_NWEEKS / params->_WKSPYR];
    CT_TSRGTG = new double[params->_NWEEKS / params->_WKSPYR];
    CT_TSRRTG = new double[params->_NWEEKS / params->_WKSPYR];
    CT_TSRGRM = new double[params->_NWEEKS / params->_WKSPYR];
    CT_TSRIRM = new double[params->_NWEEKS / params->_WKSPYR];
    CT_TSRGPT = new double[params->_NWEEKS / params->_WKSPYR];

    for (int i = 0; i < params->_NWEEKS / params->_WKSPYR; i++) {
        CT_TSRGAP[i] = 0.0; CT_TSRMKP[i] = 0.0;
        CT_TSRSTM[i] = 0.0; CT_TSRDTR[i] = 0.0;
        CT_TSRTXR[i] = 0.0; CT_TSRINF[i] = 0.0;
        CT_TSRINT[i] = 0.0; CT_TSRGTG[i] = 0.0;
        CT_TSRRTG[i] = 0.0; CT_TSRGRM[i] = 0.0;
        CT_TSRIRM[i] = 0.0; CT_TSRGPT[i] = 0.0;
    }

    CT_SMPGAP = new vector<double>[params->_NWEEKS / params->_WKSPYR];
    CT_SMPINT = new vector<double>[params->_NWEEKS / params->_WKSPYR];
    CT_SMPINF = new vector<double>[params->_NWEEKS / params->_WKSPYR];

    TS_SMPGAP = new double**[params->_NTHRDS];
    for (int t=0; t<params->_NTHRDS; t++) {
        TS_SMPGAP[t] = new double*[params->_NSRUNS];
        for (int r=0; r<params->_NSRUNS; r++) {
            TS_SMPGAP[t][r] = new double[params->_NWEEKS / params->_WKSPYR];
            for (int y=0; y<params->_NWEEKS / params->_WKSPYR; y++) {
                TS_SMPGAP[t][r][y] = 0.0;
            }
        }
    }

    for (int y = 0; y < params->_NWEEKS / params->_WKSPYR; ++y) {
        CT_SMPGAP[y].reserve(params->_NTHRDS * params->_NSRUNS);
        CT_SMPINT[y].reserve(params->_NTHRDS * params->_NSRUNS);
        CT_SMPINF[y].reserve(params->_NTHRDS * params->_NSRUNS);
    }

    CT_DCLGAP = new double[10];
    CT_DCLVOL = new double[10];
    CT_DCLUNR = new double[10];
    CT_DCLUDR = new double[10];
    CT_DCLRRT = new double[10];
    CT_DCLINF = new double[10];
    CT_DCLIVL = new double[10];
    CT_DCLCOR = new double[10];
    CT_DCLICR = new double[10];
    CT_DCLYLC = new double[10];
    CT_DCLLSS = new double[10];
    CT_DCLEXT = new double[10];
    CT_DCLVAC = new double[10];
    CT_DCLMKP = new double[10];
    CT_DCLPFT = new double[10];
    CT_DCLFQY = new double[10];
    CT_DCLBKF = new double[10];
    CT_DCLBKT = new double[10];

    TS_DCLGAP = new double*[10];
    TS_DCLVOL = new double*[10];
    for (int d=0; d<10; d++) {
      TS_DCLGAP[d] = new double[params->_NWEEKS/params->_WKSPYR];
      TS_DCLVOL[d] = new double[params->_NWEEKS/params->_WKSPYR];
    }

    for (int i = 0; i < 10; i++) {
        CT_DCLGAP[i] = 0.0; CT_DCLVOL[i] = 0.0;
        CT_DCLUNR[i] = 0.0; CT_DCLUDR[i] = 0.0;
        CT_DCLRRT[i] = 0.0; CT_DCLINF[i] = 0.0;
        CT_DCLIVL[i] = 0.0; CT_DCLCOR[i] = 0.0;
        CT_DCLICR[i] = 0.0; CT_DCLLSS[i] = 0.0;
        CT_DCLEXT[i] = 0.0; CT_DCLVAC[i] = 0.0;
        CT_DCLMKP[i] = 0.0; CT_DCLPFT[i] = 0.0;
        CT_DCLFQY[i] = 0.0; CT_DCLBKF[i] = 0.0;
        CT_DCLBKT[i] = 0.0; CT_DCLYLC[i] = 0.0;
        for (int y=0; y<params->_NWEEKS/params->_WKSPYR; y++) {
            TS_DCLGAP[i][y] = 0.0;
            TS_DCLVOL[i][y] = 0.0;
        }
    }
}

/***********************************************************************************************
 * FUNCTION FOR CALCULATING CROSS-THREAD STATISTICS
 ***********************************************************************************************/

void cal_ctstat(Parameters* params) {
    CT_AVGGAP = 0.0; CT_AVGVOL = 0.0; CT_AVGUNR = 0.0;
    CT_AVGRRT = 0.0; CT_AVGINF = 0.0; CT_AVGIVL = 0.0;
    CT_AVGCOR = 0.0; CT_AVGICR = 0.0; CT_AVGLSS = 0.0;
    CT_AVGEXT = 0.0; CT_AVGVAC = 0.0; CT_AVGMKP = 0.0;
    CT_PRFTRT = 0.0; CT_AVGFQY = 0.0; CT_AVGUDR = 0.0;
    CT_AVGBKF = 0.0; CT_AVGBKT = 0.0; CT_AVGYLC = 0.0;

    CT_MDNGAP = 0.0; CT_MDNVOL = 0.0; CT_MDNUNR = 0.0;
    CT_MDNRRT = 0.0; CT_MDNINF = 0.0; CT_MDNIVL = 0.0;
    CT_MDNCOR = 0.0; CT_MDNICR = 0.0; CT_MDNLSS = 0.0;
    CT_MDNEXT = 0.0; CT_MDNVAC = 0.0; CT_MDNMKP = 0.0;
    CT_MDNPFT = 0.0; CT_MDNFQY = 0.0; CT_MDNUDR = 0.0;
    CT_MDNBKF = 0.0; CT_MDNBKT = 0.0; CT_MDNYLC = 0.0;

    for (int i = 0; i < 10; i++) {
        CT_DCLGAP[i] = 0.0; CT_DCLVOL[i] = 0.0;
        CT_DCLUNR[i] = 0.0; CT_DCLUDR[i] = 0.0;
        CT_DCLRRT[i] = 0.0; CT_DCLINF[i] = 0.0;
        CT_DCLIVL[i] = 0.0; CT_DCLCOR[i] = 0.0;
        CT_DCLICR[i] = 0.0; CT_DCLLSS[i] = 0.0;
        CT_DCLEXT[i] = 0.0; CT_DCLVAC[i] = 0.0;
        CT_DCLMKP[i] = 0.0; CT_DCLPFT[i] = 0.0;
        CT_DCLFQY[i] = 0.0; CT_DCLBKF[i] = 0.0;
        CT_DCLBKT[i] = 0.0; CT_DCLYLC[i] = 0.0;
        for (int y=0; y<params->_NWEEKS/params->_WKSPYR; y++) {
            TS_DCLGAP[i][y] = 0.0;
            TS_DCLVOL[i][y] = 0.0;
        }
    }

    for (int i = 0; i < params->_NWEEKS / params->_WKSPYR; i++) {
        CT_TSRGAP[i] = 0.0; CT_TSRMKP[i] = 0.0;
        CT_TSRSTM[i] = 0.0; CT_TSRDTR[i] = 0.0;
        CT_TSRTXR[i] = 0.0; CT_TSRINF[i] = 0.0;
        CT_TSRINT[i] = 0.0; CT_TSRGTG[i] = 0.0;
        CT_TSRRTG[i] = 0.0; CT_TSRGRM[i] = 0.0;
        CT_TSRIRM[i] = 0.0; CT_TSRGPT[i] = 0.0;
    }

    CT_TTLBAD = 0;
    for (int i = 0; i < params->_NTHRDS; i++)
        for (int j = 0; j < params->_NSRUNS; j++)
            if (CT_BADRNS[i][j])
                CT_TTLBAD++;

    double* CT_SRTGAP = new double[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];
    double* CT_SRTVOL = new double[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];
    double* CT_SRTUNR = new double[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];
    double* CT_SRTUDR = new double[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];
    double* CT_SRTRRT = new double[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];
    double* CT_SRTINF = new double[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];
    double* CT_SRTIVL = new double[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];
    double* CT_SRTCOR = new double[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];
    double* CT_SRTICR = new double[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];
    double* CT_SRTYLC = new double[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];
    double* CT_SRTLSS = new double[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];
    double* CT_SRTEXT = new double[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];
    double* CT_SRTVAC = new double[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];
    double* CT_SRTMKP = new double[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];
    double* CT_SRTPFT = new double[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];
    double* CT_SRTFQY = new double[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];
    double* CT_SRTBKF = new double[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];
    double* CT_SRTBKT = new double[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];

    double** TS_SRTGAP = new double*[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];
    for (int r=0; r<params->_NTHRDS * params->_NSRUNS - CT_TTLBAD; r++) {
        TS_SRTGAP[r] = new double[params->_NWEEKS / params->_WKSPYR];
    }

    for (int i = 0, k = 0; i < params->_NTHRDS; i++) {
        for (int j = 0; j < params->_NSRUNS; j++) {
            if (CT_BADRNS[i][j])
                continue;

            CT_AVGGAP += CT_LOGGAP[i][j] / (params->_NTHRDS * params->_NSRUNS - CT_TTLBAD);
            CT_AVGVOL += CT_GAPVOL[i][j] / (params->_NTHRDS * params->_NSRUNS - CT_TTLBAD);
            CT_AVGUNR += CT_UNEMPL[i][j] / (params->_NTHRDS * params->_NSRUNS - CT_TTLBAD);
            CT_AVGUDR += CT_UNDURN[i][j] / (params->_NTHRDS * params->_NSRUNS - CT_TTLBAD);
            CT_AVGRRT += CT_CCRLRT[i][j] / (params->_NTHRDS * params->_NSRUNS - CT_TTLBAD);
            CT_AVGINF += CT_CCPIRT[i][j] / (params->_NTHRDS * params->_NSRUNS - CT_TTLBAD);
            CT_AVGIVL += CT_INFVOL[i][j] / (params->_NTHRDS * params->_NSRUNS - CT_TTLBAD);
            CT_AVGCOR += CT_GAPCOR[i][j] / (params->_NTHRDS * params->_NSRUNS - CT_TTLBAD);
            CT_AVGICR += CT_INFCOR[i][j] / (params->_NTHRDS * params->_NSRUNS - CT_TTLBAD);
            CT_AVGYLC += CT_YLCORD[i][j] / (params->_NTHRDS * params->_NSRUNS - CT_TTLBAD);
            CT_AVGLSS += CT_JBLSRT[i][j] / (params->_NTHRDS * params->_NSRUNS - CT_TTLBAD);
            CT_AVGEXT += CT_EXITRT[i][j] / (params->_NTHRDS * params->_NSRUNS - CT_TTLBAD);
            CT_AVGVAC += CT_VNCYRT[i][j] / (params->_NTHRDS * params->_NSRUNS - CT_TTLBAD);
            CT_AVGMKP += CT_MARKUP[i][j] / (params->_NTHRDS * params->_NSRUNS - CT_TTLBAD);
            CT_PRFTRT += CT_PROFIT[i][j] / (params->_NTHRDS * params->_NSRUNS - CT_TTLBAD);
            CT_AVGFQY += CT_PCFRQY[i][j] / (params->_NTHRDS * params->_NSRUNS - CT_TTLBAD);
            CT_AVGBKF += CT_BANKFL[i][j] / (params->_NTHRDS * params->_NSRUNS - CT_TTLBAD);
            CT_AVGBKT += CT_BANKTR[i][j] / (params->_NTHRDS * params->_NSRUNS - CT_TTLBAD);

            CT_SRTGAP[k] = CT_LOGGAP[i][j];
            CT_SRTVOL[k] = CT_GAPVOL[i][j];
            CT_SRTUNR[k] = CT_UNEMPL[i][j];
            CT_SRTUDR[k] = CT_UNDURN[i][j];
            CT_SRTRRT[k] = CT_CCRLRT[i][j];
            CT_SRTINF[k] = CT_CCPIRT[i][j];
            CT_SRTIVL[k] = CT_INFVOL[i][j];
            CT_SRTCOR[k] = CT_GAPCOR[i][j];
            CT_SRTICR[k] = CT_INFCOR[i][j];
            CT_SRTYLC[k] = CT_YLCORD[i][j];
            CT_SRTLSS[k] = CT_JBLSRT[i][j];
            CT_SRTEXT[k] = CT_EXITRT[i][j];
            CT_SRTVAC[k] = CT_VNCYRT[i][j];
            CT_SRTMKP[k] = CT_MARKUP[i][j];
            CT_SRTPFT[k] = CT_PROFIT[i][j];
            CT_SRTFQY[k] = CT_PCFRQY[i][j];
            CT_SRTBKF[k] = CT_BANKFL[i][j];
            CT_SRTBKT[k] = CT_BANKTR[i][j];

            for (int y=0; y<params->_NWEEKS/params->_WKSPYR; y++) {
                TS_SRTGAP[k][y] = TS_SMPGAP[i][j][y];
            }
            k++;
        }
    }

    int* CT_GAPIDX = new int[params->_NTHRDS * params->_NSRUNS - CT_TTLBAD];
    srt_idxArr(CT_SRTGAP, CT_GAPIDX, params->_NTHRDS * params->_NSRUNS - CT_TTLBAD);

    // compute averages by decile of the log output gap distribution
    for (int i = 0; i < 10; i++) {
        int CT_BGNIDX = (int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) * (1 - 0.1 * i)) - 1;
        int CT_ENDIDX = (int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) * (1 - 0.1 * (i + 1))) - 1;

        for (int j = CT_BGNIDX; j > CT_ENDIDX; j--) {
            CT_DCLGAP[i] += CT_SRTGAP[CT_GAPIDX[j]] / (CT_BGNIDX - CT_ENDIDX);
            CT_DCLVOL[i] += CT_SRTVOL[CT_GAPIDX[j]] / (CT_BGNIDX - CT_ENDIDX);
            CT_DCLUNR[i] += CT_SRTUNR[CT_GAPIDX[j]] / (CT_BGNIDX - CT_ENDIDX);
            CT_DCLUDR[i] += CT_SRTUDR[CT_GAPIDX[j]] / (CT_BGNIDX - CT_ENDIDX);
            CT_DCLRRT[i] += CT_SRTRRT[CT_GAPIDX[j]] / (CT_BGNIDX - CT_ENDIDX);
            CT_DCLINF[i] += CT_SRTINF[CT_GAPIDX[j]] / (CT_BGNIDX - CT_ENDIDX);
            CT_DCLIVL[i] += CT_SRTIVL[CT_GAPIDX[j]] / (CT_BGNIDX - CT_ENDIDX);
            CT_DCLCOR[i] += CT_SRTCOR[CT_GAPIDX[j]] / (CT_BGNIDX - CT_ENDIDX);
            CT_DCLICR[i] += CT_SRTICR[CT_GAPIDX[j]] / (CT_BGNIDX - CT_ENDIDX);
            CT_DCLYLC[i] += CT_SRTYLC[CT_GAPIDX[j]] / (CT_BGNIDX - CT_ENDIDX);
            CT_DCLLSS[i] += CT_SRTLSS[CT_GAPIDX[j]] / (CT_BGNIDX - CT_ENDIDX);
            CT_DCLEXT[i] += CT_SRTEXT[CT_GAPIDX[j]] / (CT_BGNIDX - CT_ENDIDX);
            CT_DCLVAC[i] += CT_SRTVAC[CT_GAPIDX[j]] / (CT_BGNIDX - CT_ENDIDX);
            CT_DCLMKP[i] += CT_SRTMKP[CT_GAPIDX[j]] / (CT_BGNIDX - CT_ENDIDX);
            CT_DCLPFT[i] += CT_SRTPFT[CT_GAPIDX[j]] / (CT_BGNIDX - CT_ENDIDX);
            CT_DCLFQY[i] += CT_SRTFQY[CT_GAPIDX[j]] / (CT_BGNIDX - CT_ENDIDX);
            CT_DCLBKF[i] += CT_SRTBKF[CT_GAPIDX[j]] / (CT_BGNIDX - CT_ENDIDX);
            CT_DCLBKT[i] += CT_SRTBKT[CT_GAPIDX[j]] / (CT_BGNIDX - CT_ENDIDX);
            for (int y=0; y<params->_NWEEKS/params->_WKSPYR; y++) {
                TS_DCLGAP[i][y] += TS_SRTGAP[CT_GAPIDX[j]][y] / (CT_BGNIDX - CT_ENDIDX);
            }
        }
    }
    
    // compute the cross-run variance of the output gap for each decile and year
    for (int i = 0; i < 10; i++) {
        int CT_BGNIDX = (int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) * (1 - 0.1 * i)) - 1;
        int CT_ENDIDX = (int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) * (1 - 0.1 * (i + 1))) - 1;

        for (int j = CT_BGNIDX; j > CT_ENDIDX; j--) {
            for (int y=0; y<params->_NWEEKS/params->_WKSPYR; y++) {
                TS_DCLVOL[i][y] += pow(TS_SRTGAP[CT_GAPIDX[j]][y] - TS_DCLGAP[i][y], 2) / (CT_BGNIDX - CT_ENDIDX);
            }
        }
    }

    qsort(CT_SRTGAP, params->_NTHRDS * params->_NSRUNS - CT_TTLBAD, sizeof(double), qst_cmpFnc);
    qsort(CT_SRTVOL, params->_NTHRDS * params->_NSRUNS - CT_TTLBAD, sizeof(double), qst_cmpFnc);
    qsort(CT_SRTUNR, params->_NTHRDS * params->_NSRUNS - CT_TTLBAD, sizeof(double), qst_cmpFnc);
    qsort(CT_SRTUDR, params->_NTHRDS * params->_NSRUNS - CT_TTLBAD, sizeof(double), qst_cmpFnc);
    qsort(CT_SRTRRT, params->_NTHRDS * params->_NSRUNS - CT_TTLBAD, sizeof(double), qst_cmpFnc);
    qsort(CT_SRTINF, params->_NTHRDS * params->_NSRUNS - CT_TTLBAD, sizeof(double), qst_cmpFnc);
    qsort(CT_SRTIVL, params->_NTHRDS * params->_NSRUNS - CT_TTLBAD, sizeof(double), qst_cmpFnc);
    qsort(CT_SRTCOR, params->_NTHRDS * params->_NSRUNS - CT_TTLBAD, sizeof(double), qst_cmpFnc);
    qsort(CT_SRTICR, params->_NTHRDS * params->_NSRUNS - CT_TTLBAD, sizeof(double), qst_cmpFnc);
    qsort(CT_SRTYLC, params->_NTHRDS * params->_NSRUNS - CT_TTLBAD, sizeof(double), qst_cmpFnc);
    qsort(CT_SRTLSS, params->_NTHRDS * params->_NSRUNS - CT_TTLBAD, sizeof(double), qst_cmpFnc);
    qsort(CT_SRTEXT, params->_NTHRDS * params->_NSRUNS - CT_TTLBAD, sizeof(double), qst_cmpFnc);
    qsort(CT_SRTVAC, params->_NTHRDS * params->_NSRUNS - CT_TTLBAD, sizeof(double), qst_cmpFnc);
    qsort(CT_SRTMKP, params->_NTHRDS * params->_NSRUNS - CT_TTLBAD, sizeof(double), qst_cmpFnc);
    qsort(CT_SRTPFT, params->_NTHRDS * params->_NSRUNS - CT_TTLBAD, sizeof(double), qst_cmpFnc);
    qsort(CT_SRTFQY, params->_NTHRDS * params->_NSRUNS - CT_TTLBAD, sizeof(double), qst_cmpFnc);
    qsort(CT_SRTBKF, params->_NTHRDS * params->_NSRUNS - CT_TTLBAD, sizeof(double), qst_cmpFnc);
    qsort(CT_SRTBKT, params->_NTHRDS * params->_NSRUNS - CT_TTLBAD, sizeof(double), qst_cmpFnc);

    CT_MDNGAP = CT_SRTGAP[(int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) / 2)];
    CT_MDNVOL = CT_SRTVOL[(int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) / 2)];
    CT_MDNUNR = CT_SRTUNR[(int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) / 2)];
    CT_MDNUDR = CT_SRTUDR[(int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) / 2)];
    CT_MDNRRT = CT_SRTRRT[(int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) / 2)];
    CT_MDNINF = CT_SRTINF[(int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) / 2)];
    CT_MDNIVL = CT_SRTIVL[(int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) / 2)];
    CT_MDNCOR = CT_SRTCOR[(int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) / 2)];
    CT_MDNICR = CT_SRTICR[(int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) / 2)];
    CT_MDNYLC = CT_SRTYLC[(int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) / 2)];
    CT_MDNLSS = CT_SRTLSS[(int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) / 2)];
    CT_MDNEXT = CT_SRTEXT[(int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) / 2)];
    CT_MDNVAC = CT_SRTVAC[(int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) / 2)];
    CT_MDNMKP = CT_SRTMKP[(int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) / 2)];
    CT_MDNPFT = CT_SRTPFT[(int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) / 2)];
    CT_MDNFQY = CT_SRTFQY[(int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) / 2)];
    CT_MDNBKF = CT_SRTBKF[(int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) / 2)];
    CT_MDNBKT = CT_SRTBKT[(int)((params->_NTHRDS * params->_NSRUNS - CT_TTLBAD) / 2)];

    double* thdwts = new double[params->_NTHRDS];
    double  ttlwts = 0.0;
    for (int i = 0; i < params->_NTHRDS; i++)
        thdwts[i] = params->_NSRUNS;
    for (int i = 0; i < params->_NTHRDS; i++)
        for (int j = 0; j < params->_NSRUNS; j++)
            if (CT_BADRNS[i][j])
                thdwts[i]--;
    for (int i = 0; i < params->_NTHRDS; i++) {
        thdwts[i] /= params->_NSRUNS;
        ttlwts    += thdwts[i];
    }
    for (int i = 0; i < params->_NTHRDS; i++)
        thdwts[i] /= ttlwts;

    for (int i = 0; i < params->_NTHRDS; i++) {
        for (int j = 0; j < params->_NWEEKS / params->_WKSPYR; j++) {
            CT_TSRGAP[j] += thdwts[i] * CT_TSAGAP[i][j];
            CT_TSRMKP[j] += thdwts[i] * CT_TSAMKP[i][j];
            CT_TSRSTM[j] += thdwts[i] * CT_TSASTM[i][j];
            CT_TSRDTR[j] += thdwts[i] * CT_TSADTR[i][j];
            CT_TSRTXR[j] += thdwts[i] * CT_TSATXR[i][j];
            CT_TSRINF[j] += thdwts[i] * CT_TSAINF[i][j];
            CT_TSRINT[j] += thdwts[i] * CT_TSAINT[i][j];
            CT_TSRGTG[j] += thdwts[i] * CT_TSAGTG[i][j];
            CT_TSRRTG[j] += thdwts[i] * CT_TSARTG[i][j];
            CT_TSRGRM[j] += thdwts[i] * CT_TSAGRM[i][j];
            CT_TSRIRM[j] += thdwts[i] * CT_TSAIRM[i][j];
            CT_TSRGPT[j] += thdwts[i] * CT_TSAGPT[i][j];
        }
    }

    delete[] thdwts; thdwts = NULL;

    delete[] CT_SRTGAP; CT_SRTGAP = NULL;
    delete[] CT_SRTVOL; CT_SRTVOL = NULL;
    delete[] CT_SRTUNR; CT_SRTUNR = NULL;
    delete[] CT_SRTUDR; CT_SRTUDR = NULL;
    delete[] CT_SRTRRT; CT_SRTRRT = NULL;
    delete[] CT_SRTINF; CT_SRTINF = NULL;
    delete[] CT_SRTIVL; CT_SRTIVL = NULL;
    delete[] CT_SRTCOR; CT_SRTCOR = NULL;
    delete[] CT_SRTICR; CT_SRTICR = NULL;
    delete[] CT_SRTYLC; CT_SRTYLC = NULL;
    delete[] CT_SRTLSS; CT_SRTLSS = NULL;
    delete[] CT_SRTEXT; CT_SRTEXT = NULL;
    delete[] CT_SRTVAC; CT_SRTVAC = NULL;
    delete[] CT_SRTMKP; CT_SRTMKP = NULL;
    delete[] CT_SRTPFT; CT_SRTPFT = NULL;
    delete[] CT_SRTFQY; CT_SRTFQY = NULL;
    delete[] CT_SRTBKF; CT_SRTBKF = NULL;
    delete[] CT_SRTBKT; CT_SRTBKT = NULL;
    delete[] CT_GAPIDX; CT_GAPIDX = NULL;

    for(int r=0; r<params->_NTHRDS*params->_NSRUNS - CT_TTLBAD; r++) {
        delete[] TS_SRTGAP[r]; TS_SRTGAP[r] = NULL;
    }
    delete[] TS_SRTGAP; TS_SRTGAP = NULL;

}

/***********************************************************************************************
 * FUNCTION FOR CLEANING CROSS-THREAD ARRAYS
 ***********************************************************************************************/

void del_ctarrs(Parameters* params) {
    for (int i = 0; i < params->_NTHRDS; i++) {
        delete[] CT_LOGGAP[i]; CT_LOGGAP[i] = NULL;
        delete[] CT_GAPVOL[i]; CT_GAPVOL[i] = NULL;
        delete[] CT_UNEMPL[i]; CT_UNEMPL[i] = NULL;
        delete[] CT_UNDURN[i]; CT_UNDURN[i] = NULL;
        delete[] CT_CCRLRT[i]; CT_CCRLRT[i] = NULL;
        delete[] CT_CCPIRT[i]; CT_CCPIRT[i] = NULL;
        delete[] CT_INFVOL[i]; CT_INFVOL[i] = NULL;
        delete[] CT_GAPCOR[i]; CT_GAPCOR[i] = NULL;
        delete[] CT_INFCOR[i]; CT_INFCOR[i] = NULL;
        delete[] CT_YLCORD[i]; CT_YLCORD[i] = NULL;
        delete[] CT_JBLSRT[i]; CT_JBLSRT[i] = NULL;
        delete[] CT_EXITRT[i]; CT_EXITRT[i] = NULL;
        delete[] CT_VNCYRT[i]; CT_VNCYRT[i] = NULL;
        delete[] CT_MARKUP[i]; CT_MARKUP[i] = NULL;
        delete[] CT_PROFIT[i]; CT_PROFIT[i] = NULL;
        delete[] CT_PCFRQY[i]; CT_PCFRQY[i] = NULL;
        delete[] CT_BANKFL[i]; CT_BANKFL[i] = NULL;
        delete[] CT_BANKTR[i]; CT_BANKTR[i] = NULL;
        delete[] CT_BADRNS[i]; CT_BADRNS[i] = NULL;

        delete[] CT_TSAGAP[i]; CT_TSAGAP[i] = NULL;
        delete[] CT_TSAMKP[i]; CT_TSAMKP[i] = NULL;
        delete[] CT_TSASTM[i]; CT_TSASTM[i] = NULL;
        delete[] CT_TSADTR[i]; CT_TSADTR[i] = NULL;
        delete[] CT_TSATXR[i]; CT_TSATXR[i] = NULL;
        delete[] CT_TSAINF[i]; CT_TSAINF[i] = NULL;
        delete[] CT_TSAINT[i]; CT_TSAINT[i] = NULL;
        delete[] CT_TSAGTG[i]; CT_TSAGTG[i] = NULL;
        delete[] CT_TSARTG[i]; CT_TSARTG[i] = NULL;
        delete[] CT_TSAGRM[i]; CT_TSAGRM[i] = NULL;
        delete[] CT_TSAIRM[i]; CT_TSAIRM[i] = NULL;
        delete[] CT_TSAGPT[i]; CT_TSAGPT[i] = NULL;

        for(int r=0; r<params->_NSRUNS; r++) {
            delete[] TS_SMPGAP[i][r]; TS_SMPGAP[i][r] = NULL;
        }
        delete[] TS_SMPGAP[i]; TS_SMPGAP[i] = NULL;
    }

    delete[] CT_LOGGAP; CT_LOGGAP = NULL;
    delete[] CT_GAPVOL; CT_GAPVOL = NULL;
    delete[] CT_UNEMPL; CT_UNEMPL = NULL;
    delete[] CT_UNDURN; CT_UNDURN = NULL;
    delete[] CT_CCRLRT; CT_CCRLRT = NULL;
    delete[] CT_CCPIRT; CT_CCPIRT = NULL;
    delete[] CT_INFVOL; CT_INFVOL = NULL;
    delete[] CT_GAPCOR; CT_GAPCOR = NULL;
    delete[] CT_INFCOR; CT_INFCOR = NULL;
    delete[] CT_YLCORD; CT_YLCORD = NULL;
    delete[] CT_JBLSRT; CT_JBLSRT = NULL;
    delete[] CT_EXITRT; CT_EXITRT = NULL;
    delete[] CT_VNCYRT; CT_VNCYRT = NULL;
    delete[] CT_MARKUP; CT_MARKUP = NULL;
    delete[] CT_PROFIT; CT_PROFIT = NULL;
    delete[] CT_PCFRQY; CT_PCFRQY = NULL;
    delete[] CT_BANKFL; CT_BANKFL = NULL;
    delete[] CT_BANKTR; CT_BANKTR = NULL;
    delete[] CT_BADRNS; CT_BADRNS = NULL;

    delete[] CT_TSAGAP; CT_TSAGAP = NULL;
    delete[] CT_TSAMKP; CT_TSAMKP = NULL;
    delete[] CT_TSASTM; CT_TSASTM = NULL;
    delete[] CT_TSADTR; CT_TSADTR = NULL;
    delete[] CT_TSATXR; CT_TSATXR = NULL;
    delete[] CT_TSAINF; CT_TSAINF = NULL;
    delete[] CT_TSAINT; CT_TSAINT = NULL;
    delete[] CT_TSAGTG; CT_TSAGTG = NULL;
    delete[] CT_TSARTG; CT_TSARTG = NULL;
    delete[] CT_TSAGRM; CT_TSAGRM = NULL;
    delete[] CT_TSAIRM; CT_TSAIRM = NULL;
    delete[] CT_TSAGPT; CT_TSAGPT = NULL;

    delete[] CT_TSRGAP; CT_TSRGAP = NULL;
    delete[] CT_TSRMKP; CT_TSRMKP = NULL;
    delete[] CT_TSRSTM; CT_TSRSTM = NULL;
    delete[] CT_TSRDTR; CT_TSRDTR = NULL;
    delete[] CT_TSRTXR; CT_TSRTXR = NULL;
    delete[] CT_TSRINF; CT_TSRINF = NULL;
    delete[] CT_TSRINT; CT_TSRINT = NULL;
    delete[] CT_TSRGTG; CT_TSRGTG = NULL;
    delete[] CT_TSRRTG; CT_TSRRTG = NULL;
    delete[] CT_TSRGRM; CT_TSRGRM = NULL;
    delete[] CT_TSRIRM; CT_TSRIRM = NULL;
    delete[] CT_TSRGPT; CT_TSRGPT = NULL;

    for(int d=0; d<10; d++) {
        delete[] TS_DCLGAP[d]; TS_DCLGAP[d] = NULL;
        delete[] TS_DCLVOL[d]; TS_DCLVOL[d] = NULL;
    }
    delete[] TS_DCLGAP; TS_DCLGAP = NULL;
    delete[] TS_DCLVOL; TS_DCLVOL = NULL;

    delete[] CT_SMPGAP; CT_SMPGAP = NULL;
    delete[] CT_SMPINT; CT_SMPINT = NULL;
    delete[] CT_SMPINF; CT_SMPINF = NULL;

    delete[] CT_DCLGAP; CT_DCLGAP = NULL;
    delete[] CT_DCLVOL; CT_DCLVOL = NULL;
    delete[] CT_DCLUNR; CT_DCLUNR = NULL;
    delete[] CT_DCLUDR; CT_DCLUDR = NULL;
    delete[] CT_DCLRRT; CT_DCLRRT = NULL;
    delete[] CT_DCLINF; CT_DCLINF = NULL;
    delete[] CT_DCLIVL; CT_DCLIVL = NULL;
    delete[] CT_DCLCOR; CT_DCLCOR = NULL;
    delete[] CT_DCLICR; CT_DCLICR = NULL;
    delete[] CT_DCLYLC; CT_DCLYLC = NULL;
    delete[] CT_DCLLSS; CT_DCLLSS = NULL;
    delete[] CT_DCLEXT; CT_DCLEXT = NULL;
    delete[] CT_DCLVAC; CT_DCLVAC = NULL;
    delete[] CT_DCLMKP; CT_DCLMKP = NULL;
    delete[] CT_DCLPFT; CT_DCLPFT = NULL;
    delete[] CT_DCLFQY; CT_DCLFQY = NULL;
    delete[] CT_DCLBKF; CT_DCLBKF = NULL;
    delete[] CT_DCLBKT; CT_DCLBKT = NULL;

    delete[] TS_SMPGAP; TS_SMPGAP = NULL;
}

/***********************************************************************************************
 * FUNCTION FOR CREATING GLOBAL ARRAYS IN IMPULSE MODE
 ***********************************************************************************************/

void ini_imarrs(Parameters* params) {
    SH_LOGGAP = new double**[params->_NTHRDS];
    CF_LOGGAP = new double**[params->_NTHRDS];
    SH_SHOPFL = new double**[params->_NTHRDS];
    CF_SHOPFL = new double**[params->_NTHRDS];
    SH_BANKFL = new double**[params->_NTHRDS];
    CF_BANKFL = new double**[params->_NTHRDS];
    SH_BANKTR = new double**[params->_NTHRDS];
    CF_BANKTR = new double**[params->_NTHRDS];
    SH_NENTRY = new double**[params->_NTHRDS];
    CF_NENTRY = new double**[params->_NTHRDS];

    CF_CYLNGP = new double*[params->_NTHRDS];

    SH_BADRNS = new bool*[params->_NTHRDS];
    CF_BADRNS = new bool*[params->_NTHRDS];

    for (int i = 0; i < params->_NTHRDS; i++) {
        SH_LOGGAP[i] = new double*[params->_NSRUNS];
        CF_LOGGAP[i] = new double*[params->_NSRUNS];
        SH_SHOPFL[i] = new double*[params->_NSRUNS];
        CF_SHOPFL[i] = new double*[params->_NSRUNS];
        SH_BANKFL[i] = new double*[params->_NSRUNS];
        CF_BANKFL[i] = new double*[params->_NSRUNS];
        SH_BANKTR[i] = new double*[params->_NSRUNS];
        CF_BANKTR[i] = new double*[params->_NSRUNS];
        SH_NENTRY[i] = new double*[params->_NSRUNS];
        CF_NENTRY[i] = new double*[params->_NSRUNS];

        CF_CYLNGP[i] = new double[params->_NSRUNS];

        SH_BADRNS[i] = new bool[params->_NSRUNS];
        CF_BADRNS[i] = new bool[params->_NSRUNS];

        for (int j = 0; j < params->_NSRUNS; j++) {
            SH_LOGGAP[i][j] = new double[params->_NWEEKS / params->_WKSPMN];
            CF_LOGGAP[i][j] = new double[params->_NWEEKS / params->_WKSPMN];
            SH_SHOPFL[i][j] = new double[params->_NWEEKS / params->_WKSPMN];
            CF_SHOPFL[i][j] = new double[params->_NWEEKS / params->_WKSPMN];
            SH_BANKFL[i][j] = new double[params->_NWEEKS / params->_WKSPMN];
            CF_BANKFL[i][j] = new double[params->_NWEEKS / params->_WKSPMN];
            SH_BANKTR[i][j] = new double[params->_NWEEKS / params->_WKSPMN];
            CF_BANKTR[i][j] = new double[params->_NWEEKS / params->_WKSPMN];
            SH_NENTRY[i][j] = new double[params->_NWEEKS / params->_WKSPMN];
            CF_NENTRY[i][j] = new double[params->_NWEEKS / params->_WKSPMN];

            CF_CYLNGP[i][j] = 0.0;

            SH_BADRNS[i][j] = false;
            CF_BADRNS[i][j] = false;

            for (int k = 0; k < params->_NWEEKS / params->_WKSPMN; k++) {
                SH_LOGGAP[i][j][k] = 0.0;
                CF_LOGGAP[i][j][k] = 0.0;
                SH_SHOPFL[i][j][k] = 0.0;
                CF_SHOPFL[i][j][k] = 0.0;
                SH_BANKFL[i][j][k] = 0.0;
                CF_BANKFL[i][j][k] = 0.0;
                SH_BANKTR[i][j][k] = 0.0;
                CF_BANKTR[i][j][k] = 0.0;
                SH_NENTRY[i][j][k] = 0.0;
                CF_NENTRY[i][j][k] = 0.0;
            }
        }
    }

    IM_AVLGAP = new double[params->_NWEEKS / params->_WKSPMN];
    IM_AVSHFL = new double[params->_NWEEKS / params->_WKSPMN];
    IM_AVBKFL = new double[params->_NWEEKS / params->_WKSPMN];
    IM_AVBKTR = new double[params->_NWEEKS / params->_WKSPMN];
    IM_AVENTR = new double[params->_NWEEKS / params->_WKSPMN];

    IM_SDLGAP = new double[params->_NWEEKS / params->_WKSPMN];
    IM_SDSHFL = new double[params->_NWEEKS / params->_WKSPMN];
    IM_SDBKFL = new double[params->_NWEEKS / params->_WKSPMN];
    IM_SDBKTR = new double[params->_NWEEKS / params->_WKSPMN];

    IM_SELGAP = new double[params->_NWEEKS / params->_WKSPMN];
    IM_SESHFL = new double[params->_NWEEKS / params->_WKSPMN];
    IM_SEBKFL = new double[params->_NWEEKS / params->_WKSPMN];
    IM_SEBKTR = new double[params->_NWEEKS / params->_WKSPMN];

    IM_IQLGAP = new double[params->_NWEEKS / params->_WKSPMN];
    IM_IQSHFL = new double[params->_NWEEKS / params->_WKSPMN];
    IM_IQBKFL = new double[params->_NWEEKS / params->_WKSPMN];
    IM_IQBKTR = new double[params->_NWEEKS / params->_WKSPMN];
    IM_IQENTR = new double[params->_NWEEKS / params->_WKSPMN];

    for (int i = 0; i < params->_NWEEKS / params->_WKSPMN; i++) {
        IM_AVLGAP[i] = 0.0;
        IM_AVSHFL[i] = 0.0;
        IM_AVBKFL[i] = 0.0;
        IM_AVBKTR[i] = 0.0;
        IM_AVENTR[i] = 0.0;

        IM_SDLGAP[i] = 0.0;
        IM_SDSHFL[i] = 0.0;
        IM_SDBKFL[i] = 0.0;
        IM_SDBKTR[i] = 0.0;

        IM_SELGAP[i] = 0.0;
        IM_SESHFL[i] = 0.0;
        IM_SEBKFL[i] = 0.0;
        IM_SEBKTR[i] = 0.0;

        IM_IQLGAP[i] = 0.0;
        IM_IQSHFL[i] = 0.0;
        IM_IQBKFL[i] = 0.0;
        IM_IQBKTR[i] = 0.0;
        IM_IQENTR[i] = 0.0;
    }

    IM_SMPGAP = new vector<double>[params->_NWEEKS / params->_WKSPMN];
    IM_SMPENT = new vector<double>[params->_NWEEKS / params->_WKSPMN];

    for (int i = 0; i < params->_NWEEKS / params->_WKSPMN; i++) {
        IM_SMPGAP[i].reserve(params->_NTHRDS * params->_NSRUNS);
        IM_SMPENT[i].reserve(params->_NTHRDS * params->_NSRUNS);
    }
}

/***********************************************************************************************
 * FUNCTION FOR CALCULATING IMPULSE STATISTICS
 ***********************************************************************************************/

void cal_imstat(Parameters* params) {
    int      IM_TTLBAD = 0;
    int      IM_VALRNS = 0;
    double** CF_SRTGAP = NULL;

    for (int i = 0; i < params->_NTHRDS; i++)
        for (int j = 0; j < params->_NSRUNS; j++)
            if (SH_BADRNS[i][j] || CF_BADRNS[i][j])
                IM_TTLBAD++;

    IM_VALRNS = params->_NTHRDS * params->_NSRUNS - IM_TTLBAD;
    CF_SRTGAP = new double*[IM_VALRNS];

    for (int i = 0, k = 0; i < params->_NTHRDS; i++) {
        for (int j = 0; j < params->_NSRUNS; j++) {
            if (SH_BADRNS[i][j] || CF_BADRNS[i][j])
                continue;

            CF_SRTGAP[k] = new double[3];

            CF_SRTGAP[k][0] = CF_CYLNGP[i][j];
            CF_SRTGAP[k][1] = i;
            CF_SRTGAP[k][2] = j;
            k++;
        }
    }

    bool CF_SRTFLG = true;
    for (int i = 1; (i <= IM_VALRNS) && CF_SRTFLG; i++) {
        CF_SRTFLG = false;
        for (int j = 0; j < (IM_VALRNS - 1); j++) {
            // sort rows of CF_SRTGAP in descending order of its first column
            if (CF_SRTGAP[j + 1][0] > CF_SRTGAP[j][0]) {
                double CF_GAPTMP = CF_SRTGAP[j][0];
                double CF_TIDTMP = CF_SRTGAP[j][1];
                double CF_RIDTMP = CF_SRTGAP[j][2];
                CF_SRTGAP[j][0] = CF_SRTGAP[j + 1][0];
                CF_SRTGAP[j][1] = CF_SRTGAP[j + 1][1];
                CF_SRTGAP[j][2] = CF_SRTGAP[j + 1][2];
                CF_SRTGAP[j + 1][0] = CF_GAPTMP;
                CF_SRTGAP[j + 1][1] = CF_TIDTMP;
                CF_SRTGAP[j + 1][2] = CF_RIDTMP;
                CF_SRTFLG = true;
            }
        }
    }

    for (int i = 0; i < params->_NTHRDS; i++) {
        for (int j = 0; j < params->_NSRUNS; j++) {
            if (SH_BADRNS[i][j] || CF_BADRNS[i][j])
                continue;

            for (int k = 0; k < params->_NWEEKS / params->_WKSPMN; k++) {
                IM_AVLGAP[k] += (SH_LOGGAP[i][j][k] - CF_LOGGAP[i][j][k]) / IM_VALRNS;
                IM_AVSHFL[k] += (SH_SHOPFL[i][j][k] - CF_SHOPFL[i][j][k]) / IM_VALRNS;
                IM_AVBKFL[k] += (SH_BANKFL[i][j][k] - CF_BANKFL[i][j][k]) / IM_VALRNS;
                IM_AVBKTR[k] += (SH_BANKTR[i][j][k] - CF_BANKTR[i][j][k]) / IM_VALRNS;
                IM_AVENTR[k] += (SH_NENTRY[i][j][k] - CF_NENTRY[i][j][k]) / IM_VALRNS;
            }
        }
    }

    for (int i = 0; i < params->_NTHRDS; i++) {
        for (int j = 0; j < params->_NSRUNS; j++) {
            if (SH_BADRNS[i][j] || CF_BADRNS[i][j])
                continue;

            for (int k = 0; k < params->_NWEEKS / params->_WKSPMN; k++) {
                IM_SDLGAP[k] += pow(SH_LOGGAP[i][j][k] - CF_LOGGAP[i][j][k] - IM_AVLGAP[k], 2) / IM_VALRNS;
                IM_SDSHFL[k] += pow(SH_SHOPFL[i][j][k] - CF_SHOPFL[i][j][k] - IM_AVSHFL[k], 2) / IM_VALRNS;
                IM_SDBKFL[k] += pow(SH_BANKFL[i][j][k] - CF_BANKFL[i][j][k] - IM_AVBKFL[k], 2) / IM_VALRNS;
                IM_SDBKTR[k] += pow(SH_BANKTR[i][j][k] - CF_BANKTR[i][j][k] - IM_AVBKTR[k], 2) / IM_VALRNS;
            }
        }
    }

    for (int i = 0; i < params->_NWEEKS / params->_WKSPMN; i++) {
        IM_SDLGAP[i] = sqrt(IM_SDLGAP[i]);
        IM_SDSHFL[i] = sqrt(IM_SDSHFL[i]);
        IM_SDBKFL[i] = sqrt(IM_SDBKFL[i]);
        IM_SDBKTR[i] = sqrt(IM_SDBKTR[i]);

        IM_SELGAP[i] = IM_SDLGAP[i] / sqrt((double)IM_VALRNS);
        IM_SESHFL[i] = IM_SDSHFL[i] / sqrt((double)IM_VALRNS);
        IM_SEBKFL[i] = IM_SDBKFL[i] / sqrt((double)IM_VALRNS);
        IM_SEBKTR[i] = IM_SDBKTR[i] / sqrt((double)IM_VALRNS);
    }

    for (int i = (int)(0.25 * IM_VALRNS); i < (int)(0.75 * IM_VALRNS); i++) {
        int CF_THDIDX = (int)CF_SRTGAP[i][1];
        int CF_RUNIDX = (int)CF_SRTGAP[i][2];
        for (int j = 0; j < params->_NWEEKS / params->_WKSPMN; j++) {
            IM_IQLGAP[j] += (SH_LOGGAP[CF_THDIDX][CF_RUNIDX][j] - CF_LOGGAP[CF_THDIDX][CF_RUNIDX][j]) / (int)(0.50 * IM_VALRNS);
            IM_IQSHFL[j] += (SH_SHOPFL[CF_THDIDX][CF_RUNIDX][j] - CF_SHOPFL[CF_THDIDX][CF_RUNIDX][j]) / (int)(0.50 * IM_VALRNS);
            IM_IQBKFL[j] += (SH_BANKFL[CF_THDIDX][CF_RUNIDX][j] - CF_BANKFL[CF_THDIDX][CF_RUNIDX][j]) / (int)(0.50 * IM_VALRNS);
            IM_IQBKTR[j] += (SH_BANKTR[CF_THDIDX][CF_RUNIDX][j] - CF_BANKTR[CF_THDIDX][CF_RUNIDX][j]) / (int)(0.50 * IM_VALRNS);
            IM_IQENTR[j] += (SH_NENTRY[CF_THDIDX][CF_RUNIDX][j] - CF_NENTRY[CF_THDIDX][CF_RUNIDX][j]) / (int)(0.50 * IM_VALRNS);
        }
    }

    for (int i = 0; i < params->_NTHRDS; i++) {
        for (int j = 0; j < params->_NSRUNS; j++) {
            if (SH_BADRNS[i][j] || CF_BADRNS[i][j])
                continue;
            for (int k = 0; k < params->_NWEEKS / params->_WKSPMN; k++) {
                IM_SMPGAP[k].push_back(SH_LOGGAP[i][j][k] - CF_LOGGAP[i][j][k]);
                IM_SMPENT[k].push_back(SH_NENTRY[i][j][k] - CF_NENTRY[i][j][k]);
            }
        }
    }

    for (int i = 0; i < IM_VALRNS; i++) {
        delete[] CF_SRTGAP[i]; CF_SRTGAP[i] = NULL;
    }
    delete[] CF_SRTGAP; CF_SRTGAP = NULL;
}

/***********************************************************************************************
 * FUNCTION FOR CLEANING GLOBAL ARRAYS IN IMPULSE MODE
 ***********************************************************************************************/

void del_imarrs(Parameters* params) {
    for (int i = 0; i < params->_NTHRDS; i++) {
        for (int j = 0; j < params->_NSRUNS; j++) {
            delete[] SH_LOGGAP[i][j]; SH_LOGGAP[i][j] = NULL;
            delete[] CF_LOGGAP[i][j]; CF_LOGGAP[i][j] = NULL;
            delete[] SH_SHOPFL[i][j]; SH_SHOPFL[i][j] = NULL;
            delete[] CF_SHOPFL[i][j]; CF_SHOPFL[i][j] = NULL;
            delete[] SH_BANKFL[i][j]; SH_BANKFL[i][j] = NULL;
            delete[] CF_BANKFL[i][j]; CF_BANKFL[i][j] = NULL;
            delete[] SH_BANKTR[i][j]; SH_BANKTR[i][j] = NULL;
            delete[] CF_BANKTR[i][j]; CF_BANKTR[i][j] = NULL;
            delete[] SH_NENTRY[i][j]; SH_NENTRY[i][j] = NULL;
            delete[] CF_NENTRY[i][j]; CF_NENTRY[i][j] = NULL;
        }
        delete[] SH_LOGGAP[i]; SH_LOGGAP[i] = NULL;
        delete[] CF_LOGGAP[i]; CF_LOGGAP[i] = NULL;
        delete[] SH_SHOPFL[i]; SH_SHOPFL[i] = NULL;
        delete[] CF_SHOPFL[i]; CF_SHOPFL[i] = NULL;
        delete[] SH_BANKFL[i]; SH_BANKFL[i] = NULL;
        delete[] CF_BANKFL[i]; CF_BANKFL[i] = NULL;
        delete[] SH_BANKTR[i]; SH_BANKTR[i] = NULL;
        delete[] CF_BANKTR[i]; CF_BANKTR[i] = NULL;
        delete[] SH_NENTRY[i]; SH_NENTRY[i] = NULL;
        delete[] CF_NENTRY[i]; CF_NENTRY[i] = NULL;

        delete[] CF_CYLNGP[i]; CF_CYLNGP[i] = NULL;

        delete[] SH_BADRNS[i]; SH_BADRNS[i] = NULL;
        delete[] CF_BADRNS[i]; CF_BADRNS[i] = NULL;
    }
    delete[] SH_LOGGAP; SH_LOGGAP = NULL;
    delete[] CF_LOGGAP; CF_LOGGAP = NULL;
    delete[] SH_SHOPFL; SH_SHOPFL = NULL;
    delete[] CF_SHOPFL; CF_SHOPFL = NULL;
    delete[] SH_BANKFL; SH_BANKFL = NULL;
    delete[] CF_BANKFL; CF_BANKFL = NULL;
    delete[] SH_BANKTR; SH_BANKTR = NULL;
    delete[] CF_BANKTR; CF_BANKTR = NULL;
    delete[] SH_NENTRY; SH_NENTRY = NULL;
    delete[] CF_NENTRY; CF_NENTRY = NULL;

    delete[] CF_CYLNGP; CF_CYLNGP = NULL;

    delete[] SH_BADRNS; SH_BADRNS = NULL;
    delete[] CF_BADRNS; CF_BADRNS = NULL;

    delete[] IM_AVLGAP; IM_AVLGAP = NULL;
    delete[] IM_AVSHFL; IM_AVSHFL = NULL;
    delete[] IM_AVBKFL; IM_AVBKFL = NULL;
    delete[] IM_AVBKTR; IM_AVBKTR = NULL;
    delete[] IM_AVENTR; IM_AVENTR = NULL;

    delete[] IM_SDLGAP; IM_SDLGAP = NULL;
    delete[] IM_SDSHFL; IM_SDSHFL = NULL;
    delete[] IM_SDBKFL; IM_SDBKFL = NULL;
    delete[] IM_SDBKTR; IM_SDBKTR = NULL;

    delete[] IM_SELGAP; IM_SELGAP = NULL;
    delete[] IM_SESHFL; IM_SESHFL = NULL;
    delete[] IM_SEBKFL; IM_SEBKFL = NULL;
    delete[] IM_SEBKTR; IM_SEBKTR = NULL;

    delete[] IM_IQLGAP; IM_IQLGAP = NULL;
    delete[] IM_IQSHFL; IM_IQSHFL = NULL;
    delete[] IM_IQBKFL; IM_IQBKFL = NULL;
    delete[] IM_IQBKTR; IM_IQBKTR = NULL;
    delete[] IM_IQENTR; IM_IQENTR = NULL;

    delete[] IM_SMPGAP; IM_SMPGAP = NULL;
    delete[] IM_SMPENT; IM_SMPENT = NULL;
}

double median( vector<double> &v) {
    size_t n = v.size() / 2;
    nth_element(v.begin(), v.begin() + n, v.end());
    return v[n];
}
