#ifndef SOLVER_DS_PR_H
#define SOLVER_DS_PR_H

#include <iostream>
#include <cstdint>
#include <string_view>
#include <functional>
#include <vector>
#include <queue>

extern "C" {
	#include "../util/MemoryWatchDog.h"
}

#include "AF.h"
#include "Decoding.h"
#include "Encoding.h"
#include "Enums.h"
#include "ExtensionPrioritised.h"
#include "Heuristic1.h"
#include "Heuristic2.h"
#include "IPrioHeuristic.h"
#include "PreProcessor_DS_PR.h"
#include "Reduct.h"
#include "SatSolver.h"
#include "SatSolver_cadical.h"
#include "ScepticalCheck.h"
#include "omp.h"

#include "../util/Printer.h"
#include "../util/ToolList.h"

using namespace std;

/// <summary>
/// This class is responsible for solving a problem of sceptical acceptance of an query-argument
/// in extensions of the preferred semantics.
/// </summary>
class Solver_DS_PR {
public:

	/// <summary>
	/// Checks if a specified argument is sceptically accepted by all preferred extensions.
	/// </summary>
	/// <param name="argument">The argument, which could be sceptical accepted or not.</param>
	/// <param name="framework">The abstract argumentation framework, specifying the underlying attack relations between the arguments.</param>
	/// <param name="proof_extension"> Extension proving, that the argument cannot be sceptically accepted.</param>
	/// <param name="numCores"> Number of cores requested to be used to solve the problem. Actual number can be lower depending on the OS scheduler.</param>
	/// <returns>TRUE iff the argument is sceptical accepted. FALSE otherwise.</returns>
	static bool solve(uint32_t argument, AF &framework, list<uint32_t> &proof_extension, uint8_t numCores);
};

#endif
