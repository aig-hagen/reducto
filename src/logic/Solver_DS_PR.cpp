#include "../../include/logic/Solver_DS_PR.h"

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void check_rejection(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, bool &is_rejected, bool &is_terminated,
	list<uint32_t> &extension_build, list<uint32_t> &output_extension, IPrioHeuristic &heuristic,
	PriorityStackManager &prio_queue)
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
	SatSolver *solver = NULL;
	solver = new SatSolver_cadical(numVars);
	Encoding::add_clauses_nonempty_complete_set(*solver, framework, reduct);
	bool continue_calculation = false;
	list<uint32_t> initial_set = Proc_DS_PR::calculate_rejecting_set(query_argument, framework, reduct, is_rejected, is_terminated,
		*solver, continue_calculation, true);
	list<uint32_t> new_extension = tools::ToolList::extend_list(extension_build, initial_set);
	if (is_rejected) output_extension = new_extension;
	if (!tools::ToolsOMP::check_termination(is_terminated, continue_calculation)) {
		prio_queue.try_insert_extension(query_argument, framework, &heuristic, new_extension, initial_set);

		//iterate through initial sets
		do {
			Encoding::add_complement_clause(*solver, reduct);
			initial_set = Proc_DS_PR::calculate_rejecting_set(query_argument, framework, reduct, is_rejected, is_terminated,
				*solver, continue_calculation, false);
			list<uint32_t> new_extension_2 = tools::ToolList::extend_list(extension_build, initial_set);
			if (is_rejected) output_extension = new_extension_2;
			if (tools::ToolsOMP::check_termination(is_terminated, continue_calculation)) break;

			prio_queue.try_insert_extension(query_argument, framework, &heuristic, new_extension_2, initial_set);
			
		} while (!tools::ToolsOMP::check_termination(is_terminated, continue_calculation));
	}

	delete solver;
	return;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool start_checking_rejection(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, list<uint32_t> &proof_extension, uint16_t numCores)
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
#pragma omp parallel shared(is_rejected, is_terminated, is_finished, proof_extension, prio_stack) \
 firstprivate(query_argument, framework, active_args, heuristic)
	{
#pragma omp single nowait
		{
			list<uint32_t> extension_build;
			check_rejection(query_argument, framework, active_args, is_rejected, is_terminated, extension_build, proof_extension,
				*heuristic, prio_stack);
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

				check_rejection(query_argument, framework, active_args, is_rejected, is_terminated, extension, proof_extension,
					*heuristic, prio_stack);
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

bool Solver_DS_PR::solve(uint32_t query_argument, AF &framework, list<uint32_t> &proof_extension, uint16_t numCores)
{
	ArrayBitSet initial_reduct = ArrayBitSet();
	pre_proc_result result_preProcessor = PreProc_GR::process(framework, query_argument, initial_reduct, proof_extension);

	switch (result_preProcessor){

		case accepted:
			return true;

		case rejected:
			return false;

		case unknown:
			return !start_checking_rejection(query_argument, framework, initial_reduct, proof_extension, numCores);

		default:
			return unknown;
	}
}