#include "../../include/logic/Solver_DC_CO.h"

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool start_checking(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, list<uint32_t> &out_certificate_extension) {
	// initialize SATSolver
	SAT_Solver solver = SAT_Solver(framework.num_args);
	// add encoding for nonempty complete sets to SATSolver
	Encoding::add_clauses_nonempty_complete_set(solver, framework, active_args);
	// compute solution using SATSolver
	bool has_solution_with_query = solver.solve(Encoding::get_literal_accepted(query_argument, true),
		Encoding::get_literal_rejected(framework, query_argument, false));
	// update certificate if solution was found
	if (has_solution_with_query) {
		tools::Tools_Solver::UpdateCertificate(solver, active_args, out_certificate_extension);
	}
	
	return has_solution_with_query;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Solver_DC_CO::solve(uint32_t query_argument, AF &framework, list<uint32_t> &out_certificate_extension) {

	ArrayBitSet initial_reduct = ArrayBitSet();
	pre_proc_result result_preProcessor;
	ConeOfInfluence coi(framework);

	result_preProcessor = PreProc_GR::process(framework, query_argument, false, true, initial_reduct, out_certificate_extension, coi);
	
	switch (result_preProcessor) {

	case accepted:
		return true;

	case rejected:
		return false;

	case unknown:
		return start_checking(query_argument, framework, initial_reduct, out_certificate_extension);

	default:
		return start_checking(query_argument, framework, initial_reduct, out_certificate_extension);
	}
}