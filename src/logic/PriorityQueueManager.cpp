#include "../../include/logic/PriorityQueueManager.h"
using namespace std;

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

list<uint32_t> PriorityQueueManager::pop_prio_queue(std::unordered_set<ExtensionPrioritised, PrioHash> &prio_set, vector<uint8_t> &task_flags, omp_lock_t *lock_task_flag) {
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

uint64_t PriorityQueueManager::check_number_unprocessed_elements(vector<uint8_t> &task_flags) {
#pragma omp flush(task_flags)
	if (task_flags.empty()) return 0;

	uint64_t num_false = 0;
	for (uint64_t i = task_flags.size() - 1; i >= 0; i--) {
		if (!task_flags[i]) num_false++;
		if (num_false >= 2) break;
	}
	return num_false;
}