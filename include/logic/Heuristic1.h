#ifndef HEURISTIC1_H
#define HEURISTIC1_H

#include <iostream>
#include <cstdint>
#include <list>

#include "AF.h"
#include "IPrioritizer.h"

using namespace std;

/// <summary>
/// This class is responsible for prioritizing tasks during the process of the solver.
/// </summary>
class Heuristic1 : IPrioritizer 
{
public:
	/// <summary>
	/// This method calculates the priority of a specified set of arguments, in the specified abstract argumentation framework.
	/// </summary>
	/// <param name="framework">The original abstract argumentation framework.</param>
	/// <param name="set_args">The set of arguments, for which a priority is to be calculated.</param>
	/// <returns>Returns a positive integer (not 0), indicating the priority of the specified set. 
	/// The higher the integer, the higher the calculated priority.</returns>
	virtual uint32_t calculate_priority(AF &framework, list<uint32_t> &set_args);
};
#endif