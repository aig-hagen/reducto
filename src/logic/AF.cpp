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

void AF::initialize(uint32_t number_args) {
	num_args = number_args;
	attackers.resize(static_cast<uint64_t>(num_args) + 1);
	victims.resize(static_cast<uint64_t>(num_args) + 1);
	self_attack.resize(static_cast<uint64_t>(num_args) + 1);
	std::fill(self_attack.begin(), self_attack.end(), 0);
	distance_to_query.resize(static_cast<uint64_t>(num_args) + 1);
	std::fill(distance_to_query.begin(), distance_to_query.end(), 0);
}

void AF::finish_initilization()
{
	const uint64_t tmp_num_args = static_cast<uint64_t>(num_args) + 1;
	vector<vector<uint32_t>> attackers_arrays;
	attackers_arrays.resize(tmp_num_args);
	vector<vector<uint8_t>> attackers_bitsets;
	attackers_bitsets.resize(tmp_num_args);
	vector<vector<uint32_t>> victims_arrays;
	victims_arrays.resize(tmp_num_args);
	vector<vector<uint8_t>> victims_bitsets;
	victims_bitsets.resize(tmp_num_args);
	for (int i = 0; i < tmp_num_args; i++) {
		attackers_bitsets[i].resize(tmp_num_args);
		victims_bitsets[i].resize(tmp_num_args);
	}

	for (const pair<uint32_t, uint32_t> &attack : attacks) {
		int32_t source = attack.first;
		int32_t target = attack.second;
		attackers_arrays[target].push_back(source);
		attackers_bitsets[target][source] = true;
		victims_arrays[source].push_back(target);
		victims_bitsets[source][target] = true;

		if (source == target)
			self_attack[source] = true;
	}

	for (int i = 0; i < attackers.size(); i++) {
		attackers_arrays[i].shrink_to_fit();
		attackers[i] = ArrayBitSet(attackers_arrays[i], attackers_bitsets[i]);
	}
	attackers_arrays.clear();
	attackers_bitsets.clear();

	for (int i = 0; i < victims.size(); i++) {
		victims_arrays[i].shrink_to_fit();
		victims[i] = ArrayBitSet(victims_arrays[i], victims_bitsets[i]);
	}
	victims_arrays.clear();
	victims_bitsets.clear();

	self_attack.shrink_to_fit();
}

bool AF::exists_attack(uint32_t attacker, uint32_t victim) const {
	return attacks.count(make_pair(attacker, victim));
}