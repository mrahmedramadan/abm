/***********************************************************************************************
 * Agent.h
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised Dec 30 2014
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

#include <iostream>
#include <math.h>

using namespace std;

class  Economy;		   // forward declarations
class  Shop;				 
class  Bank;         
class  Firesale;     
class  WorkerMan;    
struct BusinessPlan; 
enum   UnType;       

#ifndef AGENT_H
#define AGENT_H

class Agent {
    friend class AgentMode; // general behavioral mode
    friend class BankOwner; // bankowner mode
    friend class ShopOwner; // shopowner mode
    Shop* _spShop; // shop buying supply good (employer)
    Shop* _d1Shop; // shop selling primary demand good (primary store)
    Shop* _d2Shop; // shop selling secondary demand good (secondary store)
    Shop* _fmShop; // shop owned by agent if any (family shop)
    UnType _unType; // type of unemployment for most recent unemployment event
    Firesale** _frList; // list of firesale markets (not just those used by this agent)
    AgentMode* _agMode; // pointer to the agent's current mode
    BusinessPlan* _bsPlan; // business plan to use during entry state  
    int _sector; // banking sector
    int _unWeek;	// week of unemployment for most recent unemployment event  
    bool _bkrupt; // just what it sounds like
    bool _credit; // indicates that the agent has a credit line
    double _piExpW; // expected inflation factor (per week)
    double _piExpY; // expected inflation factor (per year)
    double _SPNDPP; // propensity to spend out of wealth
    double _SRCHPP;	// job search propensity
    double _SETUPC; // total capital required to set up shop
    double _dmdEls; // demand elasticity parameter (epsilon)
    double _spEffc;	// effective value of endowment of supply good
    double _d1Effc;	// effective value of money for primary demand good
    double _d2Effc; // effective value of money for secondary demand good
    double _monHld;	// money holdings
    double _prmInc;	// permanent income
    double _plnExp;	// planned expenditure
    double _depHld; // deposit holdings
    double _bkLoan; // bank loans outstanding
    double _freqTr; // frequency of trading (in weeks)
    double _shareP[2]; // demand share parameters
    double _orders[2]; // dollar value of current orders to the two demand shops
    double* _legCap;   // array of legacy capital goods
    double* _shFixK;   // array of shop's fixed capital
    const int _agntID; // agent id
    const int _spGood; // supply good             in {1,...,_NGOODS}
    const int _d1Good; // primary demand good     in  "
    const int _d2Good; // secondary demand good   in  "
  
    Agent* get_comrad();
    void buy_shFixK(); // buy fixed capital
    void be_frclosd();
    void inc_depHld(double amount); // used only by agents not needing a loan
    void start_aJob(Shop* spShop, double spEffc); // take a new job
    void patron_ize(Shop* dxShop, double dxEffc, int gdIndx); // switch to new store
    const double get_legVal() const; // return firesale value of legacy capital
    const double buy_frsale(double amount, int gdIndx); // place order (using money?)
    virtual void take_aLoan(double amount);
  
protected:
    Bank* _fmBank; // bank owned by agent if any
    Bank* _agBank; // bank used by agent
    Economy* _agEcon; // access to the economy (for certain economy-wide parameters)
    const double cal_capCst() const; // calculate and return cost of buying fixed capital

public:
    Agent(int agntID, int spGood, int d1Good, Economy* inEcon);
    virtual ~Agent();	                                           
    friend ostream& operator <<(ostream &stream, Agent* object);
    Shop* get_d1Shop() const;
    Shop* get_d2Shop() const;
    Shop* get_spShop() const;
    Shop* get_fmShop() const;
    Bank* get_fmBank() const;
    Bank* get_agBank() const;
    AgentMode* get_agMode() const;
    void pln_budget(); // adjust Yp, financial plan 
    void buy_dGoods(); // go shopping
    void ini_nwAgnt(); // initialize agent for new run
    void find_shops(); // look for a job and stores
    void dbg_chkNeg(); // check for inappropriate negatives
    void set_fmBank(Bank* fmBank);
    void set_agMode(AgentMode* agMode);
    void inc_monHld(double incrmt);
    void set_depHld(double amount);
    void add_intrst(double wkIntr);                // add interest to _depHld and _bkLoan
    void buy_nwBank(Bank* nwBank, double* frcCap); // take ownership of bank
    void adj_shareP(Shop* dxShop, int gIndex);     // adjust share spent at dxShop
    const UnType get_unType() const {return _unType;};
    const int get_spGood() const;
    const int get_sector() const;
    const int get_unWeek() const;
    const int get_agntID() const;
    const bool sell_labor();  // returns false if agent gets laid off
    const bool lose_shops(UnType cause); // returns true if the agent was employed
    const bool lose_aShop(Shop* shop, UnType cause); // returns true if shop was her employer
    const bool cls_fmShop(double invent, double wgRate); // returns true if bankrupt
    const bool is_bankrpt() const;
    const bool not_profit(double invent, double wgRate) const; // fmShop not profitable
    const double get_wealth(); // returns the agent's financial wealth
    const double leg_to_inv(); // turn leg into invent, return invent
    const double buy_invent(double demand); // process the shop's request for more inventory
    const double liquidate(int gIndex, double demand); // returns the amount sold in firesale
    const double get_spEffc() const;
    const double get_monHld() const;
    const double get_depHld() const;
    const double get_bkLoan() const;
    const double get_freqTr() const;
    const double get_cLimit() const; // returns the agent's credit limit
    const double get_fOffer(int gIndex) const; // returns amount offered in firesale
    virtual const int opn_fmShop(double salary);
            // returns 3 (enter), 2 (finance passes), 1 (finance fails) or 0
};

class Crook : public Agent { 
public:
    Crook(int agntID, int spGood, int d1Good, Economy* inEcon) : 
        Agent(agntID, spGood, d1Good, inEcon) {};
    const int opn_fmShop(double salary);
    void take_aLoan(double amount);
};

struct BusinessPlan {
    double _markup; // actual markup
    double _salary; // initial wage
    double _retail; // initial inverse retail price
    double _tSales; // target sales
    double _trPrft; // target profit  	                                           
};

enum UnType {	// declaration of unemployment type
    None,	  // 0. not applicable
    Layoff,	// 1. agent laid off by employer
    Quit,		// 2. agent quit his job
    Exit,		// 3. employer exited the market
    Broke		// 4. employer became broke
};

#endif // AGENT_H
