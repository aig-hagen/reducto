#include "../../include/logic/Solver_DS_PR.h"

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool check_finished(bool &is_finished, PriorityStackManager &prio_stack) {
	bool is_finished_tmp = false;
#pragma omp flush(is_finished)
#pragma omp atomic read
	is_finished_tmp = is_finished;
	if (is_finished_tmp) {
		omp_unset_lock(prio_stack.lock_has_entry);
	}

	return is_finished_tmp;
}
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool check_termination(bool &is_terminated, bool continue_calculation)
{
	bool is_terminated_tmp;
#pragma omp flush(is_terminated)
#pragma omp atomic read
	is_terminated_tmp = is_terminated;
	return is_terminated_tmp || !continue_calculation;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void update_is_finished(bool &is_terminated, bool &is_finished, PriorityStackManager &prio_stack)
{
	bool is_finished_tmp = prio_stack.check_number_unprocessed_elements() == 0 || check_termination(is_terminated, true);
#pragma omp atomic write
	is_finished = is_finished_tmp;
#pragma omp flush(is_finished)
	if (is_finished_tmp) {
		omp_unset_lock(prio_stack.lock_has_entry);
	}
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void check_rejection(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, bool &is_rejected, bool &is_terminated,
	list<uint32_t> &extension_build, list<uint32_t> &output_extension, IPrioHeuristic &heuristic,
	PriorityStackManager &prio_queue, uint16_t limit_calculations_iniSet, bool isMain)
{
	if (check_termination(is_terminated, true)) return;
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
	Encoding::add_clauses_nonempty_admissible_set(*solver, framework, reduct);
	bool continue_calculation = false;
	bool found_counter_evidence = false;
	list<uint32_t> initial_set = Proc_DS_PR::calculate_nonempty_adm_set(query_argument, framework, reduct, is_rejected, is_terminated,
		*solver, continue_calculation, found_counter_evidence, true);
	list<uint32_t> new_extension = tools::ToolList::extend_list(extension_build, initial_set);
	if (found_counter_evidence) output_extension = new_extension;
	if (!check_termination(is_terminated, continue_calculation)) {
		prio_queue.try_insert_extension(query_argument, framework, &heuristic, new_extension, initial_set);

		uint16_t num_iterations = 0;
		//iterate through initial sets
		do {
			Encoding::add_complement_clause(*solver, reduct);
			initial_set = Proc_DS_PR::calculate_nonempty_adm_set(query_argument, framework, reduct, is_rejected, is_terminated,
				*solver, continue_calculation, found_counter_evidence, false);
			list<uint32_t> new_extension_2 = tools::ToolList::extend_list(extension_build, initial_set);
			if (found_counter_evidence) output_extension = new_extension_2;
			if (check_termination(is_terminated, continue_calculation)) break;

			prio_queue.try_insert_extension(query_argument, framework, &heuristic, new_extension_2, initial_set);
			if (!isMain)
			{
				num_iterations++;
			}
		} while (!check_termination(is_terminated, continue_calculation) && (isMain || num_iterations < limit_calculations_iniSet));
	}

	delete solver;
	return;
}
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void check_existance_accepting_solution(uint32_t query_argument, AF& framework, ArrayBitSet& active_args, bool& is_rejected,
	bool& is_terminated) {
	uint64_t numVars = active_args._array.size();
	SatSolver* solver = NULL;
	solver = new SatSolver_cadical(numVars);
	Encoding::add_clauses_nonempty_admissible_set(*solver, framework, active_args);
	Proc_DS_PR::check_existance_accepting_solution(query_argument, framework, active_args, is_rejected, is_terminated, *solver);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/


static void check_existance_counter_evidence(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, bool &is_rejected, 
	bool &is_terminated, list<uint32_t> &output_extension) {
	uint64_t numVars = active_args._array.size();
	SatSolver *solver = NULL;
	solver = new SatSolver_cadical(numVars);
	Encoding::add_clauses_nonempty_admissible_set(*solver, framework, active_args);
	bool found_counter_evidence = false;
	list<uint32_t> counter_evidence = Proc_DS_PR::calculate_counter_evidence(query_argument, framework, active_args, is_rejected, is_terminated,
		*solver, found_counter_evidence);
	if (found_counter_evidence) output_extension = counter_evidence;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool start_checking_rejection(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, list<uint32_t> &proof_extension, uint16_t numCores,
	uint16_t limit_calculations_iniSet)
{
	if (numCores > 0)
	{
		omp_set_num_threads(numCores);
	}
	IPrioHeuristic *heuristic = NULL;
	heuristic = new Heuristic1();
	PriorityStackManager prio_stack;
	bool is_terminated = false;
	bool is_finished = false;
	bool is_rejected = false;
	omp_set_lock(prio_stack.lock_has_entry);
#pragma omp parallel shared(is_rejected, is_terminated, is_finished, proof_extension, prio_stack) firstprivate(query_argument, framework, active_args, heuristic, limit_calculations_iniSet)
	{
#pragma omp sections nowait
		{
#pragma omp section
			{
				list<uint32_t> extension_build;
				check_rejection(query_argument, framework, active_args, is_rejected, is_terminated, extension_build, proof_extension,
					*heuristic, prio_stack, limit_calculations_iniSet, true);
				update_is_finished(is_terminated, is_finished, prio_stack);
			}
#pragma omp section
			{
				check_existance_accepting_solution(query_argument, framework, active_args, is_rejected, is_terminated);
				update_is_finished(is_terminated, is_finished, prio_stack);
			}
#pragma omp section
			{
				check_existance_counter_evidence(query_argument, framework, active_args, is_rejected, is_terminated);
				update_is_finished(is_terminated, is_finished, prio_stack);
			}
		}

		while (true) {
			omp_set_lock(prio_stack.lock_has_entry);
			if (check_finished(is_finished, prio_stack)) {
				omp_unset_lock(prio_stack.lock_has_entry);
				break;
			} 
			
			if (prio_stack.check_number_unprocessed_elements() > 0) {
				list<uint32_t> extension = prio_stack.pop_prio_extension();

				if (prio_stack.check_number_unprocessed_elements() > 0) {
					omp_unset_lock(prio_stack.lock_has_entry);
				}

				check_rejection(query_argument, framework, active_args, is_rejected, is_terminated, extension, proof_extension,
					*heuristic, prio_stack, limit_calculations_iniSet, false);
				update_is_finished(is_terminated, is_finished, prio_stack);
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

bool Solver_DS_PR::solve(uint32_t query_argument, AF &framework, list<uint32_t> &proof_extension, uint16_t numCores, uint16_t limit_calculations_iniSet)
{
	ArrayBitSet initial_reduct = ArrayBitSet();
	pre_proc_result result_preProcessor = PreProc_DS_PR::process(framework, query_argument, initial_reduct);

	switch (result_preProcessor){

		case accepted:
			return true;

		case rejected:
			return false;

		case unknown:
			return !start_checking_rejection(query_argument, framework, initial_reduct, proof_extension, numCores, limit_calculations_iniSet);

		default:
			return unknown;
	}
}