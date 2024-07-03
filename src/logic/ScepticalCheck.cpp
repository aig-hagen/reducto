#include "../../include/logic/ScepticalCheck.h"

bool ScepticalCheck::check_rejection(uint32_t argument, list<uint32_t> &set_arguments, AF &framework)
{
	//printf("%d: ------- check rejection --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());												//DEBUG

	//iterate through arguments in the initial set
	for (list<uint32_t>::iterator mIter = set_arguments.begin(); mIter != set_arguments.end(); ++mIter) {
		//check if argument if victim of initial set
		if (framework.victims[*mIter]._bitset[argument]) {
			return true;
		}
	}

	return false;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool ScepticalCheck::check_terminate_extension_build(uint32_t argument, list<uint32_t> &set_arguments)
{
	//printf("%d: ------- check termination of extension --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());								//DEBUG
	//iterate through arguments in the initial set

	for (list<uint32_t>::iterator mIter = set_arguments.begin(); mIter != set_arguments.end(); ++mIter) {
		//check if argument if victim of initial set
		if (*mIter == argument) {
			return true;
		}
	}

	return false;
}