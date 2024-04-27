#ifndef SOLVER_DS_PR_H
#define SOLVER_DS_PR_H

#include <iostream>
#include <cstdint>
#include <vector>

extern "C" {
	#include "../util/MemoryWatchDog.h"
}

#include "Actives.h"
#include "AF.h"
#include "Encodings.h"
#include "Enums.h"
#include "Decodings.h"
#include "PreProcessor_DS_PR.h"
#include "Reduct.h"
#include "SatSolver.h"
#include "SatSolver_cms.h"
#include "SatSolver_cadical.h"
#include "ScepticalCheck.h"
#include "omp.h"

using namespace std;

class Solver_DS_PR {
public:

	/// <summary>
	/// Checks if a specified argument is sceptically accepted.
	/// </summary>
	/// <param name="argument">The argument, which could be sceptical accepted or not.</param>
	/// <param name="framework">The abstract argumentation framework, specifying the underlying attack relations between the arguments.</param>
	/// <param name="proof_extension"> Extension proving, that the argument cannot be sceptically accepted.</param>
	/// <param name="numCores"> Number of cores requested to be used to solve the problem. Actual number can be lower depending on the OS scheduler.</param>
	/// <param name="solver_type"> Defines which solver shal be used to solve the problem.</param>
	/// <returns>TRUE iff the argument is sceptical accepted. FALSE otherwise.</returns>
	static bool solve(uint32_t argument, AF &framework, list<uint32_t> &proof_extension, uint8_t numCores, SOLVERS solver_type);
};

#endif


