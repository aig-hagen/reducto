#ifndef EXTENSION_PRIORITISED_H
#define EXTENSION_PRIORITISED_H

#include <iostream>
#include <cstdint>
#include <list>

#include "IPrioritizer.h"
#include "Heuristic1.h"
#include "AF.h"

using namespace std;

/// <summary>
/// This class is responsible for the data structure of an extension, that get's prioritized due to a specified heuristic.
/// </summary>
class ExtensionPrioritised {
public:
	list<uint32_t> Extension;
	uint32_t Priority;

	ExtensionPrioritised(AF framework, list<uint32_t> extension, Heuristic1 heuristic);
	~ExtensionPrioritised();
};

typedef struct extPrioLess
{
	bool operator()(const ExtensionPrioritised l, const ExtensionPrioritised r) const { return l.Priority < r.Priority; }
} extPrioLess_t;

#endif