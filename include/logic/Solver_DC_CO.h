#ifndef SOLVER_DC_CO_H
#define SOLVER_DC_CO_H

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
#include "SatSolverCrypto.h"
#include "ToolsSolver.h"

#include "../util/Printer.h"
#include "../util/ToolsList.h"

using namespace std;

/// <summary>
/// This class is responsible for deciding about the credulous acceptance of a query-argument
/// using the complete semantics.
/// </summary>
class Solver_DC_CO {
public:

	/// <summary>
	/// Checks if a specified argument is credulously accepted using the complete semantics.
	/// </summary>
	/// <param name="query_argument">The argument, which could be accepted or not.</param>
	/// <param name="framework">The abstract argumentation framework, specifying the underlying attack relations between the arguments.</param>
	/// <param name="out_proof_extension"> Extension proving, that the argument is credulously accepted.</param>
	/// <returns>TRUE iff the argument is credulously accepted. FALSE otherwise.</returns>
	static bool solve(uint32_t query_argument, AF &framework, list<uint32_t> &out_proof_extension);
};

#endif