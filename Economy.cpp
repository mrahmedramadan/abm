/***********************************************************************************************
 * Economy.cpp
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised August 2 2016
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

#undef USE_TIMERS        // flag for using timers in calibration mode

#include "Agent.h"
#include "Economy.h"
#include "Parameters.h"
#include "RandUtils.h"
#include "Shop.h"
#include "Statistics.h"
#include "Bank.h"
#include "timers.h"
#include "require.h"
#include <fstream>
#include <windows.h>
#include "Firesale.h"
#include "AgentMode.h"

Economy::Economy(Parameters* PARAMS, char* RSFILE) : _numPrs(NULL) {
    _NSRUNS = PARAMS->_NSRUNS;
    _NWEEKS = PARAMS->_NWEEKS;
    _ADJLAG = PARAMS->_ADJLAG;
    _LRNLAG = PARAMS->_LRNLAG;
    _IMPLAG = PARAMS->_IMPLAG;
    _NGOODS = PARAMS->_NGOODS;
    _WKSPMN = PARAMS->_WKSPMN;
    _WKSPYR = PARAMS->_WKSPYR;
    _RNSEED = PARAMS->_RNSEED;
    _RNDMRK = PARAMS->_RNDMRK;
    _MAXSLS = PARAMS->_MAXSLS;
    _CNTPRD = PARAMS->_CNTPRD;
    _MARKUP = PARAMS->_MARKUP;
    _STDMKP = PARAMS->_STDMKP;
    _FXDCST = PARAMS->_FXDCST;
    _INFREQ = PARAMS->_INFREQ;
    _DMDELS = PARAMS->_DMDELS;
    _INCSPD = PARAMS->_INCSPD;
    _TMPRFY = PARAMS->_TMPRFY;
    _BRKFRC = PARAMS->_BRKFRC;
    _BNDINC = PARAMS->_BNDINC;
    _FSCSPD = PARAMS->_FSCSPD;
    _INFCOR = PARAMS->_INFCOR;
    _GAPCOR = PARAMS->_GAPCOR;
    _INFTYR = PARAMS->_INFTYR;
    _GAPTYR = PARAMS->_GAPTYR;
    _GAPTRG = PARAMS->_GAPTRG;
    _RRTTRG = PARAMS->_RRTTRG;
    _PITRGY = PARAMS->_PITRGY;
    _TRNOVR = PARAMS->_TRNOVR;
    _SHOCKS = PARAMS->_SHOCKS;
    _ZEROLB = PARAMS->_ZEROLB;
    _THREAD = PARAMS->_THREAD;
    _ADJRRT = PARAMS->_ADJRRT;
    _GAPLMB = PARAMS->_GAPLMB;
    _INFLMB = PARAMS->_INFLMB;
    _SRCHPP = PARAMS->_SRCHPP;
    _INIPRD = PARAMS->_INIPRD;
    _CARATE = PARAMS->_CARATE;
    _DISCPR = PARAMS->_DISCPR;
    _NBANKS = PARAMS->_NBANKS;
    _NCROOK = PARAMS->_NCROOK;
    _NOBANK = PARAMS->_NOBANK;
    _FSDISC = PARAMS->_FSDISC;
    _LVALRT = PARAMS->_LVALRT;
    _MXBAIL = PARAMS->_MXBAIL;
    _CASLOP = PARAMS->_CASLOP;
    _LVSLOP = PARAMS->_LVSLOP;
    _FDICPR = PARAMS->_FDICPR;
    _BRKPRC = PARAMS->_BRKPRC;
    _FREQTR = PARAMS->_FREQTR;
    _SETUPC = PARAMS->_SETUPC;

    _NAGNTS = _NGOODS * (_NGOODS - 2);
    _NSHOPS = _NGOODS * 3;
    _MNSPYR = _WKSPYR / _WKSPMN;
    _NYEARS = _NWEEKS / _WKSPYR;
    _NMNTHS = _NYEARS * _MNSPYR;
    _TMPRFW = pow(_TMPRFY, 1.0 / _WKSPYR);
    _PITRGW = pow(_PITRGY, 1.0 / _WKSPYR);
    _SPNDPP = (_TMPRFW - 1.0) / _TMPRFW / _FREQTR;
    _CAPGDP = _NAGNTS - _NGOODS * _FXDCST;
    _INPCAP = (_NAGNTS - _NGOODS) / _CAPGDP;
    _EQMTAX = 1 - _PITRGW * (1 - (_TMPRFW - 1) * _WKSPYR * _BNDINC) * _MARKUP
		    / (_MARKUP - (_PITRGW - 1) * _INPCAP);
    _INFRYR = _INFCOR;
    _GDPRYR = _GAPCOR;
    _GDPPOT = log(_CAPGDP) - _GAPTRG;
    _GDPINT = (1 - _GAPCOR) * _GDPPOT;

    _RSFILE = RSFILE;
	_rystrm = NULL;

    _frList = new Firesale*[_NGOODS];

    for (int i = 0; i < _NGOODS; i++) 
	    _frList[i] = new Firesale(i);

    _prList = new Agent**[_NGOODS];
    _cnList = new Agent**[_NGOODS];

    for (int i = 0; i < _NGOODS; i++) {
	    _prList[i] = new Agent*[_NGOODS - 2];
	    _cnList[i] = new Agent*[_NGOODS - 2];
    }

    _numPrs = new int[_NGOODS];
    _numCns = new int[_NGOODS];
    _nSllrs = new int[_NGOODS];

    for (int i = 0; i < _NGOODS; i++) {
        _numPrs[i] = 0;
        _numCns[i] = 0;
        _nSllrs[i] = 0;
    }

    _agList = new Agent*[_NAGNTS];
    _rnList = new Agent*[_NAGNTS];

    _crList = new Agent**[_NBANKS];

    for (int i = 0; i < _NBANKS; i++)
	    _crList[i] = new Agent*[_NCROOK * _NGOODS / _NBANKS];

    _worker = new WorkerMan();
    _sOwner = new ShopOwner();
    _bOwner = new BankOwner();

    for (int a = 0, c = 0, i = 0; i < _NGOODS; i++) { // The first N-2 make 1, next N-2 make 2, etc
        for (int j = 0; j < _NGOODS; j++) {  // a counts agents and c counts crooks
            if (i != j && i != (j + 1) % _NGOODS) {//No one consumes supply good
                if( (j >= i-(j>=i?0:_NGOODS) + 2) && 
                  (j <= i-(j>=i?0:_NGOODS) + _NCROOK + 1) ) {
                    _agList[a] = new Crook(a+1, i+1, j+1, this);
                    _crList[i*_NBANKS/_NGOODS][c%(_NCROOK * _NGOODS / _NBANKS)] = _agList[a]; c++;
                }
                else
                    _agList[a] = new Agent(a+1, i+1, j+1, this);
                _prList[i][_numPrs[i]] = _agList[a];        // Comrades ranked by their d1 goods
                _cnList[j][_numCns[j]] = _agList[a];        // Soulmates ranked by their s goods
                _numPrs[i]++;
                _numCns[j]++;
                a++;
            }
        }
    }

    _shList = new Shop*[_NSHOPS];

    for (int i = 0; i < _NSHOPS; i++) {
        _shList[i] = new Shop(i + 1, PARAMS, this);
    }

    _bkList = new Bank*[_NBANKS];

    for (int i = 0; i < _NBANKS; i++) { 
        _bkList[i] = new Bank(i, PARAMS, this); // must be created after the agents
    }

    _annVal = 0.0;
    _bndStk = 0.0;
    _cpIndx = 0.0;
    _initPm = 0.0;
    _monStk = 0.0;
    _rlDebt = 0.0;
    _rlIncm = 0.0;
    _nmIncm = 0.0;
    _cmRInc = 0.0;
    _cmNInc = 0.0;
    _rlWage = 0.0;
    _txRate = 0.0;
    _wgRate = 0.0;
    _fsaleP = 0.0;
    _hcutPr = 0.0;
    _wkIntr = 0.0;
    _crProb = 0.0;

    _lagdPc = new double[_MNSPYR];
    _lagdPi = new double[_MNSPYR];
    _lagdRI = new double[_MNSPYR];
    _lagdNI = new double[_MNSPYR];
    _lag3RI = new double[_MNSPYR];
    _lgCCRR = new double[_MNSPYR];
	_lgCCNR = new double[_MNSPYR];

    for (int i = 0; i < _MNSPYR; i++) {
        _lagdPc[i] = 0.0;
        _lagdPi[i] = 0.0;
        _lagdRI[i] = 0.0;
        _lagdNI[i] = 0.0;
        _lag3RI[i] = 0.0;
        _lgCCRR[i] = 0.0;
		_lgCCNR[i] = 0.0;
	}

    _ilggap = 0;
    _ilginf = 0;
    _gdpint = 0;
    _gdppot = 0;
    _caRate = 0;
    _lValRt = 0;
    _rrttrg = 0;
    _csttyr = 0;
    _gdpssq = 0;
    _gdpsum = 0;
    _gdpryr = 0;
    _infssq = 0;
    _infryr = 0;
    _wkCntr = 0;
    _mnCntr = 0;
    _yrCntr = 0;
    _cnCntr = 0;
    _zbCntr = 0;
    _rnCntr = 0;
    _nActSh = 0;
    _nAlive = 0;
    _nFails = 0;
    _nPrDlt = 0;
    _nPrObs = 0;
    _nHplss = 0;
    _anvCnt = 0;
    _enLfSp = 0;
    _nEntEx = 0;
    _nBkFlr = 0;
    _nBkTrb = 0;

    _nUn2Em = 0; _unDurn = 0;
    _nLf2Em = 0; _lfDurn = 0;
    _nEx2Em = 0; _exDurn = 0;
    _nQt2Em = 0; _qtDurn = 0;
    _nBk2Em = 0; _bkDurn = 0;

    _entVac = 0.0; _entShp = 0.0;
    _extShp = 0.0; _nomGdp = 0.0;
    _jobLss = 0.0; _jbLsLf = 0.0;
    _jbLsQt = 0.0; _jbLsEx = 0.0; 
    _bkrpFr = 0.0; _fConFr = 0.0;
    _fscInv = 0.0; _nEnter = 0.0;
    _entrFr = 0.0; _nJbMch = 0.0;
    _nStMch = 0.0; _nJb2Jb = 0.0;
    _nSt2St = 0.0; _wtFail = 0.0;
    _surMkp = 0.0; _agPrft = 0.0;
	_iniInv = 0.0; _aggCon = 0.0;
	_entrLn = 0.0; _avgLVR = 0.0;
  
    _crashd = false;
    _implse = false;
    _shopFl = false;
    _bankFl = false;
    _bankTr = false;
    _brkups = false;

    _OPMODE = new char[20];
    strcpy_s(_OPMODE, 20, PARAMS->_OPMODE);

    _nShops = _NSHOPS;
    _ecStat = new Statistics(this);

    _IMPULS = NULL;
    if(PARAMS->_IMPULS) {
        _IMPULS = new char[7];
        strcpy_s(_IMPULS, 7, PARAMS->_IMPULS);
    }
}

Economy::~Economy() {
    for (int i = 0; i < _NAGNTS; i++) {
        delete _agList[i];
        _agList[i] = NULL;
        _rnList[i] = NULL;
    }

    delete[] _agList; _agList = NULL;
    delete[] _rnList; _rnList = NULL;

    for (int i = 0; i < _NGOODS; i++) {
        for (int j = 0; j < _NGOODS - 2; j++) {
            _prList[i][j] = NULL;
            _cnList[i][j] = NULL;
        }
        delete[] _prList[i]; _prList[i] = NULL;
        delete[] _cnList[i]; _cnList[i] = NULL;
    }

    delete[] _prList; _prList = NULL;
    delete[] _cnList; _cnList = NULL;
    delete[] _numPrs; _numPrs = NULL;
    delete[] _numCns; _numCns = NULL;
    delete[] _nSllrs; _nSllrs = NULL;

    for (int i = 0; i < _nShops; i++) {
        delete _shList[i];
        _shList[i] = NULL;
    }
    delete[] _shList; _shList = NULL;
	
    for (int i = 0; i < _NBANKS; i++) {
        delete _bkList[i];
        _bkList[i] = NULL;
    }
    delete[] _bkList; _bkList = NULL;
	
    for (int i = 0; i < _NGOODS; i++) {
        delete _frList[i];
        _frList[i] = NULL;
    }
    delete[] _frList; 

    delete   _bOwner; _bOwner = NULL;
    delete   _sOwner; _sOwner = NULL;
    delete   _worker; _worker = NULL;

    delete[] _lagdPc; _lagdPc = NULL;
    delete[] _lagdPi; _lagdPi = NULL;
    delete[] _lgCCRR; _lgCCRR = NULL;
	delete[] _lgCCNR; _lgCCNR = NULL;
	delete[] _lagdRI; _lagdRI = NULL;
    delete[] _lagdNI; _lagdNI = NULL;
    delete[] _lag3RI; _lag3RI = NULL;

    delete   _ecStat; _ecStat = NULL;

    delete[] _OPMODE; _OPMODE = NULL;

    if (_IMPULS) {
        delete[] _IMPULS; _IMPULS = NULL;
    }

	_RSFILE = NULL;
	_rystrm = NULL;
}

ostream& operator <<(ostream &stream, Economy* object) {
    stream << "-----------------------------------------------------------------\n";
    stream << "AGGREGATE VARIABLES\n";
    stream << "-----------------------------------------------------------------\n";
    stream << "\n";
    stream << "annVal: " << object->_annVal << "\n";
    stream << "bndStk: " << object->_bndStk << "\n";
    stream << "cpIndx: " << object->_cpIndx << "\n";
    stream << "initPm: " << object->_initPm << "\n";
    stream << "monStk: " << object->_monStk << "\n";
    stream << "nmIncm: " << object->_nmIncm << "\n";
    stream << "rlDebt: " << object->_rlDebt << "\n";
    stream << "rlIncm: " << object->_rlIncm << "\n";
    stream << "cmRInc: " << object->_cmRInc << "\n";
    stream << "cmNInc: " << object->_cmNInc << "\n";	
    stream << "rlWage: " << object->_rlWage << "\n";
    stream << "txRate: " << object->_txRate << "\n";
    stream << "wgRate: " << object->_wgRate << "\n";
    stream << "wkIntr: " << object->_wkIntr << "\n";
    stream << "fsaleP: " << object->_fsaleP << "\n";   
    stream << "hcutPr: " << object->_hcutPr << "\n";   
    stream << "gdpint: " << object->_gdpint << "\n";   
    stream << "gdppot: " << object->_gdppot << "\n";   
    stream << "caRate: " << object->_caRate << "\n";   
    stream << "lValRt: " << object->_lValRt << "\n";   
    stream << "rrttrg: " << object->_rrttrg << "\n";		
    stream << "csttyr: " << object->_csttyr << "\n";		
    stream << "gdpssq: " << object->_gdpssq << "\n";		
    stream << "gdpsum: " << object->_gdpsum << "\n";		
    stream << "gdpryr: " << object->_gdpryr << "\n";		
    stream << "infssq: " << object->_infssq << "\n";		
    stream << "infryr: " << object->_infryr << "\n";		
    stream << "ilggap: " << object->_ilggap << "\n";   
    stream << "ilginf: " << object->_ilginf << "\n";   
    stream << "nActSh: " << object->_nActSh << "\n";		
    stream << "nAlive: " << object->_nAlive << "\n";		
    stream << "nFails: " << object->_nFails << "\n";		
    stream << "nPrDlt: " << object->_nPrDlt << "\n";		
    stream << "nPrObs: " << object->_nPrObs << "\n";		
    stream << "enLfSp: " << object->_enLfSp << "\n";		
    stream << "nEntEx: " << object->_nEntEx << "\n";		
    stream << "nBkFlr: " << object->_nBkFlr << "\n";		
    stream << "nBkTrb: " << object->_nBkTrb << "\n";		
    stream << "nUn2Em: " << object->_nUn2Em << "\n";		
    stream << "nLf2Em: " << object->_nLf2Em << "\n";		
    stream << "nEx2Em: " << object->_nEx2Em << "\n";		
    stream << "nQt2Em: " << object->_nQt2Em << "\n";		
    stream << "nBk2Em: " << object->_nBk2Em << "\n";		
    stream << "unDurn: " << object->_unDurn << "\n";		
    stream << "lfDurn: " << object->_lfDurn << "\n";		
    stream << "exDurn: " << object->_exDurn << "\n";		
    stream << "qtDurn: " << object->_qtDurn << "\n";		
    stream << "bkDurn: " << object->_bkDurn << "\n";		
    stream << "nHplss: " << object->_nHplss << "\n";		
    stream << "anvCnt: " << object->_anvCnt << "\n";		
    stream << "entVac: " << object->_entVac << "\n";		
    stream << "nEnter: " << object->_nEnter << "\n";
    stream << "entrFr: " << object->_entrFr << "\n";
    stream << "nJbMch: " << object->_nJbMch << "\n";		
    stream << "nJb2Jb: " << object->_nJb2Jb << "\n";		
    stream << "nStMch: " << object->_nStMch << "\n";		
    stream << "nSt2St: " << object->_nSt2St << "\n";		
    stream << "entShp: " << object->_entShp << "\n";		
    stream << "extShp: " << object->_extShp << "\n";		
    stream << "jobLss: " << object->_jobLss << "\n";		
    stream << "jbLsLf: " << object->_jbLsLf << "\n";		
    stream << "jbLsQt: " << object->_jbLsQt << "\n";		
    stream << "jbLsEx: " << object->_jbLsEx << "\n";		
    stream << "bkrpFr: " << object->_bkrpFr << "\n";		
    stream << "fConFr: " << object->_fConFr << "\n";		
	stream << "entrLn: " << object->_entrLn << "\n";
	stream << "fscInv: " << object->_fscInv << "\n";
	stream << "iniInv: " << object->_iniInv << "\n";
	stream << "aggCon: " << object->_aggCon << "\n";
	stream << "avgLVR: " << object->_avgLVR << "\n";
	stream << "wtFail: " << object->_wtFail << "\n";
    stream << "surMkp: " << object->_surMkp << "\n";		
    stream << "agPrft: " << object->_agPrft << "\n";		
    stream << "nomGdp: " << object->_nomGdp << "\n";

    stream << "ADJLAG: " << object->_ADJLAG << "\n";  
    stream << "ADJRRT: " << object->_ADJRRT << "\n";  
    stream << "bankFl: " << object->_bankFl << "\n";  
    stream << "bankTr: " << object->_bankTr << "\n";  
    stream << "BNDINC: " << object->_BNDINC << "\n";  
    stream << "BRKFRC: " << object->_BRKFRC << "\n";  
    stream << "BRKPRC: " << object->_BRKPRC << "\n";  
    stream << "brkups: " << object->_brkups << "\n";  
    stream << "CAPGDP: " << object->_CAPGDP << "\n";  
    stream << "CARATE: " << object->_CARATE << "\n";  
    stream << "CASLOP: " << object->_CASLOP << "\n";  
    stream << "cnCntr: " << object->_cnCntr << "\n";  
    stream << "CNTPRD: " << object->_CNTPRD << "\n";  
    stream << "crashd: " << object->_crashd << "\n";  
    stream << "crProb: " << object->_crProb << "\n";  
    stream << "DISCPR: " << object->_DISCPR << "\n";  
    stream << "DMDELS: " << object->_DMDELS << "\n";  
    stream << "EQMTAX: " << object->_EQMTAX << "\n";  
    stream << "FDICPR: " << object->_FDICPR << "\n";  
    stream << "FREQTR: " << object->_FREQTR << "\n";  
    stream << "FSCSPD: " << object->_FSCSPD << "\n";  
    stream << "FSDISC: " << object->_FSDISC << "\n";  
    stream << "FXDCST: " << object->_FXDCST << "\n";  
    stream << "GAPCOR: " << object->_GAPCOR << "\n";  
    stream << "GAPLMB: " << object->_GAPLMB << "\n";  
    stream << "GAPTRG: " << object->_GAPTRG << "\n";  
    stream << "GAPTYR: " << object->_GAPTYR << "\n";  
    stream << "GDPINT: " << object->_GDPINT << "\n";  
    stream << "GDPPOT: " << object->_GDPPOT << "\n";  
    stream << "GDPRYR: " << object->_GDPRYR << "\n";  
    stream << "IMPLAG: " << object->_IMPLAG << "\n";  
    stream << "implse: " << object->_implse << "\n";  
    stream << "IMPULS: " << object->_IMPULS << "\n";  
    stream << "INCSPD: " << object->_INCSPD << "\n";  
    stream << "INFCOR: " << object->_INFCOR << "\n";  
    stream << "INFLMB: " << object->_INFLMB << "\n";  
    stream << "INFREQ: " << object->_INFREQ << "\n";  
    stream << "INFRYR: " << object->_INFRYR << "\n";  
    stream << "INFTYR: " << object->_INFTYR << "\n";  
    stream << "INIPRD: " << object->_INIPRD << "\n";  
    stream << "INPCAP: " << object->_INPCAP << "\n";

    for (int i = 0; i < 12; ++i)
        stream << "lag3RI[" << i << "]: " << object->_lag3RI[i] << "\n";  
    for (int i = 0; i < 12; ++i)
        stream << "lagdNI[" << i << "]: " << object->_lagdNI[i] << "\n";  
    for (int i = 0; i < 12; ++i)
        stream << "lagdPc[" << i << "]: " << object->_lagdPc[i] << "\n";  
    for (int i = 0; i < 12; ++i)
        stream << "lagdPi[" << i << "]: " << object->_lagdPi[i] << "\n";  
    for (int i = 0; i < 12; ++i)
        stream << "lagdRI[" << i << "]: " << object->_lagdRI[i] << "\n";  
    for (int i = 0; i < 12; ++i)
        stream << "lgCCRR[" << i << "]: " << object->_lgCCRR[i] << "\n";
	for (int i = 0; i < 12; ++i)
		stream << "lgCCNR[" << i << "]: " << object->_lgCCNR[i] << "\n";

    stream << "LRNLAG: " << object->_LRNLAG << "\n";  
    stream << "LVALRT: " << object->_LVALRT << "\n";  
    stream << "LVSLOP: " << object->_LVSLOP << "\n";  
    stream << "MARKUP: " << object->_MARKUP << "\n";  
    stream << "MAXSLS: " << object->_MAXSLS << "\n";  
    stream << "mnCntr: " << object->_mnCntr << "\n";  
    stream << "MNSPYR: " << object->_MNSPYR << "\n";  
    stream << "MXBAIL: " << object->_MXBAIL << "\n";  
    stream << "NAGNTS: " << object->_NAGNTS << "\n";  
    stream << "NBANKS: " << object->_NBANKS << "\n";  
    stream << "NCROOK: " << object->_NCROOK << "\n";  
    stream << "NGOODS: " << object->_NGOODS << "\n";  
    stream << "NMNTHS: " << object->_NMNTHS << "\n";  
    stream << "NOBANK: " << object->_NOBANK << "\n";  
    stream << "nShops: " << object->_nShops << "\n";  
    stream << "NSHOPS: " << object->_NSHOPS << "\n";  
    stream << "NSRUNS: " << object->_NSRUNS << "\n";  
    for (int i=0; i<object->_NGOODS; ++i) {
        stream << "nSllrs[" << i <<"]: " << object->_nSllrs[i] << "\n";  
        stream << "numCns[" << i <<"]: " << object->_numCns[i] << "\n";  
        stream << "numPrs[" << i <<"]: " << object->_numPrs[i] << "\n";
    }
    stream << "NWEEKS: " << object->_NWEEKS << "\n";  
    stream << "NYEARS: " << object->_NYEARS << "\n";  
    stream << "OPMODE: " << object->_OPMODE << "\n";  
    stream << "PITRGW: " << object->_PITRGW << "\n";  
    stream << "PITRGY: " << object->_PITRGY << "\n";  
    stream << "rnCntr: " << object->_rnCntr << "\n";  
    stream << "RNDMRK: " << object->_RNDMRK << "\n";  
    stream << "RNSEED: " << object->_RNSEED << "\n";  
    stream << "RRTTRG: " << object->_RRTTRG << "\n";  
    stream << "RSFILE: " << object->_RSFILE << "\n";  
    stream << "SETUPC: " << object->_SETUPC << "\n";  
    stream << "SHOCKS: " << object->_SHOCKS << "\n";  
    stream << "shopFl: " << object->_shopFl << "\n";  
    stream << "SPNDPP: " << object->_SPNDPP << "\n";  
    stream << "SRCHPP: " << object->_SRCHPP << "\n";  
    stream << "STDMKP: " << object->_STDMKP << "\n";  
    stream << "THREAD: " << object->_THREAD << "\n";  
    stream << "TMPRFW: " << object->_TMPRFW << "\n";  
    stream << "TMPRFY: " << object->_TMPRFY << "\n";  
    stream << "TRNOVR: " << object->_TRNOVR << "\n";  
    stream << "wkCntr: " << object->_wkCntr << "\n";  
    stream << "WKSPMN: " << object->_WKSPMN << "\n";  
    stream << "WKSPYR: " << object->_WKSPYR << "\n";  
    stream << "yrCntr: " << object->_yrCntr << "\n";  
    stream << "zbCntr: " << object->_zbCntr << "\n";  
    stream << "ZEROLB: " << object->_ZEROLB << "\n";  
    stream << "\n";
    stream << "-----------------------------------------------------------------\n";
    stream << "AGENTS\n";
    stream << "-----------------------------------------------------------------\n";
    stream << "\n";
    for (int i = 0; i < object->_NAGNTS; i++)
	    stream << object->_agList[i];
    stream << "-----------------------------------------------------------------\n";
    stream << "SHOPS\n";
    stream << "-----------------------------------------------------------------\n";
    stream << "\n";
    for (int i = 0; i < object->_nShops; i++)
	    stream << object->_shList[i];
    stream << "-----------------------------------------------------------------\n";
    stream << "BANKS\n";
    stream << "-----------------------------------------------------------------\n";
    stream << "\n";
    for (int i = 0; i < object->_NBANKS; i++)
	    stream << object->_bkList[i];
    stream << "-----------------------------------------------------------------\n";
    stream << "FIRESALES\n";
    stream << "-----------------------------------------------------------------\n";
    stream << "\n";
    for (int i = 0; i < object->_NGOODS; i++)
	    stream << object->_frList[i];
    stream << "-----------------------------------------------------------------\n";
    stream << "RANDOMIZED AGENT SEQUENCE\n";
    stream << "-----------------------------------------------------------------\n";
    stream << "\n";
    stream << "\n";

    return stream;
}


void Economy::ini_forRun() {
    RandUtils::set_RnSeed(_RNSEED + _rnCntr);

    if (_nShops > _NSHOPS) {
        Shop** nwList = new Shop*[_NSHOPS];
        for (int i = 0; i < _NSHOPS; i++) {
            nwList[i] = _shList[i];
            _shList[i] = NULL;
        }
        for (int i = _NSHOPS; i < _nShops; i++) {
            delete _shList[i]; _shList[i] = NULL;
        }
        delete[] _shList;
        _shList = nwList;
        _nShops = _NSHOPS;
    }

    _wkCntr = 0;
    _mnCntr = 0;
    _yrCntr = 0;
    _cnCntr = 0;
    _zbCntr = 0;
    _nFails = 0;
    _nPrDlt = 0;
    _nPrObs = 0;
    _nHplss = 0;
    _nActSh = _NGOODS;
    _nAlive = _nActSh;
    _enLfSp = 0;
    _nEntEx = 0;
    _nBkFlr = 0;
    _nBkTrb = 0;

    _nUn2Em = 0; _unDurn = 0;
    _nLf2Em = 0; _lfDurn = 0;
    _nEx2Em = 0; _exDurn = 0;
    _nQt2Em = 0; _qtDurn = 0;
    _nBk2Em = 0; _bkDurn = 0;

    _wtFail = 0.0;
    _agPrft = 0.0;
    _nomGdp = 0.0;
    _cpIndx = 0.0;
    _rlIncm = 0.0;
    _nmIncm = 0.0;
    _cmRInc = 0.0;
    _cmNInc = 0.0;
    _rlDebt = 0.0;
    _rlWage = 0.0;
    _entVac = 0.0;
    _nEnter = 0.0;
    _entrFr = 0.0;
    _nJbMch = 0.0;
    _nJb2Jb = 0.0;
    _nStMch = 0.0;
    _nSt2St = 0.0;
    _entShp = 0.0;
    _extShp = 0.0;
    _jobLss = 0.0;
    _jbLsLf = 0.0;
    _jbLsQt = 0.0;
    _jbLsEx = 0.0;
    _bkrpFr = 0.0;
    _fConFr = 0.0;
	_entrLn = 0.0;
	_fscInv = 0.0;
	_iniInv = 0.0;
	_aggCon = 0.0;
	_avgLVR = 0.0;

    /*Redefinition for new PITRGW in experiment*/
    _EQMTAX = 1 - _PITRGW * (1 - (_TMPRFW - 1) * _WKSPYR * _BNDINC) * _MARKUP
            / (_MARKUP - (_PITRGW - 1) * _INPCAP);

    _txRate = _EQMTAX;
    _wgRate = 1.0;
    _lValRt = _LVALRT;
    _fsaleP = _FSDISC*_wgRate * _PITRGW; // set in terms of expected prices next week
    _hcutPr = _lValRt*_wgRate * _PITRGW; // set in terms of expected prices next week
    _initPm = (1.0 - _txRate) / (_wgRate * _MARKUP);

    for (int i = 0; i < _MNSPYR; i++) {
        _lagdPc[i] = pow(_PITRGY, (i - (_MNSPYR - 1.0)) / _MNSPYR) / _initPm;
        _lagdPi[i] = _PITRGY;
        _lagdRI[i] = _CAPGDP;
        _lagdNI[i] = _lagdPc[i] * _CAPGDP;
        _lag3RI[i] = _CAPGDP;
        _lgCCRR[i] = log(_TMPRFY);
		_lgCCNR[i] = log(_TMPRFY * _PITRGY);
		if( _GAPTRG > 0.0) {
            _lagdRI[i] *= exp(-_GAPTRG);
            _lagdNI[i] *= exp(-_GAPTRG);
            _lag3RI[i] *= exp(-_GAPTRG);
        }
    }

    _ilggap = 0.0;
    _ilginf = 0.0;
    _gdppot = _GDPPOT;
    _gdpint = _GDPINT;
    _caRate = _CARATE;
    _rrttrg = _RRTTRG;
    _gdpryr = _GDPRYR;
    _infryr = _INFRYR;
    _gdpssq = 0.0;
    _gdpsum = 0.0;
    _infssq = 0.0;
    _annVal = (1.0 / (_TMPRFW - 1.0)) / _PITRGW;
    _wkIntr = _PITRGW * _TMPRFW;
    _monStk = _wgRate * (_NAGNTS - _NGOODS) + _MARKUP * _CAPGDP;
    _bndStk = _BNDINC * _wkIntr * _WKSPYR * _CAPGDP / _initPm;
    _csttyr = 0.0;

    if(_GAPTRG > 0.0) {
        _monStk *= exp(-_GAPTRG);
        _bndStk *= exp(-_GAPTRG);
    }

    for (int i = 0; i < _NSHOPS; i++) {
        Shop* sh = _shList[i];
        sh->clr_params(_NGOODS);
    }

    for (int i = 0; i < _NBANKS; i++) {
        int spIndex = i*_NGOODS/_NBANKS;
        _bkList[i]->ini_nwBank(this, _prList[spIndex][spIndex>1 ? spIndex-2 : _NGOODS-3]);
    }

    for (int i = 0; i < _NGOODS; i++)
        _frList[i]->ini_frsale();

    for (int i = 0; i < _NGOODS; i++) {
        _shList[i]->ini_nwShop(_prList[i][(i==0||i==_NGOODS-1) ? 0 : i-1]);
        _nSllrs[i]=1;
    }

    for (int i = 0; i < _NAGNTS; i++)
        _agList[i]->ini_nwAgnt(); // _agMode has already been set in ctor, ini_nwBank and ini

    if(_GAPTRG > 0.0) {
        for (int i = 0; i < _NAGNTS; i++)
            if (RandUtils::get_RnFrac() <= _GAPTRG)
                _agList[i]->lose_shops(None);
    }

    _crashd = false;
	_iniInv = get_agInvn();

    rnd_rnList();
}

void Economy::sim_implse() {
    if (_implse && _wkCntr == _IMPLAG * _WKSPYR) {
        if(_IMPULS) {
            if (strcmp(_IMPULS, "SHOPFL")==0) {
                _shopFl = true;
                return;
            }
            if (strcmp(_IMPULS, "BANKFL")==0) {
                _bankFl = true;
                return;
            }
            if (strcmp(_IMPULS, "BANKTR")==0) {
                _bankTr = true;
                return;
            }
            if (strcmp(_IMPULS, "BRKUPS")==0) {
                _brkups = true;
                return;
            }
        }
        WaitForSingleObject(CSLMTX, INFINITE);
        cout << "The parameter IMPULS is invalid" << endl;
        ReleaseMutex(CSLMTX);
        exit(EXIT_FAILURE);
    }
}

void Economy::sim_census() {
    if ((_wkCntr + 1) % (_WKSPYR * 5) != 0)
        return;

    _ecStat->col_cnStat();

    for (int i = 0; i < _nShops; i++) {
        Shop* sh = _shList[i];
        if (sh->is_active())
            sh->set_census(1);
    }

    _cnCntr++;
}

void Economy::sim_mktEnt() {
    if (!_TRNOVR) return;
    double salary = _wgRate * pow(_PITRGW, (_CNTPRD + 1) / 2);
    double fTests = 0; // number of entrepreneurs who take the financial viability test
    double fFaild = 0; // number of entrepreneurs who failed the test

    for (int i = 0; i < _NAGNTS; i++) {
        if (RandUtils::get_RnLess(_NAGNTS) >= _INFREQ) continue;
        int result = _rnList[i]->opn_fmShop(salary);
        if (result == 0) continue;
        fTests ++;
        if (result == 1) {fFaild ++; continue;}
        if (result == 3) {_nEnter++; _nActSh++;};
    }

    _fConFr = (fTests ? fFaild / fTests : 0); 
    _entVac = get_agVncy() / get_agEmpl();
    _entShp = _nActSh;
    if(_nActSh == 0) _crashd = true;
    _entrFr = _nEnter / _nActSh;
}

void Economy::sim_mktMch() {
    for (int i = 0; i < _NAGNTS; i++)
    _rnList[i]->find_shops();
}

void Economy::sim_finMkt() {
    if(_bankFl) {                  // force a bank to fail
        _bankFl = false;
        _bkList[0]->get_bkOwnr()->inc_monHld(max(_bkList[0]->upd_equity(), 0)); // nullify the wealth effect
        _bkList[0]->cause_fail(_MXBAIL*_nmIncm/_NBANKS + 1); // ensure failure
    }
    _crProb = (_NCROOK*_NGOODS*1.0)/(_NAGNTS-_nActSh-_NBANKS); // weekly setting of crook rate
    for (int i = 0; i < _NBANKS; i++) {
        if(_bkList[i]->upd_equity() < 0 && !_NOBANK) // the bankowner keeps family cash separate
            _bkList[i]->get_nwOwnr(_MXBAIL*_nmIncm/_NBANKS);
        if(!_NOBANK) _bkList[i]->get_checkd();        // check for capital adequacy
            _bkList[i]->set_lnIntr();
    }
    if(_bankTr) {                 // force a bank into trouble
        _bkList[0]->be_troubld();
        _nBkTrb++;
    }
  
    for (int i = 0; i < _NAGNTS; i++)
        _agList[i]->pln_budget();

    for (int i = 0; i < _NBANKS; i++)
        _bkList[i]->adj_pfolio();
}

void Economy::sim_mktTrd() {			
    for (int i = 0; i < _nShops; i++)
        _shList[i]->prep_Trade();
    for (int i = 0; i < _NAGNTS; i++) {	// Start trading loop
        if (RandUtils::get_RnLess(2) == 1) {
            _rnList[i]->buy_dGoods();
            if (!_rnList[i]->sell_labor()) {
              _jobLss++; _jbLsLf++;
            }
        }
        else {
            if (!_rnList[i]->sell_labor()) {
                _jobLss++; _jbLsLf++;
            }
            _rnList[i]->buy_dGoods();
        }
    } // End trading loop
    
    for (int i = 0; i < _nShops; i++)	// tally this week's profit and subtract depreciation
        _agPrft += _shList[i]->upd_accnts();
    _rlIncm = 0.0;	                  // tally this week's agg income and price index
    _nmIncm = 0.0;
    for (int i = 0; i < _nShops; i++)
        _shList[i]->upd_natInc(_rlIncm, _nmIncm);
    _nomGdp += _nmIncm;
    if (_rlIncm == 0.0) {
        _crashd = true;
        return;
    }
    _cpIndx  = _nmIncm / _rlIncm;
    _cmRInc += _rlIncm / _WKSPMN;	    // cumulative totals for use in Taylor Rule
    _cmNInc += _nmIncm / _WKSPMN;

    double tlWage = 0.0;              // tally this week's average wage (and related prices)
    double tlEmpl = 0.0;
    for (int i = 0; i < _nShops; i++)
        _shList[i]->upd_wgStat(tlWage, tlEmpl);
    _wgRate = tlWage / tlEmpl;
    _fsaleP = _FSDISC*_wgRate * _PITRGW; // set in terms of expected prices next week
    _hcutPr = _lValRt*_wgRate * _PITRGW; // set in terms of expected prices next week
    _rlWage = min(_wgRate / _cpIndx, 9.999);
}

void Economy::sim_monPol() {
    if ((_wkCntr + 1) % _WKSPMN != 0)
        return;
    _lagdPi[_mnCntr] = _cpIndx / _lagdPc[_mnCntr];
    _lagdPc[_mnCntr] = _cpIndx;
    _lagdRI[_mnCntr] = _cmRInc;
    _lagdNI[_mnCntr] = _cmNInc;
    _lgCCRR[_mnCntr] = _WKSPYR * log(_wkIntr) - log(_lagdPi[_mnCntr]);
	_lgCCNR[_mnCntr] = _WKSPYR * log(_wkIntr);

    if (_mnCntr)
        _lag3RI[_mnCntr] = _lag3RI[_mnCntr - 1] - _lagdRI[(_mnCntr + _MNSPYR - 3) % _MNSPYR] / 3;
    else 
        _lag3RI[_mnCntr] = _lag3RI[_MNSPYR - 1] - _lagdRI[(_mnCntr + _MNSPYR - 3) % _MNSPYR] / 3;

    _lag3RI[_mnCntr] += _lagdRI[_mnCntr]/3;

    /* DELAY IN ADJUSTING TARGETS */
    
    if ((_wkCntr + 1) % _WKSPYR == 0){   // on the annual basis
        double anngdp = 0.0;               // a local variable to compute annual gdp
        for (int i = 0; i < _MNSPYR; i++)  // compute annual gdp
              anngdp += _lagdRI[i];
        anngdp /= _MNSPYR;               // average weekly GDP during the last year (new observation)
    
        if(_yrCntr >= _LRNLAG){
            double gapErr = log(anngdp) - _gdpint - _gdpryr * _ilggap;
            double wgtcff = _yrCntr + 1;
            if(_GAPLMB != 1.0)
                wgtcff = (1 - pow(_GAPLMB, _yrCntr + 1)) / (1 - _GAPLMB);
            _gdpryr += ((wgtcff * _ilggap - _gdpsum) / (wgtcff * _gdpssq - pow(_gdpsum, 2))) * gapErr;
            _gdpint += ((_gdpssq - _ilggap * _gdpsum) /(wgtcff * _gdpssq - pow(_gdpsum, 2))) * gapErr; // update the estimate of the intercept in the gap equation
            _gdppot  = _gdpint / (1-_gdpryr); // update the estimate of log potential output
            _infryr += (1 / _infssq) * ((log(_lagdPi[_mnCntr]) - log(_PITRGY)) * (log(_ilginf) - log(_PITRGY)) - pow(log(_ilginf) - log(_PITRGY), 2) * _infryr);
        }
        if (_gdpryr >= 1.0) {
            _crashd = true;
            return;
        }
        _gdpssq = _GAPLMB * _gdpssq + pow(log(anngdp), 2); // sum of squared log gdps
        _gdpsum = _GAPLMB * _gdpsum + log(anngdp);         // simple sum of log gdps

        _infssq = _INFLMB * _infssq + pow(log(_lagdPi[_mnCntr]) - log(_PITRGY), 2);

        _ilggap = log(anngdp);           // to be use in updating coefficients when the time comes
        _ilginf = _lagdPi[_mnCntr];	
    }

    if(_yrCntr >= _LRNLAG) // monthly adjustment of real IR target
        _rrttrg += do_squash(_ADJRRT * (_lagdPi[_mnCntr] - _PITRGY), _rrttrg - 1.0, _RRTTRG - 1.0);
    
    _caRate = _CARATE + do_squash(_CASLOP * (log(_lag3RI[_mnCntr]) - _gdppot), _CARATE, _CARATE); // adjust capital adequacy ratio
    
    _lValRt = _LVALRT + do_squash(_LVSLOP * (log(_lag3RI[_mnCntr]) - _gdppot), _LVALRT, _LVALRT); // adjust loan-to-value ratio

    _csttyr = log(_rrttrg) + (1 - _INFTYR) * log(_PITRGY) - _GAPTYR * _gdppot;

    _wkIntr = get_taylor(log(_lagdPi[_mnCntr]), log(_lag3RI[_mnCntr]));

    _annVal = get_annVal(log(_lagdPi[_mnCntr]), log(_lag3RI[_mnCntr]));

    _cmRInc = 0.0;
    _cmNInc = 0.0;

	_avgLVR += _lValRt / _MNSPYR;

    if ((_wkCntr + 1 - _WKSPMN) % _WKSPYR == 0) // why - _WKSPMN??
        _zbCntr = 0;
    if (_wkIntr == 1.0)
        _zbCntr++;
}

void Economy::sim_mktBrk() {
    if (!_SHOCKS) return;
    double brkfrc = (_brkups ? _BRKPRC : _BRKFRC);
    int uCount = 0;
    for (int i = 0; i < _NAGNTS; i++)
        if(RandUtils::get_RnFrac() <= brkfrc)
            uCount += _agList[i]->lose_shops(Quit);
    _jobLss += uCount;
    _jbLsQt += uCount;
}

void Economy::sim_fisPol() {
  
    _bndStk = 0.0;
    _monStk = 0.0;

    for (int i = 0; i < _NAGNTS; i++)
        _monStk += _agList[i]->get_monHld();

    for (int i = 0; i < _NBANKS; i++) {
        _monStk += _bkList[i]->get_reserv();
        _bndStk += _bkList[i]->get_gBonds();
    }
  
    if ((_wkCntr + 1) % _WKSPYR != 0)
        return;
    
    _rlDebt = _bndStk / (_wkIntr * _lagdPc[_mnCntr]);
    double newTax = get_fiscal(_rlDebt);

    if (newTax != _txRate)
        _txRate = newTax;

    _fscInv = get_agInvn();
}

void Economy::sim_mktExt() {
    if (!_TRNOVR) return;
    
    double aggEmp = get_agEmpl();
    _bkrpFr = 0.0;
  
    for (int i = 0; i < _nShops; i++) {
        Shop* sh  = _shList[i];
        if(!sh->is_active()) continue;
        int quit = sh->pln_toExit(_wgRate, _SHOCKS, _hcutPr);
        if( quit || _shopFl) {
            _shopFl = false;
            _wtFail += sh->get_aInput() / aggEmp;
            _nFails++;
            _nActSh--;
            _nHplss += (quit == 1);
            if (sh->was_iniShp())
                _nAlive--;
            else {
                _enLfSp += _wkCntr - sh->get_entrWk();
                _nEntEx ++;
            }
            _nSllrs[sh->get_shGood()-1]--;
            _bkrpFr += static_cast<double>(sh->cls_fmShop(_wgRate)) / (_nActSh+1);
      
            int uCount = 0;
            for (int j = 0; j < _NAGNTS; j++)
                uCount += _agList[j]->lose_aShop(sh, Exit);
            _jobLss += uCount;
            _jbLsEx += uCount;
        }
    }
    _extShp  = _nActSh;
}

void Economy::sim_shpUpd() {
    for (int i = 0; i < _nShops; i++) {
        Shop* sh = _shList[i];
        if (sh->is_active()) { 
            sh->upd_tSales();
            if(sh->upd_prices()) { // Add a pDelta occurence and adjust share parameters
                _nPrDlt++;
                adj_shPars(sh);
            }
            _nPrObs++;             // Add an observed pDelta opportunity
        }
    }
}

void Economy::inc_tmCntr() {
    _ecStat->col_wkStat();
    _entVac = 0.0;
    _nEnter = 0.0;
    _entrFr = 0.0;
    _nJbMch = 0.0;
    _nJb2Jb = 0.0;
    _nStMch = 0.0;
    _nSt2St = 0.0;
    _entShp = 0.0;
    _extShp = 0.0;
    _jobLss = 0.0;
    _jbLsLf = 0.0;
    _jbLsQt = 0.0;
    _jbLsEx = 0.0;
	_entrLn = 0.0;
    _enLfSp = 0;
    _nEntEx = 0;

    _nUn2Em = 0; _unDurn = 0;
    _nLf2Em = 0; _lfDurn = 0;
    _nEx2Em = 0; _exDurn = 0;
    _nQt2Em = 0; _qtDurn = 0;
    _nBk2Em = 0; _bkDurn = 0;

    for (int i = 0; i < _nShops; i++)
        _shList[i]->set_aSales(0);

    if ((_wkCntr + 1) % _WKSPMN == 0) {
        _ecStat->col_mnStat();
        _mnCntr++;
    }

    if ((_wkCntr + 1) % _WKSPYR == 0) {
        _ecStat->col_yrStat();
        _wtFail = 0.0;
        _agPrft = 0.0;
        _nomGdp = 0.0;
		_aggCon = 0.0;
		_avgLVR = 0.0;
        _nFails = 0;
        _nHplss = 0;
        _nPrDlt = 0;
        _nPrObs = 0;
        _nBkFlr = 0;
        _nBkTrb = 0;
        _mnCntr = 0;
        _bankTr = false;
        _brkups = false;
        _yrCntr++;    
    }

    for (int a = 0; a < _NAGNTS; a++) _agList[a]->add_intrst(_wkIntr);
    for (int b = 0; b < _NBANKS; b++) _bkList[b]->add_intrst(_wkIntr);

    _wkCntr++;
}

void Economy::prt_ryStat() {
    if (_wkCntr % _WKSPYR != 0)
        return;

	if (_rystrm != NULL) {
		fprintf(_rystrm, "%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
			_RNSEED + _rnCntr,															/* Unique identifier (seed) of the current run */
			_yrCntr,																	/* Current year */
			_ecStat->_yeLnGp[_yrCntr - 1],												/* Average weekly log output gap over the current year */
			_ecStat->_yeIncm[_yrCntr - 1] * _WKSPYR,									/* Total real GDP over the current year */
			_ecStat->_yeNInc[_yrCntr - 1] * _WKSPYR,									/* Total nominal GDP over the current year */
			_ecStat->_yeCons[_yrCntr - 1],												/* Total real consumption expenditures over the current year */
			_ecStat->_yeIncm[_yrCntr - 1] * _WKSPYR - _ecStat->_yeCons[_yrCntr - 1],	/* Total real investment expenditures over the current year */
			_ecStat->_yeInvn[_yrCntr - 1],												/* Total stock of year-end real inventories for the current year */
			_ecStat->_yeDInv[_yrCntr - 1],												/* Change in total year-end real inventories since last year */
			_ecStat->_yeEmpl[_yrCntr - 1],												/* Average weekly employment over the current year */
			_ecStat->_yeUnRt[_yrCntr - 1],												/* Average weekly unemployment rate over the current year */
			_ecStat->_yeVcRt[_yrCntr - 1],												/* Average weekly job vacancy rate over the current year */
			_ecStat->_yeRlWg[_yrCntr - 1],												/* Average weekly real wage rate over the current year */
			_ecStat->_yeIncm[_yrCntr - 1] * _WKSPYR / _ecStat->_yeEmpl[_yrCntr - 1],	/* Average labor productivity for the current year */
			_ecStat->_yePIdx[_yrCntr - 1],												/* Average weekly price index over the current year */
			_ecStat->_yeInfl[_yrCntr - 1] - 1.0,										/* Simple annual inflation rate since last year */
			_ecStat->_RYWtMk[_rnCntr][_yrCntr - 1] - 1.0,								/* Average weekly cross-firm sales-weighted mean markup over the current year */
			_rlDebt / (_ecStat->_yeIncm[_yrCntr - 1] * _WKSPYR),						/* Government debt to GDP ratio for the current year */
			exp(_ecStat->_yeCCRR[_yrCntr - 1]) - 1.0,									/* Average weekly annualized real interest rate over the current year */
			exp(_ecStat->_yeCCNR[_yrCntr - 1]) - 1.0,									/* Average weekly annualized nominal interest rate over the current year */
			_ecStat->_yeLoan[_yrCntr - 1] * _WKSPYR,									/* Total nominal bank loans over the current year */
			_monStk,																	/* Total year-end money supply for the current year */
			_ecStat->_yeBkrp[_yrCntr - 1],												/* Average weekly hazard rate of firm bankruptcy over the current year */
			_ecStat->_yeFCon[_yrCntr - 1],												/* Average weekly hazard rate of failing the financial viability test at market entry over the current year */
			_ecStat->_RYBkTr[_rnCntr][_yrCntr - 1]);									/* Average weekly hazard rate of banks getting into trouble over the current year */
	}
}

void Economy::inc_rnCntr() {
    _ecStat->col_rnStat();
    _rnCntr++;
}

void Economy::prt_smStat() {
    char TMFILE[256] = "";
    strncat_s(TMFILE, _RSFILE, strlen(_RSFILE) - 4);
    strcat_s (TMFILE, "_TIMESERIES.csv");

    FILE* tmstrm;
    for (;;) {
        if (fopen_s(&tmstrm, TMFILE, "w") != 0) {
            cout << endl
                 << "Problem writing to " << TMFILE << endl
                 << " Try closing it and hitting [Enter]" << endl
                 << " or if you don't want this file, just type n" << endl << endl;
            if (cin.get() == 'n') {
                cout << "Cannot print simulation results to " << TMFILE << endl;
                break;
            }
        }
        else {
            fprintf(tmstrm, "Year,LogGap,Markup,Profit,RelAge,Debt/GDP,StDevMarkup,GapTarget,RrtTarget\n");
            for (int i = 0; i < _NYEARS; i++)
                fprintf(tmstrm, "%d,%f,%f,%f,%f,%f,%f,%f,%f\n", i + 1,
                                _ecStat->_CRLnGp[i],
                                _ecStat->_CRWtMk[i], _ecStat->_CRWtPf[i],
                                _ecStat->_CRRAge[i], _ecStat->_CRDbtR[i],
                                _ecStat->_CRStMk[i], _ecStat->_CRGpTg[i],
                                _ecStat->_CRRtTg[i]);
            fclose(tmstrm);
            break;
        }
    }
}

// check for negative values
void Economy::dbg_chkNeg() {
    for (int i = 0; i < _nShops; i++)
        _shList[i]->dbg_chkNeg();
    for (int i = 0; i < _NAGNTS; i++)
        _agList[i]->dbg_chkNeg();
    for (int i = 0; i < _NBANKS; i++)
        _bkList[i]->dbg_chkNeg();
}

void Economy::inc_shList() {
    Shop** nwList = new Shop*[_nShops + _NGOODS];
    for (int i = 0; i < _nShops + _NGOODS; i++) {
        if (i < _nShops) {
             nwList[i] = _shList[i];
            _shList[i] = NULL;
        }
        else {
             nwList[i] = new Shop(i + 1, nwList[_nShops - 1], _NGOODS);
             nwList[i]->ini_markup(_MARKUP);
        }
    }
    delete[] _shList;
    _shList = nwList;
    _nShops = _nShops + _NGOODS;
}

void Economy::rnd_rnList() {
    for (int i = 0; i < _NAGNTS; i++)
        _rnList[i] = _agList[i];

    for (int i = 0; i < _NAGNTS - 1; i++) {
        int r = i + RandUtils::get_RnLess(_NAGNTS - i);
        Agent* ag  = _rnList[i];
        _rnList[i] = _rnList[r];
        _rnList[r] = ag;
    }
}

void Economy::inc_jMchSt(Shop* sh) {
    _nJbMch++;
    _nJb2Jb += (sh != NULL);
}

void Economy::inc_sMchSt(Shop* sh) {
    _nStMch++;
    _nSt2St += (sh != NULL);
}

void Economy::inc_nBkFlr() {_nBkFlr++;}

void Economy::inc_nBkTrb() {_nBkTrb++;}

void Economy::inc_nSllrs(int gIndex) {_nSllrs[gIndex] ++;}

void Economy::inc_entrLn(double amount) {_entrLn += amount;}

void Economy::inc_aggCon(double amount) {_aggCon += amount;}

void Economy::inc_drStat(UnType unType, int unWeek) {
    // only consider agents who are finding jobs from a jobless state
    // and whose unemployment episodes began after the adjustment lag
    if (unType == None || unWeek + 1 <= _ADJLAG * _WKSPYR)
        return;

    switch (unType) {
        case Layoff :
            _lfDurn += _wkCntr - unWeek;
            _nLf2Em++;
            break;
        case Quit :
            _qtDurn += _wkCntr - unWeek;
            _nQt2Em++;
            break;
        case Exit :
            _exDurn += _wkCntr - unWeek;
            _nEx2Em++;
            break;
        case Broke :
            _bkDurn += _wkCntr - unWeek;
            _nBk2Em++;
            break;
        default :
            break;
    }

    _unDurn += _wkCntr - unWeek;
    _nUn2Em++;
}

_forceinline double Economy::get_taylor(double yrCCPi, double logGDP) {
    if(_ZEROLB) return max(1.0, exp((_csttyr + _INFTYR*yrCCPi + _GAPTYR*logGDP)/_WKSPYR));
    else        return          exp((_csttyr + _INFTYR*yrCCPi + _GAPTYR*logGDP)/_WKSPYR) ;
}


double Economy::get_annVal(double yrCCPi, double logGDP) {
    double annVal = 0.0;
    double endVal = 1.0;	// value of 1 unit at start of month, discounted to wk 0
    do {
        double wkEInf = exp(yrCCPi / _WKSPYR);
        // rlDisc is the real weekly discount factor
        double rlDisc = wkEInf / get_taylor(yrCCPi, logGDP);
        // annVlM is the pres val of a real unit per week during the month
        double annVlM = rlDisc;
        for(int i = 0; i < _WKSPMN - 1; i++)
            annVlM  = rlDisc*(1.0 + annVlM);

        annVal += annVlM * endVal / wkEInf;
        endVal *= pow(rlDisc, _WKSPMN);
        yrCCPi += (1.0 - pow(_infryr, 1.0 / _MNSPYR)) * (log(_PITRGY) - yrCCPi);
        logGDP += (1.0 - pow(_gdpryr, 1.0 / _MNSPYR)) * (_gdppot      - logGDP);
        _anvCnt++;
    } while((_GAPTYR * abs(logGDP - _gdppot) + _INFTYR * abs(yrCCPi - log(_PITRGY))) * endVal > 1.0e-015);
  
    annVal += ( endVal / (pow(_rrttrg, (double) 1/_WKSPYR)-1.0) ) / _PITRGW;
    return annVal;
}

double Economy::get_fiscal(double rlDebt) {
    double txRate = _EQMTAX + _FSCSPD * (rlDebt / (_WKSPYR * exp(_gdppot)) - _BNDINC);
    return max(min(txRate, 0.99), -0.5);
}

double Economy::get_agEmpl() {
    double agEmpl = 0.0;
    for (int i = 0; i < _nShops; i++) 
    agEmpl += _shList[i]->/*upd_emStat*/get_aInput();
    return agEmpl;
}

double Economy::get_agVncy() {
    double agVncy = 0.0;
    for (int i = 0; i < _nShops; i++) 
    agVncy += _shList[i]->upd_joStat();
    return agVncy;
}

double Economy::get_agInvn() {
    double agInvn = 0.0;
    for (int i = 0; i < _nShops; i++) {
        Shop* sh = _shList[i];
        if (sh->is_active())
            agInvn += sh->get_invent();
    }
    return agInvn;
}

void Economy::sim_ABECON(){

#ifdef USE_TIMERS
    const int num_sections = 13;
    enum {IMP=0, CEN, ENT, MCH, BND, TRD, MON, BRK, FSC, EXT, UPD, TMC, YRS};
    char* Sections[] = {"Imp", "Cen", "Ent", "Mch", "Bnd", "Trd", "Mon", "Brk", "Fsc", "Ext", "Upd", "Tmc", "Yrs"};
    double Times[num_sections] = {0};
    clock_t bgnSec = 0;
    clock_t endSec = 0;
#endif

    clock_t  BEGRUN = 0;
    clock_t  ENDRUN = 0;
    //ofstream DIAGNO;
    //ofstream DIACFL;

	for (;;) {
		if (fopen_s(&_rystrm, _RSFILE, "w") != 0) {
			cout << endl
				<< "Problem writing to " << _RSFILE << endl
				<< " Try closing it and hitting [Enter]" << endl
				<< " or if you don't want this file, just type n" << endl << endl;
			if (cin.get() == 'n') {
				cout << "Cannot print simulation results to " << _RSFILE << endl;
				break;
			}
		}
		else {
			fprintf(_rystrm, "RunSeed,Year,LogGap,RealGDP,NominalGDP,Consumption,Investment,Inventories,DeltaInventories,Employment,UnemploymentRate,VacancyRate,RealWageRate,Productivity,PriceIndex,Inflation,Markup,Debt2GDP,RealInterestRate,NominalInterestRate,Credit,MoneySupply,BankruptcyHazard,FinancialConstraintHazard,BankTroubleHazard\n");
			break;
		}
	}

	for (int r = 0; r < _NSRUNS; r++) {
        BEGRUN = clock();	// run start time
        ini_forRun();		// prepare for a new run
        
        for (int w = 0; w < _NWEEKS; w++) {
            /*for(int i = 0; i < _NAGNTS; i++) {
                if((_agList[i]->get_fmShop() != NULL) && (_agList[i]->get_agMode()!=_shOwnr))
                i=i;
                if((_agList[i]->get_fmBank() != NULL) && (_agList[i]->get_agMode()!=_bkOwnr))
                i=i;
                if((_agList[i]->get_fmBank() == NULL) && (_agList[i]->get_agMode()==_bkOwnr))
                i=i;
                if((_agList[i]->get_fmBank() == NULL) && (_agList[i]->get_fmShop() == NULL) &&
                    (_agList[i]->get_agMode() != _worker))
                    i=i;
            }*/
            //if (r==1 && w==0) {
            //    if (_implse) {
            //        DIASHK.open("CAL_RESULTS_DIAGNO_SHK.TXT", ios_base::out | ios_base::trunc);
            //        DIASHK << this;
            //        DIASHK.close();
            //    }
            //    else {
            //        DIACFL.open("CAL_RESULTS_DIAGNO_CFL.TXT", ios_base::out | ios_base::trunc);
            //        DIACFL << this;
            //        DIACFL.close();
            //    }
            //}

            START_TIMER;
            sim_implse();		TIME_UP(IMP);	// apply impulse (once in a run, only in impulse mode)
            sim_census();		TIME_UP(CEN);	// conduct census (5 yearly)
            sim_mktEnt();		TIME_UP(ENT);	// market entry (weekly)
            if(_crashd) break;               
            /*if (w==0) {
                DIAGNO.open("CAL_RESULTS_DIAGNO.TXT", ios_base::out | ios_base::trunc);
                DIAGNO << this;
                DIAGNO.close();
            }

            if (w==0) {
                double Loans = 0.0;
                for (int i=0; i<_NBANKS; ++i)
                    Loans += _bkList[i]->get_sLoans();
                if (Loans>0.0) 
                    cout << r << "\t" << _THREAD << "\n";
            }*/

            sim_mktMch();		TIME_UP(MCH);	// market matching (weekly)
            sim_finMkt();		TIME_UP(BND);	// bond market activities (weekly)
            sim_mktTrd();		TIME_UP(TRD);	// market trading (weekly)
            if(_crashd) break;
            sim_monPol();		TIME_UP(MON);	// monetary policy (monthly)
            if(_crashd) break;
            //dbg_chkNeg();
            sim_mktBrk();		TIME_UP(BRK);	// market match breakups (weekly)
            sim_fisPol();		TIME_UP(FSC);	// fiscal policy (yearly)
            sim_mktExt();		TIME_UP(EXT);	// market exit (weekly)
            sim_shpUpd();		TIME_UP(UPD);	// update store prices and targets (weekly)
            inc_tmCntr();		TIME_UP(TMC);	// increment time counters
            prt_ryStat();		TIME_UP(YRS);	// print yearly statistics to results file
        }

        inc_rnCntr();		// increment run counter
        ENDRUN = clock();	// run end time

        // print to console one thread at a time
        WaitForSingleObject(CSLMTX, INFINITE);
        if(_crashd)
            cout << "CRASH week " << _wkCntr+1 << ": RUN " << r+1 << ": THREAD " << _THREAD << endl;
        else {
            cout << "COMPLETED RUN " << r + 1;
            cout.precision(2);
            cout.setf(ios::fixed | ios::showpoint);
            cout << " FOR THREAD " << _THREAD;
            cout << " IN " << (double)(ENDRUN - BEGRUN) / CLOCKS_PER_SEC
                << " SECS with " << _nAlive
                << " survivors" << " and gdp-loan correlation equal to " << _ecStat->_YLCorD[r]
                << endl;
        }
        ReleaseMutex(CSLMTX);
    }

	fclose(_rystrm);

    PRINT_TIMES
}

_forceinline double Economy::do_squash(double change, double absLim, double equLim) {
    return change*absLim / sqrt(pow(change,2) + pow(equLim,2));
}

void Economy::adj_shPars(Shop* sh) {
    for(int i = 0; i < _numCns[sh->get_shGood()-1]; i++)
        _cnList[sh->get_shGood()-1][i]->adj_shareP(sh, 0);
    int otherConsGood = (sh->get_shGood()-1? sh->get_shGood()-2 : _NGOODS-1);
    for(int i = 0; i < _numCns[otherConsGood]; i++)
        _cnList[otherConsGood][i]->adj_shareP(sh, 1);
}

Shop* Economy::get_nextSh() {
    int j = 0;
    while (_shList[j]->is_active()) j++;
    if (j < _nShops - 1)
        return _shList[j];
    else {
        inc_shList();
        return _shList[j+1];
    }
}

Agent* Economy::get_aCrook(int sector) const {
    require(!_bkList[sector]->is_troubld(), "New loan has been made by a troubled bank");
    int i = sector+1;
    while (_bkList[i % _NBANKS]->is_troubld()) i++;
    return _crList[i % _NBANKS][RandUtils::get_RnLess(_NCROOK * _NGOODS / _NBANKS)];
} 

Shop* Economy::get_shList(int number) const {return _shList[number];}
Bank* Economy::get_bkList(int sector) const {return _bkList[sector];}
Agent* Economy::get_agList(int number) const {return _agList[number];}
Agent* Economy::get_prList(int gIndex, int number) const {return _prList[gIndex][number];}
Agent* Economy::get_cnList(int gIndex, int number) const {return _cnList[gIndex][number];}
Firesale** Economy::get_frList() const {return _frList;}
WorkerMan* Economy::get_worker() const {return _worker;} 
ShopOwner* Economy::get_sOwner() const {return _sOwner;} 
BankOwner* Economy::get_bOwner() const {return _bOwner;} 
Statistics* Economy::get_ecStat() const {return _ecStat;}
const int Economy::get_NGOODS() {return _NGOODS;}

const int Economy::get_TRNOVR() {return _TRNOVR;}

const int Economy::get_MAXSLS() {return _MAXSLS;}

const int Economy::get_NCROOK() {return _NCROOK;}

const int Economy::get_NYEARS() {return _NYEARS;}

const int Economy::get_NMNTHS() {return _NMNTHS;}

const int Economy::get_MNSPYR() {return _MNSPYR;}

const int Economy::get_NAGNTS() {return _NAGNTS;}

const int Economy::get_NBANKS() {return _NBANKS;}

const int Economy::get_NOBANK() {return _NOBANK;}

const int Economy::get_RNDMRK() {return _RNDMRK;}

const int Economy::get_WKSPMN() {return _WKSPMN;}

const int Economy::get_wkCntr() {return _wkCntr;}

const int Economy::get_nShops() {return _nShops;}

const int Economy::get_numPrs(int gIndex) {return _numPrs[gIndex];}

const int Economy::get_numCns(int gIndex) {return _numCns[gIndex];}

const double Economy::get_anvCnt() {return _anvCnt;}

const double Economy::get_DMDELS() {return _DMDELS;}

const double Economy::get_SRCHPP() {return _SRCHPP;}

const double Economy::get_FDICPR() {return _FDICPR;}

const double Economy::get_INCSPD() {return _INCSPD;}

const double Economy::get_SETUPC() {return _SETUPC;}

const double Economy::get_PITRGY() {return _PITRGY;}

const double Economy::get_FXDCST() {return _FXDCST;}

const double Economy::get_PITRGW() {return _PITRGW;}

const double Economy::get_FREQTR() {return _FREQTR;}

const double Economy::get_SPNDPP() {return _SPNDPP;}

const double Economy::get_STDMKP() {return _STDMKP;}

const double Economy::get_MARKUP() {return _MARKUP;}

const double Economy::get_GAPTRG() {return _GAPTRG;}

const double Economy::get_CAPGDP() {return _CAPGDP;}

const double Economy::get_annVal() {return _annVal;}

const double Economy::get_txRate() {return _txRate;}

const double Economy::get_wgRate() {return _wgRate;}

const double Economy::get_fsaleP() {return _fsaleP;}

const double Economy::get_hcutPr() {return _hcutPr;}

const double Economy::get_wkIntr() {return _wkIntr;}

const double Economy::get_caRate() {return _caRate;}

const double Economy::get_lValRt() {return _lValRt;}

const double Economy::get_bndStk() {return _bndStk;}

const double Economy::get_initPm() {return _initPm;}

const double Economy::get_crProb() {return _crProb;}

void Economy::set_implse(bool truFal) {_implse = truFal;}