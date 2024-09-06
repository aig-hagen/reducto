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
	SatSolver &solver, bool &continue_calculation, bool &found_counter_evidence) {
	continue_calculation = solver.solve(Encoding::get_literal_accepted(query_argument, true));
	if (!continue_calculation) {
		//there is no nonempty adm. set, which is not containing the query, there might be solutions containing the query
		return list<uint32_t>();
	}

	list<uint32_t> initial_set = Decoding::get_set_from_solver(solver, active_args);

	if (ScepticalCheck::check_rejection(query_argument, initial_set, framework)) {
		set_is_rejected(is_rejected, is_terminated, found_counter_evidence);
	}

	return initial_set;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Proc_DS_PR::check_existance_nonempty_adm_set(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, bool &is_rejected, bool &is_terminated,
	bool &found_counter_evidence, SatSolver &solver) {

	bool has_solution_with_query = solver.solve(Encoding::get_literal_accepted(query_argument, false));
	if (!has_solution_with_query)
	{
		set_is_rejected(is_rejected, is_terminated, found_counter_evidence);
	}
	return has_solution_with_query;
}



