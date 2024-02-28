#ifndef SCEPTICAL_PR_SEQUENTIAL_H
#define SCEPTICAL_PR_SEQUENTIAL_H

#include <iostream>
#include <stdio.h>
#include <stdint.h>

extern "C" {
#include "Actives.h"
#include "AF.h"
#include "Decodings.h"
#include "Encodings.h"
#include "ListActives.h"
#include "Reduct.h"
#include "SatProblem.h"
#include "ScepticalPRCheck.h"

#include "../util/List.h"
}

#include "ExternalSolver.h"
#include "InitialSetSolver.h"

class ScepticalPRSequential {
public:

	/// <summary>
	/// Checks if a specified argument's sceptical acceptance can be rejected, because there exists an initial set in the specified 
	///  framework or any of its reducts, which attacks the argument. This method uses recursive method calls.
	/// </summary>
	/// <param name="argument">The argument, which could be sceptical accepted or not.</param>
	/// <param name="framework">The abstract argumentation framework, specifying the underlying attack relations between the arguments.</param>
	/// <param name="activeArgs">The active arguments of the current state of the framework.</param>
	/// <returns>TRUE iff the sceptical acceptance was rejected. FALSE otherwise.</returns>
	static bool check_rejection_per_reduct_recursiv(uint32_t argument, argFramework_t *framework, activeArgs_t *activeArgs);

	/// <summary>
	/// Checks if a specified argument's sceptical acceptance can be rejected, because there exists an initial set in the specified 
	///  framework or any of its reducts, which attacks the argument. This method uses iterative data structures.
	/// </summary>
	/// <param name="argument">The argument, which could be sceptical accepted or not.</param>
	/// <param name="framework">The abstract argumentation framework, specifying the underlying attack relations between the arguments.</param>
	/// <param name="activeArgs">The active arguments of the current state of the framework.</param>
	/// <returns>TRUE iff the sceptical acceptance was rejected. FALSE otherwise.</returns>
	static bool check_rejection_iterative(uint32_t argument, argFramework_t *framework, activeArgs_t *activeArgs);
};

#endif
