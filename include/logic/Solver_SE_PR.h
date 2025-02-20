#ifndef SOLVER_SE_PR_H
#define SOLVER_SE_PR_H

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
#include "ToolsSolver.h"

#include "../util/Printer.h"
#include "../util/ToolsList.h"

using namespace std;

/// <summary>
/// This class is responsible for calculating an extension of the preferred semantics.
/// </summary>
class Solver_SE_PR {
public:

	/// <summary>
	/// Checks if an extension of the preferred semantics can be calculated.
	/// </summary>
	/// <param name="framework">The abstract argumentation framework, specifying the underlying attack relations between the arguments.</param>
	/// <param name="proof_extension"> Calculated preferred extension.</param>
	/// <returns>TRUE iff an extension was found. FALSE otherwise.</returns>
	static bool solve(AF &framework, list<uint32_t> &proof_extension);

	/// <summary>
	/// Checks if an extension of the preferred semantics can be calculated in the specified reduct of the framework.
	/// </summary>
	/// <param name="framework">The abstract argumentation framework, specifying the underlying attack relations between the arguments.</param>
	/// <param name="reduct"></param>
	/// <param name="proof_extension"> Calculated preferred extension.</param>
	/// <returns>TRUE iff an extension was found. FALSE otherwise.</returns>
	static bool solve(AF &framework, ArrayBitSet &reduct, list<uint32_t> &proof_extension);
};

#endif