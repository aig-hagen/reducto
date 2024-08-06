#include "../../include/logic/ExtensionPrioritised.h"

ExtensionPrioritised::ExtensionPrioritised(AF &framework, uint32_t query, list<uint32_t> &extension, list<uint32_t> &initialSet, IPrioHeuristic &heuristic, uint64_t number) {
	Extension = extension;
	Priority = heuristic.calculate_priority(framework, extension, initialSet, query);
	Number = number;
}


ExtensionPrioritised::~ExtensionPrioritised() {
	Extension.clear();
}