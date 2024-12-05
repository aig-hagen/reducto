#include "../../include/logic/PreProcessor_GR.h"

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

ArrayBitSet PreProc_GR::calculate_cone_influence(AF &framework, uint32_t query) {
	vector<uint32_t> active_args_vector;
	vector<uint8_t> active_args_bitset(framework.num_args + 1, 0);

	list<uint32_t> ls_args_unprocessed;
	ls_args_unprocessed.push_back(query);
	framework.distance_to_query[query] = 0;
	active_args_vector.push_back(query);
	active_args_bitset[query] = true;

	for (list<uint32_t>::iterator mIter = ls_args_unprocessed.begin(); mIter != ls_args_unprocessed.end(); ++mIter) {
		const auto &argument = *mIter;
		uint32_t distance = framework.distance_to_query[argument];
		for (int i = 0; i < framework.attackers[argument].size(); i++) {
			uint32_t attacker = framework.attackers[argument][i];
			if (framework.distance_to_query[attacker] > 0 || attacker == query) {
				//attacker was already visited
				continue;
			}

			//increase distance to query for attacker
			framework.distance_to_query[attacker] = distance + 1;

			active_args_vector.push_back(attacker);
			active_args_bitset[attacker] = true;

			ls_args_unprocessed.push_back(attacker);
		}
	}

	ArrayBitSet active_args = ArrayBitSet(active_args_vector, active_args_bitset);
	return active_args;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

ArrayBitSet PreProc_GR::calculate_cone_influence(AF &framework, ArrayBitSet reduct, uint32_t query, std::list<uint32_t> &list_remaining_args) {
	vector<uint32_t> active_args_vector;
	vector<uint8_t> active_args_bitset(framework.num_args + 1, 0);

	list<uint32_t> ls_args_unprocessed;
	ls_args_unprocessed.push_back(query);
	framework.distance_to_query[query] = 0;
	active_args_vector.push_back(query);
	active_args_bitset[query] = true;
	list_remaining_args.remove(query);

	for (list<uint32_t>::iterator mIter = ls_args_unprocessed.begin(); mIter != ls_args_unprocessed.end(); ++mIter) {
		const auto &argument = *mIter;
		for (int i = 0; i < framework.attackers[argument].size(); i++) {
			uint32_t attacker = framework.attackers[argument][i];
			if (active_args_bitset[attacker] == true) {
				//attacker was already visited
				continue;
			}

			if (reduct._bitset[attacker] == false) {
				//attacker was not part of the reduct
				continue;
			}

			active_args_vector.push_back(attacker);
			active_args_bitset[attacker] = true;
			list_remaining_args.remove(attacker);

			ls_args_unprocessed.push_back(attacker);
		}
	}

	ArrayBitSet active_args = ArrayBitSet(active_args_vector, active_args_bitset);
	return active_args;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

pre_proc_result PreProc_GR::reduce_by_grounded(AF &framework, ArrayBitSet &active_args, uint32_t query, bool break_accepted, bool break_rejected, 
	ArrayBitSet &out_reduct, list<uint32_t> &out_gr_extension)
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
		if(ua == query) {
			if (break_accepted) {
				return pre_proc_result::accepted;
			}

			result = pre_proc_result::accepted;
		}
		
		//reject query if it gets attacked by argument of grounded extension
		if (framework.exists_attack(ua, query)){
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

pre_proc_result PreProc_GR::process(AF &framework, uint32_t query, bool break_accepted, bool break_rejected, ArrayBitSet &out_reduct, list<uint32_t> &out_gr_extension)
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

ArrayBitSet PreProc_GR::process_only_grounded(AF &framework, list<uint32_t> &out_gr_extension) {
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

pre_proc_result PreProc_GR::process_only_grounded(AF &framework, uint32_t query, bool break_acception, bool break_rejection, ArrayBitSet &out_reduct, list<uint32_t> &out_gr_extension)
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
