#include "../../include/logic/Solver_SE_PR.h"

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Solver_SE_PR::solve(AF &framework, ArrayBitSet &active_args, list<uint32_t> &out_certificate_extension)
{
	// initialize the SATSolver
	ArrayBitSet reduct = active_args.copy();
	SatSolver *solver = NULL;
	solver = new SatSolver();
	// add an encoding for a nonempty complete set to the SATSolver
	Encoding::add_clauses_nonempty_complete_set(*solver, framework, reduct);
	bool has_solution = (*solver).solve();
	// extend the complete extension to get preferred extension
	// by repeating the process until no nonempty complete set can be found anymore
	while (has_solution) {
		// get the calculated extension and update the certificate
		list<uint32_t> calculated_extension = Decoding::get_set_from_solver(*solver, reduct);
		tools::Tools_Solver::UpdateCertificate(out_certificate_extension, calculated_extension);
		// reduce the current state by the calculated extension
		ArrayBitSet new_reduct = Reduct::get_reduct_set(reduct, framework, calculated_extension);
		reduct = new_reduct;
		if (reduct._array.empty()) {
			//no more arguments left, therefor preferred extension is found
			break;
		}
		// calculate a complete extension in the new reduct
		delete solver;
		solver = new SatSolver();
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

bool Solver_SE_PR::solve(AF &framework, list<uint32_t> &out_certificate_extension)
{
	ArrayBitSet initial_reduct;
	initial_reduct = PreProc_GR::process_only_grounded(framework, out_certificate_extension);
		
	return solve(framework, initial_reduct, out_certificate_extension);
}