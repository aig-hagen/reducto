#include "../../include/logic/Heuristic2.h"


uint32_t Heuristic2::calculate_priority(AF &framework, list<uint32_t> &extension, list<uint32_t> &initialSet, uint32_t query) {

	if (initialSet.empty()) {
		cout << "calculate_priority:: initial set must not be empty" << endl;
		throw new invalid_argument("initial set must not be empty");
	}
	uint32_t prio = framework.distance_to_query[*initialSet.begin()];

	for (list<uint32_t>::iterator mIter = std::next(initialSet.begin()); mIter != initialSet.end(); ++mIter) {
		uint32_t newPrio = framework.distance_to_query[*mIter];

		if (newPrio < prio) {
			prio = newPrio;
		}
	}

	return prio;
}