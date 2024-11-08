#include "../../include/logic/Solver_SE_ST.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool start_checking(AF &framework, ArrayBitSet &active_args, list<uint32_t> &proof_extension)
{
	uint64_t numVars = active_args._array.size();
	SatSolver *solver = NULL;
	solver = new SatSolver_cadical(numVars);
	Encoding::add_clauses_stable_set(*solver, framework, active_args);
	bool has_solution = (*solver).solve();
	if (has_solution) {
		list<uint32_t> calculated_extension = Decoding::get_set_from_solver(*solver, active_args);
		list<uint32_t> new_proof = tools::ToolList::extend_list(proof_extension, calculated_extension);
		proof_extension = new_proof;
		calculated_extension.clear();
	}

	delete solver;
	return has_solution;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Solver_SE_ST::solve(AF &framework, list<uint32_t> &proof_extension)
{
	ArrayBitSet initial_reduct = PreProc_GR::process(framework, proof_extension);
	return start_checking(framework, initial_reduct, proof_extension);
}