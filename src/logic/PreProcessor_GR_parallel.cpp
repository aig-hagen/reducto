#include "../../include/logic/PreProcessor_GR_parallel.h"

ArrayBitSet PreProc_GR_parallel::calculate_cone_influence(AF &framework, uint32_t query) {
	//init shared variables
	omp_lock_t *lock_has_entry = (omp_lock_t *)malloc(sizeof * lock_has_entry);
	if (lock_has_entry == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	omp_init_lock(lock_has_entry);

	omp_lock_t *lock_resources = (omp_lock_t *)malloc(sizeof * lock_resources);
	if (lock_resources == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	omp_init_lock(lock_resources);
	
	vector<uint32_t> active_args_vector_shared;
	vector<uint8_t> active_args_bitset_shared(framework.num_args + 1, 0);
	vector<uint32_t> distance_args_shared(framework.num_args + 1, 0);
	list<uint32_t> ls_args_unprocessed_shared;

	//process query argument
	ls_args_unprocessed_shared.push_back(query);
	framework.distance_to_query[query] = 0;
	distance_args_shared[query] = 0;
	active_args_vector_shared.push_back(query);
	active_args_bitset_shared[query] = true;
	omp_unset_lock(lock_has_entry);

#pragma omp parallel shared(active_args_vector_shared, active_args_bitset_shared, ls_args_unprocessed_shared, distance_args_shared) \
	firstprivate(framework, query)
	{
		vector<uint32_t> active_args_vector_private;
		vector<uint8_t> active_args_bitset_private(framework.num_args + 1, 0);
		list<uint32_t> ls_args_unprocessed_private;
		vector<uint32_t> distance_args_private(framework.num_args + 1, 0);
		omp_set_lock(lock_has_entry);

		while (true) {
			uint32_t argument = 0;
			//check if task is finished
			omp_set_lock(lock_resources);
#pragma omp flush(ls_args_unprocessed_shared)
			if (ls_args_unprocessed_shared.size() == 0) {
				//breaking condition to free threads if task has ended
				omp_unset_lock(lock_resources);
				omp_unset_lock(lock_has_entry);
				break;
			}
			else {
				//get shared argument
				argument = *ls_args_unprocessed_shared.begin();
				ls_args_unprocessed_shared.erase(ls_args_unprocessed_shared.begin());
#pragma omp flush(ls_args_unprocessed_shared)
				ls_args_unprocessed_private.push_back(argument);
#pragma omp flush(distance_args_shared)
				distance_args_private[argument] = distance_args_shared[argument];
			}
			omp_unset_lock(lock_resources);

			//iterate private list
			do {
				//get private argument
				argument = *ls_args_unprocessed_private.begin();
				ls_args_unprocessed_private.erase(ls_args_unprocessed_private.begin());

				//process argument
				uint32_t distance = distance_args_private[argument];
				for (int i = 0; i < framework.attackers[argument].size(); i++) {
					uint32_t attacker = framework.attackers[argument][i];
					if (distance_args_private[attacker] > 0 || attacker == query) {
						//attacker was already visited
						continue;
					}

					//increase distance to query for attacker
					distance_args_private[attacker] = distance + 1;

					active_args_vector_private.push_back(attacker);
					active_args_bitset_private[attacker] = true;

					if (i == 0) {
						omp_set_lock(lock_resources);
#pragma omp flush
						ls_args_unprocessed_shared.push_back(attacker);
						distance_args_shared[attacker] = distance_args_private[attacker];
						active_args_vector_shared.push_back(attacker);
						active_args_bitset_shared[attacker] = true;
#pragma omp flush
						omp_unset_lock(lock_resources);
						omp_unset_lock(lock_has_entry);
					}
					else {
						ls_args_unprocessed_private.push_back(attacker);
					}
				}
				//iterate through private list before getting new argument from shared list
			} while (ls_args_unprocessed_private.size() > 0);
		}

		//put private data in shared data structure
		omp_set_lock(lock_resources);
#pragma omp flush
		for (int i = 0; i < active_args_vector_private.size(); i++) {
			uint32_t argument = active_args_vector_private[i];
			if (distance_args_shared[argument] == 0) {
				//argument is new
				active_args_vector_shared.push_back(argument);
				active_args_bitset_shared[argument] = true;
				distance_args_shared[argument] = distance_args_private[argument];
			}
			else if (distance_args_shared[argument] > distance_args_private[argument]) {
				//argument needs to update distance
				distance_args_shared[argument] = distance_args_private[argument];
			}
			else {
				//argument is saved with lower distance
			}
		}
#pragma omp flush
		omp_unset_lock(lock_resources);
	}

	//update framework (is private data in parallel section)
	for (int i = 1; i < distance_args_shared.size(); i++) {
		framework.distance_to_query[i] = distance_args_shared[i];
	}

	ArrayBitSet active_args = ArrayBitSet(active_args_vector_shared, active_args_bitset_shared);
	return active_args;
}


/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/


static pre_proc_result reduce_by_grounded(AF &framework, ArrayBitSet &active_args, uint32_t query, bool break_accepted, bool break_rejected, ArrayBitSet &out_reduct, list<uint32_t> &out_gr_extension)
{
	pre_proc_result result = pre_proc_result::unknown;
	// fill list with unattacked arguments
	list<uint32_t> ls_unattacked_unprocessed;
	vector<uint32_t> num_attacker;
	num_attacker.resize(framework.num_args + 1);
	//iterate through active arguments
	for (int i = 0; i < active_args._array.size(); i++) {
		//check if argument is unattacked
		if (framework.attackers[active_args._array[i]].empty()) {
			ls_unattacked_unprocessed.push_back(active_args._array[i]);
			out_gr_extension.push_back(active_args._array[i]);
		}

		num_attacker[active_args._array[i]] = framework.attackers[active_args._array[i]].size();
	}

	// init variable of current reduct
	out_reduct = active_args;

	//process list of unattacked arguments
	for (list<uint32_t>::iterator mIter = ls_unattacked_unprocessed.begin(); mIter != ls_unattacked_unprocessed.end(); ++mIter) {
		const auto &ua = *mIter;

		//accept query if query is part of grounded extension
		if (ua == query) {
			if (break_accepted) {
				return pre_proc_result::accepted;
			}

			result = pre_proc_result::accepted;
		}

		//reject query if it gets attacked by argument of grounded extension
		if (framework.exists_attack(ua, query)) {
			if (break_rejected) {
				return pre_proc_result::rejected;
			}

			result = pre_proc_result::rejected;
		}

		//iterate through victims of the victims of ua
		for (int i = 0; i < framework.victims[ua].size(); i++) {
			uint32_t vua = framework.victims[ua][i];

			if (!out_reduct._bitset[vua]) {
				//only account victims that are still active
				continue;
			}

			for (int j = 0; j < framework.victims[vua].size(); j++) {
				uint32_t vvua = framework.victims[vua][j];

				if (!out_reduct._bitset[vvua]) {
					//only account victims of victims that are still active
					continue;
				}

				//update number of attackers
				num_attacker[vvua]--;

				//check if victim of victim is unattacked
				if (num_attacker[vvua] == 0) {
					ls_unattacked_unprocessed.push_back(vvua);
					out_gr_extension.push_back(vvua);
				}
			}
		}

		//reduce active argument by unattacked argument + update current reduct
		out_reduct = Reduct::get_reduct(out_reduct, framework, ua);
	}

	return result;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

pre_proc_result PreProc_GR_parallel::process(AF &framework, uint32_t query, bool break_accepted, bool break_rejected, ArrayBitSet &out_reduct, list<uint32_t> &out_gr_extension)
{
	if (framework.self_attack[query])
	{
		return pre_proc_result::rejected;
	}

	if (framework.attackers[query].empty())
	{
		return pre_proc_result::accepted;
	}

	ArrayBitSet active_args = calculate_cone_influence(framework, query);

	return reduce_by_grounded(framework, active_args, query, break_accepted, break_rejected, out_reduct, out_gr_extension);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

ArrayBitSet PreProc_GR_parallel::process_only_grounded(AF &framework, list<uint32_t> &out_gr_extension) {
	//build basic active arguments
	vector<uint32_t> active_args_vector;
	vector<uint8_t> active_args_bitset(framework.num_args + 1, 1);
	for (int i = 0; i < framework.num_args; i++) {
		active_args_vector.push_back(i + 1);
	}
	ArrayBitSet active_args = ArrayBitSet(active_args_vector, active_args_bitset);
	//reduce by grounded extension
	ArrayBitSet initial_reduct = ArrayBitSet();
	reduce_by_grounded(framework, active_args, 0, false, false, initial_reduct, out_gr_extension);
	return initial_reduct;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

pre_proc_result PreProc_GR_parallel::process_only_grounded(AF &framework, uint32_t query, bool break_acception, bool break_rejection, ArrayBitSet &out_reduct, list<uint32_t> &out_gr_extension)
{
	if (framework.self_attack[query])
	{
		return pre_proc_result::rejected;
	}

	if (framework.attackers[query].empty())
	{
		return pre_proc_result::accepted;
	}

	//build basic active arguments
	vector<uint32_t> active_args_vector;
	vector<uint8_t> active_args_bitset(framework.num_args + 1, 1);
	for (int i = 0; i < framework.num_args; i++) {
		active_args_vector.push_back(i + 1);
	}
	ArrayBitSet active_args = ArrayBitSet(active_args_vector, active_args_bitset);

	//reduce by grounded extension
	return reduce_by_grounded(framework, active_args, query, break_acception, break_rejection, out_reduct, out_gr_extension);
}