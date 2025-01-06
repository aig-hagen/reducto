#ifndef SOLVER_DC_ST_H
#define SOLVER_DC_ST_H

#include <iostream>
#include <cstdint>
#include <vector>
#include "omp.h"

#include "AF.h"
#include "Decoding.h"
#include "Encoding.h"
#include "Enums.h"
#include "PreProcessor_GR.h"
#include "Reduct.h"
#include "SatSolver.h"
#include "SatSolver_cadical.h"
#include "ToolsSolver.h"

#include "../util/Printer.h"
#include "../util/ToolList.h"

using namespace std;

/// <summary>
/// This class is responsible for solving a problem of credulous acceptance of an query-argument
/// in extensions of the stable semantics.
/// </summary>
class Solver_DC_ST {
public:

	/// <summary>
	/// Checks if a specified argument is credulously accepted by at least one stable extension.
	/// </summary>
	/// <param name="argument">The argument, which could be sceptical accepted or not.</param>
	/// <param name="framework">The abstract argumentation framework, specifying the underlying attack relations between the arguments.</param>
	/// <param name="proof_extension"> Extension proving, that the argument can be credulously accepted.</param>
	/// <returns>TRUE iff the argument is credulously accepted. FALSE otherwise.</returns>
	static bool solve(uint32_t query_argument, AF &framework, list<uint32_t> &proof_extension);
};

#endif