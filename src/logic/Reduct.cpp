#include "../../include/logic/Reduct.h"

unordered_set<uint32_t> Reduct::get_reduct(unordered_set<uint32_t> &activeArguments, AF &framework, uint32_t argument)
{
	unordered_set<uint32_t> reduct;
	
	for (size_t bno = 0; bno < activeArguments.bucket_count(); ++bno) {
		for (auto bit = activeArguments.begin(bno), end = activeArguments.end(bno); bit != end; ++bit) {
			const auto &active_arg = *bit;

			if (active_arg == argument || framework.victims[argument].count(active_arg)) {
				continue;
			}

			reduct.insert(active_arg);
		}
	}


	return reduct;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

unordered_set<uint32_t> Reduct::get_reduct_set(unordered_set<uint32_t> &activeArguments, AF &framework, list<uint32_t> &set_arguments)
{
	if (set_arguments.empty()) {
		throw new exception;
	}

	//printf("%d: ------- before getReduct --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());												//DEBUG
	// do not free activeArguments, since its a given parameter and hence out of this method's responsibility
	unordered_set<uint32_t> reduct = get_reduct(activeArguments, framework, *set_arguments.begin());
	//printf("%d: ------- after getReduct --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());													//DEBUG
	if (reduct.empty()) {
		return reduct;
	}

	for (list<uint32_t>::iterator mIter = std::next(set_arguments.begin()); mIter != set_arguments.end(); ++mIter) {
		if (reduct.empty())
		{
			break;
		}

		// free all temporary reducts
		//printf("%d: ------- before getReduct --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());											//DEBUG
		unordered_set<uint32_t> tmp_reduct = get_reduct(reduct, framework, *mIter);
		reduct.clear();
		reduct = tmp_reduct;
		//printf("%d: ------- after getReduct + free tmp_Reduct --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());							//DEBUG
	}

	return reduct;
}