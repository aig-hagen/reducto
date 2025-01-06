#include "../../include/logic/Processor_DS_PR.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static std::__cxx11::list<uint32_t> get_set_from_solver(SatSolver &solver, ArrayBitSet &active_args, uint32_t query_argument, AF &framework, 
	bool &is_rejected)
{
	list<uint32_t> initial_set = Decoding::get_set_from_solver(solver, active_args);

	if (ScepticalCheck::check_rejection(query_argument, initial_set, framework)) {
		is_rejected = true;
	}

	return initial_set;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

list<uint32_t> Proc_DS_PR::calculate_rejecting_set(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, bool &is_rejected,
	SatSolver &solver, bool &has_solution_without_query, bool is_first_iteration) {
	has_solution_without_query = solver.solve(Encoding::get_literal_accepted(query_argument, true));
	if (!has_solution_without_query) {
		//there is no nonempty adm. set, which is not containing the query, there might be solutions containing the query
		if (is_first_iteration && !solver.solve(Encoding::get_literal_accepted(query_argument, false))) {
			// this is the first iteration, so there have been no solution excluded by a complement clause
			// there is no nonempty adm. set, with or without the query argument, that's why there is only the empty set as adm. set
			// which means we found a complete extension, which is not containing the query argument, hence we found a counter evidence
			is_rejected = true;
		}
		return list<uint32_t>();
	}

	return get_set_from_solver(solver, active_args, query_argument, framework, is_rejected);
}