#include "../../include/logic/Solver_DS_PR.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/


static bool check_termination_condition(bool &is_terminated, bool continue_calculation) {
	return tools::ToolsOMP::check_termination(is_terminated) || !continue_calculation;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void process_sat_solution(bool has_found_set, std::__cxx11::list<uint32_t> &extension_build, std::__cxx11::list<uint32_t> &calculated_set,
	bool &is_rejected, std::__cxx11::list<uint32_t> &certificate_extension, PriorityStackManager &prio_queue,
	uint32_t query_argument, AF &framework, IPrioHeuristic &heuristic, ConeOfInfluence &coi)
{
	if (has_found_set) {
		list<uint32_t> new_extension = tools::ToolList::extend_list(extension_build, calculated_set);
		if (is_rejected) {
			Tools_Solver::UpdateCertificate(certificate_extension, new_extension);
		}
		else {
			prio_queue.try_insert_extension(query_argument, framework, &heuristic, new_extension, calculated_set, coi);
		}

		//do not clear extension, since it is used as attribute of the object of class prio_stack_entry
	}
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void search_complete_sets_in_state(AF &framework, ArrayBitSet &reduct, uint32_t query_argument, bool &is_rejected, bool &is_terminated,
	std::__cxx11::list<uint32_t> &extension_build, std::__cxx11::list<uint32_t> &certificate_extension, PriorityStackManager &prio_queue,
	IPrioHeuristic &heuristic, ConeOfInfluence &coi)
{
	//calculate set in state
	uint64_t numVars = reduct._array.size();
	SatSolver *solver = NULL;
	solver = new SatSolver_cadical(numVars);
	Encoding::add_clauses_nonempty_complete_set(*solver, framework, reduct);
	bool continue_calculation = false;
	list<uint32_t> calculated_set = Proc_DS_PR::calculate_rejecting_set(query_argument, framework, reduct, is_rejected, is_terminated,
		*solver, continue_calculation, true);
	process_sat_solution(continue_calculation, extension_build, calculated_set, is_rejected, certificate_extension,
		prio_queue, query_argument, framework, heuristic, coi);

	while (!check_termination_condition(is_terminated, continue_calculation)) {
		//iterate through additional sets in state
		Encoding::add_complement_clause(*solver, reduct);
		calculated_set = Proc_DS_PR::calculate_rejecting_set(query_argument, framework, reduct, is_rejected, is_terminated,
			*solver, continue_calculation, false);
		process_sat_solution(continue_calculation, extension_build, calculated_set, is_rejected, certificate_extension,
			prio_queue, query_argument, framework, heuristic, coi);
	}
	delete solver;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static list<ArgumentDistancePair> create_candidates_coi(ArrayBitSet &active_args, uint32_t query_argument, ConeOfInfluence &coi) {
	list<ArgumentDistancePair> output_list_candidates;
	uint32_t limit_distance = coi.Max_distance / 2;

	for (int i = 0; i < active_args._array.size(); i++) {
		uint32_t argument = active_args._array[i];
		if (argument == query_argument || coi.Distance_to_query[argument] > limit_distance) continue;

		output_list_candidates.push_back(ArgumentDistancePair(argument, coi));
	}

	return output_list_candidates;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void search_adm_set_per_coi(AF &framework, ArrayBitSet &active_args, uint32_t query_argument, bool &is_rejected, bool &is_terminated,
	std::list<uint32_t> &extension_build,
	std::list<uint32_t> &certificate_extension, PriorityStackManager &prio_queue, IPrioHeuristic &heuristic, ConeOfInfluence &coi)
{
	//create list of all active arguments
	std::list<ArgumentDistancePair> list_candidates_to_start_coi = create_candidates_coi(active_args, query_argument, coi);
	list_candidates_to_start_coi.sort(ArgumentDistancePair::compare_by_distance);

	while (!list_candidates_to_start_coi.empty() && !tools::ToolsOMP::check_termination(is_terminated)) {
		//pop first argument in list and calculate COI of argument
		uint32_t argument = list_candidates_to_start_coi.front().Argument;
		//calculate COI of argument and remove all arguments in COI from list
		ArrayBitSet reduct = PreProc_GR::calculate_cone_influence_reduct(framework, active_args, argument, list_candidates_to_start_coi, coi);

		//search for set in COI
		uint64_t numVars = reduct._array.size();
		SatSolver *solver = NULL;
		solver = new SatSolver_cadical(numVars);
		bool has_found_set = false;
		Encoding::add_clauses_nonempty_admissible_set(*solver, framework, reduct);
		list<uint32_t> calculated_set = Proc_DS_PR::calculate_rejecting_set_in_random_coi(query_argument, framework, reduct, is_rejected, is_terminated,
			*solver, has_found_set);
		process_sat_solution(has_found_set, extension_build, calculated_set, is_rejected, certificate_extension,
			prio_queue, query_argument, framework, heuristic, coi);
		delete solver;
	}
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static pre_proc_result preprocess_state(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, bool &is_rejected, bool &is_terminated,
	list<uint32_t> &extension_build, list<uint32_t> &certificate_extension, IPrioHeuristic &heuristic,
	PriorityStackManager &prio_queue, ArrayBitSet &out_preprocessed_reduct)
{
	//create reduct
	ArrayBitSet reduct = Reduct::get_reduct_set(active_args, framework, extension_build);
	if (reduct._array.size() < 2)
	{
		// there is only 1 active argument, this has to be the argument to check, 
		// if not then there should have been a rejection check earlier who did not work
		return pre_proc_result::accepted;
	}

	//preprocess created reduct
	return PreProc_GR::reduce_by_grounded(framework, reduct, query_argument, true, false,
		out_preprocessed_reduct, extension_build);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void process_state(pre_proc_result result_preProcessor, bool &is_rejected, bool &is_terminated,
	std::__cxx11::list<uint32_t> &certificate_extension, std::__cxx11::list<uint32_t> &extension_build, AF &framework,
	ArrayBitSet &reduct, uint32_t query_argument, PriorityStackManager &prio_queue, IPrioHeuristic &heuristic, ConeOfInfluence &coi)
{
	switch (result_preProcessor) {
	case accepted:
		break;

	case rejected:
		tools::ToolsOMP::set_is_rejected(is_rejected, is_terminated);
		Tools_Solver::UpdateCertificate(certificate_extension, extension_build);
		break;

	default:
		search_complete_sets_in_state(framework, reduct, query_argument, is_rejected, is_terminated, extension_build,
			certificate_extension, prio_queue, heuristic, coi);
		break;
	}
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void generate_starting_points(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, bool &is_rejected, bool &is_terminated,
	list<uint32_t> &certificate_extension, IPrioHeuristic &heuristic,
	PriorityStackManager &prio_queue, ConeOfInfluence &coi) {
	list<uint32_t> extension_build;
	//search for one adm set per each COI
	search_adm_set_per_coi(framework, active_args, query_argument, is_rejected, is_terminated, extension_build,
		certificate_extension, prio_queue, heuristic, coi);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void search_for_rejecting_sets_in_origin_state(AF &framework, ArrayBitSet &active_args, uint32_t query_argument, bool &is_rejected,
	bool &is_terminated, std::__cxx11::list<uint32_t> &certificate_extension, PriorityStackManager &prio_stack,
	IPrioHeuristic *heuristic, bool &is_finished, ConeOfInfluence &coi)
{
	list<uint32_t> extension;
	search_complete_sets_in_state(framework, active_args, query_argument, is_rejected, is_terminated, extension,
		certificate_extension, prio_stack, *heuristic, coi);
}
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void search_for_rejecting_sets_in_reduct(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, bool &is_rejected, bool &is_terminated,
	std::__cxx11::list<uint32_t> &extension, std::__cxx11::list<uint32_t> &certificate_extension, IPrioHeuristic *heuristic,
	PriorityStackManager &prio_stack, ConeOfInfluence &coi)
{
	if (tools::ToolsOMP::check_termination(is_terminated)) return;
	if (extension.empty()) return;
	ArrayBitSet preprocessed_reduct;
	pre_proc_result res_preproc_reduct = preprocess_state(query_argument, framework, active_args, is_rejected, is_terminated, extension,
		certificate_extension, *heuristic, prio_stack, preprocessed_reduct);
	process_state(res_preproc_reduct, is_rejected, is_terminated, certificate_extension, extension, framework, preprocessed_reduct,
		query_argument, prio_stack, *heuristic, coi);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool start_checking_rejection(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, list<uint32_t> &certificate_extension,
	uint16_t numCores, ConeOfInfluence &coi)
{
	if (numCores > 0)
	{
		omp_set_num_threads(numCores);
	}
	IPrioHeuristic *heuristic = NULL;
	heuristic = new Heuristic5();
	PriorityStackManager prio_stack;
	bool is_terminated = false;
	bool is_finished = false;
	bool is_rejected = false;
	omp_set_lock(prio_stack.lock_has_entry);
#pragma omp parallel shared(is_rejected, is_terminated, is_finished, certificate_extension, prio_stack) \
 firstprivate(query_argument, framework, active_args, heuristic, coi)
	{
#pragma omp single nowait
		{
			generate_starting_points(query_argument, framework, active_args, is_rejected, is_terminated, certificate_extension,
				*heuristic, prio_stack, coi);
			if (!tools::ToolsOMP::check_finished(is_finished, prio_stack)) {
				search_for_rejecting_sets_in_origin_state(framework, active_args, query_argument, is_rejected, is_terminated, certificate_extension,
					prio_stack, heuristic, is_finished, coi);
			}
			tools::ToolsOMP::update_is_finished(is_terminated, is_finished, prio_stack);
		}

		while (true) {
			omp_set_lock(prio_stack.lock_has_entry);
			if (tools::ToolsOMP::check_finished(is_finished, prio_stack)) {
				omp_unset_lock(prio_stack.lock_has_entry);
				break;
			}

			if (prio_stack.check_number_unprocessed_elements() > 0) {
				list<uint32_t> extension = prio_stack.pop_prio_extension();

				if (prio_stack.check_number_unprocessed_elements() > 0) {
					omp_unset_lock(prio_stack.lock_has_entry);
				}

				search_for_rejecting_sets_in_reduct(query_argument, framework, active_args, is_rejected, is_terminated, extension, certificate_extension,
					heuristic, prio_stack, coi);
				tools::ToolsOMP::update_is_finished(is_terminated, is_finished, prio_stack);
			}
			else {
				cout << "start_checking_rejection::exception queue empty but not finished" << endl;
				throw new exception();
			}
		}
	}

	delete heuristic;
	return is_rejected;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Solver_DS_PR::solve(uint32_t query_argument, AF &framework, list<uint32_t> &certificate_extension, uint16_t numCores)
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
		return !start_checking_rejection(query_argument, framework, initial_reduct, certificate_extension, numCores, coi);
	}
}