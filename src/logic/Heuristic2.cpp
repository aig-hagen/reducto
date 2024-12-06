#include "../../include/logic/Heuristic2.h"


uint32_t Heuristic2::calculate_priority(AF &framework, list<uint32_t> &extension, list<uint32_t> &initialSet, uint32_t query, ConeOfInfluence &coi) {

	if (initialSet.empty()) {
		cout << "calculate_priority:: initial set must not be empty" << endl;
		throw new invalid_argument("initial set must not be empty");
	}
	uint32_t prio = coi.Distance_to_query[*initialSet.begin()];

	for (list<uint32_t>::iterator mIter = std::next(initialSet.begin()); mIter != initialSet.end(); ++mIter) {
		uint32_t newPrio = coi.Distance_to_query[*mIter];

		if (newPrio < prio) {
			prio = newPrio;
		}
	}

	return prio - 2; // because: distance 0 not possible, since query itself cannot be containe in initial set; distance 1 also not possible, extension triggers rejection before being pushed to priority stack
}