/***********************************************************************************************
 * RandUtils.h
 * Written in ANSI/ISO Standard C++ and compiled using Microsoft Visual Studio Express 2012/2015
 * Revised March 10 2009
 *
 * Part of the codebase for the agent-based model developed for "Banks, Market Organization, and
 * Macroeconomic Performance: An Agent-Based Computational Analysis" by Quamrul H. Ashraf, Boris
 * Gershman, and Peter Howitt.
 ***********************************************************************************************/

#include <stdlib.h>

#ifndef RANDUTILS_H
#define RANDUTILS_H

class RandUtils {
public:
	static void set_RnSeed(int RnSeed) {
		srand(RnSeed);
	}

  // get_RnLess(u) returns a random element of the set {0, 1, ..., u-1}
	static int get_RnLess(int uBound) {
		return (uBound * rand() / (RAND_MAX + 1));
	}

	static double get_RnFrac() {
		return ((double) rand() / (RAND_MAX + 1));
	}
};

#endif // RANDUTILS_H
