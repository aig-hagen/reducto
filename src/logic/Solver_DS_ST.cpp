#include "../../include/logic/Solver_DS_ST.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

/// <summary>
///  This method is used to check the skeptical acceptance after the framework has been preprocessed
/// </summary>
static bool check_for_stable_extension_without_query(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, list<uint32_t> &out_certificate_extension)
{
	// initialise the SATSolver
	SAT_Solver solver = SAT_Solver(framework.num_args);
	// add an encoding for nonempty stable sets
	Encoding::add_clauses_nonempty_stable_set(solver, framework, active_args);
	// compute a solution with the SATSolver
	bool has_solution_without_query = solver.solve(Encoding::get_literal_accepted(query_argument, false),
		Encoding::get_literal_rejected(framework, query_argument, true));
	// update the certificate iff a solution was found
	if (has_solution_without_query) {
		tools::Tools_Solver::UpdateCertificate(solver, active_args, out_certificate_extension);
	}
	//only two cases remain: 1. no stable solution is computable; 2. all stable solutions contain the query; both cases lead to scetical acceptance
		
	return has_solution_without_query;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Solver_DS_ST::solve(uint32_t query_argument, AF &framework, list<uint32_t> &out_certificate_extension)
{
	// preprocess the framework

	// check if query is unattacked
	if (framework.attackers[query_argument].empty())
	{
		return true;
	}

	// get the original state (active arguments) of the framework
	ArrayBitSet active_args = framework.create_active_arguments();
	ArrayBitSet initial_reduct = ArrayBitSet();
	// reduce by grounded extension
	pre_proc_result result_preProcessor = PreProc_GR::reduce_by_grounded(framework, active_args, query_argument, true, false, initial_reduct, out_certificate_extension);

	switch (result_preProcessor) {

	case accepted:
		return true;

	case rejected:
		if (initial_reduct._array.size() == 0) {
			//calculated grounded extension is the stable extension
			return false;
		}

		return !check_for_stable_extension_without_query(query_argument, framework, initial_reduct, out_certificate_extension);

	default:
		

		return !check_for_stable_extension_without_query(query_argument, framework, initial_reduct, out_certificate_extension);
	}
}