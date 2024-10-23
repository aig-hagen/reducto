#include "../../include/logic/Processor_DS_PR.h"

static void set_is_rejected(bool &is_rejected, bool &is_terminated, bool &found_counter_evidence)
{
#pragma atomic write
	is_rejected = true;
#pragma omp flush(is_rejected)
#pragma atomic write
	is_terminated = true;
#pragma omp flush(is_terminated)
	found_counter_evidence = true;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

list<uint32_t> Proc_DS_PR::calculate_nonempty_adm_set(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, bool &is_rejected, bool &is_terminated,
	SatSolver &solver, bool &continue_calculation, bool &found_counter_evidence, bool is_first_iteration) {
	if (is_first_iteration) {
		bool has_counter_evidence = solver.solve(Encoding::get_literal_accepted(query_argument, true),
			Encoding::get_literal_rejected(framework.num_args, query_argument, false));
		continue_calculation = !has_counter_evidence;
		if (has_counter_evidence) {
			list<uint32_t> initial_set = Decoding::get_set_from_solver(solver, active_args);
			set_is_rejected(is_rejected, is_terminated, found_counter_evidence);
			return initial_set;
		}
	}

	bool has_solution_without_query = solver.solve(Encoding::get_literal_accepted(query_argument, true));
	continue_calculation = has_solution_without_query;
	if (!has_solution_without_query) {
		//there is no nonempty adm. set, which is not containing the query, there might be solutions containing the query
		if (is_first_iteration && !solver.solve(Encoding::get_literal_accepted(query_argument, false))) {
			// this is the first iteration, so there have been no solution excluded by a complement clause
			// there is no nonempty adm. set, with or without the query argument, that's why there is only the empty set as adm. set
			// which means we found a complete extension, which is not containing the query argument, hence we found a counter evidence
			set_is_rejected(is_rejected, is_terminated, found_counter_evidence);
		}
		return list<uint32_t>();
	}

	list<uint32_t> initial_set = Decoding::get_set_from_solver(solver, active_args);
	//no check if rejecting query, since every solution with OUT_query would have been found in first iteration, hence following the NOT_IN_query assumption query has to be UNDEC
	// which means that there cannot be an attacker of the query in the extension
	return initial_set;
}
