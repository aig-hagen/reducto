#include "../../include/logic/Solver_SE_ST.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool start_checking(AF &framework, ArrayBitSet &active_args, list<uint32_t> &proof_extension)
{
	uint64_t numVars = active_args._array.size();
	SatSolver *solver = NULL;
	solver = new SatSolver_cadical(numVars);
	Encoding::add_clauses_nonempty_stable_set(*solver, framework, active_args);
	bool has_solution = (*solver).solve();
	if (has_solution) {
		Tools_Solver::UpdateCertificate(solver, active_args, proof_extension);
	}

	delete solver;
	return has_solution;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Solver_SE_ST::solve(AF &framework, list<uint32_t> &proof_extension, uint16_t numCores)
{
	ArrayBitSet initial_reduct;
	if (numCores == 1) {
		initial_reduct = PreProc_GR::process_only_grounded(framework, proof_extension);
	}
	else {
		initial_reduct = PreProc_GR_parallel::process_only_grounded(framework, proof_extension);
	}
	
	//check if grounded extension is stable
	if (initial_reduct._array.size() == 0) {
		//reduct is empty, therefore grounded extension is only stable extension
		return true;
	}
	return start_checking(framework, initial_reduct, proof_extension);
}