#include "../../include/logic/Solver_DS_PR.h"

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

/// <summary>
/// This method searched for complete sets in a specified state of a framework, which attack the specified query argument
/// </summary>
static bool search_complete_sets_in_state(AF &framework, ArrayBitSet &reduct, uint32_t query_argument,	
	std::__cxx11::list<uint32_t> &out_certificate_extension, ConeOfInfluence &coi, bool &is_query_attacked)
{
	// initialize SATSolver
	SatSolver *solver = NULL;
	solver = new SatSolver();
	// add encoding for nonempty complete sets to the SATSolver
	Encoding::add_clauses_nonempty_complete_set(*solver, framework, reduct);
	bool continue_calculation = false;
	bool is_rejected = false;
	// calculate a set of arguments by solving the SAT problem
	list<uint32_t> calculated_set = Proc_DS_PR::calculate_rejecting_set(query_argument, framework, reduct, is_rejected, is_query_attacked,
		*solver, continue_calculation, true);
	// update the certificate if such a set was found
	if (is_rejected) {
		tools::Tools_Solver::UpdateCertificate(out_certificate_extension, calculated_set);
	}
	// iterate through all possible sets in the state
	while (continue_calculation && !is_rejected) {
		// calculate a set of arguments by solving the SAT problem
		calculated_set = Proc_DS_PR::calculate_rejecting_set(query_argument, framework, reduct, is_rejected, is_query_attacked,
			*solver, continue_calculation, false);
		// update the certificate if such a set was found
		if (is_rejected) {
			tools::Tools_Solver::UpdateCertificate(out_certificate_extension, calculated_set);
		}
	}
	delete solver;
	return !is_rejected;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Solver_DS_PR::solve(uint32_t query_argument, AF &framework, list<uint32_t> &out_certificate_extension)
{
	// initialize variables
	ArrayBitSet active_args_in_coi = ArrayBitSet();
	pre_proc_result result_preProcessor;
	ConeOfInfluence coi(framework);
	list<uint32_t> grounded_extension;
	// preprocess the framework
	result_preProcessor = PreProc_GR::process(framework, query_argument, true, true, active_args_in_coi, grounded_extension, coi);
	tools::Tools_Solver::UpdateCertificate(out_certificate_extension, grounded_extension);

	switch (result_preProcessor) {

	case accepted:
		return true;

	case rejected:
		return false;

	default:
		bool is_query_attacked = false;
		bool is_skeptically_accepted = search_complete_sets_in_state(framework, active_args_in_coi, query_argument, out_certificate_extension, coi, is_query_attacked);
		return is_skeptically_accepted;
	}
}