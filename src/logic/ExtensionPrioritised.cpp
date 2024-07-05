#include "../../include/logic/ExtensionPrioritised.h"

ExtensionPrioritised::ExtensionPrioritised(AF framework, list<uint32_t> extension, Heuristic1 heuristic) {
	Extension = extension;
	Priority = heuristic.calculate_priority(framework, extension);
}


ExtensionPrioritised::~ExtensionPrioritised() {
	Extension.clear();
}