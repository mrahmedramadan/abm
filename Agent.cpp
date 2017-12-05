/***********************************************************************************************
 * Agent.cpp
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised July 25 2016
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

#include "Agent.h"
#include "AgentMode.h"
#include "Economy.h"
#include "RandUtils.h"
#include "Shop.h"
#include "Bank.h"
#include "require.h"
#include "Firesale.h"

Agent::Agent(int agntID, int spGood, int d1Good, Economy* inEcon) :
    _piExpW(inEcon->get_PITRGW()),
    _piExpY(inEcon->get_PITRGY()),
    _SPNDPP(inEcon->get_SPNDPP()),
    _SRCHPP(inEcon->get_SRCHPP()),
    _SETUPC(inEcon->get_SETUPC()),
    _dmdEls(inEcon->get_DMDELS()),
    _unType(None),
    _agntID(agntID),		  
    _spGood(spGood),		  
    _d1Good(d1Good),		  
    _d2Good(d1Good % inEcon->get_NGOODS() + 1),		  
    _bkrupt(false),		  
    _credit(false),
    _freqTr(inEcon->get_FREQTR()),
    _spEffc(0.0),
    _d1Effc(0.0),     
    _d2Effc(0.0),   
    _monHld(0.0),     
    _prmInc(0.0),   
    _plnExp(0.0),     
    _depHld(0.0),   
    _bkLoan(0.0), 
    _spShop(NULL),
    _d1Shop(NULL),
    _d2Shop(NULL),
    _fmShop(NULL),
    _fmBank(NULL),
    _agBank(NULL),
    _frList(inEcon->get_frList()),
    _agEcon(inEcon),
    _agMode(NULL),  
    _sector(0),
    _unWeek(-1)
{
    _orders[0] =  0.0;
    _orders[1] =  0.0;
    _shareP[0] = -1.0;
    _shareP[1] = -1.0;
    _legCap = new double[_agEcon->get_NGOODS()];
    _shFixK = new double[_agEcon->get_NGOODS()];
    for(int i = 0; i < _agEcon->get_NGOODS(); i++) {
        _legCap[i] = 0;
        _shFixK[i] = 0;
    }
    _bsPlan = new BusinessPlan();
    _agMode = inEcon->get_worker();  // The Economy ctor must create _worker before creating agents
}

Agent::~Agent() {
    _agMode = NULL;
    delete   _bsPlan; _bsPlan = NULL;
    _spShop = NULL;
    _d1Shop = NULL;
    _d2Shop = NULL;
    _fmShop = NULL;
    _agEcon = NULL;
    _fmBank = NULL;
    _agBank = NULL;
    _frList = NULL;

    delete[] _shFixK;
    _shFixK = NULL;
  
    delete[] _legCap; 
    _legCap = NULL; 
}

ostream& operator <<(ostream &stream, Agent* object) {
    stream << "agntID: " << object->_agntID << "\n";
    stream << "spGood: " << object->_spGood << "\n";
    stream << "spEffc: " << object->_spEffc << "\n";
    stream << "d1Good: " << object->_d1Good << "\n";
    stream << "d1Effc: " << object->_d1Effc << "\n";
    stream << "d2Good: " << object->_d2Good << "\n";
    stream << "d2Effc: " << object->_d2Effc << "\n";
    stream << "monHld: " << object->_monHld << "\n";
    stream << "depHld: " << object->_depHld << "\n";
    stream << "prmInc: " << object->_prmInc << "\n";
    stream << "plnExp: " << object->_plnExp << "\n";
    stream << "bkLoan: " << object->_bkLoan << "\n";

    stream << "bkrupt: " << object->_bkrupt << "\n";
    stream << "credit: " << object->_credit << "\n";
    stream << "orders[0]: " << object->_orders[0] << "\n";
    stream << "orders[1]: " << object->_orders[1] << "\n";
    stream << "piExpW: " << object->_piExpW << "\n";
    stream << "piExpY: " << object->_piExpY << "\n";
    stream << "unType: " << object->_unType << "\n";
    stream << "unWeek: " << object->_unWeek << "\n";
    stream << "dmdEls: " << object->_dmdEls << "\n";
    stream << "SETUPC: " << object->_SETUPC << "\n";
    stream << "shareP[0]: " << object->_shareP[0] << "\n";
    stream << "shareP[1]: " << object->_shareP[1] << "\n";
    stream << "SPNDPP: " << object->_SPNDPP << "\n";
    stream << "SRCHPP: " << object->_SRCHPP << "\n";
    stream << "bsPlan.markup: " << object->_bsPlan->_markup << "\n";
    stream << "bsPlan.salary: " << object->_bsPlan->_salary << "\n";
    stream << "bsPlan.retail: " << object->_bsPlan->_retail << "\n";
    stream << "bsPlan.tSales: " << object->_bsPlan->_tSales << "\n";
    stream << "bsPlan.trPrft: " << object->_bsPlan->_trPrft << "\n";

    stream << "spShop: " << object->_spShop->get_shopID(); stream << "\n";
    stream << "d1Shop: " << object->_d1Shop->get_shopID(); stream << "\n";
    stream << "d2Shop: " << object->_d2Shop->get_shopID(); stream << "\n";
    stream << "fmShop: " << object->_fmShop->get_shopID(); stream << "\n";
    
    stream << "sector: " << object->_sector << "\n";
    stream << "agBank: "; if (object->_agBank) stream << object->_sector; stream << "\n";
    stream << "fmBank: "; if (object->_fmBank) stream << object->_sector; stream << "\n";
  
    stream << "shFixK[" << object->_d1Good-1 << "]: " << object->_shFixK[object->_d1Good-1] << "\n"; 
    stream << "shFixK[" << object->_d2Good-1 << "]: " << object->_shFixK[object->_d2Good-1] << "\n"; 

    stream << "legCap[" << object->_spGood-1 << "]: " << object->_legCap[object->_spGood-1] << "\n"; 
    stream << "legCap[" << object->_d1Good-1 << "]: " << object->_legCap[object->_d1Good-1] << "\n"; 
    stream << "legCap[" << object->_d2Good-1 << "]: " << object->_legCap[object->_d2Good-1] << "\n"; 
    stream << "\n";

    return stream;
}

void Agent::pln_budget() {
    double income = (_fmShop ? _fmShop->get_profit() : _spEffc);
    double wealth = _monHld + (_fmBank ? _fmBank->get_buffer() : _depHld) - _bkLoan;
    double finLim = wealth;
    if(_fmBank && _fmBank->is_troubld()) finLim = _monHld;
    if(_fmShop) {
        if(!_credit && !_agEcon->get_NOBANK())
            _credit = _agBank->opn_crLine(this);
        finLim += get_cLimit();
    }
    double bizFin = (_fmShop ? _fmShop->upd_trWgBl() : 0);
    if(finLim < 0 && _fmShop) {
        be_frclosd();
        return;
    }
    _prmInc += _agEcon->get_INCSPD() * (income - _prmInc);
    _prmInc *= _piExpW;     // update to this week's dollars using expected inflation
    _plnExp = max(_SPNDPP * (wealth + get_legVal() + _agEcon->get_annVal() * _prmInc), 0); // consumption demand
  
    double amount = _monHld - min(_plnExp + bizFin, finLim);
    if(_fmBank) {
        _fmBank->inc_reserv(amount);
        _monHld -= amount;
    }
    else {
        _agBank->rcv_dposit(amount);
        _depHld += amount;
        _monHld -= amount;
        if(_fmShop) take_aLoan(-min(_bkLoan,_depHld));
        else if(_depHld < 0) {
            _monHld = max(_monHld + _depHld, 0.0);
            _depHld = 0;
        }
    }

    if(!_fmShop) _plnExp = min(_plnExp, _monHld);
    else         _plnExp = max(_monHld - bizFin, 0);
}

void Agent::adj_shareP(Shop* dxShop, int gIndex) {
    double shareP = pow(dxShop->get_retail(), _dmdEls);
    if (gIndex == 0 && _d1Shop == dxShop)
        _shareP[0] = shareP;
    if (gIndex == 1 && _d2Shop == dxShop)
        _shareP[1] = shareP;
}

void Agent::buy_dGoods() {
  
    if(RandUtils::get_RnFrac() > _freqTr)
        return;

    _orders[0]=0.0; _orders[1]=0.0;
    if( _d1Shop || _d2Shop) {
        _plnExp = min(_plnExp, _monHld);
        if (_d1Shop) {
            if (!_d2Shop)
                _orders[0] = _plnExp;
            else
                _orders[0] = _plnExp * _shareP[0] / (_shareP[0] + _shareP[1]);
        }
        _orders[1] = max(_plnExp - _orders[0], 0.0);
    }

    if ( (_orders[0]==0) && (_orders[1]==0) ) {
        _d1Effc = _d1Shop ? _d1Shop->get_retail() : 0.0;
        _d2Effc = _d2Shop ? _d2Shop->get_retail() : 0.0;
        return;
    }
  
    double d1qnty = 0.0;
    double d2qnty = 0.0;
    Agent* s1 = _d1Shop ? _d1Shop->get_shOwnr() : NULL;
    Agent* s2 = _d2Shop ? _d2Shop->get_shOwnr() : NULL;
    
  
    if (_orders[0] > 0.0) {
        if (_orders[0] * _d1Shop->get_retail() <= _d1Shop->get_invent()) {
            _d1Effc = _d1Shop->get_retail();
            d1qnty = _orders[0] * _d1Shop->get_retail();
        }
        else {
            d1qnty = _d1Shop->get_invent();
            _d1Effc = d1qnty / _orders[0];
            _orders[0] = d1qnty / _d1Shop->get_retail();
        }
        _d1Shop->inc_aSales(d1qnty);
        _d1Shop->dcr_invent(d1qnty);
		_agEcon->inc_aggCon(d1qnty);
        s1->_monHld += (1.0 - _agEcon->get_txRate()) * _orders[0];
        _monHld -= _orders[0];
    }

    if (_orders[1] > 0.0) {
        if (_orders[1] * _d2Shop->get_retail() <= _d2Shop->get_invent()) {
            _d2Effc = _d2Shop->get_retail();
            d2qnty = _orders[1] * _d2Shop->get_retail();
        }
        else {
            d2qnty = _d2Shop->get_invent();
            _d2Effc = d2qnty / _orders[1];
            _orders[1] = d2qnty / _d2Shop->get_retail();
        }
        _d2Shop->inc_aSales(d2qnty);
        _d2Shop->dcr_invent(d2qnty);
		_agEcon->inc_aggCon(d2qnty);
        s2->_monHld += (1.0 - _agEcon->get_txRate()) * _orders[1];
        _monHld -= _orders[1];
    }
}

void Agent::ini_nwAgnt() {
    _agMode = _agEcon->get_worker();

    int spIndx = _spGood - 1;
    int d1Indx = _d1Good - 1;
    int d2Indx = _d2Good - 1;

    _spEffc = _agEcon->get_wgRate();
    _d1Effc = _agEcon->get_initPm();
    _d2Effc = _agEcon->get_initPm();
    _plnExp = 0.0;
    _unType = None;
    _unWeek = -1;

    _bkrupt = false;
    _credit = false;

    _spShop = _agEcon->get_shList(spIndx);
    _d1Shop = _agEcon->get_shList(d1Indx);
    _d2Shop = _agEcon->get_shList(d2Indx);

    for(int i = 0; i < _agEcon->get_NGOODS(); i++) {
        _legCap[i] = 0;
        _shFixK[i] = 0;
    }
  
    if (this == _agEcon->get_prList(spIndx, _spGood == 1 || _spGood == _agEcon->get_NGOODS() ? 0 : _spGood - 2)) {
        _agMode = _agEcon->get_sOwner();
        _fmShop = _agEcon->get_shList(spIndx);
        _prmInc = _fmShop->get_profit();
        _spEffc = 1.0;
        _spShop = _fmShop;
        _monHld = (1.0 - _agEcon->get_txRate()) * _fmShop->get_tSales() / _agEcon->get_initPm();
        _shFixK[_d1Good-1] = _SETUPC / 2.0;
        _shFixK[_d2Good-1] = _SETUPC / 2.0;
    }
    else {
        _fmShop = NULL;
        _prmInc = _agEcon->get_wgRate();
        _monHld = _agEcon->get_wgRate();
        if(_agEcon->get_GAPTRG() > 0.0) {
            _prmInc *= exp(-_agEcon->get_GAPTRG());
            _monHld *= exp(-_agEcon->get_GAPTRG());
        }
    }
  
    _sector = (_spGood-1)*_agEcon->get_NBANKS()/_agEcon->get_NGOODS();
    _agBank = _agEcon->get_bkList(_sector);
    _bkLoan = 0.0;
  
    if(  ((_spGood-1)%(_agEcon->get_NGOODS()/_agEcon->get_NBANKS())==0)
      && (this == _agEcon->get_prList(_spGood-1, (_spGood>2) ? _spGood-3 : _agEcon->get_NGOODS() -3))) {
        _agMode = _agEcon->get_bOwner();
        _fmBank = _agBank;
        _depHld = 0;
    }
    else {
        _fmBank = NULL;
        _depHld = _agEcon->get_bndStk() / _agEcon->get_NAGNTS();
    }

    _shareP[0] = pow(_d1Shop->get_retail(), _dmdEls);
    _shareP[1] = pow(_d2Shop->get_retail(), _dmdEls);
    _orders[0] = _orders[1] =  0.0;

    _bsPlan->_markup = 0.0;
    _bsPlan->_retail = 0.0;
    _bsPlan->_salary = 0.0;
    _bsPlan->_trPrft = 0.0;
    _bsPlan->_tSales = 0.0;
}

const bool Agent::cls_fmShop(double invent, double wgRate) {
    double wealth = _monHld + _depHld - _bkLoan;
    bool bkrupt = (_bkrupt || (wealth < _agEcon->get_fsaleP()*(invent + _SETUPC)));
    _bkrupt = false;
    _credit = false;
    double bShare = 0;

    if(wealth >= 0) { // can pay off the loan
        if(_depHld >= _bkLoan) {
            _depHld          -= _bkLoan;
            _agBank->dcr_dAccts(_bkLoan);
        }
        else {
            _monHld          -= _bkLoan - _depHld;
            _agBank->inc_reserv(_bkLoan - _depHld);
            _agBank->dcr_dAccts(_depHld);
            _depHld           = 0;
        }
    }

    else { //bank will take some capital
        _agBank->dcr_dAccts(_depHld);
        _depHld           = 0;
        _agBank->inc_reserv(_monHld);
        _monHld           = 0;
        bShare = min (-wealth/(_agEcon->get_fsaleP()*(invent + _SETUPC)), 1);
    }

    _legCap[_spGood-1] = (1-bShare) * invent;
    _legCap[_d1Good-1] = (1-bShare) * _shFixK[_d1Good-1];
    _legCap[_d2Good-1] = (1-bShare) * _shFixK[_d2Good-1];
    if(_legCap[_spGood-1]) _frList[_spGood-1]->add_seller(this);  
    if(_legCap[_d1Good-1]) _frList[_d1Good-1]->add_seller(this);  
    if(_legCap[_d2Good-1]) _frList[_d2Good-1]->add_seller(this);  
    if(bShare) {
        _agBank->seize_good(_spGood-1, bShare * invent);  
        _agBank->seize_good(_d1Good-1, bShare * _shFixK[_d1Good-1]);  
        _agBank->seize_good(_d2Good-1, bShare * _shFixK[_d2Good-1]);
    }
    _shFixK[_d1Good-1]=0;
    _shFixK[_d2Good-1]=0;
     
    _agBank->writes_off(_bkLoan);
    _bkLoan = 0;
    _fmShop = NULL;
    _agMode = _agEcon->get_worker();
    _prmInc = wgRate;        // The family adjusts to a worker's standard of living

    _bsPlan->_markup = 0.0;
    _bsPlan->_retail = 0.0;
    _bsPlan->_salary = 0.0;
    _bsPlan->_trPrft = 0.0;
    _bsPlan->_tSales = 0.0;

    return bkrupt;
}

const bool Agent::not_profit(double invent, double wgRate) const {
    /*if(_monHld + _depHld - _bkLoan + _agEcon->get_fsaleP() * (invent + _SETUPC) >= 0)
      return ((wgRate - _prmInc) * _agEcon->get_annVal() > - _agEcon->get_fsaleP() * (invent + _SETUPC));
    else*/
    //  return ((wgRate - _prmInc) * _agEcon->get_annVal() > min(_monHld + _depHld - _bkLoan, 0));
    double wealth = _monHld + _depHld - _bkLoan;
    return ((wgRate - _prmInc) * _agEcon->get_annVal() + max(_agEcon->get_fsaleP() * (invent + _SETUPC) , -wealth) > 0);
}

const double Agent::buy_frsale(double amount, int gdIndx) {
    double bought = _frList[gdIndx]->run_frsale(amount);
    inc_depHld(-bought * _agEcon->get_fsaleP());
    return bought;
}

const double Agent::buy_invent(double demand) {
    double qntity = min(_frList[_spGood-1]->get_supply(), demand);
    if (qntity <= 0) return 0.0;
    if( _agBank->is_troubld() || !_credit || _agEcon->get_NOBANK() )
        qntity = min(qntity, _depHld / _agEcon->get_fsaleP());
    else if(_agEcon->get_fsaleP() * qntity - _depHld > 0)  {
        double lQuant = _agEcon->get_fsaleP() * qntity - _depHld;
        take_aLoan(lQuant);
        if(_agEcon->get_NCROOK())
            if(RandUtils::get_RnFrac() <= _agEcon->get_crProb() / (1 - _agEcon->get_crProb())) {
                Agent* cr = _agEcon->get_aCrook(_sector); // a random crook takes out a loan
                cr->take_aLoan(lQuant);
            }
    }
    return buy_frsale(qntity, _spGood - 1);
}

void Agent::dbg_chkNeg() {
    if (_piExpW < 0.0 || _piExpY < 0.0 || _orders[0] < 0.0 || _orders[1] < 0.0 ||
      _d1Effc < 0.0 || _d2Effc < 0.0 || _shFixK[_d1Good-1] < 0.0 || _shFixK[_d2Good-1] < 0.0 ||
      _monHld < 0.0 || _depHld < 0.0 || _bkLoan    < 0.0 || _spEffc    < 0.0 ||
      _legCap[_spGood-1] <0 || _legCap[_d1Good-1] <0 || _legCap[_d2Good-1] <0 /*||
      ( _fmShop && (_shFixK[_d1Good-1] + _shFixK[_d2Good-1] > _SETUPC) ) */) {
        WaitForSingleObject(CSLMTX, INFINITE);
        cout << "_wkCntr: " << _agEcon->get_wkCntr() << "  _PITRGY: " << _piExpY << endl;
        cout << this;
        getchar();
        ReleaseMutex(CSLMTX);
    }
}

const bool Agent::lose_shops(UnType cause) {
    if (_fmShop) return false;

    bool unempl = false;
    if (_spShop && cause != None) {
        unempl =  true;
        _unWeek = _agEcon->get_wkCntr();
        _unType =  cause;
    }
    
    _spShop = NULL;
    _d1Shop = NULL;
    _d2Shop = NULL;
    _spEffc = 0.0;
    _d1Effc = 0.0;
    _d2Effc = 0.0;
    _shareP[0] = -1.0;
    _shareP[1] = -1.0;

    return unempl;
}

const bool Agent::lose_aShop(Shop* shop, UnType cause) {
    if (_spShop ==  shop) {
        _unWeek  = _agEcon->get_wkCntr();
        _unType  =  cause;
        _spShop    =  NULL;
        _spEffc    =  0.0;
        return true;
    }
    if (_d1Shop ==  shop) {
        _d1Shop    =  NULL;
        _d1Effc    =  0.0;
        _shareP[0] = -1.0;
        return false;
    }
    if (_d2Shop ==  shop) {
        _d2Shop    =  NULL;
        _d2Effc    =  0.0;
        _shareP[1] = -1.0;
    }
    return false;
}

const double Agent::get_wealth() {  // returns the agent's financial wealth
    return _monHld + (_fmBank ? _fmBank->get_buffer() : _depHld) - _bkLoan;
}

const double Agent::get_cLimit() const {
    if (!_credit) return 0;
    double cLimit = (_agEcon->get_NOBANK() ? 0 : _agEcon->get_hcutPr()*(_SETUPC+_fmShop->get_invent()));
    if(_agBank->is_troubld()) 
        cLimit = min(cLimit, _bkLoan);  // troubled bank won't increase loans and will require repayment when collateral is sold
    return cLimit;
}

const bool Agent::sell_labor() {
    if (_fmShop || !_spShop) return true;
    _spEffc = _spShop->get_spEffc();
    _spShop->inc_pInput();
    if (_spEffc == 0.0 && _unType != Broke) {
        _unWeek = _agEcon->get_wkCntr();
        _unType = Broke;
    }
    if(_spShop->is_firing())
        return !lose_aShop(_spShop, Layoff); 	
    if (_spEffc > 0.0) {
        _spShop->employ_one();
        _spShop->get_shOwnr()->_monHld -= _spEffc;
        _monHld += _spEffc;
    }  
    return true;
}

void Agent::inc_depHld(double amount) {
    _agBank->rcv_dposit(amount);
    _depHld += amount;
    if(_depHld < 0) {
        _monHld = max(_monHld + _depHld, 0.0);
        _depHld = 0;
    }
}

const double Agent::liquidate(int gIndex, double demand) {
    double& capitl = _agMode->fr_capital(gIndex, this);
    double supply = min(demand, capitl);
    capitl -= supply;

    if(_fmBank) _fmBank->inc_reserv(supply * _agEcon->get_fsaleP());
    else inc_depHld(supply * _agEcon->get_fsaleP()); 

    if(capitl==0)
        _frList[gIndex]->drop_sellr(this);

    return supply;
}

const double Agent::get_fOffer(int gIndex) const {
    if (_fmBank) return _fmBank->get_frcCap(gIndex);
    else         return _legCap[gIndex];
}

const double Agent::get_legVal() const {
    return _agEcon->get_fsaleP() * (
      _legCap[_spGood-1] +
      _legCap[_d1Good-1] +
      _legCap[_d2Good-1]);
}

const double Agent::leg_to_inv() {
    double newInv = _legCap[_spGood-1];
    if(newInv > 0.0) {
        _legCap[_spGood-1] = 0.0;
        _frList[_spGood-1]->drop_sellr(this);
    }
    return newInv;
}

const int Agent::opn_fmShop(double salary) {

    //if (_agntID==94 && _agEcon->get_wkCntr()==0) {
    //    int z=94;}
    if(_fmShop || _fmBank)
        return 0;
    // calculate the cost of capital
    double capCst = cal_capCst();
    // availability of fixed capital test
    if (capCst < 0) return 0;
    // ask approval for a credit line
    bool approv = (_agEcon->get_NOBANK() ? false : _agBank->opn_crLine(this));
    // financial viability test
    if ( get_wealth() + _agEcon->get_hcutPr() * approv * (_legCap[_spGood-1] + _SETUPC) 
       < (_agEcon->get_FXDCST() - 1) * salary * _agEcon->get_WKSPMN() + capCst )
    return 1;  

    //initiate a BusinessPlan
    _bsPlan->_salary = salary;
    _bsPlan->_markup = 
    _agEcon->get_RNDMRK() ? 
      1 + RandUtils::get_RnFrac() * (_agEcon->get_MARKUP() - 1) * 2.0 : _agEcon->get_MARKUP();
    _bsPlan->_tSales = RandUtils::get_RnLess(_agEcon->get_MAXSLS()) + 1.0;
  
    Shop* fmShop = _agEcon->get_nextSh(); // Provisional family shop
    double tInput = fmShop->get_fxdCst() + _bsPlan->_tSales + fmShop->get_invSpd() * (_bsPlan->_tSales - _legCap[_spGood-1]);
    double bkLoan = capCst + salary * (tInput -1) - _monHld - _depHld;
    if(bkLoan <= 0) bkLoan = 0;
  
    // expected profit, calculated in last week's dollars
    _bsPlan->_trPrft = ( salary * 
      ( ( _bsPlan->_markup-_agEcon->get_wkIntr() ) * _bsPlan->_tSales
      -_agEcon->get_wkIntr()*(_agEcon->get_FXDCST()-1.0) )
      - (_agBank->get_lnIntr() - _agEcon->get_wkIntr()) * bkLoan ) / _piExpW;
  
    // profitability test
    if ( (_prmInc-_bsPlan->_trPrft) * _agEcon->get_annVal() + capCst + get_legVal() >= 0) {
        _bsPlan->_markup = 0.0;
        _bsPlan->_retail = 0.0;
        _bsPlan->_salary = 0.0;
        _bsPlan->_trPrft = 0.0;
        _bsPlan->_tSales = 0.0;
        return 2;
      }

    // start market research
    _bsPlan->_retail = (1.0 - _agEcon->get_txRate()) / (_bsPlan->_markup * salary);
    Agent* workr1 = NULL;
    Agent* cstmr1 = NULL;
    do workr1 = get_comrad();
    while (workr1->_fmShop);
    cstmr1 = _agEcon->get_cnList(_spGood - 1, RandUtils::get_RnLess(_agEcon->get_numCns(_spGood - 1)));
    
    // simple wage comparisons
    if (cstmr1->_d1Effc >= _bsPlan->_retail * _piExpW ||  workr1->_spEffc * _piExpW >= salary) {
        _bsPlan->_markup = 0.0;
        _bsPlan->_retail = 0.0;
        _bsPlan->_salary = 0.0;
        _bsPlan->_trPrft = 0.0;
        _bsPlan->_tSales = 0.0;
        return 2;
      }

    // open a shop
    _agMode = _agEcon->get_sOwner();
    /*_fmShop = _agEcon->get_nextSh();*/
    _fmShop = fmShop;
	if (approv && (capCst - get_wealth() > 0)) {
		take_aLoan(capCst - get_wealth());
		_agEcon->inc_entrLn(capCst - get_wealth());
	}
    _credit = approv;
    buy_shFixK();
    _spShop = _fmShop;
    _spEffc = 1.0;
    _fmShop->opn_fmShop(_bsPlan, this);
    _prmInc = _fmShop->get_profit();
    _agEcon->inc_nSllrs(_spGood-1);
    workr1->start_aJob(_fmShop, _fmShop->get_salary()/_piExpW);
    cstmr1->patron_ize(_fmShop, _fmShop->get_retail()*_piExpW, 0);
    return 3;
}

const int Crook::opn_fmShop(double salary) {
    double capCst = cal_capCst();
    if (capCst < 0) return 0;
    bool approv = (_agEcon->get_NOBANK() ? false : _agBank->opn_crLine(this));
	if (approv && (capCst - get_wealth() > 0)) {
		take_aLoan(capCst - get_wealth());
		_agEcon->inc_entrLn(capCst - get_wealth());
	}
    return 0;
}

void Crook::take_aLoan(double amount) {
    _agBank->dcr_dAccts(-amount);
    set_depHld(get_depHld() + amount);
}

void Agent::start_aJob(Shop* spShop, double spEffc) {
    _agEcon->inc_jMchSt(_spShop);
    _spShop = spShop;
    _spEffc = spEffc;
    _agEcon->inc_drStat(_unType, _unWeek);
    _unType = None;
    _unWeek = -1;
}

Agent* Agent::get_comrad() {
    int i = RandUtils::get_RnLess(_agEcon->get_numPrs(_spGood-1) - 1);
    if( _agEcon->get_prList(_spGood-1, i)->_agntID >= _agntID)
        return _agEcon->get_prList(_spGood-1, i+1);
    else
        return _agEcon->get_prList(_spGood-1, i);
}

void Agent::take_aLoan(double amount) {
    _depHld += amount;
    _agBank->make_aLoan(amount);
    _bkLoan += amount;
}

void Agent::add_intrst(double wkIntr) {
    _depHld *= wkIntr;
    _bkLoan *= _agBank->get_lnIntr();
}

void Agent::find_shops() {
    // job search
    if(RandUtils::get_RnFrac() <= _SRCHPP && !_fmShop) {
        Agent* cm = get_comrad();
        Shop*  sh = cm->_spShop;
        double spComp = (cm->_fmShop ? sh->get_salary() / _piExpW : cm->_spEffc);
        if (_spEffc < spComp && sh != _spShop && sh->is_hiring())
            start_aJob(sh, spComp);
    }
    //store search
    // search by referral
    int i = RandUtils::get_RnLess(_agEcon->get_numCns(_d1Good-1) - 1);	// find a soulmate
    Agent* sl = _agEcon->get_cnList(_d1Good-1, i);

    if (sl->_agntID >= _agntID)
        sl = _agEcon->get_cnList(_d1Good-1, i+1);
    if (sl->_d1Effc > _d1Effc)
        patron_ize(sl->_d1Shop, sl->_d1Effc, 0);
    if (sl->_d2Effc > _d2Effc)
        patron_ize(sl->_d2Shop, sl->_d2Effc, 1);
    
        // direct search
    Shop* sh = _agEcon->get_shList(RandUtils::get_RnLess(_agEcon->get_nShops()));

    while (!sh->is_active())
        sh = _agEcon->get_shList(RandUtils::get_RnLess(_agEcon->get_nShops()));
    if (sh->get_shGood() == _d1Good && sh->get_retail() * _piExpW > _d1Effc)
        patron_ize(sh, sh->get_retail()*_piExpW, 0);  
    if (sh->get_shGood() == _d2Good && sh->get_retail() * _piExpW > _d2Effc)
        patron_ize(sh, sh->get_retail()*_piExpW, 1);
}

void Agent::patron_ize(Shop* dxShop, double dxEffc, int gdIndx) {
    if(gdIndx == 0) {
        _agEcon->inc_sMchSt(_d1Shop);
        _d1Shop = dxShop;
        _d1Effc = dxEffc;
    }
    else if(gdIndx == 1) {
        _agEcon->inc_sMchSt(_d2Shop);
        _d2Shop = dxShop;
        _d2Effc = dxEffc;
    }
    else cout << "BAD PATRON" << endl;
    adj_shareP(dxShop, gdIndx);
}

void Agent::buy_nwBank(Bank* nwBank, double* frcCap) {
    set_fmBank(nwBank);
    for(int i = 0; i < _agEcon->get_NGOODS(); i++)
        if( frcCap[i] > 0 && _legCap[i]==0)
            _frList[i]->add_seller(this);
    frcCap[_spGood-1] += _legCap[_spGood-1];
    frcCap[_d1Good-1] += _legCap[_d1Good-1];
    frcCap[_d2Good-1] += _legCap[_d2Good-1];
    _legCap[_spGood-1] = 0.0;
    _legCap[_d1Good-1] = 0.0;
    _legCap[_d2Good-1] = 0.0;
    _depHld = 0;
}

const double Agent::cal_capCst() const {
    double supply = _legCap[_d1Good-1]+_legCap[_d2Good-1]; 
    if(supply >= _SETUPC)
        return 0;

    double moreSp = _frList[_d1Good-1]->get_supply() + _frList[_d2Good-1]->get_supply();
    if(moreSp >= _SETUPC) // Agent's _legCap is in the fsale queue; i.e., in moreSP
        return _agEcon->get_fsaleP() * (_SETUPC - supply);

    double capCst = _agEcon->get_fsaleP() * (moreSp - supply); // running tab
    supply = moreSp;
  
    Shop* sh1 = (_d1Shop->get_retail() >= _d2Shop->get_retail() ? _d1Shop : _d2Shop);
    Shop* sh2 = (_d1Shop->get_retail() >= _d2Shop->get_retail() ? _d2Shop : _d1Shop);
    switch ( (_d1Shop!=NULL) + 2*(_d2Shop!=NULL) ) {
        case 0 :      // entrepreneur has no stores
            return -1;
        case 1 :      // entrepreneur has just a d1Shop
            moreSp = _d1Shop->get_invent();
            if(supply + moreSp >= _SETUPC)
                return capCst + (_SETUPC - supply) / _d1Shop->get_retail();
            return -1;
        case 2 :      // entrepreneur has just a d2Shop
            moreSp = _d2Shop->get_invent();
            if(supply + moreSp >= _SETUPC)
                return capCst + (_SETUPC - supply) / _d2Shop->get_retail();
            return -1;
        case 3 :      // entrepreneur has two stores
            moreSp = sh1->get_invent();
            if(supply + moreSp >= _SETUPC)
                return capCst + (_SETUPC - supply) / sh1->get_retail();

            supply += moreSp;
            capCst += moreSp / sh1->get_retail();
            moreSp = sh2->get_invent();
            if(supply + moreSp >= _SETUPC)
                return capCst + (_SETUPC - supply) / sh2->get_retail();
            return -1;
        default :
            return -1;
    }
}

void Agent::buy_shFixK() {
  // convert legacy capital to fixed capital
    _shFixK[_d1Good-1] = _legCap[_d1Good-1];
    _shFixK[_d2Good-1] = _legCap[_d2Good-1];
    if(_legCap[_d1Good-1]>0) _frList[_d1Good-1]->drop_sellr(this);
    if(_legCap[_d2Good-1]>0) _frList[_d2Good-1]->drop_sellr(this);
    _legCap[_d1Good-1] = 0;
    _legCap[_d2Good-1] = 0;
    double needed = _SETUPC - _shFixK[_d1Good-1] - _shFixK[_d2Good-1];
    if (needed <= 0) return;

    // go to firesales
    double moreSp = buy_frsale(needed, _d1Good-1);
    _shFixK[_d1Good-1] += moreSp;
    needed -= moreSp;
    if (needed <= 1.0E-010) return;
    moreSp = buy_frsale(needed, _d2Good-1);
    _shFixK[_d2Good-1] += moreSp;
    needed -= moreSp;
    if (needed <= 1.0E-010) return;

    // buy from least expensive store
    Shop* sh = (_d1Shop->get_retail() >= _d2Shop->get_retail() ? _d1Shop : _d2Shop);
    int gd = (sh == _d1Shop ? _d1Good : _d2Good);
    double moreCa = needed/sh->get_retail() - _monHld;
    if(moreCa > 0) {
        inc_depHld(- moreCa);
        inc_monHld(moreCa);
    }
    double bought = min(needed, sh->get_invent());
    sh->get_shOwnr()->_monHld += bought/sh->get_retail();
    _monHld -= bought/sh->get_retail();
    _shFixK[gd-1] += bought;
    sh->inc_aSales(bought);
    sh->dcr_invent(bought);
    needed -= bought;
    if(needed <= 1.0E-010) return;
    
    // buy from the other store
    sh = (sh == _d1Shop ? _d2Shop : _d1Shop);
    gd = (gd == _d1Good ? _d2Good : _d1Good);
    moreCa = needed/sh->get_retail() - _monHld;
    if(moreCa > 0) {
        inc_depHld(- moreCa);
        inc_monHld(moreCa);
    }
    if (needed <= sh->get_invent() + 1.0E-010) {
        sh->get_shOwnr()->_monHld += needed/sh->get_retail();
        _monHld -= needed/sh->get_retail();
        _shFixK[gd-1] += needed;
        sh->inc_aSales(needed);
        sh->dcr_invent(needed);
        return;
    }
    else {
        cout << "Not enough inventory\n";
        char cccccc = cin.get();
        //abort();
    }
}

void Agent::be_frclosd() {
    _agBank->inc_reserv(_monHld);         // bank takes all your money
    _monHld           = 0;
    _depHld           = 0;                // deposits voided
    _bkrupt           = true;          
    _plnExp           = 0;
    _bkLoan           = 0;                  // loans voided                             
    _fmShop->lose_stock(_agBank);           // bank takes inventory to stop business
    _agBank->seize_good(_d1Good-1, _shFixK[_d1Good - 1]);
    _agBank->seize_good(_d2Good-1, _shFixK[_d2Good - 1]);      
    _shFixK[_d1Good-1] = 0;
    _shFixK[_d2Good-1] = 0;
}
const int Agent::get_spGood() const {return _spGood;}

const int Agent::get_sector() const {return _sector;}

const int Agent::get_unWeek() const {return _unWeek;}

const int Agent::get_agntID() const {return _agntID;}

const bool Agent::is_bankrpt() const {return _bkrupt;}
  
const double Agent::get_spEffc() const {return _spEffc;}

const double Agent::get_monHld() const {return _monHld;}

const double Agent::get_depHld() const {return _depHld;}

const double Agent::get_bkLoan() const {return _bkLoan;}

const double Agent::get_freqTr() const {return _freqTr;}
  
Shop* Agent::get_d1Shop() const {return _d1Shop;}

Shop* Agent::get_d2Shop() const {return _d2Shop;}

Shop* Agent::get_spShop() const {return _spShop;}

Shop* Agent::get_fmShop() const {return _fmShop;}

Bank* Agent::get_fmBank() const {return _fmBank;}

Bank* Agent::get_agBank() const {return _agBank;}

void Agent::inc_monHld(double incrmt) {_monHld += incrmt;}
  
void Agent::set_depHld(double amount) {_depHld  = amount;}

void Agent::set_fmBank(Bank* fmBank)  {_fmBank  = fmBank;}

void Agent::set_agMode(AgentMode* agMode) {_agMode = agMode;}

AgentMode* Agent::get_agMode() const {return _agMode;}