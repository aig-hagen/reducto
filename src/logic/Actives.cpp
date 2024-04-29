#include "../../include/logic/Actives.h"

using namespace std;

 int64_t Actives::isActive(vector<uint32_t> activeArgs, uint32_t argument) {
	for (int i = 0; i < activeArgs.size(); i++) {
		if (activeArgs[i] == argument) {
			return i;
		}
	}

	return -1;
}