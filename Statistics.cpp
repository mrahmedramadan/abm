/***********************************************************************************************
 * Statistics.cpp
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised July 25 2016
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

#include "Economy.h"
#include "Shop.h"
#include "Statistics.h"
#include "Agent.h"
#include "Bank.h"

Statistics::Statistics(Economy* abEcon) {
	_abEcon = abEcon;

	int NSRUNS = _abEcon->_NSRUNS;
	int NGOODS = _abEcon->_NGOODS;
	int NYEARS = _abEcon->_NYEARS;
	int NMNTHS = _abEcon->_NMNTHS;
	int ADJLAG = _abEcon->_ADJLAG;

	_AVGYRS = (double) (NYEARS-ADJLAG-1)/2.0;
	_MOMENT = 0.0;

	for (int t = 0; t < NYEARS - ADJLAG; t++)
		_MOMENT += (t-_AVGYRS)*(t-_AVGYRS);

	_lastCn = NGOODS;
	_avMkup = 0.0;
	_wtMkup = 0.0;
	_stdMkp = 0.0;
	_wtPrft = 0.0;
	_wtRAge = 0.0;
	_tlShop = 0.0;
	_tlLfSp = 0;
	_tlEnEx = 0;
	_nCrash = 0;

	_lmShFl = 0; _lmBkFl = 0; _lmBkTr = 0;

	_tlUnEp = 0.0; _tlUnDr = 0.0;
	_tlLfEp = 0.0; _tlLfDr = 0.0;
	_tlExEp = 0.0; _tlExDr = 0.0;
	_tlQtEp = 0.0; _tlQtDr = 0.0;
	_tlBkEp = 0.0; _tlBkDr = 0.0;

	_yeBnds = new double[NYEARS];
	_yeLoan = new double[NYEARS];
	_yeBkrp = new double[NYEARS];
	_yeFCon = new double[NYEARS];
	_yeIncm = new double[NYEARS];
	_yeNInc = new double[NYEARS];
	_yeLnGp = new double[NYEARS];
	_yeCons = new double[NYEARS];
	_yeInvn = new double[NYEARS];
	_yeDInv = new double[NYEARS];
	_yeEmpl = new double[NYEARS];
	_yeUnRt = new double[NYEARS];
	_yeVcRt = new double[NYEARS];
	_yeRlWg = new double[NYEARS];
	_yePIdx = new double[NYEARS];
	_yeInfl = new double[NYEARS];
	_yeCCRR = new double[NYEARS];
	_yeCCNR = new double[NYEARS];
	_yeTxRt = new double[NYEARS];

	_CRLnGp = new double[NYEARS];
	_CRFail = new double[NYEARS];
	_CRAvMk = new double[NYEARS];
	_CRWtMk = new double[NYEARS];
	_CRStMk = new double[NYEARS];
	_CRWtPf = new double[NYEARS];
	_CRRAge = new double[NYEARS];
	_CRDbtR = new double[NYEARS];
	_CRTxRt = new double[NYEARS];
	_CRCCPi = new double[NYEARS];
	_CRCCRR = new double[NYEARS];
	_CRGpTg = new double[NYEARS];
	_CRRtTg = new double[NYEARS];
	_CRGpRm = new double[NYEARS];
	_CRPiRm = new double[NYEARS];
	_CRGdPt = new double[NYEARS];

	for (int i = 0; i < NYEARS; i++) {
		_yeBnds[i] = 0.0;
		_yeLoan[i] = 0.0;
		_yeBkrp[i] = 0.0;
		_yeFCon[i] = 0.0;
		_yeIncm[i] = 0.0;
		_yeNInc[i] = 0.0;
		_yeLnGp[i] = 0.0;
		_yeCons[i] = 0.0;
		_yeInvn[i] = 0.0;
		_yeDInv[i] = 0.0;
		_yeEmpl[i] = 0.0;
		_yeUnRt[i] = 0.0;
		_yeVcRt[i] = 0.0;
		_yeRlWg[i] = 0.0;
		_yePIdx[i] = 0.0;
		_yeInfl[i] = 0.0;
		_yeCCRR[i] = 0.0;
		_yeCCNR[i] = 0.0;
		_yeTxRt[i] = 0.0;

		_CRLnGp[i] = 0.0;
		_CRFail[i] = 0.0;
		_CRAvMk[i] = 0.0;
		_CRWtMk[i] = 0.0;
		_CRStMk[i] = 0.0;
		_CRWtPf[i] = 0.0;
		_CRRAge[i] = 0.0;
		_CRDbtR[i] = 0.0;
		_CRTxRt[i] = 0.0;
		_CRCCPi[i] = 0.0;
		_CRCCRR[i] = 0.0;
		_CRGpTg[i] = 0.0;
		_CRRtTg[i] = 0.0;
		_CRGpRm[i] = 0.0;
		_CRPiRm[i] = 0.0;
		_CRGdPt[i] = 0.0;
	}

	_CYIncm = new double[NSRUNS];
	_CYLnGp = new double[NSRUNS];
	_CYFail = new double[NSRUNS];
	_CYWtFl = new double[NSRUNS];
	_CYHpls = new double[NSRUNS];
	_CYZbRt = new double[NSRUNS];
	_CYInfl = new double[NSRUNS];
	_CYCCPi = new double[NSRUNS];
	_CYCCRR = new double[NSRUNS];
	_CYExit = new double[NSRUNS];
	_CYBkFl = new double[NSRUNS];
	_CYBkTr = new double[NSRUNS];

	_vncyRt = new double[NSRUNS];
	_entrFl = new double[NSRUNS];
	_entrRt = new double[NSRUNS];
	_nwJCRt = new double[NSRUNS];
	_loan2Y = new double[NSRUNS];
	_rlLoan = new double[NSRUNS];
	_enRlLn = new double[NSRUNS];
	_lnVlRt = new double[NSRUNS];
	_bkrpRt = new double[NSRUNS];
	_fConRt = new double[NSRUNS];
	_jobMch = new double[NSRUNS];
	_job2Jb = new double[NSRUNS];
	_strMch = new double[NSRUNS];
	_str2St = new double[NSRUNS];
	_lifeEx = new double[NSRUNS];
	_shPrWk = new double[NSRUNS];
	_failHz = new double[NSRUNS];
	_hplsHz = new double[NSRUNS];
	_exitRt = new double[NSRUNS];
	_jbLsRt = new double[NSRUNS];
	_lsLfRt = new double[NSRUNS];
	_lsQtRt = new double[NSRUNS];
	_lsExRt = new double[NSRUNS];
	_uempDr = new double[NSRUNS];
	_unLfDr = new double[NSRUNS];
	_unQtDr = new double[NSRUNS];
	_unExDr = new double[NSRUNS];
	_unBkDr = new double[NSRUNS];
	_uempRt = new double[NSRUNS];
	_unLfRt = new double[NSRUNS];
	_unQtRt = new double[NSRUNS];
	_unExRt = new double[NSRUNS];
	_unBkRt = new double[NSRUNS];
	_prftRt = new double[NSRUNS];
	_pFreqy = new double[NSRUNS];
	_prcDsp = new double[NSRUNS];
	_wprDsp = new double[NSRUNS];
	_pftDsp = new double[NSRUNS];
	_salDsp = new double[NSRUNS];
	_wpfDsp = new double[NSRUNS];
	_gapVol = new double[NSRUNS];
	_gapCor = new double[NSRUNS];
	_infVol = new double[NSRUNS];
	_infCor = new double[NSRUNS];
    _YLCorD = new double[NSRUNS];
	_erWtMk = new double[NSRUNS];

	_badRns = new bool  [NSRUNS];

	_pftMrk = new double*[NSRUNS];
	_RYLnGp = new double*[NSRUNS];
	_RYFail = new double*[NSRUNS];
	_RYAvMk = new double*[NSRUNS];
	_RYWtMk = new double*[NSRUNS];
	_RYStMk = new double*[NSRUNS];
	_RYWtPf = new double*[NSRUNS];
	_RYRAge = new double*[NSRUNS];
	_RYDbtR = new double*[NSRUNS];
	_RYTxRt = new double*[NSRUNS];
	_RYCCRR = new double*[NSRUNS];
	_RYCCPi = new double*[NSRUNS];
	_RYGpTg = new double*[NSRUNS];
	_RYRtTg = new double*[NSRUNS];
	_RYGpRm = new double*[NSRUNS];
	_RYPiRm = new double*[NSRUNS];
	_RYGdPt = new double*[NSRUNS];
	_RYBkFl = new double*[NSRUNS];
	_RYBkTr = new double*[NSRUNS];

  if(strcmp(_abEcon->_OPMODE, "impulse")==0) {
	  _RMLnGp = new double*[NSRUNS];
	  _RMFail = new double*[NSRUNS];
	  _RMBkFl = new double*[NSRUNS];
	  _RMBkTr = new double*[NSRUNS];
	  _RMEntr = new double*[NSRUNS];
  }
  else {
	  _RMLnGp = NULL;
	  _RMFail = NULL;
	  _RMBkFl = NULL;
	  _RMBkTr = NULL;
	  _RMEntr = NULL;
  }

  for (int i = 0; i < NSRUNS; i++) {
		_CYIncm[i] = 0.0;
		_CYLnGp[i] = 0.0;
		_CYFail[i] = 0.0;
		_CYWtFl[i] = 0.0;
		_CYHpls[i] = 0.0;
		_CYZbRt[i] = 0.0;
		_CYInfl[i] = 0.0;
		_CYCCPi[i] = 0.0;
		_CYCCRR[i] = 0.0;
		_CYExit[i] = 0.0;
		_CYBkFl[i] = 0.0;
		_CYBkTr[i] = 0.0;

		_vncyRt[i] = 0.0;
		_entrFl[i] = 0.0;
		_entrRt[i] = 0.0;
		_nwJCRt[i] = 0.0;
		_loan2Y[i] = 0.0;
		_rlLoan[i] = 0.0;
		_enRlLn[i] = 0.0;
		_lnVlRt[i] = 0.0;
		_bkrpRt[i] = 0.0;
		_fConRt[i] = 0.0;
		_jobMch[i] = 0.0;
		_job2Jb[i] = 0.0;
		_strMch[i] = 0.0;
		_str2St[i] = 0.0;
		_lifeEx[i] = 0.0;
		_shPrWk[i] = 0.0;
		_failHz[i] = 0.0;
		_hplsHz[i] = 0.0;
		_exitRt[i] = 0.0;
		_jbLsRt[i] = 0.0;
		_lsLfRt[i] = 0.0;
		_lsQtRt[i] = 0.0;
		_lsExRt[i] = 0.0;
		_uempDr[i] = 0.0;
		_unLfDr[i] = 0.0;
		_unQtDr[i] = 0.0;
		_unExDr[i] = 0.0;
		_unBkDr[i] = 0.0;
		_uempRt[i] = 0.0;
		_unLfRt[i] = 0.0;
		_unQtRt[i] = 0.0;
		_unExRt[i] = 0.0;
		_unBkRt[i] = 0.0;
		_prftRt[i] = 0.0;
		_pFreqy[i] = 0.0;
		_prcDsp[i] = 0.0;
		_wprDsp[i] = 0.0;
		_pftDsp[i] = 0.0;
		_salDsp[i] = 0.0;
		_wpfDsp[i] = 0.0;
		_gapVol[i] = 0.0;
		_gapCor[i] = 0.0;
		_infVol[i] = 0.0;
        _YLCorD[i] = 0.0;
		_infCor[i] = 0.0;
		_erWtMk[i] = 0.0;

		_badRns[i] = false;

		_pftMrk[i] = new double[20];
		for (int j = 0; j < 20; j++)
			_pftMrk[i][j] = 0.0;

		_RYLnGp[i] = new double[NYEARS];
		_RYFail[i] = new double[NYEARS];
		_RYAvMk[i] = new double[NYEARS];
		_RYWtMk[i] = new double[NYEARS];
		_RYStMk[i] = new double[NYEARS];
		_RYWtPf[i] = new double[NYEARS];
		_RYRAge[i] = new double[NYEARS];
		_RYDbtR[i] = new double[NYEARS];
		_RYTxRt[i] = new double[NYEARS];
		_RYCCRR[i] = new double[NYEARS];
		_RYCCPi[i] = new double[NYEARS];
		_RYGpTg[i] = new double[NYEARS];
		_RYRtTg[i] = new double[NYEARS];
		_RYGpRm[i] = new double[NYEARS];
		_RYPiRm[i] = new double[NYEARS];
		_RYGdPt[i] = new double[NYEARS];
		_RYBkFl[i] = new double[NYEARS];
		_RYBkTr[i] = new double[NYEARS];

		for (int j = 0; j < NYEARS; j++) {
			_RYLnGp[i][j] = 0.0;
			_RYFail[i][j] = 0.0;
			_RYAvMk[i][j] = 0.0;
			_RYWtMk[i][j] = 0.0;
			_RYStMk[i][j] = 0.0;
			_RYWtPf[i][j] = 0.0;
			_RYRAge[i][j] = 0.0;
			_RYDbtR[i][j] = 0.0;
			_RYTxRt[i][j] = 0.0;
			_RYCCRR[i][j] = 0.0;
			_RYCCPi[i][j] = 0.0;
			_RYGpTg[i][j] = 0.0;
			_RYRtTg[i][j] = 0.0;
			_RYGpRm[i][j] = 0.0;
			_RYPiRm[i][j] = 0.0;
			_RYGdPt[i][j] = 0.0;
			_RYBkFl[i][j] = 0.0;
			_RYBkTr[i][j] = 0.0;
		}

    if(strcmp(_abEcon->_OPMODE, "impulse")==0) {
		  _RMLnGp[i] = new double[NMNTHS];
		  _RMFail[i] = new double[NMNTHS];
		  _RMBkFl[i] = new double[NMNTHS];
		  _RMBkTr[i] = new double[NMNTHS];
		  _RMEntr[i] = new double[NMNTHS];

		  for (int j = 0; j < NMNTHS; j++) {
			  _RMLnGp[i][j] = 0.0;
			  _RMFail[i][j] = 0.0;
			  _RMBkFl[i][j] = 0.0;
			  _RMBkTr[i][j] = 0.0;
			  _RMEntr[i][j] = 0.0;
		  }
    }
	}

	_avLnGp = 0.0; _avGpVl = 0.0; _avPiRt = 0.0; _avPiVl = 0.0;
	_avCCPi = 0.0; _avRlRt = 0.0; _avPrDs = 0.0; _avWPrD = 0.0;
	_avPfDs = 0.0; _avSlDs = 0.0; _avWPfD = 0.0; _avNmFl = 0.0;
	_avRlVl = 0.0; _avMkup = 0.0; _avGpCr = 0.0; _avZbRt = 0.0;
	_avPiCr = 0.0; _avPfRt = 0.0; _avPrFq = 0.0; _avHpls = 0.0;
	_avVnRt = 0.0; _avEnFl = 0.0; _avEnRt = 0.0; _avNJCR = 0.0;
	_avBkrp = 0.0; _avFCon = 0.0; _avJbMt = 0.0; _avWtFl = 0.0;
	_avStMt = 0.0; _avLfEx = 0.0; _avExRt = 0.0; _avJbLs = 0.0; 
	_avLsLf = 0.0; _avLsQt = 0.0; _avLsEx = 0.0; _avJ2Jb = 0.0;
	_avS2St = 0.0; _avWkSh = 0.0; _avFlHz = 0.0; _avHpHz = 0.0;
	_avWtMk = 0.0; _avBkFl = 0.0; _avBkTr = 0.0; _avLn2Y = 0.0;
	_avRlLn = 0.0; _avEnLn = 0.0; _avLVRt = 0.0;

	_avUnRt = 0.0; _avUnLf = 0.0;
	_avUnQt = 0.0; _avUnEx = 0.0;
	_avUnBk = 0.0;
	_avUnDr = 0.0; _avLfDr = 0.0;
	_avQtDr = 0.0; _avExDr = 0.0;
	_avBkDr = 0.0;

	_mdLnGp = 0.0; _mdGpVl = 0.0;
	_mdCCPi = 0.0; _mdPiVl = 0.0;
	_mdRlRt = 0.0; _mdRlVl = 0.0;
	_mdJbLs = 0.0; _mdLsLf = 0.0;
	_mdLsQt = 0.0; _mdLsEx = 0.0;
	_mdExRt = 0.0; _mdFlHz = 0.0;
	_mdWtMk = 0.0; _mdPfRt = 0.0;
	_mdGpCr = 0.0; _mdPiCr = 0.0;
	_mdUnRt = 0.0; _mdPrFq = 0.0;
	_mdBkFl = 0.0; _mdBkTr = 0.0;
	_mdUnDr = 0.0; _mdWkSh = 0.0;
	_mdHpHz = 0.0; _mdEnFl = 0.0;
	_mdEnRt = 0.0; _mdNJCR = 0.0;
	_mdBkrp = 0.0; _mdFCon = 0.0;
	_mdZbRt = 0.0; _mdSlDs = 0.0;
	_mdLn2Y = 0.0; _mdRlLn = 0.0;
	_mdEnLn = 0.0; _mdLVRt = 0.0;

	_dcLnGp = new double[10];
	_dcUnRt = new double[10];
	_dcUnDr = new double[10];
	_dcGpVl = new double[10];
	_dcPiVl = new double[10];
	_dcCCPi = new double[10];
	_dcBkFl = new double[10];
	_dcBkTr = new double[10];
	_dcJbLs = new double[10];
	_dcLsLf = new double[10];
	_dcLsQt = new double[10];
	_dcLsEx = new double[10];
	_dcFlHz = new double[10];
	_dcWkSh = new double[10];
	_dcHpHz = new double[10];
	_dcEnFl = new double[10];
	_dcEnRt = new double[10];
	_dcNJCR = new double[10];
	_dcLn2Y = new double[10];
	_dcRlLn = new double[10];
	_dcEnLn = new double[10];
	_dcLVRt = new double[10];
	_dcBkrp = new double[10];
	_dcFCon = new double[10];
	_dcZbRt = new double[10];
	_dcSlDs = new double[10];
	
	for (int i = 0; i < 10; i++) {
		_dcLnGp[i] = 0.0;
		_dcUnRt[i] = 0.0;
		_dcUnDr[i] = 0.0;
		_dcGpVl[i] = 0.0;
		_dcPiVl[i] = 0.0;
		_dcCCPi[i] = 0.0;
		_dcBkFl[i] = 0.0;
		_dcBkTr[i] = 0.0;
		_dcJbLs[i] = 0.0;
		_dcLsLf[i] = 0.0;
		_dcLsQt[i] = 0.0;
		_dcLsEx[i] = 0.0;
		_dcFlHz[i] = 0.0;
		_dcWkSh[i] = 0.0;
		_dcHpHz[i] = 0.0;
		_dcEnFl[i] = 0.0;
		_dcEnRt[i] = 0.0;
		_dcNJCR[i] = 0.0;
		_dcLn2Y[i] = 0.0;
		_dcRlLn[i] = 0.0;
		_dcEnLn[i] = 0.0;
		_dcLVRt[i] = 0.0;
		_dcBkrp[i] = 0.0;
		_dcFCon[i] = 0.0;
		_dcZbRt[i] = 0.0;
		_dcSlDs[i] = 0.0;
	}

	_lwOpGp = 0.0;
	_upOpGp = 0.0;

	_avPfMk = new double[20];

	for (int i = 0; i < 20; i++)
		_avPfMk[i] = 0.0;
}

Statistics::~Statistics() {
	int NSRUNS = _abEcon->_NSRUNS;

	delete[] _yeBnds; _yeBnds = NULL;
	delete[] _yeLoan; _yeLoan = NULL;
	delete[] _yeBkrp; _yeBkrp = NULL;
	delete[] _yeFCon; _yeFCon = NULL;
	delete[] _yeIncm; _yeIncm = NULL;
	delete[] _yeNInc; _yeNInc = NULL;
	delete[] _yeLnGp; _yeLnGp = NULL;
	delete[] _yeCons; _yeCons = NULL;
	delete[] _yeInvn; _yeInvn = NULL;
	delete[] _yeDInv; _yeDInv = NULL;
	delete[] _yeEmpl; _yeEmpl = NULL;
	delete[] _yeUnRt; _yeUnRt = NULL;
	delete[] _yeVcRt; _yeVcRt = NULL;
	delete[] _yeRlWg; _yeRlWg = NULL;
	delete[] _yePIdx; _yePIdx = NULL;
	delete[] _yeInfl; _yeInfl = NULL;
	delete[] _yeCCRR; _yeCCRR = NULL;
	delete[] _yeCCNR; _yeCCNR = NULL;
	delete[] _yeTxRt; _yeTxRt = NULL;

	delete[] _CRLnGp; _CRLnGp = NULL;
	delete[] _CRFail; _CRFail = NULL;
	delete[] _CRAvMk; _CRAvMk = NULL;
	delete[] _CRWtMk; _CRWtMk = NULL;
	delete[] _CRStMk; _CRStMk = NULL;
	delete[] _CRWtPf; _CRWtPf = NULL;
	delete[] _CRRAge; _CRRAge = NULL;
	delete[] _CRDbtR; _CRDbtR = NULL;
	delete[] _CRTxRt; _CRTxRt = NULL;
	delete[] _CRCCPi; _CRCCPi = NULL;
	delete[] _CRCCRR; _CRCCRR = NULL;
	delete[] _CRGpTg; _CRGpTg = NULL;
	delete[] _CRRtTg; _CRRtTg = NULL;
	delete[] _CRGpRm; _CRGpRm = NULL;
	delete[] _CRPiRm; _CRPiRm = NULL;
	delete[] _CRGdPt; _CRGdPt = NULL;

	delete[] _CYIncm; _CYIncm = NULL;
	delete[] _CYLnGp; _CYLnGp = NULL;
	delete[] _CYFail; _CYFail = NULL;
	delete[] _CYWtFl; _CYWtFl = NULL;
	delete[] _CYHpls; _CYHpls = NULL;
	delete[] _CYZbRt; _CYZbRt = NULL;
	delete[] _CYInfl; _CYInfl = NULL;
	delete[] _CYCCPi; _CYCCPi = NULL;
	delete[] _CYCCRR; _CYCCRR = NULL;
	delete[] _CYExit; _CYExit = NULL;
	delete[] _CYBkFl; _CYBkFl = NULL;
	delete[] _CYBkTr; _CYBkTr = NULL;

	delete[] _vncyRt; _vncyRt = NULL;
	delete[] _entrFl; _entrFl = NULL;
	delete[] _entrRt; _entrRt = NULL;
	delete[] _nwJCRt; _nwJCRt = NULL;
	delete[] _loan2Y; _loan2Y = NULL;
	delete[] _rlLoan; _rlLoan = NULL;
	delete[] _enRlLn; _enRlLn = NULL;
	delete[] _lnVlRt; _lnVlRt = NULL;
	delete[] _bkrpRt; _bkrpRt = NULL;
	delete[] _fConRt; _fConRt = NULL;
	delete[] _jobMch; _jobMch = NULL;
	delete[] _job2Jb; _job2Jb = NULL;
	delete[] _strMch; _strMch = NULL;
	delete[] _str2St; _str2St = NULL;
	delete[] _lifeEx; _lifeEx = NULL;
	delete[] _shPrWk; _shPrWk = NULL;
	delete[] _failHz; _failHz = NULL;
	delete[] _hplsHz; _hplsHz = NULL;
	delete[] _exitRt; _exitRt = NULL;
	delete[] _jbLsRt; _jbLsRt = NULL;
	delete[] _lsLfRt; _lsLfRt = NULL;
	delete[] _lsQtRt; _lsQtRt = NULL;
	delete[] _lsExRt; _lsExRt = NULL;
	delete[] _uempDr; _uempDr = NULL;
	delete[] _unLfDr; _unLfDr = NULL;
	delete[] _unQtDr; _unQtDr = NULL;
	delete[] _unExDr; _unExDr = NULL;
	delete[] _unBkDr; _unBkDr = NULL;
	delete[] _uempRt; _uempRt = NULL;
	delete[] _unLfRt; _unLfRt = NULL;
	delete[] _unQtRt; _unQtRt = NULL;
	delete[] _unExRt; _unExRt = NULL;
	delete[] _unBkRt; _unBkRt = NULL;
	delete[] _prftRt; _prftRt = NULL;
	delete[] _pFreqy; _pFreqy = NULL;
	delete[] _prcDsp; _prcDsp = NULL;
	delete[] _wprDsp; _wprDsp = NULL;
	delete[] _pftDsp; _pftDsp = NULL;
	delete[] _salDsp; _salDsp = NULL;
	delete[] _wpfDsp; _wpfDsp = NULL;
	delete[] _gapVol; _gapVol = NULL;
	delete[] _gapCor; _gapCor = NULL;
    delete[] _YLCorD; _YLCorD = NULL;
	delete[] _infVol; _infVol = NULL;
	delete[] _infCor; _infCor = NULL;
	delete[] _erWtMk; _erWtMk = NULL;
	delete[] _badRns; _badRns = NULL;

	for (int i = 0; i < NSRUNS; i++) {
		delete[] _pftMrk[i]; _pftMrk[i] = NULL;
		delete[] _RYLnGp[i]; _RYLnGp[i] = NULL;
		delete[] _RYFail[i]; _RYFail[i] = NULL;
		delete[] _RYAvMk[i]; _RYAvMk[i] = NULL;
		delete[] _RYWtMk[i]; _RYWtMk[i] = NULL;
		delete[] _RYStMk[i]; _RYStMk[i] = NULL;
		delete[] _RYWtPf[i]; _RYWtPf[i] = NULL;
		delete[] _RYRAge[i]; _RYRAge[i] = NULL;
		delete[] _RYDbtR[i]; _RYDbtR[i] = NULL;
		delete[] _RYTxRt[i]; _RYTxRt[i] = NULL;
		delete[] _RYCCRR[i]; _RYCCRR[i] = NULL;
		delete[] _RYCCPi[i]; _RYCCPi[i] = NULL;
		delete[] _RYGpTg[i]; _RYGpTg[i] = NULL;
		delete[] _RYRtTg[i]; _RYRtTg[i] = NULL;
		delete[] _RYGpRm[i]; _RYGpRm[i] = NULL;
		delete[] _RYPiRm[i]; _RYPiRm[i] = NULL;
		delete[] _RYGdPt[i]; _RYGdPt[i] = NULL;
		delete[] _RYBkFl[i]; _RYBkFl[i] = NULL;
		delete[] _RYBkTr[i]; _RYBkTr[i] = NULL;
		if(strcmp(_abEcon->_OPMODE, "impulse")==0) {
			delete[] _RMLnGp[i]; _RMLnGp[i] = NULL;
			delete[] _RMFail[i]; _RMFail[i] = NULL;
			delete[] _RMBkFl[i]; _RMBkFl[i] = NULL;
			delete[] _RMBkTr[i]; _RMBkTr[i] = NULL;
			delete[] _RMEntr[i]; _RMEntr[i] = NULL;
		}
	}

	delete[] _pftMrk; _pftMrk = NULL;
	delete[] _avPfMk; _avPfMk = NULL;
	delete[] _RYLnGp; _RYLnGp = NULL;
	delete[] _RYFail; _RYFail = NULL;
	delete[] _RYAvMk; _RYAvMk = NULL;
	delete[] _RYWtMk; _RYWtMk = NULL;
	delete[] _RYStMk; _RYStMk = NULL;
	delete[] _RYWtPf; _RYWtPf = NULL;
	delete[] _RYRAge; _RYRAge = NULL;
	delete[] _RYDbtR; _RYDbtR = NULL;
	delete[] _RYTxRt; _RYTxRt = NULL;
	delete[] _RYCCRR; _RYCCRR = NULL;
	delete[] _RYCCPi; _RYCCPi = NULL;
	delete[] _RYGpTg; _RYGpTg = NULL;
	delete[] _RYRtTg; _RYRtTg = NULL;
	delete[] _RYGpRm; _RYGpRm = NULL;
	delete[] _RYPiRm; _RYPiRm = NULL;
	delete[] _RYGdPt; _RYGdPt = NULL;
	delete[] _RYBkFl; _RYBkFl = NULL;
	delete[] _RYBkTr; _RYBkTr = NULL;

	if(strcmp(_abEcon->_OPMODE, "impulse")==0) {
		delete[] _RMLnGp; _RMLnGp = NULL;
		delete[] _RMFail; _RMFail = NULL;
		delete[] _RMBkFl; _RMBkFl = NULL;
		delete[] _RMBkTr; _RMBkTr = NULL;
		delete[] _RMEntr; _RMEntr = NULL;
	}

	delete[] _dcLnGp; _dcLnGp = NULL;
	delete[] _dcUnRt; _dcUnRt = NULL;
	delete[] _dcUnDr; _dcUnDr = NULL;
	delete[] _dcGpVl; _dcGpVl = NULL;
	delete[] _dcPiVl; _dcPiVl = NULL;
	delete[] _dcCCPi; _dcCCPi = NULL;
	delete[] _dcBkFl; _dcBkFl = NULL;
	delete[] _dcBkTr; _dcBkTr = NULL;
	delete[] _dcJbLs; _dcJbLs = NULL;
	delete[] _dcLsLf; _dcLsLf = NULL;
	delete[] _dcLsQt; _dcLsQt = NULL;
	delete[] _dcLsEx; _dcLsEx = NULL;
	delete[] _dcFlHz; _dcFlHz = NULL;
	delete[] _dcWkSh; _dcWkSh = NULL;
	delete[] _dcHpHz; _dcHpHz = NULL;
	delete[] _dcEnFl; _dcEnFl = NULL;
	delete[] _dcEnRt; _dcEnRt = NULL;
	delete[] _dcNJCR; _dcNJCR = NULL;
	delete[] _dcLn2Y; _dcLn2Y = NULL;
	delete[] _dcRlLn; _dcRlLn = NULL;
	delete[] _dcEnLn; _dcEnLn = NULL;
	delete[] _dcLVRt; _dcLVRt = NULL;
	delete[] _dcBkrp; _dcBkrp = NULL;
	delete[] _dcFCon; _dcFCon = NULL;
	delete[] _dcZbRt; _dcZbRt = NULL;
	delete[] _dcSlDs; _dcSlDs = NULL;

	_abEcon = NULL;
}

double Statistics::get_avPfMk(int pcIndx) const { return _avPfMk[pcIndx]; }

void Statistics::col_wkStat() {
	int NAGNTS = _abEcon->_NAGNTS;
	int WKSPYR = _abEcon->_WKSPYR;
	int ADJLAG = _abEcon->_ADJLAG;
	int NSHOPS = _abEcon->_nShops;
	int wkCntr = _abEcon->_wkCntr;
	int rnCntr = _abEcon->_rnCntr;
	int nActSh = _abEcon->_nActSh;
	int enLfSp = _abEcon->_enLfSp;
	int nEntEx = _abEcon->_nEntEx;

	int nUn2Em = _abEcon->_nUn2Em;
	int nLf2Em = _abEcon->_nLf2Em;
	int nEx2Em = _abEcon->_nEx2Em;
	int nQt2Em = _abEcon->_nQt2Em;
	int nBk2Em = _abEcon->_nBk2Em;
	int unDurn = _abEcon->_unDurn;
	int lfDurn = _abEcon->_lfDurn;
	int exDurn = _abEcon->_exDurn;
	int qtDurn = _abEcon->_qtDurn;
	int bkDurn = _abEcon->_bkDurn;

	double entVac = _abEcon->_entVac;
	double nEnter = _abEcon->_nEnter;
	double entrFr = _abEcon->_entrFr;
	double nJbMch = _abEcon->_nJbMch;
	double nJb2Jb = _abEcon->_nJb2Jb;
	double nStMch = _abEcon->_nStMch;
	double nSt2St = _abEcon->_nSt2St;
	double entShp = _abEcon->_entShp;
	double extShp = _abEcon->_extShp;
	double jobLss = _abEcon->_jobLss;
	double jbLsLf = _abEcon->_jbLsLf;
	double jbLsQt = _abEcon->_jbLsQt;
	double jbLsEx = _abEcon->_jbLsEx;
	double nmIncm = _abEcon->_nmIncm;
	double cpIndx = _abEcon->_cpIndx;

	double aggSls = 0.0;
	double mrkAvg = 0.0;
	double mrkWav = 0.0;
	double varMkp = 0.0;
	double pftAvg = 0.0;
	double pftWav = 0.0;
	double pftVar = 0.0;
	double pftWvr = 0.0;
	double prcAvg = 0.0;
	double prcWav = 0.0;
	double prcVar = 0.0;
	double prcWvr = 0.0;
	double ragWav = 0.0;
	double salVar = 0.0;

	_unempl = 0.0;
	_unLoff = 0.0;
	_unExit = 0.0;
	_unQuit = 0.0;
	_unBrok = 0.0;
	_woEmpl = 0.0;
	_nwEmpl = 0.0;

	for(int i = 0; i < NAGNTS; i++) {   // tally this week's unemployment
		if (_abEcon->_agList[i]->get_spEffc() == 0.0) {
			_unempl++;
			_unLoff += (_abEcon->_agList[i]->get_unType() == Layoff);
			_unExit += (_abEcon->_agList[i]->get_unType() == Exit);
			_unQuit += (_abEcon->_agList[i]->get_unType() == Quit);
			_unBrok += (_abEcon->_agList[i]->get_unType() == Broke);
		}
		if (_abEcon->_agList[i]->get_spShop() == NULL) _woEmpl ++;
		else if (_abEcon->_agList[i]->get_spShop()->get_entrWk() > wkCntr - WKSPYR) _nwEmpl ++;
	}

	double nwJCFr = _nwEmpl / (NAGNTS - _woEmpl);	// new-shop jobs as fraction of total
	double mchEmp = NAGNTS - _woEmpl + jobLss;		// aggregate employment after matching
	double bkrpHz = _abEcon->_bkrpFr * WKSPYR;		// bankruptcy rate (per annum)
	double fConPr = _abEcon->_fConFr;				// financial constraint probability
	double entrLn = _abEcon->_entrLn;				// book value of total commercial loans given to entrants
	double sLoans = 0.0;							// book value of total commercial loans

	for (int i = 0; i < _abEcon->_NBANKS; i++)
		sLoans += _abEcon->get_bkList(i)->get_sLoans();

	for (int i = 0; i < NSHOPS; i++) {
		Shop* sh = _abEcon->_shList[i];
		if (sh->is_active())
			aggSls += sh->get_aSales();
	}
	for (int i = 0; i < NSHOPS; i++) {
		Shop* sh = _abEcon->_shList[i];
		if (sh->is_active()) {
			mrkAvg += sh->get_markup() / nActSh;
			mrkWav += sh->get_markup() * sh->get_aSales() / aggSls;
			pftAvg += (sh->get_profit() * sh->get_retail() / (1.0-_abEcon->_txRate)) / nActSh;
			pftWav += (sh->get_profit() * sh->get_retail() / (1.0-_abEcon->_txRate)) * sh->get_aSales() / aggSls;
			prcAvg += log(sh->get_retail()) / nActSh;
			prcWav += log(sh->get_retail()) * sh->get_aSales() / aggSls;
			ragWav += (1-(sh->get_entrWk()/(wkCntr+1.0))) * sh->get_aSales() / aggSls;
		}
	}
	for (int i = 0; i < NSHOPS; i++) {
		Shop* sh = _abEcon->_shList[i];
		if (sh->is_active()) {
			varMkp += pow(sh->get_markup() - mrkWav, 2) * sh->get_aSales() / aggSls;
			pftVar += pow((sh->get_profit() * sh->get_retail() / (1.0-_abEcon->_txRate)) - pftAvg, 2) / nActSh;
			pftWvr += pow((sh->get_profit() * sh->get_retail() / (1.0-_abEcon->_txRate)) - pftWav, 2) * sh->get_aSales() / aggSls;
			prcVar += pow(log(sh->get_retail()) - prcAvg, 2) / nActSh;
			prcWvr += pow(log(sh->get_retail()) - prcWav, 2) * sh->get_aSales() / aggSls;
			salVar += pow(sh->get_aSales() - aggSls/nActSh, 2) / nActSh;
		}
	}

	_avMkup += (mrkAvg - _avMkup) / ((wkCntr % WKSPYR) + 1.0);
	_wtMkup += (mrkWav - _wtMkup) / ((wkCntr % WKSPYR) + 1.0);
	_wtPrft += (pftWav - _wtPrft) / ((wkCntr % WKSPYR) + 1.0);
	_wtRAge += (ragWav - _wtRAge) / ((wkCntr % WKSPYR) + 1.0);
	_stdMkp += (sqrt(varMkp) - _stdMkp) / ((wkCntr % WKSPYR) + 1.0);

	if(strcmp(_abEcon->_OPMODE, "impulse")==0)
		_RMEntr[rnCntr][_abEcon->_mnCntr + _abEcon->_yrCntr*_abEcon->_MNSPYR] += nEnter;
  
	if (wkCntr + 1 > ADJLAG * WKSPYR) {
		_vncyRt[rnCntr] += (entVac - _vncyRt[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_entrFl[rnCntr] += (nEnter - _entrFl[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_entrRt[rnCntr] += (entrFr - _entrRt[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_nwJCRt[rnCntr] += (nwJCFr - _nwJCRt[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_loan2Y[rnCntr] += ((sLoans / nmIncm) - _loan2Y[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_rlLoan[rnCntr] += ((sLoans / cpIndx) - _rlLoan[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_enRlLn[rnCntr] += ((entrLn / cpIndx) - _enRlLn[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_bkrpRt[rnCntr] += (bkrpHz - _bkrpRt[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_fConRt[rnCntr] += (fConPr - _fConRt[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_jobMch[rnCntr] += (nJbMch - _jobMch[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_job2Jb[rnCntr] += (nJb2Jb - _job2Jb[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_strMch[rnCntr] += (nStMch - _strMch[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_str2St[rnCntr] += (nSt2St - _str2St[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_exitRt[rnCntr] += ((1.0 - extShp / entShp) - _exitRt[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_jbLsRt[rnCntr] += ((jobLss / mchEmp) - _jbLsRt[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_lsLfRt[rnCntr] += ((jbLsLf / mchEmp) - _lsLfRt[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_lsQtRt[rnCntr] += ((jbLsQt / mchEmp) - _lsQtRt[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_lsExRt[rnCntr] += ((jbLsEx / mchEmp) - _lsExRt[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_uempRt[rnCntr] += ((_unempl / NAGNTS) - _uempRt[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_unLfRt[rnCntr] += ((_unLoff / NAGNTS) - _unLfRt[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_unExRt[rnCntr] += ((_unExit / NAGNTS) - _unExRt[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_unQtRt[rnCntr] += ((_unQuit / NAGNTS) - _unQtRt[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_unBkRt[rnCntr] += ((_unBrok / NAGNTS) - _unBkRt[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_prcDsp[rnCntr] += (sqrt(prcVar) - _prcDsp[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_wprDsp[rnCntr] += (sqrt(prcWvr) - _wprDsp[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_pftDsp[rnCntr] += (sqrt(pftVar) - _pftDsp[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_salDsp[rnCntr] += (sqrt(salVar) - _salDsp[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
		_wpfDsp[rnCntr] += (sqrt(pftWvr) - _wpfDsp[rnCntr]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);

		_tlShop += entShp;
		_tlLfSp += enLfSp;
		_tlEnEx += nEntEx;

		_tlUnEp += nUn2Em; _tlUnDr += unDurn;
		_tlLfEp += nLf2Em; _tlLfDr += lfDurn;
		_tlExEp += nEx2Em; _tlExDr += exDurn;
		_tlQtEp += nQt2Em; _tlQtDr += qtDurn;
		_tlBkEp += nBk2Em; _tlBkDr += bkDurn;
	}

	_yeEmpl[_abEcon->_yrCntr] += _abEcon->get_agEmpl() / WKSPYR;
	_yeUnRt[_abEcon->_yrCntr] += (1.0 - _abEcon->get_agEmpl() / NAGNTS) / WKSPYR;
	_yeVcRt[_abEcon->_yrCntr] += entVac / WKSPYR;
	_yeRlWg[_abEcon->_yrCntr] += _abEcon->_rlWage / WKSPYR;
	_yePIdx[_abEcon->_yrCntr] += _abEcon->_cpIndx / WKSPYR;
	_yeLoan[_abEcon->_yrCntr] += sLoans / WKSPYR;
	_yeBkrp[_abEcon->_yrCntr] += _abEcon->_bkrpFr / WKSPYR;
	_yeFCon[_abEcon->_yrCntr] += fConPr / WKSPYR;
}

void Statistics::col_mnStat() {
	int WKSPMN = _abEcon->_WKSPMN;
	int MNSPYR = _abEcon->_MNSPYR;
	int NBANKS = _abEcon->_NBANKS;
	int wkCntr = _abEcon->_wkCntr;
	int mnCntr = _abEcon->_mnCntr;
	int rnCntr = _abEcon->_rnCntr;
	int mnShFl = _abEcon->_nFails - _lmShFl;
	int mnBkFl = _abEcon->_nBkFlr - _lmBkFl;
	int mnBkTr = _abEcon->_nBkTrb - _lmBkTr;

	double CAPGDP = _abEcon->_CAPGDP;
	double lag3RI = _abEcon->_lag3RI[mnCntr];

	if(strcmp(_abEcon->_OPMODE, "impulse")==0) {
		_RMLnGp[rnCntr][(wkCntr + 1) / WKSPMN - 1] = (double) log(CAPGDP / lag3RI);
		_RMFail[rnCntr][(wkCntr + 1) / WKSPMN - 1] = (double) mnShFl / WKSPMN;
		_RMBkFl[rnCntr][(wkCntr + 1) / WKSPMN - 1] = (double) mnBkFl / NBANKS;
		_RMBkTr[rnCntr][(wkCntr + 1) / WKSPMN - 1] = (double) mnBkTr / NBANKS / WKSPMN;
	}

	if (mnCntr + 1 == MNSPYR) {
		_lmShFl = 0;
		_lmBkFl = 0;
		_lmBkTr = 0;
	}
	else {
		_lmShFl = _abEcon->_nFails;
		_lmBkFl = _abEcon->_nBkFlr;
		_lmBkTr = _abEcon->_nBkTrb;
	}
}

void Statistics::col_yrStat() {
	int NWEEKS = _abEcon->_NWEEKS;
	int MNSPYR = _abEcon->_MNSPYR;
	int WKSPYR = _abEcon->_WKSPYR;
	int ADJLAG = _abEcon->_ADJLAG;
	int NBANKS = _abEcon->_NBANKS;
	int yrCntr = _abEcon->_yrCntr;
	int rnCntr = _abEcon->_rnCntr;
	int nFails = _abEcon->_nFails;
	int zbCntr = _abEcon->_zbCntr;
	int nHplss = _abEcon->_nHplss;
	int nBkFlr = _abEcon->_nBkFlr;
	int nBkTrb = _abEcon->_nBkTrb;

	double bndStk = _abEcon->_bndStk;
	double txRate = _abEcon->_txRate;
	double fscInv = _abEcon->_fscInv;
	double wtFail = _abEcon->_wtFail;
	double agPrft = _abEcon->_agPrft;
	double nomGdp = _abEcon->_nomGdp;
	double nPrDlt = _abEcon->_nPrDlt;
	double nPrObs = _abEcon->_nPrObs;
	double rrttrg = _abEcon->_rrttrg;
	double gdpryr = _abEcon->_gdpryr;
	double infryr = _abEcon->_infryr;
    double gdppot = _abEcon->_gdppot;
	double avgLVR = _abEcon->_avgLVR;

	_yeBnds[yrCntr] = bndStk;
	_yeTxRt[yrCntr] = txRate;
	_yeCons[yrCntr] = _abEcon->_aggCon;
	_yeInvn[yrCntr] = fscInv;
	_yeInfl[yrCntr] = _abEcon->_lagdPi[MNSPYR-1];

	if (yrCntr == 0)
		_yeDInv[yrCntr] = _yeInvn[yrCntr] - _abEcon->_iniInv;
	else
		_yeDInv[yrCntr] = _yeInvn[yrCntr] - _yeInvn[yrCntr - 1];

	for (int i = 0; i < MNSPYR; i++) {		
		_yeIncm[yrCntr] += _abEcon->_lagdRI[i] ;		
		_yeNInc[yrCntr] += _abEcon->_lagdNI[i] ;
		_yeCCRR[yrCntr] += _abEcon->_lgCCRR[i] ;
		_yeCCNR[yrCntr] += _abEcon->_lgCCNR[i];
		_yeLnGp[yrCntr] -= log(_abEcon->_lag3RI[i]);
	}

	_yeIncm[yrCntr] /= MNSPYR;
	_yeNInc[yrCntr] /= MNSPYR;
	_yeCCRR[yrCntr] /= MNSPYR;
	_yeCCNR[yrCntr] /= MNSPYR;
	_yeLnGp[yrCntr] /= MNSPYR;
	_yeLnGp[yrCntr] += log(_abEcon->_CAPGDP);

	_debtRt = (bndStk/_abEcon->_cpIndx) / (_yeIncm[yrCntr]*WKSPYR);

	_RYLnGp[rnCntr][yrCntr] = _yeLnGp[yrCntr];
	_RYFail[rnCntr][yrCntr] = (double) nFails / WKSPYR;
	_RYAvMk[rnCntr][yrCntr] = _avMkup;
	_RYWtMk[rnCntr][yrCntr] = _wtMkup;
	_RYStMk[rnCntr][yrCntr] = _stdMkp;
	_RYWtPf[rnCntr][yrCntr] = _wtPrft;
	_RYRAge[rnCntr][yrCntr] = _wtRAge;
	_RYDbtR[rnCntr][yrCntr] = _debtRt;
	_RYTxRt[rnCntr][yrCntr] = txRate;
	_RYCCRR[rnCntr][yrCntr] = _yeCCRR[yrCntr];
	_RYCCPi[rnCntr][yrCntr] = log(_yeInfl[yrCntr]);
	_RYGpTg[rnCntr][yrCntr] = log(_abEcon->_CAPGDP) - gdppot;
	_RYRtTg[rnCntr][yrCntr] = rrttrg;
	_RYGpRm[rnCntr][yrCntr] = gdpryr;
	_RYPiRm[rnCntr][yrCntr] = infryr;
	_RYGdPt[rnCntr][yrCntr] = gdppot;
	_RYBkFl[rnCntr][yrCntr] = (double) nBkFlr / NBANKS;
	_RYBkTr[rnCntr][yrCntr] = (double) nBkTrb / WKSPYR / NBANKS;

	if (yrCntr + 1 > ADJLAG) {
		_CYIncm[rnCntr] += (_yeIncm[yrCntr] - _CYIncm[rnCntr]) / (yrCntr + 1.0 - ADJLAG);
		_CYLnGp[rnCntr] += (_yeLnGp[yrCntr] - _CYLnGp[rnCntr]) / (yrCntr + 1.0 - ADJLAG);
		_CYFail[rnCntr] += ((double) nFails / WKSPYR - _CYFail[rnCntr]) / (yrCntr + 1.0 - ADJLAG);
		_CYWtFl[rnCntr] += ((double) wtFail / WKSPYR - _CYWtFl[rnCntr]) / (yrCntr + 1.0 - ADJLAG);
		_CYHpls[rnCntr] += ((double) nHplss / WKSPYR - _CYHpls[rnCntr]) / (yrCntr + 1.0 - ADJLAG);
		_CYZbRt[rnCntr] += ((double) zbCntr / MNSPYR - _CYZbRt[rnCntr]) / (yrCntr + 1.0 - ADJLAG);
		_CYInfl[rnCntr] += (_yeInfl[yrCntr] -1.0 - _CYInfl[rnCntr]) / (yrCntr + 1.0 - ADJLAG);
		_CYCCPi[rnCntr] += (log(_yeInfl[yrCntr]) - _CYCCPi[rnCntr]) / (yrCntr + 1.0 - ADJLAG);
		_CYCCRR[rnCntr] += (_yeCCRR[yrCntr] - _CYCCRR[rnCntr]) / (yrCntr + 1.0 - ADJLAG);
		_CYBkFl[rnCntr] += ((double) nBkFlr / NBANKS - _CYBkFl[rnCntr]) / (yrCntr + 1.0 - ADJLAG);
		_CYBkTr[rnCntr] += ((double) nBkTrb / WKSPYR / NBANKS - _CYBkTr[rnCntr]) / (yrCntr + 1.0 - ADJLAG);
		_prftRt[rnCntr] += (agPrft/nomGdp - _prftRt[rnCntr]) / (yrCntr + 1.0 - ADJLAG);
		_pFreqy[rnCntr] += (WKSPYR*nPrDlt/nPrObs - _pFreqy[rnCntr]) / (yrCntr + 1.0 - ADJLAG);
		_lnVlRt[rnCntr] += (avgLVR - _lnVlRt[rnCntr]) / (yrCntr + 1.0 - ADJLAG);
	}

	if (yrCntr + 1 == NWEEKS / WKSPYR)
		_erWtMk[rnCntr] = _wtMkup;

	_avMkup = 0.0;
	_wtMkup = 0.0;
	_stdMkp = 0.0;
	_wtPrft = 0.0;
	_wtRAge = 0.0;
}

void Statistics::col_cnStat() {
	int ADJLAG = _abEcon->_ADJLAG;
	int NSHOPS = _abEcon->_nShops;
	int cnCntr = _abEcon->_cnCntr;
	int rnCntr = _abEcon->_rnCntr;
	int nActSh = _abEcon->_nActSh;

	int numSrv = 0;
	for (int i = 0; i < NSHOPS; i++) {
		Shop* sh = _abEcon->_shList[i];
		if (sh->is_active() && sh->get_census())
			numSrv++;
	}

	if (cnCntr > ADJLAG / 5 - 1)
		_CYExit[rnCntr] += ((1.0 - (double) numSrv / _lastCn) - _CYExit[rnCntr]) / (cnCntr + 1.0 - ADJLAG / 5.0);

	_lastCn = nActSh;
}

void Statistics::col_rnStat() {
	int NGOODS = _abEcon->_NGOODS;
	int NWEEKS = _abEcon->_NWEEKS;
	int NYEARS = _abEcon->_NYEARS;
	int WKSPYR = _abEcon->_WKSPYR;
	int ADJLAG = _abEcon->_ADJLAG;
	int rnCntr = _abEcon->_rnCntr;

	_badRns[rnCntr] = _abEcon->_crashd;
	_lifeEx[rnCntr] = _tlEnEx ? (double) _tlLfSp / (double) _tlEnEx : -1.0;
	_shPrWk[rnCntr] = _tlShop / (double) (NWEEKS - ADJLAG * WKSPYR);
	_failHz[rnCntr] = _CYFail[rnCntr]  / _shPrWk[rnCntr];
	_hplsHz[rnCntr] = _CYHpls[rnCntr]  / _shPrWk[rnCntr];
	_uempDr[rnCntr] = _tlUnEp ? _tlUnDr / _tlUnEp : -1.0;
	_unLfDr[rnCntr] = _tlLfEp ? _tlLfDr / _tlLfEp : -1.0;
	_unQtDr[rnCntr] = _tlQtEp ? _tlQtDr / _tlQtEp : -1.0;
	_unExDr[rnCntr] = _tlExEp ? _tlExDr / _tlExEp : -1.0;
	_unBkDr[rnCntr] = _tlBkEp ? _tlBkDr / _tlBkEp : -1.0;

	_lastCn = NGOODS;
	_avMkup = 0.0;
	_wtMkup = 0.0;
	_stdMkp = 0.0;
	_wtPrft = 0.0;
	_wtRAge = 0.0;
	_tlShop = 0.0;
	_tlLfSp = 0;
	_tlEnEx = 0;

	_tlUnEp = 0.0; _tlUnDr = 0.0;
	_tlLfEp = 0.0; _tlLfDr = 0.0;
	_tlExEp = 0.0; _tlExDr = 0.0;
	_tlQtEp = 0.0; _tlQtDr = 0.0;
	_tlBkEp = 0.0; _tlBkDr = 0.0;

	if(_abEcon->_crashd) {
		for (int i = 0; i < NYEARS; i++) {
			_yeBnds[i] = 0.0;
			_yeLoan[i] = 0.0;
			_yeBkrp[i] = 0.0;
			_yeFCon[i] = 0.0;
			_yeIncm[i] = 0.0;
			_yeNInc[i] = 0.0;
			_yeLnGp[i] = 0.0;
			_yeCons[i] = 0.0;
			_yeInvn[i] = 0.0;
			_yeDInv[i] = 0.0;
			_yeEmpl[i] = 0.0;
			_yeUnRt[i] = 0.0;
			_yeVcRt[i] = 0.0;
			_yeRlWg[i] = 0.0;
			_yePIdx[i] = 0.0;
			_yeInfl[i] = 0.0;
			_yeCCRR[i] = 0.0;
			_yeCCNR[i] = 0.0;
			_yeTxRt[i] = 0.0;
		}
		_nCrash++;
		return;
	}

	double avLInc = 0.0;
	double avCCPi = 0.0;
	double varInc = 0.0;
	double corInc = 0.0;
	double varInf = 0.0; // variance of continuously compounded annual inflation rates
	double corInf = 0.0; // autocorrelation of continuously compounded annual inflation rates

	for (int i = ADJLAG; i < NYEARS; i++) {
		avLInc += log(_yeIncm[i]);
		avCCPi += log(_yeInfl[i]);
	}

	avLInc /= (NYEARS - ADJLAG);
	avCCPi /= (NYEARS - ADJLAG);

	for (int i = ADJLAG; i < NYEARS; i++) {
		varInc += pow(log(_yeIncm[i]) - avLInc, 2);
		varInf += pow(log(_yeInfl[i]) - avCCPi, 2);
		if(i>ADJLAG) {
			corInc += (log(_yeIncm[i]) - avLInc) * (log(_yeIncm[i-1]) - avLInc);
			corInf += (log(_yeInfl[i]) - avCCPi) * (log(_yeInfl[i-1]) - avCCPi);
		}
	}

	varInc /= (NYEARS - ADJLAG);
	varInf /= (NYEARS - ADJLAG);
	corInc /= (NYEARS - ADJLAG -1);
	corInf /= (NYEARS - ADJLAG -1);

	double covyrt = 0.0;
	for (int t = 0; t < NYEARS - ADJLAG; t++)
		covyrt += (log(_yeIncm[t+ADJLAG]) - avLInc) * (t - _AVGYRS);

	double tslope = covyrt/_MOMENT;
	varInc -= tslope * tslope * _MOMENT / (double) (NYEARS - ADJLAG);

	_gapVol[rnCntr] = sqrt(varInc);
	_gapCor[rnCntr] = corInc / varInc;
	_infVol[rnCntr] = sqrt(varInf);
	_infCor[rnCntr] = corInf / varInf;

    // Construct the list of correlation coeff's of differenced logged loans and nominal GDP
    double* dLogLn = new double[NYEARS - ADJLAG - 1];
    double* dLgGDP = new double[NYEARS - ADJLAG - 1];
    for (int i = 0; i < NYEARS - ADJLAG - 1; ++i) {
        dLogLn[i] = log(_yeLoan[i+ADJLAG+1]) - log(_yeLoan[i+ADJLAG]);
        dLgGDP[i] = log(_yeNInc[i+ADJLAG+1]) - log(_yeNInc[i+ADJLAG]);
    }
    double avDLgL=0, avDLgY=0;
    for (int i=0; i<NYEARS-ADJLAG-1; ++i) {
        avDLgL += dLogLn[i];
        avDLgY += dLgGDP[i];
    }
    avDLgL /= NYEARS-ADJLAG-1;
    avDLgY /= NYEARS-ADJLAG-1;

    double ssdDLL=0, ssdDLY=0;
    for (int i=0; i<NYEARS-ADJLAG-1; ++i) {
        ssdDLL += pow(dLogLn[i]-avDLgL, 2);
        ssdDLY += pow(dLgGDP[i]-avDLgY, 2);
    }
    double scdLYL=0;
    for (int i=0; i<NYEARS-ADJLAG-1; ++i)
        scdLYL += (dLogLn[i]-avDLgL)*(dLgGDP[i]-avDLgY);

    _YLCorD[rnCntr] = scdLYL / (sqrt(ssdDLL) * sqrt(ssdDLY));

	for (int i = 0; i < NYEARS; i++) {
		_yeBnds[i] = 0.0;
		_yeLoan[i] = 0.0;
		_yeBkrp[i] = 0.0;
		_yeFCon[i] = 0.0;
		_yeIncm[i] = 0.0;
		_yeNInc[i] = 0.0;
		_yeLnGp[i] = 0.0;
		_yeCons[i] = 0.0;
		_yeInvn[i] = 0.0;
		_yeDInv[i] = 0.0;
		_yeEmpl[i] = 0.0;
		_yeUnRt[i] = 0.0;
		_yeVcRt[i] = 0.0;
		_yeRlWg[i] = 0.0;
		_yePIdx[i] = 0.0;
		_yeInfl[i] = 0.0;
		_yeCCRR[i] = 0.0;
		_yeCCNR[i] = 0.0;
		_yeTxRt[i] = 0.0;
	}
}

void Statistics::cal_CRStat() {
	int NSRUNS = _abEcon->_NSRUNS;
	int NYEARS = _abEcon->_NYEARS;

	for(int i = 0; i < NSRUNS; i++) {
		if(_badRns[i])
			continue;
		for( int j = 0; j < NYEARS; j++) {
			_CRLnGp[j] += _RYLnGp[i][j] / (NSRUNS - _nCrash);
			_CRFail[j] += _RYFail[i][j] / (NSRUNS - _nCrash);
			_CRAvMk[j] += _RYAvMk[i][j] / (NSRUNS - _nCrash);
			_CRWtMk[j] += _RYWtMk[i][j] / (NSRUNS - _nCrash);
			_CRStMk[j] += _RYStMk[i][j] / (NSRUNS - _nCrash);
			_CRWtPf[j] += _RYWtPf[i][j] / (NSRUNS - _nCrash);
			_CRRAge[j] += _RYRAge[i][j] / (NSRUNS - _nCrash);
			_CRDbtR[j] += _RYDbtR[i][j] / (NSRUNS - _nCrash);
			_CRTxRt[j] += _RYTxRt[i][j] / (NSRUNS - _nCrash);
			_CRCCRR[j] += _RYCCRR[i][j] / (NSRUNS - _nCrash);
			_CRCCPi[j] += _RYCCPi[i][j] / (NSRUNS - _nCrash);
			_CRGpTg[j] += _RYGpTg[i][j] / (NSRUNS - _nCrash);
			_CRRtTg[j] += _RYRtTg[i][j] / (NSRUNS - _nCrash);
			_CRGpRm[j] += _RYGpRm[i][j] / (NSRUNS - _nCrash);
			_CRPiRm[j] += _RYPiRm[i][j] / (NSRUNS - _nCrash);
			_CRGdPt[j] += _RYGdPt[i][j] / (NSRUNS - _nCrash);
		}
	}

	double* stLnGp = new double[NSRUNS - _nCrash];
	double* stGpVl = new double[NSRUNS - _nCrash];
	double* stCCPi = new double[NSRUNS - _nCrash];
	double* stPiVl = new double[NSRUNS - _nCrash];
	double* stRlRt = new double[NSRUNS - _nCrash];
	double* stUnRt = new double[NSRUNS - _nCrash];
	double* stUnDr = new double[NSRUNS - _nCrash];
	double* stJbLs = new double[NSRUNS - _nCrash];
	double* stLsLf = new double[NSRUNS - _nCrash];
	double* stLsQt = new double[NSRUNS - _nCrash];
	double* stLsEx = new double[NSRUNS - _nCrash];
	double* stExRt = new double[NSRUNS - _nCrash];
	double* stFlHz = new double[NSRUNS - _nCrash];
	double* stWtMk = new double[NSRUNS - _nCrash];
	double* stPfRt = new double[NSRUNS - _nCrash];
	double* stPrFq = new double[NSRUNS - _nCrash];
	double* stBkFl = new double[NSRUNS - _nCrash];
	double* stBkTr = new double[NSRUNS - _nCrash];
	double* stGpCr = new double[NSRUNS - _nCrash];
	double* stPiCr = new double[NSRUNS - _nCrash];
	double* stWkSh = new double[NSRUNS - _nCrash];
	double* stHpHz = new double[NSRUNS - _nCrash];
	double* stEnFl = new double[NSRUNS - _nCrash];
	double* stEnRt = new double[NSRUNS - _nCrash];
	double* stNJCR = new double[NSRUNS - _nCrash];
	double* stLn2Y = new double[NSRUNS - _nCrash];
	double* stRlLn = new double[NSRUNS - _nCrash];
	double* stEnLn = new double[NSRUNS - _nCrash];
	double* stLVRt = new double[NSRUNS - _nCrash];
	double* stBkrp = new double[NSRUNS - _nCrash];
	double* stFCon = new double[NSRUNS - _nCrash];
	double* stZbRt = new double[NSRUNS - _nCrash];
	double* stSlDs = new double[NSRUNS - _nCrash];

	for(int i = 0, k = 0; i < NSRUNS; i++) {
		if (_badRns[i])
			continue;

		_avLnGp += _CYLnGp[i] / (NSRUNS - _nCrash);
		_avGpVl += _gapVol[i] / (NSRUNS - _nCrash);
		_avGpCr += _gapCor[i] / (NSRUNS - _nCrash);
		_avCCPi += _CYCCPi[i] / (NSRUNS - _nCrash);
		_avPiRt += _CYInfl[i] / (NSRUNS - _nCrash);
		_avPiVl += _infVol[i] / (NSRUNS - _nCrash);
		_avPiCr += _infCor[i] / (NSRUNS - _nCrash);
		_avRlRt += _CYCCRR[i] / (NSRUNS - _nCrash);
		_avPrDs += _prcDsp[i] / (NSRUNS - _nCrash);
		_avWPrD += _wprDsp[i] / (NSRUNS - _nCrash);
		_avPfDs += _pftDsp[i] / (NSRUNS - _nCrash);
		_avSlDs += _salDsp[i] / (NSRUNS - _nCrash);
		_avWPfD += _wpfDsp[i] / (NSRUNS - _nCrash);
		_avNmFl += _CYFail[i] / (NSRUNS - _nCrash);
		_avWtFl += _CYWtFl[i] / (NSRUNS - _nCrash);
		_avHpls += _CYHpls[i] / (NSRUNS - _nCrash);
		_avZbRt += _CYZbRt[i] / (NSRUNS - _nCrash);
		_avPfRt += _prftRt[i] / (NSRUNS - _nCrash);
		_avPrFq += _pFreqy[i] / (NSRUNS - _nCrash);
		_avBkFl += _CYBkFl[i] / (NSRUNS - _nCrash);
		_avBkTr += _CYBkTr[i] / (NSRUNS - _nCrash);

		_avUnRt += _uempRt[i] / (NSRUNS - _nCrash);
		_avUnLf += _unLfRt[i] / (NSRUNS - _nCrash);
		_avUnQt += _unQtRt[i] / (NSRUNS - _nCrash);
		_avUnEx += _unExRt[i] / (NSRUNS - _nCrash);
		_avUnBk += _unBkRt[i] / (NSRUNS - _nCrash);
		_avUnDr += _uempDr[i] / (NSRUNS - _nCrash);
		_avLfDr += _unLfDr[i] / (NSRUNS - _nCrash);
		_avQtDr += _unQtDr[i] / (NSRUNS - _nCrash);
		_avExDr += _unExDr[i] / (NSRUNS - _nCrash);
		_avBkDr += _unBkDr[i] / (NSRUNS - _nCrash);

		_avVnRt += _vncyRt[i] / (NSRUNS - _nCrash);
		_avEnFl += _entrFl[i] / (NSRUNS - _nCrash);
		_avEnRt += _entrRt[i] / (NSRUNS - _nCrash);
		_avNJCR += _nwJCRt[i] / (NSRUNS - _nCrash);
		_avLn2Y += _loan2Y[i] / (NSRUNS - _nCrash);
		_avRlLn += _rlLoan[i] / (NSRUNS - _nCrash);
		_avEnLn += _enRlLn[i] / (NSRUNS - _nCrash);
		_avLVRt += _lnVlRt[i] / (NSRUNS - _nCrash);
		_avBkrp += _bkrpRt[i] / (NSRUNS - _nCrash);
		_avFCon += _fConRt[i] / (NSRUNS - _nCrash);
		_avJbMt += _jobMch[i] / (NSRUNS - _nCrash);
		_avJ2Jb += _job2Jb[i] / (NSRUNS - _nCrash);
		_avStMt += _strMch[i] / (NSRUNS - _nCrash);
		_avS2St += _str2St[i] / (NSRUNS - _nCrash);
		_avLfEx += _lifeEx[i] / (NSRUNS - _nCrash);
		_avWkSh += _shPrWk[i] / (NSRUNS - _nCrash);
		_avFlHz += _failHz[i] / (NSRUNS - _nCrash);
		_avHpHz += _hplsHz[i] / (NSRUNS - _nCrash);
		_avExRt += _CYExit[i] / (NSRUNS - _nCrash);
		_avJbLs += _jbLsRt[i] / (NSRUNS - _nCrash);
		_avLsLf += _lsLfRt[i] / (NSRUNS - _nCrash);
		_avLsQt += _lsQtRt[i] / (NSRUNS - _nCrash);
		_avLsEx += _lsExRt[i] / (NSRUNS - _nCrash);
		_avWtMk += _erWtMk[i] / (NSRUNS - _nCrash);

		for (int j = 0; j < 20; j++)
			_avPfMk[j] += _pftMrk[i][j] / (NSRUNS - _nCrash);

		stLnGp[k] = _CYLnGp[i];
		stGpVl[k] = _gapVol[i];
		stCCPi[k] = _CYCCPi[i];
		stPiVl[k] = _infVol[i];
		stRlRt[k] = _CYCCRR[i];
		stUnRt[k] = _uempRt[i];
		stUnDr[k] = _uempDr[i];
		stJbLs[k] = _jbLsRt[i];
		stLsLf[k] = _lsLfRt[i];
		stLsQt[k] = _lsQtRt[i];
		stLsEx[k] = _lsExRt[i];
		stExRt[k] = _CYExit[i];
		stFlHz[k] = _failHz[i];
		stWtMk[k] = _erWtMk[i];
		stPfRt[k] = _prftRt[i];
		stPrFq[k] = _pFreqy[i];
		stBkFl[k] = _CYBkFl[i];
		stBkTr[k] = _CYBkTr[i];
		stGpCr[k] = _gapCor[i];
		stPiCr[k] = _infCor[i];
		stWkSh[k] = _shPrWk[i];
		stHpHz[k] = _hplsHz[i];
		stEnFl[k] = _entrFl[i];
		stEnRt[k] = _entrRt[i];
		stNJCR[k] = _nwJCRt[i];
		stLn2Y[k] = _loan2Y[i];
		stRlLn[k] = _rlLoan[i];
		stEnLn[k] = _enRlLn[i];
		stLVRt[k] = _lnVlRt[i];
		stBkrp[k] = _bkrpRt[i];
		stFCon[k] = _fConRt[i];
		stZbRt[k] = _CYZbRt[i];
		stSlDs[k] = _salDsp[i];

		k++;
	}

	int* ixLnGp = new int[NSRUNS - _nCrash];
	srt_idxArr(stLnGp, ixLnGp, NSRUNS - _nCrash);

	// compute averages by decile of the log output gap distribution
	for (int i = 0; i < 10; i++) {
		int bgnIdx = (int)((NSRUNS - _nCrash) * (1 - 0.1 * i)) - 1;
		int endIdx = (int)((NSRUNS - _nCrash) * (1 - 0.1 * (i + 1))) - 1;

		for (int j = bgnIdx; j > endIdx; j--) {
			_dcLnGp[i] += stLnGp[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcUnRt[i] += stUnRt[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcUnDr[i] += stUnDr[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcGpVl[i] += stGpVl[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcPiVl[i] += stPiVl[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcCCPi[i] += stCCPi[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcBkFl[i] += stBkFl[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcBkTr[i] += stBkTr[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcJbLs[i] += stJbLs[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcLsLf[i] += stLsLf[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcLsQt[i] += stLsQt[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcLsEx[i] += stLsEx[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcFlHz[i] += stFlHz[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcWkSh[i] += stWkSh[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcHpHz[i] += stHpHz[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcEnFl[i] += stEnFl[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcEnRt[i] += stEnRt[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcNJCR[i] += stNJCR[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcLn2Y[i] += stLn2Y[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcRlLn[i] += stRlLn[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcEnLn[i] += stEnLn[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcLVRt[i] += stLVRt[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcBkrp[i] += stBkrp[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcFCon[i] += stFCon[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcZbRt[i] += stZbRt[ixLnGp[j]] / (bgnIdx - endIdx);
			_dcSlDs[i] += stSlDs[ixLnGp[j]] / (bgnIdx - endIdx);
		}
	}

	qsort(stLnGp, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stGpVl, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stCCPi, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stPiVl, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stRlRt, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stUnRt, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stUnDr, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stJbLs, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stLsLf, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stLsQt, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stLsEx, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stExRt, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stFlHz, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stWtMk, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stPfRt, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stPrFq, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stBkFl, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stBkTr, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stGpCr, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stPiCr, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stWkSh, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stHpHz, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stEnFl, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stEnRt, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stNJCR, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stLn2Y, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stRlLn, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stEnLn, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stLVRt, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stBkrp, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stFCon, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stZbRt, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);
	qsort(stSlDs, NSRUNS - _nCrash, sizeof(double), qst_cmpFnc);

	_mdLnGp = stLnGp[(int)((NSRUNS - _nCrash) / 2)];
	_mdGpVl = stGpVl[(int)((NSRUNS - _nCrash) / 2)];
	_mdCCPi = stCCPi[(int)((NSRUNS - _nCrash) / 2)];
	_mdPiVl = stPiVl[(int)((NSRUNS - _nCrash) / 2)];
	_mdRlRt = stRlRt[(int)((NSRUNS - _nCrash) / 2)];
	_mdUnRt = stUnRt[(int)((NSRUNS - _nCrash) / 2)];
	_mdUnDr = stUnDr[(int)((NSRUNS - _nCrash) / 2)];
	_mdJbLs = stJbLs[(int)((NSRUNS - _nCrash) / 2)];
	_mdLsLf = stLsLf[(int)((NSRUNS - _nCrash) / 2)];
	_mdLsQt = stLsQt[(int)((NSRUNS - _nCrash) / 2)];
	_mdLsEx = stLsEx[(int)((NSRUNS - _nCrash) / 2)];
	_mdExRt = stExRt[(int)((NSRUNS - _nCrash) / 2)];
	_mdFlHz = stFlHz[(int)((NSRUNS - _nCrash) / 2)];
	_mdWtMk = stWtMk[(int)((NSRUNS - _nCrash) / 2)];
	_mdPfRt = stPfRt[(int)((NSRUNS - _nCrash) / 2)];
	_mdPrFq = stPrFq[(int)((NSRUNS - _nCrash) / 2)];
	_mdBkFl = stBkFl[(int)((NSRUNS - _nCrash) / 2)];
	_mdBkTr = stBkTr[(int)((NSRUNS - _nCrash) / 2)];
	_mdGpCr = stGpCr[(int)((NSRUNS - _nCrash) / 2)];
	_mdPiCr = stPiCr[(int)((NSRUNS - _nCrash) / 2)];
	_mdWkSh = stWkSh[(int)((NSRUNS - _nCrash) / 2)];
	_mdHpHz = stHpHz[(int)((NSRUNS - _nCrash) / 2)];
	_mdEnFl = stEnFl[(int)((NSRUNS - _nCrash) / 2)];
	_mdEnRt = stEnRt[(int)((NSRUNS - _nCrash) / 2)];
	_mdNJCR = stNJCR[(int)((NSRUNS - _nCrash) / 2)];
	_mdLn2Y = stLn2Y[(int)((NSRUNS - _nCrash) / 2)];
	_mdRlLn = stRlLn[(int)((NSRUNS - _nCrash) / 2)];
	_mdEnLn = stEnLn[(int)((NSRUNS - _nCrash) / 2)];
	_mdLVRt = stLVRt[(int)((NSRUNS - _nCrash) / 2)];
	_mdBkrp = stBkrp[(int)((NSRUNS - _nCrash) / 2)];
	_mdFCon = stFCon[(int)((NSRUNS - _nCrash) / 2)];
	_mdZbRt = stZbRt[(int)((NSRUNS - _nCrash) / 2)];
	_mdSlDs = stSlDs[(int)((NSRUNS - _nCrash) / 2)];

	_lwOpGp = stLnGp[(int)((NSRUNS - _nCrash) * 0.05)];
	_upOpGp = stLnGp[(int)((NSRUNS - _nCrash) * 0.95)];

	delete[] stLnGp;
	delete[] stGpVl;
	delete[] stCCPi;
	delete[] stPiVl;
	delete[] stRlRt;
	delete[] stUnRt;
	delete[] stUnDr;
	delete[] stJbLs;
	delete[] stLsLf;
	delete[] stLsQt;
	delete[] stLsEx;
	delete[] stExRt;
	delete[] stFlHz;
	delete[] stWtMk;
	delete[] stPfRt;
	delete[] stPrFq;
	delete[] stBkFl;
	delete[] stBkTr;
	delete[] stGpCr;
	delete[] stPiCr;
	delete[] stWkSh;
	delete[] stHpHz;
	delete[] stEnFl;
	delete[] stEnRt;
	delete[] stNJCR;
	delete[] stLn2Y;
	delete[] stRlLn;
	delete[] stEnLn;
	delete[] stLVRt;
	delete[] stBkrp;
	delete[] stFCon;
	delete[] stZbRt;
	delete[] stSlDs;
	delete[] ixLnGp;
}

void Statistics::cal_pftMrk() {
	int NSHOPS = _abEcon->_nShops;
	int WKSPYR = _abEcon->_WKSPYR;
	int ADJLAG = _abEcon->_ADJLAG;
	int wkCntr = _abEcon->_wkCntr;
	int rnCntr = _abEcon->_rnCntr;

	double MARKUP = _abEcon->_MARKUP;

	double shpCtr[20];		// the number of active shops in each of 20 markup percentiles
	double avgPft[20];		// avg profit of active shops in each of 20 markup percentiles

	for (int i = 0; i < 20; i++) {
		shpCtr[i] = 0.0;
		avgPft[i] = 0.0;
	}

	double avgAll = 0.0;	// avg profit of active shops in the 40-60 markup percentile
	double ctrAll = 0.0;	// the number of active shops in the 40-60 markup percentile

	for (int i = 0; i < NSHOPS; i++) {
		Shop* sh = _abEcon->_shList[i];
		if (sh->is_active()) {
			double markup = sh->get_markup();
			double profit = sh->get_profit();
			for (int j = 0; j < 20; j++) {
				double lwrbnd = MARKUP * 2.0 * (0.40 + (j * 0.01));
				double uprbnd = MARKUP * 2.0 * (0.40 + (j * 0.01) + 0.01);
				if (lwrbnd < markup && markup <= uprbnd) {
					avgPft[j] += (profit - avgPft[j]) / (shpCtr[j] + 1.0);
					shpCtr[j]++;

					avgAll += (profit - avgAll) / (ctrAll + 1.0);
					ctrAll++;
					break;
				}
			}
		}
	}

	for (int i = 0; i < 20; i++)
		_pftMrk[rnCntr][i] += (avgPft[i] / avgAll - _pftMrk[rnCntr][i]) / (wkCntr + 1.0 - ADJLAG * WKSPYR);
}

int qst_cmpFnc(const void* argOne, const void* argTwo) {
	if (*(double*)argOne > *(double*)argTwo)
		return 1;
	else if (*(double*)argOne < *(double*)argTwo)
		return -1;
	else
		return 0;
}

void srt_idxArr(double* dblArr, int* idxArr, int arrDim) {
	for (int i = 0; i < arrDim; i++)
		idxArr[i] = i;

	// standard bubble sort algorithm
	bool idxSwp = true;
	for (int i = 1; (i <= arrDim) && idxSwp; i++) {
		idxSwp = false;
		for (int j = 0; j < (arrDim - 1); j++) {
			// sort idxArr in descending order
			if (dblArr[idxArr[j + 1]] > dblArr[idxArr[j]]) {
				int idxTmp = idxArr[j];
				idxArr[j] = idxArr[j + 1];
				idxArr[j + 1] = idxTmp;
				idxSwp = true;
			}
		}
	}

	return;
}
