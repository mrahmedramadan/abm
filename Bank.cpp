/***********************************************************************************************
 * Bank.cpp
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised Dec 30 2014
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

#include "Bank.h"
#include "Agent.h"
#include "Parameters.h"
#include "Shop.h"
#include "RandUtils.h"
#include "Firesale.h"
#include "Economy.h"
#include "AgentMode.h"

Bank::Bank(int sector, Parameters* params, Economy* inEcon) :
    _bkrCst(params->_BKRCST),
    _iniPrd(params->_INIPRD),
    _sector(sector),
    _numCst((params->_NGOODS -2)*params->_NGOODS/params->_NBANKS),
    _troubl(false),
    _newBnk(false),
    _spread(params->_SPREAD / params->_WKSPYR),
    _discPr(params->_DISCPR / params->_WKSPYR),
    _PSLOPE(params->_PSLOPE),
    _bkEcon(inEcon),
    _frcCap(NULL),
    _bkOwnr(NULL),
    _cuList(NULL),
    _frList(inEcon->get_frList())
{
    _cuList = new Agent*[_numCst];
    for(int i = 0; i < _numCst; i++) 
        _cuList[i] = inEcon->get_agList(_sector*_numCst + i);
    _frcCap = new double[inEcon->get_NGOODS()];
    for(int i = 0; i < inEcon->get_NGOODS(); i++)
        _frcCap[i] = 0;
}

Bank::~Bank() {
    _bkOwnr = NULL;
    _frList = NULL;
    for(int i = 0; i < _numCst; i++) 
        _cuList[i] = NULL;
    delete[] _cuList; _cuList = NULL;
    delete[] _frcCap; _frcCap = NULL;
}

ostream& operator <<(ostream &stream, Bank* object) {
    stream << "sector: " << object->_sector << "\n";      
    stream << "sLoans: " << object->_sLoans << "\n";   
    stream << "collat: " << object->_collat << "\n";   
    stream << "dAccts: " << object->_dAccts << "\n";   
    stream << "equity: " << object->_equity << "\n";   
    stream << "borrow: " << object->_borrow << "\n";   
    stream << "reserv: " << object->_reserv << "\n";   
    stream << "gBonds: " << object->_gBonds << "\n";   
    stream << "spread: " << object->_spread << "\n";   
    stream << "discPr: " << object->_discPr << "\n";   
    
    stream << "iniPrd: " << object->_iniPrd << "\n";      
    stream << "numCst: " << object->_numCst << "\n";      
    stream << "PSLOPE: " << object->_PSLOPE << "\n";    

    stream << "troubl: " << object->_troubl << "\n";     

    for (int i=0; i<object->_bkEcon->get_NGOODS(); ++i)
        stream << "frcCap[" << i << "]: " << object->_frcCap[i] << "\n";

    stream << "bkOwnr: " << object->_bkOwnr->get_agntID() << "\n";
    stream << "bkrCst: " << object->_bkrCst << "\n";
    stream << "clFrac: " << object->_clFrac << "\n";
    stream << "entrWk: " << object->_entrWk << "\n";
    stream << "lnIntr: " << object->_lnIntr << "\n";
    stream << "newBnk: " << object->_newBnk << "\n";

    stream << "\n";
    return stream;
}

void Bank::ini_nwBank(Economy* inEcon, Agent* bkOwnr) {
    _sLoans = 0.0;
    _collat = 0.0;
    _borrow = 0.0;
    _reserv = 0.0; // the bank owner will deposit his monHld in Agent::ini_nwAgent()
    _equity = 0.0;
    _clFrac = 1.0;
    _entrWk = -_iniPrd;
    _lnIntr = inEcon->get_wkIntr() + _spread;
  
    _gBonds = inEcon->get_bndStk()/inEcon->get_NBANKS();
    _dAccts = _gBonds - inEcon->get_bndStk() / inEcon->get_NAGNTS(); // no deposit by owner
    _troubl = false;
    _newBnk = false;

    for(int i = 0; i < _bkEcon->get_NGOODS(); i++)
        _frcCap[i] = 0;

    _bkOwnr = bkOwnr;
}

void Bank::adj_pfolio() {
    _dAccts = 0;
    _sLoans = 0;   // tally deposits and loans
    for(int i = 0; i < _numCst; i++) {
        _dAccts += _cuList[i]->get_depHld();
        _sLoans += _cuList[i]->get_bkLoan();
    }
    _reserv += _gBonds;   // govt bonds redeemed
    _gBonds  = 0;

    _reserv -= _borrow;  // central bank debt charged to reserve account
    _borrow  = 0;

    if (_reserv < 0)     // borrow the shortfall, invest the surplus in govt bonds
        _borrow = -_reserv;
    else
        _gBonds = _reserv;
    _reserv = 0;
}

const double Bank::upd_equity() {
    _collat = 0;
    for (int i = 0;  i < _bkEcon->get_NGOODS(); i++)
        _collat += _frcCap[i] * _bkEcon->get_fsaleP();
    return _equity  = _sLoans + _gBonds + _collat + _reserv - _dAccts - _borrow;
}

const double Bank::get_buffer() const {
    return _equity - (_sLoans + _collat) * _bkEcon->get_caRate();
}

void Bank::set_lnIntr() { 
    _lnIntr = _bkEcon->get_wkIntr() + _spread;
    if(_newBnk) {
        if(_bkEcon->get_wkCntr() - _entrWk >= _iniPrd)
            _newBnk = false;
        else 
            _lnIntr /= (1-_bkEcon->get_crProb());
    }
}

void Bank::get_checkd() {
    _troubl = false;
    double buffer = get_buffer();
    if( buffer < 0 ) {
        _troubl = true;
        _bkEcon->inc_nBkTrb();
    }
    if (buffer <= 0)            _clFrac = 0; //in or bordering trouble
    else if (_equity <= buffer) _clFrac = 1; //no trouble, no risky assets
    else                        _clFrac = min(_PSLOPE * buffer / (_equity-buffer), 1.0); 
                                           // weekly setting of loan approval rate
}

void Bank::get_nwOwnr(double mxbail) {
    if(_equity + mxbail > 0) {
        _reserv -= _equity;
        _equity  = 0;
        return;
    }
    _bkEcon->inc_nBkFlr();

    _entrWk = _bkEcon->get_wkCntr();
    _newBnk = true;

    Agent* nwOwnr = NULL; // get a rich customer (not Crook) to take over
    do(nwOwnr = _cuList[RandUtils::get_RnLess(_numCst)]);
        while (nwOwnr->get_fmBank() || nwOwnr->get_fmShop() || dynamic_cast<Crook*>(nwOwnr));
    double wealth = nwOwnr->get_wealth();
    for(int i = 0; i < _numCst; i++)
        if((!_cuList[i]->get_fmShop()) &&
            (!_cuList[i]->get_fmBank()) &&
            (RandUtils::get_RnFrac() <= _bkEcon->get_FDICPR()) &&
            (_cuList[i]->get_wealth() > wealth)) {
                nwOwnr = _cuList[i];
                wealth = _cuList[i]->get_wealth();
        }   
      // the random method of selecting a new owner
      //do(nwOwnr = _cuList[RandUtils::get_RnLess(_numCst)]);
      //  while (nwOwnr->get_fmBank() || nwOwnr->get_fmShop());
      for (int i = 0; i < _bkEcon->get_NGOODS(); i++)
            if( _frcCap[i] > 0)
                _frList[i]->drop_sellr(get_bkOwnr());
      _bkOwnr->set_fmBank(NULL);
      _bkOwnr->set_agMode(_bkEcon->get_worker());
      _bkOwnr = nwOwnr;
      _bkOwnr->set_agMode(_bkEcon->get_bOwner());
      _reserv += (_sLoans + _collat) * _bkEcon->get_caRate() - _equity; // FDIC injects enough to make it fully capitalized
      _dAccts -= _bkOwnr->get_depHld(); // new owner's deposits converted into extra equity
      _bkOwnr->buy_nwBank(this, _frcCap);
      upd_equity();
}

void Bank::dbg_chkNeg() {
    if (_sLoans < 0.0 || _collat < 0.0 || _dAccts < 0.0 || _borrow < 0.0 ||
      _gBonds < 0.0 || _frcCap[0] < 0.0 || _frcCap[10] < 0.0 || _frcCap[20] < 0.0 ||
      _frcCap[30] < 0.0 || _frcCap[40] < 0.0 ) {
        WaitForSingleObject(CSLMTX, INFINITE);
        cout << "_wkCntr: " << _bkEcon->get_wkCntr() << "  _PITRGY: " << endl;
        cout << this;
        getchar();
        ReleaseMutex(CSLMTX);
    }
}

void Bank::rcv_dposit(double amount) {
    _reserv += amount;
    _dAccts += amount;
}

void Bank::make_aLoan(double amount) {
    _dAccts += amount;
    _sLoans += amount;
}

void Bank::add_intrst(double wkIntr) {
    _dAccts *= wkIntr;
    _sLoans *= _lnIntr;
    _gBonds *= wkIntr;
    _borrow *= (wkIntr + _discPr);
}	

void Bank::inc_reserv(double dposit) {
    _reserv += dposit;
}

void Bank::writes_off(double bdLoan) {
    _sLoans -= bdLoan;
}

void Bank::cause_fail(double excess) {
    _borrow += _equity + excess;
}

void Bank::dcr_dAccts(double amount) {
    _dAccts -= amount;
}

const double Bank::get_reserv() const {
    return _reserv;
}

const double Bank::get_gBonds() const {
    return _gBonds;
}

const double Bank::get_sLoans() const {
    return _sLoans;
}

const bool   Bank::is_troubld() const {
    return _troubl;
}

void Bank::be_troubld() {
    _troubl = true;
}

Agent* Bank::get_bkOwnr() {
    return _bkOwnr;
}

const double Bank::get_lnIntr() const {
    return _lnIntr;
}

const double Bank::get_frcCap(int gIndex) const {
    return _frcCap[gIndex];
}

void Bank::inc_frcCap(int gIndex, double amount) {
    _frcCap[gIndex] += amount;
}

void Bank::seize_good(int gIndex, double amount) {
    if(amount==0.0) return;
    if(_frcCap[gIndex] == 0)
        _frList[gIndex]->add_seller(_bkOwnr);
    _frcCap[gIndex] += amount * (1-_bkrCst);
}

const bool Bank::opn_crLine(Agent* custmr) const {
      if(is_troubld()) return false;
      if((!_newBnk) && dynamic_cast<Crook*>(custmr)) return false;
      return (RandUtils::get_RnFrac() <= _clFrac);
}