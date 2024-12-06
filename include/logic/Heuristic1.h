#ifndef HEURISTIC1_H
#define HEURISTIC1_H

#include <iostream>
#include <cstdint>
#include <list>

#include "AF.h"
#include "IPrioHeuristic.h"

using namespace std;

/// <summary>
/// This class is responsible for prioritizing extensions, based on the minimal distance of one of its members to the query argument.
/// </summary>
class Heuristic1 : public IPrioHeuristic 
{
public:
	/// <summary>
	/// This method calculates the priority of a specified extension, based on the minimal distance of one of its members to the query argument.
	/// </summary>
	/// <param name="framework">The original abstract argumentation framework.</param>
	/// <param name="extension">The set of arguments, which define the reduct state of the framework.</param>
	/// <param name="initialSet">The set of arguments, calculated in the former state to extend the extension.</param>
	/// <param name="query">The argument to check for sceptical acceptance.</param>
	/// <param name="coi">The cone of influence over the query in the specified framework.</param>
	/// <returns>Returns a positive integer (not 0), indicating the priority of the specified set. 
	/// The higher the integer, the higher the calculated priority.</returns>
	virtual uint32_t calculate_priority(AF &framework, list<uint32_t> &set_args, list<uint32_t> &initialSet, uint32_t query, ConeOfInfluence &coi);
};
#endif