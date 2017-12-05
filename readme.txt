August 12, 2016

Code underlying the version of "Banks, Market Organization
and Macroeconomic Performance: An Agent Based Computational
Analysis" dated August 12, 2016

agent.cpp	implementation file for the Agent class
agent.h		header file for the Agent class
agentMode.cpp   implementation file for the AgentMode class
agentMode.h     header file for the AgentMode class
bank.cpp	implementation file for the Bank class
bank.h		header file for the Bank class
bankfail.cpp	main source file
bankfail.ini    contains user-specified parameter values
economy.cpp	implementation file for the Economy class
economy.h	header file for the Economy class
expresults.h	header file used for reporting results in experiment mode
firesale.cpp	implementation file for the Firesale class
firesale.h	header file for the Firesale class
globals.h	header file defining global variables
parameters.cpp  implementation file for the Parameters class
parameters.h    header file for the Parameters class
randutils.h	header file providing some static utility interfaces to the random number generator
readme.txt	this file
require.h	header file used in exception handling
shop.cpp	implementation file for the Shop class
shop.h		header file for the Shop class
statistics.cpp  implementation file for the Statistics class
statistics.h    header file for the Statistics class
timers.h	header file for profiling

This codebase will compile and run under Microsoft Visual Studio Express 2012 or later on a Windows machine.

The user will need to supply "bankfail.ini" in the command line.

The user can vary the parameter values by altering the constants in the bankfail.ini file.

Setting OPMODE = "experiment" will make the code run NTHRDS simultaneous threads of NSRUNS runs each, setting the EXPVAR variable equal to EXPINI in thread 0 and incrementing it by EXPINC in each subsequent thread.

Setting OPMODE = "calibration" will make the code run NTHRDS simulataneous threads of NSRUNS runs each, all using the same parameter values.

Setting OPMODE = "impulse" will make the code run NTHRDS x NSRUNS runs twice; once with with baseline parameter values and then again with a shock applied after IMPLAG years, where the nature of the shock is defined by the parameter IMPULS.
