#include "../../include/logic/PreProcessor.h"

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

ArrayBitSet PreProcessor::calculate_cone_influence(AF &framework, uint32_t query, ConeOfInfluence &out_coi) {
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

ArrayBitSet PreProcessor::calculate_cone_influence_reduct(AF &framework, ArrayBitSet reduct, uint32_t query) {
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