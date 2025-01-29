#include "../../include/logic/Solver_DS_ST.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool start_checking(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, list<uint32_t> &proof_extension)
{
	uint64_t numVars = active_args._array.size();
	SatSolver *solver = NULL;
	solver = new SatSolver(numVars);
	Encoding::add_clauses_nonempty_stable_set(*solver, framework, active_args);
	bool has_solution_without_query = (*solver).solve(Encoding::get_literal_accepted(query_argument, true),
		Encoding::get_literal_rejected(framework.num_args, query_argument, false));
	if (has_solution_without_query) {
		Tools_Solver::UpdateCertificate(solver, active_args, proof_extension);
	}
	//only two cases remain: 1. no stable solution is computable; 2. all stable solutions contain the query; both cases lead to scetical acceptance
		
	delete solver;
	return !has_solution_without_query;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Solver_DS_ST::solve(uint32_t query_argument, AF &framework, list<uint32_t> &proof_extension)
{
	ArrayBitSet initial_reduct = ArrayBitSet();
	pre_proc_result result_preProcessor;
	result_preProcessor = PreProc_GR::process_only_grounded(framework, query_argument, true, false, initial_reduct, proof_extension);

	switch (result_preProcessor) {

	case accepted:
		return true;

	case rejected:
		if (initial_reduct._array.size() == 0) {
			//calculated grounded extension is the stable extension
			return false;
		}

		return !Tools_Solver::check_existance_stable_extension(framework, initial_reduct, proof_extension);

	default:
		if (initial_reduct._array.size() == 0) {
			//calculated grounded extension is the stable extension
			cout << "ERROR should be impossible" << endl;
			return false;
		}

		return start_checking(query_argument, framework, initial_reduct, proof_extension);
	}
}