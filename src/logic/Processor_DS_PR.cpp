#include "../../include/logic/Processor_DS_PR.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static std::__cxx11::list<uint32_t> get_set_from_solver(SatSolver &solver, ArrayBitSet &active_args, uint32_t query_argument, AF &framework, bool &is_rejected, bool &is_terminated)
{
	list<uint32_t> initial_set = Decoding::get_set_from_solver(solver, active_args);

	if (ScepticalCheck::check_rejection(query_argument, initial_set, framework)) {
		tools::ToolsOMP::set_is_rejected(is_rejected, is_terminated);
	}

	return initial_set;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

list<uint32_t> Proc_DS_PR::calculate_rejecting_set(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, bool &is_rejected, bool &is_terminated,
	SatSolver &solver, bool &has_solution_without_query, bool is_first_iteration) {
	has_solution_without_query = solver.solve(Encoding::get_literal_accepted(query_argument, true));
	if (!has_solution_without_query) {
		//there is no nonempty adm. set, which is not containing the query, there might be solutions containing the query
		if (is_first_iteration && !solver.solve(Encoding::get_literal_accepted(query_argument, false))) {
			// this is the first iteration, so there have been no solution excluded by a complement clause
			// there is no nonempty adm. set, with or without the query argument, that's why there is only the empty set as adm. set
			// which means we found a complete extension, which is not containing the query argument, hence we found a counter evidence
			tools::ToolsOMP::set_is_rejected(is_rejected, is_terminated);
		}
		return list<uint32_t>();
	}

	return get_set_from_solver(solver, active_args, query_argument, framework, is_rejected, is_terminated);
}
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

list<uint32_t> Proc_DS_PR::calculate_rejecting_set_in_random_coi(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, 
	bool &is_rejected, bool &is_terminated,
	SatSolver &solver, bool &has_found_adm_set) {
	has_found_adm_set = solver.solve();
	if (!has_found_adm_set) {
		return list<uint32_t>();
	}

	return get_set_from_solver(solver, active_args, query_argument, framework, is_rejected, is_terminated);
}