#include "../../include/logic/Solver_SE_PR.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Solver_SE_PR::solve(AF &framework, ArrayBitSet &active_args, list<uint32_t> &proof_extension)
{
	ArrayBitSet reduct = active_args.copy();
	SatSolver *solver = NULL;
	solver = new SatSolver(framework.num_args);
	Encoding::add_clauses_nonempty_complete_set(*solver, framework, reduct);
	bool has_solution = (*solver).solve();
	//extend complete extension to get preferred extension
	while (has_solution) {
		list<uint32_t> calculated_extension = Decoding::get_set_from_solver(*solver, reduct);
		tools::Tools_Solver::UpdateCertificate(proof_extension, calculated_extension);
		ArrayBitSet new_reduct = Reduct::get_reduct_set(reduct, framework, calculated_extension);
		reduct = new_reduct;
		if (reduct._array.empty()) {
			//no more arguments left, therefor preferred extension is found
			break;
		}
		// calculate complete extension in new reduct
		delete solver;
		solver = new SatSolver(framework.num_args);
		Encoding::add_clauses_nonempty_complete_set(*solver, framework, reduct);
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
		
	return solve(framework, initial_reduct, proof_extension);
}