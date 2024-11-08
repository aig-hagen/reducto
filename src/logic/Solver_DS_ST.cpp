#include "../../include/logic/Solver_DS_ST.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool start_checking(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, list<uint32_t> &proof_extension)
{
	uint64_t numVars = active_args._array.size();
	SatSolver *solver = NULL;
	solver = new SatSolver_cadical(numVars);
	Encoding::add_clauses_stable_set(*solver, framework, active_args);
	bool has_solution_without_query = (*solver).solve(Encoding::get_literal_accepted(query_argument, true),
		Encoding::get_literal_rejected(framework.num_args, query_argument, false));
	if (has_solution_without_query) {
		proof_extension = Decoding::get_set_from_solver(*solver, active_args);
	}
	
	delete solver;
	return has_solution_without_query;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Solver_DS_ST::solve(uint32_t query_argument, AF &framework, list<uint32_t> &proof_extension)
{
	ArrayBitSet initial_reduct = ArrayBitSet();
	pre_proc_result result_preProcessor = PreProc_GR::process(framework, query_argument, initial_reduct);

	switch (result_preProcessor) {

	case accepted:
		return true;

	case rejected:
		return false;

	case unknown:
		return !start_checking(query_argument, framework, initial_reduct, proof_extension);

	default:
		return unknown;
	}
}