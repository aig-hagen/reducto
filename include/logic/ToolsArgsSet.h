#ifndef TOOLS_ARGS_SET_H
#define TOOLS_ARGS_SET_H

#include <vector>
#include <cstdint>
#include <list>

#include "AF.h"
#include "../util/ArrayBitSet.h"

extern "C" {
	#include "../util/MemoryWatchDog.h"
}

namespace tools {
	/// <summary>
	/// This class is responsible for operations facilitating the work with sets of arguments in abstract argumentation.
	/// </summary>
	class Tools_ArgsSet {
	public:
		/// <summary>
		/// Checks if a specified argument gets attacked by any member of the specified set of arguments.
		/// </summary>
		/// <param name="argument">The argument, which could be attacked by the set or not.</param>
		/// <param name="set_arguments">Set of arguments of the framework.</param>
		/// <param name="framework">The abstract argumentation framework, specifying the underlying attack relations between the arguments.</param>
		/// <returns>TRUE iff the any member of the set attacks the specified argument. FALSE otherwise.</returns>
		static bool check_attack(std::uint32_t argument, std::list<std::uint32_t> &set_arguments, AF &framework);

	};
};
#endif
