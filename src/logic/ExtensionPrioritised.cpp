#include "../../include/logic/ExtensionPrioritised.h"

ExtensionPrioritised::ExtensionPrioritised(AF &framework, uint32_t query, list<uint32_t> &extension, list<uint32_t> &initialSet, 
	IPrioHeuristic &heuristic, uint64_t number, ConeOfInfluence &coi) {
	Extension = extension;
	Priority = heuristic.calculate_priority(framework, extension, initialSet, query, coi);
	Number = number;
}


ExtensionPrioritised::~ExtensionPrioritised() {
	Extension.clear();
}