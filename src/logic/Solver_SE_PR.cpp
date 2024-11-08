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
	if (has_solution) {
		//extend complete extension to get preferred extension
		ArrayBitSet reduct = active_args.copy();
		while (has_solution) {
			list<uint32_t> calculated_extension = Decoding::get_set_from_solver(*solver, reduct);
			list<uint32_t> new_proof = tools::ToolList::extend_list(proof_extension, calculated_extension);
			proof_extension = new_proof;

			ArrayBitSet new_reduct = Reduct::get_reduct_set(reduct, framework, calculated_extension);
			if (new_reduct._array.empty()) {
				//no more arguments left, therefor preferred extension is found
				break;
			}
			reduct = new_reduct;
			SatSolver *new_solver = NULL;
			new_solver = new SatSolver_cadical(reduct._array.size());
			Encoding::add_clauses_nonempty_complete_set(*new_solver, framework, reduct);
			has_solution = (*new_solver).solve();
			delete new_solver;
			//if has_solution == false, then no more complete set can be calculated, therefor extension found so far cannot be extended, and is therefor preferred
		}
	}
	
	delete solver;
	return calculated_extension;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Solver_SE_PR::solve(AF &framework, list<uint32_t> &proof_extension)
{
	ArrayBitSet initial_reduct = PreProc_GR::process(framework, proof_extension);
	return start_checking(framework, initial_reduct, proof_extension);
}