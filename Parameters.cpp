/***********************************************************************************************
 * Parameter.cpp
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised May 25 2010
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

#include "Parameters.h"
#include <math.h>
#include "require.h"

 Parameters::Parameters(char* flName) {
	_OPMODE = NULL;
	_EXPVAR = NULL;
	_SNSVAR = NULL;
  _IMPULS = NULL;
	_NTHRDS =  0;
	_NSRUNS =  0;
	_NWEEKS =  0;
	_ADJLAG =  0;
	_LRNLAG =  0;
	_IMPLAG =  0;
	_NGOODS =  0;
	_WKSPMN =  0;
	_WKSPYR =  0;
	_MAXSLS =  0;
	_CNTPRD =  0;
	_RNSEED = -1;
	_RNDMRK = -1;
	_TRNOVR = -1;
	_FLEXWG = -1;
	_ZEROLB = -1;
	_SHOCKS = -1;
	_DISEQU = -1;
	_USEEND = -1;
	_USECOR = -1;
	_THREAD = -1;
  _NOBANK = -1;
	_INIPRD =  0;
	_NBANKS =  0;
  _NCROOK = 0;

	_MARKUP = 0.0;
	_FAILRT = 0.0;
	_FXDCST = 0.0;
	_INVSPD = 0.0;
	_WAGSPD = 0.0;
	_INFREQ = 0.0;
	_DMDELS = 0.0;
	_INCSPD = 0.0;
	_TMPRFY = 0.0;
	_BRKFRC = 0.0;
	_TRIGGR = 0.0;
	_LTRIGG = 0.0;
	_PDELTA = 0.0;

	_EXPINI = 0.0;
	_EXPINC = 0.0;
	_SNSDEV = 0.0;

	_BNDINC = 0.0;
	_FSCSPD = 0.0;
	_INFCOR = 0.0;
	_GAPCOR = 0.0;
	_INFTYR = 0.0;
	_GAPTYR = 0.0;
	_GAPTRG = 0.0;
	_GDPINT = 0.0;
	_RRTTRG = 0.0;
	_PITRGY = 0.0;
	_STDMKP = 0.0;
	_ADJRRT = 0.0;
	_GAPLMB = 0.0;
	_INFLMB = 0.0;
	_SRCHPP = 0.0;
  _FREQTR = 0.0;
	_SPREAD = 0.0;
	_CARATE = 0.0;
	_DISCPR = 0.0;
  _CASLOP = 0.0;
  _LVSLOP = 0.0;
  _FDICPR = 0.0;
  _EQUFRC = 0.0;
  _BRKPRC = 0.0;
  _MXBAIL = 0.0;
  _BKRCST = 0.0;
	_SETUPC = 0.0;
  _PSLOPE = 0.0;

	FILE* flStrm;
	char  flLine[256];
	char* nmTokn;
	char* vlTokn;
	char* contxt;

	if (fopen_s(&flStrm, flName, "r") == 0) {
		while (fgets(flLine, 256, flStrm) != NULL) {
			nmTokn = strtok_s(flLine, " ="  , &contxt);
			vlTokn = strtok_s(NULL  , " =\n", &contxt);
      
			if (strcmp(nmTokn, "OPMODE") == 0) {
				_OPMODE = new char[20];
				strcpy_s (_OPMODE, 20, vlTokn);
			}
			if (strcmp(nmTokn, "EXPVAR") == 0) {
				_EXPVAR = new char[7];
				strcpy_s (_EXPVAR, 7, vlTokn);
			}
			if (strcmp(nmTokn, "SNSVAR") == 0) {
				_SNSVAR = new char[7];
				strcpy_s (_SNSVAR, 7, vlTokn);
			}
			if (strcmp(nmTokn, "IMPULS") == 0) {
				_IMPULS = new char[7];
				strcpy_s (_IMPULS, 7, vlTokn);
			}


			if (strcmp(nmTokn, "NTHRDS") == 0) _NTHRDS = atoi(vlTokn);
			if (strcmp(nmTokn, "NSRUNS") == 0) _NSRUNS = atoi(vlTokn);
			if (strcmp(nmTokn, "NWEEKS") == 0) _NWEEKS = atoi(vlTokn);
			if (strcmp(nmTokn, "ADJLAG") == 0) _ADJLAG = atoi(vlTokn);
			if (strcmp(nmTokn, "LRNLAG") == 0) _LRNLAG = atoi(vlTokn);
			if (strcmp(nmTokn, "IMPLAG") == 0) _IMPLAG = atoi(vlTokn);
			if (strcmp(nmTokn, "NGOODS") == 0) _NGOODS = atoi(vlTokn);
			if (strcmp(nmTokn, "WKSPMN") == 0) _WKSPMN = atoi(vlTokn);
			if (strcmp(nmTokn, "WKSPYR") == 0) _WKSPYR = atoi(vlTokn);
			if (strcmp(nmTokn, "RNSEED") == 0) _RNSEED = atoi(vlTokn);
			if (strcmp(nmTokn, "RNDMRK") == 0) _RNDMRK = atoi(vlTokn);
			if (strcmp(nmTokn, "TRNOVR") == 0) _TRNOVR = atoi(vlTokn);
			if (strcmp(nmTokn, "FLEXWG") == 0) _FLEXWG = atoi(vlTokn);
			if (strcmp(nmTokn, "ZEROLB") == 0) _ZEROLB = atoi(vlTokn);
			if (strcmp(nmTokn, "SHOCKS") == 0) _SHOCKS = atoi(vlTokn);
			if (strcmp(nmTokn, "DISEQU") == 0) _DISEQU = atoi(vlTokn);
			if (strcmp(nmTokn, "USEEND") == 0) _USEEND = atoi(vlTokn);
			if (strcmp(nmTokn, "USECOR") == 0) _USECOR = atoi(vlTokn);
			if (strcmp(nmTokn, "INIPRD") == 0) _INIPRD = atoi(vlTokn);
			if (strcmp(nmTokn, "NBANKS") == 0) _NBANKS = atoi(vlTokn);
			if (strcmp(nmTokn, "NOBANK") == 0) _NOBANK = atoi(vlTokn);
			if (strcmp(nmTokn, "NCROOK") == 0) _NCROOK = atoi(vlTokn);

			if (strcmp(nmTokn, "MAXSLS") == 0) _MAXSLS = atoi(vlTokn);
			if (strcmp(nmTokn, "CNTPRD") == 0) _CNTPRD = atoi(vlTokn);
			if (strcmp(nmTokn, "MARKUP") == 0) _MARKUP = atof(vlTokn);
			if (strcmp(nmTokn, "FAILRT") == 0) _FAILRT = atof(vlTokn);
			if (strcmp(nmTokn, "FXDCST") == 0) _FXDCST = atof(vlTokn);
			if (strcmp(nmTokn, "INVSPD") == 0) _INVSPD = atof(vlTokn);
			if (strcmp(nmTokn, "WAGSPD") == 0) _WAGSPD = atof(vlTokn);
			if (strcmp(nmTokn, "INFREQ") == 0) _INFREQ = atof(vlTokn);
			if (strcmp(nmTokn, "DMDELS") == 0) _DMDELS = atof(vlTokn);
			if (strcmp(nmTokn, "INCSPD") == 0) _INCSPD = atof(vlTokn);
			if (strcmp(nmTokn, "TMPRFY") == 0) _TMPRFY = atof(vlTokn);
			if (strcmp(nmTokn, "BRKFRC") == 0) _BRKFRC = atof(vlTokn);
			if (strcmp(nmTokn, "TRIGGR") == 0) _TRIGGR = atof(vlTokn);
			if (strcmp(nmTokn, "LTRIGG") == 0) _LTRIGG = atof(vlTokn);
			if (strcmp(nmTokn, "PDELTA") == 0) _PDELTA = atof(vlTokn);

			if (strcmp(nmTokn, "EXPINI") == 0) _EXPINI = atof(vlTokn);
			if (strcmp(nmTokn, "EXPINC") == 0) _EXPINC = atof(vlTokn);
			if (strcmp(nmTokn, "SNSDEV") == 0) _SNSDEV = atof(vlTokn);

			if (strcmp(nmTokn, "BNDINC") == 0) _BNDINC = atof(vlTokn);
			if (strcmp(nmTokn, "FSCSPD") == 0) _FSCSPD = atof(vlTokn);
			if (strcmp(nmTokn, "INFCOR") == 0) _INFCOR = atof(vlTokn);
			if (strcmp(nmTokn, "GAPCOR") == 0) _GAPCOR = atof(vlTokn);
			if (strcmp(nmTokn, "INFTYR") == 0) _INFTYR = atof(vlTokn);
			if (strcmp(nmTokn, "GAPTYR") == 0) _GAPTYR = atof(vlTokn);
			if (strcmp(nmTokn, "GAPTRG") == 0) _GAPTRG = atof(vlTokn);
			if (strcmp(nmTokn, "RRTTRG") == 0) _RRTTRG = atof(vlTokn);
			if (strcmp(nmTokn, "PITRGY") == 0) _PITRGY = atof(vlTokn);
			if (strcmp(nmTokn, "STDMKP") == 0) _STDMKP = atof(vlTokn);
			if (strcmp(nmTokn, "ADJRRT") == 0) _ADJRRT = atof(vlTokn);
			if (strcmp(nmTokn, "GAPLMB") == 0) _GAPLMB = atof(vlTokn);
			if (strcmp(nmTokn, "INFLMB") == 0) _INFLMB = atof(vlTokn);
			if (strcmp(nmTokn, "SRCHPP") == 0) _SRCHPP = atof(vlTokn);
			if (strcmp(nmTokn, "FREQTR") == 0) _FREQTR = atof(vlTokn);
			if (strcmp(nmTokn, "SPREAD") == 0) _SPREAD = atof(vlTokn);/*
			if (strcmp(nmTokn, "NWPREM") == 0) _NWPREM = atof(vlTokn);*/
			if (strcmp(nmTokn, "CARATE") == 0) _CARATE = atof(vlTokn);
			if (strcmp(nmTokn, "DISCPR") == 0) _DISCPR = atof(vlTokn);
			if (strcmp(nmTokn, "FSDISC") == 0) _FSDISC = atof(vlTokn);
			if (strcmp(nmTokn, "LVALRT") == 0) _LVALRT = atof(vlTokn);
			if (strcmp(nmTokn, "CASLOP") == 0) _CASLOP = atof(vlTokn);
			if (strcmp(nmTokn, "LVSLOP") == 0) _LVSLOP = atof(vlTokn);
			if (strcmp(nmTokn, "FDICPR") == 0) _FDICPR = atof(vlTokn);
			if (strcmp(nmTokn, "EQUFRC") == 0) _EQUFRC = atof(vlTokn);
			if (strcmp(nmTokn, "BRKPRC") == 0) _BRKPRC = atof(vlTokn);
			if (strcmp(nmTokn, "MXBAIL") == 0) _MXBAIL = atof(vlTokn);
			if (strcmp(nmTokn, "BKRCST") == 0) _BKRCST = atof(vlTokn);
			if (strcmp(nmTokn, "SETUPC") == 0) _SETUPC = atof(vlTokn);
			if (strcmp(nmTokn, "PSLOPE") == 0) _PSLOPE = atof(vlTokn);
		}
		fclose(flStrm);
		require(_NGOODS%_NBANKS==0, "Number of goods must be divisible by number of bank sectors");

		if (!_DISEQU) {
			_SHOCKS = 0;
			_TRNOVR = 0;
			_RNDMRK = 0;
			_CNTPRD = 1;
		}
		if (!_SHOCKS) {
			_GAPTRG = 0.0;
			_RRTTRG = _TMPRFY;
		}
	}
}

Parameters::~Parameters() {
	if (_OPMODE) {
		delete[] _OPMODE; _OPMODE = NULL;
	}
	if (_EXPVAR) {
		delete[] _EXPVAR; _EXPVAR = NULL;
	}
	if (_SNSVAR) {
		delete[] _SNSVAR; _SNSVAR = NULL;
	}
  if (_IMPULS) {
		delete[] _IMPULS; _IMPULS = NULL;
	}
}

ostream& operator <<(ostream &stream, Parameters* object) {
	stream << "OPMODE = " << object->_OPMODE << "\n";
	stream << "EXPVAR = " << object->_EXPVAR << "\n";
	stream << "SNSVAR = " << object->_SNSVAR << "\n";
	stream << "IMPULS = " << object->_IMPULS << "\n";
	stream << "NTHRDS = " << object->_NTHRDS << "\n";
	stream << "NSRUNS = " << object->_NSRUNS << "\n";
	stream << "NWEEKS = " << object->_NWEEKS << "\n";
	stream << "ADJLAG = " << object->_ADJLAG << "\n";
	stream << "NGOODS = " << object->_NGOODS << "\n";
	stream << "WKSPMN = " << object->_WKSPMN << "\n";
	stream << "WKSPYR = " << object->_WKSPYR << "\n";
	stream << "RNSEED = " << object->_RNSEED << "\n";
	stream << "RNDMRK = " << object->_RNDMRK << "\n";
	stream << "TRNOVR = " << object->_TRNOVR << "\n";
	stream << "FLEXWG = " << object->_FLEXWG << "\n";
	stream << "ZEROLB = " << object->_ZEROLB << "\n";
	stream << "SHOCKS = " << object->_SHOCKS << "\n";
	stream << "DISEQU = " << object->_DISEQU << "\n";
	stream << "\n";

	stream << "EXPINI = " << object->_EXPINI << "\n";
	stream << "EXPINC = " << object->_EXPINC << "\n";
	stream << "SNSDEV = " << object->_SNSDEV << "\n";

	stream << "MAXSLS = " << object->_MAXSLS << "\n";
	stream << "CNTPRD = " << object->_CNTPRD << "\n";
	stream << "MARKUP = " << object->_MARKUP << "\n";
	stream << "FAILRT = " << object->_FAILRT << "\n";
	stream << "FXDCST = " << object->_FXDCST << "\n";
	stream << "INVSPD = " << object->_INVSPD << "\n";
	stream << "WAGSPD = " << object->_WAGSPD << "\n";
	stream << "\n";

	stream << "DMDELS = " << object->_DMDELS << "\n";
	stream << "INCSPD = " << object->_INCSPD << "\n";
	stream << "TMPRFY = " << object->_TMPRFY << "\n";
	stream << "INFREQ = " << object->_INFREQ << "\n";
	stream << "BRKFRC = " << object->_BRKFRC << "\n";
	stream << "TRIGGR = " << object->_TRIGGR << "\n";
	stream << "PDELTA = " << object->_PDELTA << "\n";
	stream << "\n";

	stream << "BNDINC = " << object->_BNDINC << "\n";
	stream << "FSCSPD = " << object->_FSCSPD << "\n";
	stream << "INFCOR = " << object->_INFCOR << "\n";
	stream << "GAPCOR = " << object->_GAPCOR << "\n";
	stream << "INFTYR = " << object->_INFTYR << "\n";
	stream << "GAPTYR = " << object->_GAPTYR << "\n";
	stream << "GAPTRG = " << object->_GAPTRG << "\n";
	stream << "RRTTRG = " << object->_RRTTRG << "\n";
	stream << "PITRGY = " << object->_PITRGY << "\n";
	stream << "BKRCST = " << object->_BKRCST << "\n";
	stream << "\n";

	return stream;
}

int Parameters::adj_EXPVAR() {

#define expFix(Xvar) {                  \
	Xvar = _EXPINI + _THREAD * _EXPINC; \
	return 1;                           \
}

	if (strcmp(_EXPVAR, "PITRGY") == 0) expFix(_PITRGY);
	if (strcmp(_EXPVAR, "CARATE") == 0) expFix(_CARATE);
	if (strcmp(_EXPVAR, "SPREAD") == 0) expFix(_SPREAD);
	if (strcmp(_EXPVAR, "DISCPR") == 0) expFix(_DISCPR);
	if (strcmp(_EXPVAR, "LVALRT") == 0) expFix(_LVALRT);
	if (strcmp(_EXPVAR, "FSDISC") == 0) expFix(_FSDISC);
	if (strcmp(_EXPVAR, "CASLOP") == 0) expFix(_CASLOP);
	if (strcmp(_EXPVAR, "LVSLOP") == 0) expFix(_LVSLOP);
	if (strcmp(_EXPVAR, "FDICPR") == 0) expFix(_FDICPR);
	if (strcmp(_EXPVAR, "EQUFRC") == 0) expFix(_EQUFRC);
	if (strcmp(_EXPVAR, "MXBAIL") == 0) expFix(_MXBAIL);
	if (strcmp(_EXPVAR, "PSLOPE") == 0) expFix(_PSLOPE);
  if (strcmp(_EXPVAR, "GAPTYR") == 0) expFix(_GAPTYR);
  if (strcmp(_EXPVAR, "INIPRD") == 0) expFix(_INIPRD);
  if (strcmp(_EXPVAR, "NCROOK") == 0) expFix(_NCROOK);

	if (strcmp(_EXPVAR, "NBANKS") == 0) {
		vector<int> dvsors;
		for (int i = 1; i <= _NGOODS / 2; i++) {
			if (_NGOODS % i == 0)
				dvsors.push_back(i);
		}
		dvsors.push_back(_NGOODS);

		if (_NTHRDS > (int)dvsors.size()) {
			cout << "The number of threads must be less than or equal to "
				 << dvsors.size() << endl;
			return 0;
		}
		else {
			_NBANKS = dvsors[_THREAD];
			return 1;
		}
	}

	cout << "The code for running experiments with " << _EXPVAR
		 << " has not been implemented." << endl
		 << "See adj_EXPVAR() and get_expVal() in Parameters.cpp." << endl;

	return 0;
}

int Parameters::adj_SNSVAR(int snsDir) {

	static double snsVal;

	if (strcmp(_SNSVAR, "MAXSLS") == 0) {
		if(snsDir == -1)
			snsVal = _MAXSLS;
		_MAXSLS = (int) floor ((1 + snsDir * _SNSDEV) * snsVal + 0.5);
		return 1;
	}

#define snsFix(Xvar) {                      \
	if(snsDir == -1)                        \
		snsVal = Xvar;                      \
	Xvar = (1 + snsDir * _SNSDEV) * snsVal; \
	return 1;                               \
}

	if (strcmp(_SNSVAR, "PDELTA") == 0) snsFix(_PDELTA);
	if (strcmp(_SNSVAR, "WAGSPD") == 0) snsFix(_WAGSPD);
	if (strcmp(_SNSVAR, "FAILRT") == 0) snsFix(_FAILRT);
	if (strcmp(_SNSVAR, "BRKFRC") == 0) snsFix(_BRKFRC);
	if (strcmp(_SNSVAR, "INFREQ") == 0) snsFix(_INFREQ);
	if (strcmp(_SNSVAR, "TRIGGR") == 0) snsFix(_TRIGGR);
	if (strcmp(_SNSVAR, "LTRIGG") == 0) snsFix(_LTRIGG);
	if (strcmp(_SNSVAR, "FXDCST") == 0) snsFix(_FXDCST);
	if (strcmp(_SNSVAR, "DMDELS") == 0) snsFix(_DMDELS);
	if (strcmp(_SNSVAR, "INCSPD") == 0) snsFix(_INCSPD);
	if (strcmp(_SNSVAR, "INVSPD") == 0) snsFix(_INVSPD);
	if (strcmp(_SNSVAR, "BNDINC") == 0) snsFix(_BNDINC);
	if (strcmp(_SNSVAR, "FSCSPD") == 0) snsFix(_FSCSPD);
	if (strcmp(_SNSVAR, "INFTYR") == 0) snsFix(_INFTYR);
	if (strcmp(_SNSVAR, "GAPTYR") == 0) snsFix(_GAPTYR);

	return 0;
}

void Parameters::adj_endPrm() {
	FILE* flStrm;
	char* nmTokn;
	char* vlTokn;
	char* contxt;
	char  flLine[256];

	char flName[50] = "";
	char cvtBuf[50] = "";
	int  cvtDec = -1;
	int  cvtSgn = -1;

	_fcvt_s (cvtBuf, _PITRGY, 2, &cvtDec, &cvtSgn);
    strcat_s(flName, "EPARAM_PITRGY_");
    strcat_s(flName, cvtBuf);
    strcat_s(flName, ".ini");

	if (fopen_s(&flStrm, flName, "r") == 0) {    
		while (fgets(flLine, 256, flStrm) != NULL) {
			nmTokn = strtok_s(flLine, " ="  , &contxt);
			vlTokn = strtok_s(NULL  , " =\n", &contxt);

			if (strcmp(nmTokn, "RRTTRG") == 0 && _USEEND) _RRTTRG = atof(vlTokn);
			if (strcmp(nmTokn, "GAPTRG") == 0 && _USEEND) _GAPTRG = atof(vlTokn);
			if (strcmp(nmTokn, "MARKUP") == 0 && _USEEND) _MARKUP = atof(vlTokn);
			if (strcmp(nmTokn, "GAPCOR") == 0 && _USECOR) _GAPCOR = atof(vlTokn);
			if (strcmp(nmTokn, "INFCOR") == 0 && _USECOR) _INFCOR = atof(vlTokn);
			if (strcmp(nmTokn, "STDMKP") == 0 && _USEEND) _STDMKP = atof(vlTokn);
		}
		fclose(flStrm);
	}
	else {
		fprintf(stderr, "Could not open file %s\n", flName);
		exit(1);
	}
}

void Parameters::adj_endPrm(double PITRGY) {
	double PITRGL = 1.0 + floor((PITRGY - 1.0)*100)/100;
	double PITRGH = 1.0 + ceil ((PITRGY - 1.0)*100)/100;

	char fNameL[50] = "";
	char fNameH[50] = "";
	char cvBufL[50] = "";
	char cvBufH[50] = "";

	int cvDecL = -1;
	int cvDecH = -1;
	int cvSgnL = -1;
	int cvSgnH = -1;

	_fcvt_s (cvBufL, PITRGL, 2, &cvDecL, &cvSgnL);
	strcat_s(fNameL, "EPARAM_PITRGY_");
	strcat_s(fNameL, cvBufL);
	strcat_s(fNameL, ".ini");

	_fcvt_s (cvBufH, PITRGH, 2, &cvDecH, &cvSgnH);
	strcat_s(fNameH, "EPARAM_PITRGY_");
	strcat_s(fNameH, cvBufH);
	strcat_s(fNameH, ".ini");

	double RTTRGL = 0; double RTTRGH = 0;
	double GPTRGL = 0; double GPTRGH = 0;
	double MRKUPL = 0; double MRKUPH = 0;
	double GPCORL = 0; double GPCORH = 0;
	double PICORL = 0; double PICORH = 0;
	double SDMKPL = 0; double SDMKPH = 0;

	FILE* flStrm;
	char* nmTokn;
	char* vlTokn;
	char* contxt;
	char  flLine[256];

	if (fopen_s(&flStrm, fNameL, "r") == 0) {    
		while (fgets(flLine, 256, flStrm) != NULL) {
			nmTokn = strtok_s(flLine, " ="  , &contxt);
			vlTokn = strtok_s(NULL  , " =\n", &contxt);

			if (strcmp(nmTokn, "RRTTRG") == 0) RTTRGL = atof(vlTokn);
			if (strcmp(nmTokn, "GAPTRG") == 0) GPTRGL = atof(vlTokn);
			if (strcmp(nmTokn, "MARKUP") == 0) MRKUPL = atof(vlTokn);
			if (strcmp(nmTokn, "GAPCOR") == 0) GPCORL = atof(vlTokn);
			if (strcmp(nmTokn, "INFCOR") == 0) PICORL = atof(vlTokn);
			if (strcmp(nmTokn, "STDMKP") == 0) SDMKPL = atof(vlTokn);
		}
		fclose(flStrm);
	}
	else {
		fprintf(stderr, "Could not open file %s\n", fNameL);
		exit(1);
	}

	if (fopen_s(&flStrm, fNameH, "r") == 0) {    
		while (fgets(flLine, 256, flStrm) != NULL) {
			nmTokn = strtok_s(flLine, " ="  , &contxt);
			vlTokn = strtok_s(NULL  , " =\n", &contxt);

			if (strcmp(nmTokn, "RRTTRG") == 0) RTTRGH = atof(vlTokn);
			if (strcmp(nmTokn, "GAPTRG") == 0) GPTRGH = atof(vlTokn);
			if (strcmp(nmTokn, "MARKUP") == 0) MRKUPH = atof(vlTokn);
			if (strcmp(nmTokn, "GAPCOR") == 0) GPCORH = atof(vlTokn);
			if (strcmp(nmTokn, "INFCOR") == 0) PICORH = atof(vlTokn);
			if (strcmp(nmTokn, "STDMKP") == 0) SDMKPH = atof(vlTokn);
		}
		fclose(flStrm);
	}
	else {
		fprintf(stderr, "Could not open file %s\n", fNameH);
		exit(1);
	}

	if (_USEEND) {
		if (PITRGH > PITRGL) {
			_RRTTRG = RTTRGL + (PITRGY - PITRGL) * (RTTRGH - RTTRGL) / (PITRGH - PITRGL);
			_GAPTRG = GPTRGL + (PITRGY - PITRGL) * (GPTRGH - GPTRGL) / (PITRGH - PITRGL);
			_MARKUP = MRKUPL + (PITRGY - PITRGL) * (MRKUPH - MRKUPL) / (PITRGH - PITRGL);
			_STDMKP = SDMKPL + (PITRGY - PITRGL) * (SDMKPH - SDMKPL) / (PITRGH - PITRGL);
		}
		else {
			_RRTTRG = RTTRGL;
			_GAPTRG = GPTRGL;
			_MARKUP = MRKUPL;
			_STDMKP = SDMKPL;
		}
	}

	if (_USECOR) {
		if (PITRGH > PITRGL) {
			_GAPCOR = GPCORL + (PITRGY - PITRGL) * (GPCORH - GPCORL) / (PITRGH - PITRGL);
			_INFCOR = PICORL + (PITRGY - PITRGL) * (PICORH - PICORL) / (PITRGH - PITRGL);
		}
		else {
			_GAPCOR = GPCORL;
			_INFCOR = PICORL;
		}
	}
}

double Parameters::get_expVal() {	
	if (strcmp(_EXPVAR, "PITRGY") == 0) return _PITRGY;
	if (strcmp(_EXPVAR, "CARATE") == 0) return _CARATE;
	if (strcmp(_EXPVAR, "SPREAD") == 0) return _SPREAD;
	if (strcmp(_EXPVAR, "DISCPR") == 0) return _DISCPR;
	if (strcmp(_EXPVAR, "LVALRT") == 0) return _LVALRT;
	if (strcmp(_EXPVAR, "FSDISC") == 0) return _FSDISC;
	if (strcmp(_EXPVAR, "NBANKS") == 0) return _NBANKS;
	if (strcmp(_EXPVAR, "CASLOP") == 0) return _CASLOP;
	if (strcmp(_EXPVAR, "LVSLOP") == 0) return _LVSLOP;
	if (strcmp(_EXPVAR, "FDICPR") == 0) return _FDICPR;
	if (strcmp(_EXPVAR, "EQUFRC") == 0) return _EQUFRC;
	if (strcmp(_EXPVAR, "MXBAIL") == 0) return _MXBAIL;
	if (strcmp(_EXPVAR, "PSLOPE") == 0) return _PSLOPE;
	if (strcmp(_EXPVAR, "GAPTYR") == 0) return _GAPTYR;
	if (strcmp(_EXPVAR, "INIPRD") == 0) return _INIPRD;
	if (strcmp(_EXPVAR, "NCROOK") == 0) return _NCROOK;

	return -1;
}
