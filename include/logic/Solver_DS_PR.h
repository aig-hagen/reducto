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
#include "PreProcessor_GR.h"
#include "Processor_DS_PR.h"
#include "Reduct.h"
#include "SatSolver.h"
#include "ScepticalCheck.h"
#include "ToolsSolver.h"

#include "../util/Printer.h"
#include "../util/ToolsList.h"

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
	/// <param name="query_argument">The argument, which could be sceptical accepted or not.</param>
	/// <param name="framework">The abstract argumentation framework, specifying the underlying attack relations between the arguments.</param>
	/// <param name="proof_extension"> Extension proving, that the argument cannot be sceptically accepted.</param>
	/// <returns>TRUE iff the argument is sceptically accepted. FALSE otherwise.</returns>
	static bool solve(uint32_t query_argument, AF &framework, list<uint32_t> &proof_extension);
};

#endif