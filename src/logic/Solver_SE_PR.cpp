#include "../../include/logic/Solver_SE_PR.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool start_checking(AF &framework, ArrayBitSet &active_args, list<uint32_t> &proof_extension)
{
	SatSolver *solver = NULL;
	solver = new SatSolver(active_args._array.size());
	Encoding::add_clauses_nonempty_complete_set(*solver, framework, active_args);
	bool has_solution = (*solver).solve();
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
		solver = new SatSolver(active_args._array.size());
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

bool Solver_SE_PR::solve(AF &framework, list<uint32_t> &proof_extension)
{
	ArrayBitSet initial_reduct;
	initial_reduct = PreProc_GR::process_only_grounded(framework, proof_extension);
		
	return start_checking(framework, initial_reduct, proof_extension);
}