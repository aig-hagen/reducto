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
}

void AF::initialize_af()
{
	//float start_time = omp_get_wtime();																										//DEBUG

	attackers.clear();
	self_attack.clear();
	symmetric_attacks.clear();
	attackers.resize(static_cast<uint64_t>(num_args) + 1);
	victims.resize(static_cast<uint64_t>(num_args) + 1);
	self_attack.resize(static_cast<uint64_t>(num_args) + 1);
	for (const pair<uint32_t, uint32_t> &attack : attacks) {
		int32_t source = attack.first;
		int32_t target = attack.second;
		if (!attackers[target].count(source)) {
			attackers[target].insert(source);
		}
		if (!victims[source].count(target)) {
			victims[source].insert(target);
		}
		if (source == target)
			self_attack[source] = true;
		if (attacks.count(make_pair(target, source))) {
			symmetric_attacks.insert(make_pair(source, target));
			symmetric_attacks.insert(make_pair(target, source));
		}
	}

	//float end_time = omp_get_wtime();																											//DEBUG
	//float duration = end_time - start_time;																									//DEBUG
	//printf("runtime initialize_af [s]: %.2f s\n", duration);																					//DEBUG
}

bool AF::exists_attack(uint32_t attacker, uint32_t victim) const {
	return attacks.count(make_pair(attacker, victim));
}