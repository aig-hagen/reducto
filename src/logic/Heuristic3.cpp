#include "../../include/logic/Heuristic3.h"

static uint32_t calculate_punished_priority(uint32_t distance, uint8_t punishment_factor) {
	return distance % 2 == 0 ? distance + punishment_factor : distance;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint32_t Heuristic3::calculate_priority(AF &framework, list<uint32_t> &extension, list<uint32_t> &initialSet, uint32_t query) {
	const uint8_t punishment_factor = 4;

	if (initialSet.empty()) {
		cout << "calculate_priority:: initial set must not be empty" << endl;
		throw new invalid_argument("initial set must not be empty");
	}
	uint32_t dist = framework.distance_to_query[*initialSet.begin()];
	uint32_t prio = calculate_punished_priority(dist, punishment_factor);

	for (list<uint32_t>::iterator mIter = std::next(initialSet.begin()); mIter != initialSet.end(); ++mIter) {
		dist = framework.distance_to_query[*mIter];
		uint32_t newPrio = calculate_punished_priority(dist, punishment_factor);

		if (newPrio < prio) {
			prio = newPrio;
		}
	}

	return prio;
}