#include "../../include/logic/Solver_DC_ST.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool start_checking(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, list<uint32_t> &proof_extension)
{
	uint64_t numVars = active_args._array.size();
	SatSolver *solver = NULL;
	solver = new SatSolver_cadical(numVars);
	Encoding::add_clauses_nonempty_stable_set(*solver, framework, active_args);
	bool has_solution_with_query = (*solver).solve(Encoding::get_literal_accepted(query_argument, false),
		Encoding::get_literal_rejected(framework.num_args, query_argument, true));
	if (has_solution_with_query) {
		Tools_Solver::UpdateCertificate(solver, active_args, proof_extension);
	}

	delete solver;
	return has_solution_with_query;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Solver_DC_ST::solve(uint32_t query_argument, AF &framework, list<uint32_t> &proof_extension)
{
	ArrayBitSet initial_reduct = ArrayBitSet();
	pre_proc_result result_preProcessor = PreProc_GR::process_only_grounded(framework, query_argument, false, false, initial_reduct, proof_extension);

	switch (result_preProcessor) {

	case accepted:
		return true;

	case rejected:
		return false;

	default:
		return start_checking(query_argument, framework, initial_reduct, proof_extension);
	}
}