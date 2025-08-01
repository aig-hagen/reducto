#include "../../include/logic/Solver_DC_CO.h"

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool start_checking(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, list<uint32_t> &out_certificate_extension) {
	// initialize SATSolver
	SatSolver *solver = NULL;
	solver = new SatSolver();
	// add encoding for nonempty complete sets to SATSolver
	Encoding::add_clauses_nonempty_complete_set(*solver, framework, active_args);
	// compute solution using SATSolver
	bool has_solution_with_query = (*solver).solve(Encoding::get_literal_accepted(query_argument, true),
		Encoding::get_literal_rejected(framework, query_argument, false));
	// update certificate if solution was found
	if (has_solution_with_query) {
		tools::Tools_Solver::UpdateCertificate(solver, active_args, out_certificate_extension);
	}
	
	delete solver;
	return has_solution_with_query;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static acceptance_result apply_shortcuts(AF &framework, uint32_t query_argument, ArrayBitSet &out_reduct, std::__cxx11::list<uint32_t> &out_grounded_extension)
{
	if (framework.self_attack[query_argument])
	{
		return acceptance_result::rejected;
	}

	if (framework.attackers[query_argument].empty())
	{
		return acceptance_result::accepted;
	}

	ArrayBitSet initial_actives = framework.create_active_arguments();
	return Solver_GR::reduce_by_grounded(framework, initial_actives, query_argument, false, true, out_reduct, out_grounded_extension);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/


bool Solver_DC_CO::solve(uint32_t query_argument, AF &framework, list<uint32_t> &out_certificate_extension) {
	ArrayBitSet reduct_after_grounded = ArrayBitSet();
	switch (apply_shortcuts(framework, query_argument, reduct_after_grounded, out_certificate_extension)) {
		case accepted:
			return true;

		case rejected:
			return false;

		case unknown:
			return start_checking(query_argument, framework, reduct_after_grounded, out_certificate_extension);

		default:
			return start_checking(query_argument, framework, reduct_after_grounded, out_certificate_extension);
	}
}