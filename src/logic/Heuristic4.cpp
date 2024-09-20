#include "../../include/logic/Heuristic4.h"


uint32_t Heuristic4::calculate_priority(AF &framework, list<uint32_t> &extension, list<uint32_t> &initialSet, uint32_t query) {

	if (initialSet.empty()) {
		cout << "calculate_priority:: initial set must not be empty" << endl;
		throw new invalid_argument("initial set must not be empty");
	}

	float sum_victims = 0.0f;
	for (list<uint32_t>::iterator mIter = extension.begin(); mIter != extension.end(); ++mIter) {
		uint32_t argument = *mIter;

		sum_victims = sum_victims + framework.victims[argument].size();
	}

	float prio = ((4.0f / 5.0f) * framework.num_args) / sum_victims;
	if (prio > 1.0f) {
		prio = ((prio - 1.0f) * 10) - 1.0;
	}else if (prio < 1.0f) {
		prio = 0.0f;
	}

	return static_cast<uint32_t>(prio);
}