#ifndef PRIO_QUEUE_MANAGER_H
#define PRIO_QUEUE_MANAGER_H

#include <vector>
#include <queue>
#include <unordered_set>
#include "omp.h"

#include "ExtensionPrioritised.h"



/// <summary>
/// This class is responsible for managing the priority queue of the extension.
/// </summary>
class PriorityQueueManager {
public:
	/// <summary>
	/// This method returns the element with the highest priority from the queue, without removing it, and marks it as processed.
	/// </summary>
	/// <param name="prio_set">Priority queue of extensions.</param>
	/// <param name="task_flags">Flags indicating if an element has been processed.</param>
	/// <param name="lock_task_flag">Lock to synchronize access to flags.</param>
	/// <returns></returns>
	static list<uint32_t> pop_prio_queue(std::unordered_set<ExtensionPrioritised, PrioHash> &prio_set, vector<uint8_t> &task_flags, omp_lock_t *lock_task_flag);

	/// <summary>
	/// This method calculated the number of all element not yet processed.
	/// </summary>
	/// <param name="task_flags">Flags indicating if an element has been processed.</param>
	/// <returns></returns>
	static uint64_t check_number_unprocessed_elements(vector<uint8_t> &task_flags);
};
#endif