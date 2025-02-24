#include "../../include/logic/Solver_DS_PR.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void process_sat_solution(bool has_found_set, std::__cxx11::list<uint32_t> &extension_build, std::__cxx11::list<uint32_t> &calculated_set,
	bool &is_rejected, std::__cxx11::list<uint32_t> &certificate_extension,
	uint32_t query_argument, AF &framework, ConeOfInfluence &coi)
{
	if (has_found_set && is_rejected) {
		list<uint32_t> new_extension = tools::Tools_List::extend_list(extension_build, calculated_set);
		tools::Tools_Solver::UpdateCertificate(certificate_extension, new_extension);

		//do not clear extension, since it is used as attribute of the object of class prio_stack_entry
	}
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool search_complete_sets_in_state(AF &framework, ArrayBitSet &reduct, uint32_t query_argument,	
	std::__cxx11::list<uint32_t> &extension_build, 
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
	process_sat_solution(continue_calculation, extension_build, calculated_set, is_rejected, certificate_extension, 
		query_argument, framework, coi);

	while (continue_calculation && !is_rejected) {
		//iterate through additional sets in state
		Encoding::add_complement_clause(*solver, reduct);
		calculated_set = Proc_DS_PR::calculate_rejecting_set(query_argument, framework, reduct, is_rejected, is_query_attacked,
			*solver, continue_calculation, false);
		process_sat_solution(continue_calculation, extension_build, calculated_set, is_rejected, certificate_extension, 
			query_argument, framework, coi);
	}
	delete solver;
	return is_rejected;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool start_checking_rejection(uint32_t query_argument, AF &framework, ArrayBitSet &active_args_preprocessed, list<uint32_t> &certificate_extension,
	ConeOfInfluence &coi)
{
	list<uint32_t> extension;
	bool is_query_attacked = false;
	bool is_rejected = search_complete_sets_in_state(framework, active_args_preprocessed, query_argument, extension,
		certificate_extension, coi, is_query_attacked);

	//if skeptical acceptance of query got rejected, but query is not attacked by certificate, then extend certificate to get complete PR extension in original AF
	if (is_rejected && !is_query_attacked) {
		ArrayBitSet original_active_args = framework.create_active_arguments();
		list<uint32_t> list_coi = active_args_preprocessed.to_list();
		list<uint32_t> list_processed_args = tools::Tools_List::extend_list(list_coi, certificate_extension);
		ArrayBitSet reduct = Reduct::get_reduct_set(original_active_args, framework, list_processed_args);
		list<uint32_t> extension_outside_coi;
		bool has_extension_outside_coi = Solver_SE_PR::solve(framework, reduct, extension_outside_coi);
		if (has_extension_outside_coi) {
			tools::Tools_Solver::UpdateCertificate(certificate_extension, extension_outside_coi);
		}
	}

	return is_rejected;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Solver_DS_PR::solve(uint32_t query_argument, AF &framework, list<uint32_t> &certificate_extension)
{
	ArrayBitSet initial_reduct = ArrayBitSet();
	pre_proc_result result_preProcessor;
	ConeOfInfluence coi(framework);
	result_preProcessor = PreProc_GR::process(framework, query_argument, true, false, initial_reduct, certificate_extension, coi);

	switch (result_preProcessor) {

	case accepted:
		return true;

	case rejected:
		return false;

	default:
		return !start_checking_rejection(query_argument, framework, initial_reduct, certificate_extension, coi);
	}
}