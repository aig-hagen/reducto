#include "../../include/logic/Heuristic4.h"


uint32_t Heuristic4::calculate_priority(AF &framework, list<uint32_t> &extension, list<uint32_t> &initialSet, uint32_t query) {

	if (initialSet.empty()) {
		cout << "calculate_priority:: initial set must not be empty" << endl;
		throw new invalid_argument("initial set must not be empty");
	}

	uint32_t sum_victims = 0;
	for (list<uint32_t>::iterator mIter = extension.begin(); mIter != extension.end(); ++mIter) {
		uint32_t argument = *mIter;

		sum_victims = sum_victims + framework.victims[argument].size();
	}

	float prio = framework.num_args / sum_victims;
	if (prio > 1.0f) {
		prio = (prio - 1.0f) * 10;
	}

	return static_cast<uint32_t>(prio);
}