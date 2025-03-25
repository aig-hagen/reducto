#include "../../include/logic/Solver_SE_ST.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool start_checking(AF &framework, ArrayBitSet &active_args, list<uint32_t> &out_certificate_extension)
{
	uint64_t numVars = active_args._array.size();
	SatSolver *solver = NULL;
	solver = new SatSolver(numVars);
	Encoding::add_clauses_nonempty_stable_set(*solver, framework, active_args);
	bool has_solution = (*solver).solve();
	if (has_solution) {
		tools::Tools_Solver::UpdateCertificate(solver, active_args, out_certificate_extension);
	}

	delete solver;
	return has_solution;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Solver_SE_ST::solve(AF &framework, list<uint32_t> &out_certificate_extension)
{
	ArrayBitSet initial_reduct;
	initial_reduct = PreProc_GR::process_only_grounded(framework, out_certificate_extension);
		
	//check if grounded extension is stable
	if (initial_reduct._array.size() == 0) {
		//reduct is empty, therefore grounded extension is only stable extension
		return true;
	}
	return start_checking(framework, initial_reduct, out_certificate_extension);
}