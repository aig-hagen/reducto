#include "../../include/logic/ExtensionPrioritised.h"

ExtensionPrioritised::ExtensionPrioritised(AF &framework, list<uint32_t> &extension, Heuristic1 &heuristic) {
	Extension = extension;
	Priority = heuristic.calculate_priority(framework, extension);
}

ExtensionPrioritised::ExtensionPrioritised(AF framework, list<uint32_t> extension, Heuristic1 heuristic, vector<vector<int64_t>> complement_clauses) {
	Extension = extension;
	Priority = heuristic.calculate_priority(framework, extension);
	Complement_clauses = complement_clauses;
}


ExtensionPrioritised::~ExtensionPrioritised() {
	Extension.clear();
	for (int i = 0; i < Complement_clauses.size(); i++) {
		Complement_clauses[i].clear();
	}
	Complement_clauses.clear();
}