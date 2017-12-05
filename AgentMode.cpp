/***********************************************************************************************
 * AgentMode.cpp
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised July 9 2014
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/
#include "AgentMode.h"
#include "Agent.h"
#include "Firesale.h"
#include "Bank.h"
#include "Shop.h"
#include "Economy.h"

AgentMode::AgentMode() {}

AgentMode::~AgentMode() {}

WorkerMan::WorkerMan() {}

WorkerMan::~WorkerMan() {}

ShopOwner::ShopOwner() {}

ShopOwner::~ShopOwner() {}

BankOwner::BankOwner() {}

BankOwner::~BankOwner() {}

double& AgentMode::fr_capital(int gIndex, Agent* mdAgnt) {
  return mdAgnt->_legCap[gIndex];
}
double& BankOwner::fr_capital(int gIndex, Agent* mdAgnt) {
  return mdAgnt->_fmBank->_frcCap[gIndex];
}

void AgentMode::inc_depHld(double amount, Agent* mdAgnt) {
  mdAgnt->inc_depHld(amount);
}
void BankOwner::inc_depHld(double amount, Agent* mdAgnt) {
  mdAgnt->_fmBank->inc_reserv(amount);
}

void AgentMode::job_search(Agent* mdAgnt) {
  Agent* cm = mdAgnt->get_comrad();
	Shop*  sh = cm->_spShop;
  double spComp = (cm->_fmShop ? sh->get_salary() / mdAgnt->_agEcon->get_PITRGW() : cm->_spEffc);
  if (sh->is_hiring() && sh != mdAgnt->_spShop && mdAgnt->_spEffc < spComp)
    mdAgnt->start_aJob(sh, spComp);
}
void ShopOwner::job_search(Agent* mdAgnt) {
  return;
}

const double AgentMode::get_wealth(Agent* mdAgnt) {
  return mdAgnt->_monHld + mdAgnt->_depHld - mdAgnt->_bkLoan;
}
const double BankOwner::get_wealth(Agent* mdAgnt) {
  return mdAgnt->_monHld + mdAgnt->_fmBank->get_buffer();
}

const double AgentMode::get_income(Agent* mdAgnt) {
  return mdAgnt->_spEffc;
}
const double ShopOwner::get_income(Agent* mdAgnt) {
  return mdAgnt->_fmShop->_profit;
}

const double AgentMode::get_finLim(double wealth, Agent* mdAgnt) {
  return wealth;
}
const double BankOwner::get_finLim(double wealth, Agent* mdAgnt) {
  if (mdAgnt->_fmBank->is_troubld())
    return mdAgnt->_monHld;
  else
    return wealth;
}
const double ShopOwner::get_finLim(double wealth, Agent* mdAgnt) {
  if(!mdAgnt->_credit && !mdAgnt->_agEcon->get_NOBANK())
    mdAgnt->_credit = mdAgnt->_agBank->opn_crLine(mdAgnt);
  return wealth + mdAgnt->get_cLimit();
}

const double AgentMode::get_bizFin(Agent* mdAgnt) {
  return 0;
}
const double ShopOwner::get_bizFin(Agent* mdAgnt) {
  return mdAgnt->_fmShop->upd_trWgBl();
}

void AgentMode::be_frclosd(Agent* mdAgnt) {
  return;
}
void ShopOwner::be_frclosd(Agent* mdAgnt) {
  mdAgnt->be_frclosd();
}

void AgentMode::scale_back(double bizFin, Agent* mdAgnt) {
  mdAgnt->_plnExp = min(mdAgnt->_plnExp, mdAgnt->_monHld);
}
void ShopOwner::scale_back(double bizFin, Agent* mdAgnt) {
  mdAgnt->_plnExp = max(mdAgnt->_monHld - bizFin, 0);
}
