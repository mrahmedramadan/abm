/***********************************************************************************************
 * timers.h
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised May 25 2010
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

#include <iostream>
#include <time.h>

using namespace std;

#ifndef TIMERS_H
#define TIMERS_H

#ifdef USE_TIMERS

//const int num_sections = 13;
//enum {IMP=0, CEN, ENT, MCH, BND, TRD, MON, BRK, FSC, EXT, UPD, TMC, YRS};
//char* Sections[] = {"Imp", "Cen", "Ent", "Mch", "Bnd", "Trd", "Mon", "Brk", "Fsc", "Ext", "Upd", "Tmc", "Yrs"};
//double Times[num_sections] = {0};
//clock_t bgnSec = 0;
//clock_t endSec = 0;

#define START_TIMER bgnSec = clock()

#define TIME_UP(X) {                                              \
    endSec = clock();                                             \
    Times[ X ] += (double) (endSec - bgnSec) / CLOCKS_PER_SEC;    \
    bgnSec = clock();                                             \
  }

#define PRINT_TIMES {                       \
    cout.precision(2);                      \
    cout.setf(ios::fixed | ios::showpoint); \
    WaitForSingleObject(CSLMTX, INFINITE);  \
    for(int i = 0; i < num_sections; i++)   \
      cout << "Time in " << Sections[i] <<  \
      " " << Times[i] << endl;              \
    double TotalTime = 0.0;                 \
    for(int i = 0; i < num_sections; i++)   \
      TotalTime += Times[i];                \
    cout << "Sum over all sections = "      \
    << TotalTime << endl;                   \
    ReleaseMutex(CSLMTX);                   \
  }

#else  // USE_TIMERS

#define START_TIMER
#define TIME_UP(X) {}
#define PRINT_TIMES

#endif // USE_TIMERS

#endif // TIMERS_H