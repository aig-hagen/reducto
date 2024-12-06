#ifndef CONEOFINFLUENCE_H
#define CONEOFINFLUENCE_H


#include <vector>
#include <cstdint>
#include <list>

#include "AF.h"

using namespace std;

/// <summary>
/// This class is responsible for representing the data of a cone of influence created by an argument in a abstract argumentation framework.
/// </summary>
class ConeOfInfluence {
public:
	/// <summary>
	/// List for all arguments (in the cone of influence of the query argument) and their distance to the argument of the query.
	/// </summary>
	vector<uint32_t> distance_to_query;
	/// <summary>
	/// The maximum distance of an argument within the cone of influence of the query argument, to the query argument
	/// </summary>
	uint32_t max_distance;


	ConeOfInfluence(AF &framework);
};

/// <summary>
/// This class is responsible for representing the data of pair consisting of an argument, and its distance to a query argument in a framework.
/// </summary>
class ArgumentDistancePair {
public:
	bool operator == (ArgumentDistancePair const &other)
	{
		return (other.argument == argument);
	};

	uint32_t argument;
	uint32_t distance_to_query;

	ArgumentDistancePair(uint32_t argument, ConeOfInfluence coi);

	static bool compare_by_distance(ArgumentDistancePair firstElem, ArgumentDistancePair secondElem);
};

inline bool operator == (ArgumentDistancePair const &lhs, ArgumentDistancePair const &rhs)
{
	return (lhs.argument == rhs.argument);
};
#endif