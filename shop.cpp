/***********************************************************************************************
 * Shop.cpp
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised Dec 30 2014
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

#include "Agent.h"
#include "Shop.h"
#include "Parameters.h"
#include "RandUtils.h"
#include "Economy.h"
#include "Bank.h"
#include "AgentMode.h"

Shop::Shop(int shopID, Parameters* params, Economy* inEcon) {

    _shEcon = inEcon;
    _shopID = shopID;

    _flexWg = params->_FLEXWG;
    _fxdCst = params->_FXDCST;
    _invSpd = params->_INVSPD;
    _wagSpd = params->_WAGSPD;
    _lTrigg = params->_TRIGGR;
    _triggr = params->_TRIGGR;
    _pDelta = params->_PDELTA;
    _cntPrd = params->_CNTPRD;
    _failRt = params->_FAILRT;
    _brkFrc = params->_BRKFRC;
    _piExpY = params->_PITRGY; 
    _piExpW = pow(_piExpY, 1.0 / params->_WKSPYR);
    _cntPrY = (double)_cntPrd / (double) params->_WKSPYR;
  
    _active = false;
    _shGood = 0;
    _salAge = 0;
    _census = 0;
    _iniShp = 0;
    _entrWk = 0;
    _onsale = 0;
    _gouger = 0;
    _sector = 0;

    _salary = 0.0;
    _retail = 0.0;
    _aSales = 0.0;
    _aInput = 0.0;
    _tSales = 0.0;
    _tInput = 0.0;
    _pInput = 0.0;
    _markup = 0.0;
    _invent = 0.0;
    _profit = 0.0;
    _avgdem = 0.0;
    _avgsup = 0.0;

    _shOwnr = NULL;
}

Shop::Shop(int shopID, Shop* sOther, int NGOODS) {

    _shEcon = sOther->_shEcon;
    _flexWg = sOther->_flexWg;
    _fxdCst = sOther->_fxdCst;
    _invSpd = sOther->_invSpd;
    _wagSpd = sOther->_wagSpd;
    _lTrigg = sOther->_lTrigg;
    _triggr = sOther->_triggr;
    _pDelta = sOther->_pDelta;
    _cntPrd = sOther->_cntPrd;
    _failRt = sOther->_failRt;
    _brkFrc = sOther->_brkFrc;
    _piExpY = sOther->_piExpY;
    _piExpW = sOther->_piExpW;
    _cntPrY = sOther->_cntPrY;
  
    _active = false;
    _shGood = 0;
    _salAge = 0;
    _census = 0;
    _iniShp = 0;
    _entrWk = 0;
    _onsale = 0;
    _gouger = 0;
    _sector = 0;

    _salary = 0.0;
    _retail = 0.0;
    _aSales = 0.0;
    _aInput = 0.0;
    _tSales = 0.0;
    _tInput = 0.0;
    _pInput = 0.0;
    _markup = 0.0;
    _invent = 0.0;
    _profit = 0.0;
    _avgdem = 0.0;
    _avgsup = 0.0;

    _shOwnr = NULL;
    _shopID = shopID;
}

Shop::~Shop() {
    _shOwnr = NULL;
    _shEcon = NULL;
}

void Shop::clr_params(int NGOODS) {
    _active = false;
    _shGood = 0;
    _salAge = 0;
    _census = 0;
    _iniShp = 0;
    _entrWk = 0;
    _onsale = 0;
    _gouger = 0;
    _sector = 0;

    _salary = 0.0;
    _retail = 0.0;
    _aSales = 0.0;
    _aInput = 0.0;
    _avgdem = 0.0;
    _avgsup = 0.0;
    _tSales = 0.0;
    _tInput = 0.0;
    _pInput = 0.0;
    _markup = 0.0;
    _invent = 0.0;
    _profit = 0.0;

    _shOwnr = NULL;
}

void Shop::ini_markup(double markup) {
    _markup = markup;
}

void Shop::upd_tSales() {
    _tSales = _aSales;
}

const int Shop::upd_prices() {
    _avgdem += min(_tInput, 1 + (_shOwnr->get_wealth() + _shOwnr->get_cLimit()) / _salary) / (double) _cntPrd;
    _avgsup += _pInput / (double) _cntPrd;
    _salAge ++;

    int change = 0;
    if (_salAge==_cntPrd) {   		// yearly wage change
        _salAge = 0;
        double tYrInc = 1.0 + _wagSpd * ( _avgdem / max(_avgsup,_fxdCst) - 1.0);
        double actInc = pow( tYrInc * _piExpY, _cntPrY);

        if (actInc > 1.0 || _profit < 0 || _flexWg)  /* DNWR if !FLEXWG */
            _salary *= actInc;
        _avgdem = 0.0;
        _avgsup = 0.0;
    }

    double normal = (1-_shEcon->get_txRate()) / (_salary*_markup);
                                                        // new price if normal markup applied
    double salepr = normal * _pDelta;            // sale price
    double highpr = normal / _pDelta;            // high price

    if (_onsale==0 && _invent > _triggr * _aSales)
        _onsale=1;	                                // start a sale
    if (_onsale==1 && _invent <= _aSales)
        _onsale=0;	                                // end a sale
    if (_gouger==0 && _invent*_triggr < _aSales)
        _gouger=1;	                                // start gouging
    if (_gouger==1 && _invent >= _aSales)
        _gouger=0;	                                // stop gouging 

    if (_onsale==1 && _retail < salepr) {
        change = 1;  _retail = salepr; }       // change to sale price
    if (_gouger==1 && _retail > highpr) {			
        change = 1;  _retail = highpr; }       // change to high price
    if (_onsale==0 && _gouger==0 && _retail != normal) {
        change = 1;  _retail = normal; }           // change to normal price
    return change;
}

void Shop::ini_nwShop(Agent* shOwnr) {
    //Without turnover, we initialize random markups based on the distribution
    //at the end of a calibration run, which has a stdev around 1/30.
    if(_shEcon->get_TRNOVR() == 0) {
        double a = _shEcon->get_STDMKP() * sqrt(3.0) / 2; // Radius of uniform distrib yielding stdev = _STDMKP
        _markup = _shEcon->get_RNDMRK() ? _shEcon->get_MARKUP() + a * (1 - 2.0 * RandUtils::get_RnFrac()) : _shEcon->get_MARKUP();
    }
    else
    _markup = _shEcon->get_RNDMRK() ? 1 + RandUtils::get_RnFrac() * (_shEcon->get_MARKUP() - 1) * 2.0 : _shEcon->get_MARKUP();
    _active =  true;
    _shGood =  shOwnr->get_spGood();
    _sector =  (_shGood-1)*_shEcon->get_NBANKS()/_shEcon->get_NGOODS();
    _salAge =  RandUtils::get_RnLess(_cntPrd);
    _census =  1;
    _iniShp =  1;
    _onsale =  0;
    _gouger =  0;
    _aSales = 0;
    _tSales =  (_shEcon->get_NGOODS() - 2.0 - _fxdCst);
    if(_shEcon->get_GAPTRG() > 0.0)
        _tSales *= exp(-_shEcon->get_GAPTRG());
    _invent = _tSales;
    _aInput = _fxdCst + _tSales;

    _tInput = _aInput;
    _pInput = _aInput;
    _avgsup = _pInput * (1.0 - (double) (_salAge + 1.0)/ (double) _cntPrd);
    _avgdem = _tInput * (1.0 - (double) (_salAge + 1.0)/ (double) _cntPrd);
    _salary =  _shEcon->get_wgRate() * _piExpW;
    _retail =  ( ( 1.0 - _shEcon->get_txRate() ) / _markup ) / _piExpW;
    _profit =  ( _markup - _shEcon->get_wkIntr() ) * (_tInput - _fxdCst) - _shEcon->get_wkIntr() * (_fxdCst-1);
    _shOwnr =  shOwnr;
}

void Shop::opn_fmShop(BusinessPlan* bsPlan, Agent* shOwnr) {  
    _shOwnr = shOwnr;
    _invent = _shOwnr->leg_to_inv();
    _active = true;
    _onsale = 0;
    _gouger = 0;
    _shGood = _shOwnr->get_spGood();
    _sector = _shOwnr->get_sector();
    _salAge = 0;
    _entrWk = _shEcon->get_wkCntr();
    _markup = bsPlan->_markup;
    _tSales = bsPlan->_tSales;
    _tInput = _tSales + _fxdCst + _invSpd * (_tSales/_shOwnr->get_freqTr() - _invent);
    _salary = bsPlan->_salary;
    _retail = bsPlan->_retail;
    _profit = bsPlan->_trPrft;
}

const bool Shop::cls_fmShop(double wgRate) {
    bool bkrupt = _shOwnr->cls_fmShop(_invent, wgRate);
    _active = false;
    _shGood = 0;
    _salAge = 0;
    _census = 0;
    _iniShp = 0;
    _onsale = 0;
    _gouger = 0;
    _entrWk = 0;
    _sector = 0;
    _markup = 0.0;
    _salary = 0.0;
    _retail = 0.0;
    _aSales = 0.0;
    _aInput = 0.0;
    _pInput = 0.0;
    _avgdem = 0.0;
    _avgsup = 0.0;
    _tSales = 0.0;
    _tInput = 0.0;
    _invent = 0.0;
    _profit = 0.0;

    _shOwnr = NULL;
    return bkrupt;
}

ostream& operator <<(ostream &stream, Shop* object) {
    stream << "shopID: " << object->_shopID << "\n";
    stream << "active: " << object->_active << "\n";
    stream << "shGood: " << object->_shGood << "\n";
    stream << "salAge: " << object->_salAge << "\n";
    stream << "census: " << object->_census << "\n";
    stream << "entrWk: " << object->_entrWk << "\n";
    stream << "inishp: " << object->_iniShp << "\n";
    stream << "salary: " << object->_salary << "\n";
    stream << "retail: " << object->_retail << "\n";
    stream << "aSales: " << object->_aSales << "\n";
    stream << "aInput: " << object->_aInput << "\n";
    stream << "avgdem: " << object->_avgdem << "\n";
    stream << "avgsup: " << object->_avgsup << "\n";
    stream << "tSales: " << object->_tSales << "\n";
    stream << "tInput: " << object->_tInput << "\n";
    stream << "markup: " << object->_markup << "\n";
    stream << "invent: " << object->_invent << "\n";
    stream << "profit: " << object->get_profit() << "\n";
    stream << "onsale: " << object->_onsale << "\n";
    stream << "gouger: " << object->_gouger << "\n";

    stream << "brkFrc: " << object->_brkFrc << "\n";
    stream << "cntPrd: " << object->_cntPrd << "\n";
    stream << "cntPrY: " << object->_cntPrY << "\n";
    stream << "failRt: " << object->_failRt << "\n";
    stream << "flexWg: " << object->_flexWg << "\n";
    stream << "fxdCst: " << object->_fxdCst << "\n";
    stream << "invSpd: " << object->_invSpd << "\n";
    stream << "lTrigg: " << object->_lTrigg << "\n";
    stream << "pDelta: " << object->_pDelta << "\n";
    stream << "piExpW: " << object->_piExpW << "\n";
    stream << "piExpY: " << object->_piExpY << "\n";
    stream << "pInput: " << object->_pInput << "\n";
    stream << "sector: " << object->_sector << "\n";
    stream << "shOwnr: "; if (object->_active) stream << object->_shOwnr->get_agntID(); stream << "\n";
    stream << "triggr: " << object->_triggr << "\n";
    stream << "wagSpd: " << object->_wagSpd << "\n";

    stream << "\n";
    return stream;
}

const double Shop::upd_trWgBl() {
    _tInput = _tSales + _fxdCst + _invSpd * (_tSales/_shOwnr->get_freqTr() - _invent);
    _tInput = max(_tInput, 0.0);
    return max(_tInput - 1.0 , 0.0) * _salary;
}

void Shop::prep_Trade() {
    if ( (!_active) || _shOwnr->is_bankrpt())
        return;
    if ( _tSales - _invent > 0) {
        double bought = _shOwnr->buy_invent(_tSales - _invent);
        _invent += bought;
        _tInput -= _invSpd*bought;
    }
    _aInput = 1.0;	// shopkeeper applies own input before trading starts
    _pInput = 1.0;
}

void Shop::dbg_chkNeg() {
    if (!_active)
        return;
    if (_retail < 0.0 || _invent < 0.0 || _tSales < 0.0 || _avgdem < 0.0 ||
      _tInput < 0.0 || _salary < 0.0 || _avgdem < 0.0 || _avgsup < 0.0) {
        WaitForSingleObject(CSLMTX, INFINITE);
        cout << "_wkCntr: " << _shEcon->get_wkCntr() << "  _PITRGY: " << _piExpY << endl;
        cout << this;
        getchar();
        ReleaseMutex(CSLMTX);
    }
}

const int Shop::pln_toExit(double wgRate, int shocks, double hcutPr) { // returns 0 if not exiting, 1 if hopeless, 2 if exiting for other reasons
    if( /*RandUtils::get_RnFrac() <= _failRt &&*/
      _shOwnr->get_wealth()+_shOwnr->get_cLimit() < _salary*(_fxdCst-1) )
        return 1; // hopeless - even borrowing to the max, won't be able to pay fixed cost
    if (RandUtils::get_RnFrac() <= _failRt &&
      _shOwnr->not_profit(_invent, wgRate) )
        return 2; // not profitable
    if (shocks && RandUtils::get_RnFrac() <= _brkFrc)
        return 2; // random exogenous exit;
    if (_shOwnr->is_bankrpt())
        return 2; // exit due to bankruptcy;
    return 0;
}

const double Shop::get_spEffc() const {
    return min(_salary, _shOwnr->get_monHld());
}
  
const bool Shop::is_firing() const {
    return (_invent > _lTrigg * _tSales && _aInput > _tInput-1);
}
          
void Shop::employ_one() {
    _aInput += 1.0;
    if(_aInput > _fxdCst)
        _invent += min(_aInput - _fxdCst, 1.0);
}

void Shop::inc_pInput() {
    _pInput ++;
}

const double Shop::upd_accnts() {
    if (!_active) return 0;
        return _profit = max(_aInput - _fxdCst, 0.0) * (1 - _shEcon->get_txRate()) / _retail
                              - _shEcon->get_wkIntr() * (_aInput - 1.0) * _salary
                - (_shOwnr->get_agBank()->get_lnIntr() - _shEcon->get_wkIntr()) * _shOwnr->get_bkLoan();
}

void Shop::upd_natInc(double& rlIncm, double& nmIncm) {
    if (!_active) return;
    rlIncm += (_aInput > _fxdCst) ? _aInput - _fxdCst : 0.0;
    if (_retail > 0.0 && _aInput > _fxdCst)
        nmIncm += (_aInput - _fxdCst) / _retail;
}

void Shop::upd_wgStat(double& tlWage, double& tlEmpl) {
    if (!_active) return;
    tlWage += (_aInput - 1.0) * _salary;
    tlEmpl += (_aInput - 1.0);
}

const double Shop::upd_joStat() {
    if (!_active || _tInput <= _aInput || _invent > _lTrigg * _tSales)
        return 0;
    return _tInput - _aInput;
}

void Shop::dcr_invent(double amount) {
    _invent -= amount;
}

void Shop::lose_stock(Bank* agBank) {
    agBank->seize_good(_shGood-1, _invent);
    _invent = 0;
}

const bool Shop::is_active() const {return this ? _active : false;}

const bool Shop::is_hiring() const {return (is_active() && (_aInput <= _tInput));}

const double Shop::get_profit() const {return this ? _profit : 0;}

const double Shop::get_markup() const {return this ? _markup : 0;}

const double Shop::get_aSales() const {return this ? _aSales : 0;}

const double Shop::get_aInput() const {return this ? _aInput : 0;}

const double Shop::get_salary() const {return this ? _salary : 0;}

const double Shop::get_retail() const {return this ? _retail : 0;}

const double Shop::get_invent() const {return _invent;}

const double Shop::get_shopID() const {return this ? _shopID : 0;}

const double Shop::was_iniShp() const {return _iniShp;}

const double Shop::get_tSales() const {return this ? _tSales : 0;}

const double Shop::get_fxdCst() const {return this ? _fxdCst : 0;}

const double Shop::get_invSpd() const {return this ? _invSpd : 0;}

const int Shop::get_shGood() const {return this ? _shGood : 0;}

const int Shop::get_census() const {return this ? _census : 0;}

const int Shop::get_entrWk() const {return this ? _entrWk : 0;}

Agent* Shop::get_shOwnr() {return this ? _shOwnr : NULL;}

void Shop::set_census(int number) {_census = number;}

void Shop::set_aSales(double aSales) {_aSales = aSales;}

void Shop::inc_aSales(double nwSale) {_aSales += nwSale;} 