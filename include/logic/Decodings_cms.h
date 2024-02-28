#ifndef DECODINGS_CMS_H
#define DECODINGS_CMS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

extern "C" {
	#include "AF.h"
	#include "Actives.h"
	#include "Decodings.h"
}

#include <cryptominisat.h>
#include <vector>

using namespace CMSat;

class DecodingCMS {
public:

	/// <summary>
	/// Translates the solution of a specified SAT-problem to a set of argument, in the context of a specified set of active arguments.
	/// </summary>
	/// <param name="problem">The SAT-Solver, which solution if to be tranlated.</param>
	/// <param name="activeArgs">The set of active arguments, in the current state of the framework.</param>
	/// <returns>Set of arguments, which solve the specified SAT-problem.</returns>
	static nodeUInt32_t *get_set_from_solver(SATSolver *solver, activeArgs_t *activeArgs);

};
#endif