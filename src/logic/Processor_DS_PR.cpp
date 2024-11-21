#include "../../include/logic/Processor_DS_PR.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

list<uint32_t> Proc_DS_PR::calculate_rejecting_set(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, bool &is_rejected, bool &is_terminated,
	SatSolver &solver, bool &continue_calculation, bool is_first_iteration) {
	bool has_solution_without_query = solver.solve(Encoding::get_literal_accepted(query_argument, true));
	continue_calculation = has_solution_without_query;
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

	list<uint32_t> initial_set = Decoding::get_set_from_solver(solver, active_args);

	if (ScepticalCheck::check_rejection(query_argument, initial_set, framework)) {
		tools::ToolsOMP::set_is_rejected(is_rejected, is_terminated);
	}

	return initial_set;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

list<uint32_t> Proc_DS_PR::calculate_rejecting_set_in_random_coi(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, bool &is_rejected, bool &is_terminated,
	SatSolver &solver, bool &continue_calculation, bool is_first_iteration) {
	int i = 0;
	int lim = 6;
	bool has_solution_without_query;
	SatSolver *solver_coi = NULL;
	solver_coi = new SatSolver_cadical(framework.num_args);
	do {
		
		// Generate a random number of an argument, that is not the query
		uint32_t random_argument;
		do {
			random_argument = rand() % framework.num_args + 1;
		} while (random_argument == query_argument);

		ArrayBitSet reduct_coi = PreProc_GR_parallel::calculate_cone_influence(framework, random_argument);
		Encoding::add_clauses_nonempty_admissible_set(*solver_coi, framework, reduct_coi);
		has_solution_without_query = (*solver_coi).solve(Encoding::get_literal_accepted(query_argument, true));
		i++;
	} while (i < lim && !has_solution_without_query);

	if (!has_solution_without_query) {
		delete solver_coi;
		return calculate_rejecting_set(query_argument, framework, active_args, is_rejected, is_terminated, solver, continue_calculation, is_first_iteration);
	}

	continue_calculation = has_solution_without_query;
	list<uint32_t> initial_set = Decoding::get_set_from_solver(*solver_coi, active_args);
	if (ScepticalCheck::check_rejection(query_argument, initial_set, framework)) {
		tools::ToolsOMP::set_is_rejected(is_rejected, is_terminated);
	}

	delete solver_coi;
	return initial_set;
}
