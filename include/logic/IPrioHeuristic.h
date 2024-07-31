#ifndef IPRIORITIZER_H
#define IPRIORITIZER_H

#include <iostream>
#include <cstdint>
#include <list>

#include "AF.h"

using namespace std;

/// <summary>
/// This interface expects a method to prioritize calculated extensions during the process of the solver.
/// </summary>
class IPrioHeuristic
{
public:
    virtual ~IPrioHeuristic() {}
	/// <summary>
	/// This method calculates the priority of a specified extension and a specified initial set, used to extend the extension in this step.
	/// </summary>
	/// <param name="framework">The original abstract argumentation framework.</param>
	/// <param name="extension">The set of arguments, which define the reduct state of the framework.</param>
	/// <param name="initialSet">The set of arguments, calculated in the former state to extend the extension.</param>
	/// <param name="query">The argument to check for sceptical acceptance.</param>
	/// <returns>Returns a positive integer (not 0), indicating the priority of the specified set. 
	/// The higher the integer, the higher the calculated priority.</returns>
	virtual uint32_t calculate_priority(AF &framework, list<uint32_t> &extension, list<uint32_t> &initialSet, uint32_t query) = 0;
};

#endif