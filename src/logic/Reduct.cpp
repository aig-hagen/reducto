#include "../../include/logic/Reduct.h"

vector<uint32_t> Reduct::get_reduct(vector<uint32_t> &activeArguments, AF &framework, uint32_t argument)
{
	vector<uint32_t> reduct;
	for (int i = 0; i < activeArguments.size(); i++) {		
		if (activeArguments[i] == argument || framework.victims[argument].count(activeArguments[i])){
			continue;
		}

		reduct.push_back(activeArguments[i]);
	}

	reduct.shrink_to_fit();
	return reduct;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

vector<uint32_t> Reduct::get_reduct_set(vector<uint32_t> &activeArguments, AF &framework, list<uint32_t> &set_arguments)
{
	if (set_arguments.empty()) {
		throw new exception;
	}

	//printf("%d: ------- before getReduct --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());												//DEBUG
	// do not free activeArguments, since its a given parameter and hence out of this method's responsibility
	vector<uint32_t> reduct = get_reduct(activeArguments, framework, *set_arguments.begin());
	//printf("%d: ------- after getReduct --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());													//DEBUG
	
	for (list<uint32_t>::iterator mIter = std::next(set_arguments.begin()); mIter != set_arguments.end(); ++mIter) {
		if (reduct.size() == 0)
		{
			break;
		}

		// free all temporary reducts
		//printf("%d: ------- before getReduct --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());											//DEBUG
		vector<uint32_t> tmp_reduct = reduct;
		reduct = get_reduct(tmp_reduct, framework, *mIter);
		tmp_reduct.clear();
		//printf("%d: ------- after getReduct + free tmp_Reduct --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());							//DEBUG
	}

	return reduct;
}