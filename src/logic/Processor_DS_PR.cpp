#include "../../include/logic/Processor_DS_PR.h"

void set_is_rejected(bool &is_rejected, bool &is_terminated)
{
#pragma atomic write
	is_rejected = true;
#pragma omp flush(is_rejected)
#pragma atomic write
	is_terminated = true;
#pragma omp flush(is_terminated)
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

list<uint32_t> Proc_DS_PR::process_iteration(uint32_t query_argument, AF &framework, ArrayBitSet &activeArgs, bool &is_rejected, bool &is_terminated,
	SatSolver &solver, bool &continue_calculation, bool is_first_iteration) {
	bool has_solution_without_query = solver.solve(Encoding::get_literal_accepted(query_argument, false));

	if (!has_solution_without_query) {
		//there is no nonempty adm. set, which is not containing the query, there might be solutions containing the query
		continue_calculation = false;
		if (is_first_iteration && !solver.solve(Encoding::get_literal_accepted(query_argument, true))) {
			// this is the first iteration, so there have been no solution excluded by a complement clause
			// there is no nonempty adm. set, with or without the query argument, that's why there is only the empty set as adm. set
			// which means we found a complete extension, which is not containing the query argument, hence we found a counter evidence
			set_is_rejected(is_rejected, is_terminated);
		}
		return list<uint32_t>();
	}

	list<uint32_t> initial_set = Decoding::get_set_from_solver(solver, activeArgs);

	if (ScepticalCheck::check_rejection(query_argument, initial_set, framework)) {
		set_is_rejected(is_rejected, is_terminated);
	}

	return initial_set;
}
