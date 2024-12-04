#include "../../include/logic/Solver_DS_PR.h"

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void check_rejection(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, bool &is_rejected, bool &is_terminated,
	list<uint32_t> &extension_build, list<uint32_t> &certificate_extension, IPrioHeuristic &heuristic,
	PriorityStackManager &prio_queue, bool isMain)
{
	if (tools::ToolsOMP::check_termination(is_terminated, true)) return;
	ArrayBitSet reduct = extension_build.empty() ? active_args.copy() : Reduct::get_reduct_set(active_args, framework, extension_build);
	if (reduct._array.size() < 2)
	{
		// there is only 1 active argument, this has to be the argument to check, 
		// if not then there should have been a rejection check earlier who did not work
		return;
	}
	
	uint64_t numVars = reduct._array.size();
	SatSolver* solver = NULL;
	solver = new SatSolver_cadical(numVars);
	if (isMain) {
		Encoding::add_clauses_nonempty_admissible_set(*solver, framework, reduct);
	}
	else {
		Encoding::add_clauses_nonempty_complete_set(*solver, framework, reduct);
	}
	
	bool continue_calculation = false;
	list<uint32_t> calculated_set = Proc_DS_PR::calculate_rejecting_set(query_argument, framework, reduct, is_rejected, is_terminated,
		*solver, continue_calculation, true);
	list<uint32_t> new_extension = tools::ToolList::extend_list(extension_build, calculated_set);
	if (is_rejected) Tools_Solver::UpdateCertificate(certificate_extension, new_extension);
	if (!tools::ToolsOMP::check_termination(is_terminated, continue_calculation)) {
		prio_queue.try_insert_extension(query_argument, framework, &heuristic, new_extension, calculated_set);
		new_extension.clear();

		//iterate through initial sets
		do {
			Encoding::add_complement_clause(*solver, reduct);
			calculated_set = Proc_DS_PR::calculate_rejecting_set(query_argument, framework, reduct, is_rejected, is_terminated,
				*solver, continue_calculation, false);
			list<uint32_t> new_extension_2 = tools::ToolList::extend_list(extension_build, calculated_set);
			if (is_rejected) Tools_Solver::UpdateCertificate(certificate_extension, new_extension_2);
			if (tools::ToolsOMP::check_termination(is_terminated, continue_calculation)) break;
			
			prio_queue.try_insert_extension(query_argument, framework, &heuristic, new_extension_2, calculated_set);
			new_extension_2.clear();
			
		} while (!tools::ToolsOMP::check_termination(is_terminated, continue_calculation));
	}

	delete solver;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void check_rejection(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, bool &is_rejected, bool &is_terminated,
	list<uint32_t> &extension_build, list<uint32_t> &output_extension, IPrioHeuristic &heuristic,
	PriorityStackManager &prio_queue)
{
	if (check_termination(is_terminated, true)) return;
	ArrayBitSet reduct = extension_build.empty() ? active_args.copy() : Reduct::get_reduct_set(active_args, framework, extension_build);
	if (reduct._array.size() < 2)
	{
		// there is only 1 active argument, this has to be the argument to check, 
		// if not then there should have been a rejection check earlier who did not work
		return;
	}

	//Preprocess created reduct
	ArrayBitSet preprocessed_reduct = ArrayBitSet();
	pre_proc_result result_preProcessor = PreProc_DS_PR::process_reduct(framework, reduct, query_argument, preprocessed_reduct);

	switch (result_preProcessor) {
	case accepted:
		break;

	case rejected:
		set_is_rejected(is_rejected, is_terminated);
		output_extension = extension_build;
		break;

	default:
		check_reduct(preprocessed_reduct, framework, query_argument, is_rejected, is_terminated, extension_build, output_extension, prio_queue, heuristic);
		break;
	}
	
	return;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool start_checking_rejection(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, list<uint32_t> &certificate_extension, uint16_t numCores)
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
 firstprivate(query_argument, framework, active_args, heuristic)
	{
#pragma omp single nowait
		{
			list<uint32_t> extension_build;
			check_rejection(query_argument, framework, active_args, is_rejected, is_terminated, extension_build, certificate_extension,
				*heuristic, prio_stack, true);
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

				check_rejection(query_argument, framework, active_args, is_rejected, is_terminated, extension, certificate_extension,
					*heuristic, prio_stack, false);
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
	if (numCores == 1) {
		result_preProcessor = PreProc_GR::process(framework, query_argument, true, true, initial_reduct, certificate_extension);
	}
	else {
		result_preProcessor = PreProc_GR_parallel::process(framework, query_argument, true, true, initial_reduct, certificate_extension);
	}
	

	switch (result_preProcessor){

		case accepted:
			return true;

		case rejected:
			return false;

		default:
			return !start_checking_rejection(query_argument, framework, initial_reduct, certificate_extension, numCores);
	}
}