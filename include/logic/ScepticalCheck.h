#ifndef SCEPTICAL_PR_CHECK_H
#define SCEPTICAL_PR_CHECK_H

#include <vector>
#include <cstdint>
#include <list>

#include "AF.h"

extern "C" {
	#include "../util/MemoryWatchDog.h"
}

using namespace std;

/// <summary>
/// This class is responsible for the logic to check if there is proof, that the argument of a query is sceptically accepted or rejected.
/// </summary>
class ScepticalCheck {
	public:
		/// <summary>
		/// Checks if a query about the sceptical acceptance of a specified argument can be rejected using the specified set of arguments. For example, 
		/// this is the case if any argument of the initial set attacks the argument in question.
		/// <br> - <\br>
		/// Precondition: Before calling this method it has to be checked, that the query argument is not contained in the specified set of arguments.
		/// </summary>
		/// <param name="query">The argument, which could be sceptical accepted or not.</param>
		/// <param name="set_arguments">Set of arguments of the framework.</param>
		/// <param name="framework">The abstract argumentation framework, specifying the underlying attack relations between the arguments.</param>
		/// <returns>TRUE iff the sceptical acceptance is rejected by this initial set. FALSE otherwise.</returns>
		static bool check_rejection(uint32_t query, list<uint32_t> &set_arguments, AF &framework);

		/// <summary>
		/// Checks if the generation of an extension, using the specified set of arguments, can be terminated, since the set contains the argument 
		/// of the query, and can therefore never lead to an extension, with an argument attacking the specified argument of the query, which would 
		/// lead into rejecting the sceptical acceptance.
		/// </summary>
		/// <param name="query">The argument, which could be sceptical accepted or not.</param>
		/// <param name="set_arguments">Set or arguments which is used to generate an extension.</param>
		/// <returns>TRUE iff the generation of an extension can be stopped. FALSE otherwise.</returns>
		static bool check_terminate_extension_build(uint32_t query, list<uint32_t> &set_arguments);
};
#endif
