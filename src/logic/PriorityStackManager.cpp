#include "../../include/logic/PriorityStackManager.h"
using namespace std;

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

PriorityStackManager::PriorityStackManager() {
	lock_stack = (omp_lock_t *)malloc(sizeof * lock_stack);
	if (lock_stack == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	omp_init_lock(lock_stack);
		
	lock_task_flag = (omp_lock_t *)malloc(sizeof * lock_task_flag);
	if (lock_task_flag == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	omp_init_lock(lock_task_flag);
		
	lock_has_entry = (omp_lock_t *)malloc(sizeof * lock_has_entry);
	if (lock_stack == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	omp_init_lock(lock_has_entry);
		
	int num_buckets = 20;
	prio_stack.rehash(num_buckets);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

PriorityStackManager::~PriorityStackManager() {
	omp_destroy_lock(lock_has_entry);
	free(lock_has_entry);
	omp_destroy_lock(lock_stack);
	free(lock_stack);
	omp_destroy_lock(lock_task_flag);
	free(lock_task_flag);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint64_t PriorityStackManager::check_number_unprocessed_elements() {
#pragma omp flush(task_flags)
	if (task_flags.empty()) return 0;

	uint64_t num_false = 0;
	for (uint64_t i = task_flags.size(); i > 0; i--) {
		if (!task_flags[i - 1]) num_false++;
		if (num_false >= 2) break;
	}
	return num_false;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

list<uint32_t> PriorityStackManager::pop_prio_queue() {
	for (int i = 0; i < prio_stack.bucket_count(); i++) {
		//ensure starting to iterate at bucket 0
		for (auto iter = prio_stack.begin(i); iter != prio_stack.end(i); ++iter) {
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

bool PriorityStackManager::try_insert_extension(uint32_t query_argument, AF &framework, IPrioHeuristic *heuristic,
	list<uint32_t> &extension, list<uint32_t> &initial_set) {
	ExtensionPrioritised newEntryQueue_dummy = ExtensionPrioritised(framework, query_argument, extension, initial_set, *heuristic, 0);
	if (prio_stack.find(newEntryQueue_dummy) == prio_stack.end()) {
		omp_set_lock(lock_stack);
		omp_set_lock(lock_task_flag);
		uint64_t numberElement = task_flags.size();
		ExtensionPrioritised newEntryQueue = ExtensionPrioritised(framework, query_argument, extension, initial_set, *heuristic, numberElement);
		bool result = prio_stack.insert(newEntryQueue).second;
		task_flags.push_back(false);
		omp_unset_lock(lock_task_flag);
		omp_unset_lock(lock_stack);
		omp_unset_lock(lock_has_entry);
		return result;
	}

	return false;
}