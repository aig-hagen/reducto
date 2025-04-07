#include "../../include/logic/Solver_DC_ST.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool start_checking(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, list<uint32_t> &out_certificate_extension)
{
	// initialize SATSolver
	uint64_t numVars = framework.num_args;
	SatSolver *solver = NULL;
	solver = new SatSolver(numVars);
	// add encoding for nonempty stable sets to SATSolver
	Encoding::add_clauses_nonempty_stable_set(*solver, framework, active_args);
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

bool Solver_DC_ST::solve(uint32_t query_argument, AF &framework, list<uint32_t> &out_certificate_extension)
{
	ArrayBitSet initial_reduct = ArrayBitSet();
	pre_proc_result result_preProcessor;
	result_preProcessor = PreProc_GR::process_only_grounded(framework, query_argument, false, true, initial_reduct, out_certificate_extension);
	
	switch (result_preProcessor) {

	case rejected:
		return false;

	case accepted:
		if (initial_reduct._array.size() == 0) {
			//calculated grounded extension is the stable extension
			return true;
		}

		return tools::Tools_Solver::check_existance_stable_extension(framework, initial_reduct, out_certificate_extension);

	default:
		if (initial_reduct._array.size() == 0) {
			//calculated grounded extension is the stable extension
			cout << "ERROR should be impossible" << endl;
			return true;
		}

		return start_checking(query_argument, framework, initial_reduct, out_certificate_extension);
	}
}