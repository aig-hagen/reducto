#ifndef EXTENSION_PRIORITISED_H
#define EXTENSION_PRIORITISED_H

#include <iostream>
#include <cstdint>
#include <list>

#include "IPrioHeuristic.h"
#include "AF.h"

using namespace std;

/// <summary>
/// This class is responsible for the data structure of an extension, that get's prioritized due to a specified heuristic.
/// </summary>
class ExtensionPrioritised {
public:

	bool operator == (ExtensionPrioritised const &other)
	{
		return (other.Priority == Priority)
			&& (other.Extension == Extension);
	};

	list<uint32_t> Extension;
	uint32_t Priority = 100;

	ExtensionPrioritised(AF &framework, uint32_t query, list<uint32_t> &extension, list<uint32_t> &initialSet, IPrioHeuristic &heuristic);
	~ExtensionPrioritised();
};

struct PrioHash {
	auto operator()(ExtensionPrioritised const &extension) const -> size_t {
		return extension.Priority < 20 ? extension.Priority : 20;
	}
};

inline bool operator == (ExtensionPrioritised const &lhs, ExtensionPrioritised const &rhs)
{
	return (lhs.Priority == rhs.Priority)
		&& (lhs.Extension == rhs.Extension);
};

#endif