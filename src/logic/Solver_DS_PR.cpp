#include "../../include/logic/Solver_DS_PR.h"
using namespace std;

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static list<uint32_t> pop_prio_queue(std::unordered_set<ExtensionPrioritised, PrioHash> &prio_set, vector<uint8_t> &task_flags, omp_lock_t *lock_task_flag) {
	for (int i = 0; i < prio_set.bucket_count(); i++) {
		//ensure starting to iterate at bucket 0
		for (auto iter = prio_set.begin(i); iter != prio_set.end(i); ++iter) {
			//iterate through elements of the bucket
			ExtensionPrioritised entry = *iter;
#pragma omp flush(task_flags)
			if (!task_flags[entry.Number]) {
				omp_set_lock(lock_task_flag);
				if (task_flags[entry.Number]) {
					omp_unset_lock(lock_task_flag);
					continue;
				}
				//found unprocessed entry
				list<uint32_t> result = entry.Extension;
				task_flags[entry.Number] = true;
				omp_unset_lock(lock_task_flag);
				return result;
			}
		}
	}
	
	//no unprocessed elements found
	return list<uint32_t>();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static uint64_t check_prio_queue_size(vector<uint8_t> &task_flags) {
#pragma omp flush(task_flags)
	if (task_flags.empty()) return 0;

	uint64_t num_false = 0;
	for (uint64_t i = task_flags.size() - 1; i >= 0 ; i--) {
		if (!task_flags[i]) num_false++;
		if (num_false >= 2) break;
	}
	return num_false;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void check_rejection(uint32_t argument, AF &framework, ArrayBitSet &activeArgs, bool &isRejected, bool &isTerminated,
	list<uint32_t> &extension_build, list<uint32_t> &output_extension, IPrioHeuristic &heuristic,
	std::unordered_set<ExtensionPrioritised, PrioHash> &extension_priority_queue,
	omp_lock_t *lock_prio_queue, omp_lock_t *lock_has_entry, vector<uint8_t> &task_flags)
{
	int id = omp_get_thread_num();
	bool isTerminated_tmp = false;

#pragma omp flush(isTerminated)
#pragma omp atomic read
	isTerminated_tmp = isTerminated;
	if (isTerminated_tmp)
	{
		return;
	}

	ArrayBitSet reduct = extension_build.empty() ? activeArgs.copy() : Reduct::get_reduct_set(activeArgs, framework, extension_build);

	if (reduct._array.size() < 2)
	{
		// there is only 1 active argument, this has to be the argument to check, 
		// if not then there should have been a rejection check earlier who did not work
		return;
	}

	//flag used to signal that clauses need to be extended
	bool *isSolved = NULL;
	isSolved = (bool *)malloc(sizeof * isSolved);
	*isSolved = false;
	//flag used to signal that reduct has only empty set as admissible set
	bool *isFirstCalculation = NULL;
	isFirstCalculation = (bool *)malloc(sizeof * isFirstCalculation);
	*isFirstCalculation = true;
	uint64_t numVars = reduct._array.size();
	SatSolver *solver = NULL;
	solver = new SatSolver_cadical(numVars);
	Encoding::add_clauses_nonempty_admissible_set(*solver, framework, reduct);

#pragma omp flush(isTerminated)
#pragma omp atomic read
	isTerminated_tmp = isTerminated;
	if (isTerminated_tmp == true)
	{
		free(isSolved);
		free(isFirstCalculation);
		delete solver;
		return;
	}

	bool has_Solution = true;

	//iterate through initial sets
	do {
		if (*isSolved)
		{
			Encoding::add_complement_clause(*solver, reduct);
		}

		*isSolved = true;
		has_Solution = (*solver).solve();

		if (!has_Solution)
		{
			//no more initial sets to calculate after this one
			if (*isFirstCalculation)
			{
				// since this is the first calculation and no IS was found, this reduct has only the empty set as an admissible set, 
				// therefore the extension_build is complete and since only extensions not containing the query argument proceed 
				// in the calculation, extension_build cannot contain the query argument, so that there exists
				// an extension not containing the query argument, so that the argument gets sceptical rejected				
#pragma atomic write
				isRejected = true;
#pragma omp flush(isRejected)
#pragma atomic write
				isTerminated = true;
#pragma omp flush(isTerminated)

				free(isSolved);
				free(isFirstCalculation);
				delete solver;
				return;
			}

			break;
		}

		list<uint32_t> initial_set = Decoding::get_set_from_solver(*solver, reduct);		
		if (initial_set.empty())
		{
			if (*isFirstCalculation)
			{
				// since this is the first calculation and only the empty set as IS was found, the extension_build is complete
				// and since only extensions not containing the query argument proceed in the calculation, extension_build 
				// cannot contain the query argument, so that there exists
				// an extension not containing the query argument, so that the argument gets sceptical rejected				
#pragma atomic write
				isRejected = true;
#pragma omp flush(isRejected)
#pragma atomic write
				isTerminated = true;
#pragma omp flush(isTerminated)
				free(isSolved);
				free(isFirstCalculation);
				delete solver;
				initial_set.clear();
				return;
			}
			else
			{
				printf("ERROR impossible that empty IS calculated and is not first calculation");
				exit(1);
			}
		}

		*isFirstCalculation = false;

		if (ScepticalCheck::check_rejection(argument, initial_set, framework))
		{
#pragma atomic write
			isRejected = true;
#pragma omp flush(isRejected)
#pragma atomic write
			isTerminated = true;
#pragma omp flush(isTerminated)

			list<uint32_t> new_extension_build = tools::ToolList::extend_list(extension_build, initial_set);
#pragma atomic write
			output_extension = new_extension_build;

			free(isSolved);
			free(isFirstCalculation);
			delete solver;
			initial_set.clear();
			return;
		}
		else if (ScepticalCheck::check_terminate_extension_build(argument, initial_set))
		{
			initial_set.clear();
			continue;
		}

		if (!initial_set.empty()) {
			list<uint32_t> new_extension_build = tools::ToolList::extend_list(extension_build, initial_set);
			ExtensionPrioritised newEntryQueue_dummy = ExtensionPrioritised(framework, argument, new_extension_build, initial_set, heuristic, 0);
			if (extension_priority_queue.find(newEntryQueue_dummy) == extension_priority_queue.end()) {
				omp_set_lock(lock_prio_queue);
				uint64_t numberElement = task_flags.size();
				ExtensionPrioritised newEntryQueue = ExtensionPrioritised(framework, argument, new_extension_build, initial_set, heuristic, numberElement);
				extension_priority_queue.insert(newEntryQueue);
				task_flags.push_back(false);
				omp_unset_lock(lock_prio_queue);
				omp_unset_lock(lock_has_entry);
			}
		}
		initial_set.clear();

#pragma omp flush(isTerminated)
#pragma omp atomic read
		isTerminated_tmp = isTerminated;
	} while (has_Solution && !isTerminated_tmp);

	free(isSolved);
	free(isFirstCalculation);
	delete solver;
	return;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool check_if_finished(bool &isTerminated, bool &isFinished, vector<uint8_t> &task_flags) {
	bool isTerminated_tmp = false;
#pragma omp flush(isTerminated)
#pragma omp atomic read
	isTerminated_tmp = isTerminated;

	uint64_t size = check_prio_queue_size(task_flags);
	return size == 0 || isTerminated_tmp;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void update_isFinished(bool &isTerminated, bool &isFinished, omp_lock_t *lock_has_entry, vector<uint8_t> &task_flags)
{
	bool isFinished_tmp = check_if_finished(isTerminated, isFinished, task_flags);
#pragma atomic write
	isFinished = isFinished_tmp;
#pragma omp flush(isFinished)
	if (isFinished_tmp) {
		omp_unset_lock(lock_has_entry);
	}
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool start_checking_rejection(uint32_t argument, AF &framework, ArrayBitSet &active_args, list<uint32_t> &proof_extension, uint8_t numCores)
{
	omp_lock_t *lock_queue = NULL;
	lock_queue = (omp_lock_t *)malloc(sizeof * lock_queue);
	if (lock_queue == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	omp_init_lock(lock_queue);

	omp_lock_t *lock_task_flag = NULL;
	lock_task_flag = (omp_lock_t *)malloc(sizeof * lock_task_flag);
	if (lock_task_flag == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	omp_init_lock(lock_task_flag);

	omp_lock_t *lock_has_entry = NULL;
	lock_has_entry = (omp_lock_t *)malloc(sizeof * lock_has_entry);
	if (lock_queue == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	omp_init_lock(lock_has_entry);

	IPrioHeuristic *heuristic = NULL;
	heuristic = new Heuristic2();

	if (numCores > 0)
	{
		omp_set_num_threads(numCores);
	}

	std::unordered_set<ExtensionPrioritised, PrioHash> prio_set;
	int num_buckets = 20;
	prio_set.rehash(num_buckets);
	std::vector<uint8_t> task_flags;

	bool isTerminated = false;
	bool isFinished = false;
	bool isRejected = false;
	omp_set_lock(lock_has_entry);
#pragma omp parallel shared(isRejected, isTerminated, isFinished, proof_extension, prio_set, task_flags, lock_task_flag) \
 firstprivate(argument, framework, active_args, heuristic)
	{
#pragma omp single nowait
		{
			list<uint32_t> extension_build;
			check_rejection(argument, framework, active_args, isRejected, isTerminated, extension_build, proof_extension,
				*heuristic, prio_set, lock_queue, lock_has_entry, task_flags);
			update_isFinished(isTerminated, isFinished, lock_has_entry, task_flags);
		}

		bool isFinished_tmp = false;
		while (true) {
			omp_set_lock(lock_has_entry);
#pragma omp flush(isFinished)
#pragma omp atomic read
			isFinished_tmp = isFinished;
			if (isFinished_tmp) {
				omp_unset_lock(lock_has_entry);
				break;
			}
			
			if (check_prio_queue_size(task_flags) > 0) {
				list<uint32_t> extension = pop_prio_queue(prio_set, task_flags, lock_task_flag);
				if (extension.empty()) {
					continue;
				}

				if (check_prio_queue_size(task_flags) > 0) {
					omp_unset_lock(lock_has_entry);
				}

				check_rejection(argument, framework, active_args, isRejected, isTerminated, extension, proof_extension,
					*heuristic, prio_set, lock_queue, lock_has_entry, task_flags);
				update_isFinished(isTerminated, isFinished, lock_has_entry, task_flags);
			}
			else {
				cout << "start_checking_rejection::exception queue empty but not finished" << endl;
				throw new exception();
			}
		}
	}
	
	delete heuristic;
	omp_destroy_lock(lock_queue);
	free(lock_queue);
	omp_destroy_lock(lock_has_entry);
	free(lock_has_entry);
	return isRejected;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Solver_DS_PR::solve(uint32_t argument, AF &framework, list<uint32_t> &proof_extension, uint8_t numCores) 
{
	ArrayBitSet initial_reduct = ArrayBitSet();
	pre_proc_result result_preProcessor = PreProc_DS_PR::process(framework, argument, initial_reduct);

	switch (result_preProcessor){

		case accepted:
			return true;

		case rejected:
			return false;

		case unknown:
			return !start_checking_rejection(argument, framework, initial_reduct, proof_extension, numCores);

		default:
			return unknown;
	}
}