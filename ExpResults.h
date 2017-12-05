/***********************************************************************************************
 * ExpResults.h
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised July 25 2016
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

#include <string>
#include "Economy.h"

using namespace std;

#ifndef EXPRESULTS_H
#define EXPRESULTS_H

const int NSTATS = 28;

const string avgVar[NSTATS] = {
	"EXPVAR", "AVGGAP", "AVGUNR", "AVGVOL",
	"AVGIVL", "AVGINF", "AVGBFL", "AVGBTR",
	"AVGLSS", "AVGLLO", "AVGLQT", "AVGLEX",
	"AVGUDR", "AVGSHP", "AVGSFL", "AVGHPL",
	"AVGENF", "AVGENR", "AVGNJC", "AVGL2Y",
	"AVGRLN", "AVGELN", "AVGLVR", "AVGBRP",
	"AVGFCN", "AVGZLB", "AVGSDS", "NCRASH"
};

const string mdnVar[NSTATS] = {
	"EXPVAR", "MDNGAP", "MDNUNR", "MDNVOL",
	"MDNIVL", "MDNINF", "MDNBFL", "MDNBTR",
	"MDNLSS", "MDNLLO", "MDNLQT", "MDNLEX",
	"MDNUDR", "MDNSHP", "MDNSFL", "MDNHPL",
	"MDNENF", "MDNENR", "MDNNJC", "MDNL2Y",
	"MDNRLN", "MDNELN", "MDNLVR", "MDNBRP",
	"MDNFCN", "MDNZLB", "MDNSDS", "NCRASH"
};

const string d01Var[NSTATS] = {
	"EXPVAR", "D01GAP", "D01UNR", "D01VOL",
	"D01IVL", "D01INF", "D01BFL", "D01BTR",
	"D01LSS", "D01LLO", "D01LQT", "D01LEX",
	"D01UDR", "D01SHP", "D01SFL", "D01HPL",
	"D01ENF", "D01ENR", "D01NJC", "D01L2Y",
	"D01RLN", "D01ELN", "D01LVR", "D01BRP",
	"D01FCN", "D01ZLB", "D01SDS", "NCRASH"
};

const string d02Var[NSTATS] = {
	"EXPVAR", "D02GAP", "D02UNR", "D02VOL",
	"D02IVL", "D02INF", "D02BFL", "D02BTR",
	"D02LSS", "D02LLO", "D02LQT", "D02LEX",
	"D02UDR", "D02SHP", "D02SFL", "D02HPL",
	"D02ENF", "D02ENR", "D02NJC", "D02L2Y",
	"D02RLN", "D02ELN", "D02LVR", "D02BRP",
	"D02FCN", "D02ZLB", "D02SDS", "NCRASH"
};

const string d03Var[NSTATS] = {
	"EXPVAR", "D03GAP", "D03UNR", "D03VOL",
	"D03IVL", "D03INF", "D03BFL", "D03BTR",
	"D03LSS", "D03LLO", "D03LQT", "D03LEX",
	"D03UDR", "D03SHP", "D03SFL", "D03HPL",
	"D03ENF", "D03ENR", "D03NJC", "D03L2Y",
	"D03RLN", "D03ELN", "D03LVR", "D03BRP",
	"D03FCN", "D03ZLB", "D03SDS", "NCRASH"
};

const string d04Var[NSTATS] = {
	"EXPVAR", "D04GAP", "D04UNR", "D04VOL",
	"D04IVL", "D04INF", "D04BFL", "D04BTR",
	"D04LSS", "D04LLO", "D04LQT", "D04LEX",
	"D04UDR", "D04SHP", "D04SFL", "D04HPL",
	"D04ENF", "D04ENR", "D04NJC", "D04L2Y",
	"D04RLN", "D04ELN", "D04LVR", "D04BRP",
	"D04FCN", "D04ZLB", "D04SDS", "NCRASH"
};

const string d05Var[NSTATS] = {
	"EXPVAR", "D05GAP", "D05UNR", "D05VOL",
	"D05IVL", "D05INF", "D05BFL", "D05BTR",
	"D05LSS", "D05LLO", "D05LQT", "D05LEX",
	"D05UDR", "D05SHP", "D05SFL", "D05HPL",
	"D05ENF", "D05ENR", "D05NJC", "D05L2Y",
	"D05RLN", "D05ELN", "D05LVR", "D05BRP",
	"D05FCN", "D05ZLB", "D05SDS", "NCRASH"
};

const string d06Var[NSTATS] = {
	"EXPVAR", "D06GAP", "D06UNR", "D06VOL",
	"D06IVL", "D06INF", "D06BFL", "D06BTR",
	"D06LSS", "D06LLO", "D06LQT", "D06LEX",
	"D06UDR", "D06SHP", "D06SFL", "D06HPL",
	"D06ENF", "D06ENR", "D06NJC", "D06L2Y",
	"D06RLN", "D06ELN", "D06LVR", "D06BRP",
	"D06FCN", "D06ZLB", "D06SDS", "NCRASH"
};

const string d07Var[NSTATS] = {
	"EXPVAR", "D07GAP", "D07UNR", "D07VOL",
	"D07IVL", "D07INF", "D07BFL", "D07BTR",
	"D07LSS", "D07LLO", "D07LQT", "D07LEX",
	"D07UDR", "D07SHP", "D07SFL", "D07HPL",
	"D07ENF", "D07ENR", "D07NJC", "D07L2Y",
	"D07RLN", "D07ELN", "D07LVR", "D07BRP",
	"D07FCN", "D07ZLB", "D07SDS", "NCRASH"
};

const string d08Var[NSTATS] = {
	"EXPVAR", "D08GAP", "D08UNR", "D08VOL",
	"D08IVL", "D08INF", "D08BFL", "D08BTR",
	"D08LSS", "D08LLO", "D08LQT", "D08LEX",
	"D08UDR", "D08SHP", "D08SFL", "D08HPL",
	"D08ENF", "D08ENR", "D08NJC", "D08L2Y",
	"D08RLN", "D08ELN", "D08LVR", "D08BRP",
	"D08FCN", "D08ZLB", "D08SDS", "NCRASH"
};

const string d09Var[NSTATS] = {
	"EXPVAR", "D09GAP", "D09UNR", "D09VOL",
	"D09IVL", "D09INF", "D09BFL", "D09BTR",
	"D09LSS", "D09LLO", "D09LQT", "D09LEX",
	"D09UDR", "D09SHP", "D09SFL", "D09HPL",
	"D09ENF", "D09ENR", "D09NJC", "D09L2Y",
	"D09RLN", "D09ELN", "D09LVR", "D09BRP",
	"D09FCN", "D09ZLB", "D09SDS", "NCRASH"
};

const string d10Var[NSTATS] = {
	"EXPVAR", "D10GAP", "D10UNR", "D10VOL",
	"D10IVL", "D10INF", "D10BFL", "D10BTR",
	"D10LSS", "D10LLO", "D10LQT", "D10LEX",
	"D10UDR", "D10SHP", "D10SFL", "D10HPL",
	"D10ENF", "D10ENR", "D10NJC", "D10L2Y",
	"D10RLN", "D10ELN", "D10LVR", "D10BRP",
	"D10FCN", "D10ZLB", "D10SDS", "NCRASH"
};

double get_avgRes(Parameters* PARAMS, Statistics* ECSTAT, int varidx) {
	if(avgVar[varidx]=="EXPVAR") return PARAMS->get_expVal();
	if(avgVar[varidx]=="AVGGAP") return ECSTAT->_avLnGp;
	if(avgVar[varidx]=="AVGUNR") return ECSTAT->_avUnRt;
	if(avgVar[varidx]=="AVGVOL") return ECSTAT->_avGpVl;
	if(avgVar[varidx]=="AVGIVL") return ECSTAT->_avPiVl;
	if(avgVar[varidx]=="AVGINF") return exp(ECSTAT->_avCCPi) - 1;
	if(avgVar[varidx]=="AVGBFL") return ECSTAT->_avBkFl;
	if(avgVar[varidx]=="AVGBTR") return ECSTAT->_avBkTr;
	if(avgVar[varidx]=="AVGLSS") return ECSTAT->_avJbLs;
	if(avgVar[varidx]=="AVGLLO") return ECSTAT->_avLsLf;
	if(avgVar[varidx]=="AVGLQT") return ECSTAT->_avLsQt;
	if(avgVar[varidx]=="AVGLEX") return ECSTAT->_avLsEx;
	if(avgVar[varidx]=="AVGUDR") return ECSTAT->_avUnDr;
	if(avgVar[varidx]=="AVGSHP") return ECSTAT->_avWkSh;
	if(avgVar[varidx]=="AVGSFL") return ECSTAT->_avFlHz;
	if(avgVar[varidx]=="AVGHPL") return ECSTAT->_avHpHz;
	if(avgVar[varidx]=="AVGENF") return ECSTAT->_avEnFl;
	if(avgVar[varidx]=="AVGENR") return ECSTAT->_avEnRt;
	if(avgVar[varidx]=="AVGNJC") return ECSTAT->_avNJCR;
	if(avgVar[varidx]=="AVGL2Y") return ECSTAT->_avLn2Y;
	if(avgVar[varidx]=="AVGRLN") return ECSTAT->_avRlLn;
	if(avgVar[varidx]=="AVGELN") return ECSTAT->_avEnLn;
	if(avgVar[varidx]=="AVGLVR") return ECSTAT->_avLVRt;
	if(avgVar[varidx]=="AVGBRP") return ECSTAT->_avBkrp;
	if(avgVar[varidx]=="AVGFCN") return ECSTAT->_avFCon;
	if(avgVar[varidx]=="AVGZLB") return ECSTAT->_avZbRt;
	if(avgVar[varidx]=="AVGSDS") return ECSTAT->_avSlDs;
	if(avgVar[varidx]=="NCRASH") return ECSTAT->_nCrash;

	cout << "You need to add " << avgVar[varidx]
		 << " to the list of names in the get_avgRes()" << endl
		 << "function in the header file ExpResults.h " << endl << endl
		 << "Print any key to continue . . . " << endl;

	return 0;
}

double get_mdnRes(Parameters* PARAMS, Statistics* ECSTAT, int varidx) {
	if(mdnVar[varidx]=="EXPVAR") return PARAMS->get_expVal();
	if(mdnVar[varidx]=="MDNGAP") return ECSTAT->_mdLnGp;
	if(mdnVar[varidx]=="MDNUNR") return ECSTAT->_mdUnRt;
	if(mdnVar[varidx]=="MDNVOL") return ECSTAT->_mdGpVl;
	if(mdnVar[varidx]=="MDNIVL") return ECSTAT->_mdPiVl;
	if(mdnVar[varidx]=="MDNINF") return exp(ECSTAT->_mdCCPi) - 1;
	if(mdnVar[varidx]=="MDNBFL") return ECSTAT->_mdBkFl;
	if(mdnVar[varidx]=="MDNBTR") return ECSTAT->_mdBkTr;
	if(mdnVar[varidx]=="MDNLSS") return ECSTAT->_mdJbLs;
	if(mdnVar[varidx]=="MDNLLO") return ECSTAT->_mdLsLf;
	if(mdnVar[varidx]=="MDNLQT") return ECSTAT->_mdLsQt;
	if(mdnVar[varidx]=="MDNLEX") return ECSTAT->_mdLsEx;
	if(mdnVar[varidx]=="MDNUDR") return ECSTAT->_mdUnDr;
	if(mdnVar[varidx]=="MDNSHP") return ECSTAT->_mdWkSh;
	if(mdnVar[varidx]=="MDNSFL") return ECSTAT->_mdFlHz;
	if(mdnVar[varidx]=="MDNHPL") return ECSTAT->_mdHpHz;
	if(mdnVar[varidx]=="MDNENF") return ECSTAT->_mdEnFl;
	if(mdnVar[varidx]=="MDNENR") return ECSTAT->_mdEnRt;
	if(mdnVar[varidx]=="MDNNJC") return ECSTAT->_mdNJCR;
	if(mdnVar[varidx]=="MDNL2Y") return ECSTAT->_mdLn2Y;
	if(mdnVar[varidx]=="MDNRLN") return ECSTAT->_mdRlLn;
	if(mdnVar[varidx]=="MDNELN") return ECSTAT->_mdEnLn;
	if(mdnVar[varidx]=="MDNLVR") return ECSTAT->_mdLVRt;
	if(mdnVar[varidx]=="MDNBRP") return ECSTAT->_mdBkrp;
	if(mdnVar[varidx]=="MDNFCN") return ECSTAT->_mdFCon;
	if(mdnVar[varidx]=="MDNZLB") return ECSTAT->_mdZbRt;
	if(mdnVar[varidx]=="MDNSDS") return ECSTAT->_mdSlDs;
	if(mdnVar[varidx]=="NCRASH") return ECSTAT->_nCrash;

	cout << "You need to add " << mdnVar[varidx]
		 << " to the list of names in the get_mdnRes()" << endl
		 << "function in the header file ExpResults.h " << endl << endl
		 << "Print any key to continue . . . " << endl;

	return 0;
}

double get_d01Res(Parameters* PARAMS, Statistics* ECSTAT, int varidx) {
	if(d01Var[varidx]=="EXPVAR") return PARAMS->get_expVal();
	if(d01Var[varidx]=="D01GAP") return ECSTAT->_dcLnGp[0];
	if(d01Var[varidx]=="D01UNR") return ECSTAT->_dcUnRt[0];
	if(d01Var[varidx]=="D01VOL") return ECSTAT->_dcGpVl[0];
	if(d01Var[varidx]=="D01IVL") return ECSTAT->_dcPiVl[0];
	if(d01Var[varidx]=="D01INF") return exp(ECSTAT->_dcCCPi[0]) - 1;
	if(d01Var[varidx]=="D01BFL") return ECSTAT->_dcBkFl[0];
	if(d01Var[varidx]=="D01BTR") return ECSTAT->_dcBkTr[0];
	if(d01Var[varidx]=="D01LSS") return ECSTAT->_dcJbLs[0];
	if(d01Var[varidx]=="D01LLO") return ECSTAT->_dcLsLf[0];
	if(d01Var[varidx]=="D01LQT") return ECSTAT->_dcLsQt[0];
	if(d01Var[varidx]=="D01LEX") return ECSTAT->_dcLsEx[0];
	if(d01Var[varidx]=="D01UDR") return ECSTAT->_dcUnDr[0];
	if(d01Var[varidx]=="D01SHP") return ECSTAT->_dcWkSh[0];
	if(d01Var[varidx]=="D01SFL") return ECSTAT->_dcFlHz[0];
	if(d01Var[varidx]=="D01HPL") return ECSTAT->_dcHpHz[0];
	if(d01Var[varidx]=="D01ENF") return ECSTAT->_dcEnFl[0];
	if(d01Var[varidx]=="D01ENR") return ECSTAT->_dcEnRt[0];
	if(d01Var[varidx]=="D01NJC") return ECSTAT->_dcNJCR[0];
	if(d01Var[varidx]=="D01L2Y") return ECSTAT->_dcLn2Y[0];
	if(d01Var[varidx]=="D01RLN") return ECSTAT->_dcRlLn[0];
	if(d01Var[varidx]=="D01ELN") return ECSTAT->_dcEnLn[0];
	if(d01Var[varidx]=="D01LVR") return ECSTAT->_dcLVRt[0];
	if(d01Var[varidx]=="D01BRP") return ECSTAT->_dcBkrp[0];
	if(d01Var[varidx]=="D01FCN") return ECSTAT->_dcFCon[0];
	if(d01Var[varidx]=="D01ZLB") return ECSTAT->_dcZbRt[0];
	if(d01Var[varidx]=="D01SDS") return ECSTAT->_dcSlDs[0];
	if(d01Var[varidx]=="NCRASH") return ECSTAT->_nCrash;

	cout << "You need to add " << d01Var[varidx]
		 << " to the list of names in the get_d01Res()" << endl
		 << "function in the header file ExpResults.h " << endl << endl
		 << "Print any key to continue . . . " << endl;

	return 0;
}

double get_d02Res(Parameters* PARAMS, Statistics* ECSTAT, int varidx) {
	if(d02Var[varidx]=="EXPVAR") return PARAMS->get_expVal();
	if(d02Var[varidx]=="D02GAP") return ECSTAT->_dcLnGp[1];
	if(d02Var[varidx]=="D02UNR") return ECSTAT->_dcUnRt[1];
	if(d02Var[varidx]=="D02VOL") return ECSTAT->_dcGpVl[1];
	if(d02Var[varidx]=="D02IVL") return ECSTAT->_dcPiVl[1];
	if(d02Var[varidx]=="D02INF") return exp(ECSTAT->_dcCCPi[1]) - 1;
	if(d02Var[varidx]=="D02BFL") return ECSTAT->_dcBkFl[1];
	if(d02Var[varidx]=="D02BTR") return ECSTAT->_dcBkTr[1];
	if(d02Var[varidx]=="D02LSS") return ECSTAT->_dcJbLs[1];
	if(d02Var[varidx]=="D02LLO") return ECSTAT->_dcLsLf[1];
	if(d02Var[varidx]=="D02LQT") return ECSTAT->_dcLsQt[1];
	if(d02Var[varidx]=="D02LEX") return ECSTAT->_dcLsEx[1];
	if(d02Var[varidx]=="D02UDR") return ECSTAT->_dcUnDr[1];
	if(d02Var[varidx]=="D02SHP") return ECSTAT->_dcWkSh[1];
	if(d02Var[varidx]=="D02SFL") return ECSTAT->_dcFlHz[1];
	if(d02Var[varidx]=="D02HPL") return ECSTAT->_dcHpHz[1];
	if(d02Var[varidx]=="D02ENF") return ECSTAT->_dcEnFl[1];
	if(d02Var[varidx]=="D02ENR") return ECSTAT->_dcEnRt[1];
	if(d02Var[varidx]=="D02NJC") return ECSTAT->_dcNJCR[1];
	if(d02Var[varidx]=="D02L2Y") return ECSTAT->_dcLn2Y[1];
	if(d02Var[varidx]=="D02RLN") return ECSTAT->_dcRlLn[1];
	if(d02Var[varidx]=="D02ELN") return ECSTAT->_dcEnLn[1];
	if(d02Var[varidx]=="D02LVR") return ECSTAT->_dcLVRt[1];
	if(d02Var[varidx]=="D02BRP") return ECSTAT->_dcBkrp[1];
	if(d02Var[varidx]=="D02FCN") return ECSTAT->_dcFCon[1];
	if(d02Var[varidx]=="D02ZLB") return ECSTAT->_dcZbRt[1];
	if(d02Var[varidx]=="D02SDS") return ECSTAT->_dcSlDs[1];
	if(d02Var[varidx]=="NCRASH") return ECSTAT->_nCrash;

	cout << "You need to add " << d02Var[varidx]
		 << " to the list of names in the get_d02Res()" << endl
		 << "function in the header file ExpResults.h " << endl << endl
		 << "Print any key to continue . . . " << endl;

	return 0;
}

double get_d03Res(Parameters* PARAMS, Statistics* ECSTAT, int varidx) {
	if(d03Var[varidx]=="EXPVAR") return PARAMS->get_expVal();
	if(d03Var[varidx]=="D03GAP") return ECSTAT->_dcLnGp[2];
	if(d03Var[varidx]=="D03UNR") return ECSTAT->_dcUnRt[2];
	if(d03Var[varidx]=="D03VOL") return ECSTAT->_dcGpVl[2];
	if(d03Var[varidx]=="D03IVL") return ECSTAT->_dcPiVl[2];
	if(d03Var[varidx]=="D03INF") return exp(ECSTAT->_dcCCPi[2]) - 1;
	if(d03Var[varidx]=="D03BFL") return ECSTAT->_dcBkFl[2];
	if(d03Var[varidx]=="D03BTR") return ECSTAT->_dcBkTr[2];
	if(d03Var[varidx]=="D03LSS") return ECSTAT->_dcJbLs[2];
	if(d03Var[varidx]=="D03LLO") return ECSTAT->_dcLsLf[2];
	if(d03Var[varidx]=="D03LQT") return ECSTAT->_dcLsQt[2];
	if(d03Var[varidx]=="D03LEX") return ECSTAT->_dcLsEx[2];
	if(d03Var[varidx]=="D03UDR") return ECSTAT->_dcUnDr[2];
	if(d03Var[varidx]=="D03SHP") return ECSTAT->_dcWkSh[2];
	if(d03Var[varidx]=="D03SFL") return ECSTAT->_dcFlHz[2];
	if(d03Var[varidx]=="D03HPL") return ECSTAT->_dcHpHz[2];
	if(d03Var[varidx]=="D03ENF") return ECSTAT->_dcEnFl[2];
	if(d03Var[varidx]=="D03ENR") return ECSTAT->_dcEnRt[2];
	if(d03Var[varidx]=="D03NJC") return ECSTAT->_dcNJCR[2];
	if(d03Var[varidx]=="D03L2Y") return ECSTAT->_dcLn2Y[2];
	if(d03Var[varidx]=="D03RLN") return ECSTAT->_dcRlLn[2];
	if(d03Var[varidx]=="D03ELN") return ECSTAT->_dcEnLn[2];
	if(d03Var[varidx]=="D03LVR") return ECSTAT->_dcLVRt[2];
	if(d03Var[varidx]=="D03BRP") return ECSTAT->_dcBkrp[2];
	if(d03Var[varidx]=="D03FCN") return ECSTAT->_dcFCon[2];
	if(d03Var[varidx]=="D03ZLB") return ECSTAT->_dcZbRt[2];
	if(d03Var[varidx]=="D03SDS") return ECSTAT->_dcSlDs[2];
	if(d03Var[varidx]=="NCRASH") return ECSTAT->_nCrash;

	cout << "You need to add " << d03Var[varidx]
		 << " to the list of names in the get_d03Res()" << endl
		 << "function in the header file ExpResults.h " << endl << endl
		 << "Print any key to continue . . . " << endl;

	return 0;
}

double get_d04Res(Parameters* PARAMS, Statistics* ECSTAT, int varidx) {
	if(d04Var[varidx]=="EXPVAR") return PARAMS->get_expVal();
	if(d04Var[varidx]=="D04GAP") return ECSTAT->_dcLnGp[3];
	if(d04Var[varidx]=="D04UNR") return ECSTAT->_dcUnRt[3];
	if(d04Var[varidx]=="D04VOL") return ECSTAT->_dcGpVl[3];
	if(d04Var[varidx]=="D04IVL") return ECSTAT->_dcPiVl[3];
	if(d04Var[varidx]=="D04INF") return exp(ECSTAT->_dcCCPi[3]) - 1;
	if(d04Var[varidx]=="D04BFL") return ECSTAT->_dcBkFl[3];
	if(d04Var[varidx]=="D04BTR") return ECSTAT->_dcBkTr[3];
	if(d04Var[varidx]=="D04LSS") return ECSTAT->_dcJbLs[3];
	if(d04Var[varidx]=="D04LLO") return ECSTAT->_dcLsLf[3];
	if(d04Var[varidx]=="D04LQT") return ECSTAT->_dcLsQt[3];
	if(d04Var[varidx]=="D04LEX") return ECSTAT->_dcLsEx[3];
	if(d04Var[varidx]=="D04UDR") return ECSTAT->_dcUnDr[3];
	if(d04Var[varidx]=="D04SHP") return ECSTAT->_dcWkSh[3];
	if(d04Var[varidx]=="D04SFL") return ECSTAT->_dcFlHz[3];
	if(d04Var[varidx]=="D04HPL") return ECSTAT->_dcHpHz[3];
	if(d04Var[varidx]=="D04ENF") return ECSTAT->_dcEnFl[3];
	if(d04Var[varidx]=="D04ENR") return ECSTAT->_dcEnRt[3];
	if(d04Var[varidx]=="D04NJC") return ECSTAT->_dcNJCR[3];
	if(d04Var[varidx]=="D04L2Y") return ECSTAT->_dcLn2Y[3];
	if(d04Var[varidx]=="D04RLN") return ECSTAT->_dcRlLn[3];
	if(d04Var[varidx]=="D04ELN") return ECSTAT->_dcEnLn[3];
	if(d04Var[varidx]=="D04LVR") return ECSTAT->_dcLVRt[3];
	if(d04Var[varidx]=="D04BRP") return ECSTAT->_dcBkrp[3];
	if(d04Var[varidx]=="D04FCN") return ECSTAT->_dcFCon[3];
	if(d04Var[varidx]=="D04ZLB") return ECSTAT->_dcZbRt[3];
	if(d04Var[varidx]=="D04SDS") return ECSTAT->_dcSlDs[3];
	if(d04Var[varidx]=="NCRASH") return ECSTAT->_nCrash;

	cout << "You need to add " << d04Var[varidx]
		 << " to the list of names in the get_d04Res()" << endl
		 << "function in the header file ExpResults.h " << endl << endl
		 << "Print any key to continue . . . " << endl;

	return 0;
}

double get_d05Res(Parameters* PARAMS, Statistics* ECSTAT, int varidx) {
	if(d05Var[varidx]=="EXPVAR") return PARAMS->get_expVal();
	if(d05Var[varidx]=="D05GAP") return ECSTAT->_dcLnGp[4];
	if(d05Var[varidx]=="D05UNR") return ECSTAT->_dcUnRt[4];
	if(d05Var[varidx]=="D05VOL") return ECSTAT->_dcGpVl[4];
	if(d05Var[varidx]=="D05IVL") return ECSTAT->_dcPiVl[4];
	if(d05Var[varidx]=="D05INF") return exp(ECSTAT->_dcCCPi[4]) - 1;
	if(d05Var[varidx]=="D05BFL") return ECSTAT->_dcBkFl[4];
	if(d05Var[varidx]=="D05BTR") return ECSTAT->_dcBkTr[4];
	if(d05Var[varidx]=="D05LSS") return ECSTAT->_dcJbLs[4];
	if(d05Var[varidx]=="D05LLO") return ECSTAT->_dcLsLf[4];
	if(d05Var[varidx]=="D05LQT") return ECSTAT->_dcLsQt[4];
	if(d05Var[varidx]=="D05LEX") return ECSTAT->_dcLsEx[4];
	if(d05Var[varidx]=="D05UDR") return ECSTAT->_dcUnDr[4];
	if(d05Var[varidx]=="D05SHP") return ECSTAT->_dcWkSh[4];
	if(d05Var[varidx]=="D05SFL") return ECSTAT->_dcFlHz[4];
	if(d05Var[varidx]=="D05HPL") return ECSTAT->_dcHpHz[4];
	if(d05Var[varidx]=="D05ENF") return ECSTAT->_dcEnFl[4];
	if(d05Var[varidx]=="D05ENR") return ECSTAT->_dcEnRt[4];
	if(d05Var[varidx]=="D05NJC") return ECSTAT->_dcNJCR[4];
	if(d05Var[varidx]=="D05L2Y") return ECSTAT->_dcLn2Y[4];
	if(d05Var[varidx]=="D05RLN") return ECSTAT->_dcRlLn[4];
	if(d05Var[varidx]=="D05ELN") return ECSTAT->_dcEnLn[4];
	if(d05Var[varidx]=="D05LVR") return ECSTAT->_dcLVRt[4];
	if(d05Var[varidx]=="D05BRP") return ECSTAT->_dcBkrp[4];
	if(d05Var[varidx]=="D05FCN") return ECSTAT->_dcFCon[4];
	if(d05Var[varidx]=="D05ZLB") return ECSTAT->_dcZbRt[4];
	if(d05Var[varidx]=="D05SDS") return ECSTAT->_dcSlDs[4];
	if(d05Var[varidx]=="NCRASH") return ECSTAT->_nCrash;

	cout << "You need to add " << d05Var[varidx]
		 << " to the list of names in the get_d05Res()" << endl
		 << "function in the header file ExpResults.h " << endl << endl
		 << "Print any key to continue . . . " << endl;

	return 0;
}

double get_d06Res(Parameters* PARAMS, Statistics* ECSTAT, int varidx) {
	if(d06Var[varidx]=="EXPVAR") return PARAMS->get_expVal();
	if(d06Var[varidx]=="D06GAP") return ECSTAT->_dcLnGp[5];
	if(d06Var[varidx]=="D06UNR") return ECSTAT->_dcUnRt[5];
	if(d06Var[varidx]=="D06VOL") return ECSTAT->_dcGpVl[5];
	if(d06Var[varidx]=="D06IVL") return ECSTAT->_dcPiVl[5];
	if(d06Var[varidx]=="D06INF") return exp(ECSTAT->_dcCCPi[5]) - 1;
	if(d06Var[varidx]=="D06BFL") return ECSTAT->_dcBkFl[5];
	if(d06Var[varidx]=="D06BTR") return ECSTAT->_dcBkTr[5];
	if(d06Var[varidx]=="D06LSS") return ECSTAT->_dcJbLs[5];
	if(d06Var[varidx]=="D06LLO") return ECSTAT->_dcLsLf[5];
	if(d06Var[varidx]=="D06LQT") return ECSTAT->_dcLsQt[5];
	if(d06Var[varidx]=="D06LEX") return ECSTAT->_dcLsEx[5];
	if(d06Var[varidx]=="D06UDR") return ECSTAT->_dcUnDr[5];
	if(d06Var[varidx]=="D06SHP") return ECSTAT->_dcWkSh[5];
	if(d06Var[varidx]=="D06SFL") return ECSTAT->_dcFlHz[5];
	if(d06Var[varidx]=="D06HPL") return ECSTAT->_dcHpHz[5];
	if(d06Var[varidx]=="D06ENF") return ECSTAT->_dcEnFl[5];
	if(d06Var[varidx]=="D06ENR") return ECSTAT->_dcEnRt[5];
	if(d06Var[varidx]=="D06NJC") return ECSTAT->_dcNJCR[5];
	if(d06Var[varidx]=="D06L2Y") return ECSTAT->_dcLn2Y[5];
	if(d06Var[varidx]=="D06RLN") return ECSTAT->_dcRlLn[5];
	if(d06Var[varidx]=="D06ELN") return ECSTAT->_dcEnLn[5];
	if(d06Var[varidx]=="D06LVR") return ECSTAT->_dcLVRt[5];
	if(d06Var[varidx]=="D06BRP") return ECSTAT->_dcBkrp[5];
	if(d06Var[varidx]=="D06FCN") return ECSTAT->_dcFCon[5];
	if(d06Var[varidx]=="D06ZLB") return ECSTAT->_dcZbRt[5];
	if(d06Var[varidx]=="D06SDS") return ECSTAT->_dcSlDs[5];
	if(d06Var[varidx]=="NCRASH") return ECSTAT->_nCrash;

	cout << "You need to add " << d06Var[varidx]
		 << " to the list of names in the get_d06Res()" << endl
		 << "function in the header file ExpResults.h " << endl << endl
		 << "Print any key to continue . . . " << endl;

	return 0;
}

double get_d07Res(Parameters* PARAMS, Statistics* ECSTAT, int varidx) {
	if(d07Var[varidx]=="EXPVAR") return PARAMS->get_expVal();
	if(d07Var[varidx]=="D07GAP") return ECSTAT->_dcLnGp[6];
	if(d07Var[varidx]=="D07UNR") return ECSTAT->_dcUnRt[6];
	if(d07Var[varidx]=="D07VOL") return ECSTAT->_dcGpVl[6];
	if(d07Var[varidx]=="D07IVL") return ECSTAT->_dcPiVl[6];
	if(d07Var[varidx]=="D07INF") return exp(ECSTAT->_dcCCPi[6]) - 1;
	if(d07Var[varidx]=="D07BFL") return ECSTAT->_dcBkFl[6];
	if(d07Var[varidx]=="D07BTR") return ECSTAT->_dcBkTr[6];
	if(d07Var[varidx]=="D07LSS") return ECSTAT->_dcJbLs[6];
	if(d07Var[varidx]=="D07LLO") return ECSTAT->_dcLsLf[6];
	if(d07Var[varidx]=="D07LQT") return ECSTAT->_dcLsQt[6];
	if(d07Var[varidx]=="D07LEX") return ECSTAT->_dcLsEx[6];
	if(d07Var[varidx]=="D07UDR") return ECSTAT->_dcUnDr[6];
	if(d07Var[varidx]=="D07SHP") return ECSTAT->_dcWkSh[6];
	if(d07Var[varidx]=="D07SFL") return ECSTAT->_dcFlHz[6];
	if(d07Var[varidx]=="D07HPL") return ECSTAT->_dcHpHz[6];
	if(d07Var[varidx]=="D07ENF") return ECSTAT->_dcEnFl[6];
	if(d07Var[varidx]=="D07ENR") return ECSTAT->_dcEnRt[6];
	if(d07Var[varidx]=="D07NJC") return ECSTAT->_dcNJCR[6];
	if(d07Var[varidx]=="D07L2Y") return ECSTAT->_dcLn2Y[6];
	if(d07Var[varidx]=="D07RLN") return ECSTAT->_dcRlLn[6];
	if(d07Var[varidx]=="D07ELN") return ECSTAT->_dcEnLn[6];
	if(d07Var[varidx]=="D07LVR") return ECSTAT->_dcLVRt[6];
	if(d07Var[varidx]=="D07BRP") return ECSTAT->_dcBkrp[6];
	if(d07Var[varidx]=="D07FCN") return ECSTAT->_dcFCon[6];
	if(d07Var[varidx]=="D07ZLB") return ECSTAT->_dcZbRt[6];
	if(d07Var[varidx]=="D07SDS") return ECSTAT->_dcSlDs[6];
	if(d07Var[varidx]=="NCRASH") return ECSTAT->_nCrash;

	cout << "You need to add " << d07Var[varidx]
		 << " to the list of names in the get_d07Res()" << endl
		 << "function in the header file ExpResults.h " << endl << endl
		 << "Print any key to continue . . . " << endl;

	return 0;
}

double get_d08Res(Parameters* PARAMS, Statistics* ECSTAT, int varidx) {
	if(d08Var[varidx]=="EXPVAR") return PARAMS->get_expVal();
	if(d08Var[varidx]=="D08GAP") return ECSTAT->_dcLnGp[7];
	if(d08Var[varidx]=="D08UNR") return ECSTAT->_dcUnRt[7];
	if(d08Var[varidx]=="D08VOL") return ECSTAT->_dcGpVl[7];
	if(d08Var[varidx]=="D08IVL") return ECSTAT->_dcPiVl[7];
	if(d08Var[varidx]=="D08INF") return exp(ECSTAT->_dcCCPi[7]) - 1;
	if(d08Var[varidx]=="D08BFL") return ECSTAT->_dcBkFl[7];
	if(d08Var[varidx]=="D08BTR") return ECSTAT->_dcBkTr[7];
	if(d08Var[varidx]=="D08LSS") return ECSTAT->_dcJbLs[7];
	if(d08Var[varidx]=="D08LLO") return ECSTAT->_dcLsLf[7];
	if(d08Var[varidx]=="D08LQT") return ECSTAT->_dcLsQt[7];
	if(d08Var[varidx]=="D08LEX") return ECSTAT->_dcLsEx[7];
	if(d08Var[varidx]=="D08UDR") return ECSTAT->_dcUnDr[7];
	if(d08Var[varidx]=="D08SHP") return ECSTAT->_dcWkSh[7];
	if(d08Var[varidx]=="D08SFL") return ECSTAT->_dcFlHz[7];
	if(d08Var[varidx]=="D08HPL") return ECSTAT->_dcHpHz[7];
	if(d08Var[varidx]=="D08ENF") return ECSTAT->_dcEnFl[7];
	if(d08Var[varidx]=="D08ENR") return ECSTAT->_dcEnRt[7];
	if(d08Var[varidx]=="D08NJC") return ECSTAT->_dcNJCR[7];
	if(d08Var[varidx]=="D08L2Y") return ECSTAT->_dcLn2Y[7];
	if(d08Var[varidx]=="D08RLN") return ECSTAT->_dcRlLn[7];
	if(d08Var[varidx]=="D08ELN") return ECSTAT->_dcEnLn[7];
	if(d08Var[varidx]=="D08LVR") return ECSTAT->_dcLVRt[7];
	if(d08Var[varidx]=="D08BRP") return ECSTAT->_dcBkrp[7];
	if(d08Var[varidx]=="D08FCN") return ECSTAT->_dcFCon[7];
	if(d08Var[varidx]=="D08ZLB") return ECSTAT->_dcZbRt[7];
	if(d08Var[varidx]=="D08SDS") return ECSTAT->_dcSlDs[7];
	if(d08Var[varidx]=="NCRASH") return ECSTAT->_nCrash;

	cout << "You need to add " << d08Var[varidx]
		 << " to the list of names in the get_d08Res()" << endl
		 << "function in the header file ExpResults.h " << endl << endl
		 << "Print any key to continue . . . " << endl;

	return 0;
}

double get_d09Res(Parameters* PARAMS, Statistics* ECSTAT, int varidx) {
	if(d09Var[varidx]=="EXPVAR") return PARAMS->get_expVal();
	if(d09Var[varidx]=="D09GAP") return ECSTAT->_dcLnGp[8];
	if(d09Var[varidx]=="D09UNR") return ECSTAT->_dcUnRt[8];
	if(d09Var[varidx]=="D09VOL") return ECSTAT->_dcGpVl[8];
	if(d09Var[varidx]=="D09IVL") return ECSTAT->_dcPiVl[8];
	if(d09Var[varidx]=="D09INF") return exp(ECSTAT->_dcCCPi[8]) - 1;
	if(d09Var[varidx]=="D09BFL") return ECSTAT->_dcBkFl[8];
	if(d09Var[varidx]=="D09BTR") return ECSTAT->_dcBkTr[8];
	if(d09Var[varidx]=="D09LSS") return ECSTAT->_dcJbLs[8];
	if(d09Var[varidx]=="D09LLO") return ECSTAT->_dcLsLf[8];
	if(d09Var[varidx]=="D09LQT") return ECSTAT->_dcLsQt[8];
	if(d09Var[varidx]=="D09LEX") return ECSTAT->_dcLsEx[8];
	if(d09Var[varidx]=="D09UDR") return ECSTAT->_dcUnDr[8];
	if(d09Var[varidx]=="D09SHP") return ECSTAT->_dcWkSh[8];
	if(d09Var[varidx]=="D09SFL") return ECSTAT->_dcFlHz[8];
	if(d09Var[varidx]=="D09HPL") return ECSTAT->_dcHpHz[8];
	if(d09Var[varidx]=="D09ENF") return ECSTAT->_dcEnFl[8];
	if(d09Var[varidx]=="D09ENR") return ECSTAT->_dcEnRt[8];
	if(d09Var[varidx]=="D09NJC") return ECSTAT->_dcNJCR[8];
	if(d09Var[varidx]=="D09L2Y") return ECSTAT->_dcLn2Y[8];
	if(d09Var[varidx]=="D09RLN") return ECSTAT->_dcRlLn[8];
	if(d09Var[varidx]=="D09ELN") return ECSTAT->_dcEnLn[8];
	if(d09Var[varidx]=="D09LVR") return ECSTAT->_dcLVRt[8];
	if(d09Var[varidx]=="D09BRP") return ECSTAT->_dcBkrp[8];
	if(d09Var[varidx]=="D09FCN") return ECSTAT->_dcFCon[8];
	if(d09Var[varidx]=="D09ZLB") return ECSTAT->_dcZbRt[8];
	if(d09Var[varidx]=="D09SDS") return ECSTAT->_dcSlDs[8];
	if(d09Var[varidx]=="NCRASH") return ECSTAT->_nCrash;

	cout << "You need to add " << d09Var[varidx]
		 << " to the list of names in the get_d09Res()" << endl
		 << "function in the header file ExpResults.h " << endl << endl
		 << "Print any key to continue . . . " << endl;

	return 0;
}

double get_d10Res(Parameters* PARAMS, Statistics* ECSTAT, int varidx) {
	if(d10Var[varidx]=="EXPVAR") return PARAMS->get_expVal();
	if(d10Var[varidx]=="D10GAP") return ECSTAT->_dcLnGp[9];
	if(d10Var[varidx]=="D10UNR") return ECSTAT->_dcUnRt[9];
	if(d10Var[varidx]=="D10VOL") return ECSTAT->_dcGpVl[9];
	if(d10Var[varidx]=="D10IVL") return ECSTAT->_dcPiVl[9];
	if(d10Var[varidx]=="D10INF") return exp(ECSTAT->_dcCCPi[9]) - 1;
	if(d10Var[varidx]=="D10BFL") return ECSTAT->_dcBkFl[9];
	if(d10Var[varidx]=="D10BTR") return ECSTAT->_dcBkTr[9];
	if(d10Var[varidx]=="D10LSS") return ECSTAT->_dcJbLs[9];
	if(d10Var[varidx]=="D10LLO") return ECSTAT->_dcLsLf[9];
	if(d10Var[varidx]=="D10LQT") return ECSTAT->_dcLsQt[9];
	if(d10Var[varidx]=="D10LEX") return ECSTAT->_dcLsEx[9];
	if(d10Var[varidx]=="D10UDR") return ECSTAT->_dcUnDr[9];
	if(d10Var[varidx]=="D10SHP") return ECSTAT->_dcWkSh[9];
	if(d10Var[varidx]=="D10SFL") return ECSTAT->_dcFlHz[9];
	if(d10Var[varidx]=="D10HPL") return ECSTAT->_dcHpHz[9];
	if(d10Var[varidx]=="D10ENF") return ECSTAT->_dcEnFl[9];
	if(d10Var[varidx]=="D10ENR") return ECSTAT->_dcEnRt[9];
	if(d10Var[varidx]=="D10NJC") return ECSTAT->_dcNJCR[9];
	if(d10Var[varidx]=="D10L2Y") return ECSTAT->_dcLn2Y[9];
	if(d10Var[varidx]=="D10RLN") return ECSTAT->_dcRlLn[9];
	if(d10Var[varidx]=="D10ELN") return ECSTAT->_dcEnLn[9];
	if(d10Var[varidx]=="D10LVR") return ECSTAT->_dcLVRt[9];
	if(d10Var[varidx]=="D10BRP") return ECSTAT->_dcBkrp[9];
	if(d10Var[varidx]=="D10FCN") return ECSTAT->_dcFCon[9];
	if(d10Var[varidx]=="D10ZLB") return ECSTAT->_dcZbRt[9];
	if(d10Var[varidx]=="D10SDS") return ECSTAT->_dcSlDs[9];
	if(d10Var[varidx]=="NCRASH") return ECSTAT->_nCrash;

	cout << "You need to add " << d10Var[varidx]
		 << " to the list of names in the get_d10Res()" << endl
		 << "function in the header file ExpResults.h " << endl << endl
		 << "Print any key to continue . . . " << endl;

	return 0;
}

#endif // EXPRESULTS_H