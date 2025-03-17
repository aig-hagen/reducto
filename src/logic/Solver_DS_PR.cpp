#include "../../include/logic/Solver_DS_PR.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool search_complete_sets_in_state(AF &framework, ArrayBitSet &reduct, uint32_t query_argument,	
	std::__cxx11::list<uint32_t> &certificate_extension, ConeOfInfluence &coi, bool &is_query_attacked)
{
	//calculate set in state
	uint64_t numVars = reduct._array.size();
	SatSolver *solver = NULL;
	solver = new SatSolver(numVars);
	Encoding::add_clauses_nonempty_complete_set(*solver, framework, reduct);
	bool continue_calculation = false;
	bool is_rejected = false;
	list<uint32_t> calculated_set = Proc_DS_PR::calculate_rejecting_set(query_argument, framework, reduct, is_rejected, is_query_attacked,
		*solver, continue_calculation, true);
	if (is_rejected) {
		tools::Tools_Solver::UpdateCertificate(certificate_extension, calculated_set);
	}

	while (continue_calculation && !is_rejected) {
		//iterate through additional sets in state
		calculated_set = Proc_DS_PR::calculate_rejecting_set(query_argument, framework, reduct, is_rejected, is_query_attacked,
			*solver, continue_calculation, false);
		if (is_rejected) {
			tools::Tools_Solver::UpdateCertificate(certificate_extension, calculated_set);
		}
	}
	delete solver;
	return is_rejected;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void complete_certificate(AF &framework, list<uint32_t> &certificate_extension) {
	ArrayBitSet original_active_args = framework.create_active_arguments();
	ArrayBitSet reduct = certificate_extension.empty() ? original_active_args : Reduct::get_reduct_set(original_active_args, framework, certificate_extension);
	if (reduct._array.empty()) {
		return;
	}
	list<uint32_t> extension_outside_certificate;
	bool has_extension_outside_certificate = Solver_SE_PR::solve(framework, reduct, extension_outside_certificate);
	if (has_extension_outside_certificate) {
		tools::Tools_Solver::UpdateCertificate(certificate_extension, extension_outside_certificate);
	}
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool start_checking_rejection(uint32_t query_argument, AF &framework, ArrayBitSet &active_args_in_coi,
	list<uint32_t> &grounded_extension, ConeOfInfluence &coi, list<uint32_t> &certificate_extension)
{
	bool is_query_attacked = false;
	bool is_rejected = search_complete_sets_in_state(framework, active_args_in_coi, query_argument, certificate_extension, coi, is_query_attacked);

	//if skeptical acceptance of query got rejected, but query is not attacked by certificate, then extend certificate to get complete PR extension in original AF
	if (is_rejected && !is_query_attacked) {
		complete_certificate(framework, certificate_extension);
		return is_rejected;
	}

	return is_rejected;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Solver_DS_PR::solve(uint32_t query_argument, AF &framework, list<uint32_t> &certificate_extension)
{
	ArrayBitSet active_args_in_coi = ArrayBitSet();
	pre_proc_result result_preProcessor;
	ConeOfInfluence coi(framework);
	list<uint32_t> grounded_extension;
	result_preProcessor = PreProc_GR::process(framework, query_argument, true, true, active_args_in_coi, grounded_extension, coi);
	tools::Tools_Solver::UpdateCertificate(certificate_extension, grounded_extension);

	switch (result_preProcessor) {

	case accepted:
		return true;

	case rejected:
		if (!tools::Tools_ArgsSet::check_attack(query_argument, certificate_extension, framework)) {
			complete_certificate(framework, certificate_extension);
		}
		return false;

	default:
		return !start_checking_rejection(query_argument, framework, active_args_in_coi, grounded_extension, coi, certificate_extension);
	}
}