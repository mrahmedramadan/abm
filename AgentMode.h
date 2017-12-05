/***********************************************************************************************
 * AgentMode.h
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised July 9 2014
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

class  Agent;        // forward declaration of Agent
class  Economy;		   // forward declaration of Economy
class  Shop;				 // forward declaration of Shop
class  Bank;         // forward declaration of Bank
class  Firesale;     // forward declaration of Firesale
struct BusinessPlan; // forward declaration of BusinessPlan
enum   UnType;       // forward declaration of UnType

#ifndef AGENTMODE_H
#define AGENTMODE_H

class AgentMode {
public:
  virtual double& fr_capital(int gIndex, Agent* mdAgnt);        // capital in firesale queue
  virtual void inc_depHld(double amount, Agent* mdAgnt);        // put directly in bank
  virtual void job_search(Agent* mdAgnt);
  virtual const double get_wealth(Agent* mdAgnt);
  virtual const double get_income(Agent* mdAgnt);
  virtual const double get_finLim(double wealth, Agent* mdAgnt); // shOwnrs w/o credit line reapply
  virtual const double get_bizFin(Agent* mdAgnt);                // update trInput and add trWgBl
  virtual void be_frclosd(Agent* mdAgnt);
  virtual void scale_back(double bizFin, Agent* mdAgnt);                // restrict _plnExp in duress
  AgentMode();
  virtual ~AgentMode();
};

class WorkerMan : public AgentMode {
public:
  WorkerMan();
  ~WorkerMan();
};
    
class ShopOwner : public AgentMode {
public:
  void job_search(Agent* mdAgnt);
  const double get_income(Agent* mdAgnt);
  const double get_finLim(double wealth, Agent* mdAgnt);
  const double get_bizFin(Agent* mdAgnt);
  void be_frclosd(Agent* mdAgnt);
  void scale_back(double bizFin, Agent* mdAgnt);
  ShopOwner();
  ~ShopOwner();
};

class BankOwner : public AgentMode {
public:
  virtual double& fr_capital(int gIndex, Agent* mdAgnt);
  void inc_depHld(double amount, Agent* mdAgnt);
  const double get_wealth(Agent* mdAgnt);
  const double get_finLim(double wealth, Agent* mdAgnt);
  BankOwner();
  ~BankOwner();
};

#endif // AGENTMODE_H
