#ifndef SOLVER_DS_ST_H
#define SOLVER_DS_ST_H

#include <iostream>
#include <cstdint>
#include <vector>

#include "AF.h"
#include "Decoding.h"
#include "Encoding.h"
#include "Enums.h"
#include "PreProcessor_GR.h"
#include "PreProcessor_GR_parallel.h"
#include "Reduct.h"
#include "SatSolver.h"
#include "ScepticalCheck.h"
#include "ToolsSolver.h"
#include "omp.h"

#include "../util/Printer.h"
#include "../util/ToolList.h"
#include "../util/Tools_omp.h"

using namespace std;

/// <summary>
/// This class is responsible for solving a problem of sceptical acceptance of an query-argument
/// in extensions of the stable semantics.
/// </summary>
class Solver_DS_ST {
public:

	/// <summary>
	/// Checks if a specified argument is sceptically accepted by all stable extensions.
	/// </summary>
	/// <param name="query_argument">The argument, which could be sceptical accepted or not.</param>
	/// <param name="framework">The abstract argumentation framework, specifying the underlying attack relations between the arguments.</param>
	/// <param name="proof_extension"> Extension proving, that the argument cannot be sceptically accepted.</param>
	/// <param name="numCores"> Number of cores requested to be used to solve the problem. Actual number can be lower depending on the OS scheduler.</param>
	/// <returns>TRUE iff the argument is sceptically accepted. FALSE otherwise.</returns>
	static bool solve(uint32_t query_argument, AF &framework, list<uint32_t> &proof_extension, uint16_t numCores);
};

#endif