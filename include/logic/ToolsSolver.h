#ifndef TOOLS_SOLVER_H
#define TOOLS_SOLVER_H

#include <iostream>
#include <cstdint>
#include <vector>
#include "omp.h"

#include "../util/ArrayBitSet.h"
#include "Decoding.h"
#include "SatSolver.h"
#include "SatSolver_cadical.h"

#include "../util/Printer.h"
#include "../util/ToolList.h"

using namespace std;

/// <summary>
/// This class is responsible for calculating an extension of the stable semantics.
/// </summary>
class Tools_Solver {
public:

	/// <summary>
	/// Updates the certificate with the solution of a specified solver.
	/// </summary>
	/// <param name="solver">The solver, which has calculated and saved an extension as a solution.</param>
	/// <param name="active_args">The arguments that are active in the current state of the framework.</param>
	/// <param name="certificate_extension"> The extension used as certificate/proof of the result.</param>
	static void UpdateCertificate(SatSolver *solver, ArrayBitSet &active_args, std::__cxx11::list<uint32_t> &certificate_extension);
};

#endif