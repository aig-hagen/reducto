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
		for (std::list<uint32_t>::iterator mIter = calculated_extension.begin(); mIter != calculated_extension.end(); ++mIter) {
			solver->add_clause_short(Encoding::get_literal_accepted(*mIter, true), NULL);
			solver->add_clause_short(Encoding::get_literal_rejected(framework, *mIter, false), NULL);
		}

		//ensure that solver does not find same solution again
		Encoding::add_complement_clause(*solver, active_args);

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