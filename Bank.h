/***********************************************************************************************
 * Bank.h
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised July 24 2014
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

#include <iostream>
#include <string>

using namespace std;

class Agent;       // forward declaration of Agent
class Parameters;  // forward declaration of Parameters
class Economy;     // forward declaration of Economy
class Firesale;    // forward declaration of Firesale
class BankOwner;   // forward declaration of BankOwner

#ifndef BANK_H
#define BANK_H

class Bank {
    friend class BankOwner;
    Agent* _bkOwnr;     // bank's owner
    Agent** _cuList;    // list of customers
    Economy* _bkEcon;   // access to public information
    Firesale** _frList; // list of firesale markets (not just those used by this bank)
    int    _iniPrd; // length (in weeks) of initiation period for new customers
    int    _entrWk; // week of entry
    int    _sector; // bank's sector
    int    _numCst; // number of customers in the sector
    bool   _troubl; // indicator of troubled bank
    bool   _newBnk; // indicator of bank with no soft customer info yet
    double _bkrCst; // bankruptcy cost (fraction of inventories lost in foreclosure)
    double _sLoans; // value of outstanding commercial loans to shops (NOT kept current)
    double _collat; // value of seized collateral (NOT kept current)
    double _equity; // bank capital (NOT kept current)
    double _dAccts; // deposit account liability (NOT kept current)
    double _borrow; // central bank borrowing (kept current)
    double _reserv; // reserves (kept current)
    double _spread; // normal loan premium (per week) over deposit rate
    double _gBonds; // value of government bond holdings (kept current)
    double _discPr; // discount premium (per week) for central bank borrowing
    double _PSLOPE; // slope parameter in bank's loan demand function
    double _clFrac; // fraction of credit line applications approved
    double _lnIntr; // interest factor on loans 
    double* _frcCap; // list of quantities of siezed inventories (kept current)
  
public:
    Bank(int sector, Parameters* params, Economy* inEcon);
    ~Bank();
    friend ostream& operator << (ostream &stream, Bank* object);
    Agent* get_bkOwnr();
    void set_lnIntr(); // set loan rate and update _newBnk
    void get_checkd(); // check for capital adequacy
    void adj_pfolio(); // adj portfolio
    void dbg_chkNeg(); // check for unwanted negative values
    void be_troubld(); // declare the bank troubled
    void rcv_dposit(double amount); // receive deposit of amount - no new loan
    void get_nwOwnr(double mxbail); // either get a new owner or get bailed out
    void inc_reserv(double dposit); // increase reserves
    void writes_off(double bdLoan); // write off loan
    void cause_fail(double excess); // make the bank borrow enough to fail
    void dcr_dAccts(double amount); // decrement dAccts by amount
    void make_aLoan(double amount); // account for a new loan
    void add_intrst(double wkIntr); // add interest to loans, deposits, govt bonds, and borrowing from Fed
    void ini_nwBank(Economy* inEcon, Agent* bkOwnr); // initialize for new run
    void seize_good(int gIndex, double amount); // foreclose, add name to frSale queue
    void inc_frcCap(int gIndex, double amount); // increment foreclosed inventories
    const bool is_troubld() const;              // returns true if the bank is troubled
    const bool opn_crLine(Agent* custmr) const; // returns true if bank opens a credit line
    const double upd_equity();                  // calculates collateral and returns equity
    const double get_buffer() const;            // returns equity minus required capital
    const double get_reserv() const;            // returns the bank's reserve holding
    const double get_gBonds() const;            // returns the bank's govt bond holding
    const double get_sLoans() const;            // returns the book value of the bank's loans
    const double get_lnIntr() const;            // returns the loan rate as fn of bankWk
    const double get_frcCap(int gIndex) const;  // returns the foreclosed inv holding
};

#endif // BANK_H