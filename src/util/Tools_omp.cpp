#include "../../include/util/Tools_omp.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool tools::ToolsOMP::check_finished(bool &is_finished, PriorityStackManager &prio_stack) {
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

bool tools::ToolsOMP::check_termination(bool &is_terminated, bool continue_calculation)
{
	bool is_terminated_tmp;
#pragma omp flush(is_terminated)
#pragma omp atomic read
	is_terminated_tmp = is_terminated;
	return is_terminated_tmp || !continue_calculation;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void tools::ToolsOMP::update_is_finished(bool &is_terminated, bool &is_finished, PriorityStackManager &prio_stack)
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

void tools::ToolsOMP::set_is_rejected(bool &is_rejected, bool &is_terminated)
{
#pragma atomic write
	is_rejected = true;
#pragma omp flush(is_rejected)
#pragma atomic write
	is_terminated = true;
#pragma omp flush(is_terminated)
}