/***********************************************************************************************
 * Shop.h
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised Jan 05 2015
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

#include <iostream>
using namespace std;
class Agent;			   // forward declaration of Agent
class Parameters;    // forward declaration of Parameters
class Economy;       // forward declaration of Economy
class Bank;          // forward declaration of Bank
struct BusinessPlan; // forward declaration of BusinessPlan
class ShopOwner;     // forward declaration of ShopOwner

#ifndef SHOP_H
#define SHOP_H

class Shop {
    friend class ShopOwner;
    Agent* _shOwnr; // shop owner
    Economy* _shEcon; // access to economy-wide variables
    int _salAge; // time since implementation of current salary in {0,...,CNTPRD-1}
    int _onsale; // whether or not there is a sale on
    int _gouger; // whether or not the shop is gouging
    int _flexWg; // downward wage flexibility flag
    int _cntPrd; // contract period measured in weeks
    int _iniShp; // whether or not it was one of the initial shops
    int _shopID; // shop id
    int _shGood; // good traded at shop in {1,...,_NGOODS}
    int _census; // whether or not recorded as an active shop during the last census
    int _entrWk; // week of entry  
    int _sector; // banking sector
    bool _active;	// shop active or not  
    double _avgdem;	// average demand for labor over the contract period
    double _avgsup;	// average supply of labor over the contract period
    double _invSpd; // weekly inventory adjustment speed (lambda_I)
    double _wagSpd; // annual wage adjustment coefficient (beta)
    double _triggr;	// critical inventory/sales ratio triggering a price cut
    double _pDelta;	// size of price cut (oldPrice/newPrice)
    double _cntPrY;	// contract period measured in years
    double _piExpY; // Expected inflation factor (per year)
    double _piExpW; // Expected inflation factor (per week)
    double _failRt;	// weekly failure rate when unprofitable (phi)
    double _brkFrc;	// weekly quit rate (delta)
    double _fxdCst; // fixed cost
    double _tSales; // target sales
    double _lTrigg;	// critical inventory/sales ratio triggering a layoff
    double _profit;	// current profits
    double _tInput;	// target input (including shop owner's)
    double _pInput; // potential input (including shop owner's)
    double _invent; // inventory stock of traded good
    double _markup; // markup factor
    double _aSales; // actual sales
    double _aInput; // actual input (including shop owner's)
    double _salary; // wage rate
    double _retail; // inverse retail price
    
public: 
    Shop(int shopID, Parameters* params, Economy* inEcon); // constructor used in Economy()
    Shop(int shopID, Shop* sOther, int NGOODS);            // used in Economy::get_nextSh()
    ~Shop();			                                     
    friend ostream& operator <<(ostream &stream, Shop* object);  
    Agent* get_shOwnr();
    void set_census(int number);
    void set_aSales(double aSales);
    void inc_aSales(double nwSale);
    void upd_tSales();
    void prep_Trade(); // prepare for the week's trade
    void dbg_chkNeg(); // check for unwanted negative values
    void employ_one(); // add one employee and update inventory
    void inc_pInput(); // add one to _pInput
    void lose_stock(Bank*  agBank); // have inventories seized by agBank
    void ini_nwShop(Agent* shOwnr); // set initial shop parameters for run
    void opn_fmShop(BusinessPlan* bsPlan, Agent* shOwnr); // open new shop
    void clr_params(int NGOODS);    // reset all parameters (except shopID)
    void ini_markup(double markup); // initialize markup ratio
    void dcr_invent(double amount); // decrement inventory
    void upd_natInc(double& rlIncm, double& nmIncm); // update rlIncm, nmIncm
    void upd_wgStat(double& tlWage, double& tlEmpl); // update wage stats
    const int upd_prices();         // update wages and prices
    const int get_shGood() const;
    const int get_census() const;
    const int get_entrWk() const;
    const int pln_toExit(double wgRate, int shocks, double hcutPr);
                                    // return 0 (no exit), 1 (hopeless) or 2
    const bool is_firing()  const;
    const bool is_active()  const;
    const bool is_hiring()  const;
    const bool cls_fmShop(double wgRate); // returns true if bankrupt  
    const double upd_trWgBl();      // update tInput, return targ wage bill
    const double upd_accnts();      // depreciate, update profit and agPrft
    const double upd_joStat();      // update agVncy
    const double get_spEffc() const;
    const double get_profit() const;
    const double get_markup() const;
    const double get_aSales() const;
    const double get_aInput() const;
    const double get_salary() const;
    const double get_retail() const;
    const double get_invent() const;
    const double get_shopID() const;
    const double was_iniShp() const;
    const double get_tSales() const;
    const double get_fxdCst() const;
    const double get_invSpd() const;
};

#endif // SHOP_H
