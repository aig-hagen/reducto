#include "../../include/logic/Solver_SE_PR.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool start_checking(AF &framework, ArrayBitSet &active_args, list<uint32_t> &proof_extension)
{
	SatSolver *solver = NULL;
	solver = new SatSolver_cadical(active_args._array.size());
	Encoding::add_clauses_nonempty_complete_set(*solver, framework, active_args);
	bool has_solution = (*solver).solve();
	bool calculated_extension = has_solution ? true : !proof_extension.empty();
	//extend complete extension to get preferred extension
	while (has_solution) {
		list<uint32_t> calculated_extension = Decoding::get_set_from_solver(*solver, active_args);
		Tools_Solver::UpdateCertificate(proof_extension, calculated_extension);
		ArrayBitSet new_reduct = Reduct::get_reduct_set(active_args, framework, calculated_extension);
		active_args = new_reduct;
		if (active_args._array.empty()) {
			//no more arguments left, therefor preferred extension is found
			break;
		}
		// calculate complete extension in new reduct
		solver = new SatSolver_cadical(active_args._array.size());
		Encoding::add_clauses_nonempty_complete_set(*solver, framework, active_args);
		has_solution = (*solver).solve();
		//if has_solution == false, then no more complete set can be calculated, therefor extension found so far cannot be extended, and is therefor preferred
		calculated_extension.clear();
	}

	delete solver;
	return true;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Solver_SE_PR::solve(AF &framework, list<uint32_t> &proof_extension, uint16_t numCores)
{
	ArrayBitSet initial_reduct;
	if (numCores > 0) {
		initial_reduct = PreProc_GR_parallel::process_only_grounded(framework, proof_extension);
	}
	else {
		initial_reduct = PreProc_GR::process_only_grounded(framework, proof_extension);
	}
	
	return start_checking(framework, initial_reduct, proof_extension);
}