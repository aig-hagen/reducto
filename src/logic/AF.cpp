#include "../../include/logic/AF.h"

using namespace std;

bool AF::add_attack(uint32_t attacker, uint32_t victim)
{
	if (attacker < 0 || victim < 0) return false;
	
	if (attacks.count(make_pair(attacker, victim))) {
		return false;
	}
	attacks.insert(make_pair(attacker, victim));
	return true;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void AF::initialize(uint32_t number_args) {
	num_args = number_args;
	attackers.resize(static_cast<uint64_t>(num_args) + 1);
	victims.resize(static_cast<uint64_t>(num_args) + 1);
	self_attack.resize(static_cast<uint64_t>(num_args) + 1);
	std::fill(self_attack.begin(), self_attack.end(), 0);
	distance_to_query.resize(static_cast<uint64_t>(num_args) + 1);
	std::fill(distance_to_query.begin(), distance_to_query.end(), 0);
	sorted_by_num_victims.resize(num_args);
	for (uint32_t i = 0; i < num_args; i++) {
		sorted_by_num_victims[i] = i + 1;
	}
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void AF::finish_initilization()
{
	const uint64_t tmp_num_args = static_cast<uint64_t>(num_args) + 1;

	for (const pair<uint32_t, uint32_t> &attack : attacks) {
		int32_t source = attack.first;
		int32_t target = attack.second;
		attackers[target].push_back(source);
		victims[source].push_back(target);

		if (source == target)
			self_attack[source] = true;
	}

	std::sort(sorted_by_num_victims.begin(), sorted_by_num_victims.end(), ComparatorNumVictims(*this));
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool AF::exists_attack(uint32_t attacker, uint32_t victim) const {
	return attacks.count(make_pair(attacker, victim));
}