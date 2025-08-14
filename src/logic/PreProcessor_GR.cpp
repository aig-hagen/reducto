#include "../../include/logic/PreProcessor_GR.h"

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

ArrayBitSet PreProc_GR::calculate_cone_influence(AF &framework, uint32_t query, ConeOfInfluence &out_coi) {
	// initialize variables
	vector<uint32_t> active_args_vector;
	vector<uint8_t> active_args_bitset(framework.num_args + 1, 0);
	list<uint32_t> ls_args_unprocessed;
	// start list with query
	ls_args_unprocessed.push_back(query);
	out_coi.Distance_to_query[query] = 0;
	out_coi.Max_distance = 0;
	active_args_vector.push_back(query);
	active_args_bitset[query] = true;

	// iterate through list as long as it has elements
	for (list<uint32_t>::iterator mIter = ls_args_unprocessed.begin(); mIter != ls_args_unprocessed.end(); ++mIter) {
		const auto &argument = *mIter;
		uint32_t distance = out_coi.Distance_to_query[argument];
		// iterate through attackers of the current argument
		for (std::vector<unsigned int>::size_type i = 0; i < framework.attackers[argument].size(); i++) {
			uint32_t attacker = framework.attackers[argument][i];
			if (out_coi.Distance_to_query[attacker] > 0 || attacker == query) {
				// attacker was already visited
				continue;
			}

			// increase distance to query for attacker
			uint32_t distance_attacker = distance + 1;
			out_coi.Distance_to_query[attacker] = distance_attacker;
			// update max distance in cone of influence
			if (distance_attacker > out_coi.Max_distance) out_coi.Max_distance = distance_attacker;

			// add attacker to the cone of influence
			active_args_vector.push_back(attacker);
			active_args_bitset[attacker] = true;
			// add attacker to the list
			ls_args_unprocessed.push_back(attacker);
		}
	}

	ArrayBitSet active_args = ArrayBitSet(active_args_vector, active_args_bitset);
	return active_args;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

ArrayBitSet PreProc_GR::calculate_cone_influence_reduct(AF &framework, ArrayBitSet reduct, uint32_t query) {
	// initialize variables
	vector<uint32_t> active_args_vector;
	vector<uint8_t> active_args_bitset(framework.num_args + 1, 0);
	list<uint32_t> ls_args_unprocessed;
	// start list with query
	ls_args_unprocessed.push_back(query);
	active_args_vector.push_back(query);
	active_args_bitset[query] = true;

	// iterate through list as long as it has elements
	for (list<uint32_t>::iterator mIter = ls_args_unprocessed.begin(); mIter != ls_args_unprocessed.end(); ++mIter) {
		const auto &argument = *mIter;
		// iterate through attackers of the current argument
		for (std::vector<unsigned int>::size_type i = 0; i < framework.attackers[argument].size(); i++) {
			uint32_t attacker = framework.attackers[argument][i];
			if (active_args_bitset[attacker] == true) {
				// attacker was already visited
				continue;
			}

			if (reduct._bitset[attacker] == false) {
				// attacker was not part of the reduct
				continue;
			}
			// add attacker to the cone of influence
			active_args_vector.push_back(attacker);
			active_args_bitset[attacker] = true;
			// add attacker to the list
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
	for (std::vector<unsigned int>::size_type i = 0; i < active_args._array.size(); i++) {
		//check if argument is unattacked
		if (framework.attackers[active_args._array[i]].empty()) {
			// add unattacked argument to list and to output grounded extension
			ls_unattacked_unprocessed.push_back(active_args._array[i]);
			out_gr_extension.push_back(active_args._array[i]);
		}
		// set number of attacker for current argument
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

		//iterate through victims of the unattacked argument
		for (std::vector<unsigned int>::size_type i = 0; i < framework.victims[ua].size(); i++) {
			uint32_t vua = framework.victims[ua][i];
			//only account victims that are still active
			if (!out_reduct._bitset[vua]) {
				continue;
			}
			//iterate through victims of the victims of unattacked argument
			for (std::vector<unsigned int>::size_type j = 0; j < framework.victims[vua].size(); j++) {
				uint32_t vvua = framework.victims[vua][j];
				//only account victims of victims that are still active
				if (!out_reduct._bitset[vvua]) {
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

pre_proc_result PreProc_GR::process(AF &framework, uint32_t query, bool break_accepted, bool break_rejected, ArrayBitSet &out_reduct,
	list<uint32_t> &out_gr_extension, ConeOfInfluence &out_coi)
{
	if (framework.self_attack[query])
	{
		return pre_proc_result::rejected;
	}

	if (framework.attackers[query].empty())
	{
		out_gr_extension.push_back(query);
		return pre_proc_result::accepted;
	}

	ArrayBitSet active_args = calculate_cone_influence(framework, query, out_coi);

	return reduce_by_grounded(framework, active_args, query, break_accepted, break_rejected, out_reduct, out_gr_extension);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

ArrayBitSet PreProc_GR::process_only_grounded(AF &framework, list<uint32_t> &out_gr_extension) {
	//build active arguments of the original framework
	
	ArrayBitSet active_args = framework.create_active_arguments();
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
		out_gr_extension.push_back(query);
		return pre_proc_result::accepted;
	}

	//build active arguments of the original framework
	ArrayBitSet active_args = framework.create_active_arguments();

	//reduce by grounded extension
	return reduce_by_grounded(framework, active_args, query, break_acception, break_rejection, out_reduct, out_gr_extension);
}
