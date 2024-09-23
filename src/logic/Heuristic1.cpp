#include "../../include/logic/Heuristic1.h"

uint32_t Heuristic1::calculate_priority(AF &framework, list<uint32_t> &extension, list<uint32_t> &initialSet, uint32_t query) {

	if (extension.empty()) {
		cout << "calculate_priority:: extension must not be empty" << endl;
		throw new invalid_argument("extension must not be empty");
	}
	uint32_t prio = framework.distance_to_query[*extension.begin()];

	for (list<uint32_t>::iterator mIter = std::next(extension.begin()); mIter != extension.end(); ++mIter) {
		uint32_t newPrio = framework.distance_to_query[*mIter];

		if (newPrio < prio) {
			prio = newPrio;
		}
	}

	return prio;
}