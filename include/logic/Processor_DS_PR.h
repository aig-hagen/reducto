#ifndef PROC_DS_PR_H
#define PROC_DS_PR_H

#include <cstdint>
#include <list>
#include "omp.h"

#include "Encoding.h"
#include "Decoding.h"
#include "PreProcessor_GR.h"
#include "SatSolver.h"
#include "ScepticalCheck.h"

#include "../util/ArrayBitSet.h"
#include "../util/ToolList.h"

using namespace std;

/// <summary>
/// This class is responsible for processing solving steps for the DS-PR problem.
/// </summary>
class Proc_DS_PR {

public:
	/// <summary>
	/// This method calculates a nonemtpy set, if such a set can be calculated in the specified reduct, using the specified solver.
	/// </summary>
	/// <param name="query_argument">The query argument of the DS-PR problem</param>
	/// <param name="framework">The abstract argumentation framework of the problem.</param>
	/// <param name="active_args">The arguments that are active in the current state of the framework.</param>
	/// <param name="is_rejected">Flag used to signal that the argument is rejected or not.</param>
	/// <param name="solver">The solver used to calculate sets for this state of the framework.</param>
	/// <param name="has_found_solution">Flag used to signal if the calculation of sets in state should be continued.</param>
	/// <param name="found_counter_evidence">Flag that indicates that the processor has found an nonempty adm. set that is a counter evidence</param>
	/// <param name="is_first_iteration">Flag indicating if this iteration to calculate a set is the first in the specified state or not.</param>
	/// <returns>An non emtpy set, if such a set can be computed. Empty list otherwise.</returns>
	static list<uint32_t> calculate_rejecting_set(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, bool &is_rejected,
		SatSolver &solver, bool &has_found_solution, bool is_first_iteration);

	/// <summary>
	/// This method calculates a nonempty complete set of arguments, which attacks the query argument or returns an empty list if such a set does not exist.
	/// </summary>
	/// <param name="query_argument">The query argument of the DS-PR problem</param>
	/// <param name="framework">The abstract argumentation framework of the problem.</param>
	/// <param name="active_args">The arguments that are active in the current state of the framework.</param>
	/// <param name="has_found_solution">TRUE iff the solver has found such a set. FALSE otherwise.</param>
	/// <returns>The calculated set or EMPTY if no such set was found.</returns>
	static list<uint32_t> calculate_complete_set_query_out(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, bool &has_found_solution);
};
#endif