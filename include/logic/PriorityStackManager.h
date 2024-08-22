#ifndef PRIO_QUEUE_MANAGER_H
#define PRIO_QUEUE_MANAGER_H

#include <vector>
#include <queue>
#include <unordered_set>
#include "omp.h"

#include "ExtensionPrioritised.h"



/// <summary>
/// This class is responsible for managing the priority stack of the extension.
/// </summary>
class PriorityStackManager {
public:
	/// <summary>
	/// Lock to signal that there are unprocessed entries on the stack.
	/// </summary>
	omp_lock_t *lock_has_entry = NULL;
	/// <summary>
	/// Lock to synchronize access to the stack.
	/// </summary>
	omp_lock_t *lock_stack = NULL;
	/// <summary>
	/// Lock to synchronize access to the flags of the elements. 
	/// </summary>
	omp_lock_t *lock_task_flag = NULL;
	/// <summary>
	/// Priority stack, in which elements will be stacked in buckets due to their priority.
	/// </summary>
	std::unordered_set<ExtensionPrioritised, PrioHash> prio_stack;
	/// <summary>
	/// List of flags indicating if a associated element has already been processed.
	/// </summary>
	std::vector<uint8_t> task_flags;

	PriorityStackManager();
	~PriorityStackManager();

	/// <summary>
	/// This method calculated the number of all element not yet processed.
	/// </summary>
	/// <returns></returns>
	uint64_t check_number_unprocessed_elements();

	/// <summary>
	/// This method returns the element with the highest priority from the queue, without removing it, and marks it as processed.
	/// </summary>
	/// <returns></returns>
	list<uint32_t> pop_prio_queue();

	/// <summary>
	/// This method tries to insert an extension in the specified priority queue.
	/// </summary>
	/// <param name="query_argument">The query argument of the DS-PR problem</param>
	/// <param name="framework">The abstract argumentation framework of the problem.</param>
	/// <param name="heuristic">The heuristic used to create the prioritised extensions.</param>
	/// <param name="extension">The extension to add to the priority stack.</param>
	/// <param name="initial_set">The initial set of the extension to add.</param>
	/// <returns>TRUE iff the element could be added to the priority stack, otherwise FALSE.</returns>
	bool try_insert_extension(uint32_t query_argument, AF &framework, IPrioHeuristic *heuristic, list<uint32_t> &extension, list<uint32_t> &initial_set);
};
#endif